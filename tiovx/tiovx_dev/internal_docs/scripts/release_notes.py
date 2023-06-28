# Copyright (c) 2020 Texas Instruments

##########################################################################
# GENERATE REQUIREMENTS HTML FROM CSV
#
# Usage: python requirements.py <csvfile>
#
# Example: python requirements.py requirements.csv
#
# Below filters must be used to generate the 3 different CSV files needed for
# release notes then run with Python script
#
# Requirements:
# project = TIOVX AND issuetype=Requirement AND fixVersion = TIOVX_01.09.00 AND
# fixVersion not in (TIOVX_01.08.00)
#
# Fixed Bugs:
# project = TIOVX AND issuetype=Bug AND fixVersion = TIOVX_01.09.00 AND
# affectedVersion != TIOVX_01.09.00 AND (status = Closed OR status = Resolved
# OR status = "In Build")
#
# Remaining Bugs:
# project = TIOVX AND issuetype = Bug AND status != Closed AND status != Resolved
# AND status != "In Build" AND platform = J7-EVM
#
# Author: Lucas Weaver - TI
##########################################################################

import csv
import re
import sys

file_name = sys.argv[1]

with open(file_name) as csvfile:

    issueKeys = []
    summaries = []
    rows = []
    issueKeyIndex = -1
    summaryKeyIndex = -1

    # Parse CSV file
    readCSV = csv.reader(csvfile, delimiter=',')

    fields = next(readCSV)

    # Get indices for Issue Key and Summary
    try:
        issueKeyIndex = fields.index("Issue key")
    except ValueError:
        print("Error: Issue key not included in CSV file\n")
        sys.exit(0)

    try:
        summaryKeyIndex = fields.index("Summary")
    except ValueError:
        print("Error: Summary not included in CSV file\n")
        sys.exit(0)

    for row in readCSV: 
        rows.append(row)

    htmlname = file_name.split(".")[0]
    htmlfile = open("%s.html" % htmlname, "w")  

    for row in rows:
        s = "<li>%s: %s</li>\n" % (row[issueKeyIndex], row[summaryKeyIndex])
        htmlfile.write(s) 

    htmlfile.close() 




