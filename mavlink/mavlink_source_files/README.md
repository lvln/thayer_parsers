# MAVLink source files

## Contents:

* `shorterRun.pcap` contains the raw packet captures from a shorter flight with the drone
* `longerRun.pcap` contains the raw packet captures from a longer flight with the drone
* `run1.pcap` contains the cleanded data from `shorterRun.pcap` (only MAVLink messages but still with the PCAP headers and wrapper such that is is still viewable in Wireshark)
* `run2.pcap` contains the cleanded data from `longerRun.pcap` (only MAVLink messages but still with the PCAP headers and wrapper such that is is still viewable in Wireshark)
* `cleandata.c` cleans the data, removing MDNS messages; `usage: cleandata inputFile[.pcap] outputFile[.pcap]`
* `extractbymessageid.c` extracts all messages of a given id; `usage: extractbymessageid inputFile[.pcap] outputFile[.mav] messageID[s]`
* `extractbymessagenumber.c` extracts messages at a given position in the file; `usage: extractbymessagenumber inputFile[.pcap]/[.mav] outputFile[.mav] messageNumber[s]`
* `countmessages.c` counts the number of messages in a file (default.pcap if not .mav extension) and prints that value; `usage: countmessages inputFile[.pcap]/[.mav]`
* `countcodes.c` counts the number of unique message ids; `usage: countcodes inputFile[.pcap]`
* `makemav.c` removes all PCAP wrappers from a file and outputs a file containing only MAVLink messages; `usage: makemav inputFile[.pcap] outputFile[.mav]`

All files include the MAVLink data processing library for which the source code can be found in `../../utils`.

## Usage:

In the `../utils` directory:
* `make clean`
* `make`

Back in the `mavlink_source_code` directory:
* `make clean`
* `make`
* Run any other programs as outlined by their above usage statements to preform their respective tasks
