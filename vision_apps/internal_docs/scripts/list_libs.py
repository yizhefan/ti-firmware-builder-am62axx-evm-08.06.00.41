# Copyright (c) 2020 Texas Instruments

##########################################################################
# LIST THE LIBS USED IN A MAP FILE
#
# Usage: python list_libs.py
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

def filesWithMatchInDir(a):
    cmd = "ls " + a
    try:
        return subprocess.check_output(cmd, shell=True).split()
    except:
        return []

def listLibsFromMap(infile):

    lib_list = []
    state = 0
    for line in open(infile,'r').readlines():
        if(state == 0 and re.search("Allocating common symbols", line)):
            state = 1
        elif(state == 1):
            result = re.search(".*\/lib(.*)\.a\(.*\.o\)", line)
            if (result):
                lib_list.append(result.group(1))
    res = list(OrderedDict.fromkeys(lib_list))
    res.sort()
    return res

def listLibsFromMapRtos(infile):

    lib_list = []
    state = 0
    for line in open(infile,'r').readlines():
        if(state == 0 and re.search("SECTION ALLOCATION MAP", line)):
            state = 1
        elif(state == 1):
            result = re.search("([A-Za-z0-9\._]*\.aer5f|[A-Za-z0-9\._]*\.ae66|[A-Za-z0-9\._]*\.ae71|[A-Za-z0-9\._]*\.lib|[A-Za-z0-9\._]*\.a\s)", line)
            if (result):
                lib_list.append(result.group(1).rstrip("\n"))

    res = list(OrderedDict.fromkeys(lib_list))
    res.sort()
    return res

def listIntersectionOfMaps(file_list):

    first_time = 1
    common_list = []
    for file in file_list:
        file_lib_list = listLibsFromMap(file)
        if (first_time == 1):
            common_list = file_lib_list
            first_time = 0
        else:
            file_lib_list_set = set(file_lib_list)
            intersection = file_lib_list_set.intersection(common_list)
            common_list = list(intersection)
    common_list.sort()
    return common_list


def FullMergeDict(D1, D2):
    for key, value in D1.items():
        if key in D2:
            if type(value) is dict:
                FullMergeDict(D1[key], D2[key])
            else:
                if type(value) in (int, float, str):
                    D1[key] = [value]
                if type(D2[key]) is list:
                    D1[key].extend(D2[key])
                else:
                    D1[key].append(D2[key])
    for key, value in D2.items():
        if key not in D1:
            D1[key] = value

def listDictOfMaps(file_list):

    first_time = 1
    common_list = {}
    for file in file_list:
        file_lib_list = listLibsFromMap(file)
        dict_list = dict.fromkeys(file_lib_list, [str(file)])

        FullMergeDict(common_list, dict_list)

        for key, value in common_list.items():
            common_list[key] = list(OrderedDict.fromkeys(common_list[key]))
    #print(common_list)

    f = open("out.csv",'w+')
    f.write(",")
    for file in file_list:
        f.write(str(file)+",")
    f.write("\n")
    for key, value in common_list.items():
        f.write(key)
        for file in file_list:
            f.write(",")
            if(str(file) in value):
                f.write("X")
        f.write("\n")
    f.close()



intersection = False
dictionary = False
usage_help = False
full = False
argfile = 0

if (len(sys.argv) > 1):
    if (sys.argv[1] == 'help'):
        usage_help = True
    elif (sys.argv[1] == 'intersection'):
        intersection = True
    elif (sys.argv[1] == 'table'):
        dictionary = True
    elif (sys.argv[1] == 'full'):
        full = True
    else:
        argfile = sys.argv[1]

if (len(sys.argv) == 1 or usage_help):
    print("Usage: " + sys.argv[0] + "<options>")
    print("  where <options> are:")
    print("  help or no arg:     : Prints this usage message and returns")
    print("  <name of .map file> : Prints names of static libs used in single map file")
    print("  intersection        : Prints only common static libs used in all map files in directory")
    print("  full                : Prints all static libs used in all map files in directory")
    print("  table               : outputs out.csv file with table of static libs to exe files from all map files in directory")


common_libs = []
if(intersection):
    common_libs = listIntersectionOfMaps(filesWithMatchInDir("*.map"))
    for lib in common_libs:
        print("\t"+ lib)

elif(dictionary):
    listDictOfMaps(filesWithMatchInDir("*.map"))

elif (full):
    file_list = filesWithMatchInDir("*.map")
    for file in file_list:
        file_lib_list = listLibsFromMap(file)
        print(str(file) + ":")
        for lib in file_lib_list:
            print("\t"+ lib)
        print()

elif (argfile != 0):
    common_libs = listLibsFromMap(argfile)
    for lib in common_libs:
        print("\t"+ lib)
    common_libs = listLibsFromMapRtos(argfile)
    for lib in common_libs:
        print("\t"+ lib)
