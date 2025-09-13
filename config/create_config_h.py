#
# create_config_h.py - create config.h from config.json
#
# Part of HumbleOS
#
# Copyright 2025 Thomas Shrader
#
# Permission is hereby granted, free of charge, to any person obtaining a copy of this software
# and associated documentation files (the "Software"), to deal in the Software without restriction,
# including without limitation the rights to use, copy, modify, merge, publish, distribute,
# sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all copies or substantial
# portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
# NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
# DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
#

import json
import os
import sys

CONFIG = "config.json"
SCHEMA = "schema.json"
OUTPUT = "../src/config.h"

os.chdir(os.path.dirname(os.path.abspath(sys.argv[0])))

config_h = """// HumbleOS file: config.h
// Purpose: Generated config from create_config_h.py. DO NOT EDIT!

#ifndef CONFIG_H
#define CONFIG_H

// Start config

"""

with open(SCHEMA, mode="r") as schema_fp, open(CONFIG, mode="r") as config_fp:
    schema = json.load(schema_fp)
    config = json.load(config_fp)

    for option in schema:
        value = config[option["define"]]
        option_type = option["type"]

        config_h += "/* " + option["define"] + " - " + option["text"] + " (" + option_type + ") */\n"
        config_h += "#define " + option["define"] + " "

        if option_type == "boolean":
            config_h += "1" if value else "0"
        elif option_type == "string":
            config_h += "\"" + value + "\""
            
            if option["doEmptyFlag"]:
                config_h += "\n#define " + option["define"] + "_IS_EMPTY " + ("1" if len(value) == 0 else "0")

        config_h += "\n\n"

config_h += """// End config

#endif // CONFIG_H"""

with open(OUTPUT, mode="w") as output:
    output.write(config_h)
