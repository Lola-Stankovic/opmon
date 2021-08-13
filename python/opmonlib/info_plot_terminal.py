import json
import rich.traceback
from rich.console import Console
import plotext as plt
from datetime import datetime
import time

# Add -h as default help option
CONTEXT_SETTINGS = dict(help_option_names=['-h', '--help'])

console = Console()

import click

@click.command(context_settings=CONTEXT_SETTINGS)
@click.option('-p', '--plugin', type=str) # Name of the plugin for the metric
@click.option('-m', '--metric', type=str) # Name of the metric to be plotted
@click.option('-l', '--last', default=None) # Plot only the last 30m or 1h with -l 30m or -l 1h',
@click.option('-c', '--continuous-time', default=0) # How many seconds between updates in continuous mode
@click.argument('json_files', type=click.File('r'), nargs=-1)
def cli(plugin, metric, last, continuous_time, json_files):

    jsons = []
    for jf in json_files:
        console.log(f"Reading info JSON file {jf.name}")
        for line in jf:
            jsons.append(json.loads(line))

    while True:

        timevalues = []
        data = []

        # format is partition.objectinstance.key.timevalues: value

        for jsonobj in jsons:
            if '__parent' not in jsonobj:
                continue
            for partition in jsonobj['__parent']:
                partitionobj = jsonobj['__parent'][partition]

                if '__children' not in partitionobj:
                    continue

                objectinstanceobj = partitionobj['__children'][plugin]

                for datatype in objectinstanceobj['__properties']:
                    datatypeobj = objectinstanceobj['__properties'][datatype]
                    thistime = datatypeobj['__time']

                    thisvalue = datatypeobj['__data'][metric]
                    timevalues.append(thistime)
                    data.append(thisvalue)

        # Plot only the last minutes or the last hours
        if last is None:
            time_limit = None
        elif last[-1] == 'm':
            time_limit = int(last[:-1]) * 60
        elif last[-1] == 'h':
            time_limit = int(last[:-1]) * 3600
        idx = len(timevalues) - 1
        ts = timevalues[-1]
        if time_limit:
            time_limit = time_limit
            while idx >= 0:
                if ts - timevalues[idx]  > time_limit:
                    break
                idx -= 1
            timevalues = timevalues[idx:]
            data = data[idx:]

        timevalues = [datetime.utcfromtimestamp(x).strftime('%d/%m/%Y %H:%M') for x in timevalues]

        time_x = [plt.string_to_time(x) for x in timevalues]
        plt.xticks(time_x, timevalues)
        # plt.grid(True)
        plt.scatter(time_x, data, marker='dot')
        plt.colorless()
        plt.show()

        if continuous_time:
            time.sleep(continuous_time)
        else:
            break

if __name__ == '__main__':
    try:
        cli(show_default=True, standalone_mode=True)
    except Exception as e:
        console.print_exception()
