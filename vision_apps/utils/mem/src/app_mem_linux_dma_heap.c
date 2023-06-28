/*
 *
 * Copyright (c) 2018 Texas Instruments Incorporated
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
/*INCLUDES*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>

#include <utils/mem/include/linux/dma_buf_phys.h>
#include <utils/mem/include/app_mem.h>
#include <linux/dma-heap.h>
#include <linux/dma-buf.h>
#include <app_mem_map.h>

/*MACROS*/
/* #define APP_MEM_DEBUG */
#ifdef SOC_AM62A
#define DMA_HEAP_NAME          "/dev/dma_heap/edgeai_shared-memories"
#else
#define DMA_HEAP_NAME          "/dev/dma_heap/vision_apps_shared-memories"
#endif
#define DMA_HEAP_ALLOC_FLAGS   (0u)

/*STRUCTURES*/

typedef struct
{
    uint32_t dma_buf_fd;
    int32_t  dmabuf2phys_fd;
    uint64_t phys_addr;
    uint64_t virt_addr;
    uint64_t offset;
    uint32_t size;
}app_mem_type_t;

typedef struct mem_type_node
{
    app_mem_type_t mem_data;
    struct mem_type_node *next;
} app_mem_list_t;

typedef struct
{
    app_mem_list_t *plist;
    pthread_mutex_t list_mutex;
    int dma_heap_fd;
    uint32_t app_heap_id;

    uint64_t total_alloc_bytes;
    uint64_t total_free_bytes;
    uint64_t cur_alloc_bytes;
    uint64_t num_alloc;
    uint64_t num_free;
    uint64_t cur_alloc;

}app_mem_obj_t;

/*GLOBALS*/
app_mem_obj_t g_app_mem_obj;

/*INTERNAL FUNCTION DEFINITIONS*/
void appMemRemoveFromList(app_mem_list_t **list, app_mem_list_t *node);
int  appMemPushToList(app_mem_list_t **list, app_mem_type_t *data);
int  appMemDmaHeapGetPhys(int phys_fd, int fd, uint64_t *phys);
void appMemFreeList(app_mem_list_t *list);
static int32_t appMemAddTupleToList(uint32_t dmaBufFd, uint32_t size, uint64_t *virtPtr, uint64_t *phyPtr);

int32_t appMemInit(app_mem_init_prm_t *prm)
{
    int32_t status = 0;

    /* Initialize our book-keeping structures */
    app_mem_obj_t *obj = &g_app_mem_obj;

    obj->total_alloc_bytes = 0;
    obj->total_free_bytes = 0;
    obj->cur_alloc_bytes = 0;
    obj->num_alloc = 0;
    obj->num_free = 0;
    obj->cur_alloc = 0;

    obj->plist = NULL;
    obj->dma_heap_fd = -1;
    obj->app_heap_id = -1;

    /* Initialize the pthread mutex */
    pthread_mutex_init(&obj->list_mutex, NULL);

    printf("MEM: Init ... !!!\n");

    /* Initialize the DMA HEAP module */
    obj->dma_heap_fd = open(DMA_HEAP_NAME, O_RDONLY | O_CLOEXEC);
    if (obj->dma_heap_fd < 0)
    {
        printf("MEM: ERROR: Failed to initialize DMA HEAP [%s] !!!\n", DMA_HEAP_NAME);
        status = -1;
    }
    else
    {
        printf("MEM: Initialized DMA HEAP (fd=%d) !!!\n", obj->dma_heap_fd);
    }

    printf("MEM: Init ... Done !!!\n");

    return(status);
}

void appMemPrintMemAllocInfo()
{
    app_mem_obj_t *obj = &g_app_mem_obj;

    printf("DDR_SHARED_MEM: Alloc's: %ld alloc's of %ld bytes \n", obj->num_alloc, obj->total_alloc_bytes );
    printf("DDR_SHARED_MEM: Free's : %ld free's  of %ld bytes \n", obj->num_free, obj->total_free_bytes );
    printf("DDR_SHARED_MEM: Open's : %ld allocs  of %ld bytes \n", obj->cur_alloc, obj->cur_alloc_bytes );
    printf("DDR_SHARED_MEM: Total size: %d bytes \n", DDR_SHARED_MEM_SIZE );
}

int32_t appMemDeInit(void)
{
    int32_t status = 0;

    app_mem_obj_t *obj = &g_app_mem_obj;

    printf("MEM: Deinit ... !!!\n");

    pthread_mutex_lock(&obj->list_mutex);

    /* Clear the heap off our memory map list */
    appMemFreeList(obj->plist);

    appMemPrintMemAllocInfo();

    pthread_mutex_unlock(&obj->list_mutex);

    /* Deinitialize the DMA HEAP module */
    if (close(obj->dma_heap_fd) < 0)
    {
        printf("MEM: ERROR: Failed to deinit the DMA HEAP module !!!\n");
        status = -1;
    }

    printf("MEM: Deinit ... Done !!!\n");

    return(status);
}

uint32_t appMemGetDmaBufFd(void *virPtr, volatile uint32_t *dmaBufFdOffset)
{
    app_mem_obj_t *obj = &g_app_mem_obj;

    int dmaBufFd = -1;
    uint64_t virt_addr = (uint64_t)(virPtr);

    *dmaBufFdOffset = 0;

    app_mem_list_t *mem_map_list;

    /* Enter critical section */
    pthread_mutex_lock(&obj->list_mutex);
    mem_map_list = obj->plist;
    while(mem_map_list != NULL)
    {
        if( (virt_addr >= mem_map_list->mem_data.virt_addr)
            && (virt_addr < (mem_map_list->mem_data.virt_addr+mem_map_list->mem_data.size))
            )
        {
            uint32_t offset = (virt_addr - mem_map_list->mem_data.virt_addr);

            dmaBufFd = mem_map_list->mem_data.dma_buf_fd;
            *dmaBufFdOffset = offset;
            break;
        }
        mem_map_list = mem_map_list->next;
    }
    /* Exit critical section */
    pthread_mutex_unlock(&obj->list_mutex);
    #ifdef APP_MEM_DEBUG
    if (dmaBufFd < 0)
    {
        printf("MEM: ERROR: Failed to export dmaBufFd for virtPtr %p !!!\n", virPtr);
    }
    printf("MEM: Exported dmaBufFd %d @ offset = %d bytes !!!\n", dmaBufFd, *dmaBufFdOffset);
    #endif

    return (uint32_t)(dmaBufFd);
}

int32_t appMemTranslateDmaBufFd(uint32_t dmaBufFd, uint32_t size, uint64_t *virtPtr, uint64_t *phyPtr)
{
    app_mem_obj_t  *obj = &g_app_mem_obj;
    app_mem_list_t *mem_map_list;
    int32_t         status = 0;

    *virtPtr = 0;
    *phyPtr  = 0;

    /* Enter critical section */
    pthread_mutex_lock(&obj->list_mutex);
    mem_map_list = obj->plist;
    while(mem_map_list != NULL)
    {
        if (dmaBufFd == mem_map_list->mem_data.dma_buf_fd)
        {
            app_mem_type_t *mem_data = &mem_map_list->mem_data;
            uint32_t offset = mem_data->offset;

            *virtPtr = mem_data->virt_addr + offset;
            *phyPtr  = mem_data->phys_addr + offset;
            break;
        }

        mem_map_list = mem_map_list->next;
    }

    /* Exit critical section */
    pthread_mutex_unlock(&obj->list_mutex);

    if (*virtPtr == 0)
    {
        /* We do not have the record of the 'dmaBufFd' in the list.
         * Create a record, translate it to 'virtPtr, 'phyPtr' and add a
         * record for later use.
         */
        status = appMemAddTupleToList(dmaBufFd, size, virtPtr, phyPtr);
    }

    if ((*virtPtr == 0) || (*phyPtr == 0))
    {
        printf("MEM: ERROR: Failed to translate dmaBufFd [%d]\n", dmaBufFd);
        status = -1;
    }

    #ifdef APP_MEM_DEBUG
    printf("MEM: Translated dmaBufFd %d to virtPtr %p and phyPtr %p!!!\n",
           dmaBufFd, (void*)*virtPtr, (void*)*phyPtr);
    #endif

    return status;
}

/* Delete the table entry when someone closes the dma buf fd */
void appMemCloseDmaBufFd(int32_t dmaBufFd)
{
    /* This is done in appMemFree */
}

int appMemDmaHeapAlloc(int dma_heap_fd, size_t len, unsigned int flags, int *dma_buf_fd)
{
    if (dma_buf_fd == NULL)
        return -EINVAL;

    struct dma_heap_allocation_data data = {
        .len = len,
        .fd_flags = O_CLOEXEC | O_RDWR,
        .heap_flags = flags,
    };

    *dma_buf_fd = -1;

    int ret = ioctl(dma_heap_fd, DMA_HEAP_IOCTL_ALLOC, &data);
    if (ret < 0)
        return errno;

    *dma_buf_fd = data.fd;

    return 0;
}

uint64_t appMemGetVirt2PhyBufPtr(uint64_t virtPtr, uint32_t heap_id)
{
    off_t phyPtr = 0;
    app_mem_obj_t *obj = &g_app_mem_obj;
    /* Find out the physical address of this buffer */

    app_mem_list_t *mem_map_list;

    uint64_t virt_addr = (uint64_t)(virtPtr);

    /* Enter critical section */
    pthread_mutex_lock(&obj->list_mutex);
    mem_map_list = obj->plist;
    while(mem_map_list != NULL)
    {
        if( (virt_addr >= mem_map_list->mem_data.virt_addr)
            && (virt_addr < (mem_map_list->mem_data.virt_addr+mem_map_list->mem_data.size))
            )
        {
            uint32_t offset = (virt_addr - mem_map_list->mem_data.virt_addr);

            phyPtr = (mem_map_list->mem_data.phys_addr + offset);
            break;
        }
        mem_map_list = mem_map_list->next;
    }
    /* Exit critical section */
    pthread_mutex_unlock(&obj->list_mutex);
    #ifdef APP_MEM_DEBUG
    if (phyPtr == 0)
    {
        printf("MEM: ERROR: Failed to get physical address of virt addr = %p !!!\n", (void*)virtPtr);
    }
    printf("MEM: Translated virt addr = %p -> phy addr = %lx !!!\n", (void*)virtPtr, phyPtr);
    #endif

    return (uint64_t)(phyPtr);
}

void *appMemAlloc(uint32_t block, uint32_t size, uint32_t align)
{
    app_mem_obj_t  *obj = &g_app_mem_obj;
    uint64_t        virtual_ptr = 0;
    int             dma_buf_fd = -1;
    uint64_t        phys_addr = 0;
    uint32_t        flags = DMA_HEAP_ALLOC_FLAGS;
    int             status = -1;

    /* align is ignored, all DMA HEAP allcoated buffers are minimum 64KB aligned */

    /* alloc memory from DMA HEAP */
    status = appMemDmaHeapAlloc(obj->dma_heap_fd,
                                    size,
                                    flags,
                                    &dma_buf_fd
                                );

    if(status != 0)
    {
        printf("MEM: ERROR: Alloc failed with status = %d !!!\n", status);
    }

    if (status == 0)
    {
        /* Create the {dma_buf_fd, virtual_ptr, phys_addr} mapping and add it
         * to the list for later use.
         */
        status = appMemAddTupleToList(dma_buf_fd, size, &virtual_ptr, &phys_addr);
    }

    return ((void *)(uintptr_t)virtual_ptr);
}

void appMemFreeListItem(app_mem_list_t *mem_map_list)
{
    app_mem_obj_t *obj = &g_app_mem_obj;
    uint32_t dmaBufFd = -1;
    uint32_t dmabuf2phys_fd = -1;
    uint32_t size;

    munmap((void*)mem_map_list->mem_data.virt_addr, mem_map_list->mem_data.size);
    dmaBufFd = mem_map_list->mem_data.dma_buf_fd;
    dmabuf2phys_fd = mem_map_list->mem_data.dmabuf2phys_fd;
    size = mem_map_list->mem_data.size;
    close(dmabuf2phys_fd);
    close(dmaBufFd);

    obj->total_free_bytes += size;
    obj->num_free++;
    if(obj->cur_alloc_bytes < size || obj->cur_alloc==0)
    {
        printf("MEM: WARNING: Free'ing more memory than allocated\n");
    }
    else
    {
        obj->cur_alloc_bytes -= size;
        obj->cur_alloc--;
    }

    #ifdef APP_MEM_DEBUG
    appMemPrintMemAllocInfo();
    #endif
}


int32_t appMemFree(uint32_t block, void *virPtr, uint32_t size )
{
    int32_t status = 0;
    uint8_t b_found_addr = 0;

    app_mem_obj_t *obj = &g_app_mem_obj;
    app_mem_list_t *mem_map_list;

    pthread_mutex_lock(&(obj->list_mutex));
    mem_map_list = obj->plist;

    while(mem_map_list != NULL)
    {
        if(mem_map_list->mem_data.virt_addr == (uint64_t)(virPtr))
        {
            #ifdef APP_MEM_DEBUG
            printf("MEM: Freeing virt addr = %p -> phy addr = %lx, size = %d (FD's = %d %d) !!!\n",
                (void*)virPtr, mem_map_list->mem_data.phys_addr, size,
                mem_map_list->mem_data.dmabuf2phys_fd,
                mem_map_list->mem_data.dma_buf_fd
                );
            #endif

            appMemFreeListItem(mem_map_list);

            b_found_addr = 1;
            break;
        }
        mem_map_list = mem_map_list->next;
    }

    if(1 != b_found_addr)
    {
        printf("MEM: ERROR: Failed to free memory at virt addr = %p !!!\n", virPtr);
    }
    else
    {
        appMemRemoveFromList(&(obj->plist), mem_map_list);
    }


    pthread_mutex_unlock(&(obj->list_mutex));

    return(status);
}

int32_t appMemStats(uint32_t heap_id, app_mem_stats_t *stats)
{
    if(heap_id==APP_MEM_HEAP_DDR && stats != NULL)
    {
        stats->heap_id = heap_id;
        strncpy(stats->heap_name, "DDR", APP_MEM_HEAP_NAME_MAX);
        stats->heap_name[APP_MEM_HEAP_NAME_MAX-1]=0;
        stats->heap_size = 0;
        stats->free_size = 0;
    }
    else
    {
        if(stats!=NULL)
        {
            stats->heap_id = heap_id;
            strncpy(stats->heap_name, "INVALID", APP_MEM_HEAP_NAME_MAX);
            stats->heap_name[APP_MEM_HEAP_NAME_MAX-1]=0;
            stats->heap_size = 0;
            stats->free_size = 0;
        }
    }
    return(0);
}

void  appMemCacheInv(void *ptr, uint32_t size)
{
    int32_t status = 0;
    uint32_t offset = 0;
    uint32_t dmaBufFd = appMemGetDmaBufFd(ptr, &offset);
    struct dma_buf_sync sync_flags = {0};

    sync_flags.flags = DMA_BUF_SYNC_START | DMA_BUF_SYNC_RW;

    if(dmaBufFd > 0)
    {
        status = ioctl(dmaBufFd, DMA_BUF_IOCTL_SYNC, &sync_flags);
        if(status < 0)
        {
            printf("MEM: ERROR: DMA_BUF_IOCTL_SYNC failed for appMemCacheInv(%p, %d) !!!\n",
                ptr, size
                );
        }
    }
}

void  appMemCacheWb(void *ptr, uint32_t size)
{
    int32_t status = 0;
    uint32_t offset = 0;
    uint32_t dmaBufFd = appMemGetDmaBufFd(ptr, &offset);
    struct dma_buf_sync sync_flags ={0};

    sync_flags.flags = DMA_BUF_SYNC_RW | DMA_BUF_SYNC_END;

    if(dmaBufFd > 0)
    {
        status = ioctl(dmaBufFd, DMA_BUF_IOCTL_SYNC, &sync_flags);
        if(status < 0)
        {
            printf("MEM: ERROR: DMA_BUF_IOCTL_SYNC failed for appMemCacheWb(%p, %d) !!!\n",
                ptr, size
                );
        }
    }
}

void  appMemCacheWbInv(void *ptr, uint32_t size)
{
    appMemCacheWb(ptr, size);
    appMemCacheInv(ptr, size);
}

/* NOT SUPPORTED on Linux A72, needed for linking tivx mem platform layer */
int32_t appMemResetScratchHeap(uint32_t heap_id)
{
    return(0);
}

/* give a 'dma_buf_fd' get physical address presented by that 'dma_buf_fd' */
int appMemDmaHeapGetPhys(int phys_fd, int dma_buf_fd, uint64_t *phys)
{
    if (phys == NULL)
        return -EINVAL;

    struct dma_buf_phys_data data = {
        .fd = dma_buf_fd,
    };

    data.phys = 0;

    int ret = ioctl(phys_fd, DMA_BUF_PHYS_IOC_CONVERT, &data);
    if (ret < 0)
        return -errno;

    *phys = data.phys;

    return 0;
}

/* Linked list manipulation functions */
int appMemPushToList(app_mem_list_t **list, app_mem_type_t *data)
{
    app_mem_list_t *new_node = (app_mem_list_t *)malloc(sizeof(app_mem_list_t));
    int status = 0;

    if(NULL != new_node)
    {
        new_node->next = *list;
        new_node->mem_data.virt_addr = data->virt_addr;
        new_node->mem_data.phys_addr = data->phys_addr;
        new_node->mem_data.dma_buf_fd = data->dma_buf_fd;
        new_node->mem_data.dmabuf2phys_fd = data->dmabuf2phys_fd;
        new_node->mem_data.offset = data->offset;
        new_node->mem_data.size = data->size;
        *list = new_node;
        status = 0;
    }
    else
    {
        status = -1;
    }
    return status;
}

void appMemFreeList(app_mem_list_t *list)
{
    app_mem_list_t *tmp;
    while(list != NULL)
    {
        tmp = list;
        list = list->next;
        appMemFreeListItem(tmp);
        free(tmp);
    }
}

void appMemRemoveFromList(app_mem_list_t **list, app_mem_list_t *node)
{
    app_mem_list_t *curr = *list;
    app_mem_list_t *prev = *list;

    while(curr != NULL)
    {
        /*Check for match found*/
        if(curr == node)
        {
            /*Check if match found is HEAD*/
            if(*list == node)
            {
                *list = curr->next;
            }
            else
            {
                prev->next = curr->next;
            }
            free(curr);
            break;
        }
        else
        {
            prev = curr;
            curr = curr->next;
        }
    }
}

static int32_t appMemAddTupleToList(uint32_t dmaBufFd, uint32_t size, uint64_t *virtPtr, uint64_t *phyPtr)
{
    app_mem_obj_t  *obj = &g_app_mem_obj;
    void           *vPtr = NULL;
    app_mem_type_t  m;
    int32_t         dmabuf2phys_fd = -1;
    int32_t         status = 0;

    *virtPtr = 0;
    *phyPtr  = 0;

    if (size == 0)
    {
        printf("MEM: ERROR: Mapped memory size cannot be 0.\n");
        status = -1;
    }

    if (status == 0)
    {
        vPtr = mmap((void *)0x00000000u,
                    size,
                    PROT_WRITE | PROT_READ,
                    MAP_SHARED,
                    dmaBufFd,
                    0u);

        if (vPtr == NULL)
        {
            printf("MEM: ERROR: Failed to translate dmaBufFd %d to "
                   "virtPtr!!!\n", dmaBufFd);
            status = -1;
        }
    }

    /* open driver to convert to physical address */
    if (status == 0)
    {
        *virtPtr = (uint64_t)vPtr;

        dmabuf2phys_fd = open("/dev/dma-buf-phys", O_RDONLY | O_CLOEXEC);

        if (dmabuf2phys_fd < 0)
        {
            printf("MEM: ERROR: /dev/dma-buf-phys open failed !!!\n");
            status = -1;
        }
    }

    /* get physical address */
    if (status == 0)
    {
        status = appMemDmaHeapGetPhys(dmabuf2phys_fd, dmaBufFd, phyPtr);

        if(status != 0)
        {
            perror("appMemDmaHeapGetPhys");
            printf("MEM: ERROR: appMemDmaHeapGetPhys failed "
                   "(dma buf = %d, dmabuf2phys = %d)!!!\n",
                   dmaBufFd, dmabuf2phys_fd);
        }
    }

    /* add to local linked list for later retrival and conversion */
    if(status==0)
    {
        m.dma_buf_fd = dmaBufFd;
        m.dmabuf2phys_fd = dmabuf2phys_fd;
        m.phys_addr = *phyPtr;
        m.virt_addr = *virtPtr;
        m.offset    = 0;
        m.size      = size;

        /* Enter critical section */
        pthread_mutex_lock(&(obj->list_mutex));

        status = appMemPushToList(&(obj->plist), &m);

        if(status==0)
        {
            obj->total_alloc_bytes += size;
            obj->cur_alloc_bytes += size;
            obj->num_alloc++;
            obj->cur_alloc++;

            #ifdef APP_MEM_DEBUG
            appMemPrintMemAllocInfo();
            #endif
        }

        /* Exit critical section */
        pthread_mutex_unlock(&(obj->list_mutex));

        if(status!=0)
        {
            printf("MEM: ERROR: appMemPushToList failed !!!\n");
        }
    }

    if(status==0)
    {
        #ifdef APP_MEM_DEBUG
        printf("MEM: Allocated virt addr = %lx -> phy addr = %lx, size = %d (FD's = %d %d) !!!\n",
            *virtPtr, *phyPtr, size,
            dmaBufFd,
            dmabuf2phys_fd
            );
        #endif
    }
    else
    {
        /* release resources incase of alloc failure */
        printf("MEM: ERROR: memory alloc of size = %d bytes, failed with status = %d !!!\n", size, status);
        if(vPtr!=NULL)
        {
            munmap(vPtr, size);
        }
        if(dmabuf2phys_fd>=0)
        {
            close(dmabuf2phys_fd);
            dmabuf2phys_fd = -1;
        }
        if(dmaBufFd>=0)
        {
            close(dmaBufFd);
        }
        appMemPrintMemAllocInfo();
    }

    if ((*virtPtr == 0) || (*phyPtr == 0))
    {
        printf("MEM: ERROR: Failed to translate dmaBufFd [%d]\n", dmaBufFd);
        status = -1;
    }

    #ifdef APP_MEM_DEBUG
    printf("MEM: Translated dmaBufFd %d to virtPtr %p and phyPtr %p!!!\n",
           dmaBufFd, (void*)*virtPtr, (void*)*phyPtr);
    #endif

    return status;
}

