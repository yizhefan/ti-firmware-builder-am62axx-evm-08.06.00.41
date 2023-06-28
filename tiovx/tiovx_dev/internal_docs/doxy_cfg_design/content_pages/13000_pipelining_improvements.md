# Design Topic : Pipelining Improvements {#did_pipelining_improvements}

[TOC]

# Requirements Addressed {#did_pipelining_improvements_requirements}

- https://jira.itg.ti.com/browse/TIOVX-902
- https://jira.itg.ti.com/browse/TIOVX-903

# Introduction {#did_pipelining_improvements_intro}

- The previous implementation of the TIOVX framework requires the applications to manually 
  set the pipeline depth of a graph and the buffer depth of a given node.  For customers
  who may not be as familiar with TI's pipelining implementation, this presents a challenge
  when trying to get a prototype of an application working in real time.  Therefore, in order
  to alleviate this burden on the application, a scheme has been developed for the framework
  to parse the graph structure and automatically set the pipeline depth and the buffer depth
  in the case that the application has not already done so.  The customer is still free to
  override this value in the case that they would like to have more control over the application
  memory usage.

# Directory Structure {#did_pipelining_improvements_dir_structure}

    source/framework/vx_graph_verify.c
    source/framework/vx_graph_pipeline.c

# Resource usage {#did_pipelining_improvements_resource_usage}

- Given the implementation of the framework, the pipeline depth corresponds to the number of copies
  of this graph structure that is being created.  Additionally, the buffer depth setting will also
  increase the memory usage of the application.  Given the static memory allocation scheme of OpenVX,
  if the required values exceed the limit of the required buffers and pipeline depth, an error will
  be thrown during the graph verification.

# Error handling {#did_pipelining_improvements_error_handling}

- As previously mentioned, if the user explicitly sets the pipeline depth using tivxSetGraphPipelineDepth
  or sets the buffer depth via tivxSetNodeParameterNumBufByIndex, the framework will use the value set
  by the framework.
- The framework will not try to set a buffer depth if the parameter has already been set as a graph
  parameter.
- If the required pipeline depth or buffer depth exceeds the statically allocated values of either of
  these parameters, these values will be set to the maximum value and a message will be given to the
  user about this.
- A new logging level called VX_ZONE_OPTIMIZATION was created to be used to indicate to the user the
  information about the values being automatically set.

# Interface {#did_pipelining_improvements_interface}

## Pipelining Improvements Implementation {#did_pipelining_improvements_interface_implementation}

- The implementation of the graph pipelining depth detection involved first creating a new parameter
  of the vx_node object to track the "depth" of each individual node.  Each node is initially assigned
  a depth value of 1.  During the verify graph stage, the depth value of the nodes are set during the sorting
  of the graph.  This depth value is determined by taking the max depth of the input nodes providing inputs to
  this node and adding 1 to this max value.  Therefore, when determining the pipeline depth of the graph, the
  largest node depth value will be the value set as the total pipeline depth.
- The implementation of the node buffer depth involved first querying all of the output parameters of the graph
  for two sets of information.
   - First, the information obtained from these parameters was whether or not they were graph parameters.  If
     they are graph parameters, then the buffer depth is set via the application and therefore, this parameter
     would be skipped. 
   - If the parameter was not a graph parameter, the framework would query this parameter to see if a buffer
     value was set at this parameter.
      - If there was a buffer depth set, the framework would still calculate the optimal value in order to
        communicate to the user whether or not the set value was optimal.
      - If the buffer depth was not set, the framework would see how many nodes touch this buffer and set the
        buffer value to this number.  It would also flag to the user via the VX_ZONE_OPTIMIZATION logging level
        that the buffer value had been set automatically.

## Pipelining Improvements Testing {#did_pipelining_improvements_interface_testing}

- There were three different tests developed to test the graph pipeline depth.  They involved different graph
  structures to ensure the pipeline depth was set properly.  A new graph query attribute was added to query
  the graph in these scenarios to ensure the value was set properly.
- The first test was a serial construction of the graph.  In this test, the graph pipelining depth will return
  the length of the entire serial sequence of nodes.

![](pipelining_serial.png "Pipelining Test 1")

- The second test was a parallel construction of the graph.  In this test, the first parallel strand has a
  longer depth (4) than the second strand (3).  Therefore, this will return a depth of 4.

![](pipelining_parallel.png "Pipelining Test 2")

- The third test was a parallel construction of the graph that converges back onto a single node.
  In this test, the first sequence of nodes is only 3 nodes deep while the second sequence of nodes is 4.
  Therefore, the graph depth will be set to 4.

![](pipelining_parallel_merge.png "Pipelining Test 3")

- The buffer depth setting had multiple tests to ensure the detection worked properly.  In order to validate
  the number of buffers set, an API was created to query the buffer depth value.
  - In the first test, the graph construction required multiple buffers at a reference that was
    connected to only two nodes, a producer and a consumer.  Double buffering was all that was required for
    this test.
  - In the second test, the graph construction required multiple buffers at a reference that was
    connected to a producer node and two consumer nodes.  Therefore, triple buffering was required for
    this test.

# Revision History {#did_rev_history}

TIOVX Version    | Date          | Author             | Description
-----------------|---------------|--------------------|------------
\ref TIOVX_1_11  | 14 Dec  2020  | Lucas W            | First draft
