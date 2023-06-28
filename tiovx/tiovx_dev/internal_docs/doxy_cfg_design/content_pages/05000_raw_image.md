# Design Topic : Raw Image Data Object {#did_raw_image_object}

[TOC]

# Requirements Addressed {#did_raw_image_topic_requirements}

- https://jira.itg.ti.com/browse/TIOVX-512

# Introduction {#did_raw_image_topic_intro}

Purpose of the raw image data object is to provide native support in
TIOVX for raw images which come from Bayer image sensors. This was
decided upon after considering the alternatives:

- Using built-in vx_image object: One option is to define new DF_IMAGE
  formats for raw images, but there would be so many varieties if you
  consider different number of exposures from 1 to 3, meta information,
  bit widths, etc.  The multiple combinations of these would generate
  too many non-standard formats.
- Using user data object: A user data object is generic enough to handle
  the many combinations, but this option leaves all of the offset math
  into exposures and meta information to the application.

In the end, creating a new TI extension for raw image data objects will
give first class status to the object, enabling the framework to manage
the memory layout and indexing, simplifying the application's role in
the process.

# Directory Structure {#did_raw_image_topic_dir_structure}

    include/TI/
    └── tivx_ext_raw_image.h  # Raw image extension external API definition
    source
    ├── include
    │   └── tivx_raw_image.h  # Raw image internal data structures
    └── framework
        └── vx_raw_image.c    # Internal implementation

# Diagrams {#did_raw_image_topic_diagrams}

## Sequence Diagram {#did_raw_image_topic_sequence_diagram}

na

## Component Interaction {#did_raw_image_topic_component_interaction}

na

# Resource usage {#did_raw_image_topic_resource_usage}

- When the tivxCreateRawImage is called, a vx_reference is consumed
- When the tivx_raw_image is written to the first time (either by the user or the 
  framework, shared cacheable memory is carved out from the static memory
  allocator up to the size of the data buffer memory requirements.

# Error handling {#did_raw_image_topic_error_handling}

- All the raw image APIs must return a error to the application in case of error
  and not assert within the APIs

# Interface {#did_raw_image_topic_interface}

- See the following interface file for detailed interface specification:
  include/TI/tivx_ext_raw_image.h

- As a design goal, the object API shall be modeled after the existing OpenVX
  vx_image data object, so as to be familiar and consistent with existing
  OpenVX object design.

- In order to enable sensor drivers to fill configuration information for
  the node, the creation API shall take a pointer to a single configuration
  data structure rather than individiual scalar settings.  This enables
  scalability for the data structure to change over time without needing
  to change the create APIs, as well as the sensor driver to easily fill
  out the configuration structure before passing back to the application
  to simply pass to the creation API.

- The design requirements and details were hashed out at a meeting with
  stakeholders.  The link to the meeting minutes is located:
  https://confluence.itg.ti.com/x/ZETECQ

## Features {#did_raw_image_topic_features}

- Node interfaces:
  - Input to VISS
  - One of the object formats of the capture node
- Multi-exposure:
  - VISS supports up to 3 exposures, so the object shall support a programmable
    number of exposures with maximum of 3.
  - The exposures may be read out of the sensor on different CSI channels, or on
    the same channel as line interleaved.  When readout is on different channels,
    they can be stored in different buffers, but when readout is line interleaved,
    they must be stored in a single line-interleaved buffer.  Therefore, the data 
    object shall be configured to support "line_interleaved" mode when there are
    more than 1 exposures contained therein.
  - Each exposure shall share the same width and height dimentions, but may have 
    different bit depths (if in separate buffers).  Therefore, when configuring
    the object, a single width and height parameter is sufficient, but format
    shall be programmable for each exposure. 
- Meta-information
  - Sensors can optionally readout meta-information for each exposure, so the
    object shall be configured to account for this information in the data buffers.
  - Meta information is usually readout as extra lines either before or after the
    pixel data, so the object shall configure meta as number of lines, and before or
    after the pixel data.

# Design Analysis and Resolution (DAR) {#did_raw_image_topic_dar}

## Design Decision : xyz {#did_raw_image_topic_dar_01}

### Design Criteria: xyz {#did_raw_image_topic_dar_01_criteria}

### Design Alternative: xyz {#did_raw_image_topic_dar_01_alt_01}

### Design Alternative: xyz {#did_raw_image_topic_dar_01_alt_02}

### Final Decision {#did_raw_image_topic_dar_01_decision}

# Revision History {#did_rev_history}

TIOVX Version     | Date          | Author             | Description
------------------|---------------|--------------------|------------
\ref TIOVX_1_04  | 24 Oct 2019   | Lucas W            | First draft
