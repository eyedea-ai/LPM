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
//         License plate reading library example         //
///////////////////////////////////////////////////////////

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <iostream>
#include <vector>
#include <algorithm>

#include <lpm.h>
#include <er_image.h>


// Path to module(s) directory
#define MODULES_BASE_DIR        "../../modules-v7/"

#ifdef _WIN32 // Windows paths

#ifdef _WIN64
#define MODULES_DIR             MODULES_BASE_DIR "x64/"
#else
#define MODULES_DIR             MODULES_BASE_DIR "Win32/"
#endif

#else // Linux paths

#ifdef __aarch64__
#define MODULES_DIR             MODULES_BASE_DIR "aarch64/"
#else
#define MODULES_DIR             MODULES_BASE_DIR "x86_64/"
#endif

#endif

#define VIEW_CONFIG_FILENAME    MODULES_BASE_DIR "config_camera_view.ini"
#define IMAGES_DIR              "images/"

#define NUM_IMG 10
const char TestImageList[NUM_IMG][LPM_MAX_PATH_LEN] = {
    IMAGES_DIR "img_1.jpg",
    IMAGES_DIR "img_2.jpg",
    IMAGES_DIR "img_3.jpg",
    IMAGES_DIR "img_4.jpg",
    IMAGES_DIR "img_5.jpg",
    IMAGES_DIR "img_6.jpg",
    IMAGES_DIR "img_7.jpg",
    IMAGES_DIR "img_8.jpg",
    IMAGES_DIR "img_9.jpg",
    IMAGES_DIR "img_10.jpg",
};


//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
// LPM module example                                                       //
//////////////////////////////////////////////////////////////////////////////
//   This example demonstrates the basic usage of the LPM:                  //
//       1) It starts with LPM library initialization,                      //
//       2) then it lists all available modules,                            //
//       3) writes an example camera config parameters,                     //
//       4) loads the example camera config parameters,                     //
//       5) sets the optional module parameters,                            //
//       6) loads the required module,                                      //
//       7) processes all input images in a loop, which consists of:        //
//          7.1) input image file loading,                                  //
//          7.2) specifying scanning area,                                  //
//          7.3) detection computation,                                     //
//          7.4) OCR computation,                                           //
//       8) and cleaning up at the end.                                     //
//////////////////////////////////////////////////////////////////////////////
int main(int argc, char *argv[])
{
    int i;
    LPMState lpm_state;                 // A void pointer to the LPM state variable
    LpmDetResult  *det_result = NULL;   // A pointer to the detection result structure
    LpmOcrResult  *ocr_result = NULL;   // A double-pointer to the OCR result structure
    LpmBoundingBox bb;                  // A variable holding the bounding box of an area of detector's interest
    LpmModuleInfo *module_info = NULL;  // A pointer to the module info structure
    int module_idx;                     // Module index (handle)
    int ret_code;


    //////////////////////////////////////////////////////////////////////////////
    //
    // Init LPM module and list the installed modules
    //

    // The LPM needs to be initialized first.
    // This routine scans the given directory for LPM modules and returns a state/handle for the LPM.
    if ((ret_code = lpmInit(MODULES_DIR, &lpm_state)) != 0)
    {
        printf("LPM could not be initialized, code %d.\n", ret_code);

        // Wait for ENTER
        std::cout << "Press [ENTER] to exit." << std::endl;
        getchar();

        return -1;
    }

    printf("LPM v%u.%u initialized\n\n", (unsigned char)(lpmVersion() >> CHAR_BIT), (unsigned char)(lpmVersion()));


    //////////////////////////////////////////////////////////////////////////////
    //
    // List the installed modules
    //

    // Vector for storing available module IDs
    std::vector<int> available_ids;

    // Let's get the number of available LPM modules
    int num_available_modules = lpmGetNumAvlbModules(lpm_state);

    // Print some basic information about them
    printf("Listing %d modules:\n", num_available_modules);
    for (i = 0; i < num_available_modules; i++)
    {
        // Note that the index of a module is determined by the order, in which the modules
        // were found within the main modules directory.
        module_info = lpmGetModuleInfo(lpm_state, i);

        // Each module has its own ID
        printf("  Module ID     : %d\n", module_info->id);
        available_ids.push_back(module_info->id);
        // Module name, version, date and others are available as defined in LpmModuleInfo structure
        printf("  Module name   : %s\n", module_info->name);
        printf("  Module path   : %s\n", module_info->path);
        printf("  Module date   : %s\n", module_info->date);
        printf("  Module version: %d.%d\n\n", module_info->version, module_info->subversion);

    }

    // Select model ID
    int module_id = 0;
    if (num_available_modules > 1)
    {
        printf("Select LPM module ID from the list above:");
        std::cin >> module_id;
    }
    else if (num_available_modules == 1) {
        module_id = available_ids.at(0);
        printf("Selected the only available LPM module ID: %d\n", module_id);
    }
    // Find out if the selected module ID is available
    bool found = (std::find(available_ids.begin(), available_ids.end(), module_id) != available_ids.end());
    if (!found)
    {
        printf("LPM module with ID %d is not available.\n", module_id);

        // Free the LPM state
        lpmFree(&lpm_state);

        // Wait for ENTER
        std::cout << "Press [ENTER] to exit." << std::endl;
        getchar();

        return 0;
    }


    //////////////////////////////////////////////////////////////////////////////
    //
    // Write the camera view parameters (image resolution, aspect etc) to a config file.
    //

    // The parameters are stored in LpmCameraViewParams structure
    LpmCameraViewParams camera_view_params;
    camera_view_params.view_type = LPM_VIEW_FRONTAL;
    // Example images are 1:1
    camera_view_params.density_ratio = 1.f;
    // E.g.: the Swiss licence plate on the last image is around 60px -> 60/0.52 ~ 115 px/m
    camera_view_params.min_horizontal_resolution = 115;
    // The maximum LP width should be around 135px -> 135/0.52 ~ 260 px/m
    camera_view_params.max_horizontal_resolution = 260;

    // Write them to a file
    lpmWriteViewConfig(VIEW_CONFIG_FILENAME, camera_view_params);


    //////////////////////////////////////////////////////////////////////////////
    //
    // Load the camera view parameters (image resolution, aspect etc) from a config file.
    // The parameters are stored in LpmCameraViewParams structure.
    //

    // Load the camera view parameters from config file
    if (lpmLoadViewConfig(VIEW_CONFIG_FILENAME, &camera_view_params) != 0)
    {
        // Error, something went wrong
        // Wait for ENTER
        std::cout << "Press [ENTER] to exit." << std::endl;
        getchar();

        return -1;
    }


    //////////////////////////////////////////////////////////////////////////////
    //
    // Load/initialize module MODULE_ID
    //

    // Check if a module with a given MODULE_ID is available and if so return its index (handle). 
    // NOTE: If there are more modules with the same ID then take the latest one, i.e. the latest version.
    if ((module_idx = lpmGetModuleIndex(lpm_state, module_id, 0, 0)) == -1)
    {
        // Error, something went wrong
        // Wait for ENTER
        std::cout << "Press [ENTER] to exit." << std::endl;
        getchar();

        return -1;
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Set module configuration parameters
    //
    LpmModuleConfig lpm_module_config;
    // Unused values must be zero-initialized
    memset(&lpm_module_config, 0, sizeof(lpm_module_config));
	// Set to 0 for CPU or 1 for GPU processing mode
    lpm_module_config.compute_on_gpu = 0;
    // Selects the one desired from all of the available GPU devices
    lpm_module_config.gpu_device_id = 0;

#ifdef LPM_EXTENSIONS_v7_3
    // Optional extension parameters from LPMv7.3
    LpmModuleConfig_extension1 lpm_module_config_extension1;
    // Unused values must be zero-initialized
    memset(&lpm_module_config_extension1, 0, sizeof(lpm_module_config_extension1));
    lpm_module_config_extension1.lpm_config_filename = "config.ini";
    lpm_module_config_extension1.ocr_compute_on_gpu = 1;
    lpm_module_config_extension1.det_config_filename = "config-det.ini";
    lpm_module_config_extension1.det_compute_on_gpu = 1;
    lpm_module_config.extras = &lpm_module_config_extension1;
#endif

    // Now load the module. The third parameter camera_view_params is optional. 
    // If the third parameter is NULL then the default values are used.
    if (lpmLoadModule(lpm_state, module_idx, &camera_view_params, &lpm_module_config) != 0)
    {
        // Error, something went wrong
        // Wait for ENTER
        std::cout << "Press [ENTER] to exit." << std::endl;
        getchar();

        return -1;
    }


    //////////////////////////////////////////////////////////////////////////////
    //
    // Process images
    //
    for (i = 0; i < NUM_IMG; i++)
    {
        // Read the input image from a file
        ERImage er_image;

        std::cout << "Processing image " << TestImageList[i] << "..." << std::endl;
        if (erImageRead(&er_image, TestImageList[i]) != 0)
        {
            std::cerr << "Can't load the file: " << TestImageList[i] << std::endl;
            getchar();

            return 1;
        }

        // Specify an area of the input image where the lp detection will be performed
        bb.top_left_col = 0;
        bb.top_left_row = 0;
        bb.bot_right_col = er_image.width - 1;
        bb.bot_right_row = er_image.height - 1;

        // Run LP detection
        if ((det_result = lpmRunDet(lpm_state, module_idx, er_image, &bb)) == NULL)
        {
            // Free the image data structure
            erImageFree(&er_image);

            // Wait for ENTER
            std::cout << "Press [ENTER] to exit." << std::endl;
            getchar();

            continue;
        }

        // Run OCR on each LP detection
        printf(" - found %d detections\n", det_result->num_detections);
        for (int j = 0; j < det_result->num_detections; j++)
        {
            // The detection crop can be saved to a file using an ER function
            // erImageWrite does not write anything for ERImages with no data,
            // so if lp_crop_enabled is set to False in the config, detection.image has no data and erImageWrite will not write anything
            LpmDetection &detection = det_result->detections[j];
            if (detection.image.data != NULL)
            {
                erImageWrite(&detection.image, "crop.jpg");
            }

            // Print information about the detection
#ifdef LPM_EXTENSIONS_v7_3
            if (det_result->extras != NULL)
            {
                LpmDetection_extension1 &detection_extension1 = det_result->extras->detections[j];
                printf(" - Detection %d, label %d, confidence %.2f, truncated %d, occlusion %.2f, cluster_id %d:\n", j + 1,
                    detection.label,
                    detection.confidence,
                    detection_extension1.truncated,
                    detection_extension1.occlusion,
                    detection_extension1.cluster_id);
                printf(" - detection [x;y]: TL[%.1f;%.1f] TR[%.1f;%.1f] BR[%f;%f] BL[%f;%f]\n",
                    detection.position.top_left_col, detection.position.top_left_row,
                    detection.position.top_right_col, detection.position.top_right_row,
                    detection.position.bot_right_col, detection.position.bot_right_row,
                    detection.position.bot_left_col, detection.position.bot_left_row);
            }
            else
            {
#endif
                printf(" - Detection %d, label %d, confidence %.2f:\n", j + 1, detection.label, detection.confidence);
#ifdef LPM_EXTENSIONS_v7_3
            }
#endif

            // Skip detections based on their labels
            if (detection.label == LPM_LABEL_VEHICLE || detection.label == LPM_LABEL_VEHICLE_WINDSHIELD
                || detection.label == LPM_LABEL_VEHICLE_FRONT || detection.label == LPM_LABEL_VEHICLE_REAR
                || detection.label == LPM_LABEL_VEHICLE_WHEEL)
            {
                continue;
            }

            // Run OCR on the detection
            if ((ocr_result = lpmRunOcr(lpm_state, module_idx, er_image, &(detection.position), detection.label)) != NULL)
            {
                // We take the first OCR hypothesis
                LpmOcrHypothesis &hypothesis = ocr_result->hypotheses[0];

                // Print the ILPC/LP type classification result
                printf("    - Ilpc: %s, confidence: %.2f\n", hypothesis.plate_type, hypothesis.confidence);

                // Print the physical dimensions classification result
                printf("    - dimensions: w*h=%d*%d[mm], confidence: %.2f\n", hypothesis.lp_dimensions.physical_width, hypothesis.lp_dimensions.physical_height, hypothesis.lp_dimensions_confidence);

#ifdef LPM_EXTENSIONS_v7_6
                if (hypothesis.extras != NULL)
                {
                    // Print the unreadable & obstructed predictions
                    printf("    - unreadable: %.2f, obstructed: %.2f\n", hypothesis.extras->unreadable, hypothesis.extras->obstructed);
                }
#endif

                // Print all the lines contained in the hypothesis
                for (unsigned int k = 0; k < hypothesis.num_lines; k++)
                {
                    // Note that the prediction can contain non-ASCII characters
                    printf("    - line %d, ASCII: '", k + 1);
                    for (unsigned int l = 0; l < hypothesis.text_lines[k].length; l++)
                    {
                        printf("%c", hypothesis.text_lines[k].characters[l]);
                    }
                    printf("', Unicode: ");
                    for (unsigned int l = 0; l < hypothesis.text_lines[k].length; l++)
                    {
                        printf("0x%X ", hypothesis.text_lines[k].characters[l]);
                    }
                    printf(", length %d, confidence %.2f\n", hypothesis.text_lines[k].length, hypothesis.text_lines[k].line_confidence);
                }
                // Empty LP/ADR table can be recognized using predicted number of lines
                if (hypothesis.num_lines == 0)
                {
                    printf("       - empty\n");
                }
                printf("\n");
            }

            // Free the OCR result structure
            lpmFreeOcrResult(lpm_state, ocr_result);
        }

        // Free the detection results
        lpmFreeDetResult(lpm_state, det_result);

        // Free the image data structure
        erImageFree(&er_image);
    }


    //////////////////////////////////////////////////////////////////////////////
    //
    // Cleaning up
    //

    // Finish work with the current module
    lpmFreeModule(lpm_state, module_idx);

    // Free the LPM state
    lpmFree(&lpm_state);

    // Wait for ENTER
    std::cout << "Press [ENTER] to exit." << std::endl;
    getchar();
}
