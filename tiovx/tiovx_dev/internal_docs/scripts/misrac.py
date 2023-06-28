# Copyright (c) 2019 Texas Instruments

##########################################################################
# FIX HIGH VOLUME MISRAC VIOLATIONS
#
# Usage: python misrac.py
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

verbose = False
if (len(sys.argv) > 1) and (sys.argv[1] == '-v'):
    verbose = True

def filesWithMatchInRepo(a):
    cmd = "git --git-dir .git grep -l \"" + a + "\""
    try:
        return subprocess.check_output(cmd, shell=True).split()
    except:
        return []


def addCast(enum_list, enum_string):

    mod_files = []
    for enum in enum_list:
        files = filesWithMatchInRepo(enum)
        for file in files:
            file = file.decode('ascii')
            if (file[-1] == 'c' and file[:17] != "conformance_tests"):
                mod_files.append(file)
    res = list(OrderedDict.fromkeys(mod_files))

    for file in res:
        count = 0
        bak_file = file+".bak"
        f = open(bak_file,'w+')
        for line in open(file,'r').readlines():
            if(len(line.strip()) > 2 and line.strip()[:2] != "* " and line.strip()[:2] != "/*" and not re.search('"', line)):
                for enum in enum_list:
                    if(re.search(enum, line)):
                        if(None == re.search(enum_string+enum, line)):
                            line = re.sub(enum, enum_string+enum, line)
                            #line = re.sub(r'(\W+)' + enum + r'(\W+)', r'\1'+enum_string+enum+r'\2', line)
                            count+=1
            f.write(line)
        f.close()
        os.rename(bak_file, file)
        print(file+":\t"+str(count))


def castEnums():

    h_paths = [
        'include/VX',
        'include/TI',
        'source/include'
    ]

    h_files = []
    list_enums = []
    list_status = []
    list_bool = ["vx_false_e", "vx_true_e"]
    list_image = []
    list_vxlib = ["VXLIB_SUCCESS"]
    list_vxlib_data_type = ["VXLIB_UINT", "VXLIB_INT", "VXLIB_FLOAT"]

    for dir in h_paths:
        local = os.listdir(dir)
        for file in local:
            h_files.append(dir + '/' + file)

    #print (h_files)

    for file in h_files:
        state = 0
        list_type = "enum"
        for line in open(file,'r').readlines():
            line = line.rstrip()
            # Start of enum
            if (state == 0) and ((line[0:5] == 'enum ') or (line[0:13] == 'typedef enum ')):
                state = 1
                if(re.search("vx_status_e",line)):
                    list_type = "status"
                elif(re.search("vx_df_image_e",line)):
                    list_type = "image"
                else:
                    list_type = "enum"
                #print(line)
            # End of enum
            elif (state == 1):
                if (re.search(";",line)):
                    #print(line)
                    state = 0
                elif (re.search("VX_",line) and line.strip()[0] != "*" and line.strip()[:2] != "/*" and not re.search("VX_ZONE_",line)):
                    words = re.findall("TIVX_\w+|VX_\w+", line)
                    print(list_type + ":" + words[0])
                    if(list_type == "enum"):
                        list_enums.append(words[0])
                    elif(list_type == "image"):
                        list_image.append(words[0])
                    else:
                        list_status.append(words[0])
                    #print(words[0])

    #addCast(list_bool, "(vx_bool)")
    #addCast(list_status, "(vx_status)")
    #addCast(list_image, "(vx_df_image)")
    #addCast(list_enums, "(vx_enum)")
    #addCast(list_vxlib, "(vx_status)")
    addCast(list_vxlib_data_type, "(uint32_t)")

def castVxlib():

    search = "status = VXLIB_"
    replace = "status = (vx_status)VXLIB_"
    files = filesWithMatchInRepo(search)
    mod_files = []
    for file in files:
        file = file.decode('ascii')
        if (file[-1] == 'c' and file[:17] != "conformance_tests"):
            mod_files.append(file)
    res = list(OrderedDict.fromkeys(mod_files))

    for file in res:
        count = 0
        bak_file = file+".bak"
        f = open(bak_file,'w+')
        for line in open(file,'r').readlines():
            if(len(line.strip()) > 2 and line.strip()[:2] != "* " and line.strip()[:2] != "/*" and not re.search('"', line)):
                if(re.search(search, line)):
                    if(None == re.search(replace, line)):
                        line = re.sub(search, replace, line)
                        count+=1
            f.write(line)
        f.close()
        os.rename(bak_file, file)
        print(file+":\t"+str(count))



castEnums()
#castVxlib()





