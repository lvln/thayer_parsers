# MAVLink source files

## Contents:

* `shorterRun.pcap` contains the raw packet captures from a shorter flight with the drone
* `longerRun.pcap` contains the raw packet captures from a longer flight with the drone
* `run1.pcap` contains the cleanded data from `shorterRun.pcap`
* `run2.pcap` contains the cleanded data from `longerRun.pcap`
* `cleandata.c` cleans the data, removing MDNS messages; `usage: cleandata inputFile outputFile [message numbers]`, where both `inputFile` and `outputFile` are PCAP files and `message numbers` are sequential numbers of the messages to be extracted from `inputFile` and placed in `outputFile`
* `extractdata.c` extracts all messages of a given id; `usage: extractdata inputFile outputFile messageIDs`, where both `inputFile` and `outputFile` are PCAP files and `message IDs` are ids of messages to be extracted from `inputFile` and placed in `outputFile`
* `untruncate.c` standardizes the length of MAVLink 2 messages by inserting 0s for truncated empty bytes (see main README for information on packet truncation); `usage: untruncate inputFile outputFile`, where both `inputFile` and `outputFile` are PCAP files
* `countcodes.c` counts the number of unique message ids; `usage: countcodes inputFile`, where `inputFile` is a PCAP file
* `removewrappers.c` removes all PCAP wrappers from a file and outputs a file containing only MAVLink messages; `usage: removewrappers inputFile`, where `inputFile` is a PCAP file and it outputs a .mav file of the same name as `inputFile`

All files include the mavlink data processing library for which the source code can be found in `../../utils`.

## Usage:

* `make clean` removes all generated executable files as well
* `make` builds all executables
* `./clean.sh inputFile outputFile` removes all non-MAVLink messages (MDNS) from a Wireshark capture completed with a UDP filter
* `./preprocess.sh inputFile` removes all PCAP wrappers from a file and outputs a file containing only MAVLink message headers and payloads - input is a `.pcap` file, output is a `.mav` file of the same name as the input file
