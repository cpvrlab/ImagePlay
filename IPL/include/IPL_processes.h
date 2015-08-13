//#############################################################################
//
//  This file is part of ImagePlay.
//
//  ImagePlay is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  ImagePlay is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with ImagePlay.  If not, see <http://www.gnu.org/licenses/>.
//
//#############################################################################

#ifndef IPL_H
#define IPL_H

#include "IPL_global.h"

// include all processes for easier library usage
#include "IPLLoadImage.h"
#include "IPLCamera.h"
#include "IPLLoadImageSequence.h"
#include "IPLSynthesize.h"
#include "IPLSaveImage.h"
#include "IPLBinarize.h"
#include "IPLGaussianLowPass.h"
#include "IPLGammaCorrection.h"
#include "IPLSplitPlanes.h"
#include "IPLMergePlanes.h"
#include "IPLConvolutionFilter.h"
#include "IPLMorphologyBinary.h"
#include "IPLMorphologyGrayscale.h"
#include "IPLMorphologyHitMiss.h"
#include "IPLArithmeticOperations.h"
#include "IPLArithmeticOperationsConstant.h"
#include "IPLConvertToGray.h"
#include "IPLBlendImages.h"
#include "IPLFlipImage.h"
#include "IPLGradientOperator.h"
#include "IPLRandomPoint.h"
#include "IPLCanvasSize.h"
#include "IPLResize.h"
#include "IPLRotate.h"
#include "IPLAgglomerate.h"
#include "IPLEnhanceMode.h"
#include "IPLFillConcavities.h"
#include "IPLGabor.h"
#include "IPLInverseContrastRatioMapping.h"
#include "IPLMax.h"
#include "IPLMaxMinMedian.h"
#include "IPLMedian.h"
#include "IPLCanny.h"
#include "IPLHoughCircles.h"
#include "IPLHarrisCorner.h"
#include "IPLExtractLines.h"
#include "IPLExtrema.h"
#include "IPLLaplaceOfGaussian.h"
#include "IPLMin.h"
#include "IPLMorphologicalEdge.h"
#include "IPLNormalizeIllumination.h"
#include "IPLBinarizeSavola.h"
#include "IPLOnePixelEdge.h"
#include "IPLRankTransform.h"
#include "IPLUnsharpMasking.h"
#include "IPLCompassMask.h"

#include "IPLTriangleSegmentation.h"
#include "IPLStretchContrast.h"
#include "IPLNegate.h"
#include "IPLMarkImage.h"
#include "IPLLocalThreshold.h"
#include "IPLHysteresisThreshold.h"
#include "IPLFalseColor.h"
#include "IPLEqualizeHistogram.h"
#include "IPLBinarizeUnimodal.h"
#include "IPLBinarizeOtsu.h"
#include "IPLBinarizeKMeans.h"
#include "IPLBinarizeEntropy.h"
#include "IPLAddNoise.h"

#include "IPLFFT.h"
#include "IPLIFFT.h"
#include "IPLFrequencyFilter.h"

#include "IPLLabelBlobs.h"


#include "IPLFloodFill.h"
#include "IPLAccumulate.h"
#include "IPLHoughLines.h"
#include "IPLMatchTemplate.h"
#include "IPLGoodFeaturesToTrack.h"

#endif // IPL_H
