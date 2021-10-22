/******************************************************************************
* File Name: cy_ml_inference.h
*
* Description: This file contains public interface for MTB ML inference engine
*
* Related Document: See README.md
*
*******************************************************************************
* (c) 2019-2021, Cypress Semiconductor Corporation. All rights reserved.
*******************************************************************************
* This software, including source code, documentation and related materials
* ("Software"), is owned by Cypress Semiconductor Corporation or one of its
* subsidiaries ("Cypress") and is protected by and subject to worldwide patent
* protection (United States and foreign), United States copyright laws and
* international treaty provisions. Therefore, you may use this Software only
* as provided in the license agreement accompanying the software package from
* which you obtained this Software ("EULA").
*
* If no EULA applies, Cypress hereby grants you a personal, non-exclusive,
* non-transferable license to copy, modify, and compile the Software source
* code solely for use in connection with Cypress's integrated circuit products.
* Any reproduction, modification, translation, compilation, or representation
* of this Software except as specified above is prohibited without the express
* written permission of Cypress.
*
* Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress
* reserves the right to make changes to the Software without notice. Cypress
* does not assume any liability arising out of the application or use of the
* Software or any product or circuit described in the Software. Cypress does
* not authorize its products for use in any products where a malfunction or
* failure of the Cypress product may reasonably be expected to result in
* significant property damage, injury or death ("High Risk Product"). By
* including Cypress's product in a High Risk Product, the manufacturer of such
* system or application assumes all risk of such use and in doing so agrees to
* indemnify Cypress against all liability.
*******************************************************************************/

/*******************************************************************************
* Include guard
*******************************************************************************/
#ifndef __CY_ML_INFERENCE_H
#define __CY_ML_INFERENCE_H

/*******************************************************************************
* Include header file
*******************************************************************************/
#include <stdint.h>
#include <stdbool.h>
/*******************************************************************************
* Compile-time flags
*******************************************************************************/

/*******************************************************************************
* NN data type
*******************************************************************************/
#if COMPONENT_ML_FLOAT32
  typedef float CY_ML_DATA_TYPE_T;
#elif COMPONENT_ML_INT16x16 || COMPONENT_ML_INT16x8
  typedef int16_t CY_ML_DATA_TYPE_T;
#elif COMPONENT_ML_INT8x8
  typedef int8_t CY_ML_DATA_TYPE_T;
#else
    #error Unsupported data type
#endif

/******************************************************************************
 * Defines
 *****************************************************************************/
#define CY_ML_INFERENCE_VERSION_MAJOR       1
#define CY_ML_INFERENCE_VERSION_MINOR       2
#define CY_ML_INFERENCE_VERSION_PATCH       0
#define CY_ML_INFERENCE_VERSION             120

#define CY_ML_SUCCESS                       (0)

#define CY_ML_LINE_SHIFT                    (16u)
#define CY_ML_LINE_MASK                     (0xFFFF0000)

#define CY_ML_LAYER_ID_SHIFT                (8u)
#define CY_ML_LAYER_ID_MASK                 (0x0000FF00)
#define CY_ML_LAYER_ID_INVALID              (255u)

#define CY_ML_ERR_CODE_MASK                 (0x000000FF)
#define CY_ML_ERR_CORE_TOOL_VERSION         (0x01)
#define CY_ML_ERR_EXCEED_MAX_SCRATCH_MEM    (0x02)
#define CY_ML_ERR_LAYER_NOT_SUPPORTED       (0x03)
#define CY_ML_ERR_ACT_NOT_SUPPORTED         (0x04)
#define CY_ML_ERR_INPUT_DIMENTION           (0x05)
#define CY_ML_ERR_CONNECTION                (0x06)
#define CY_ML_ERR_OTHER_INPUT_MISSING       (0x07)
#define CY_ML_ERR_INVALID_ARGUMENT          (0x08)
#define CY_ML_ERR_MISMATCH_DATA_TYPE        (0x09)
#define CY_ML_ERR_MISMATCH_PARM_CHECKSUM    (0x0A)
#define CY_ML_ERR_INVALID_SCRATCH_MEM_OPT   (0x0B)

#define CY_ML_ERROR(x, y)                   ((__LINE__ << CY_ML_LINE_SHIFT) | \
                                            (((x) << CY_ML_LAYER_ID_SHIFT) & CY_ML_LAYER_ID_MASK) | \
                                            ((y) & CY_ML_ERR_CODE_MASK))
#define CY_ML_ERR_CODE(x)                   (uint8_t)((x) & CY_ML_ERR_CODE_MASK)
#define CY_ML_ERR_LAYER_INDEX(x)            (uint8_t)(((x) & CY_ML_LAYER_ID_MASK) >> CY_ML_LAYER_ID_SHIFT)
#define CY_ML_ERR_LINE_NUMBER(x)            (uint16_t)(((x) & CY_ML_LINE_MASK) >> CY_ML_LINE_SHIFT)

#define CY_ML_PROFILE_FRAME                 (0x01)
#define CY_ML_PROFILE_LAYER                 (0x02)
#define CY_ML_PROFILE_MODEL                 (0x04)
#define CY_ML_LOG_MODEL_OUTPUT              (0x10)

#define CY_ML_MODEL_OBJECT_SIZE             (352)
#define CY_ML_RES_CONN_OBJECT_SIZE          (8)
#if COMPONENT_ML_FLOAT32
  #define CY_ML_LAYER_OBJECT_SIZE           (80)
#else
  #define CY_ML_LAYER_OBJECT_SIZE           (88)
#endif

/*******************************************************************************
* Structures and enumerations
*******************************************************************************/
typedef enum
{
    CY_ML_DATA_UNKNOWN            = 0u,      /**< Unknown data type */
    CY_ML_DATA_INT8               = 1u,      /**< 8-bit fixed-point */
    CY_ML_DATA_INT16              = 2u,      /**< 16-bit fixed-point */
    CY_ML_DATA_FLOAT              = 3u,      /**< 32-bit float-point */
} cy_en_ml_data_type_t;

/** Defines profile settings */
typedef enum
{
    CY_ML_PROFILE_DISABLE                     = 0,
    CY_ML_PROFILE_ENABLE_MODEL                = CY_ML_PROFILE_MODEL,
    CY_ML_PROFILE_ENABLE_LAYER                = CY_ML_PROFILE_LAYER,
    CY_ML_PROFILE_ENABLE_MODEL_PER_FRAME      = (CY_ML_PROFILE_MODEL | CY_ML_PROFILE_FRAME),
    CY_ML_PROFILE_ENABLE_LAYER_PER_FRAME      = (CY_ML_PROFILE_LAYER | CY_ML_PROFILE_FRAME),
    CY_ML_LOG_ENABLE_MODEL_LOG  = CY_ML_LOG_MODEL_OUTPUT
} cy_en_ml_profile_config_t;

/**
 * Shared control/model structure
 */
typedef struct
{
    /*@{*/
        int output_size;        /**< NN model inference output size */
        int scratch_mem;        /**< Scratch memory size required for inference */
        int persistent_mem;     /**< Persistent memory size required for inference */
        int input_size;         /**< Input data size */
        int num_of_layers;      /**< The number of layers in NN model */
        int num_of_res_conns;   /**< The number of concurrent residual connections */
        int recurrent_ts_size;  /**< Recurrent time series sample size or zero if not recurrent network */
        int libml_version;                      /**< The version of ML inference engine library */
        uint32_t ml_coretool_version;           /**< The number of Coretool version */
        cy_en_ml_data_type_t libml_input_type;  /**< Inference engin supported input data type */
        cy_en_ml_data_type_t libml_weight_type; /**< Inference engin supported weight data type */
    /*@}*/
} cy_stc_ml_model_info_t;

/******************************************************************************
* Function prototype
******************************************************************************/

typedef int (*cy_ml_cb_fun) (void* arg, char* buf, cy_en_ml_profile_config_t type);

/**
 * \addtogroup API
 * @{
 */

/**
 * \brief : Cy_ML_Model_Inference() is the API function to perform NN inference.
 *
 * CY NN inference API function targeted for PSoC6 embedded ML application.
 * CY NN inference supports floating-point and fixed-point data format.
 * When doing floating-point inference, input data and output data are in
 * floating-point format.
 * When doing fixed-point inference, input data can be either 16-bit or 8-bit
 * fixed-point. Its inference output will be 8-bit fixed-point when both input data
 * and NN weight are 8-bit, otherwise it will be 16-bit fixed-point.  The parameter
 * in_ou_q shall be set to input data fixed-point Q factor at the start of function
 * call, and it will be equal to output data fixed-point Q factor at the end of the
 * function call.
 *
 * \param[in]   modelPt     : Pointer to CY parsed NN model data container
 * \param[in]   input       : Input data pointer
 * \param[out]  output      : Inference output data pointer
 * \param[in, out] in_out_q : Pointer to input data and output data fixed-point Q factor; not applicable (i.e. not used) to floating-point
 *
 * \return                  : Return 0 when success, otherwise return following error code
 *                            CY_ML_ERR_INVALID_ARGUMENT if input or output argument is invalid
 *                            CY_ML_ERR_OTHER_INPUT_MISSING
 *                            or error code from specific inference engine module.
 *                            Please note error code is 8bit LSB, line number where the error happened in
 *                            code is in 16bit MSB, and its layer index if applicable will be at bit 8 to 15
 *                            in the combined 32bit return value.
 */
extern int Cy_ML_Model_Inference(void *modelPt, void *input, void *output,
                                 int* in_out_q);

/**
 * \brief : Cy_ML_Model_Parse() is the API function to parse NN model parameters to get basic info.
 *
 * It will parse info from the NN model parameter buffer to get required basic info such as
 * persistent and scratch memory sizes, input data size, output classification size and stored
 * them in to cy_stc_ml_model_info_t structure.
 *
 * \param[in]   fn_prms     : CY parsed NN model parameter buffer pointer
 * \param[out]  mdl_infoPt  : Pointer to cy_stc_ml_model_info_t structure
 *
 * \return                  : Return 0 0 when success, otherwise return following error code
 *                                CY_ML_ERR_INVALID_ARGUMENT if input or output argument is invalid
 *                                CY_ML_ERR_LAYER_NOT_SUPPORTED if layer is not supported
 *                                CY_ML_ERR_ACT_NOT_SUPPORTED if activation is not supported
 *                                Please note error code is 8bit LSB, line number where the error happened in
 *                                code is in 16bit MSB, and its layer index if applicable will be at bit 8 to 15
 *                                in the combined 32bit return value.
 */

extern int Cy_ML_Model_Parse(char *fn_prms , cy_stc_ml_model_info_t *mdl_infoPt);

/**
 * \brief : Cy_ML_Model_Init() is the API function to parse NN model and initialize CY data container before start inference.
 *
 * From the API inputs, it parses NN model and initializes CY data container.
 * For floating-point NN model, its weights and biases shall be in floating-point stored in
 * buffer pointer by parameter fn_ptr.
 * For fixed-point NN mode, its weights and biases can be either 16-bit or 8-bit format stored
 * in buffer pointer by parameter fn_ptr.
 *
 * \param[out]  dPt_container   : Pointer to CY parsed NN model data container pointer
 * \param[in]   fn_prms         : CY parsed NN model parameter buffer pointer
 * \param[in]   fn_ptr          : NN model weights and biases buffer pointer
 * \param[in]   persistent_mem  : Pointer to allocated persistent memory
 * \param[in]   scratch_mem     : Pointer to allocated scratch memory
 * \param[in]   mdl_infoPt      : Pointer to cy_stc_ml_model_info_t structure
 *
 * \return                      : Return 0 when success, otherwise return error code
 *                                Return CY_ML_ERR_INVALID_ARGUMENT if input or output argument is invalid,
 *                                Otherwise return other errors:
 *                                e.g. CY_ML_ERR_OTHER_INPUT_MISSING if the other input is missing in ADD layer
 *                                Please note error code is 8bit LSB, line number where the error happened in
 *                                code is in 16bit MSB, and its layer index if applicable will be at bit 8 to 15
 *                                in the combined 32bit return value.
*/

extern int Cy_ML_Model_Init(void **dPt_container
           , char *fn_prms, char *fn_ptr
           ,char *persistent_mem, char* scratch_mem, cy_stc_ml_model_info_t *mdl_infoPt);

/**
 * \brief : Cy_ML_Rnn_State_Control() is the API function to rest recurrent NN state,
 *          inference engine will reset Rnn state when this API is called.
 *
 * This API is used to control RNN state. This API can be called after initilization.
 * if one only time reset is desired, use this API to 1) reset the network, and then
 * 2) clear the reset with this API.
 * If the NN model is not recurrent type network. This API will do nothing.
 *
 * \param[out]  modelPt         : Pointer to CY parsed NN model data container pointer
 * \param[in]   rnn_status      : Recurrent NN reset (1) or clear (0)
 * \param[in]   window_size     : Recurrent NN reset window size
 *
 * \return                      : Return 0 when success, otherwise return error code
 *                                Return CY_ML_ERR_INVALID_ARGUMENT if input argument is invalid.
*/

extern int Cy_ML_Rnn_State_Control(void* modelPt, int rnn_status, int window_size);

/**
 * \brief : Cy_ML_Profile_Get_Tsc() is an API function to read time stamp counter (TSC) .
 *
 * Platform specific function to read HW time stamp counter or OS tick timer counter for profiling.
 * The application program developer should provide this function if profiling is enabled.
 *
 * \param[out]   val        : Pointer to time stamp counter return value
 *
 * \return                  : Return 0 when success, otherwise return error code
 */
int Cy_ML_Profile_Get_Tsc(uint32_t *val);

/**
 * \brief : Initialize profile configuration.
 *
 * This API is used to setup profile configuration and specify the callback function to handle the
 * profile log. If no callback function is specified, the profile log will be printed out on console.
 *
 * \param[in]  modelPt         : Pointer to CY parsed NN model data container pointer
 * \param[in]  config          : Profile setting
 * \param[in]  cb_func         : Callback function to handle the profile result.
 * \param[in]  cb_arg          : Callback function argument
 *
 * \return                     : Return 0 when success, otherwise return error code
 *                               Return CY_ML_ERR_INVALID_ARGUMENT if input parameter is invalid.
*/
int Cy_ML_Profile_Init(void *modelPt, cy_en_ml_profile_config_t config, cy_ml_cb_fun cb_func, void *cb_arg);

/**
 * \brief : Update  profile configuration.
 *
 * This API is used to update  profile configuration.
 *
 * \param[in]  modelPt         : Pointer to CY parsed NN model data container pointer
 * \param[in]  config          : Profile setting
 *
 * \return                     : Return 0 when success, otherwise return error code
 *                               Return CY_ML_ERR_INVALID_ARGUMENT if input parameter is invalid.
*/
int Cy_ML_Profile_Control(void *modelPt, cy_en_ml_profile_config_t config);

/**
 * \brief : Print profile log.
 *
 *
 * \param[in]  modelPt         : Pointer to CY parsed NN model data container pointer
 * \return                     : Return 0 when success, otherwise return error code
 *                               Return CY_ML_ERR_INVALID_ARGUMENT if input parameter is invalid.
*/
int Cy_ML_Profile_Print(void *modelPt);

/**
 * @} end of API group
 */

#endif /*__CY_ML_INFERENCE_H */

/* [] END OF FILE */
