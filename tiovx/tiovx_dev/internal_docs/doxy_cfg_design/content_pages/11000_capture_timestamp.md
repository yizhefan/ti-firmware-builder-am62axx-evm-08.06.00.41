# Design Topic : Capture Timestamp {#did_capture_timestamp}

[TOC]

# Requirements Addressed {#did_capture_timestamp_requirements}

- https://jira.itg.ti.com/browse/TIOVX-812
- https://jira.itg.ti.com/browse/TIOVX-913
- https://jira.itg.ti.com/browse/TIOVX-914
- https://jira.itg.ti.com/browse/ADASVISION-4027

# Design Discussions {#did_capture_timestamp_design_discussions}

- https://confluence.itg.ti.com/x/IqMSEg

# Introduction {#did_capture_timestamp_intro}

- The TIOVX shall support the propagating of a timestamp property of a frame in the case of a source node.
  The included source node within the TIOVX framework is the tivxCaptureNode which captures images over 
  CSIRX.  This node shall take timestamp each of the frames and propagate these timestamps further through
  the OpenVX graph.  In addition, if a user creates a node, it shall have the ability to associate a timestamp
  with the OpenVX data reference.  Additionally, in the case that the source object is coming from the application,
  an API shall be provided to set the timestamp of the OpenVX data object from the application.  In the case that
  multiple inputs are received of a node, the most recent timestamp is propagated to output objects.  The GTC
  timer is used on all cores as the default timer so that each core is using the same timer.

# Directory Structure {#did_capture_timestamp_dir_structure}

    tiovx_dev/kernels_j7/hwa/capture/vx_capture_target.c
    source/framework/vx_target.c
    vision_apps/utils/console_io/src/

# Interface {#did_capture_timestamp_interface}

## Capture Timestamp API's {#did_capture_timestamp_interface_apis}

- In order to provide the required functionality of querying and setting timestamps from the application, a new
  reference attribute called "TIVX_REFERENCE_TIMESTAMP" was created.  In addition, an API did not exist which
  allowed the application to set an attribute within a vx_reference.  Therefore, the tivxSetReferenceAttribute
  API was created to allow for this functionality.  Finally, the existing tivxPlatformGetTimeInUsecs API was
  updated to use the GTC timer rather than a local timestamp.

## Capture Timestamp Implementation {#did_capture_timestamp_interface_implementation}

- The implementation of the tivxSetReferenceAttribute API and the querying of the TIVX_REFERENCE_TIMESTAMP
  attribute followed the same convention as outlined in the OpenVX spec for other similar data object
  set and query functionality.
- The GTC timer was implemented by updating the exising tivxPlatformGetTimeInUsecs to instead read the GTC
  timer value.  On the C66, a RAT translation was required to read the GTC timer.
- The FVID2 driver calls provides a control command called FVID2_REGISTER_TIMESTAMP_FXN that allows kernel to
  provide a timestamp API which then is returned as a property of the dequeued Fvid2_Frame.  This control command
  was added and the timestamp was set inside the newly created timestamp property of the object descriptor.
- An additional function was added to the framework to loop through the input objects of a node prior to calling
  the process callback and obtaining the most recent timestamp.  The function then loops through the outputs of
  the node and sets the timestamp of each of these objects to be the most recent timestamp found during the first
  step.

## Capture Timestamp Testing {#did_capture_timestamp_interface_testing}

- The tivxCaptureNode was tested by querying the incoming timestamps of multiple channels and ensuring they are
  not identical.  Additionally, successive timestamps in a 30 fps test case were tested to ensure that the timestamps
  received were roughly 33ms apart.
- The propagation of timestamps across a graph was tested by using dummy capture nodes and querying the timestamp of
  output of the capture node and ensuring that it matched the same timestamp as downstream outputs.
- The querying and setting of timestamps was done by first setting a timestamp and querying the same reference and
  downstream nodes in the graph to ensure that it matched the set timestamp.

# Design Analysis and Resolution (DAR) {#did_capture_timestamp_dar}

## Design Decision : How to set timestamps {#did_capture_timestamp_dar_01}

- A decision was made on how to set timestamps.  Is this required to be set in the node or either node/application?

### Design Criteria: Application Developer Ease of Use {#did_capture_timestamp_dar_01_criteria}

- The primary objective with this feature is to ensure application developer ease of use

### Design Alternative: Require to be set in node {#did_capture_timestamp_dar_01_alt_01}

- One way is to require it to be set in the node.

### Design Alternative: Flexibility to be set in node or application {#did_capture_timestamp_dar_01_alt_02}

- Another way is to provide flexiblity for this to be set in node or application via set reference API or natively
  allowing node to access timestamp parameter in object descriptor.

### Final Decision {#did_capture_timestamp_dar_01_decision}

- Flexibility to be set in node or application

## Design Decision : Whether or not to have one timestamp in multi channel app {#did_capture_timestamp_dar_02}

- A decision was made on whether or not a single timestamp would be propagated in the case of multi channel
  applications.

### Design Criteria: Information provided to user {#did_capture_timestamp_dar_02_criteria}

### Design Alternative: Have single timestamp in multi channel applications {#did_capture_timestamp_dar_02_alt_01}

### Design Alternative: Have per channel timestamps in multi channel applications {#did_capture_timestamp_dar_02_alt_02}

### Final Decision: Have per channel timestamps in multi channel applications {#did_capture_timestamp_dar_02_decision}

# Revision History {#did_rev_history}

TIOVX Version    | Date          | Author             | Description
-----------------|---------------|--------------------|------------
\ref TIOVX_1_09  | 1 May  2020   | Lucas W            | First draft
