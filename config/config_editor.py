#
# config_editor.py - edit the current config
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
            elif opt_type == "string":
                w = self.add(
                    npyscreen.TitleText,
                    name=f"{option['text']} [{define}]",
                    value=str(config.get(define, ""))
                )
            else:
                continue

            self.widgets.append((define, opt_type, w))

    def push_values(self):
        for define, opt_type, widget in self.widgets:
            if opt_type == "boolean":
                config[define] = bool(widget.value)
            elif opt_type == "string":
                config[define] = str(widget.value)

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
