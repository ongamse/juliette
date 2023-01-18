import os
import re
from typing import List

from engine import tune_dir


def is_array_decl(tok: str) -> bool:
    lb: int = tok.count('[')
    rb: int = tok.count(']')
    return lb > 0 and lb == rb


def open_src(path: str) -> str:
    os.chdir(tune_dir)
    file = open(path, 'r')
    template = file.read()
    file.close()
    return template


def write_to(path: str, content: str) -> None:
    os.chdir(tune_dir)
    f = open(path, 'w')
    f.write(content)


def evaluate(expr: str) -> int:
    split: int
    try:
        split = expr.index('+')
    except ValueError:
        split = -1

    if split != -1:
        l_op = expr[0:split:1]
        l_op = l_op.strip()
        r_op = expr[split + 1::1]
        r_op = r_op.strip()
        return evaluate(l_op) + evaluate(r_op)
    else:
        expr = expr.strip()
        return int(expr)


def generate_psqt_template(path: str) -> str:
    template = open_src(path)
    tokens: List[str] = re.split(' |,', template)
    i: int = 1
    for tok in tokens:
        print(str(i) + ". " + tok)
        i += 1

    within_array: bool = False
    template_str: str = ""
    i: int = 0
    for tok in tokens:
        if within_array:
            if tok.replace(' ', '').replace('-', '').replace('+', '').isnumeric():
                tok = str(evaluate(tok))
                tok += '+%d'
                i += 1
                if i != 64:
                    tok += ', '
                if i % 8 == 0:
                    tok += '\n'
            elif '}' in tok:
                within_array = False
                i = 0
                tok = '\t' + tok
            else:
                tok += ' '
            template_str += tok
        elif is_array_decl(tok):
            within_array = True
            template_str += tok + ' '
        else:
            template_str += tok + ' '
    return template_str


def fill_displacements(fmt: str, delim_symb: str, vector: List[int]):
    i: int = 0
    while i < len(vector) and delim_symb in fmt:
        fmt = fmt.replace(delim_symb, str(vector[i]), 1)
        i += 1
    return fmt


if __name__ == '__main__':
    # template = generate_psqt_template('data\\weights0.h')
    # template = fill_displacements(template, '%d', [1, 2, 3])
    # write_to('psqt_template.txt', template)

# IDE Likes New Line at EOF
