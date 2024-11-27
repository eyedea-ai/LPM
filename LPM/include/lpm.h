///////////////////////////////////////////////////////////
//                                                       //
// Copyright (c) 2014-2022 by Eyedea Recognition, s.r.o. //
//                  ALL RIGHTS RESERVED.                 //
//                                                       //
// Author: Eyedea Recognition, s.r.o.                    //
//                                                       //
// Contact:                                              //
//           web: http://www.eyedea.cz                   //
//           email: info@eyedea.cz                       //
//                                                       //
// Consult your license regarding permissions and        //
// restrictions.                                         //
//                                                       //
///////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////
//                        LPM SDK                        //
//             License plate reading library             //
///////////////////////////////////////////////////////////


#ifndef _LPM_H_
#define _LPM_H_

#include "lpm_type.h"

/*! \defgroup LPM Lpr modules API.
 @{
*/
/*! @} */

/*! \addtogroup LPM
 @{
*/


#if defined(CPP) || defined(__cplusplus) || defined(c_plusplus)
extern "C" 
{
#endif



/* ======================================================== */
/*                                                          */
/*  LPM FUNCTION TYPES DEFINITIONS                          */
/*                                                          */
/*                                                          */
/* ======================================================== */

typedef int                  (*fcn_lpmInit)(const char *, LPMState *);
typedef void                 (*fcn_lpmFree)(LPMState *);
typedef unsigned long        (*fcn_lpmVersion)(void);
typedef char const          *(*fcn_lpmCompilationDate)(void);

typedef int                  (*fcn_lpmLoadViewConfig)(const char *, LpmCameraViewParams *);
typedef int                  (*fcn_lpmWriteViewConfig)(const char *, LpmCameraViewParams);

typedef int                  (*fcn_lpmLoadModule)(LPMState, int, LpmCameraViewParams *, const LpmModuleConfig *);
typedef void                 (*fcn_lpmFreeModule)(LPMState, int);

typedef LpmDetResult        *(*fcn_lpmRunDet)(LPMState, int, ERImage, const LpmBoundingBox *);
typedef void                 (*fcn_lpmFreeDetResult)(LPMState, LpmDetResult *);

typedef LpmOcrResult        *(*fcn_lpmRunOcr)(LPMState, int, ERImage, const LpmBoundingBox *, LpmDetectionLabel);
typedef void                 (*fcn_lpmFreeOcrResult)(LPMState, LpmOcrResult *);

typedef int                  (*fcn_lpmGetNumAvlbModules)(LPMState);
typedef int                  (*fcn_lpmGetModuleIndex)(LPMState, int, int, int);
typedef int                  (*fcn_lpmGetModuleIndexByName)(LPMState, const char *);
typedef LpmModuleInfo       *(*fcn_lpmGetModuleInfo)(LPMState, int);

typedef int                  (*fcn_lpmGetLastError)(void);



/* ======================================================== */
/*                                                          */
/*  LPM INIT / FREE FUNCTIONS                               */
/*                                                          */
/*                                                          */
/* ======================================================== */

/*! \defgroup LPMInit  LPM Initialization / Free
 @{
*/

/*! \fn LPMState lpmInit(const char *lpm_directory, LPMState *lpm_state)

    \brief  Initializes the LPM engine and searches the given directory for installed LPM modules.
            
    \param  lpm_directory LPM module base directory (e.g. "../../modules-v[VERSION]/x64").
    \param  lpm_state LPM state structure (LPM context) to be initialized.

    \return 0 on success, non-zero otherwise.

    \see    lpmFree, lpmGetNumAvlbModules, lpmGetModuleIndex, lpmGetModuleIndex
*/
ER_FUNCTION_PREFIX int lpmInit(const char *lpm_directory, LPMState *lpm_state);


/*! \fn void lpmFree(LPMState *lpm_state)

    \brief  Frees the initialized LPM engine.

    \param  lpm_state The LPM state created by lpmInit() function.

    \see    lpmInit
*/
ER_FUNCTION_PREFIX void lpmFree(LPMState *lpm_state);


/*! \fn unsigned long lpmVersion(void)

  \brief  Returns the LPM engine version encoded in one unsigned long integer.
          Cast (unsigned char)(lpmVersion() >> CHAR_BIT) to get LPM version.
          Cast (unsigned char)(lpmVersion()) to get LPM subversion.

  \return The encoded LPM engine version.

  \see    lpmCompilationDate
*/
ER_FUNCTION_PREFIX unsigned long lpmVersion(void);


/*! \fn char const *lpmCompilationDate(void)

    \brief  Returns the compilation date of the LPM engine.

    \return String with compilation date __DATE__ in Mmm dd yyyy format.

    \see    lpmVersion
*/
ER_FUNCTION_PREFIX char const *lpmCompilationDate(void);

/*!  @} */ 



/* ======================================================== */
/*                                                          */
/*  LPM LOAD / WRITE VIEW CONFIG                            */
/*                                                          */
/*                                                          */
/* ======================================================== */

/*! \defgroup LPMViewConfig  LPM Load / Write ViewConfig
@{
*/

/*! \fn LpmCameraViewParams lpmLoadViewConfig(const char *filename, LpmCameraViewParams *camera_view_params)

    \brief  Loads the camera view parameters from file.

    \param  filename            Path to a file, from which the config should be loaded, or NULL to use default parameters.
    \param  camera_view_params  Structure to be loaded with parameters from a given file.

    \return Zero on success, error code otherwise.

    \see    lpmWriteViewConfig
*/
ER_FUNCTION_PREFIX int lpmLoadViewConfig(const char *filename, LpmCameraViewParams *camera_view_params);


/*! \fn int lpmWriteViewConfig(const char *filename, LpmCameraViewParams camera_view_params)

    \brief  Writes the camera view parameters to a given file.

    \param  filename            Path to the file where camera view parameters will be written.
    \param  camera_view_params  Pointer to LpmCameraViewParams to write.

    \return 0 - File was successfully written, other - Error while saving file.

    \see    lpmLoadViewConfig
*/
ER_FUNCTION_PREFIX int lpmWriteViewConfig(const char *filename, LpmCameraViewParams camera_view_params);

/*!  @} */



/* ======================================================== */
/*                                                          */
/*  LPM LOAD / FREE MODULE                                  */
/*                                                          */
/*                                                          */
/* ======================================================== */

/*! \defgroup LPMLoadModule  Load / Free module 
 @{
*/

/*! \fn int lpmLoadModule(LPMState lpm_state, int module_index, LpmCameraViewParams *camera_view_params, const LpmModuleConfig *module_config)

    \brief  Loads an LPM module with a given index.

    \param  lpm_state           The LPM state created by the lpmInit() function.
    \param  module_index        Index of the LPM module you wish to load. Note that module index and module ID are two different things.
    \param  camera_view_params  Pointer to optional camera view parameters. Use NULL for default parameters.
    \param  module_config       Pointer to optional module configuration parameters. Use NULL to load values from configuration file.

    \return 0 - the module was successfully initialized, other - Error while initializing the module.

    \see lpmInit, lpmGetModuleIndex, lpmGetNumAvlbModules, lpmFreeModule, lpmLoadViewConfig
*/
ER_FUNCTION_PREFIX int lpmLoadModule(LPMState lpm_state, int module_index, LpmCameraViewParams *camera_view_params, const LpmModuleConfig *module_config);


/*! \fn void lpmFreeModule(LPMState lpm_state, int module_index)

    \brief  Frees previously loaded LPM module with the given index.

    \param  lpm_state     The LPM state created by lpmInit() function.
    \param  module_index  Index of the LPM module to free.

    \see    lpmInit, lpmLoadModule
*/
ER_FUNCTION_PREFIX void lpmFreeModule(LPMState lpm_state, int module_index);

/*!  @} */ 



/* ======================================================== */
/*                                                          */
/*  LP DETECTION FUNCTION                                   */
/*                                                          */
/*                                                          */
/* ======================================================== */

/*! \defgroup LPMDetection  Run detection 
 @{
*/

/*! \fn LpmDetResult *lpmRunDet(LPMState lpm_state, int module_index, ERImage image, const LpmBoundingBox *bounding_box)

    \brief  Runs license/ADR plate detection on the given image.

    \param  lpm_state     The LPM state created by lpmInit() function.
    \param  module_index  Index of LPM module to use. Note that module index and module ID are two different things.
    \param  image         ERImage structure containing the image for detection.
    \param  bounding_box  The bounding box of a detection area.

    \return NULL - Error during computation occurred, other - LpmDetResult structure with all detections.

    \see    lpmInit, lpmLoadModule, lpmFreeDetResult
*/
ER_FUNCTION_PREFIX LpmDetResult *lpmRunDet(LPMState lpm_state, int module_index, ERImage image, const LpmBoundingBox *bounding_box);


/*! \fn void lpmFreeDetResult(LPMState lpm_state, LpmDetResult *detection_result)

    \brief  Frees detection result structure generated by lpmRunDet().

    \param  lpm_state         The LPM state created by lpmInit() function.
    \param  detection_result  Pointer to the detection result structure to be freed.

    \see    lpmInit, lpmLoadModule, lpmRunDet
*/ 
ER_FUNCTION_PREFIX void lpmFreeDetResult(LPMState lpm_state, LpmDetResult *detection_result);

/*! @} */ 



/* ======================================================== */
/*                                                          */
/*  LPM OCR FUNCTIONS                                       */
/*                                                          */
/*                                                          */
/* ======================================================== */

/*! \defgroup LPMOcr  Run OCR 
 @{
*/

/*! \fn LpmOcrResult *lpmRunOcr(LPMState lpm_state, int module_index, ERImage image, const LpmBoundingBox *detection_position, LpmDetectionLabel detection_label)

    \brief  Runs OCR on the given image.

    \param  lpm_state           The LPM state created by lpmInit() function.
    \param  module_index        Index of the LPM module to use. Note that module index and module ID are two different things.
    \param  image               ERImage structure containing the input image.
    \param  detection_position  The 4-point position of the detection.
    \param  detection_label     The detection label specifying the type of detection; can be obtained from the LpmDetection structure if using the lpmRunDet() function.

    \return NULL - Error during computation occurred, other - LpmOcrResult structure with all detections.

    \see    lpmInit, lpmLoadModule, lpmFreeOcrResult, lpmRunDet
*/
ER_FUNCTION_PREFIX LpmOcrResult *lpmRunOcr(LPMState lpm_state, int module_index, ERImage image, const LpmBoundingBox *detection_position, LpmDetectionLabel detection_label);


/*! \fn void lpmFreeOcrResult(LPMState lpm_state, LpmOcrResult *ocr_result)

    \brief  Frees the detection result structure generated by lpmRunOcr().

    \param  lpm_state     The LPM state created by lpmInit() function.
    \param  ocr_result    Pointer to the OCR result structure to be freed.

    \see    lpmInit, lpmLoadModule, lpmRunOcr
*/  
ER_FUNCTION_PREFIX void lpmFreeOcrResult(LPMState lpm_state, LpmOcrResult *ocr_result);

/*!  @} */



/* ======================================================== */
/*                                                          */
/*  LPM MODULE HANDLING FUNCTIONS                           */
/*                                                          */
/*                                                          */
/* ======================================================== */

/*! \defgroup LPMutils  LPM module utils
@{
*/

/*! \fn int lpmGetNumAvlbModules(LPMState lpm_state)

    \brief  Gets the number of available LPM modules.

    \param  lpm_state The LPM state created by lpmInit() function.

    \return -1 - Error during computation occurred, other - Number of LPM modules.

    \see    lpmInit
*/
ER_FUNCTION_PREFIX int lpmGetNumAvlbModules(LPMState lpm_state);


/*! \fn int lpmGetModuleIndex(LPMState lpm_state, int module_id, int version, int subversion)

    \brief  Gets the LPM module index (handle) from the module ID and its version.

    Module indices can be vary with each program execution because they depend
    on the search order of the LPM module directory.

    \param  lpm_state   The LPM state created by lpmInit() function.
    \param  module_id   ID of the module.
    \param  version     Version of the module.
    \param  subversion  Subversion of the module.

    \note   Set version and subversion to zero to get an index of the latest available module.

    \return -1 - Error during computation occurred, other - Module index.

    \see    lpmInit, lpmGetModuleInfo
*/
ER_FUNCTION_PREFIX int lpmGetModuleIndex(LPMState lpm_state, int module_id, int version, int subversion);


/*! \fn int lpmGetModuleIndexByName(LPMState lpm_state, const char *module_name)

    \brief  Gets the LPM module index (handle) from the module name.

    Module indices can be vary with each program execution because it depends
    on the search order of the given module directory.

    \param  lpm_state    The LPM state created by lpmInit() function.
    \param  module_name  The name of the given module (see lpmGetModuleInfo()).

    \return -1 - Error during computation occurred, other - Module index.

    \see    lpmInit, lpmGetModuleInfo
*/
ER_FUNCTION_PREFIX int lpmGetModuleIndexByName(LPMState lpm_state, const char *module_name);


/*! \fn LpmModuleInfo *lpmGetModuleInfo(LPMState lpm_state, int module_index)

    \brief  Retrieves information about the LPM module.

    \param  lpm_state     The LPM state created by lpmInit() function.
    \param  module_index  Index of the LPM module to use. Note that module index and module ID are two different things.

    \return A pointer to the LpmModuleInfo structure or NULL on error (see errno for details).

    \see    lpmInit
*/
ER_FUNCTION_PREFIX LpmModuleInfo *lpmGetModuleInfo(LPMState lpm_state, int module_index);


/*!  @} */



/* ======================================================== */
/*                                                          */
/*  LPM ERROR HANDLING FUNCTIONS                            */
/*                                                          */
/*                                                          */
/* ======================================================== */


/*! \defgroup LPMerror  LPM error handling
 @{
*/

/*! \fn int lpmGetLastError(void)

    \brief  Gets the code of the last occurred error. Immediate next call of this function returns LPM_SUCCESS.

    \return The ID of the last occurred error.
*/ 
ER_FUNCTION_PREFIX int lpmGetLastError(void);


/*! @} */


/*! @} */
#if defined(CPP) || defined(__cplusplus) || defined(c_plusplus)
}
#endif


#endif
