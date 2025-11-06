#!/usr/bin/env python3
"""
Generate opcode profile from LLVM profile data.

This script takes a LLVM profile file and extracts function call statistics
for files containing "execFn" in their name, sorted by function count.
"""

import argparse
import json
import subprocess
import sys
from typing import List, Tuple

FUNCTION_SIGNATURE = "execFn"

class ProgressSpinner:
    """A simple progress spinner that writes to stderr."""
    
    def __init__(self, message: str = "Processing"):
        self.message = message
        self.spinner_chars = ['⠋', '⠙', '⠹', '⠸', '⠼', '⠴', '⠦', '⠧', '⠇', '⠏']
        self.index = 0
    
    def advance(self):
        """Advance the spinner by one step."""
        char = self.spinner_chars[self.index % len(self.spinner_chars)]
        print(f'\r{char} {self.message}...', end='', file=sys.stderr)
        sys.stderr.flush()
        self.index += 1
    
    def clear(self):
        """Clear the spinner line."""
        print('\r' + ' ' * (len(self.message) + 10), end='\r', file=sys.stderr)
        sys.stderr.flush()


def demangle_function_name(mangled_name: str) -> str:
    """
    Demangle a C++ function name using c++filt.
    
    Args:
        mangled_name: The mangled function name
        
    Returns:
        The demangled function name, or the original name if demangling fails
    """
    try:
        # Prepend underscore as required for c++filt
        name_with_underscore = f"_{mangled_name}"
        
        # Run c++filt to demangle the name
        result = subprocess.run(
            ['c++filt', name_with_underscore],
            capture_output=True,
            text=True,
            check=True
        )
        
        demangled = result.stdout.strip()
        
        # If c++filt couldn't demangle it, it returns the input unchanged
        # In that case, return the original name without the underscore
        if demangled == name_with_underscore:
            return mangled_name
        else:
            return demangled
            
    except (subprocess.CalledProcessError, FileNotFoundError):
        # If c++filt is not available or fails, return the original name
        return mangled_name


def parse_profile_output(output: str) -> List[Tuple[str, int]]:
    """
    Parse llvm-profdata output and extract function information.
    
    Returns list of (filename, function_count) tuples for files containing the signature.
    """
    results = []
    lines = output.strip().split('\n')
    
    i = 0
    while i < len(lines):
        line = lines[i].strip()
        
        # Skip empty lines
        if not line:
            i += 1
            continue
            
        # Look for function definition lines (end with colon)
        if line.endswith(':'):
            function_name = line[:-1]  # Remove the colon
            
            # Extract filename and function name
            # Format can be "filename:function" or just "function"
            if ':' in function_name:
                filename = function_name.split(':')[0]
                func_name = function_name.split(':', 1)[1]
            else:
                # If no filename prefix, treat the whole thing as function name
                filename = ""
                func_name = function_name
            
            # Only process functions containing the signature
            if FUNCTION_SIGNATURE not in func_name:
                i += 1
                continue
            
            # Look for the "Function count:" line in the following lines
            function_count = None
            j = i + 1
            while j < len(lines) and j < i + 10:  # Look ahead max 10 lines
                next_line = lines[j].strip()
                if next_line.startswith('Function count:'):
                    try:
                        function_count = int(next_line.split(':')[1].strip())
                        break
                    except (IndexError, ValueError):
                        pass
                elif next_line.endswith(':'):
                    # Hit another function definition, stop looking
                    break
                j += 1
            
            if function_count is not None:
                # Use only the function name, without filename prefix
                results.append((func_name, function_count))
        
        i += 1
    
    return results


def run_llvm_profdata(profile_file: str) -> str:
    """
    Run llvm-profdata show command and return the output.
    """
    try:
        cmd = ['llvm-profdata', 'show', profile_file, '--all-functions']
        result = subprocess.run(cmd, capture_output=True, text=True, check=True)
        return result.stdout
    except subprocess.CalledProcessError as e:
        print(f"Error running llvm-profdata: {e}", file=sys.stderr)
        print(f"Command output: {e.stderr}", file=sys.stderr)
        sys.exit(1)
    except FileNotFoundError:
        print("Error: llvm-profdata command not found. Make sure LLVM tools are installed and in PATH.", file=sys.stderr)
        sys.exit(1)


def main():
    """Main function."""
    parser = argparse.ArgumentParser(
        description="Generate opcode profile from LLVM profile data",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
This script processes LLVM profile data to extract function call statistics
for functions containing "execFn" in their name. The results are sorted by
function count in descending order and show the demangled function name in
quotes followed by the function count.

Examples:
    python generate_opcode_profile.py profile.profdata
    python generate_opcode_profile.py --json profile.profdata
        """
    )
    
    parser.add_argument(
        'profile_file',
        help='LLVM profile file to process'
    )
    
    parser.add_argument(
        '--json',
        action='store_true',
        help='Output results in JSON format instead of plain text'
    )
    
    args = parser.parse_args()
    
    # Run llvm-profdata and get output
    output = run_llvm_profdata(args.profile_file)
    
    # Parse the output
    results = parse_profile_output(output)
    
    # Sort by function count in descending order
    results.sort(key=lambda x: x[1], reverse=True)
    
    # Output results
    if args.json:
        # JSON output format
        json_results = []
        if results:
            spinner = ProgressSpinner("Demangling function names")
            for func_name, count in results:
                spinner.advance()
                demangled_name = demangle_function_name(func_name)
                json_results.append({
                    "name": demangled_name,
                    "calls": count
                })
            spinner.clear()
        print(json.dumps(json_results, indent=2))
    else:
        # Plain text output format with demangled function names in quotes
        if results:
            spinner = ProgressSpinner("Demangling function names")
            demangled_results = []
            for func_name, count in results:
                spinner.advance()
                demangled_name = demangle_function_name(func_name)
                demangled_results.append((demangled_name, count))
            spinner.clear()
            
            for demangled_name, count in demangled_results:
                print(f'"{demangled_name}" {count}')


if __name__ == '__main__':
    main()