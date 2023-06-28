# Copyright (c) 2020 Texas Instruments

##########################################################################
# LIST THE LIBS USED IN A MAP FILE
#
# Usage: python3 list_dep_headers.py
#        - Run this script from the vision_apps/out/J7/A72/LINUX/release/module/modules
#          folder (or debug)
#        - It will list the union of all headers required from compiled .dep files in
#          filtered list of apps
#
# Author: Jesse Villarreal - TI
##########################################################################

import sys
import subprocess
import os
from os import listdir
import re
import fileinput
from collections import OrderedDict
import argparse

def filesWithMatchInDir(cmd):
    try:
        return subprocess.check_output(cmd, shell=True).split()
    except:
        return []

def parseDepFile(dep_file, prefix):
	
    hdr_list = []
    for line in open(dep_file,'r').readlines():
        if(not re.search("targetfs", line)):
            result = re.search(prefix + '(.*h)', line)
            if (result):
                hdr_list.append(result.group(1))
                #print (result.group(1))

    res = list(OrderedDict.fromkeys(hdr_list))
    res.sort()
    return res


def listHeadersFromFiles(file_list, workarea):

    first_time = 1
    common_list = {}
    for file in file_list:
        file_lib_list = parseDepFile(file, workarea)
        
        common_list = list(set(common_list)|set(file_lib_list))
    return common_list


file_list=listHeadersFromFiles(filesWithMatchInDir("find apps.* -name *.dep"), "/home/a0323847local/gitrepo/psdkra/" )
file_list.sort()

for file in file_list:
    print(file)
