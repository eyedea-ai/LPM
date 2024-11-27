////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2020-2021 by Eyedea Recognition, s.r.o.                              //
//                                                                                    //
// Author: Eyedea Recognition, s.r.o.                                                 //
//                                                                                    //
// Contact:                                                                           //
//           web: http://www.eyedea.cz                                                //
//           email: info@eyedea.cz                                                    //
//                                                                                    //
// BSD License                                                                        //
// -----------------------------------------------------------------------------------//
// Copyright (c) 2020-2021, Eyedea Recognition, s.r.o.                                //
// All rights reserved.                                                               //
// Redistribution and use in source and binary forms, with or without modification,   //
// are permitted provided that the following conditions are met :                     //
//     1. Redistributions of source code must retain the above copyright notice,      //
//        this list of conditions and the following disclaimer.                       //
//     2. Redistributions in binary form must reproduce the above copyright notice,   //
//        this list of conditions and the following disclaimer in the documentation   //
//        and / or other materials provided with the distribution.                    //
//     3. Neither the name of the copyright holder nor the names of its contributors  //
//        may be used to endorse or promote products derived from this software       //
//        without specific prior written permission.                                  //
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"        //
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED  //
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. //
// IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,   //
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES(INCLUDING, BUT  //
// NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR //
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,  //
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT(INCLUDING NEGLIGENCE OR OTHERWISE)  //
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF             //
// THE POSSIBILITY OF SUCH DAMAGE.                                                    //
////////////////////////////////////////////////////////////////////////////////////////

package cz.eyedea.lpm.java.example;

import java.io.Console;

import cz.eyedea.lpm.java.LpmJavaSDK;
import cz.eyedea.lpm.java.structures.LpmBoundingBox;
import cz.eyedea.lpm.java.structures.LpmCameraViewParams;
import cz.eyedea.lpm.java.structures.LpmCameraViewParams.LpmViewType;
import cz.eyedea.lpm.java.structures.LpmDetectionLabel;
import cz.eyedea.lpm.java.structures.LpmDetResult;
import cz.eyedea.lpm.java.structures.LpmDetResult.LpmDetection;
import cz.eyedea.lpm.java.structures.LpmException;
import cz.eyedea.lpm.java.structures.LpmModuleConfig;
import cz.eyedea.lpm.java.structures.LpmModuleInfo;
import cz.eyedea.lpm.java.structures.LpmOcrResult;
import cz.eyedea.lpm.java.structures.LpmOcrResult.LpmOcrHypothesis;
import cz.eyedea.er.java.structures.ERException.NoImageException;
import cz.eyedea.er.java.structures.ERImage;

public class ExampleLPM {

    private static final String LPM_ENGINE_ROOT = "../../../LPM";
    private static final String LPM_MODULES_ROOT = "../../../modules-v7";
    private static final String PLATFORM = getPlatform();
    private static final String IMAGE_PATH = "../../../examples/example-anpr-implink/images";
    private static final String[] IMAGE_NAMES = new String[] { "img_1.jpg", "img_2.jpg", "img_3.jpg", "img_4.jpg", "img_5.jpg", "img_6.jpg", "img_7.jpg", "img_8.jpg", "img_9.jpg", "img_10.jpg" };

    private static final boolean PRINT_RESULTS = true;
    private static final boolean LOAD_VIEW_CONFIG = false;
    private static final boolean LET_USER_SELECT_MODULE = true;
    private static final boolean USE_NEW_API = true;
    private static final boolean COMPUTE_ON_GPU = false;

	// relevant only for LET_USER_SELECT_MODULE = false
    private static final int DEFAULT_MODULE_ID = 800;


    private static String getPlatform() {
        String osArch = System.getProperty("os.arch");
        String osName = System.getProperty("os.name");
        if (osName.startsWith("Windows")) {
            if (osArch.equals("amd64")) {
                return "x64";
            } else {
                return "Win32";
            }
        } else if (osName.startsWith("Linux")) {
            if (osArch.equals("amd64")) {
                return "x86_64";
            } else {
                throw new RuntimeException("System architecture " + osArch + " is not supported!");
            }
        } else {
            throw new RuntimeException("Operating system " + osName + " is not supported!");
        }
    }

    public static void main(String[] args) {
        LpmJavaSDK lpmJavaSDK = null;
        try {
            lpmJavaSDK = new LpmJavaSDK(LPM_ENGINE_ROOT, LPM_MODULES_ROOT + "/" + PLATFORM);
        } catch (Exception e) {
            String message = e.getMessage();
            System.err.println("LpmJavaSDK error: " + message);
            return;
        }

        System.out.println("LPM version: " + lpmJavaSDK.getVersion() + "." + lpmJavaSDK.getSubversion());
        System.out.println("Compiled on: " + lpmJavaSDK.getCompilationDate());

        // NOTE: Camera view params are relevant only for certain modules; modern net detectors are not using them.
        LpmCameraViewParams cameraViewParams;
        if (LOAD_VIEW_CONFIG) {
            String cameraViewParamsFilePath = LPM_MODULES_ROOT + "/config_camera_view.ini";
            cameraViewParams = lpmJavaSDK.loadViewConfig(cameraViewParamsFilePath);
            if (cameraViewParams == null) {
                System.err.println("Failed to load camera config file '" + cameraViewParamsFilePath + "'. Default values will be used instead.");
            }
        } else {
            System.out.println("Using default camera view params (if needed).");
            cameraViewParams = null;
        }

        System.out.println();
        int availableModulesCount = lpmJavaSDK.getAvailableModulesCount();
        System.out.println("Listing " + availableModulesCount + " module(s):");
        for (int i = 0; i < availableModulesCount; i++) {
            LpmModuleInfo moduleInfo = lpmJavaSDK.getModuleInfo(i);
            if (moduleInfo != null) {
                System.out.println("- Module ID: " + moduleInfo.getId());
                System.out.println("  Module name: " + moduleInfo.getName());
                System.out.println("  Module path: " + moduleInfo.getPath());
                System.out.println("  Module date: " + moduleInfo.getDate());
                System.out.println("  Module version: " + moduleInfo.getVersion() + "." + moduleInfo.getSubversion());
                System.out.println();
            }
        }
        System.out.println();

        int moduleIndex;
        int moduleId;
        if (LET_USER_SELECT_MODULE) {
            Console console = System.console();
            if (console == null) {
                System.err.println("Unable to obtain console. Please run the example again with LET_USER_SELECT_MODULE = false and DEFAULT_MODULE_ID set to the desired ID.");
                lpmJavaSDK.dispose();
                return;
            }
            System.out.print("Select LPM module ID from the list above: ");
            try {
                moduleId = Integer.parseInt(console.readLine());
            } catch (NumberFormatException e) {
                System.err.println("LPM module ID must be an integer value. " + e.getMessage());
                lpmJavaSDK.dispose();
                return;
            }
        } else {
            moduleId = DEFAULT_MODULE_ID;
        }
        try {
            moduleIndex = lpmJavaSDK.getModuleIndex(moduleId, 0, 0);
        } catch (LpmException e) {
            System.err.println("LpmJavaSDK error: " + e.getMessage());
            lpmJavaSDK.dispose();
            return;
        }

        System.out.println("Initializing module ID: " + moduleId + " (module index: " + moduleIndex + ")...");
        try {
            if (USE_NEW_API) {
                System.out.println("Loading module using the new LpmModuleConfig API...");
                lpmJavaSDK.loadModule(moduleIndex, cameraViewParams, new LpmModuleConfig("config.ini", COMPUTE_ON_GPU, 0, 1, false, "config-det.ini", COMPUTE_ON_GPU, 0, 1, false));
            } else {
                  System.out.println("Loading module using the deprecated LpmModuleConfig API...");
                  lpmJavaSDK.loadModule(moduleIndex, cameraViewParams, new LpmModuleConfig(COMPUTE_ON_GPU, 0));
            }
        } catch (LpmException e) {
            System.err.println("LpmJavaSDK error: " + e.getMessage());
            lpmJavaSDK.dispose();
            return;
        }
        System.out.println("Module initialized.");
        System.out.println();

        for (String imageName : IMAGE_NAMES) {
            String imageFilePath = IMAGE_PATH + "/" + imageName;
            if (PRINT_RESULTS) {
                System.out.println();
                System.out.println("Processing image " + imageName + "...");
             }

            ERImage image;
            try {
                image = new ERImage(imageFilePath);
            } catch (Exception e) {
                System.err.println("ERImage error: " + e.getMessage());
                continue;
            }

            // Specify an area of the input image where the lp detection will be performed
            int left = 0;
            int top = 0;
            int right = image.getWidth() - 1;
            int bottom = image.getHeight() - 1;
            LpmBoundingBox boundingBox = new LpmBoundingBox(left, top, right, bottom);

            // Run detection
            LpmDetResult detectionResult;
            try {
                detectionResult = lpmJavaSDK.runDetector(image, boundingBox);
            } catch (NoImageException | LpmException e) {
                System.err.println("LpmJavaSDK error: " + e.getMessage());
                continue;
            }
            if (detectionResult != null) {
                if (PRINT_RESULTS) {
                     System.out.println(String.format(" - found %d detections", detectionResult.getDetections().size()));
                }
            }
            else {
                if (PRINT_RESULTS) {
                    System.err.println("Invalid detection result!");
                }
                continue;
            }

            // Run OCR on each detection
            for (LpmDetection detection : detectionResult.getDetections()) {
                if (PRINT_RESULTS) {
                    if (detection == null) {
                        System.out.println(" - Detection: null");
                        continue;
                    } else if (detection.getPosition() == null) {
                        System.out.println(" - Detection position: null");
                        continue;
                    } else {
                        System.out.println(String.format(" - Detection confidence: %.2f", detection.getConfidence()));
                        System.out.println(String.format("    - detection occlusion: %.2f", detection.getOcclusion()));
                        System.out.println(String.format("    - detection truncated: %d", detection.getTruncated()));
                        System.out.println(String.format("    - detection clusterId: %d, confidence: %.2f", detection.getClusterId(), detection.getClusterConfidence()));
                        if (detection.getLabel() != null) {
                            System.out.println(String.format("    - detection label: %d", detection.getLabel().getValue()));
                        } else {
                            System.out.println("    - detection label: null");
                        }
                        if (detection.getImage() == null) {
                            System.out.println("    - detection image: null");
                        }
                    }
                }

                LpmOcrResult ocrResult;
                try {
                    ocrResult = lpmJavaSDK.runOcr(image, detection.getPosition(), detection.getLabel() != null ? detection.getLabel() : LpmDetectionLabel.LPM_LABEL_LP);
                } catch (NoImageException | LpmException e) {
                    System.err.println("LpmJavaSDK error: " + e.getMessage());
                    continue;
                }
                if (ocrResult != null && !ocrResult.getHypotheses().isEmpty()) {
                    LpmOcrHypothesis ocrHypothesis = ocrResult.getHypotheses().get(0);
                    if (PRINT_RESULTS) {
                        System.out.println(String.format("    - Ilpc: %s, confidence: %.2f", ocrHypothesis.getPlateType(), ocrHypothesis.getPlateTypeConfidence()));
                        System.out.println(String.format("    - dimensions: w * h = %d * %d [mm], confidence: %.2f", ocrHypothesis.getLpDimensions().getPhysicalWidth(), ocrHypothesis.getLpDimensions().getPhysicalheight(), ocrHypothesis.getLpDimensionsConfidence()));
                        System.out.println(String.format("    - text: %s, confidence: %.2f", ocrHypothesis.getText(), ocrHypothesis.getConfidence()));
                        System.out.println(String.format("    - unreadable: %.2f, obstructed: %.2f", ocrHypothesis.getUnreadable(), ocrHypothesis.getObstructed()));
                     }
                }
            }
        }

        System.out.println("disposing...");
        // Do not forget to call dispose() to free all underlying C/C++ structures
        lpmJavaSDK.dispose();
        System.out.println("disposed.");
    }
}
