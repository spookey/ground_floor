#!/usr/bin/env python3

from argparse import ArgumentParser
from os import listdir, path
from sys import exit as _exit


def arguments():
    parser = ArgumentParser(__name__)
    parser.add_argument('source')
    parser.add_argument('target')
    return parser.parse_args()


def collect_base(source, target):
    for name in listdir(source):
        base = path.join(source, name)
        if not path.isdir(base):
            print('SKIPPING:', base, '-', 'not a folder')
        else:
            file = path.join(target, f'{name}.rst')
            if path.exists(file):
                print('SKIPPING:', file, '-', 'already exists')
            else:
                yield name.replace('_', ' '), base, file


def collect_inner(base, target):
    src = path.join(base, 'src')
    for elem in reversed(sorted(listdir(src))):
        if any(elem.lower().endswith(ext) for ext in ('.cpp', '.hpp')):
            yield (elem, path.join(path.relpath(src, target), elem))


def heading(char, text):
    line = char * len(text)
    return '\n'.join((line, text, line, ''))


def main():
    args = arguments()
    source = path.realpath(args.source)
    target = path.realpath(args.target)
    for elem in (source, target):
        if not path.isdir(elem):
            print('ERROR: source and target must be a folder')
            return False

    for name, base, file in collect_base(source, target):
        bucket = [heading('=', name)]
        bucket.extend(('.. todo', '\n'))
        for f_name, incl in collect_inner(base, target):
            bucket.append(heading('-', f_name))
            bucket.extend((f'''
.. literalinclude:: {incl}
    :language: cpp
    :linenos:
            '''.strip(), '\n'))

        with open(file, 'w') as rst:
            rst.write('\n'.join(bucket))

    return True


if __name__ == '__main__':
    _exit(0 if main() else 1)
