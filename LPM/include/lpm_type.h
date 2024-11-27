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


#ifndef _LPM_TYPE_H_
#define _LPM_TYPE_H_

#include "er_image.h"


/*! \addtogroup LPM
 @{
*/

/*! \defgroup LPM_TYPES LPM type definitions
 @{
*/


typedef void *LPMState;


/* ======================================================== */
/*                                                          */
/*  DATE/TIME STRUCTURES                                    */
/*                                                          */
/*                                                          */
/* ======================================================== */

/*! \defgroup LPM_TYPES_DATETIME Date/time types 
@{
*/

/*! A date/time structure */
typedef struct
{
    /*! Year in XXXX form */
    unsigned int  year;
    /*! Month 1-12 */
    unsigned char month;
    /*! Day 1-31 */
    unsigned char day_of_month;
    /*! Hour 0-23? */
    unsigned char hour;
    /*! Minutes 0-59 */
    unsigned char minute;
    /*! Seconds 0-59 */
    unsigned char second;
} LpmDateTime;


/*! Structure for a license information */
typedef struct
{
    /*! Flag determining whether the license is valid or not. Zero means invalid, otherwise valid. */
    int             is_valid;
    /*! License expiration date. Note: the license is time-unlimited if all fields of LpmDateTime structure are zeros. */
    LpmDateTime     expiration_date;
    /*! The counter is enabled if non-zero. */
    int             is_using_counter;
    /*! Number of module executions left. License is execution-unlimited if is_using_counter is zero. */
    unsigned long   executions_left;
} LpmLicenseInfo;

/*!
 @}
 */



/* ======================================================== */
/*                                                          */
/*  COMMON STRUCTURE DEFINITIONS                            */
/*                                                          */
/*                                                          */
/* ======================================================== */

/*! \defgroup LPM_TYPES_COMMON Common types
 @{
*/

#define LPM_MAX_PATH_LEN        4096
#define LPM_MAX_STR_LEN         256

#define LPM_EXTENSIONS_v7_3     1
#define LPM_EXTENSIONS_v7_6     1


#if defined(WIN32) || defined(WIN64) || defined(WINDOWS)
/* 64bit enumerator defines of module properties */
#define    LPM_EMPTY                   0x0000i64

/* Bits from 0x08 up to 0x80 are reserved - 8 bits total for detector's type */
#define    LPM_DET_ENABLED             0xFFFFi64
#define    LPM_DET_FRONTAL             0x0001i64
#define    LPM_DET_GENERIC             0x0002i64
#define    LPM_DET_LFRONTAL            0x0004i64
#define    LPM_DET_RGB_FRONTAL         0x0008i64   // temporary
#define    LPM_DET_RGB_GENERIC         0x0010i64   // temporary
#define    LPM_DET_WFRONTAL            0x0020i64

/* Bits from 0x100 up to 0x8000 are reserved - 8 bits total for object's type*/    
#define    LPM_OBJ_LP                  0x0000000100i64  // 0000 0001  0000 0000
#define    LPM_OBJ_ADR                 0x0000000200i64  // 0000 0010  0000 0000
#define    LPM_OBJ_LP2                 0x0000000400i64  // 0000 0100  0000 0000
#define    LPM_OBJ_WIN                 0x0000000800i64  // 0000 1000  0000 0000
#define    LPM_OBJ_FACE                0x0000001000i64  // 0001 0000  0000 0000
#define    LPM_OBJ_LCD                 0x0000002000i64  // 0010 0000  0000 0000
#define    LPM_OBJ_CAR                 0x0000004000i64  // 0100 0000  0000 0000

/* Bits from 0x200000 up to 0x8000000000 are reserved  - 24 bits total for ocr types */                                  
#define    LPM_OCR_ENABLED             0xFFFFFF0000i64
#define    LPM_OCR_CZ                  0x0000010000i64 // Czech Republic
#define    LPM_OCR_EU                  0x0000020000i64 // Europe
#define    LPM_OCR_AS                  0x0000040000i64 // Asia
#define    LPM_OCR_AF                  0x0000080000i64 // Africa
#define    LPM_OCR_ADR                 0x0000100000i64 // ADR
#define    LPM_OCR_OC                  0x0000200000i64 // Oceania
#define    LPM_OCR_LCD                 0x0000400000i64 // LCD
#define    LPM_OCR_GEN                 0x0000800000i64 // General OCR
//#define    LPM_OCR_RESERVED            0x0001000000i64
//#define    LPM_OCR_RESERVED            0x0002000000i64
//#define    LPM_OCR_RESERVED            0x0004000000i64
#define    LPM_OCR_NA                 0x0008000000i64  // North America
//#define    LPM_OCR_RESERVED            0x0010000000i64
//#define    LPM_OCR_RESERVED            0x0020000000i64
//#define    LPM_OCR_RESERVED            0x0040000000i64
//#define    LPM_OCR_RESERVED            0x0080000000i64
//#define    LPM_OCR_RESERVED            0x0100000000i64
//#define    LPM_OCR_RESERVED            0x0200000000i64
//#define    LPM_OCR_RESERVED            0x0400000000i64
//#define    LPM_OCR_RESERVED            0x0800000000i64
//#define    LPM_OCR_RESERVED            0x1000000000i64
//#define    LPM_OCR_RESERVED            0x2000000000i64
//#define    LPM_OCR_RESERVED            0x4000000000i64
//#define    LPM_OCR_RESERVED            0x8000000000i64
#define    LPM_OCR_DISABLED            0x0000000000i64
    

/* Bits from 0x01000000000 up to 0x080000000000 are reserved  - 8 bits total for rcg types */    
#define    LPM_RCG_ENABLED             0x0F0000000000i64
#define    LPM_RCG_VCL                 0x010000000000i64


#else
/* 64bit enumerator defines of module properties */
#define    LPM_EMPTY                   0x0000LL

/* Bits from 0x08 up to 0x80 are reserved - 8 bits total for detector's type */
#define    LPM_DET_ENABLED             0xFFFFLL
#define    LPM_DET_FRONTAL             0x0001LL
#define    LPM_DET_GENERIC             0x0002LL
#define    LPM_DET_LFRONTAL            0x0004LL
#define    LPM_DET_RGB_FRONTAL         0x0008LL   // temporary
#define    LPM_DET_RGB_GENERIC         0x0010LL   // temporary
#define    LPM_DET_WFRONTAL            0x0020LL

/* Bits from 0x100 up to 0x8000 are reserved - 8 bits total for object's type*/    
#define    LPM_OBJ_LP                  0x0000000100LL  // 0000 0001  0000 0000
#define    LPM_OBJ_ADR                 0x0000000200LL  // 0000 0010  0000 0000
#define    LPM_OBJ_LP2                 0x0000000400LL  // 0000 0100  0000 0000
#define    LPM_OBJ_WIN                 0x0000000800LL  // 0000 1000  0000 0000
#define    LPM_OBJ_FACE                0x0000001000LL  // 0001 0000  0000 0000
#define    LPM_OBJ_LCD                 0x0000002000LL  // 0010 0000  0000 0000
#define    LPM_OBJ_CAR                 0x0000004000LL  // 0100 0000  0000 0000

/* Bits from 0x200000 up to 0x8000000000 are reserved  - 24 bits total for ocr types */                                  
#define    LPM_OCR_ENABLED             0xFFFFFF0000LL
#define    LPM_OCR_CZ                  0x0000010000LL
#define    LPM_OCR_EU                  0x0000020000LL
#define    LPM_OCR_AS                  0x0000040000LL
#define    LPM_OCR_AF                  0x0000080000LL
#define    LPM_OCR_ADR                 0x0000100000LL
#define    LPM_OCR_OC                  0x0000200000LL
#define    LPM_OCR_LCD                 0x0000400000LL
#define    LPM_OCR_GEN                 0x0000800000LL
//#define    LPM_OCR_RESERVED            0x0001000000LL
//#define    LPM_OCR_RESERVED            0x0002000000LL
//#define    LPM_OCR_RESERVED            0x0004000000LL
#define    LPM_OCR_NA                  0x0008000000LL
//#define    LPM_OCR_RESERVED            0x0010000000LL
//#define    LPM_OCR_RESERVED            0x0020000000LL
//#define    LPM_OCR_RESERVED            0x0040000000LL
//#define    LPM_OCR_RESERVED            0x0080000000LL
//#define    LPM_OCR_RESERVED            0x0100000000LL
//#define    LPM_OCR_RESERVED            0x0200000000LL
//#define    LPM_OCR_RESERVED            0x0400000000LL
//#define    LPM_OCR_RESERVED            0x4000000000LL
//#define    LPM_OCR_RESERVED            0x0800000000LL
#define    LPM_OCR_DISABLED            0x0000000000LL
    

/* Bits from 0x10000000000 up to 0x800000000000 are reserved  - 8 bits total for rcg types */    
#define    LPM_RCG_ENABLED             0xFF0000000000LL
#define    LPM_RCG_VCL                 0x010000000000LL

#endif

/* 12 bits left untouched */
typedef long long LpmPropertyFlags;


/*! View type */
typedef enum
{
    /*! Frontal images of cars (e. g. overhead installation on motorway gantries). */
    LPM_VIEW_FRONTAL = 0,
    /*! Generic images of cars (e. g. camera in a moving vehicle). */
    LPM_VIEW_GENERIC = 1
} LpmViewType;


/*! Camera view parameters */
typedef struct
{
    /*! LpmViewType with values LPM_VIEW_FRONTAL or LPM_VIEW_GENERIC. */
    LpmViewType  view_type;
    /*! Minimal horizontal resolution in number of pixels per meter. */
    unsigned int min_horizontal_resolution;
    /*! Maximal horizontal resolution in number of pixels per meter. */
    unsigned int max_horizontal_resolution;
    /*! Camera pixel density ratio which is defined as vertical pixel density / horizontal pixel density.
    For standard cameras with square pixels, use value 1. */
    float        density_ratio;
} LpmCameraViewParams;


/*! Extension of the configuration for module initialization */
typedef struct
{
    /*! Filename of the module's configuration file (config.ini by default if NULL). */
    const char *lpm_config_filename;
    /*! Specifies if the OCR computation should be done on a CPU=0 or a GPU=1. */
    int         ocr_compute_on_gpu;
    /*! GPU device identifier (used only when the computation is running on a GPU) for the OCR. */
    int         ocr_gpu_device_id;
    /*! Specifies the number of threads available for the OCR (used only when the computation is running on a CPU).
    Uses approximately 90% of logical processors if set to 0 or negative. */
    int         ocr_num_threads;
    /*! If set to 1, the OCR submodule will not be loaded and will not be available. */
    int         disable_ocr;
    /*! Filename of the detector's configuration file (config-det.ini by default if NULL). */
    const char *det_config_filename;
    /*! Specifies if the computation should be done on CPU=0 or a GPU=1 for the detector. */
    int         det_compute_on_gpu;
    /*! GPU device identifier (used only when the computation of the detector is running on a GPU). */
    int         det_gpu_device_id;
    /*! Specifies number of threads available for the detector (used only when computation runs on CPU).
    Uses approximately 90% of logical processors if set to 0 or negative. */
    int         det_num_threads;
    /*! If 1, the detection submodule will not be loaded and available. If set to 0, it has no effect. */
    int         disable_det;
    /*! General void pointer allocated for future use, must be NULL if not in use. */
    void       *extras;
} LpmModuleConfig_extension1;


/*! Configuration for module initialization */
typedef struct
{
    /*! DEPRECATED specifies whether the computation should be done on a CPU=0 or a GPU=1. */
    int     compute_on_gpu;
    /*! DEPRECATED GPU device identifier (used only when the computation is running on a GPU). */
    int     gpu_device_id;
    /*! Extension of the configuration structure, must be NULL if not in use. Used in version 7.3 and higher. */
    LpmModuleConfig_extension1 *extras;
} LpmModuleConfig;


/*! Module Info */
typedef struct
{
    /*! Full name of the module. */
    char    name[LPM_MAX_STR_LEN];
    /*! Id of the module. */
    int     id;
    /*! Release date of the module in YYYY-mm-dd format. */
    char    date[LPM_MAX_STR_LEN];
    /*! Full path to the module. */
    char    path[LPM_MAX_PATH_LEN];
    /*! Version number of the module. */
    int     version;
    /*! Subversion number of the module. */
    int     subversion;
    /*! Char array with the detector type ("frontal", "generic", "lfrontal"). */
    char    det_type[LPM_MAX_STR_LEN];
    /*! Char array with the type of the detected object ("license plates", "adr plates", ...). */
    char    obj_type[LPM_MAX_STR_LEN];
    /*! Char array with the recognition type ("ceu3", "cz", "adr", "vcl", ...). */
    char    rcg_type[LPM_MAX_STR_LEN];
    /*! Input image type (e.g. "ERImage"). */
    char    input_img_type[LPM_MAX_STR_LEN]; 
    /*! Desired pixels aspect ratio of input images. */
    double  pxl_aspect_ratio;
    /*! Supported LP countries codes returned as a comma separated list (e.g. "CZ,SK,A" ). */
    char    lp_countries[LPM_MAX_STR_LEN];
    /*! Required LP width range.  */
    int     lp_min_mean_max_width[3];
    /*! Required LP height range. */
    int     lp_min_mean_max_height[3];
    /*! Range of LP inplane rotation. */
    double  lp_min_mean_max_rotation[3];
    /*! Switch indicating whether the module is active or not. */
    int     is_active;
    /*! Module properties in the LpmPropertyFlags structure. See header file lpm_types.h for more information. */
    LpmPropertyFlags prop;
    /*! Information about the license in the LpmLicenseInfo struct. */
    LpmLicenseInfo  *license_info;
} LpmModuleInfo;

/*!
 @} 
 */



/* ======================================================== */
/*                                                          */
/*  DETECTION STRUCTURE DEFINITIONS                         */
/*                                                          */
/*                                                          */
/* ======================================================== */

/*! \defgroup LPM_TYPES_DET Detection types
 @{
*/

/*! Detection label */
typedef enum
{
    /*! Default label value for generic usage. */
    LPM_LABEL_DEFAULT = 0,

    /*! Generic person object. */
    LPM_LABEL_PERSON = 200,

    /*! Generic license plate. */
    LPM_LABEL_LP = 1000,
    /*! European license plate. */
    LPM_LABEL_LP_EU_ONE_LINE = 1001,
    /*! European multiline license plate. */
    LPM_LABEL_LP_EU_MULTI_LINE = 1002,

    /*! North American license plate. */
    LPM_LABEL_LP_NORTH_AMERICA = 1200,

    /*! Asian license plate. */
    LPM_LABEL_LP_ASIA_PACIFIC = 1300,
    /*! Middle Eastern license plate. */
    LPM_LABEL_LP_MIDDLE_EAST = 1400,

    /*! ADR (the European Agreement on International Carriage of Dangerous Goods by Road). */
    LPM_LABEL_ADR = 2000,
    /*! ADR with text. */
    LPM_LABEL_ADR_STRING = 2001,
    /*! Empty ADR. */
    LPM_LABEL_ADR_EMPTY = 2002,

    /*! Plate indicating trash load. */
    LPM_LABEL_TRASH = 2100,
    /*! Speed limit sticker. */
    LPM_LABEL_SPEED_LIMIT = 2200,
    /*! Oversize load sign. */
    LPM_LABEL_OVERSIZE_LOAD = 2210,
    /*! Vignette sticker. */
    LPM_LABEL_VIGNETTE = 2300,

    /*! General vehicle bounding box. */
    LPM_LABEL_VEHICLE = 3000,
    /*! Frontal vehicle bounding box. */
    LPM_LABEL_VEHICLE_FRONT = 3001,
    /*! Rear vehicle bounding box. */
    LPM_LABEL_VEHICLE_REAR = 3002,

    /*! Vehicle windshield. */
    LPM_LABEL_VEHICLE_WINDSHIELD = 3010,
    /*! Vehicle wheel. */
    LPM_LABEL_VEHICLE_WHEEL = 3020,
} LpmDetectionLabel;


/*! Bounding-box coordinates of a detection area */
typedef struct
{
    /*! Top left column */
    float top_left_col;
    /*! Top left row */
    float top_left_row;
    /*! Top right column */
    float top_right_col;
    /*! Top right row */
    float top_right_row;
    /*! Bottom left column */
    float bot_left_col;
    /*! Bottom left row */
    float bot_left_row;
    /*! Bottom right column */
    float bot_right_col;
    /*! Bottom right row */
    float bot_right_row;
} LpmBoundingBox;


/*! Data related to a single license plate detection. 
    \see LpmOcrResult */
typedef struct
{
    /*! License plate detection confidence factor. */
    double              confidence;
    /*! License plate position. */
    LpmBoundingBox      position;
    /*! Detection type label. */
    LpmDetectionLabel   label;
    /*! The image crop of the detection. Please note that generation of this image can be disabled in configuration files. */
    ERImage             image;
    /*! Array with affine mapping from plate image coordinates to source image coordinates.
    The array contains the first three rows of the affine transformation matrix (a 3x2 matrix), saved row-wise.
    Example: (x, y, 1) * affine_matrix will transform (x, y) in crop coordinates to the corresponding coordinates in the source image. */
    double              affine_mapping[6];
} LpmDetection;


/*! Data related to a single license plate detection */
typedef struct
{
    /*! Specifies how much the detection is occluded.
    Negative value - not known, 0.0f - not occluded, 1.0f - fully occluded. */
    float   occlusion;
    /*! Contains -1 if not known, 0 if the detection is not truncated, 1 if it is truncated (the bbox does not cover the whole object). */
    int     truncated;
    /*! ID of the cluster, to which this detection belongs.
    -1 if the cluster is not known, 0 means undefined, ID starts generally at 1.
    Detections of objects which are physically connected have same cluster_id,
    for example bounding box and license plate of same car will have same cluster_id. */
    int     cluster_id;
    /*! Confidence factor for cluster_id prediction. */
    double  cluster_confidence;
    /*! General void pointer allocated for future use, NULL if not in use. */
    void   *extras;
} LpmDetection_extension1;


/*! Detection result structure extension for holding additional detection information */
typedef struct
{
    /*! An array of additional information for detections. */
    LpmDetection_extension1 *detections;
    /*! General void pointer allocated for future use. */
    void *extras;
} LpmDetResult_extension1;


/*! Detection result structure. Holds an array of all license plate detections */
typedef struct
{
    /*! ID of the used lpm module. */
    int             lpm_id;
    /*! Index of the used lpm module. */
    int             lpm_idx;
    /*! Number of detections. */
    int             num_detections; 
    /*! Array of detections. */
    LpmDetection   *detections;
    /*! Additional details for detections, NULL if not in use. Used in version 7.3 and higher. */
    LpmDetResult_extension1 *extras;
} LpmDetResult;

/*!
 @} 
 */



/* ======================================================== */
/*                                                          */
/*  OCR STRUCTURE DEFINITIONS                               */
/*                                                          */
/*                                                          */
/* ======================================================== */

/*! \defgroup LPM_TYPES_OCR Ocr types
 @{
*/

/*! Structure for a single text line */
typedef struct
{
    /*! Output confidence for the whole line. */
    double          line_confidence;
    /*! Text length (i.e. number of characters). */
    unsigned int    length;
    /*! Text in Unicode (UTF-32) of length-many characters. */
    int            *characters;
    /*! Array of length-many items containing the confidence for each character. */
    double         *characters_confidences;
} LpmTextLine;

/*! Structure for holding a physical width and height of a license plate in mm.
\see LpmOcrHypothesis */
typedef struct
{
    /*! Physical width of the license plate in mm. */
    unsigned int    physical_width;
    /*! Physical height of the license plate in mm. */
    unsigned int    physical_height;
} LpmLpDimensions;


/*! Detection result structure extension for holding additional ANPR information */
typedef struct
{
    /*! Represents the readability score of the license plate.
        A value between 0 and 1, where 1 indicates the plate is unreadable, and 0 means it is readable.
        If the value is below 0, it indicates that the prediction is not available. */
    double      unreadable;
    /*! Indicates the degree of obstruction of the license plate.
        A value between 0 and 1, where 1 denotes that the plate is fully obstructed, and 0 implies it is not obstructed.
        If the value is below 0, it indicates that the prediction is not available. */
    double      obstructed;
    /*! General void pointer allocated for future use. */
    void       *extras;
} LpmOcrHypothesis_extension1;


/*! Structure for a single OCR hypothesis */
typedef struct
{
    /*! Confidence factor for the current OCR result. */
    double          confidence; 
    /*! Number of license/ADR plate text lines. */
    unsigned int    num_lines;
    /*! Array of text lines of type LpmTextLine of the current license/ADR plate. */
    LpmTextLine    *text_lines;
    /*! A NULL-terminated string pointing to international license plate code.
    Note: When reading ADR plates, the value is "ADR" or "TRASH".
    Note: If the value is "UNK", then it was recognized as a false positive detection. */
    char           *plate_type;
    /*! Confidence for the plate type prediction. */
    double          plate_type_confidence;
    /*! Predicted physical dimensions of the license plate. */
    LpmLpDimensions lp_dimensions;
    /*! Confidence factor for the dimensions prediction. */
    double          lp_dimensions_confidence;
    /*! Additional details for OCR, NULL if not in use. Used in version 7.6 and higher. */
    LpmOcrHypothesis_extension1 *extras;
} LpmOcrHypothesis;


/*! OCR result structure. Holds an array of hypotheses related to a single detected license plate. 
\see LpmOcrHypothesis */
typedef struct
{
    /*! ID of the used LPM module. */
    int               lpm_id;
    /*! Index of the used LPM module. */
    int               lpm_idx;
    /*! Number of OCR-hypotheses. */
    unsigned int      num_hypotheses;
    /*! Array  of OCR-hypotheses. */
    LpmOcrHypothesis *hypotheses;
} LpmOcrResult;

/*!
 @} 
 */


/*!
 @} @}
 */

#endif
