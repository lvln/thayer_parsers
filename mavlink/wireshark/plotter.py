# 
# plotter.py --- plots frequency versus message type for a given capture
# 
# Author: Joshua Meise
# Created: 06-28-2024
# Version: 1.0
# 
# Inputs: * csv file with message type and frequency
#         * path to save plot to
# 
# 
import matplotlib.pyplot as plt 
import sys

try:
    f = open(sys.argv[1], "r")
except FileNotFoundError:
    print("File does not exist, exiting")
else:
    ids = []
    freq = []
    
    for line in f:
        currentLine = line.split(",")
        ids.append(currentLine[0])
        freq.append(int(currentLine[1]))
    f.close()

    plt.tight_layout()
    plt.subplots_adjust(bottom=0.55)
    plt.bar(ids, freq, color ='maroon', width = 0.5)
    plt.xlabel("Message type")
    plt.ylabel("Number of occurances")
    plt.title("Number of occurances of each message type during a capture")
    plt.xticks(ids, rotation=90)
    plt.savefig(f"{sys.argv[2]}")

