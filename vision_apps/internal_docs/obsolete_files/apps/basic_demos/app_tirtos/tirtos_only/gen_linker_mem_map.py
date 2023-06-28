#!/usr/bin/env python3.6
#
# Copyright (c) 2018 Texas Instruments Incorporated
#
# All rights reserved not granted herein.
#
# Limited License.
#
# Texas Instruments Incorporated grants a world-wide, royalty-free, non-exclusive
# license under copyrights and patents it now or hereafter owns or controls to make,
# have made, use, import, offer to sell and sell ("Utilize") this software subject to the
# terms herein.  With respect to the foregoing patent license, such license is granted
# solely to the extent that any such patent is necessary to Utilize the software alone.
# The patent license shall not apply to any combinations which include this software,
# other than combinations with devices manufactured by or for TI ("TI Devices").
# No hardware patent is licensed hereunder.
#
# Redistributions must preserve existing copyright notices and reproduce this license
# (including the above copyright notice and the disclaimer and (if applicable) source
# code license limitations below) in the documentation and/or other materials provided
# with the distribution
#
# Redistribution and use in binary form, without modification, are permitted provided
# that the following conditions are met:
#
#       No reverse engineering, decompilation, or disassembly of this software is
# permitted with respect to any software provided in binary form.
#
#       any redistribution and use are licensed by TI for use only with TI Devices.
#
#       Nothing shall obligate TI to provide you with source code for the software
# licensed and provided to you in object code.
#
# If software source code is provided to you, modification and redistribution of the
# source code are permitted provided that the following conditions are met:
#
#       any redistribution and use of the source code, including any resulting derivative
# works, are licensed by TI for use only with TI Devices.
#
#       any redistribution and use of any object code compiled from the source code
# and any resulting derivative works, are licensed by TI for use only with TI Devices.
#
# Neither the name of Texas Instruments Incorporated nor the names of its suppliers
#
# may be used to endorse or promote products derived from this software without
# specific prior written permission.
#
# DISCLAIMER.
#
# THIS SOFTWARE IS PROVIDED BY TI AND TI'S LICENSORS "AS IS" AND ANY EXPRESS
# OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
# OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
# IN NO EVENT SHALL TI AND TI'S LICENSORS BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
# BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
# OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
# OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
# OF THE POSSIBILITY OF SUCH DAMAGE.
#
#

#
# This script is used to generate 'MEMORY' section for multiple CPUs
# in different CPU specific linker command files.
#
# This helps to define the memory map in one file vs having to manually
# keep the system memory map consistant across multiple CPUs
# in different linker commnd files.
#
# Make sure PyTI_PSDK_RTOS module is installed before running this script.
# See vision_apps/tools/PyTI_PSDK_RTOS/README.txt to install PyTI_PSDK_RTOS module.
#
# Edit this file to change the memory map
#
# Run this script by doing below,
# ./gen_linker_mem_map.py
#
# This will generate linker command file at below folders
# ./<cpu name>/linker_mem_map.cmd
#
# Here CPU name is mpu1, c66x_1, c66x_2, c7x_1, mcu1_0, mcu2_0, mcu3_0
#
#
from ti_psdk_rtos_tools import *

KB = 1024;
MB = KB*KB;
GB = KB*MB;

#
# Notes,
# - recommend to keep all memory segment sizes in units of KB at least
#

#
# DDR, MSMC base and size
#
ddr_mem_addr  = 0x80000000;
ddr_mem_size  = 1*GB;
msmc_mem_addr = 0x70000000;

#
# MSMC memory allocation for various CPUs
#
dmsc_msmc_size   = 64*KB;
mpu1_msmc_addr   = msmc_mem_addr;
mpu1_msmc_size   = 64*KB;
c7x_1_msmc_addr  = mpu1_msmc_addr + mpu1_msmc_size;
c7x_1_msmc_size  = 8*MB - mpu1_msmc_size - dmsc_msmc_size;


#
# C7x L1, L2 memory allocation
#
c7x_1_l2_addr  = 0x64800000;
c7x_1_l2_size  = 448*KB;
c7x_1_l1_addr  = 0x64E00000;
c7x_1_l1_size  = 16*KB;

#
# DDR memory allocation for various CPUs
#
mpu1_ddr_addr   = ddr_mem_addr;
mpu1_ddr_size   = (16 + 16 + 128)*MB;
c66x_1_ddr_addr = mpu1_ddr_addr   + mpu1_ddr_size;
c66x_1_ddr_size = 64*MB;
c66x_2_ddr_addr = c66x_1_ddr_addr + c66x_1_ddr_size;
c66x_2_ddr_size = 16*MB;
c7x_1_ddr_boot_addr  = c66x_2_ddr_addr + c66x_2_ddr_size;
c7x_1_ddr_boot_size = 64*KB
c7x_1_ddr_addr  = c7x_1_ddr_boot_addr + c7x_1_ddr_boot_size;
c7x_1_ddr_size  = 128*MB - c7x_1_ddr_boot_size;
mcu1_0_ddr_addr = c7x_1_ddr_addr  + c7x_1_ddr_size;
mcu1_0_ddr_size = 16*MB;
mcu1_1_ddr_addr = mcu1_0_ddr_addr + mcu1_0_ddr_size;
mcu1_1_ddr_size = 16*MB;
mcu2_0_ddr_addr = mcu1_1_ddr_addr + mcu1_1_ddr_size;
mcu2_0_ddr_size = 32*MB;
mcu2_1_ddr_addr = mcu2_0_ddr_addr + mcu2_0_ddr_size;
mcu2_1_ddr_addr = mcu2_1_ddr_addr
mcu2_1_ddr_size = 64*MB;
mcu3_0_ddr_addr = mcu2_1_ddr_addr + mcu2_1_ddr_size;
mcu3_0_ddr_size = 32*MB;
mcu3_1_ddr_addr = mcu3_0_ddr_addr + mcu3_0_ddr_size;
mcu3_1_ddr_size = 32*MB;

#
# DDR memory allocation for various shared memories
#
app_log_mem_addr        = mcu3_1_ddr_addr         + mcu3_1_ddr_size;
app_log_mem_size        = 256*KB;
tiovx_obj_desc_mem_addr = app_log_mem_addr        + app_log_mem_size;
tiovx_obj_desc_mem_size = 32*MB - app_log_mem_size;
ipc_vring_mem_addr      = tiovx_obj_desc_mem_addr + tiovx_obj_desc_mem_size;
ipc_vring_mem_size      = 32*MB;
ddr_shared_mem_addr     = ipc_vring_mem_addr      + ipc_vring_mem_size;
ddr_shared_mem_size     = ddr_mem_size - (ddr_shared_mem_addr - ddr_mem_addr);

#
# Create memory section based on addr and size defined above, including
# any CPU specific internal memories
#

# r5f local memory sections
r5f_tcma    = MemSection("R5F_TCMA" , "X"   , 0x00000000, 32*KB);
r5f_tcmb0   = MemSection("R5F_TCMB0", "RWIX", 0x41010000, 32*KB);

# MSMC memory sections
mpu1_msmc   = MemSection("MSMC_MPU1", "RWIX", mpu1_msmc_addr  , mpu1_msmc_size  , "MSMC for MPU1");
c7x_1_msmc  = MemSection("MSMC_C7x_1", "RWIX", c7x_1_msmc_addr  , c7x_1_msmc_size  , "MSMC for C7x_1");

# C7x L1/L2 memory sections
c7x_1_l2   = MemSection("L2RAM_C7x_1", "RWIX", c7x_1_l2_addr  , c7x_1_l2_size  , "L2 for C7x_1");
c7x_1_l1   = MemSection("L1RAM_C7x_1", "RWIX", c7x_1_l1_addr  , c7x_1_l1_size  , "L1 for C7x_1");

# CPU code/data memory sections in DDR
mpu1_ddr    = MemSection("DDR_MPU1", "RWIX", mpu1_ddr_addr  , mpu1_ddr_size  , "DDR for MPU1   for code/data");
c66x_1_ddr  = MemSection("DDR_C66x_1", "RWIX", c66x_1_ddr_addr, c66x_1_ddr_size, "DDR for C66x_1 for code/data");
c66x_2_ddr  = MemSection("DDR_C66x_2", "RWIX", c66x_2_ddr_addr, c66x_2_ddr_size, "DDR for C66x_2 for code/data");
c7x_1_ddr_boot = MemSection("DDR_C7x_1_BOOT", "RWIX", c7x_1_ddr_boot_addr , c7x_1_ddr_boot_size , "DDR for C7x_1  for boot code");
c7x_1_ddr   = MemSection("DDR_C7x_1", "RWIX", c7x_1_ddr_addr , c7x_1_ddr_size , "DDR for C7x_1  for code/data");
mcu1_0_ddr  = MemSection("DDR_MCU1_0", "RWIX", mcu1_0_ddr_addr, mcu1_0_ddr_size, "DDR for MCU1_0 for code/data");
mcu1_1_ddr  = MemSection("DDR_MCU1_1", "RWIX", mcu1_1_ddr_addr, mcu1_1_ddr_size, "DDR for MCU1_1 for code/data");
mcu2_0_ddr  = MemSection("DDR_MCU2_0", "RWIX", mcu2_0_ddr_addr, mcu2_0_ddr_size, "DDR for MCU2_0 for code/data");
mcu2_1_ddr  = MemSection("DDR_MCU2_1", "RWIX", mcu2_1_ddr_addr, mcu2_1_ddr_size, "DDR for MCU2_1 for code/data");
mcu3_0_ddr  = MemSection("DDR_MCU3_0", "RWIX", mcu3_0_ddr_addr, mcu3_0_ddr_size, "DDR for MCU3_0 for code/data");
mcu3_1_ddr  = MemSection("DDR_MCU3_1", "RWIX", mcu3_1_ddr_addr, mcu3_1_ddr_size, "DDR for MCU3_1 for code/data");

# Shared memory memory sections in DDR
app_log_mem        = MemSection("APP_LOG_MEM"       , "", app_log_mem_addr       , app_log_mem_size       , "Memory for remote core logging");
tiovx_obj_desc_mem = MemSection("TIOVX_OBJ_DESC_MEM", "", tiovx_obj_desc_mem_addr, tiovx_obj_desc_mem_size, "Memory for TI OpenVX shared memory. MUST be non-cached or cache-coherent");
ipc_vring_mem      = MemSection("IPC_VRING_MEM"     , "", ipc_vring_mem_addr     , ipc_vring_mem_size     , "Memory for IPC Vring's. MUST be non-cached or cache-coherent");
ddr_shared_mem     = MemSection("DDR_SHARED_MEM"    , "", ddr_shared_mem_addr    , ddr_shared_mem_size    , "Memory for shared memory buffers in DDR");

non_cache_mem      = MemSection("NON_CACHED_MEM"     , "", 0, 0, "Non-cached memory");
non_cache_mem.concat(app_log_mem);
non_cache_mem.concat(tiovx_obj_desc_mem);
non_cache_mem.concat(ipc_vring_mem);

#
# Create CPU specific memory maps using memory sections created above
#

mpu1_mmap = MemoryMap("mpu1");
mpu1_mmap.addMemSection( mpu1_msmc          );
mpu1_mmap.addMemSection( mpu1_ddr           );
mpu1_mmap.addMemSection( app_log_mem        );
mpu1_mmap.addMemSection( tiovx_obj_desc_mem );
mpu1_mmap.addMemSection( ipc_vring_mem      );
mpu1_mmap.addMemSection( ddr_shared_mem     );
mpu1_mmap.checkOverlap();

c66x_1_mmap = MemoryMap("c66x_1");
c66x_1_mmap.addMemSection( c66x_1_ddr         );
c66x_1_mmap.addMemSection( app_log_mem        );
c66x_1_mmap.addMemSection( tiovx_obj_desc_mem );
c66x_1_mmap.addMemSection( ipc_vring_mem      );
c66x_1_mmap.addMemSection( ddr_shared_mem     );
c66x_1_mmap.checkOverlap();

c66x_2_mmap = MemoryMap("c66x_2");
c66x_2_mmap.addMemSection( c66x_2_ddr         );
c66x_2_mmap.addMemSection( app_log_mem        );
c66x_2_mmap.addMemSection( tiovx_obj_desc_mem );
c66x_2_mmap.addMemSection( ipc_vring_mem      );
c66x_2_mmap.addMemSection( ddr_shared_mem     );
c66x_2_mmap.checkOverlap();

c7x_1_mmap = MemoryMap("c7x_1");
c7x_1_mmap.addMemSection( c7x_1_l2           );
c7x_1_mmap.addMemSection( c7x_1_l1           );
c7x_1_mmap.addMemSection( c7x_1_msmc         );
c7x_1_mmap.addMemSection( c7x_1_ddr_boot     );
c7x_1_mmap.addMemSection( c7x_1_ddr          );
c7x_1_mmap.addMemSection( app_log_mem        );
c7x_1_mmap.addMemSection( tiovx_obj_desc_mem );
c7x_1_mmap.addMemSection( ipc_vring_mem      );
c7x_1_mmap.addMemSection( ddr_shared_mem     );
c7x_1_mmap.checkOverlap();

mcu2_0_mmap = MemoryMap("mcu2_0");
mcu2_0_mmap.addMemSection( r5f_tcma           );
mcu2_0_mmap.addMemSection( r5f_tcmb0          );
mcu2_0_mmap.addMemSection( mcu2_0_ddr         );
mcu2_0_mmap.checkOverlap();

mcu2_1_mmap = MemoryMap("mcu2_1");
mcu2_1_mmap.addMemSection( r5f_tcma           );
mcu2_1_mmap.addMemSection( r5f_tcmb0          );
mcu2_1_mmap.addMemSection( mcu2_1_ddr         );
mcu2_1_mmap.addMemSection( app_log_mem        );
mcu2_1_mmap.addMemSection( tiovx_obj_desc_mem );
mcu2_1_mmap.addMemSection( ipc_vring_mem      );
mcu2_1_mmap.addMemSection( ddr_shared_mem     );
mcu2_1_mmap.checkOverlap();


sys_mmap = MemoryMap("System Memory Map for RTOS only mode");
sys_mmap.addMemSection( c7x_1_l2           );
sys_mmap.addMemSection( c7x_1_l1           );
sys_mmap.addMemSection( mpu1_msmc          );
sys_mmap.addMemSection( c7x_1_msmc         );
sys_mmap.addMemSection( mpu1_ddr           );
sys_mmap.addMemSection( c66x_1_ddr         );
sys_mmap.addMemSection( c66x_2_ddr         );
sys_mmap.addMemSection( c7x_1_ddr_boot     );
sys_mmap.addMemSection( c7x_1_ddr          );
sys_mmap.addMemSection( mcu1_0_ddr         );
sys_mmap.addMemSection( mcu1_1_ddr         );
sys_mmap.addMemSection( mcu2_0_ddr         );
sys_mmap.addMemSection( mcu2_1_ddr         );
sys_mmap.addMemSection( mcu3_0_ddr         );
sys_mmap.addMemSection( mcu3_1_ddr         );
sys_mmap.addMemSection( app_log_mem        );
sys_mmap.addMemSection( tiovx_obj_desc_mem );
sys_mmap.addMemSection( ipc_vring_mem      );
sys_mmap.addMemSection( ddr_shared_mem     );
sys_mmap.checkOverlap();

c_header_mmap = MemoryMap("Memory Map for C header file");
c_header_mmap.addMemSection( non_cache_mem     );
c_header_mmap.addMemSection( mpu1_msmc );
c_header_mmap.addMemSection( mpu1_ddr   );
c_header_mmap.addMemSection( c7x_1_msmc );
c_header_mmap.addMemSection( c7x_1_ddr_boot   );
c_header_mmap.addMemSection( c7x_1_ddr   );
c_header_mmap.addMemSection( mcu2_1_ddr   );
c_header_mmap.addMemSection( c66x_1_ddr );
c_header_mmap.addMemSection( c66x_2_ddr );
c_header_mmap.addMemSection( ddr_shared_mem );
c_header_mmap.checkOverlap();

#
# Generate linker command files containing "MEMORY" definitions
#
LinkerCmdFile(mpu1_mmap  , "./mpu1/linker_mem_map.cmd"  ).export();
LinkerCmdFile(c66x_1_mmap, "./c66x_1/linker_mem_map.cmd").export();
LinkerCmdFile(c66x_2_mmap, "./c66x_2/linker_mem_map.cmd").export();
LinkerCmdFile(c7x_1_mmap , "./c7x_1/linker_mem_map.cmd" ).export();
LinkerCmdFile(mcu2_0_mmap, "./mcu2_0/linker_mem_map.cmd").export();
LinkerCmdFile(mcu2_1_mmap, "./mcu2_1/linker_mem_map.cmd").export();

CHeaderFile(c_header_mmap, "./app_mem_map.h").export();

HtmlMmapTable(sys_mmap, "./system_memory_map.html").export();
