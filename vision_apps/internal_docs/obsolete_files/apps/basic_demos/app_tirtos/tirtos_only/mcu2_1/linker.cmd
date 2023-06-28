/* linker options */
--fill_value=0
--stack_size=0x2000
--heap_size=0x1000

-e __VECS_ENTRY_POINT

SECTIONS
{
    .vecs       : {
        __VECS_ENTRY_POINT = .;
    } palign(8) > R5F_TCMA
    .text_boot {
        *boot.aer5f<*boot.o*>(.text)
     }  palign(8)   > R5F_TCMB0
    .text:xdc_runtime_Startup_reset__I     : {} palign(8) > R5F_TCMB0
    .text:ti_sysbios_family_arm_v7r_Cache* : {} palign(8) > R5F_TCMB0
    .text:ti_sysbios_family_arm_MPU*       : {} palign(8) > R5F_TCMB0

    .text       : {} palign(8)   > DDR_MCU2_1
    .cinit      : {} palign(8)   > DDR_MCU2_1
    .bss        : {} align(8)    > DDR_MCU2_1
    .const      : {} palign(8)   > DDR_MCU2_1
    .data       : {} palign(128) > DDR_MCU2_1
    .sysmem     : {} align(8)    > DDR_MCU2_1
    .stack      : {} align(4)    > DDR_MCU2_1
    .bss:taskStackSection > DDR_MCU2_1

    .bss:ddr_shared_mem     (NOLOAD) : {} > DDR_MCU2_1
    .bss:app_log_mem        (NOLOAD) : {} > APP_LOG_MEM
    .bss:tiovx_obj_desc_mem (NOLOAD) : {} > TIOVX_OBJ_DESC_MEM
    .bss:ipc_vring_mem      (NOLOAD) : {} > IPC_VRING_MEM

}
