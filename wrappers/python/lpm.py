import weakref

from cffi import FFI
from enum import IntEnum


global_weakkeydict = weakref.WeakKeyDictionary()


class LPMError(Exception):
    """LMP Error class."""

    def __init__(self, internal_function_name, internal_error_code):
        self.internal_function_name = internal_function_name
        self.internal_error_code = internal_error_code


class LpmDateTime:
    """Mirror of LpmDateTime structure."""

    def __init__(self):
        self.year = 0
        self.month = 0
        self.day_of_month = 0
        self.hour = 0
        self.minute = 0
        self.second = 0

    def c_init(self, ffi: FFI, c_structure):
        """
        Fills this mirror structure with given C structure data.
        :param ffi: Instance of the FFI class.
        :param c_structure: C structure data.
        """
        if c_structure == ffi.NULL:
            return

        self.year = c_structure.year
        self.month = c_structure.month
        self.day_of_month = c_structure.day_of_month
        self.hour = c_structure.hour
        self.minute = c_structure.minute
        self.second = c_structure.second


class LpmLicenseInfo:
    """Mirror of LpmLicenseInfo structure."""

    def __init__(self):
        self.is_valid = 0
        self.expiration_date = LpmDateTime()
        self.is_using_counter = 0
        self.executions_left = 0

    def c_init(self, ffi: FFI, c_structure):
        """
        Fills this mirror structure with given C structure data.
        :param ffi: Instance of the FFI class.
        :param c_structure: C structure data.
        """
        if c_structure == ffi.NULL:
            return

        self.is_valid = c_structure.is_valid
        self.expiration_date.c_init(ffi, c_structure.expiration_date)
        self.is_using_counter = c_structure.is_using_counter
        self.executions_left = c_structure.executions_left


class LpmViewType(IntEnum):
    """Mirror of LpmViewType enum."""
    LPM_VIEW_FRONTAL = 0
    LPM_VIEW_GENERIC = 1


class LpmCameraViewParams:
    """Mirror of LpmCameraViewParams structure."""

    def __init__(self):
        self.view_type = LpmViewType.LPM_VIEW_GENERIC.value
        self.min_horizontal_resolution = 0
        self.max_horizontal_resolution = 0
        self.density_ratio = 0

    def c_init(self, ffi: FFI, c_structure):
        """
        Fills this mirror structure with given C structure data.
        :param ffi: Instance of the FFI class.
        :param c_structure: C structure data.
        """
        if c_structure == ffi.NULL:
            return

        self.view_type = c_structure.view_type
        self.max_horizontal_resolution = c_structure.max_horizontal_resolution
        self.min_horizontal_resolution = c_structure.min_horizontal_resolution
        self.density_ratio = c_structure.density_ratio

    def get_c(self, ffi: FFI):
        """
        Converts this Python structure into a C structure.
        :param ffi: The FFI to use for creation of the C structure.
        :return: The resulting C structure.
        """
        c_structure = ffi.new("LpmCameraViewParams *")
        c_structure.view_type = ffi.cast("LpmViewType", self.view_type)
        c_structure.min_horizontal_resolution = ffi.cast("unsigned int", self.min_horizontal_resolution)
        c_structure.max_horizontal_resolution = ffi.cast("unsigned int", self.max_horizontal_resolution)
        c_structure.density_ratio = ffi.cast("float", self.density_ratio)

        return c_structure


class LpmModuleConfig:
    """Mirror of LpmModuleConfig merged with LpmModuleConfig_extension1 structure."""

    def __init__(self):
        # Deprecated
        self.compute_on_gpu = False
        self.gpu_device_id = 0
        # Extension parameters
        self.lpm_config_filename = None
        self.ocr_compute_on_gpu = False
        self.ocr_gpu_device_id = 0
        self.ocr_num_threads = 0
        self.disable_ocr = False
        self.det_config_filename = None
        self.det_compute_on_gpu = False
        self.det_gpu_device_id = 0
        self.det_num_threads = 0
        self.disable_det = False
        self.extras = False

    def get_c(self, ffi: FFI):
        """
        Converts this Python structure into a C structure.
        :param ffi: The FFI to use for creation of the C structure.
        :return: The resulting C structure.
        """
        c_structure = ffi.new("LpmModuleConfig *")
        c_structure.compute_on_gpu = ffi.cast("int", self.compute_on_gpu)
        c_structure.gpu_device_id = ffi.cast("int", self.gpu_device_id)
        # Config extension structure
        c_extension = ffi.new("LpmModuleConfig_extension1 *")
        c_lpm_config_filename = ffi.NULL
        if self.lpm_config_filename is not None and type(self.lpm_config_filename) == str:
            c_lpm_config_filename = ffi.new("const char[]", str.encode(self.lpm_config_filename))
        c_extension.lpm_config_filename = c_lpm_config_filename
        c_extension.ocr_compute_on_gpu = ffi.cast("int", self.ocr_compute_on_gpu)
        c_extension.ocr_gpu_device_id = ffi.cast("int", self.ocr_gpu_device_id)
        c_extension.ocr_num_threads = ffi.cast("int", self.ocr_num_threads)
        c_extension.disable_ocr = ffi.cast("int", self.disable_ocr)
        c_det_config_filename = ffi.NULL
        if self.det_config_filename is not None and type(self.det_config_filename) == str:
            c_det_config_filename = ffi.new("const char[]", str.encode(self.det_config_filename))
        c_extension.det_config_filename = c_det_config_filename
        c_extension.det_compute_on_gpu = ffi.cast("int", self.det_compute_on_gpu)
        c_extension.det_gpu_device_id = ffi.cast("int", self.det_gpu_device_id)
        c_extension.det_num_threads = ffi.cast("int", self.det_num_threads)
        c_extension.disable_det = ffi.cast("int", self.disable_det)
        c_structure.extras = c_extension
        # Prevent garbage-collection of the allocated structures
        global_weakkeydict[c_structure] = (c_extension, c_lpm_config_filename, c_det_config_filename)

        return c_structure


class LpmModuleInfo:
    """Mirror of LpmModuleInfo structure."""

    def __init__(self):
        self.name = ""
        self.id = 0
        self.date = ""
        self.path = ""
        self.version = 0
        self.subversion = 0
        self.det_type = ""
        self.obj_type = ""
        self.rcg_type = ""
        self.input_img_type = ""
        self.pxl_aspect_ratio = 0
        self.lp_countries = ""
        self.lp_min_mean_max_width = []
        self.lp_min_mean_max_height = []
        self.lp_min_mean_max_rotation = []
        self.is_active = 0
        self.prop = 0
        self.license_info = LpmLicenseInfo()

    def c_init(self, ffi: FFI, c_structure):
        """
        Fills this mirror structure with given C structure data.
        :param ffi: Instance of the FFI class.
        :param c_structure: C structure data.
        """
        if c_structure == ffi.NULL:
            return

        self.name = ffi.string(c_structure.name).decode("utf-8")
        self.id = c_structure.id
        self.date = ffi.string(c_structure.date).decode("utf-8")
        self.path = ffi.string(c_structure.path).decode("utf-8")
        self.version = c_structure.version
        self.subversion = c_structure.subversion
        self.det_type = ffi.string(c_structure.det_type).decode("utf-8")
        self.obj_type = ffi.string(c_structure.obj_type).decode("utf-8")
        self.rcg_type = ffi.string(c_structure.rcg_type).decode("utf-8")
        self.input_img_type = ffi.string(c_structure.input_img_type).decode("utf-8")
        self.pxl_aspect_ratio = c_structure.pxl_aspect_ratio
        self.lp_countries = ffi.string(c_structure.lp_countries).decode("utf-8")
        self.lp_min_mean_max_width = [c for c in c_structure.lp_min_mean_max_width]
        self.lp_min_mean_max_height = [c for c in c_structure.lp_min_mean_max_height]
        self.lp_min_mean_max_rotation = [c for c in c_structure.lp_min_mean_max_rotation]
        self.is_active = c_structure.is_active
        self.prop = c_structure.prop
        self.license_info.c_init(ffi, c_structure.license_info)


class LpmDetectionLabel(IntEnum):
    """Mirror of LpmDetectionLabel enum."""
    LPM_LABEL_DEFAULT = 0

    LPM_LABEL_PERSON = 200

    LPM_LABEL_LP = 1000
    LPM_LABEL_LP_EU_ONE_LINE = 1001
    LPM_LABEL_LP_EU_MULTI_LINE = 1002

    LPM_LABEL_LP_NORTH_AMERICA = 1200

    LPM_LABEL_LP_ASIA_PACIFIC = 1300
    LPM_LABEL_LP_MIDDLE_EAST = 1400

    LPM_LABEL_ADR = 2000
    LPM_LABEL_ADR_STRING = 2001
    LPM_LABEL_ADR_EMPTY = 2002

    LPM_LABEL_TRASH = 2100
    LPM_LABEL_SPEED_LIMIT = 2200
    LPM_LABEL_OVERSIZE_LOAD = 2210
    LPM_LABEL_VIGNETTE = 2300

    LPM_LABEL_VEHICLE = 3000
    LPM_LABEL_VEHICLE_FRONT = 3001
    LPM_LABEL_VEHICLE_REAR = 3002

    LPM_LABEL_VEHICLE_WINDSHIELD = 3010
    LPM_LABEL_VEHICLE_WHEEL = 3020
    

class LpmBoundingBox:
    """Mirror of LpmBoundingBox structure."""

    def __init__(self):
        self.top_left_col = 0
        self.top_left_row = 0
        self.top_right_col = 0
        self.top_right_row = 0
        self.bot_left_col = 0
        self.bot_left_row = 0
        self.bot_right_col = 0
        self.bot_right_row = 0

    def c_init(self, ffi: FFI, c_structure):
        """
        Fills this mirror structure with given C structure data.
        :param ffi: Instance of the FFI class.
        :param c_structure: C structure data.
        """
        if c_structure == ffi.NULL:
            return

        self.top_left_col = c_structure.top_left_col
        self.top_left_row = c_structure.top_left_row
        self.top_right_col = c_structure.top_right_col
        self.top_right_row = c_structure.top_right_row
        self.bot_left_col = c_structure.bot_left_col
        self.bot_left_row = c_structure.bot_left_row
        self.bot_right_col = c_structure.bot_right_col
        self.bot_right_row = c_structure.bot_right_row

    def get_c(self, ffi: FFI):
        """
        Converts this Python structure into a C structure.
        :param ffi: The FFI to use for creation of the C structure.
        :return: The resulting C structure
        """
        c_structure = ffi.new("LpmBoundingBox *")
        c_structure.top_left_col = ffi.cast("float", self.top_left_col)
        c_structure.top_left_row = ffi.cast("float", self.top_left_row)
        c_structure.top_right_col = ffi.cast("float", self.top_right_col)
        c_structure.top_right_row = ffi.cast("float", self.top_right_row)
        c_structure.bot_left_col = ffi.cast("float", self.bot_left_col)
        c_structure.bot_left_row = ffi.cast("float", self.bot_left_row)
        c_structure.bot_right_col = ffi.cast("float", self.bot_right_col)
        c_structure.bot_right_row = ffi.cast("float", self.bot_right_row)

        return c_structure


class LpmDetection:
    """Mirror of LpmDetection & LpmDetection_extension1 structure."""

    def __init__(self):
        self.confidence = 0
        self.position = LpmBoundingBox()
        self.label = LpmDetectionLabel.LPM_LABEL_DEFAULT
        self.image = None
        self.affine_mapping = []
        self.occlusion = -1
        self.truncated = -1
        self.cluster_id = -1
        self.cluster_confidence = 0

    def c_init(self, ffi: FFI, c_structure, c_structure_extension1):
        """
        Fills this mirror structure with given C structure data.
        :param ffi: Instance of the FFI class.
        :param c_structure: C structure data.
        :param c_structure_extension1: C structure extension data.
        """
        if c_structure == ffi.NULL:
            return

        self.confidence = c_structure.confidence
        self.position.c_init(ffi, c_structure.position)
        self.label = LpmDetectionLabel(int(c_structure.label))
        self.image = c_structure.image
        self.affine_mapping = []
        for c_affine in c_structure.affine_mapping:
            self.affine_mapping.append(c_affine)
        if c_structure_extension1 != ffi.NULL:
            self.occlusion = c_structure_extension1.occlusion
            self.truncated = c_structure_extension1.truncated
            self.cluster_id = c_structure_extension1.cluster_id
            self.cluster_confidence = c_structure_extension1.cluster_confidence


class LpmDetResult:
    """Mirror of LpmDetResult structure."""

    def __init__(self):
        self.lpm_id = 0
        self.lpm_idx = 0
        self.num_detections = 0
        self.detections = []
        self.extras = None

    def c_init(self, ffi: FFI, c_structure):
        """
        Fills this mirror structure with given C structure data.
        :param ffi: Instance of the FFI class.
        :param c_structure: C structure data.
        """
        if c_structure == ffi.NULL:
            return

        self.lpm_id = c_structure.lpm_id
        self.lpm_idx = c_structure.lpm_idx
        self.num_detections = c_structure.num_detections
        self.detections = []
        c_detection_result_extension1 = ffi.NULL
        if c_structure.extras != ffi.NULL:
            c_detection_result_extension1 = ffi.new("LpmDetResult_extension1 *")
            ffi.memmove(c_detection_result_extension1, c_structure.extras, ffi.sizeof("LpmDetResult_extension1"))
        for i in range(c_structure.num_detections):
            c_detection = ffi.new("LpmDetection *")
            ffi.memmove(c_detection, c_structure.detections + i, ffi.sizeof("LpmDetection"))
            detection = LpmDetection()
            if c_detection_result_extension1 != ffi.NULL:
                detection_extension1 = c_detection_result_extension1[0]
                c_detection_extension1 = ffi.new("LpmDetection_extension1 *")
                ffi.memmove(c_detection_extension1,
                            detection_extension1.detections + i, ffi.sizeof("LpmDetection_extension1"))
                detection.c_init(ffi, c_detection, c_detection_extension1)
            else:
                detection.c_init(ffi, c_detection, ffi.NULL)
            self.detections.append(detection)


class LpmTextLine:
    """Mirror of LpmTextLine structure."""

    def __init__(self):
        self.line_confidence = 0
        self.length = 0
        self.characters = ""
        self.characters_confidences = []

    def c_init(self, ffi: FFI, c_structure):
        """
        Fills this mirror structure with given C structure data.
        :param ffi: Instance of the FFI class.
        :param c_structure: C structure data.
        """
        if c_structure == ffi.NULL:
            return

        self.line_confidence = c_structure.line_confidence
        self.length = c_structure.length

        # Decode UTF-32 string from integers
        ints = []
        self.characters_confidences = []
        for i in range(self.length):
            ints.append(c_structure.characters[i])
            self.characters_confidences.append(c_structure.characters_confidences[i])
        self.characters = "".join(map(chr, ints))


class LpmLpDimensions:
    """Mirror of LpmLpDimensions structure."""

    def __init__(self):
        self.physical_width = 0
        self.physical_height = 0

    def c_init(self, ffi: FFI, c_structure):
        """
        Fills this mirror structure with given C structure data.
        :param ffi: Instance of the FFI class.
        :param c_structure: C structure data.
        """
        if c_structure == ffi.NULL:
            return

        self.physical_width = c_structure.physical_width
        self.physical_height = c_structure.physical_height


class LpmOcrHypothesis:
    """Mirror of LpmOcrHypothesis structure."""

    def __init__(self):
        self.confidence = 0
        self.num_lines = 0
        self.text_lines = []
        self.plate_type = ""
        self.plate_type_confidence = 0
        self.lp_dimensions = LpmLpDimensions()
        self.lp_dimensions_confidence = 0
        self.obstructed = -1
        self.unreadable = -1
        self.extras = None

    def c_init(self, ffi: FFI, c_structure):
        """
        Fills this mirror structure with given C structure data.
        :param ffi: Instance of the FFI class.
        :param c_structure: C structure data.
        """
        if c_structure == ffi.NULL:
            return

        self.confidence = c_structure.confidence
        self.num_lines = c_structure.num_lines
        self.text_lines = []
        for i in range(c_structure.num_lines):
            c_text_line = ffi.new("LpmTextLine *")
            ffi.memmove(c_text_line, c_structure.text_lines + i, ffi.sizeof("LpmTextLine"))
            text_line = LpmTextLine()
            text_line.c_init(ffi, c_text_line)
            self.text_lines.append(text_line)
        if ffi.NULL != c_structure.plate_type:
            self.plate_type = ffi.string(c_structure.plate_type).decode("utf-8")
        self.plate_type_confidence = c_structure.plate_type_confidence
        self.lp_dimensions.c_init(ffi, c_structure.lp_dimensions)
        self.lp_dimensions_confidence = c_structure.lp_dimensions_confidence
        c_ocr_hypothesis_extension1 = ffi.NULL
        if c_structure.extras != ffi.NULL:
            c_ocr_hypothesis_extension1 = ffi.new("LpmOcrHypothesis_extension1 *")
            ffi.memmove(c_ocr_hypothesis_extension1, c_structure.extras, ffi.sizeof("LpmOcrHypothesis_extension1"))
        if c_ocr_hypothesis_extension1 != ffi.NULL:
            self.obstructed = c_ocr_hypothesis_extension1.obstructed
            self.unreadable = c_ocr_hypothesis_extension1.unreadable


class LpmOcrResult:
    """Mirror of LpmOcrResult structure."""

    def __init__(self):
        self.lpm_id = 0
        self.lpm_idx = 0
        self.num_hypotheses = 0
        self.hypotheses = []

    def c_init(self, ffi: FFI, c_structure):
        """
        Fills this mirror structure with given C structure data.
        :param ffi: Instance of the FFI class.
        :param c_structure: C structure data.
        """
        if c_structure == ffi.NULL:
            return

        self.lpm_id = c_structure.lpm_id
        self.lpm_idx = c_structure.lpm_idx
        self.num_hypotheses = c_structure.num_hypotheses
        self.hypotheses = []
        for i in range(c_structure.num_hypotheses):
            c_hypothesis = ffi.new("LpmOcrHypothesis *")
            ffi.memmove(c_hypothesis, c_structure.hypotheses + i, ffi.sizeof("LpmOcrHypothesis"))
            hypothesis = LpmOcrHypothesis()
            hypothesis.c_init(ffi, c_hypothesis)
            self.hypotheses.append(hypothesis)


class LPM:
    """
    Python wrapper class for LPM
    """

    @staticmethod
    def __define_ffi(ffi):
        # type definitions from lpm_type.h
        ffi.cdef("""
                typedef void **LPMState;
        """)
        ffi.cdef("""
                typedef struct
                {
                    unsigned int  year;
                    unsigned char month;
                    unsigned char day_of_month;
                    unsigned char hour;
                    unsigned char minute;
                    unsigned char second;
                } LpmDateTime;
        """)
        ffi.cdef("""
                typedef struct
                {
                    /*! Flag determining whether the license is valid or not. */
                    int is_valid;
                    /*! License expiration date. License is time-unlimited if it contains only zeros. */
                    LpmDateTime expiration_date;
                    /*! The counter is enabled if non-zero. */
                    int is_using_counter;
                    /*! Number of module executions left. License is exe-unlimited if is_using_counter is zero. */
                    unsigned long executions_left;
                } LpmLicenseInfo;
        """)
        ffi.cdef("""
                #define LPM_MAX_PATH_LEN        4096
                #define LPM_MAX_STR_LEN         256
                
                #define LPM_EXTENSIONS_v7_3     1
        """)
        ffi.cdef("""
                typedef long long LpmPropertyFlags;
        """)
        ffi.cdef("""
                typedef enum
                {
                    /*! For EyeScan detector */
                    LPM_VIEW_FRONTAL = 0,
                    /*! For MSER + WaldBoost detector */
                    LPM_VIEW_GENERIC = 1
                } LpmViewType;
        """)
        ffi.cdef("""
                typedef struct
                {
                    /*! LPM_VIEW_FRONTAL or LPM_VIEW_GENERIC */
                    LpmViewType  view_type;
                    /*! Minimal horizontal resolution in number of pixels per meter */
                    unsigned int min_horizontal_resolution;
                    /*! Maximal horizontal resolution in number of pixels per meter */
                    unsigned int max_horizontal_resolution;
                    /*! Density ratio = vertical pixel density / horizontal pixel density */
                    float        density_ratio;
                } LpmCameraViewParams;
        """)
        ffi.cdef("""
                typedef struct
                {
                    /*! Filename of the module's configuration file (config.ini by default if NULL). */
                    const char *lpm_config_filename;
                    /*! Specifies if the computation should be done on CPU=0 or a GPU=1 for the OCR */
                    int         ocr_compute_on_gpu;
                    /*! GPU device identifier (used only when the computation is running on a GPU) for the OCR */
                    int         ocr_gpu_device_id;
                    /*! Specifies number of threads available for the OCR (used only when computation runs on CPU). Use all logical processors if 0 or below. */
                    int         ocr_num_threads;
                    /*! If 1, the OCR submodule will not be loaded and available. If set to 0, it has no effect. */
                    int         disable_ocr;
                    /*! Filename of the detector's configuration file (config-det.ini by default if NULL). */
                    const char *det_config_filename;
                    /*! Specifies if the computation should be done on CPU=0 or a GPU=1 for the detector */
                    int         det_compute_on_gpu;
                    /*! GPU device identifier (used only when the computation of the detector is running on a GPU) */
                    int         det_gpu_device_id;
                    /*! Specifies number of threads available for the detector (used only when computation runs on CPU). Use all logical processors if 0 or below. */
                    int         det_num_threads;
                    /*! If 1, the detection submodule will not be loaded and available. If set to 0, it has no effect. */
                    int         disable_det;
                    /*! General void pointer allocated for future use, must be NULL if not in use */
                    void       *extras;
                } LpmModuleConfig_extension1;
        """)
        ffi.cdef("""
                typedef struct
                {
                    /*! Specifies if the computation should be done on CPU=0 or a GPU=1 */
                    int compute_on_gpu;
                    /*! GPU device identifier (used only when the computation is running on a GPU) */
                    int gpu_device_id;
                    /*! Extension of the configuration structure, must be NULL if not in use */
                    LpmModuleConfig_extension1 *extras;
                } LpmModuleConfig;
        """)
        ffi.cdef("""
                typedef struct
                {
                    /*!  Module full name */
                    char    name[LPM_MAX_STR_LEN];
                    /*!  Module ID */
                    int     id;
                    /*!  Module release date */
                    char    date[LPM_MAX_STR_LEN];
                    /*!  Module full path */
                    char    path[LPM_MAX_PATH_LEN];
                    /*!  Module version */
                    int     version;
                    /*!  Module subversion */
                    int     subversion;
                    /*!  Detector type (frontal, generic, lfrontal) */
                    char    det_type[LPM_MAX_STR_LEN];
                    /*!  Object type (lp, adr,...) */
                    char    obj_type[LPM_MAX_STR_LEN];
                    /*!  Recognition type (ceu3, cz, adr, vcl, ...)*/
                    char    rcg_type[LPM_MAX_STR_LEN];
                    /*!  Input image type (e.g. Gray-Uc) */
                    char    input_img_type[LPM_MAX_STR_LEN]; 
                    /*!  Pxl aspect ratio     */
                    double  pxl_aspect_ratio;
                    /*!  Supported LP countries (e.g. "CZ,SK,A" ) */
                    char    lp_countries[LPM_MAX_STR_LEN];
                    /*!  Required LP width range  */
                    int     lp_min_mean_max_width[3];
                    /*!  Required LP height range */
                    int     lp_min_mean_max_height[3];
                    /*!  Range of LP inplane rotation */
                    double  lp_min_mean_max_rotation[3];
                    /*!  Switch indicating whether the module is active or not */
                    int     is_active;
                    /*!  Module's property flags */
                    LpmPropertyFlags prop;
                    /*!  A pointer to the information about a module licensing, ie. expiration date.*/
                    LpmLicenseInfo  *license_info;
                } LpmModuleInfo;
        """)
        ffi.cdef("""
                typedef enum
                {
                    LPM_LABEL_DEFAULT = 0,

                    LPM_LABEL_LP = 1000,
                    LPM_LABEL_LP_EU_ONE_LINE = 1001,
                    LPM_LABEL_LP_EU_MULTI_LINE = 1002,
                
                    LPM_LABEL_LP_I_FRONTAL_ONE_LINE = 1010,
                
                    LPM_LABEL_LP_KZ_ONE_LINE = 1101,
                    LPM_LABEL_LP_KZ_MULTI_LINE = 1102,
                
                    LPM_LABEL_LP_NORTH_AMERICA = 1200,
                
                    LPM_LABEL_LP_ASIA_PACIFIC = 1300,
                    LPM_LABEL_LP_MIDDLE_EAST = 1400,
                
                    LPM_LABEL_ADR = 2000,
                    LPM_LABEL_ADR_STRING = 2001,
                    LPM_LABEL_ADR_EMPTY = 2002,
                
                    LPM_LABEL_TRASH = 2100,
                    LPM_LABEL_SPEED_LIMIT = 2200,
                    LPM_LABEL_OVERSIZE_LOAD = 2210,
                    LPM_LABEL_VIGNETTE = 2300,

                    LPM_LABEL_VEHICLE = 3000,
                    LPM_LABEL_VEHICLE_FRONT = 3001,
                    LPM_LABEL_VEHICLE_REAR = 3002,
                
                    LPM_LABEL_VEHICLE_WINDSHIELD = 3010,
                    LPM_LABEL_VEHICLE_WHEEL = 3020,
                } LpmDetectionLabel;
        """)
        ffi.cdef("""
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
        """)
        ffi.cdef("""
                typedef struct
                {
                    /*! License plate detection confidence factor */
                    double              confidence;
                    /*! License plate position */
                    LpmBoundingBox      position;
                    /*! Detection type label */
                    LpmDetectionLabel   label;
                    /*! The image cut of the detected license plate */
                    ERImage             image;
                    /*! Affine mapping from Lp-Image coordinates to source image coordinates */
                    double              affine_mapping[6];
                } LpmDetection;
        """)
        ffi.cdef("""
                typedef struct
                {
                    /*! Fraction of detection occlusion. Negative value: not known, 0.0f - not occluded, 1.0f - 100% occluded */
                    float   occlusion;
                    /*! Contains -1 if not known, 0 if the detection is not truncated, 1 if it is truncated (the bbox does not cover the whole object) */
                    int     truncated;
                    /*! ID of a cluster, to which this detection belongs. -1 if the cluster is not known, 0 is undefined, ID starts generalily at 1 */
                    int     cluster_id;
                    /*! Confidence factor for cluster_id prediction */
                    double  cluster_confidence;
                    /*! General void pointer allocated for future use, NULL if not in use yet */
                    void   *extras;
                } LpmDetection_extension1;
        """)
        ffi.cdef("""
                typedef struct
                {
                    /*! Array of 'num_detections' detections */
                    LpmDetection_extension1 *detections;
                    /*! General void pointer allocated for future use, NULL if not in use yet */
                    void *extras;
                } LpmDetResult_extension1;
        """)
        ffi.cdef("""
                typedef struct
                {
                    /*! ID of the used lpm module */
                    int             lpm_id;
                    /*! Index of the used lpm module */
                    int             lpm_idx;
                    /*! Number of detections */
                    int             num_detections; 
                    /*! Array of detections */
                    LpmDetection   *detections;
                    /*! Additional details for detections, NULL if not in use yet */
                    LpmDetResult_extension1 *extras;
                } LpmDetResult;
        """)
        ffi.cdef("""
                typedef struct
                {
                    /*! Confidence of the line */
                    double          line_confidence;
                    /*! Text length (i.e. number of chars) */  
                    unsigned int    length;
                    /*! Text in unicode (UTF-32) */
                    int            *characters;
                    /*! Array of char confidences for each char */
                    double         *characters_confidences;
                } LpmTextLine;
        """)
        ffi.cdef("""
                typedef struct
                {
                    /*! Physical width of the license plate in mm */
                    unsigned int    physical_width;
                    /*! Physical height of the license plate in mm */
                    unsigned int    physical_height;
                } LpmLpDimensions;
        """)
        ffi.cdef("""
                typedef struct
                {
                    /*! Represents the readability score of the license plate.
                        A value between 0 and 1, where 1 indicates the plate is unreadable, and 0 means it is readable.
                        If the value is below 0, it indicates that the prediction is not available. */
                    double      unreadable;
                    /*! Indicates the degree of obstruction of the license plate.
                        A value between 0 and 1, where 1 denotes that the plate is fully obstructed, and 0 implies it 
                        is not obstructed.
                        If the value is below 0, it indicates that the prediction is not available. */
                    double      obstructed;
                    /*! General void pointer allocated for future use. */
                    void       *extras;
                } LpmOcrHypothesis_extension1;
        """)
        ffi.cdef("""
                typedef struct
                {
                    /*! Confidence factor for the current OCR result */
                    double          confidence; 
                    /*! Number of license plate text lines. */
                    unsigned int    num_lines;
                    /*! Array of text lines of the current license plate. */
                    LpmTextLine    *text_lines;
                    /*! A NULL-terminated string pointing to international license plate code */
                    char           *plate_type;
                    /*! Confidence factor for the plate type prediction */
                    double          plate_type_confidence;
                    /*! Predicted physical dimensions of the license plate */
                    LpmLpDimensions lp_dimensions;
                    /*! Confidence factor for the dimensions prediction */
                    double          lp_dimensions_confidence;
                    /*! Additional details for OCR, NULL if not in use. Used in version 7.6 and higher. */
                    LpmOcrHypothesis_extension1 *extras;
                } LpmOcrHypothesis;
        """)
        ffi.cdef("""
                typedef struct
                {
                    /*! ID of the used lpm module */
                    int               lpm_id;
                    /*! Index of the used lpm module */
                    int               lpm_idx;
                    /*! Number of ocr-hypotheses */
                    int               num_hypotheses;
                    /*! Array  of ocr-hypotheses */
                    LpmOcrHypothesis *hypotheses;
                } LpmOcrResult;
        """)

        # Function definitions from lpm.h
        ffi.cdef("""
                int lpmInit(const char *lpm_directory, LPMState *lpm_state);
        """)
        ffi.cdef("""
                void lpmFree(LPMState *lpm_state);
        """)
        ffi.cdef("""
                unsigned long lpmVersion(void);
        """)
        ffi.cdef("""
                char *lpmCompilationDate(void);
        """)
        ffi.cdef("""
                int lpmLoadViewConfig(const char *filename, LpmCameraViewParams *camera_view_params);
        """)
        ffi.cdef("""
                int lpmWriteViewConfig(const char *filename, LpmCameraViewParams camera_view_params);
        """)
        ffi.cdef("""
                int lpmLoadModule(LPMState lpm_state, int module_index, LpmCameraViewParams *camera_view_params, 
                                  const LpmModuleConfig *module_config);
        """)
        ffi.cdef("""
                void lpmFreeModule(LPMState lpm_state, int module_index);
        """)
        ffi.cdef("""
                LpmDetResult *lpmRunDet(LPMState lpm_state, int module_index, ERImage image,
                                        const LpmBoundingBox *bounding_box);
        """)
        ffi.cdef("""
                void lpmFreeDetResult(LPMState lpm_state, LpmDetResult *detection_result);  
        """)
        ffi.cdef("""
                LpmOcrResult *lpmRunOcr(LPMState lpm_state, int module_index, ERImage image,
                                        const LpmBoundingBox *bounding_box, LpmDetectionLabel detection_label);
        """)
        ffi.cdef("""
                void lpmFreeOcrResult(LPMState lpm_state, LpmOcrResult *ocr_result);
        """)
        ffi.cdef("""
                int lpmGetNumAvlbModules(LPMState lpm_state);
        """)
        ffi.cdef("""
                int lpmGetModuleIndex(LPMState lpm_state, int module_id, int version, int subversion);
        """)
        ffi.cdef("""
                int lpmGetModuleIndexByName(LPMState lpm_state, const char *module_name);
        """)
        ffi.cdef("""
                LpmModuleInfo *lpmGetModuleInfo(LPMState lpm_state, int module_index);
        """)
        ffi.cdef("""
                int lpmGetLastError(void);
        """)
        ffi.cdef("""
                char *lpmGetErrorMsg(int errcode);
        """)
        ffi.cdef("""
                int lpmOpenErrorLog(const char *errlog_filename);
        """)
        ffi.cdef("""
                void lpmCloseErrorLog();
        """)

    def __init__(self, ffi: FFI, sdk_dir_path: str, support_libs: list = None) -> None:
        self.lpm_path = sdk_dir_path
        if type(self.lpm_path) is not str:
            raise TypeError("Variable lpm_path must be a string.")

        # Add function definitions to ffi
        self.__define_ffi(ffi)
        self.ffi = ffi

        # Load all the support libs
        if support_libs is not None:
            for lib in support_libs:
                ffi.dlopen(lib)

        # Load the main library
        self.__lpm = ffi.dlopen(self.lpm_path)

        self.__module_state = ffi.new("LPMState", ffi.NULL)

        # Holds loaded module indexes for deallocation of all modules
        self.module_indices = []

    def _free_lpm(self, _):
        for module_index in self.module_indices:
            self.free_module(module_index)
        self.__lpm.lpmFree(self.__module_state)

    def init_lpm(self, directory: str) -> None:
        # Unwrap the input parameters
        c_directory = self.ffi.new("const char []", directory.encode("utf-8"))
        self.__module_state = self.ffi.new("LPMState *")

        # Call the C function
        ret_code = self.__lpm.lpmInit(c_directory, self.__module_state)

        # Check the output
        if ret_code != 0 or self.__module_state[0] == self.ffi.NULL:
            raise LPMError("lpmInit", ret_code)

        # Ensure deallocation
        self.__module_state = self.ffi.gc(self.__module_state, self._free_lpm)

    def get_version(self):
        ulong = self.__lpm.lpmVersion()
        version = int(self.ffi.cast("unsigned char", ulong >> 8))
        subversion = int(self.ffi.cast("unsigned char", ulong))

        return "{}.{}".format(version, subversion)

    def get_compilation_date(self):
        # Call the C function
        comp_date_chars = self.__lpm.lpmCompilationDate()

        # Wrap the result
        comp_date = self.ffi.string(comp_date_chars).decode("utf-8")

        return comp_date

    def load_view_config(self, filename: str) -> LpmCameraViewParams:
        # Unwrap the input parameters
        c_filename = self.ffi.new("const char []", filename.encode("utf-8"))
        c_config = self.ffi.new("LpmCameraViewParams *")

        # Call the C function
        ret_code = self.__lpm.lpmLoadViewConfig(c_filename, c_config)

        # Check the output
        if ret_code != 0:
            raise LPMError("lpmLoadViewConfig", ret_code)

        # Wrap the result
        config = LpmCameraViewParams()
        config.c_init(self.ffi, c_config)

        return config

    def write_view_config(self, filename: str, config: LpmCameraViewParams) -> None:
        # Unwrap the input parameters
        c_filename = self.ffi.new("const char []", filename.encode("utf-8"))

        c_config = self.ffi.new("LpmCameraViewParams *")
        c_config.view_type = self.ffi.cast("LpmViewType", config.view_type)
        c_config.min_horizontal_resolution = self.ffi.cast("unsigned int", config.min_horizontal_resolution)
        c_config.max_horizontal_resolution = self.ffi.cast("unsigned int", config.max_horizontal_resolution)
        c_config.density_ratio = self.ffi.cast("float", config.density_ratio)

        # Call the C function
        ret_code = self.__lpm.lpmWriteViewConfig(c_filename, c_config[0])

        # Check the output
        if ret_code != 0:
            raise LPMError("lpmWriteViewConfig", ret_code)

    def load_module(self, module_index: int, view_params: LpmCameraViewParams, module_config: LpmModuleConfig) -> None:
        # Unwrap the input parameters
        if view_params is None:
            c_cvp = self.ffi.NULL
        else:
            c_cvp = view_params.get_c(self.ffi)
        if module_config is None:
            c_config = self.ffi.NULL
        else:
            c_config = module_config.get_c(self.ffi)

        # Call the C function
        ret_code = self.__lpm.lpmLoadModule(self.__module_state[0], module_index, c_cvp, c_config)

        # Check the output
        if ret_code != 0:
            raise LPMError("lpmLoadModule", ret_code)

        self.module_indices.append(module_index)

    def free_module(self, module_index):
        # Call the C function
        self.__lpm.lpmFreeModule(self.__module_state[0], module_index)

    def run_detection_module(self, module_index: int, image, bounding_box: LpmBoundingBox = None):
        # Unwrap the input parameters
        c_image = image[0]
        if bounding_box is not None:
            c_bounding_box = bounding_box.get_c(self.ffi)
        else:
            c_bounding_box = self.ffi.NULL

        # Call the C function
        c_detection_result = self.__lpm.lpmRunDet(self.__module_state[0], module_index, c_image, c_bounding_box)

        # Check the output
        if c_detection_result == self.ffi.NULL:
            raise LPMError("lpmRunDetModule", 1)

        # Wrap the result
        detection_result = LpmDetResult()
        detection_result.c_init(self.ffi, c_detection_result)

        # Free the result
        self.__lpm.lpmFreeDetResult(self.__module_state[0], c_detection_result)

        return detection_result

    def run_ocr_module(self, module_index: int, image, bounding_box: LpmBoundingBox = None,
                       detection_label: LpmDetectionLabel = LpmDetectionLabel.LPM_LABEL_DEFAULT):
        # Unwrap the input parameters
        c_image = image[0]
        if bounding_box is not None:
            c_bounding_box = bounding_box.get_c(self.ffi)
        else:
            c_bounding_box = self.ffi.NULL

        # Call the C function
        c_ocr_result = self.__lpm.lpmRunOcr(self.__module_state[0], module_index, c_image, c_bounding_box,
                                            detection_label.value)

        # Check the output
        if c_ocr_result == self.ffi.NULL:
            raise LPMError("lpmRunOcrModule", 1)

        # Wrap the result
        ocr_result = LpmOcrResult()
        ocr_result.c_init(self.ffi, c_ocr_result)

        # Free the result
        self.__lpm.lpmFreeOcrResult(self.__module_state[0], c_ocr_result)

        return ocr_result

    def get_num_modules(self):
        # Call the C function
        c_num_modules = self.__lpm.lpmGetNumAvlbModules(self.__module_state[0])

        # Check the output
        if c_num_modules == -1:
            raise LPMError("lpmGetNumAvlbModules", c_num_modules)

        return c_num_modules

    def get_module_index(self, module_id: int, version: int, subversion: int) -> int:
        # Call the C function
        c_module_index = self.__lpm.lpmGetModuleIndex(self.__module_state[0], module_id, version, subversion)

        # Check the output
        if c_module_index == -1:
            raise LPMError("lpmGetModuleIndex", c_module_index)

        return c_module_index

    def get_module_index_by_name(self, module_name: str) -> int:
        # Unwrap the input parameters
        c_module_name = self.ffi.new("const char []", module_name.encode("utf-8"))

        # Call the C function
        c_module_index = self.__lpm.lpmGetModuleIndexByName(self.__module_state[0], c_module_name)

        # Check the output
        if c_module_index == -1:
            raise LPMError("lpmGetModuleIndexByName", c_module_index)

        return c_module_index

    def get_module_info(self, module_index: int) -> LpmModuleInfo:
        # Unwrap the input parameters
        c_module_index = self.ffi.cast("int", module_index)

        # Call the C function
        c_module_info = self.__lpm.lpmGetModuleInfo(self.__module_state[0], c_module_index)

        # Wrap the result
        module_info = LpmModuleInfo()
        module_info.c_init(self.ffi, c_module_info)

        return module_info

    def get_last_error(self) -> int:
        # Call the C function
        c_error = self.__lpm.lpmGetLastError()

        return c_error

    def get_error_message(self, err_code: int) -> str:
        # Call the C function
        c_message = self.__lpm.lpmGetErrorMsg(err_code)

        # Wrap the result
        message = self.ffi.string(c_message).decode("utf-8")

        return message

    def open_error_log(self, filename: str) -> int:
        # Unwrap the input parameters
        c_filename = self.ffi.new("const char []", filename.encode("utf-8"))

        # Call the C function
        c_error = self.__lpm.lpmOpenErrorLog(c_filename)

        return c_error

    def close_error_log(self):
        # Call the C function
        self.__lpm.lpmCloseErrorLog()
