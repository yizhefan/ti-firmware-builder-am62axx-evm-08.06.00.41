# Remote Core Heap Statistics {#group_apps_utilities_app_heap_stats}

[TOC]

# Introduction

This application queries remote services running on each of the remote
RTOS cores and reports instantaneous memory utilization of the various memory
regions for each remote core. This utility is helpful to see the remote core memory
utilization required for running one or more applications on the HLOS.

It can also be used to quickly identify if running an application on the HLOS incurrs any
memory leaks on a remote core.  To do this, one simply needs run this utility before and
after running an application, and then compare the printout between the two runs to see if
memory utilization has changed.

# Background

Upon booting of different remote cores, some of the local memory sections
are initialized and allocated according to the compiled programs running
on each core (regardless of what is running on the HLOS).  This is why if
you run this utility after the HLOS finishes booting before running any other
program, there is some of the remote core local memory regions already reserved.

Likewise, when an OpenVX graph is created and "validated", some of the node which
run on the remote cores may require to reserve some persistent or scratch memory.
This is taken from the pool of remote local carveout.  If this memory runs out, then
the OpenVX vxVerifyGraph() function will fail, which should prompt the developer to carve out
larger memory carveouts for that specific remote core and recompile so that the
application can run with sufficient memory.  Upon deleting the graph, any
memory reserved by associated nodes shall be freed, thus returning the memory allocation
state to what it was before running creating the graph and running the application.


# Supported plaforms

Platform  | Linux x86_64 | Linux+RTOS mode | QNX+RTOS mode | SoC
----------|--------------|-----------------|---------------|----
Support   | NO           | YES             |  YES          | J721e / J721S2 / J784S4

# Steps to run the application on J7 EVM (Linux + RTOS mode)

-# Build the application and related libraries as mentioned in \ref BUILD_INSTRUCTIONS for Linux+RTOS mode
-# Run the app as shown below
   \code
   cd /opt/vision_apps
   source ./vision_apps_init.sh
   ./vx_app_heap_stats.out
   \endcode
-# Output will be sent to the terminal standard output.

# Steps to run the application on J7 EVM (QNX + RTOS mode)

-# Build the application and related libraries as mentioned in \ref BUILD_INSTRUCTIONS for QNX+RTOS mode
-# Run the app as shown below
   \code
   cd /ti_fs/vision_apps
   . ./vision_apps_init.sh
   ./vx_app_heap_stats.out
   \endcode
-# Output will be sent to the terminal standard output.

# Sample Output

Shown below is a example output from running this utility:

\if DOCS_J721E

```
root@j7-evm:/opt/vision_apps# ./vx_app_heap_stats.out
APP: Init ... !!!
MEM: Init ... !!!
MEM: Initialized DMA HEAP (fd=4) !!!
MEM: Init ... Done !!!
IPC: Init ... !!!
IPC: Init ... Done !!!
REMOTE_SERVICE: Init ... !!!
REMOTE_SERVICE: Init ... Done !!!
APP: Init ... Done !!!

Detailed CPU memory statistics,
===============================

CPU: mcu2_0: HEAP:   DDR_SHARED_MEM: size =    8388608 B, free =    8386304 B ( 99 % unused)
CPU: mcu2_0: HEAP:           L3_MEM: size =     131072 B, free =     131072 B (100 % unused)
CPU: mcu2_0: HEAP:  DDR_NON_CACHE_M: size =      65536 B, free =      65536 B (100 % unused)

CPU: mcu2_1: HEAP:   DDR_SHARED_MEM: size =   16777216 B, free =   16774912 B ( 99 % unused)
CPU: mcu2_1: HEAP:           L3_MEM: size =     131072 B, free =     131072 B (100 % unused)
CPU: mcu2_1: HEAP:  DDR_NON_CACHE_M: size =   67043328 B, free =   47144960 B (  6 % unused)

CPU:  c6x_1: HEAP:   DDR_SHARED_MEM: size =   16777216 B, free =   16774912 B ( 99 % unused)
CPU:  c6x_1: HEAP:           L2_MEM: size =     229376 B, free =     229376 B (100 % unused)
CPU:  c6x_1: HEAP:  DDR_SCRATCH_MEM: size =   50331648 B, free =   50331648 B ( 14 % unused)

CPU:  c6x_2: HEAP:   DDR_SHARED_MEM: size =   16777216 B, free =   16774912 B ( 99 % unused)
CPU:  c6x_2: HEAP:           L2_MEM: size =     229376 B, free =     229376 B (100 % unused)
CPU:  c6x_2: HEAP:  DDR_SCRATCH_MEM: size =   50331648 B, free =   50331648 B ( 14 % unused)

CPU:  c7x_1: HEAP:   DDR_SHARED_MEM: size =  268435456 B, free =  268435456 B (  4 % unused)
CPU:  c7x_1: HEAP:           L3_MEM: size =    8159232 B, free =    8159232 B (100 % unused)
CPU:  c7x_1: HEAP:           L2_MEM: size =     491520 B, free =     491520 B (100 % unused)
CPU:  c7x_1: HEAP:           L1_MEM: size =      16384 B, free =      16384 B (100 % unused)
CPU:  c7x_1: HEAP:  DDR_SCRATCH_MEM: size =  234881024 B, free =  234881024 B (  8 % unused)


APP: Deinit ... !!!
REMOTE_SERVICE: Deinit ... !!!
REMOTE_SERVICE: Deinit ... Done !!!
IPC: Deinit ... !!!
IPC: DeInit ... Done !!!
MEM: Deinit ... !!!
MEM: Alloc's: 0 alloc's of 0 bytes
MEM: Free's : 0 free's  of 0 bytes
MEM: Open's : 0 allocs  of 0 bytes
MEM: Deinit ... Done !!!
APP: Deinit ... Done !!!
APP HEAP STATS: Done !!!
```

\endif

\if DOCS_J721S2

```
root@j721s2-evm:/opt/vision_apps# ./vx_app_heap_stats.out 
APP: Init ... !!!
MEM: Init ... !!!
MEM: Initialized DMA HEAP (fd=4) !!!
MEM: Init ... Done !!!
IPC: Init ... !!!
IPC: Init ... Done !!!
REMOTE_SERVICE: Init ... !!!
REMOTE_SERVICE: Init ... Done !!!
   359.259491 s: GTC Frequency = 200 MHz
APP: Init ... Done !!!

Detailed CPU memory statistics,
===============================

CPU: mcu2_0: HEAP:   DDR_SHARED_MEM: size =   16777216 B, free =   16773120 B ( 99 % unused)

CPU: mcu2_1: HEAP:   DDR_SHARED_MEM: size =   16777216 B, free =   16773120 B ( 99 % unused)

CPU:  c7x_1: HEAP:   DDR_SHARED_MEM: size =  268435456 B, free =  268435200 B ( 99 % unused)
CPU:  c7x_1: HEAP:           L3_MEM: size =    3964928 B, free =    3964928 B (100 % unused)
CPU:  c7x_1: HEAP:           L2_MEM: size =     458752 B, free =     458752 B (100 % unused)
CPU:  c7x_1: HEAP:           L1_MEM: size =      16384 B, free =      16384 B (100 % unused)
CPU:  c7x_1: HEAP:  DDR_SCRATCH_MEM: size =  419430400 B, free =  419430400 B (100 % unused)

CPU:  c7x_2: HEAP:   DDR_SHARED_MEM: size =   16777216 B, free =   16776960 B ( 99 % unused)
CPU:  c7x_2: HEAP:           L2_MEM: size =     458752 B, free =     458752 B (100 % unused)
CPU:  c7x_2: HEAP:           L1_MEM: size =      16384 B, free =      16384 B (100 % unused)
CPU:  c7x_2: HEAP:  DDR_SCRATCH_MEM: size =   67108864 B, free =   67108864 B (100 % unused)



Detailed CPU OS memory statistics,
==================================

CPU: mcu2_0
  semaphore:  cnt=    123
  mutex:      cnt=      0
  queue:      cnt=      0
  event:      cnt=      0
  heap:       cnt=      1
  mailbox:    cnt=      0
  task:       cnt=     32
  clock:      cnt=      0
  hwi:        cnt=     15
  timer:      cnt=      1

CPU: mcu2_1
  semaphore:  cnt=     42
  mutex:      cnt=      0
  queue:      cnt=      0
  event:      cnt=      0
  heap:       cnt=      1
  mailbox:    cnt=      0
  task:       cnt=     13
  clock:      cnt=      0
  hwi:        cnt=      6
  timer:      cnt=      1

CPU:  c7x_1
  semaphore:  cnt=     37
  mutex:      cnt=      0
  queue:      cnt=      0
  event:      cnt=      0
  heap:       cnt=      1
  mailbox:    cnt=      0
  task:       cnt=     18
  clock:      cnt=      0
  hwi:        cnt=      4
  timer:      cnt=      1

CPU:  c7x_2
  semaphore:  cnt=     30
  mutex:      cnt=      0
  queue:      cnt=      0
  event:      cnt=      0
  heap:       cnt=      1
  mailbox:    cnt=      0
  task:       cnt=     11
  clock:      cnt=      0
  hwi:        cnt=      4
  timer:      cnt=      1


APP: Deinit ... !!!
REMOTE_SERVICE: Deinit ... !!!
REMOTE_SERVICE: Deinit ... Done !!!
IPC: Deinit ... !!!
IPC: DeInit ... Done !!!
MEM: Deinit ... !!!
MEM: Alloc's: 0 alloc's of 0 bytes 
MEM: Free's : 0 free's  of 0 bytes 
MEM: Open's : 0 allocs  of 0 bytes 
MEM: Deinit ... Done !!!
APP: Deinit ... Done !!!
APP HEAP STATS: Done !!!
```

\endif

\if DOCS_J784S4

```
root@j784s4-evm:/opt/vision_apps# ./vx_app_heap_stats.out 
APP: Init ... !!!
MEM: Init ... !!!
MEM: Initialized DMA HEAP (fd=4) !!!
MEM: Init ... Done !!!
IPC: Init ... !!!
IPC: Init ... Done !!!
REMOTE_SERVICE: Init ... !!!
REMOTE_SERVICE: Init ... Done !!!
   372.914097 s: GTC Frequency = 200 MHz
APP: Init ... Done !!!

Detailed CPU memory statistics,
===============================

CPU: mcu2_0: HEAP:    DDR_LOCAL_MEM: size =   16777216 B, free =   16773120 B ( 99 % unused)

CPU: mcu2_1: HEAP:    DDR_LOCAL_MEM: size =   16777216 B, free =   16776960 B ( 99 % unused)

CPU: mcu3_0: HEAP:    DDR_LOCAL_MEM: size =    8388608 B, free =    8388352 B ( 99 % unused)

CPU: mcu3_1: HEAP:    DDR_LOCAL_MEM: size =    8388608 B, free =    8388352 B ( 99 % unused)

CPU: mcu4_0: HEAP:    DDR_LOCAL_MEM: size =    8388608 B, free =    8388352 B ( 99 % unused)

CPU: mcu4_1: HEAP:    DDR_LOCAL_MEM: size =    8388608 B, free =    8388352 B ( 99 % unused)

CPU:  c7x_1: HEAP:    DDR_LOCAL_MEM: size =  134217728 B, free =  134217472 B ( 99 % unused)
CPU:  c7x_1: HEAP:           L3_MEM: size =    3145728 B, free =    3145728 B (100 % unused)
CPU:  c7x_1: HEAP:           L2_MEM: size =     458752 B, free =     458752 B (100 % unused)
CPU:  c7x_1: HEAP:           L1_MEM: size =      16384 B, free =      16384 B (100 % unused)
CPU:  c7x_1: HEAP:  DDR_SCRATCH_MEM: size =  134217728 B, free =  134217728 B (100 % unused)

CPU:  c7x_2: HEAP:    DDR_LOCAL_MEM: size =  134217728 B, free =  134217472 B ( 99 % unused)
CPU:  c7x_2: HEAP:           L3_MEM: size =    3145728 B, free =    3145728 B (100 % unused)
CPU:  c7x_2: HEAP:           L2_MEM: size =     458752 B, free =     458752 B (100 % unused)
CPU:  c7x_2: HEAP:           L1_MEM: size =      16384 B, free =      16384 B (100 % unused)
CPU:  c7x_2: HEAP:  DDR_SCRATCH_MEM: size =  134217728 B, free =  134217728 B (100 % unused)

CPU:  c7x_3: HEAP:    DDR_LOCAL_MEM: size =  134217728 B, free =  134217472 B ( 99 % unused)
CPU:  c7x_3: HEAP:           L3_MEM: size =    3145728 B, free =    3145728 B (100 % unused)
CPU:  c7x_3: HEAP:           L2_MEM: size =     458752 B, free =     458752 B (100 % unused)
CPU:  c7x_3: HEAP:           L1_MEM: size =      16384 B, free =      16384 B (100 % unused)
CPU:  c7x_3: HEAP:  DDR_SCRATCH_MEM: size =  134217728 B, free =  134217728 B (100 % unused)

CPU:  c7x_4: HEAP:    DDR_LOCAL_MEM: size =  134217728 B, free =  134217472 B ( 99 % unused)
CPU:  c7x_4: HEAP:           L3_MEM: size =    3145728 B, free =    3145728 B (100 % unused)
CPU:  c7x_4: HEAP:           L2_MEM: size =     458752 B, free =     458752 B (100 % unused)
CPU:  c7x_4: HEAP:           L1_MEM: size =      16384 B, free =      16384 B (100 % unused)
CPU:  c7x_4: HEAP:  DDR_SCRATCH_MEM: size =  134217728 B, free =  134217728 B (100 % unused)



Detailed CPU OS memory statistics,
==================================

CPU: mcu2_0
  semaphore:  cnt=    132
  mutex:      cnt=      0
  queue:      cnt=      0
  event:      cnt=      0
  heap:       cnt=      1
  mailbox:    cnt=      0
  task:       cnt=     42
  clock:      cnt=      0
  hwi:        cnt=     17
  timer:      cnt=      1

CPU: mcu2_1
  semaphore:  cnt=     94
  mutex:      cnt=      0
  queue:      cnt=      0
  event:      cnt=      0
  heap:       cnt=      1
  mailbox:    cnt=      0
  task:       cnt=     24
  clock:      cnt=      0
  hwi:        cnt=     12
  timer:      cnt=      1

CPU: mcu3_0
  semaphore:  cnt=     34
  mutex:      cnt=      0
  queue:      cnt=      0
  event:      cnt=      0
  heap:       cnt=      1
  mailbox:    cnt=      0
  task:       cnt=     17
  clock:      cnt=      0
  hwi:        cnt=      4
  timer:      cnt=      1

CPU: mcu3_1
  semaphore:  cnt=     34
  mutex:      cnt=      0
  queue:      cnt=      0
  event:      cnt=      0
  heap:       cnt=      1
  mailbox:    cnt=      0
  task:       cnt=     17
  clock:      cnt=      0
  hwi:        cnt=      4
  timer:      cnt=      1

CPU: mcu4_0
  semaphore:  cnt=     34
  mutex:      cnt=      0
  queue:      cnt=      0
  event:      cnt=      0
  heap:       cnt=      1
  mailbox:    cnt=      0
  task:       cnt=     17
  clock:      cnt=      0
  hwi:        cnt=      4
  timer:      cnt=      1

CPU: mcu4_1
  semaphore:  cnt=     34
  mutex:      cnt=      0
  queue:      cnt=      0
  event:      cnt=      0
  heap:       cnt=      1
  mailbox:    cnt=      0
  task:       cnt=     17
  clock:      cnt=      0
  hwi:        cnt=      4
  timer:      cnt=      1

CPU:  c7x_1
  semaphore:  cnt=     43
  mutex:      cnt=      0
  queue:      cnt=      0
  event:      cnt=      0
  heap:       cnt=      1
  mailbox:    cnt=      0
  task:       cnt=     24
  clock:      cnt=      0
  hwi:        cnt=      5
  timer:      cnt=      1

CPU:  c7x_2
  semaphore:  cnt=     43
  mutex:      cnt=      0
  queue:      cnt=      0
  event:      cnt=      0
  heap:       cnt=      1
  mailbox:    cnt=      0
  task:       cnt=     24
  clock:      cnt=      0
  hwi:        cnt=      5
  timer:      cnt=      1

CPU:  c7x_3
  semaphore:  cnt=     43
  mutex:      cnt=      0
  queue:      cnt=      0
  event:      cnt=      0
  heap:       cnt=      1
  mailbox:    cnt=      0
  task:       cnt=     24
  clock:      cnt=      0
  hwi:        cnt=      5
  timer:      cnt=      1

CPU:  c7x_4
  semaphore:  cnt=     43
  mutex:      cnt=      0
  queue:      cnt=      0
  event:      cnt=      0
  heap:       cnt=      1
  mailbox:    cnt=      0
  task:       cnt=     24
  clock:      cnt=      0
  hwi:        cnt=      5
  timer:      cnt=      1


APP: Deinit ... !!!
REMOTE_SERVICE: Deinit ... !!!
REMOTE_SERVICE: Deinit ... Done !!!
IPC: Deinit ... !!!
IPC: DeInit ... Done !!!
MEM: Deinit ... !!!
DDR_SHARED_MEM: Alloc's: 0 alloc's of 0 bytes 
DDR_SHARED_MEM: Free's : 0 free's  of 0 bytes 
DDR_SHARED_MEM: Open's : 0 allocs  of 0 bytes 
DDR_SHARED_MEM: Total size: 536870912 bytes 
MEM: Deinit ... Done !!!
APP: Deinit ... Done !!!
APP HEAP STATS: Done !!!

```

\endif
