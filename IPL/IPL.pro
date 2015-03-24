##############################################################################
#  File:      IPL.pro
#  Purpose:   QMake project definition file for ImagePlay Library (IPL)
#  Author:    Kaspar Schmid
#  Date:      December 2014
#  Copyright: Kaspar Schmid, Switzerland
#             THIS SOFTWARE IS PROVIDED FOR EDUCATIONAL PURPOSE ONLY AND
#             WITHOUT ANY WARRANTIES WHETHER EXPRESSED OR IMPLIED.
##############################################################################
# QT       -= gui

CONFIG -= qt

TARGET = IPL
CONFIG(debug, debug|release): DESTDIR  = ../ImagePlay/debug
else: DESTDIR  = ../ImagePlay/release

#define platform variable for folder name
win32 {contains(QMAKE_TARGET.arch, x86_64) {PLATFORM = x64} else {PLATFORM = Win32}}
macx {PLATFORM = macx}
unix:!macx:!android {PLATFORM = linux}

#define configuration variable for folder name
CONFIG(debug, debug|release) {CONFIGURATION = Debug} else {CONFIGURATION = Release}

DESTDIR = ../_bin/$$CONFIGURATION/$$PLATFORM
OBJECTS_DIR = ../intermediate/$$TARGET/$$CONFIGURATION/$$PLATFORM
MOC_DIR = ../intermediate/$$TARGET/$$CONFIGURATION/$$PLATFORM
RCC_DIR = ../intermediate/$$TARGET/$$CONFIGURATION/$$PLATFORM
UI_DIR = ../intermediate/$$TARGET/$$CONFIGURATION/$$PLATFORM

#TEMPLATE = vclib
TEMPLATE = lib
#CONFIG += lib_bundle


DEFINES += IPL_LIBRARY

SOURCES += \
    src/IPLData.cpp \
    src/IPLPoint.cpp \
    src/IPLImagePlane.cpp \
    src/IPLImage.cpp \
    src/IPLProcess.cpp \
    src/IPLProcessProperty.cpp \
    src/IPLFileIO.cpp \
    src/IPLCameraIO.cpp \
    src/IPLComplexImage.cpp \
    src/IPLOrientedImage.cpp \
    src/IPLMatrix.cpp \
    src/processes/IPLResize.cpp \
    src/processes/IPLRotate.cpp \
    src/processes/IPLLoadImage.cpp \
    src/processes/IPLLoadImageSequence.cpp \
    src/processes/IPLArithmeticOperations.cpp \
    src/processes/IPLArithmeticOperationsConstant.cpp \
    src/processes/IPLSplitPlanes.cpp \
    src/processes/IPLBinarizeImage.cpp \
    src/processes/IPLMorphologyBinary.cpp \
    src/processes/IPLMorphologyGrayscale.cpp \
    src/processes/IPLMorphologyHitMiss.cpp \
    src/processes/IPLSaveImage.cpp \
    src/processes/IPLBlendImages.cpp \
    src/processes/IPLMergePlanes.cpp \
    src/processes/IPLSynthesize.cpp \
    src/processes/IPLFlipImage.cpp \
    src/processes/IPLRandomPoint.cpp \
    src/processes/IPLGradientOperator.cpp \
    src/processes/IPLConvolutionFilter.cpp \
    src/processes/IPLConvertToGray.cpp \
    src/processes/IPLGaussianLowPass.cpp \
    src/processes/IPLGammaCorrection.cpp \
    src/processes/IPLConvertToColor.cpp \
    src/processes/IPLCanvasSize.cpp \
    src/processes/IPLAgglomerate.cpp \
    src/processes/IPLEnhanceMode.cpp \
    src/processes/IPLFillConcavities.cpp \
    src/processes/IPLGabor.cpp \
    src/processes/IPLInverseContrastRatioMapping.cpp \
    src/processes/IPLMax.cpp \
    src/processes/IPLMaxMinMedian.cpp \
    src/processes/IPLMedian.cpp \
    src/processes/IPLCanny.cpp \
    src/processes/IPLExtractLines.cpp \
    src/processes/IPLExtrema.cpp \
    src/processes/IPLLaplaceOfGaussian.cpp \
    src/processes/IPLMin.cpp \
    src/processes/IPLMorphologicalEdge.cpp \
    src/processes/IPLNormalizeIllumination.cpp \
    src/processes/IPLBinarizeSavola.cpp \
    src/processes/IPLOnePixelEdge.cpp \
    src/processes/IPLRankTransform.cpp \
    src/processes/IPLUnsharpMasking.cpp \
    src/processes/IPLCompassMask.cpp \
    src/processes/IPLTriangleSegmentation.cpp \
    src/processes/IPLStretchContrast.cpp \
    src/processes/IPLNegate.cpp \
    src/processes/IPLMarkImage.cpp \
    src/processes/IPLLocalThreshold.cpp \
    src/processes/IPLHysteresisThreshold.cpp \
    src/processes/IPLFalseColor.cpp \
    src/processes/IPLEqualizeHistogram.cpp \
    src/processes/IPLBinarizeUnimodal.cpp \
    src/processes/IPLBinarizeOtsu.cpp \
    src/processes/IPLBinarizeKMeans.cpp \
    src/processes/IPLBinarizeEntropy.cpp \
    src/processes/IPLAddNoise.cpp \
    src/processes/IPLCamera.cpp \
    src/processes/IPLLabelBlobs.cpp \
    src/processes/IPLFFT.cpp \
    src/processes/IPLIFFT.cpp \
    src/processes/IPLFrequencyFilter.cpp \

HEADERS += \
    include/IPL_plugininterface.h \
    include/IPL_processes.h \
    include/IPL_global.h \
    include/IPLData.h \
    include/IPLProgressEventHandler.h \
    include/IPLPropertyChangedEventHandler.h \
    include/IPLErrorHandler.h \
    include/IPLProcess.h \
    include/IPLProcessProperty.h \
    include/IPLImagePlane.h \
    include/IPLHistogram.h \
    include/IPLFileIO.h \
    include/IPLCameraIO.h \
    include/IPLColor.h \
    include/IPLImage.h \
    include/IPLComplexImage.h \
    include/IPLPoint.h \
    include/IPLOrientedImage.h \
    include/IPLMatrix.h \
    include/processes/IPLResize.h \
    include/processes/IPLGaussianLowPass.h \
    include/processes/IPLGammaCorrection.h \
    include/processes/IPLConvertToGray.h \
    include/processes/IPLLoadImage.h \
    include/processes/IPLLoadImageSequence.h \
    include/processes/IPLArithmeticOperations.h \
    include/processes/IPLArithmeticOperationsConstant.h \
    include/processes/IPLSplitPlanes.h \
    include/processes/IPLBinarizeImage.h \
    include/processes/IPLConvolutionFilter.h \
    include/processes/IPLMorphologyBinary.h \
    include/processes/IPLMorphologyGrayscale.h \
    include/processes/IPLMorphologyHitMiss.h \
    include/processes/IPLSaveImage.h \
    include/processes/IPLBlendImages.h \
    include/processes/IPLMergePlanes.h \
    include/processes/IPLSynthesize.h \
    include/processes/IPLFlipImage.h \
    include/processes/IPLRandomPoint.h \
    include/processes/IPLGradientOperator.h \
    include/processes/IPLConvertToColor.h \
    include/processes/IPLCanvasSize.h \
    include/processes/IPLRotate.h \
    include/processes/IPLAgglomerate.h \
    include/processes/IPLEnhanceMode.h \
    include/processes/IPLFillConcavities.h \
    include/processes/IPLGabor.h \
    include/processes/IPLInverseContrastRatioMapping.h \
    include/processes/IPLMax.h \
    include/processes/IPLMaxMinMedian.h \
    include/processes/IPLMedian.h \
    include/processes/IPLCanny.h \
    include/processes/IPLExtractLines.h \
    include/processes/IPLExtrema.h \
    include/processes/IPLLaplaceOfGaussian.h \
    include/processes/IPLMin.h \
    include/processes/IPLMorphologicalEdge.h \
    include/processes/IPLNormalizeIllumination.h \
    include/processes/IPLBinarizeSavola.h \
    include/processes/IPLOnePixelEdge.h \
    include/processes/IPLRankTransform.h \
    include/processes/IPLUnsharpMasking.h \
    include/processes/IPLCompassMask.h \
    include/processes/IPLTriangleSegmentation.h \
    include/processes/IPLStretchContrast.h \
    include/processes/IPLNegate.h \
    include/processes/IPLMarkImage.h \
    include/processes/IPLLocalThreshold.h \
    include/processes/IPLHysteresisThreshold.h \
    include/processes/IPLFalseColor.h \
    include/processes/IPLEqualizeHistogram.h \
    include/processes/IPLBinarizeUnimodal.h \
    include/processes/IPLBinarizeOtsu.h \
    include/processes/IPLBinarizeKMeans.h \
    include/processes/IPLBinarizeEntropy.h \
    include/processes/IPLAddNoise.h \
    include/processes/IPLCamera.h \
    include/processes/IPLLabelBlobs.h \
    include/processes/IPLFFT.h \
    include/processes/IPLIFFT.h \
    include/processes/IPLFrequencyFilter.h \

#win32: LIBS += -L$$PWD/lib/FreeImage/ -lFreeImage

#INCLUDEPATH += $$PWD/lib/FreeImage/
#DEPENDPATH += $$PWD/lib/FreeImage/

#win32: PRE_TARGETDEPS += $$PWD/lib/FreeImage/FreeImage.lib

win32: {

    # dirent
    INCLUDEPATH += $$PWD/lib/
    SOURCES += include/dirent/dirent.c
    HEADERS += include/dirent/dirent.h

    # freeimage
    LIBS += -L$$PWD/../_lib/freeimage/ -lFreeImage
    INCLUDEPATH += $$PWD/../_lib/freeimage
    DEPENDPATH += $$PWD/../_lib/freeimage

    # opencv
    LIBS += -L$$PWD/../_lib/opencv/x64/vc12/lib/ -lopencv_core2410
    LIBS += -L$$PWD/../_lib/opencv/x64/vc12/lib/ -lopencv_imgproc2410
    LIBS += -L$$PWD/../_lib/opencv/x64/vc12/lib/ -lopencv_highgui2410

    # openmp for vs
    QMAKE_CXXFLAGS += -openmp
    QMAKE_LFLAGS   += -openmp

    QMAKE_CXXFLAGS_RELEASE -= -O1
    QMAKE_CXXFLAGS_RELEASE -= -O2
    QMAKE_CXXFLAGS_RELEASE *= -O3
}

macx: {
    CONFIG += c++11

    LIBS += -L$$PWD/../_lib/freeimage/ -lfreeimage-3.16.0

    INCLUDEPATH += $$PWD/../_lib/freeimage
    DEPENDPATH += $$PWD/../_lib/freeimage

    DESTDIR = ../_bin/$$CONFIGURATION/$$PLATFORM/ImagePlay.app/Contents/Frameworks/

    LIBS += -L$$PWD/../_lib/opencv/x64/clang/lib/ -lopencv_core
    LIBS += -L$$PWD/../_lib/opencv/x64/clang/lib/ -lopencv_imgproc
    LIBS += -L$$PWD/../_lib/opencv/x64/clang/lib/ -lopencv_highgui

    # openmp for clang
    QMAKE_CXXFLAGS += -openmp
    QMAKE_LFLAGS   += -openmp
}

linux: {
    CONFIG += c++11 staticlib

    LIBS += -lfreeimage
    LIBS += -lopencv_core
    LIBS += -lopencv_imgproc
    LIBS += -lopencv_highgui

    DESTDIR = ../_bin/$$CONFIGURATION/$$PLATFORM/

    # openmp for gcc
    QMAKE_CXXFLAGS += -fopenmp
    QMAKE_LFLAGS   += -fopenmp
}

# IPL
INCLUDEPATH += $$PWD/include/
INCLUDEPATH += $$PWD/include/processes/

# OpenCV
INCLUDEPATH += $$PWD/include/opencv/
