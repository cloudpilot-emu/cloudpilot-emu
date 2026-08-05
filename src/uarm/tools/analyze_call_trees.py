#!/usr/bin/env python3
"""Analyze a .wat (WebAssembly text format) file and print call trees.

For each function defined in the module, prints the function's size (in .wat
source lines) and the list of call sites -- i.e. the functions that contain
a `call $foo` referencing it. Each referenced function name is annotated
with that function's own size in lines.
"""

import argparse
import re
import sys
from collections import defaultdict

# Matches the start of a function definition, e.g. "  (func $foo (type 3) ...".
# Imported funcs use "(import ... (func $x (type N)))" -- those live on a
# single line so the paren-depth logic below closes them out immediately and
# they end up with size 1. We tag them so they can be filtered from the size
# column if desired.
FUNC_START_RE = re.compile(r'^(\s*)\(func\s+(\$[^\s()]+)')
IMPORT_LINE_RE = re.compile(r'^\s*\(import\b')

# `call $foo`. `\b` around `call` prevents matching `call_indirect` because
# the `_` following `call` is a word char (so there is no word boundary).
CALL_RE = re.compile(r'\bcall\s+(\$[^\s()]+)')


def analyze(path):
    functions = {}          # name -> line count
    imported = set()        # names that came from (import ...)
    # callee -> {caller: count}
    call_edges = defaultdict(lambda: defaultdict(int))

    current_func = None
    func_start_line = 0
    paren_depth = 0

    with open(path, 'r', encoding='utf-8', errors='replace') as f:
        for lineno, line in enumerate(f, 1):
            opens = line.count('(')
            closes = line.count(')')

            if current_func is None:
                m = FUNC_START_RE.match(line)
                if m:
                    name = m.group(2)
                    is_import = IMPORT_LINE_RE.match(line) is not None
                    delta = opens - closes
                    if delta <= 0:
                        # Single-line func decl (typically an import).
                        functions[name] = 1
                        if is_import:
                            imported.add(name)
                    else:
                        current_func = name
                        func_start_line = lineno
                        paren_depth = delta
                        if is_import:
                            imported.add(name)
            else:
                for cm in CALL_RE.finditer(line):
                    callee = cm.group(1)
                    call_edges[callee][current_func] += 1

                paren_depth += opens - closes
                if paren_depth <= 0:
                    functions[current_func] = lineno - func_start_line + 1
                    current_func = None
                    paren_depth = 0

    if current_func is not None:
        # File ended mid-function -- record what we have.
        functions[current_func] = lineno - func_start_line + 1

    return functions, imported, call_edges


def emit_callers(name, depth, max_depth, on_path, functions, call_edges, out):
    """Recursively emit callers of `name`, up to `max_depth` frames deep.

    A frame == one `<-` arrow. `on_path` tracks the callee->...->name chain
    so we don't recurse forever through cycles (recursion, mutual recursion).
    """
    if depth > max_depth:
        return

    callers = call_edges.get(name, {})
    if not callers:
        if depth == 1:
            out.append('  (no call sites found)')
        return

    indent = '  ' * depth
    for caller, count in sorted(
        callers.items(), key=lambda kv: (-kv[1], kv[0])
    ):
        caller_size = functions.get(caller, 0)
        times = f' x{count}' if count > 1 else ''
        if caller in on_path:
            out.append(
                f'{indent}<- {caller} ({caller_size} lines){times} '
                f'[cycle]'
            )
            continue
        out.append(f'{indent}<- {caller} ({caller_size} lines){times}')
        on_path.add(caller)
        emit_callers(
            caller, depth + 1, max_depth, on_path,
            functions, call_edges, out,
        )
        on_path.remove(caller)


def format_report(functions, imported, call_edges, sort_by, max_frames):
    if sort_by == 'name':
        ordered = sorted(functions.items(), key=lambda kv: kv[0])
    elif sort_by == 'size':
        ordered = sorted(functions.items(), key=lambda kv: (-kv[1], kv[0]))
    else:  # 'callers'
        ordered = sorted(
            functions.items(),
            key=lambda kv: (-len(call_edges.get(kv[0], {})), kv[0]),
        )

    lines = []
    for name, size in ordered:
        tag = ' [import]' if name in imported else ''
        lines.append(f'{name} ({size} lines){tag}')
        emit_callers(
            name, 1, max_frames, {name},
            functions, call_edges, lines,
        )
        lines.append('')

    return '\n'.join(lines)


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('wat', help='Path to the .wat file')
    ap.add_argument(
        '--sort',
        choices=('name', 'size', 'callers'),
        default='name',
        help='Sort functions by name (default), size (desc), '
             'or number of distinct callers (desc)',
    )
    ap.add_argument(
        '--frames', '-n',
        type=int,
        default=3,
        help='Max call-tree depth to print per function (default: 3). '
             '1 == direct callers only.',
    )
    args = ap.parse_args()

    if args.frames < 1:
        ap.error('--frames must be >= 1')

    functions, imported, call_edges = analyze(args.wat)

    total_lines = sum(functions.values())
    print(f'# {len(functions)} functions, {total_lines} function lines total',
          file=sys.stderr)

    # Warn about calls whose targets we never saw as function definitions.
    unknown = [c for c in call_edges if c not in functions]
    if unknown:
        print(f'# note: {len(unknown)} call targets have no matching '
              f'(func $...) definition (may be aliased or renamed)',
              file=sys.stderr)

    sys.stdout.write(format_report(
        functions, imported, call_edges, args.sort, args.frames,
    ))
    sys.stdout.write('\n')


if __name__ == '__main__':
    main()
