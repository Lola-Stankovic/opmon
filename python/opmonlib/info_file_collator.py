#!/usr/bin/env python3

import json
import os
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
    for jf in json_files:
        console.log(f"Reading info JSON file {jf.name}")
        for line in jf:
            jsons.append(json.loads(line))

    #console.log(jsons)

    data = {}

    # format is partition.objectinstance.key.time: value

    for jsonobj in jsons:
        if '__parent' not in jsonobj:
            continue
        for partition in jsonobj['__parent']:
            if partition not in data:
                data[partition] = {}
            partitionobj = jsonobj['__parent'][partition]
    
            if '__children' not in partitionobj:
                continue

            if '__properties' in partitionobj:
                for datatype in partitionobj['__properties']:
                    datatypeobj = partitionobj['__properties'][datatype]
                    thistime = datatypeobj['__time']
                    for key in datatypeobj['__data']:
                        if key not in data[partition]:
                            data[partition][key] = {}
                        thisvalue = datatypeobj['__data'][key]
                        data[partition][key][thistime] = thisvalue

            for objectinstance in partitionobj['__children']:
                if objectinstance not in data[partition]:
                    data[partition][objectinstance] = {}
                objectinstanceobj = partitionobj['__children'][objectinstance]
            
                if '__properties' not in objectinstanceobj:
                    continue

                for datatype in objectinstanceobj['__properties']:
                    datatypeobj = objectinstanceobj['__properties'][datatype]
                    thistime = datatypeobj['__time']
                    for key in datatypeobj['__data']:
                        if key not in data[partition][objectinstance]:
                            data[partition][objectinstance][key] = {}
                        thisvalue = datatypeobj['__data'][key]
                        data[partition][objectinstance][key][thistime] = thisvalue

    json.dump(data, output_file, indent=4, sort_keys=True)
    console.log(f"Operation complete")


if __name__ == '__main__':
    try:
        cli(show_default=True, standalone_mode=True)
    except Exception as e:
        console.print_exception()
