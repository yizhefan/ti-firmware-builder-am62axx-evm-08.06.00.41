/******************************************************************************
Automatically generated parser for DCC data
Generated by DCC generator ver 0.1.10
for dcc_descriptor_id_type = UNKNOWN
vendor ID 1
******************************************************************************/

#include "fw_to_dcc.h"
#include "mctnf_config_dcc.h"


static uint8 get_uint8(uint8** p_bin);
#define get_int8(a)  (int8)get_uint8(a)

static uint16 get_uint16(uint8** p_bin);
#define get_int16(a)  (int16)get_uint16(a)

static uint32 get_uint32(uint8** p_bin);
#define get_int32(a)  (int32)get_uint32(a)


/* ============================================================================ 
* dcc_bin_parse()
* 
* @param b_sys_prm : pointer to binary data with system parameters
* @param b_uc_prm  : pointer to binary data with use case parameters
* @param b_parpack : pointer to binary data with parameter packet
* @param sys_prm : pointer output structure with system parameters
* @param uc_prm  : pointer output structure with use case parameters
* @param parpack : pointer output structure with parameter packet
* @return : 0 if success, !0 if error
* ========================================================================== */
int mctnf_config_dcc_dcc_bin_parse(uint8* b_sys_prm, uint8* b_uc_prm, uint8* b_parpack,
                  void* sys_prm, void* uc_prm, void* parpack,
                  uint32 crc)
{
    uint8* p_bin;
    if(crc != 0xE3ED4D50){
        return (-1);//Use this error code to detect different CRC
    }

    //parse parameter packets
    if(parpack){
        mctnf_cfg_dcc_descriptor_t* parpack_p = (mctnf_cfg_dcc_descriptor_t*)parpack;

        p_bin = b_parpack;
        parpack_p->update = get_uint8(&p_bin);
        parpack_p->enable = get_uint8(&p_bin);
        parpack_p->blending_factor_q4 = get_uint8(&p_bin);
        parpack_p->min_blend_q4 = get_uint8(&p_bin);
        parpack_p->max_blend_q4 = get_uint8(&p_bin);
        parpack_p->me_lambda_type = get_uint8(&p_bin);
        parpack_p->me_lambda_factor_q2 = get_uint8(&p_bin);
        parpack_p->max_lambda_q2 = get_uint8(&p_bin);
        parpack_p->sad_for_min_lambda = get_uint16(&p_bin);
        parpack_p->fix_wt_cur_q8 = get_uint16(&p_bin);
        parpack_p->bias_zero_motion = get_uint8(&p_bin);
        parpack_p->static_mb_th_zero_mv = get_uint8(&p_bin);
        parpack_p->static_mb_th_non_zero_mv = get_uint8(&p_bin);
        parpack_p->blockiness_rem_factor = get_uint8(&p_bin);
        parpack_p->me_effectiveness_th = get_uint16(&p_bin);
        parpack_p->min_wt_cur_q8 = get_uint8(&p_bin);
        parpack_p->sad_max_strength = get_uint16(&p_bin);
        parpack_p->reserved_param1 = get_uint16(&p_bin);
        parpack_p->reserved_param2 = get_uint16(&p_bin);
        parpack_p->reserved_param3 = get_uint16(&p_bin);
    }
    return 0;
}
/* ============================================================================ 
* dcc_bin_free()
*
* Frees any memory that was allocated in dcc_bin_parse()
* 
* @param sys_prm : pointer output structure with system parameters
* @param uc_prm  : pointer output structure with use case parameters
* @param parpack : pointer output structure with parameter packet
* @return : 0 if success, !0 if error
* ========================================================================== */
void mctnf_config_dcc_dcc_bin_free(void* sys_prm, void* uc_prm, void* parpack)
{

}

/* ============================================================================ 
* get_uint8(),get_int8(),get_uint16(),get_int16(),get_uint32(),get_int32()
* 
* reads from binary buffer, assembles 1B,2B and 4B data and updates the buffer
* pointer
* ========================================================================== */
static uint8 get_uint8(uint8** p_bin){
    uint8 byte;
    uint8 *p = *p_bin;
    byte = *p++;
    *p_bin = p;
    return byte;
}

static uint16 get_uint16(uint8** p_bin){
    uint16 byte2;
    uint8 *p = *p_bin;
    byte2 = *p++;
    byte2 |= (uint16)(*p++) << 8;
    *p_bin = p;
    return byte2;
}
