# HumbleOS file: create_config_h.py
# Purpose: Generate the config.h file based off config.json

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

        config_h += "\n\n"

config_h += """// End config

#endif // CONFIG_H"""

with open(OUTPUT, mode="w") as output:
    output.write(config_h)
