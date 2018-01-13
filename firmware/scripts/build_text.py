#!/usr/bin/python

#
# Convert a YAML file into a C header and source file defining progmem strings
#

from __future__ import print_function

import argparse
import os
import yaml

parser = argparse.ArgumentParser(
    description="Generate progmem string header and source files from YAML"
)

parser.add_argument("yaml_file", help="Path to a YAML file to read string definitions from")

def get_pstring_code(varName, value=None):
    """ Generate C code to define a progmem variable """

    # If no value is passed, generate forward declaration
    if value is None:
        return 'extern const __flash uint8_t %s[];\n' % varName

    # Turn the value into a C string if it's not defined as code
    if not value.startswith('"'):
        value.replace('"', '\\"')
        value = '"%s"' % value

    return 'const __flash uint8_t %s[] = %s;\n' % (varName, value)

def get_metadata_code(varName, value):
    """ Generate C code to define metadata for a progmem string """

    return 'static const uint8_t len_%s = %d;\n' % (varName, len(value))


comment_template = """
/**
 * This file is generated from {yaml_file}. Do not edit it directly.
 */
"""

if __name__ == "__main__":
    args = parser.parse_args()

    # Read YAML file
    with open(args.yaml_file) as stream:
        data = yaml.load(stream)

    try:
        # Find and open target files
        basename = os.path.splitext(args.yaml_file)[0]
        headerPath = basename + ".h"
        sourcePath = basename + ".c"

        header = open(headerPath, "w")
        implementation = open(sourcePath, "w")

        # Write header and implementation code and comments
        header.write("#pragma once\n\n")
        header.write("#include <stdint.h>\n")
        header.write(comment_template.format(yaml_file=args.yaml_file))

        implementation.write('#include "%s"\n' % headerPath)
        implementation.write(comment_template.format(yaml_file=args.yaml_file))

        # Add progmem strings as defined in the YAML
        for var, value in data.iteritems():
            header.write(get_pstring_code(var))
            implementation.write(get_pstring_code(var, value))

        # Add string length information
        for var, value in data.iteritems():
            header.write(get_metadata_code(var, value))

    finally:
        if header:
            header.close()

        if implementation:
            implementation.close()
