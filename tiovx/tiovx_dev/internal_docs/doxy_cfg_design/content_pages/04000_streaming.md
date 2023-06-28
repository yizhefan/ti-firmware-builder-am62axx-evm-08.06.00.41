# Design Topic : Streaming {#did_streaming}

[TOC]

# Requirements Addressed {#did_streaming_requirements}

- https://jira.itg.ti.com/browse/TIOVX-73

# Introduction {#did_streaming_intro}

Purpose of streaming is to allow graph executions to occur continuously
without requiring a user to execute the graph continuously. The streaming
API's are defined in the OpenVX Pipelining and Streaming Extension.

# Directory Structure {#did_streaming_dir_structure}

    source/framework/vx_graph_stream.c    # streaming framework location

# Diagrams {#did_streaming_diagrams}

## Component Interaction {#did_streaming_component_interaction}

![](streaming_diagram.png "Streaming State Machine")

# Resource usage {#did_streaming_resource_usage}

# Error handling {#did_streaming_error_handling}

- All the streaming APIs must return a error to the application in case of error
  and not assert within the APIs

- The allocation of streaming resources is performed during the verification stage
  and will return error during verification if the allocation of these resources
  fails.

# Interface {#did_streaming_interface}

## Streaming API's {#did_streaming_interface_apis}

- The streaming API's are defined by the OpenVX Pipelining and Streaming Extension.

- Streaming must be enabled by the graph by using the TI-specific API 
  tivxGraphEnableStreaming, which takes as arguments the graph and the capture
  node.

- All streaming API's can be found in the location source/framework/vx_graph_stream.c

## Streaming Implementation {#did_streaming_interface_implementation}

- The streaming API's are implemented by creating a new thread per graph to be streamed.
  A different thread task is created depending on whether pipelining is enabled or not.
  Within the thread is a state machine that tracks the streaming execution state of the
  graph. The state machines are designed to allow re-starting of the graph even after it
  has been stopped. The threads make use of graph event queues to communicate when to re-
  start the graph.

- In the non-pipelining streaming thread, re-triggering of the graph occurs only once the
  previous execution of the graph has completed. In the pipelining streaming thread, the
  re-triggering of the graph occurs once the capture node has completed.

## Streaming Testing {#did_streaming_interface_testing}

- The streaming is tested using kernels found in conformance_tests/kernels. The test
  kernels simulate the operation of a capture and display node by having only an output
  from the capture node and only an input to the display node. The capture node acts as a
  counter by incrementing the scalar output from the capture node. The display node verifies
  that the input has incremented and returns an error if the operation has failed. The test
  kernels also consistent of an intermediate node that passes the value from the capture
  node to the display node.

- The test cases are found in conformance_tests/kernels/test_kernels/test/test_graph_stream.c.
  The test cases verify streaming functionality when pipelining is both enabled and disabled.
  A set of negative test cases are also contained within this file to verify scenarios that
  are not allowed by the framework.

# Design Analysis and Resolution (DAR) {#did_streaming_dar}

## Design Decision : xyz {#did_streaming_dar_01}

### Design Criteria: xyz {#did_streaming_dar_01_criteria}

### Design Alternative: xyz {#did_streaming_dar_01_alt_01}

### Design Alternative: xyz {#did_streaming_dar_01_alt_02}

### Final Decision {#did_streaming_dar_01_decision}

# Revision History {#did_rev_history}

TIOVX Version     | Date          | Author             | Description
------------------|---------------|--------------------|------------
\ref TIOVX_1_04  | 24 Oct 2019   | Lucas W            | First draft
