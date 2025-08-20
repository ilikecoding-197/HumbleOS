# HumbleOS file: config_editor.py
# Purpose: Edit the current config

import npyscreen
import json
import os
import sys

os.chdir(os.path.dirname(os.path.abspath(sys.argv[0])))

CONFIG = "config.json"
SCHEMA = "schema.json"

config = {}
schema = []

class MyForm(npyscreen.Form):
    def create(self):
        self.widgets = []  # (define, type, widget)

        for option in schema:
            opt_type = option["type"]
            define   = option["define"]

            if opt_type == "boolean":
                w = self.add(
                    npyscreen.Checkbox,
                    name=f"{option['text']} [{define}]",
                    value=config.get(define, False)
                )
                self.widgets.append((define, opt_type, w))

    def push_values(self):
        for define, opt_type, widget in self.widgets:
            if opt_type == "boolean":
                config[define] = bool(widget.value)

    def afterEditing(self):
        self.push_values()
        self.parentApp.setNextForm(None)

class ConfigApp(npyscreen.NPSAppManaged):
    def onStart(self):
        self.addForm("MAIN", MyForm, name="config menu")

def main():
    global config, schema
    with open(SCHEMA, "r") as schema_fp, open(CONFIG, "r") as config_fp:
        schema = json.load(schema_fp)
        config = json.load(config_fp)

    App = ConfigApp()
    App.run()

    # after app closes, config has updated values
    with open(CONFIG, "w") as config_fp:
        json.dump(config, config_fp, indent=2)

if __name__ == "__main__":
    main()
