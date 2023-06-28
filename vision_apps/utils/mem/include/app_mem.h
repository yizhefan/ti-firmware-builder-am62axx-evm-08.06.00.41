/*
 *
 * Copyright (c) 2017 Texas Instruments Incorporated
 *
 * All rights reserved not granted herein.
 *
 * Limited License.
 *
 * Texas Instruments Incorporated grants a world-wide, royalty-free, non-exclusive
 * license under copyrights and patents it now or hereafter owns or controls to make,
 * have made, use, import, offer to sell and sell ("Utilize") this software subject to the
 * terms herein.  With respect to the foregoing patent license, such license is granted
 * solely to the extent that any such patent is necessary to Utilize the software alone.
 * The patent license shall not apply to any combinations which include this software,
 * other than combinations with devices manufactured by or for TI ("TI Devices").
 * No hardware patent is licensed hereunder.
 *
 * Redistributions must preserve existing copyright notices and reproduce this license
 * (including the above copyright notice and the disclaimer and (if applicable) source
 * code license limitations below) in the documentation and/or other materials provided
 * with the distribution
 *
 * Redistribution and use in binary form, without modification, are permitted provided
 * that the following conditions are met:
 *
 * *       No reverse engineering, decompilation, or disassembly of this software is
 * permitted with respect to any software provided in binary form.
 *
 * *       any redistribution and use are licensed by TI for use only with TI Devices.
 *
 * *       Nothing shall obligate TI to provide you with source code for the software
 * licensed and provided to you in object code.
 *
 * If software source code is provided to you, modification and redistribution of the
 * source code are permitted provided that the following conditions are met:
 *
 * *       any redistribution and use of the source code, including any resulting derivative
 * works, are licensed by TI for use only with TI Devices.
 *
 * *       any redistribution and use of any object code compiled from the source code
 * and any resulting derivative works, are licensed by TI for use only with TI Devices.
 *
 * Neither the name of Texas Instruments Incorporated nor the names of its suppliers
 *
 * may be used to endorse or promote products derived from this software without
 * specific prior written permission.
 *
 * DISCLAIMER.
 *
 * THIS SOFTWARE IS PROVIDED BY TI AND TI'S LICENSORS "AS IS" AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL TI AND TI'S LICENSORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef APP_MEM_H_
#define APP_MEM_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \defgroup group_vision_apps_utils_mem Memory allocation APIs
 *
 * \brief This section contains APIs for memory allocation
 *
 * \ingroup group_vision_apps_utils
 *
 * @{
 */

/** \brief Max characters to use for heap name */
#define APP_MEM_HEAP_NAME_MAX   (16u)

/**
 *  \anchor APP_MEM_HEAP
 *  \name Memory heap ID
 *
 *  @{
 */

/** \brief Heap located in DDR */
#define APP_MEM_HEAP_DDR        (0u)

/** \brief Heap located in L3 memory (MSMC) */
#define APP_MEM_HEAP_L3         (1u)

/** \brief Heap located in L2 local memory of a CPU */
#define APP_MEM_HEAP_L2         (2u)

/** \brief Heap located in L1 local memory of a CPU */
#define APP_MEM_HEAP_L1         (3u)

/** \brief Heap located in DDR and is used as scratch */
#define APP_MEM_HEAP_DDR_SCRATCH (4u)

/** \brief Heap located in DDR and in non-cache memory of persistent property */
#define APP_MEM_HEAP_DDR_NON_CACHE  (5u)

/** \brief Heap located in DDR and in non-cache memory of scratch property */
#define APP_MEM_HEAP_DDR_NON_CACHE_SCRATCH  (6u)

/** \brief Max heaps in system */
#define APP_MEM_HEAP_MAX        (7u)

/* @} */

/**
 *  \anchor APP_MEM_HEAP_FLAGS
 *  \name Memory heap creation flags
 *
 *  @{
 */

/** \brief Heap flag to indicate heap is of type "linear allocater"
 *
 * Here alloc increments a offset by alloc size and free resets
 * the offset. This is used typically for L2 memory to allocate it
 * as scratch among multiple algorithms on DSP.
 *
 * When this flag is not set heap type is normal dynamic memory allocator heap
 */
#define APP_MEM_HEAP_FLAGS_TYPE_LINEAR_ALLOCATE     (0x00000001u)

/** \brief Heap flag to indicate if memory that is allcoate will be shared with another CPU
 */
#define APP_MEM_HEAP_FLAGS_IS_SHARED                (0x00000004u)

/** \brief Heap flag to indicate if memory should be set to 0 after alloc
 */
#define APP_MEM_HEAP_FLAGS_DO_CLEAR_ON_ALLOC        (0x00000008u)

/* @} */

/**
 * \brief Heap initialization parameters
 */
typedef struct {

    void    *base;                   /**< heap memory base address */
    char    name[APP_MEM_HEAP_NAME_MAX];  /**< heap name */
    uint32_t size;                   /**< heap size in bytes */
    uint32_t flags;                  /**< flags, see \ref APP_MEM_HEAP_FLAGS */

} app_mem_heap_prm_t;

/**
 * \brief Memory module initialization parameters
 */
typedef struct {

    app_mem_heap_prm_t heap_info[APP_MEM_HEAP_MAX]; /**< heap init parameters */
} app_mem_init_prm_t;


/**
 * \brief Heap statistics and information
 */
typedef struct {

    uint32_t heap_id;                       /**< Heap ID, see \ref APP_MEM_HEAP */
    char heap_name[APP_MEM_HEAP_NAME_MAX];  /**< Heap name */
    uint32_t heap_size;                     /**< Heap size in bytes */
    uint32_t free_size;                     /**< Free space in bytes */

} app_mem_stats_t;

/**
 * \brief Align ptr value to 'align' bytes
 */
static inline void *APP_MEM_ALIGNPTR(void *val, uint32_t align)
{
    return (void*)((((uintptr_t)val+align-1) / align) * align);
}

/**
 * \brief Align 64b value to 'align' bytes
 */
static inline uint64_t APP_MEM_ALIGN64(uint64_t val, uint32_t align)
{
    return (uint64_t)( (uint64_t)(val+align-1) / align) * align;
}

/**
 * \brief Align 32b value to 'align' bytes
 */
static inline uint32_t APP_MEM_ALIGN32(uint32_t val, uint32_t align)
{
    return (uint32_t)( (uint32_t)(val+align-1) / align) * align;
}

#if defined(C7120) && defined(SOC_J784S4)

/**
 * \brief J784S4 Set L2 Write Back Invalidate
 */
void appMemC7xSetL2WBINV(uint64_t param);

/**
 * \brief J784S4 Get L2 Write Back Invalidate
 */
uint64_t appMemC7xGetL2WBINV(void);

/**
 * \brief J784S4 Invalidating entire L2 Cache
 */
void appMemC7xCleaninvalidateL2Cache();

/**
 * \brief J784S4 Set L1D Write Back Invalidate
 */
void appMemC7xSetL1DWBINV(uint64_t param);

/**
 * \brief J784S4 Get L1D Write Back Invalidate
 */
uint64_t appMemC7xGetL1DWBINV(void);

/**
 * \brief J784S4 Invalidating entire L1D Cache
 */
void appMemC7xCleaninvalidateL1DCache();
#endif

/**
 * \brief Enable L1D and L2 Cache by performing a WB
 *
 * Applicable only for J784S4 SOC due to
 * coherency difference on these SOC's
 */
void static inline appMemEnableL1DandL2CacheWb()
{
#if defined(C7120) && defined(SOC_J784S4)
  appMemC7xCleaninvalidateL1DCache();
  appMemC7xCleaninvalidateL2Cache();
#endif
}

/**
 * \brief Set defaults to app_mem_init_prm_t
 *
 * Recommend to be called before calling appMemInit()
 * Override default with user specific parameters
 *
 * By default all heaps are disabled.
 *
 * \param prm [out] Default initialized parameters
 */
void    appMemInitPrmSetDefault(app_mem_init_prm_t *prm);

/**
 * \brief Init heaps for memory allocation
 *
 * \param prm [in] Initialization parameters
 *
 * \return 0 on success else failure
 */
int32_t appMemInit(app_mem_init_prm_t *prm);

/**
 * \brief De-Init heaps for memory allocation
 *
 * \return 0 on success else failure
 */
int32_t appMemDeInit();

/**
 * \brief Alloc memory from specific heap
 *
 * \param heap_id [in] See APP_MEM_HEAP_*
 * \                   Incase of Linux CMEM, set CMEM block ID
 * \param size    [in] Size in bytes to allocate
 * \param align   [in] Minimum alignment requested
 *
 * \return pointer to memory or NULL in case of failure
 */
void    *appMemAlloc(uint32_t heap_id, uint32_t size, uint32_t align);


/**
 * \brief Reset scratch memory
 *
 * \param heap_id [in] See APP_MEM_HEAP_*
 * \                   Incase of Linux CMEM, set CMEM block ID
 *
 * \return 0 on success else failure
 */
int32_t appMemResetScratchHeap(uint32_t heap_id);

/**
 * \brief Free memory that was previously allocated
 *
 * \param heap_id [in] See APP_MEM_HEAP_*
 * \                   Incase of Linux CMEM, set CMEM block ID
 * \param ptr     [in] pointer to allocated memory
 * \param size    [in] size of allocated memory
 *
 * \return 0 on success else failure
 */
int32_t appMemFree(uint32_t heap_id, void *ptr, uint32_t size);

/**
 * \brief Return dmaBufFd of a CMEM buffer pointer
 *
 * \param virPtr  [in] pointer to allocated memory
 * \param dmaBufFdOffset  [out] offset within the DMA BUF FD where the data is located
 *
 * \return dmaBufFd or NULL in case of failure
 */
#if defined(x86_64) || defined(QNX)
uint64_t appMemGetDmaBufFd(void *virPtr, volatile uint32_t *dmaBufFdOffset);
#else
uint32_t appMemGetDmaBufFd(void *virPtr, volatile uint32_t *dmaBufFdOffset);
#endif

/**
 * \brief Translates a given 'dmaBufFd' to virtual and physical addresses.
 *
 * \param dmaBufFd  [in] DMA Buffer Id to translate.
 * \param size     [in] Size of buffer in bytes
 * \param virtPtr  [out] Virtual address of the memory represented by 'dmaBufFd'.
 * \param phyPtr  [out] Physical address of the memory represented by 'dmaBufFd'.
 *
 * \return 0, if the translation is successful or < 0 otherwise.
 */
#if defined(x86_64) || defined(QNX)
int32_t appMemTranslateDmaBufFd(uint64_t dmaBufFd, uint32_t size, uint64_t *virtPtr, uint64_t *phyPtr);
#else
int32_t appMemTranslateDmaBufFd(uint32_t dmaBufFd, uint32_t size, uint64_t *virtPtr, uint64_t *phyPtr);
#endif

/**
 * \brief Close the dmaBufFd of a CMEM buffer pointer
 *
 * \param dmaBufFd  [in] value of dmaBufFd
 */
#if defined(x86_64) || defined(QNX)
void appMemCloseDmaBufFd(uint64_t dmaBufFd);
#else
void appMemCloseDmaBufFd(int32_t dmaBufFd);
#endif

/**
 * \brief Return physical ptr corresponds to a CMEM virtual ptr
 *
 * \param virtPtr  [in] pointer to allocated memory
 * \param heap_id [in] heap ID
 *
 * \return Physical pointer to memory or NULL in case of failure
 */
uint64_t appMemGetVirt2PhyBufPtr(uint64_t virtPtr, uint32_t heap_id);

/**
 * \brief Return heap statistics and information
 *
 * \param heap_id [in] See APP_MEM_HEAP_*
 * \                   Incase of Linux CMEM, set CMEM block ID
 * \param stats   [in] Heap statistics and information
 *
 * \return 0 on success else failure
 */
int32_t appMemStats(uint32_t heap_id, app_mem_stats_t *stats);

/**
 * \brief Invalidate a section of memory from cache
 *
 * \param ptr [in] Memory address for cache ops
 * \param size [in] Size of memory to perform the cache ops
 *
 */
void  appMemCacheInv(void *ptr, uint32_t size);

/**
 * \brief Write back a section of memory from cache
 *
 * \param ptr [in] Memory address for cache ops
 * \param size [in] Size of memory to perform the cache ops
 *
 */
void  appMemCacheWb(void *ptr, uint32_t size);



/**
 * \brief Write back and invalidate a section of memory from cache
 *
 * \param ptr [in] Memory address for cache ops
 * \param size [in] Size of memory to perform the cache ops
 *
 */
void  appMemCacheWbInv(void *ptr, uint32_t size);

/**
 * \brief Print memory allocation from shared memory region
 *
 */
void appMemPrintMemAllocInfo();

/* @} */

#ifdef __cplusplus
}
#endif

#endif

