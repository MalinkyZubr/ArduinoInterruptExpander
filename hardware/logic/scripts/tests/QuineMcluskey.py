import sys
sys.path.append("/home/malinkyzubr/Desktop/InterruptExpander/hardware/logic")

import pytest

from scripts.QuineMcluskey import *


def test_extract_single_out_cols():
    row = {"a":1, "b":2, "c":3, "O1":4, "O5":3}
    assert extract_single_out_cols(row, "O1") == {"a":1, "b":2, "c":3, "O1":4}

def test_extract_single_output_rows():
    rows = [{"a":1, "b":2, "c":3, "O1":4, "O5":3} for x in range(3)]
    assert extract_single_output_rows(rows, "O1") == [{"a":1, "b":2, "c":3, "O1":4} for x in range(3)]

def test_separate_outputs():
    rows = [{"a":1, "b":2, "c":3, "O1":4, "O5":3} for x in range(3)]

    first_set = [{"a":1, "b":2, "c":3, "O1":4} for x in range(3)]
    second_set = [{"a":1, "b":2, "c":3, "O5":3} for x in range(3)]

    assert separate_outputs(rows) == {"O1":first_set, "O5":second_set}

def test_identify_minterms():
    pass