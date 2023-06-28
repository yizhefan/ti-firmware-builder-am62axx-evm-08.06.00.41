# Design Topic : Multi-process support {#did_multi_process}

[TOC]

# Requirements Addressed {#did_multi_process_requirements}

- https://jira.itg.ti.com/browse/TIOVX-705
- https://jira.itg.ti.com/browse/TIOVX-847
- https://jira.itg.ti.com/browse/TIOVX-848

# Design Discussions {#did_multi_process_design_discussions}

- https://confluence.itg.ti.com/x/XyjPE

# Introduction {#did_multi_process_intro}

The purpose of enabling multi-process support in Linux is to allow multiple OpenVX applications to run simultaneously in the Linux OS.  There are many scenarios in which a customer may have a separate executable that must be run simultaneously, for instance, in the case that a surround view and a valet parking application should be run simultaneously.

# Directory Structure {#did_multi_process_dir_structure}

    source/framework/vx_obj_desc.c
    tiovx_dev/platform/psdk_j7/linux/tivx_platform_linux.c
    tiovx_dev/platform/psdk_j7/common/tivx_ipc.c

# Error handling {#did_multi_process_error_handling}

- Error handling of object descriptor allocation is performed by returning a NULL object descriptor in the event that it cannot be allocated.

- Error handling of POSIX semphore usage is given by an error statement during tivxPlatformInit

# Interface {#did_multi_process_interface}

## Multi-process API's {#did_multi_process_interface_apis}

- No new API's were introduced in this design that are exposed to the application.

- The API tivxIpcGetSelfPortId was introduced as an internal API to the framework to return
  the object descriptor IPC port being used. This calls into the OSAL IPC layer to return the
  IPC port.

## Multi-process Implementation {#did_multi_process_interface_implementation}

- During the design dicussion, it was determined that in order to allow multi-process execution on Linux, the following items had to be addressed:
     - 1. Linux shared memory region protected
     - 2. Object descriptor allocation protected
     - 3. Exchange of buffers across cores needs to be threadsafe
- During the design discussion, it was determined that the TIOVX 1.07 implementation was in the state below:
     - 1. Linux shared memory region was already protected due to the usage of DMA buffers
     - 2. Object descriptor allocation was not protected
     - 3. Exchange of buffers across cores was not threadsafe due to usage of same port for all buffer exchanges
- The following design was discussed for each of the areas below:
     - 1. N/A
     - 2. Protect object descriptor allocation by the use of POSIX semphore
     - 3. Add an additional API for exchanging both the port and CPU ID for IPC and store this within the port and CPU ID for future IPC transactions.

## Multi-process Testing {#did_multi_process_interface_testing}

- In order to perform initial testing the #2 from above, existing applications were run to ensure that there were no changes in how they operated.

- Final testing of the entire design was done by running the test case ADASVISION-3733

# Design Analysis and Resolution (DAR) {#did_multi_process_dar}

## Design Decision : Object descriptor allocation protection {#did_multi_process_dar_01}

- A design descision was made determining how to protect the object descriptor allocation in the case of multi-process.

### Design Criteria: The mechanism for protecting object descriptors allow for multi-process and be contained within framework {#did_multi_process_dar_01_criteria}

- The mechanism chosen for protecting the object descriptor allocation must allow for multi-process execution in addition to no application interference.

### Design Alternative: POSIX semaphore {#did_multi_process_dar_01_alt_01}

- Use a POSIX semaphore to protect allocation.

### Design Alternative: Mutex {#did_multi_process_dar_01_alt_02}

- Protect this allocation using mutext.

### Final Decision {#did_multi_process_dar_01_decision}

- Uses POSIX semaphore because it is multi-process safe.

## Design Decision : Object descriptor allocation protection {#did_multi_process_dar_02}

- A design descision was made determining how to have thread-safe passing of object descriptors.

### Design Criteria: The mechanism for passing object descriptors allow for multi-process and be contained within framework {#did_multi_process_dar_02_criteria}

- The mechanism chosen for passing object descriptors must allow for multi-process execution in addition to no application interference.

### Design Alternative: IPC port and CPU ID {#did_multi_process_dar_02_alt_01}

- The IPC port and CPU ID must be contained within object descriptor.

### Design Alternative: CPU ID only {#did_multi_process_dar_02_alt_02}

- The CPU ID only must be contained within object descriptor.

### Final Decision {#did_multi_process_dar_02_decision}

- The IPC port and CPU ID must be contained within object descriptor in order to be multi-process safe.

# Revision History {#did_rev_history}

TIOVX Version    | Date          | Author             | Description
-----------------|---------------|--------------------|------------
\ref TIOVX_1_08 | 4 Feb  2020   | Lucas W            | First draft
