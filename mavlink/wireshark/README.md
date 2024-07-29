# wireshark

This directory contains the MAVLink dissector for Wireshark with statistical analysis capabilities added.

In please see the MAVLink documentation [here](https://mavlink.io/en/guide/wireshark.html) for instructions on how to generte the `mavlink_2_common.lua` plugin. Once generated, it should be placed in the the directory `Users/user/.local/lib/wireshark/plugins` if working on macOS along with the python script `plotter.py`.

Note: The statistical analysis portion is still a work-in-progress and has been completed for all of the messages captured thus far.

## Contents

* `mavlink_2_common.lua` is a Wireshark plugin which contains a dissector for the MAVLink protocol
* `plotter.py` is a python script that plots a histogram of number of occorances versus message type for all messages in a capture

## Usage

* Update the `dir` variable in `mavlink_2_common.lua` to reflect the path to your Wireshark plugins directory
* Launch Wireshark by typing `wireshark` in the command line
* Open a PCAP file contaning MAVLink messages or start a packet capture
* After opening the PCAP file or upon completion of the packet capture, navigate to `Statistics/MAVLink Analysis` in the Wireshark GUI to initiate statistical analysis

## Statistical analysis

* Break-down of number of occurances of each message type:
  <br><img src="../.images/nooccurances.jpg" alt="Number of occurances" width="350">
* Break-down of the number of occurances of each message length (in bytes):
  <br><img src="../.images/len.jpg" alt="Message length" width="350">
* Bar graph of the number of occurances of each message type:
  <br><img src="../.images/bar.jpg" alt="Bar chart" width="500">
* Analysis on each field part of the MAVLink header:
  <br><img src="../.images/header.jpg" alt="Header" width="700">
* For each message type, provides an analysis of each field of the message:
  <br><img src="../.images/message.jpg" alt="Message" width="700">
  
  
  
  
