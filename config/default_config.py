# HumbleOS file: default_config.py
# Description: Loads defaults from schema.json

import json
import sys
import os

SCHEMA = "schema.json"
CONFIG = "config.json"

os.chdir(os.path.dirname(os.path.abspath(sys.argv[0])))

with open(SCHEMA, mode="r") as schema_fp:
    data = json.load(schema_fp)

    out = {}

    for option in data:
        out[option["define"]] = option["default"]
    
    with open(CONFIG, mode="w") as config_fp:
        json.dump(out, config_fp)