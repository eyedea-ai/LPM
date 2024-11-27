import os
import platform

from er import ER
from cffi import FFI
from PIL import Image
from lpm import LPM, LpmBoundingBox, LpmModuleConfig, LpmDetection, LpmOcrHypothesis, LpmTextLine, LpmCameraViewParams


# Setup main variables
LIB_PREFIX = 'lib' if os.name == 'posix' else ''
LIB_SUFFIX = 'dll' if os.name == 'nt' else 'so'
if '32bit' in platform.architecture()[0]:
    ARCH_STRING = 'win32'
else:
    if os.name == 'nt':
        ARCH_STRING = 'x64'
    else:
        ARCH_STRING = 'x86_64'

# Set main LPM version
LPM_VERSION = 7

# Set the main paths
# Root directory of the SDK (contains README.txt, LICENSE.txt, LPM directory, etc.)
ROOT_DIR = os.path.join('..', '..')
SDK_DIR = os.path.join(ROOT_DIR, 'LPM')
LPM_LIB = os.path.join(SDK_DIR, 'lib', ARCH_STRING, '{}lpm-v{}.{}'.format(LIB_PREFIX, LPM_VERSION, LIB_SUFFIX))
libs_dir = os.path.dirname(LPM_LIB)
SUPPORT_LIBS = [os.path.join(libs_dir, f) for f in os.listdir(libs_dir)
                if (f.endswith('.dll') or f.endswith('.so')) and 'lpm' not in f]

MODULES_DIR = os.path.join(ROOT_DIR, 'modules-v{}'.format(LPM_VERSION), ARCH_STRING)
VIEW_CONFIG = os.path.join(MODULES_DIR, '..', 'config_camera_view_generic.ini')

TEST_IMAGES_BASE = os.path.join(ROOT_DIR, 'examples', 'example-anpr-implink', 'images')
TEST_IMAGES = [os.path.join(TEST_IMAGES_BASE, img) for img in os.listdir(TEST_IMAGES_BASE) if img.endswith('.jpg')]

# Set LPM module ID to load
MODULE_ID = 800


# Initialize the LPM
ffi = FFI()

# NOTE: Main LPM library contains the ERImage
eyedea_er = ER(ffi, LPM_LIB, SUPPORT_LIBS)
lpm = LPM(ffi, LPM_LIB, SUPPORT_LIBS)

lpm.init_lpm(MODULES_DIR)

print("\nLPM initialized")
print("===============")
print(" - version {}\n - compilation date {}".format(lpm.get_version(), lpm.get_compilation_date()))

# Print information about available modules
num_modules = lpm.get_num_modules()
print("\nListing {} available modules:".format(num_modules))
print("------------------------------")
for i in range(num_modules):
    module_info = lpm.get_module_info(i)
    print(" - module: ID {}, name {}, version {}.{}, created {}"
          .format(module_info.id, module_info.name, module_info.version, module_info.subversion, module_info.date))

# Save camera view configuration
view_config = LpmCameraViewParams()
view_config.density_ratio = 1.0
lpm.write_view_config(VIEW_CONFIG, view_config)

# Load camera view configuration
view_config = lpm.load_view_config(VIEW_CONFIG)
print("\nCamera view configuration:")
print("------------------------------")
print(" - min horizontal: {}, max horizontal: {}, camera aspect: {}"
      .format(view_config.min_horizontal_resolution, view_config.max_horizontal_resolution,
              view_config.density_ratio))

module_config = LpmModuleConfig()
module_config.ocr_compute_on_gpu = False
module_config.ocr_gpu_device_id = 0
module_config.ocr_num_threads = 1
module_config.det_compute_on_gpu = False
module_config.det_gpu_device_id = 0
module_config.det_num_threads = 1
print("\nModule configuration:")
print("------------------------------")
print(" - Compute on GPU: {}, GPU device ID: {}".format(module_config.compute_on_gpu, module_config.gpu_device_id))

# Load the desired module
module_index = lpm.get_module_index(MODULE_ID, 0, 0)
# view_config = None
lpm.load_module(module_index, view_config, module_config)

num_detections = 0
for image in TEST_IMAGES:
    # Read the image
    img = Image.open(image).convert("RGB")
    er_image = eyedea_er.convert_pil_image_to_erimage(img)

    # Run the detection module
    bounding_box = LpmBoundingBox()
    bounding_box.top_left_col = 0
    bounding_box.top_left_row = 0
    bounding_box.bot_right_col = er_image.width - 1
    bounding_box.bot_right_row = er_image.height - 1
    detection_results = lpm.run_detection_module(module_index, er_image, bounding_box)

    # Run the OCR
    print("\nRunning OCR on {} detections:".format(detection_results.num_detections))
    print(" - image: {}".format(image))
    print("------------------------------")
    detection_index = 1
    for detection_result in detection_results.detections:  # type: LpmDetection
        ocr_result = lpm.run_ocr_module(module_index, er_image, detection_result.position)
        hypothesis = ocr_result.hypotheses[0]  # type: LpmOcrHypothesis

        print("  {}. detection, confidence {:.2f}%:".format(detection_index, detection_result.confidence))
        print("  ----------------")
        print("    - type: '{}', confidence {:.2f}%"
              .format(hypothesis.plate_type, hypothesis.plate_type_confidence * 100))
        print("    - dimensions: W*H = {}*{}[mm], confidence {:.2f}%"
              .format(hypothesis.lp_dimensions.physical_width, hypothesis.lp_dimensions.physical_height,
                      hypothesis.lp_dimensions_confidence * 100))
        if hypothesis.num_lines == 0:
            print("    - empty")
        for line_index in range(hypothesis.num_lines):
            text_line = hypothesis.text_lines[line_index]  # type: LpmTextLine
            print("    - {}. line: '{}', length: {}, confidence {:.2f}%"
                  .format(line_index + 1, text_line.characters, text_line.length, text_line.line_confidence * 100))

        detection_index += 1
        num_detections += 1

print("{} detections in total".format(num_detections))
