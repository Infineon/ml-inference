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
#define CY_ML_FIXED_POINT (CY_ML_FIXED_POINT_16_NN || CY_ML_FIXED_POINT_8_NN)

#if CY_ML_FLOATING_POINT_fltxflt_NN && CY_ML_FIXED_POINT
#error Floating-point & fixed-pint can not both be enabled in embedded applications!
#elif !(CY_ML_FLOATING_POINT_fltxflt_NN || CY_ML_FIXED_POINT)
#error Either floating-point nor fixed-pint are enabled in embedded applications!
#endif

#if CY_ML_FIXED_POINT_16_NN && CY_ML_FIXED_POINT_8_NN
#error Fixed-point embedded application can not have both 16-bit and 8-bit weights enabled!
#endif

#if CY_ML_FIXED_POINT_16_IN && CY_ML_FIXED_POINT_8_IN
#error Fixed-point embedded application can not have both 16-bit and 8-bit input enabled!
#endif
/*******************************************************************************
* NN data type
*******************************************************************************/
#if CY_ML_FLOATING_POINT_fltxflt_NN
typedef float CY_ML_DATA_TYPE_T;
#else
#if CY_ML_FIXED_POINT_8_IN && CY_ML_FIXED_POINT_8_NN
typedef int8_t CY_ML_DATA_TYPE_T;
#else
typedef int16_t CY_ML_DATA_TYPE_T;
#endif
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

/**
 * Shared control/model structure
 */
typedef struct
{
    /*@{*/
        int n_out_classes;      /**< NN model inference classification output size */
        int scratch_mem;        /**< Scratch memory size required for inference */
        int persistent_mem;     /**< Persistent memory size required for inference */
        int input_sz;           /**< Input data size */
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
 * Defines
 *****************************************************************************/
#define CY_ML_INFERENCE_VERSION_MAJOR       1
#define CY_ML_INFERENCE_VERSION_MINOR       1
#define CY_ML_INFERENCE_VERSION             110

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

#define CY_ML_ERROR(x, y)        ((__LINE__ << CY_ML_LINE_SHIFT) | (((x) << CY_ML_LAYER_ID_SHIFT) & CY_ML_LAYER_ID_MASK) | ((y) & CY_ML_ERR_CODE_MASK) )
#define CY_ML_ERR_CODE(x)        (uint8_t)((x) & CY_ML_ERR_CODE_MASK)
#define CY_ML_ERR_LAYER_INDEX(x) (uint8_t)(((x) & CY_ML_LAYER_ID_MASK) >> CY_ML_LAYER_ID_SHIFT)
#define CY_ML_ERR_LINE_NUMBER(x) (uint16_t)(((x) & CY_ML_LINE_MASK) >> CY_ML_LINE_SHIFT)
/******************************************************************************
* Function prototype
******************************************************************************/
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
 * \param[in]   in_size     : Input data sample size
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
                                 int* in_out_q, int in_size);

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
 * \param[in]   rnn_reset       : Recurrent NN reset flag
 * \param[in]   rnn_reset_win   : Recurrent NN reset window size
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
           ,char *persistent_mem, char* scratch_mem, cy_stc_ml_model_info_t *mdl_infoPt
           ,int rnn_reset, int rnn_reset_win);

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
 * \brief : Cy_ML_Profile_Init() is CY ML cycle profiler initialization API.
 *
 * It initialize CY cycle profiler cycle counters.
 */
extern void Cy_ML_Profile_Init();

/**
 * \brief : Cy_ML_Enable_Model_Profile() is API function to enable/disable NN model cycle profile.
 *
 * Enable or disable NN model cycle profile.
 *
 * \param[in]   enable_flag : Set to 1 to enable, set 0 to disable
 */
extern void Cy_ML_Enable_Model_Profile(bool enable_flag);

/**
 * \brief : Cy_ML_Enable_Layer_Profile() is API function to enable/disable NN model layer cycle profile.
 *
 * Enable or disable NN model layer cycle profile.
 *
 * \param[in]   enable_flag : Set to 1 to enable, set 0 to disable
 */
extern void Cy_ML_Enable_Layer_Profile(bool enable_flag);

/**
 * \brief : Cy_ML_Enable_Model_Output() is API function to enable/disable inference output print out.
 *
 * Enable or disable inference output print out.
 * For easier to display and read of the print out data when output data is fixed-point, output data
 * will be converted from fixed-point to floating-point based on output fixed-point Q factor.
*
 * \param[in]   enable_flag : Set to 1 to enable, set 0 to disable
 */
extern void Cy_ML_Enable_Model_Output(bool enable_flag);

/**
 * \brief : Cy_ML_Enable_Per_Frame_Profile() is API function to enable/disable per frame cycle print out.
 *
 * Enable or disable per frame cycle print out.
 *
 * \param[in]   enable_flag : Set to 1 to enable, set 0 to disable
 */
extern void Cy_ML_Enable_Per_Frame_Profile(bool enable_flag);

/**
 * \brief : Cy_ML_Profile_Start() is API function to set up cycle profiler.
 *
 * To set up and initialize cycle profiler.
 */
extern void Cy_ML_Profile_Start(void);

/**
 * \brief : Cy_ML_Profile_Update() is API function to update cycle profiler.
 *
 * Update cycle information needed by profiler.
 */
extern void Cy_ML_Profile_Update(void);

/**
 * \brief : Cy_ML_Profile_Close() is API function to get summarized cycle profile information.
 *
 * Generate and print out summarized cycle profile information.
 *
 * \param[in]   modelPt     : Pointer to cy_stc_ml_model_info_t structure
 * \param[in]   result      : Flag to be used in print out of PASS or FAIL
 */
extern void Cy_ML_Profile_Close(void *modelPt, bool result);

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
 * @} end of API group
 */

#endif /*__CY_ML_INFERENCE_H */

/* [] END OF FILE */
