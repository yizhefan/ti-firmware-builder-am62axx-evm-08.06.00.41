# Vision Apps Datasheet {#mainpage}

-# \subpage basic_demos_datasheets
-# \subpage dl_demos_datasheets
-# \subpage cv_demos_datasheets
-# \subpage srv_demos_datasheets

# Legend

Each of the datasheets in this document are automatically generated at run-time using performance and memory statistics
collection libraries running across the SoC.  As such, each core may have different memories that it considers L3_MEM heap.
The following table can be used to identify which physical memory is being referred to by L3_MEM heap for each core.

Core                         | L3_MEM Location
-----------------------------|----------------------
mpux_y (R5F cores)           | OCRAM (512 KB total) (TRM calls it MSRAM16KX256E0_RAM)
\if DOCS_J721E
c7x_1                        | MSMC (8MB total)
\endif
\if DOCS_J721S2
c7x_1                        | MSMC (4MB total)
\endif
\if DOCS_J784S4
c7x_1                        | MSMC (3MB total)
c7x_2                        | MSMC (3MB total)
c7x_3                        | MSMC (3MB total)
c7x_4                        | MSMC (3MB total)
\endif
