# opmonlib - Operational Monitoring library

### Desctription

*opmonlib* defines the interface that allows applications and application components to publish operational monitoring data.
The package contains some basic output plugins, to stdout (trace) or to file.

The behavior of the output is controlled via the URI that is passed to the OpMonManager constructor.

For the two plugins privided within opmonlib the following URI can be used:

- stdout:
outputs one block for every schema in a json format.
- file:///file/path/file_name.out

[Instructions for DAQ module users](Instructions-for-DAQ-module-users.md)

%% ### Building and running examples (_under construction_)


