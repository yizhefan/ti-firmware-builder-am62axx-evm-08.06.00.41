Windows VS++ 2010 solution and project are in dcc_gen_win
To build for Linux - use dcc_gen subfolder
basic, devel and debug build options are chosen in inc\private.h
basic(generates only BIN file):
//#define DEVEL       //This turns on the developer option
//#define DCCREAD     //This turns on reverse reading option: from bin to txt

devel (generates BIN,C,H files):
#define DEVEL       //This turns on the developer option
//#define DCCREAD     //This turns on reverse reading option: from bin to txt

debug(prints BIN file as text):
//#define DEVEL       //This turns on the developer option
#define DCCREAD     //This turns on reverse reading option: from bin to txt

Version info can be found on bottom of inc\private.h


===========================================================================
===========================================================================
===========================================================================

command line:

dcc_gen_win_basic.exe <XML file name>
generates DCC BIN file from XML

====================
dcc_gen_win_devel.exe <XML file name>
generates DCC BIN, H file defining DCC structure, automatic C parser - to be used when DCC structure in XML and algorithm is changed (by developers only)

====================
dcc_gen_win_debug.exe <XML file name>
searches BIN file in the same folder with name same as the XML
generates TXT file with contents of the BIN
