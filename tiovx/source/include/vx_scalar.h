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



#ifndef VX_SCALAR_H_
#define VX_SCALAR_H_


#ifdef __cplusplus
extern "C" {
#endif

/*!
 * \file
 * \brief Implementation of Scalar object
 */



/*!
 * \brief Scalar object internal state
 *
 * \ingroup group_vx_scalar
 */
typedef struct _vx_scalar
{
    /*! \brief reference object */
    tivx_reference_t base;

} tivx_scalar_t;


/*!
 * \brief Print scalar info
 *
 * \param scalar [in] scalar
 *
 * \ingroup group_vx_scalar
 */
void ownPrintScalar(vx_scalar scalar);

#ifdef __cplusplus
}
#endif

#endif
