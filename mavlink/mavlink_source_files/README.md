# MAVLink source files

## Contents:

* `rural.tlog` contains the raw data packet captures from a flight with the drone over rural terrain (golf course)
* `extractbymessageid.c` extracts all messages of a given ID; `usage: extractbymessageid inputFile[.pcap|.tlog|.mav] outputFile[.mav] messageID[s]`
* `extractonebyid.c` extracts a single of a given ID; `usage: extractonebyid inputFile[.pcap|.tlog|.mav] outputFile[.mav] messageID[s]`
* `extractbymessagenumber.c` extracts messages at a given position in the file; `usage: extractbymessagenumber inputFile[.pcap|.tlog|.mav] outputFile[.mav] messageNumber[s]`
* `countcodes.c` counts the number of unique message IDs; `usage: countcodes inputFile[.pcap|.mav|.tlog]`
* `makemav.c` creates a file containing only the MAVLink portion of the messages; `usage: makemav inputFile[.pcap|.tlog] outputFile[.mav]`
* `makepcap.c` creates a file containing full PCAP messages for analysis in Wireshark; `usage: makepcap inputFile[.mav|.tlog] outputFile[.pcap]`

All files include the MAVLink data processing library for which the source code can be found in `../../utils/`.

## Usage:

In the `../utils/` directory:
* `make clean`
* `make`

Back in the `mavlink_source_code/` directory:
* `make clean`
* `make`
* Run any other programs as outlined by their above usage statements to preform their respective tasks
