/*
 * Copyright (c) 2012-2016 The Khronos Group Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */



#ifndef VX_OBJARRAY_H_
#define VX_OBJARRAY_H_


#ifdef __cplusplus
extern "C" {
#endif

/*!
 * \file
 * \brief Implementation of Object Array data object
 */



/*!
 * \brief object array internal state
 *
 * \ingroup group_vx_objarray
 */
typedef struct _vx_object_array
{
    /*! \brief reference object */
    tivx_reference_t base;

    /*! \brief array of references to the objects */
    vx_reference ref[TIVX_OBJECT_ARRAY_MAX_ITEMS];
} tivx_objarray_t;




#ifdef __cplusplus
}
#endif

#endif
