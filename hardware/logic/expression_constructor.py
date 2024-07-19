import csv


def process_single_output(output_column_header: str, dataset) -> list[str]:
    output_expression = []
    for row in range(len(dataset[output_column_header])):
        if int(dataset[output_column_header][row]):
            row_expression = locate_true_row_inputs(row, dataset)
            if row_expression:
                output_expression.append(row_expression)
    
    return output_expression
            
def locate_true_row_inputs(row: int, dataset: list[bool]) -> str:
    expression = ""
    for header, column in dataset.items():
        print(header)
        print(column[row])
        if "O" not in header and "N" not in header and int(column[row]):
            expression += header
    return expression

def process_truth_table(dataset: dict[str, list])-> dict[list[str]]:
    output_expression_dict = dict()

    for header in dataset.keys():
        if "O" in header:
            output_expression_dict[header] = process_single_output(header, dataset)
    
    return output_expression_dict

def load_truth_csv(filepath: str) -> dict[str, list]:
    with open(filepath) as file:
        csv_reader = csv.reader(file, delimiter=',')
        line_count = 0

        for row in csv_reader:
            if line_count == 0:
                headers = row
                table = dict(zip(headers, [[] for x in range(len(headers))]))
            else:
                for index, item in enumerate(table.items()):
                    header, column = item
                    column.append(row[index])
            line_count += 1
    
    return table


def parse_output(generated_output: str):
    parsed_output = ""
    for header, result in generated_output.items():
        parsed_output += f'{header}: {" + ".join(result)}\n'

    return parsed_output


if __name__ == "__main__":
    print(parse_output(process_truth_table(load_truth_csv(r"/home/malinkyzubr/Desktop/ARISS-Main/interrupt_expander/TRUTH_TABLE.csv"))))
    #print(process_truth_table(load_truth_csv(r"/home/malinkyzubr/Desktop/ARISS-Main/interrupt_expander/TRUTH_TABLE.csv")))