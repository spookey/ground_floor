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
    for main_name in listdir(source):
        base = path.join(source, main_name)
        if not path.isdir(base):
            print('SKIPPING:', base, '-', 'not a folder')
        else:
            target_file = path.join(target, f'{main_name}.rst')
            if path.exists(target_file):
                print('SKIPPING:', target_file, '-', 'already exists')
            else:
                yield (
                    main_name.replace('_', ' '),
                    base,
                    target_file
                )


def collect_inner(base, target):
    for source in (path.join(base, 'src'), path.join(base, 'test')):
        for ext in ('.hpp', '.cpp'):
            for elem in sorted(listdir(source)):
                if elem.lower().endswith(ext):
                    yield (
                        elem,
                        path.join(path.relpath(source, target), elem)
                    )


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

    for main_name, base, target_file in collect_base(source, target):
        bucket = [heading('=', main_name)]
        bucket.extend(('.. todo:: Write something here!', '\n'))
        for src_name, doclink in collect_inner(base, target):
            bucket.append(heading('-', src_name))
            bucket.extend((f'''
.. literalinclude:: {doclink}
    :language: cpp
    :linenos:
            '''.strip(), '\n'))

        with open(target_file, 'w') as rst:
            rst.write('\n'.join(bucket))

    return True


if __name__ == '__main__':
    _exit(0 if main() else 1)
