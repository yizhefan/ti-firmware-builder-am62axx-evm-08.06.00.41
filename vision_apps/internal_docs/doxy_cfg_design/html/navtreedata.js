/*
@ @licstart  The following is the entire license notice for the
JavaScript code in this file.

Copyright (C) 1997-2017 by Dimitri van Heesch

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

@licend  The above is the entire license notice
for the JavaScript code in this file
*/
var NAVTREE =
[
  [ "Vision Apps Design", "index.html", [
    [ "Introduction", "index.html", [
      [ "Functional Overview", "index.html#did_intro_overview", null ],
      [ "Assumptions and Constraints", "index.html#did_intro_assumptions", null ],
      [ "Features Not Supported", "index.html#did_intro_features_not_supported", null ],
      [ "Revision History", "index.html#did_rev_history", null ]
    ] ],
    [ "Top Level Design", "did_top_level_design.html", [
      [ "Requirements Addressed", "did_top_level_design.html#did_top_level_requirements", null ],
      [ "Top Level Directory Structure", "did_top_level_design.html#did_top_level_dir_structure", null ],
      [ "Top Level Component Interaction", "did_top_level_design.html#did_top_level_component_interaction", null ]
    ] ],
    [ "Application Design : Dense Optical Flow", "md_internal_docs_doxy_cfg_design_content_pages_03000_app_dof.html", [
      [ "Requirements Addressed", "md_internal_docs_doxy_cfg_design_content_pages_03000_app_dof.html#did_app_dof_requirements", null ],
      [ "Introduction", "md_internal_docs_doxy_cfg_design_content_pages_03000_app_dof.html#did_app_dof_intro", [
        [ "Purpose", "md_internal_docs_doxy_cfg_design_content_pages_03000_app_dof.html#did_app_dof_purpose", null ],
        [ "Short Application Description", "md_internal_docs_doxy_cfg_design_content_pages_03000_app_dof.html#did_app_dof_short_desc", null ],
        [ "Input and Output format", "md_internal_docs_doxy_cfg_design_content_pages_03000_app_dof.html#did_app_dof_io_format", null ]
      ] ],
      [ "Directory Structure", "md_internal_docs_doxy_cfg_design_content_pages_03000_app_dof.html#did_app_dof_dir_structure", null ],
      [ "Diagrams", "md_internal_docs_doxy_cfg_design_content_pages_03000_app_dof.html#did_app_dof_diagrams", [
        [ "Sequence Diagram", "md_internal_docs_doxy_cfg_design_content_pages_03000_app_dof.html#did_app_dof_sequence_diagram", null ],
        [ "Component Interaction", "md_internal_docs_doxy_cfg_design_content_pages_03000_app_dof.html#did_app_dof_component_interaction", null ],
        [ "OpenVX Graph", "md_internal_docs_doxy_cfg_design_content_pages_03000_app_dof.html#did_app_dof_openvx_graph", null ]
      ] ],
      [ "Resource usage", "md_internal_docs_doxy_cfg_design_content_pages_03000_app_dof.html#did_app_dof_resource_usage", null ],
      [ "Error handling", "md_internal_docs_doxy_cfg_design_content_pages_03000_app_dof.html#did_app_dof_error_handling", null ],
      [ "Interface", "md_internal_docs_doxy_cfg_design_content_pages_03000_app_dof.html#did_app_dof_interface", null ],
      [ "Design Analysis and Resolution (DAR)", "md_internal_docs_doxy_cfg_design_content_pages_03000_app_dof.html#did_app_dof_dar", [
        [ "Design Decision : none", "md_internal_docs_doxy_cfg_design_content_pages_03000_app_dof.html#did_app_dof_dar_01", [
          [ "Design Criteria: none", "md_internal_docs_doxy_cfg_design_content_pages_03000_app_dof.html#did_app_dof_dar_01_criteria", null ],
          [ "Design Alternative: none", "md_internal_docs_doxy_cfg_design_content_pages_03000_app_dof.html#did_app_dof_dar_01_alt_01", null ],
          [ "Design Alternative: none", "md_internal_docs_doxy_cfg_design_content_pages_03000_app_dof.html#did_app_dof_dar_01_alt_02", null ],
          [ "Final Decision", "md_internal_docs_doxy_cfg_design_content_pages_03000_app_dof.html#did_app_dof_dar_01_decision", null ]
        ] ]
      ] ]
    ] ],
    [ "Application Design : TI Deep Learning", "md_internal_docs_doxy_cfg_design_content_pages_03001_app_tidl.html", [
      [ "Requirements Addressed", "md_internal_docs_doxy_cfg_design_content_pages_03001_app_tidl.html#did_app_tidl_requirements", null ],
      [ "Introduction", "md_internal_docs_doxy_cfg_design_content_pages_03001_app_tidl.html#did_app_tidl_intro", [
        [ "Purpose", "md_internal_docs_doxy_cfg_design_content_pages_03001_app_tidl.html#did_app_tidl_purpose", null ],
        [ "Short Application Description", "md_internal_docs_doxy_cfg_design_content_pages_03001_app_tidl.html#did_app_tidl_short_desc", null ],
        [ "Input and Output format", "md_internal_docs_doxy_cfg_design_content_pages_03001_app_tidl.html#did_app_tidl_io_format", null ]
      ] ],
      [ "Directory Structure", "md_internal_docs_doxy_cfg_design_content_pages_03001_app_tidl.html#did_app_tidl_dir_structure", null ],
      [ "Diagrams", "md_internal_docs_doxy_cfg_design_content_pages_03001_app_tidl.html#did_app_tidl_diagrams", [
        [ "Sequence Diagram", "md_internal_docs_doxy_cfg_design_content_pages_03001_app_tidl.html#did_app_tidl_sequence_diagram", null ],
        [ "Component Interaction", "md_internal_docs_doxy_cfg_design_content_pages_03001_app_tidl.html#did_app_tidl_component_interaction", null ],
        [ "OpenVX Graph", "md_internal_docs_doxy_cfg_design_content_pages_03001_app_tidl.html#did_app_tidl_openvx_graph", null ]
      ] ],
      [ "Resource usage", "md_internal_docs_doxy_cfg_design_content_pages_03001_app_tidl.html#did_app_tidl_resource_usage", null ],
      [ "Error handling", "md_internal_docs_doxy_cfg_design_content_pages_03001_app_tidl.html#did_app_tidl_error_handling", null ],
      [ "Interface", "md_internal_docs_doxy_cfg_design_content_pages_03001_app_tidl.html#did_app_tidl_interface", null ],
      [ "Design Analysis and Resolution (DAR)", "md_internal_docs_doxy_cfg_design_content_pages_03001_app_tidl.html#did_app_tidl_dar", [
        [ "Design Decision : None", "md_internal_docs_doxy_cfg_design_content_pages_03001_app_tidl.html#did_app_tidl_dar_01", [
          [ "Design Criteria: None", "md_internal_docs_doxy_cfg_design_content_pages_03001_app_tidl.html#did_app_tidl_dar_01_criteria", null ],
          [ "Design Alternative: None", "md_internal_docs_doxy_cfg_design_content_pages_03001_app_tidl.html#did_app_tidl_dar_01_alt_01", null ],
          [ "Design Alternative: None", "md_internal_docs_doxy_cfg_design_content_pages_03001_app_tidl.html#did_app_tidl_dar_01_alt_02", null ],
          [ "Final Decision", "md_internal_docs_doxy_cfg_design_content_pages_03001_app_tidl.html#did_app_tidl_dar_01_decision", null ]
        ] ]
      ] ]
    ] ],
    [ "Infrastructure : Console IO", "md_internal_docs_doxy_cfg_design_content_pages_04000_infrastructure_console_io.html", [
      [ "Requirements Addressed", "md_internal_docs_doxy_cfg_design_content_pages_04000_infrastructure_console_io.html#did_infrastructure_console_io_requirements", null ],
      [ "Introduction", "md_internal_docs_doxy_cfg_design_content_pages_04000_infrastructure_console_io.html#did_infrastructure_console_io_intro", [
        [ "Purpose", "md_internal_docs_doxy_cfg_design_content_pages_04000_infrastructure_console_io.html#did_infrastructure_console_io_purpose", null ],
        [ "Short Application Description", "md_internal_docs_doxy_cfg_design_content_pages_04000_infrastructure_console_io.html#did_infrastructure_console_io_short_desc", null ],
        [ "Input and Output format", "md_internal_docs_doxy_cfg_design_content_pages_04000_infrastructure_console_io.html#did_infrastructure_console_io_io_format", null ]
      ] ],
      [ "Directory Structure", "md_internal_docs_doxy_cfg_design_content_pages_04000_infrastructure_console_io.html#did_infrastructure_console_io_dir_structure", null ],
      [ "Diagrams", "md_internal_docs_doxy_cfg_design_content_pages_04000_infrastructure_console_io.html#did_infrastructure_console_io_diagrams", [
        [ "Sequence Diagram", "md_internal_docs_doxy_cfg_design_content_pages_04000_infrastructure_console_io.html#did_infrastructure_console_io_sequence_diagram", null ],
        [ "Component Interaction", "md_internal_docs_doxy_cfg_design_content_pages_04000_infrastructure_console_io.html#did_infrastructure_console_io_component_interaction", null ]
      ] ],
      [ "Resource usage", "md_internal_docs_doxy_cfg_design_content_pages_04000_infrastructure_console_io.html#did_infrastructure_console_io_resource_usage", null ],
      [ "Error handling", "md_internal_docs_doxy_cfg_design_content_pages_04000_infrastructure_console_io.html#did_infrastructure_console_io_error_handling", null ],
      [ "Interface", "md_internal_docs_doxy_cfg_design_content_pages_04000_infrastructure_console_io.html#did_infrastructure_console_io_interface", [
        [ "UART", "md_internal_docs_doxy_cfg_design_content_pages_04000_infrastructure_console_io.html#did_infrastructure_console_io_interface_uart", [
          [ "Data structures", "md_internal_docs_doxy_cfg_design_content_pages_04000_infrastructure_console_io.html#did_infrastructure_console_io_interface_uart_ds", null ],
          [ "Functions", "md_internal_docs_doxy_cfg_design_content_pages_04000_infrastructure_console_io.html#did_infrastructure_console_io_interface_uart_funcs", null ]
        ] ],
        [ "Log reader and writer", "md_internal_docs_doxy_cfg_design_content_pages_04000_infrastructure_console_io.html#did_infrastructure_console_io_interface_log_rd_wr", [
          [ "Data structures", "md_internal_docs_doxy_cfg_design_content_pages_04000_infrastructure_console_io.html#did_infrastructure_console_io_interface_log_rd_wr_ds", null ],
          [ "Functions", "md_internal_docs_doxy_cfg_design_content_pages_04000_infrastructure_console_io.html#did_infrastructure_console_io_interface_log_rd_wr_funcs", null ]
        ] ],
        [ "Command line interface", "md_internal_docs_doxy_cfg_design_content_pages_04000_infrastructure_console_io.html#did_infrastructure_console_io_interface_cli", [
          [ "Data structures", "md_internal_docs_doxy_cfg_design_content_pages_04000_infrastructure_console_io.html#did_infrastructure_console_io_interface_cli_ds", null ],
          [ "Functions", "md_internal_docs_doxy_cfg_design_content_pages_04000_infrastructure_console_io.html#did_infrastructure_console_io_interface_cli_funcs", null ]
        ] ],
        [ "Example API calls and CLI interaction", "md_internal_docs_doxy_cfg_design_content_pages_04000_infrastructure_console_io.html#did_infrastructure_console_io_interface_ex", [
          [ "Example init sequence", "md_internal_docs_doxy_cfg_design_content_pages_04000_infrastructure_console_io.html#did_infrastructure_console_io_interface_init_ex", [
            [ "Host CPU init and main", "md_internal_docs_doxy_cfg_design_content_pages_04000_infrastructure_console_io.html#did_infrastructure_console_io_interface_init_host_ex", null ]
          ] ],
          [ "Example CLI interaction", "md_internal_docs_doxy_cfg_design_content_pages_04000_infrastructure_console_io.html#did_infrastructure_console_io_interface_cli_ex", null ]
        ] ]
      ] ],
      [ "Design Analysis and Resolution (DAR)", "md_internal_docs_doxy_cfg_design_content_pages_04000_infrastructure_console_io.html#did_infrastructure_console_io_dar", [
        [ "Design Decision : Init Sequence", "md_internal_docs_doxy_cfg_design_content_pages_04000_infrastructure_console_io.html#did_infrastructure_console_io_dar_01", [
          [ "Design Criteria: Init Sequence", "md_internal_docs_doxy_cfg_design_content_pages_04000_infrastructure_console_io.html#did_infrastructure_console_io_dar_01_criteria", null ],
          [ "Design Alternative: Init sequence inside the module", "md_internal_docs_doxy_cfg_design_content_pages_04000_infrastructure_console_io.html#did_infrastructure_console_io_dar_01_alt_01", null ],
          [ "Design Alternative: Init sequence inside the application", "md_internal_docs_doxy_cfg_design_content_pages_04000_infrastructure_console_io.html#did_infrastructure_console_io_dar_01_alt_02", null ],
          [ "Final Decision", "md_internal_docs_doxy_cfg_design_content_pages_04000_infrastructure_console_io.html#did_infrastructure_console_io_dar_01_decision", null ]
        ] ]
      ] ]
    ] ],
    [ "Design Topic : Surround View", "md_internal_docs_doxy_cfg_design_content_pages_05000_app_linux_opengl_srv.html", [
      [ "Requirements Addressed", "md_internal_docs_doxy_cfg_design_content_pages_05000_app_linux_opengl_srv.html#did_app_srv_requirements", null ],
      [ "Introduction", "md_internal_docs_doxy_cfg_design_content_pages_05000_app_linux_opengl_srv.html#did_app_srv_intro", [
        [ "Purpose", "md_internal_docs_doxy_cfg_design_content_pages_05000_app_linux_opengl_srv.html#did_app_srv_purpose", null ],
        [ "Short Application Description", "md_internal_docs_doxy_cfg_design_content_pages_05000_app_linux_opengl_srv.html#did_app_srv_short_desc", null ],
        [ "Input and Output format", "md_internal_docs_doxy_cfg_design_content_pages_05000_app_linux_opengl_srv.html#did_app_srv_io_format", null ]
      ] ],
      [ "Directory Structure", "md_internal_docs_doxy_cfg_design_content_pages_05000_app_linux_opengl_srv.html#did_app_srv_dir_structure", null ],
      [ "Diagrams", "md_internal_docs_doxy_cfg_design_content_pages_05000_app_linux_opengl_srv.html#did_app_srv_diagrams", [
        [ "OpenVX Graph", "md_internal_docs_doxy_cfg_design_content_pages_05000_app_linux_opengl_srv.html#did_app_srv_openvx_graph", null ]
      ] ],
      [ "Resource usage", "md_internal_docs_doxy_cfg_design_content_pages_05000_app_linux_opengl_srv.html#did_app_srv_resource_usage", null ],
      [ "Error handling", "md_internal_docs_doxy_cfg_design_content_pages_05000_app_linux_opengl_srv.html#did_app_srv_error_handling", null ],
      [ "Interface", "md_internal_docs_doxy_cfg_design_content_pages_05000_app_linux_opengl_srv.html#did_app_srv_interface", null ],
      [ "Design Analysis and Resolution (DAR)", "md_internal_docs_doxy_cfg_design_content_pages_05000_app_linux_opengl_srv.html#did_app_srv_dar", [
        [ "Design Decision : xyz", "md_internal_docs_doxy_cfg_design_content_pages_05000_app_linux_opengl_srv.html#did_app_srv_dar_01", [
          [ "Design Criteria: xyz", "md_internal_docs_doxy_cfg_design_content_pages_05000_app_linux_opengl_srv.html#did_app_srv_dar_01_criteria", null ],
          [ "Design Alternative: xyz", "md_internal_docs_doxy_cfg_design_content_pages_05000_app_linux_opengl_srv.html#did_app_srv_dar_01_alt_01", null ],
          [ "Design Alternative: xyz", "md_internal_docs_doxy_cfg_design_content_pages_05000_app_linux_opengl_srv.html#did_app_srv_dar_01_alt_02", null ],
          [ "Final Decision", "md_internal_docs_doxy_cfg_design_content_pages_05000_app_linux_opengl_srv.html#did_app_srv_dar_01_decision", null ]
        ] ]
      ] ]
    ] ],
    [ "Design Topic : Design Topic Name", "md_internal_docs_doxy_cfg_design_content_pages_06000_design_topic_template.html", [
      [ "Requirements Addressed", "md_internal_docs_doxy_cfg_design_content_pages_06000_design_topic_template.html#did_des_topic_requirements", null ],
      [ "Introduction", "md_internal_docs_doxy_cfg_design_content_pages_06000_design_topic_template.html#did_des_topic_intro", [
        [ "Purpose", "md_internal_docs_doxy_cfg_design_content_pages_06000_design_topic_template.html#did_des_topic_purpose", null ],
        [ "Short Application Description", "md_internal_docs_doxy_cfg_design_content_pages_06000_design_topic_template.html#did_des_topic_short_desc", null ],
        [ "Input and Output format", "md_internal_docs_doxy_cfg_design_content_pages_06000_design_topic_template.html#did_des_topic_io_format", null ]
      ] ],
      [ "Directory Structure", "md_internal_docs_doxy_cfg_design_content_pages_06000_design_topic_template.html#did_des_topic_dir_structure", null ],
      [ "Diagrams", "md_internal_docs_doxy_cfg_design_content_pages_06000_design_topic_template.html#did_des_topic_diagrams", [
        [ "Sequence Diagram", "md_internal_docs_doxy_cfg_design_content_pages_06000_design_topic_template.html#did_des_topic_sequence_diagram", null ],
        [ "Component Interaction", "md_internal_docs_doxy_cfg_design_content_pages_06000_design_topic_template.html#did_des_topic_component_interaction", null ],
        [ "OpenVX Graph", "md_internal_docs_doxy_cfg_design_content_pages_06000_design_topic_template.html#did_des_topic_openvx_graph", null ]
      ] ],
      [ "Resource usage", "md_internal_docs_doxy_cfg_design_content_pages_06000_design_topic_template.html#did_des_topic_resource_usage", null ],
      [ "Error handling", "md_internal_docs_doxy_cfg_design_content_pages_06000_design_topic_template.html#did_des_topic_error_handling", null ],
      [ "Interface", "md_internal_docs_doxy_cfg_design_content_pages_06000_design_topic_template.html#did_des_topic_interface", null ],
      [ "Design Analysis and Resolution (DAR)", "md_internal_docs_doxy_cfg_design_content_pages_06000_design_topic_template.html#did_des_topic_dar", [
        [ "Design Decision : xyz", "md_internal_docs_doxy_cfg_design_content_pages_06000_design_topic_template.html#did_des_topic_dar_01", [
          [ "Design Criteria: xyz", "md_internal_docs_doxy_cfg_design_content_pages_06000_design_topic_template.html#did_des_topic_dar_01_criteria", null ],
          [ "Design Alternative: xyz", "md_internal_docs_doxy_cfg_design_content_pages_06000_design_topic_template.html#did_des_topic_dar_01_alt_01", null ],
          [ "Design Alternative: xyz", "md_internal_docs_doxy_cfg_design_content_pages_06000_design_topic_template.html#did_des_topic_dar_01_alt_02", null ],
          [ "Final Decision", "md_internal_docs_doxy_cfg_design_content_pages_06000_design_topic_template.html#did_des_topic_dar_01_decision", null ]
        ] ]
      ] ]
    ] ]
  ] ]
];

var NAVTREEINDEX =
[
"did_top_level_design.html"
];

var SYNCONMSG = 'click to disable panel synchronisation';
var SYNCOFFMSG = 'click to enable panel synchronisation';