import csv
import argparse
import enum
import abc
import multiprocessing
from typing import Iterable, Callable
from pprint import pprint


class Minterm:
    pass

class Minterm:
    def __init__(self, expression: dict[str, str], comprising: tuple[Minterm, Minterm] | None, id: str = ""):
        self.expression: dict[str, str] = expression
        self.matched: bool = False
        self.layer: int = 0
        self.comprising: tuple[Minterm, Minterm] = comprising
        self.id: str = id

    def get_num_terms(self) -> int:
        return len(self.expression)

    def count_true(self) -> int:
        count = 0
        
        for value in self.expression.values():
            if(value == "1"):
                count += 1

        return count
    
    def __diff(self, minterm: Minterm) -> list[str]:
        foreign_expression: Iterable = minterm.expression.items()
        non_matching_columns: list[str] = []

        for column, value in foreign_expression: # change to while
            if value != self.expression[column]:
                non_matching_columns.append(column)
            
        return non_matching_columns

    def will_match(self, minterm: Minterm) -> bool: # optimize later
        return len(self.__diff(minterm)) == 1

    def match_next_minterm(self, minterm: Minterm) -> Minterm:
        difference: list[str] = self.__diff(minterm)
        new_expression = {column:(value if column not in difference else "x") for column, value in self.expression.items()}

        new_minterm: Minterm = Minterm(new_expression, (self, minterm))

        return new_minterm

    def set_matched(self):
        self.matched = True

    def is_matched(self) -> bool:
        return self.matched
    
    def get_root_comprising(self) -> list[Minterm]:
        root_comprising: list[Minterm] = []

        if not self.comprising:
            root_comprising = [self]
        else:
            root_comprising = self.comprising[0].get_root_comprising() + self.comprising[1].get_root_comprising()

        return root_comprising
    
    def __str__(self):
        stringified: str = ""

        for column, value in self.expression.items():
            if value == "1":
                stringified += column
            elif value == "0":
                stringified += "!" + column

        return stringified
    

class RootMinterm:
    def __init__(self, minterm: Minterm):
        self.minterm: Minterm = minterm
        self.is_alone = False
    
    def set_alone(self):
        self.is_alone = True

    def check_alone(self) -> bool:
        return self.is_alone
    

def extract_single_out_cols(row: dict[str, str], output_key: str) -> dict[str, str]:
    return {key:value for key, value in row.items() if key == output_key or "O" not in key.upper()}

def extract_single_output_rows(rows: list[dict[str, str]], output_key: str) -> list[dict, str]:
    return [extract_single_out_cols(row, output_key) for row in rows]

def separate_outputs(rows: list[dict[str, str]]) -> dict[str, dict[str, str]]:
    headers: list[str] = list(rows[0].keys())
    output_keys: list[str] = list(filter(lambda x: "O" in str(x), headers))

    output_datasets: dict = {key:extract_single_output_rows(rows, key) for key in output_keys}

    return output_datasets

def identify_minterms(output_group: list[dict[str, str]], output_column_header: str) -> list[Minterm]:
    minterms: list[dict[str, str]] = list(filter(lambda x: x[output_column_header] == "1", output_group))

    minterm_object_list: list[Minterm] = []
    for index, row in enumerate(minterms):
        row.pop(output_column_header)
        minterm_object_list.append(Minterm(row, None, str(index)))

    return minterms

def generate_groups(minterms: list[Minterm]) -> dict[int, list[Minterm]]:
    groups: dict[int, list[Minterm]] = dict()

    for minterm in minterms:
        num_true: int = minterm.count_true()
        if num_true not in groups:
            groups[num_true] = []
        groups[num_true].append(minterm)

    return groups

def generate_matched_pairs_single(selected_minterm: Minterm, next_group: list[Minterm]) -> list[Minterm]:
    new_minterms: list[Minterm] = []

    for minterm in next_group:
        if selected_minterm.will_match(minterm):
            new_minterms.append(selected_minterm.match_next_minterm(minterm))
    
    return new_minterms

def generate_matched_pairs_2_group(group_1: list[Minterm], group_2: list[Minterm]) -> tuple[list[Minterm], list[Minterm]]:
    new_minterms: list[Minterm] = []
    prime_implicants: list[Minterm] = []

    for minterm in group_1:
        new_minterms += generate_matched_pairs_single(minterm, group_2)

        if not minterm.is_matched():
            prime_implicants.append(minterm)
    
    return new_minterms, prime_implicants

def generate_matched_pairs_group(groups: dict[int, list[Minterm]]) -> tuple[list[Minterm], list[Minterm]]:
    minterm_pairs: list[Minterm] = []
    prime_implicants: list[Minterm] = []

    for group_num, group in groups.items():
        if group_num + 1 in groups:
            generated_minterms, generated_prime_implicants = generate_matched_pairs_2_group(group, groups[group_num + 1])
            minterm_pairs += generated_minterms
            prime_implicants += generated_prime_implicants
    
    return minterm_pairs, prime_implicants

def recurse_all_prime_implicants(minterms: list[Minterm]) -> list[Minterm]:
    groups: dict[int, list[Minterm]] = generate_groups(minterms)
    minterm_pairs, prime_implicants = generate_matched_pairs_group(groups)

    if len(prime_implicants) < len(minterm_pairs):
        prime_implicants += recurse_all_prime_implicants(minterm_pairs)

    return prime_implicants

def generate_prime_implicant_table(prime_implicants: list[Minterm]) -> dict[Minterm, list[Minterm]]:
    origin_minterms_table: dict[Minterm, list[Minterm]]

    for prime_implicant in prime_implicants:
        origin_minterms_table[prime_implicant] = prime_implicant.get_root_comprising()

    return origin_minterms_table

def identify_essential_prime_implicants(prime_implicants: list[Minterm], all_origin_minterms: list[Minterm]) -> set[Minterm]:
    origin_minterms_table = generate_prime_implicant_table(prime_implicants)
    essential_prime_implicants: set[Minterm] = set()

    for prime_implicant, associated_minterms in origin_minterms_table:
        for minterm in associated_minterms:
            if all_origin_minterms.count(minterm) == 1:
                essential_prime_implicants.add(prime_implicant)
    
    return essential_prime_implicants

def format_expression(essential_prime_implicants: list[Minterm], output_column_header: str) -> str:
    return output_column_header + ": " + " + ".join([str(epi) for epi in essential_prime_implicants])

def quine_mckluskey_minimize(boolean_group: list[dict[str, str]], output_column_header: str) -> str:
    all_origin_minterms: list[Minterm] = identify_minterms(boolean_group, output_column_header)
    prime_implicants: list[Minterm] = recurse_all_prime_implicants(all_origin_minterms)
    essential_prime_implicants: list[Minterm] = identify_essential_prime_implicants(prime_implicants, all_origin_minterms)

    return format_expression(essential_prime_implicants)

def quine_mckluskey_mppool(output_group: tuple[list[dict[str, str]], str]):
    return quine_mckluskey_minimize(output_group[1], output_group[0])

def minimize_dataset(dataset_loaded: list[dict[str, str]]) -> list[str]:
    separated_output_tables: dict[str, dict[str, str]] = separate_outputs(dataset_loaded)
    
    with multiprocessing.Pool(len(separated_output_tables)) as p:
        fully_processed = p.map(quine_mckluskey_mppool, list(separated_output_tables.items()))

    return list(fully_processed)

def load_csv(csv_path: str) -> list[dict[str, str]]:
    with open(csv_path) as file:
        csv_reader = csv.reader(file, delimiter=',')
        line_count = 0
        rows: list[str] = []

        for row in csv_reader:
            if line_count == 0:
                headers = row
            else:
                row: dict[str, str] = dict(zip(headers, row))
                rows.append(row)
            line_count += 1
    
    return rows


if __name__ == "__main__":
    pprint(minimize_dataset(load_csv(r"/home/malinkyzubr/Desktop/InterruptExpander/hardware/logic/scripts/TRUTH_TABLE2.csv")))
