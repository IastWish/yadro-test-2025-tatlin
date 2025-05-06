# Test task for TATLIN.Unified
## Description
This project implements a data storage device of tape type (Tape), designed for sequential writing and reading of data, and an algorithm for sorting data from the input tape to the output tape.
- The RAM size limit is set manually as one of the command line parameters as a number of bytes.
- The configuration file `config.txt` contains tape delay information written in milliseconds.
- Input and output files can be specified both in text `*.txt` and binary `*.bin` form. If the input file is specified in text form, it is converted to binary for the algorithm working, and at the end it is converted back to text form if necessary.

## Build
After compilation, the programme must be run with the following command:
```
./main <input_file> <output_file> <config.txt> <MemoryLimit>
```
