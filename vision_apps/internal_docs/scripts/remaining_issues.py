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
# project = ADASVISION AND issuetype=Requirement AND fixVersion = SDK_J7_07_00_00  AND fixVersion not in (SDK_J7_06_02_00)
#
# Fixed Bugs:
# project = ADASVISION AND issuetype=Bug AND fixVersion = SDK_J7_07_00_00  AND affectedVersion != SDK_J7_07_00_00  AND (status = Closed OR status = Resolved OR status = "In Build")
#
# Remaining Bugs:
# project = ADASVISION AND issuetype = Bug AND status != Closed AND status != Resolved AND status != "In Build" AND platform = J7-EVM
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
    affectedVersionIndex = -1
    platformIndex = -1

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

    try:
        affectedVersionIndex = fields.index("Affects Version/s")
    except ValueError:
        print("Error: Affected version not included in CSV file\n")
        sys.exit(0)

    try:
        platformIndex = fields.index("Custom field (Platform)")
    except ValueError:
        print("Error: Platform not included in CSV file\n")
        sys.exit(0)

    for row in readCSV: 
        rows.append(row)

    htmlname = file_name.split(".")[0]
    htmlfile = open("%s.html" % htmlname, "w")  

    for row in rows:
        s = "<tr>\n"
        htmlfile.write(s)
        s = "<td valign=\"center\">%s</td>\n" % (row[issueKeyIndex])
        htmlfile.write(s)
        s = "<td>%s</td>\n" % (row[summaryKeyIndex])
        htmlfile.write(s)
        s = "<td align=\"center\">'''TODO Module'''</td>\n"
        htmlfile.write(s)
        s = "<td align=\"center\">%s</td>\n" % (row[affectedVersionIndex])
        htmlfile.write(s)
        s = "<td align=\"center\">%s</td>\n" % (row[platformIndex])
        htmlfile.write(s)
        s = "<td align=\"center\">'''TODO Occurrence'''</td>\n"
        htmlfile.write(s)
        s = "<td align=\"center\">'''TODO Workaround'''</td>\n"
        htmlfile.write(s)
        s = "</tr>\n"
        htmlfile.write(s)

    htmlfile.close() 




