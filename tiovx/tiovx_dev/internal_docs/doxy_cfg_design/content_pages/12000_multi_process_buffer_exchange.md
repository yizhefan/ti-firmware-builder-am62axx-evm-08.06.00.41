# Design Topic : Memory hande import/export and multi-process Buffer Exchange {#did_multi_process_buffer_exchange}

[TOC]

# Requirements Addressed {#did_multi_process_buffer_exchange_requirements}

- https://jira.itg.ti.com/browse/TIOVX-900

# Design Discussions {#did_multi_process_buffer_exchange_design_discussions}

- https://jira.itg.ti.com/browse/TIOVX-900

# Introduction {#did_multi_process_buffer_exchange_intro}

1. In performance crtical applications where data needs to flow across different frameworks, it is important
   to ensure zero-copy semantics at the framework boundaries, where possible. The data could flow across
   multiple framworks with in the same process or across multiple processes. The feature TIOVX-900 tries to
   address this within the context of Linux environment.
   Following are a few examples of the data flows across framework boundaries:
   1. ProcessA running openCV and openVX frameworks and the data needs to flow across the frameworks
   2. ProcessA (ex:- sensor capture) and processB (ex:- sensor data consumer) running openVX framework based
      applications and these need to exchange data with zero-copy.
   3. ProcessA running two threads, one dealing with bare virtual memory pointers (ex:- ethernet based Lidar
      sensor driver) and another dealing with openVX objects (ex:- a graph with Lidar processing chain) that
      drives the sensor data into the graph. In this case there should be no copying of the sensor data into the
      openVX objects.
2. In order to achieve this, we need support for a few operations from the openVX framework, namely
   1. Ability to export the handles (pointers to memory buffers embedded in the objects) from an openVX object.
   2. Ability to import external handles into an openVX object. These handles could be a result of an export from
      another openVX object or could be created directly through tivxMemAlloc() or equivalent API, but from a valid
      DMA heap.
   3. Ability to translate a handle to/from a unix 'fd' (file descriptor) suitable for communication with related
      processes (parent/child resulting from fork/spawn) or unrelated processes (processes with no relationship).

# Directory Structure {#did_multi_process_buffer_exchange_dir_structure}

    include/TI/tivx.h
    include/TI/tivx_mem.h
    tiovx_dev/platform/psdk_j7/common/tivx_mem.c
    source/framework/vx_reference.c
    utils/mem/src/

# Interface {#did_multi_process_buffer_exchange_interface}

## Memory Address Translation API's {#did_multi_process_buffer_exchange_translation_apis}

- In order to provide the ability to translate the buffer handles to/from the file descriptors, the following
  APIs have been designed. These address 2(c) above.
  - tivxMemTranslateVirtAddr(): This translates a given handle (virtual address) to a 'file descriptor' and
    a 'physical address'
  - tivxMemTranslateFd(): This translates a given 'file descriptor' to virtual and physical addresses. 

## Memory Handle Import/Export API's {#did_multi_process_buffer_exchange_import_export_apis}

- In order to provide the ability to import and export handles into/from openVX objects, the following
  APIs have been designed:
  - tivxReferenceImportHandle(): This imports a set of handles into an openVX object. The handles can be 'NULL'.
    This addresses 2(b) above.
  - tivxReferenceExportHandle(): This exports a valid set of handles from an openVX object. The exported handles
    can never be 'NULL', if the operation succeeds. This addresses 2(a) above.

## Memory Address Translation API Implementation {#did_multi_process_buffer_exchange_translation_apis_implementation}

- TIVX memory allocation on Linux uses DMA heaps. Any memory block allocated from the DMA is considered to be
  'valid' for this discussion. Allocations from other places are considered 'invalid' and will fail to be
  imported, if attempted.
- TIVX memory manager maintains a linked list for tracking all the allocated blocks on Linux. As a part of this
  implementation, the tuple {fd, virtAddr, phyAddr} is derived and maintained for each block. This information
  is looked up anytime there is a need for translating a given fd (file descriptor) to the associated
  {virtAddr, phyAddr} pair or for translating a given 'virtAddr' to the associated {fd, phyAddr} pair. These
  translations will work for any memory block allocated by the tivxMemAlloc() API.
- For the memory blocks allocated from the same DMA heap but not using tivxMemAlloc(), no record exists in the
  TIVX memory manager linked list until the first import operation. This poses a problem when attempts are made to
  translate
  - <b>fd to {virtAddr, phyAddr}</b>: This is addressed through the implementation of the tivxMemTranslateFd() API. When
    the default look-up in the internal memory list fails during an import operation, an entry will be created for
    a valid memory block and added to the list. This is possible because using the 'fd', the {virtAddr, phyAddr}
    pair can be derived. Once the import operation succeeds, any subsequent look-up of the same memory block
    will succeed until the block is released from the list due to a memory release operation.
  - <b>virtAddr to {fd, phyAddr}</b>: If the default look-up fails, the import operation cannot proceed since it is not
    possible to derive the {fd, phyAddr} pair from the 'virtAddr' information, thus the import operation results in a
    failure.

## Memory Handle Import/Export API Implementation {#did_multi_process_buffer_exchange_import_export_apis_implementation}

- The reference Import/Export APIs are  designed such that, if for any reason the processing encounters failures, either
  due to the interface parameter check failures, or memory allocation failures, the state of the object involved is not
  changed. The 'state' of the object includes object meta-data and internal handle allocation status. Please refer to the
  API documentation of the tivxReferenceImportHandle() and tivxReferenceExportHandle() for further details.

## Memory Address Translation and Import/Export API Testing {#did_multi_process_buffer_exchange_interface_testing}

- The translation APIs have been tested with the memory blocks created using tivxMemAlloc() and malloc() and expected
  outcome is checked.
- A test has been created on Linux that allocates openVX objects, exports the handles and translates them to 'fd' values
  and sent over to another unrelated process where the 'fd' values are translated back to the virtPtrs and imported into
  newly created openVX objects. This creates memory aliasing where two objects belonging to different processes point to
  the same memory block and the updates made by one process are seen and verified by another process.

# Design Analysis and Resolution (DAR) {#did_multi_process_buffer_exchange_dar}

## Design Decision : State of the internal handles after Export Handle operation {#did_multi_process_buffer_exchange_dar_01}

- A question arised as what should be the state of the internal handles once they have been exported.

### Design Criteria: Support common use cases and eliminate repeated memory allocations {#did_multi_process_buffer_exchange_dar_01_criteria}

- The primary objective is to support the common "memory" alias use case and to eliminate repeated memory allocations.

### Design Alternative: Set them to NULL {#did_multi_process_buffer_exchange_dar_01_alt_01}

- One of the operations of exporting handle API is that if the handles are NULL, memory is allocated.
  If the handles are set to NULL every time upon each export operation, it would lead to unnecessary memory
  allocations and if the application is not careful, it can lead to memory management issues.

### Design Alternative: Not set them to NULL {#did_multi_process_buffer_exchange_dar_01_alt_02}

- This allows exporting handles and either import them into another object or use the handle as raw pointers, where
  by memory buffer aliasing is created which can be exploited in certain usecases.

### Final Decision {#did_multi_process_buffer_exchange_dar_01_decision}

- The decision has been to not set internal handles to NULL upon export operation.

## Design Decision : What should be done to the internal handles during import operation {#did_multi_process_buffer_exchange_dar_02}

- A question arised as what should be done to the internal handles, if they are non-NULL, during the import operation.
- A second question was whether to allow importing NULL handles.

### Design Criteria: Memory manager state consistency and system stability {#did_multi_process_buffer_exchange_dar_02_criteria}
- The import/export handle operations should not create system stability issues especially when releasing aliased memory blocks.

### Design Alternative: Overwrite handles with warning message to user {#did_multi_process_buffer_exchange_dar_02_alt_01}
- Overwrite the internal handles with the provided handles and issue a warning message, if the internal handles are
  not NULL at the time of importing the handles.

### Design Alternative: Provide a flag in the API to control the overwrite {#did_multi_process_buffer_exchange_dar_02_alt_02}
- Add an interface boolean flag to let the user specify whether to overwrite the internal handles during the import. 

### Final Decision: {#did_multi_process_buffer_exchange_dar_02_decision}
- The decision has been made to not add the boolean interface flag but leave a possibility of adding it in future
  based on the user feedback. The decision was taken againt adding this flag so as to not expose too many internal
  details on how and when the memory is allocated in the openVX objects, even though this knowledge seems to be quite
  pervasive.
- Allow NULL handles to be imported. This necessary since the export and import operations could lead to memory aliasing
  and when one of the objects involved in alising is released then the associated memory handle is also released and this
  causes system instability issues. Allowing NULL handle import allows to remove the alising in one of the objects before
  rleasing the objects themselves.

# Revision History {#did_rev_history}

TIOVX Version    | Date          | Author             | Description
-----------------|---------------|--------------------|------------
\ref TIOVX_1_09  | 9 June 2020   | Vijay Pothukuchi   | First draft
