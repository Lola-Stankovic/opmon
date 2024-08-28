#!/usr/bin/env python3

import json
import os
from re import A, I
import rich.traceback
from rich.console import Console
from os.path import exists, join

# Add -h as default help option
CONTEXT_SETTINGS = dict(help_option_names=['-h', '--help'])

console = Console()

import click

@click.command(context_settings=CONTEXT_SETTINGS)
@click.option('-o', '--output-file', type=click.File('w'), default='opmon_collated.json')
@click.argument('json_files', type=click.File('r'), nargs=-1)

def cli(output_file, json_files):
    
    console.log(f"Reading specified JSON files and outputting collated value traces to {output_file.name}")

    jsons = []
    jd = json.JSONDecoder()
    for jf in json_files:
        console.log(f"Reading info JSON file {jf.name}")
        text = jf.read()
        idx=0
        while idx < len(text):
            res = jd.raw_decode(text, idx)
            jsons.append(res[0])
            idx = res[1] + 2

    #console.log(jsons)

    data = {}

    # format is session.application.substructure.name: value

    for jsonobj in jsons:
        session = jsonobj["origin"]["session"]
        application = jsonobj["origin"]["application"]
        substructure = ""
        if "substructure" in jsonobj["origin"]:
            substructure = ".".join(jsonobj["origin"]["substructure"])

        customOrigin = ""
        if "customOrigin" in jsonobj:
            first = True
            for k,v in jsonobj["customOrigin"].items():
                if not first:
                    customOrigin += "."
                customOrigin += f"{k}:{v}"
                first = False

        if session not in data:
            data[session] = {}

        if application not in data[session]:
            data[session][application] = {}

        if substructure != "":
            if substructure not in data[session][application]:
                data[session][application][substructure] = {}
            objref = data[session][application][substructure]
        else:
            objref = data[session][application]

        if customOrigin != "":
            if customOrigin not in objref:
                objref[customOrigin] = {}
            objref = objref[customOrigin]

        for datapoint in jsonobj["data"]:
            if datapoint not in objref:
                objref[datapoint] = {}

            for value in jsonobj["data"][datapoint]:
                objref[datapoint][jsonobj["time"]]  = jsonobj["data"][datapoint][value]

    json.dump(data, output_file, indent=4, sort_keys=True)
    console.log(f"Operation complete")


if __name__ == '__main__':
    try:
        cli(show_default=True, standalone_mode=True)
    except Exception as e:
        console.print_exception()
