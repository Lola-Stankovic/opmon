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

    def parse_children(childarr, parentobj):
        for child in childarr:
            if child not in parentobj:
                parentobj[child] = {}
            childobj = childarr[child]

            if '__children' in childobj:
                parse_children(childobj['__children'], parentobj[child])

            if '__properties' in childobj:
                for datatype in childobj['__properties']:
                    datatypeobj = childobj['__properties'][datatype]
                    thistime = datatypeobj['__time']
                    for key in datatypeobj['__data']:
                        if key not in parentobj[child]:
                            parentobj[child][key] = {}
                        thisvalue = datatypeobj['__data'][key]
                        parentobj[child][key][thistime] = thisvalue

    for jsonobj in jsons:
        parse_children(jsonobj['__parent'], data)

    json.dump(data, output_file, indent=4, sort_keys=True)
    console.log(f"Operation complete")


if __name__ == '__main__':
    try:
        cli(show_default=True, standalone_mode=True)
    except Exception as e:
        console.print_exception()
