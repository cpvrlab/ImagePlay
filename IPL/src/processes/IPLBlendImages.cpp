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

#include "IPLBlendImages.h"

using namespace std;

#define ChannelBlend_Normal(B,L)     (B)
#define ChannelBlend_Lighten(B,L)    ((L > B) ? L:B)
#define ChannelBlend_Darken(B,L)     ((L > B) ? B:L)
#define ChannelBlend_Multiply(B,L)   ((B * L) / 1.0)
#define ChannelBlend_Average(B,L)    ((B + L) / 2)
#define ChannelBlend_Add(B,L)        (B + L)
#define ChannelBlend_Subtract(B,L)   (B + L - 1.0)
#define ChannelBlend_Difference(B,L) (abs(B - L))
#define ChannelBlend_Negation(B,L)   (1.0 - abs(1.0 - B - L))
#define ChannelBlend_Screen(B,L)     (1.0 - ((1.0 - B) * (1.0 - L)))
#define ChannelBlend_Exclusion(B,L)  (B + L - 2 * B * L / 1.0)
#define ChannelBlend_Overlay(B,L)    ((L < 0.5) ? (2 * B * L):(1.0 - 2 * (B - 0.5) * (1.0 - L)))
//#define ChannelBlend_SoftLight(B,L)  ((L < 0.5)?(2*((B>>1)+64))*((float)L/1.0):(1.0-(2*(1.0-((B>>1)+64))*(float)(1.0-L)/1.0)))
#define ChannelBlend_SoftLight(B,L)  (((1.0-L)*B*L)+ (L*ChannelBlend_Screen(B,L)))
#define ChannelBlend_HardLight(B,L)  (ChannelBlend_Overlay(L,B))
#define ChannelBlend_ColorDodge(B,L) (B/(1.0-L))
#define ChannelBlend_ColorBurn(B,L)  (1-(1.0-B)/L)
#define ChannelBlend_LinearDodge(B,L)(ChannelBlend_Add(B,L))
#define ChannelBlend_LinearBurn(B,L) (ChannelBlend_Subtract(B,L))
#define ChannelBlend_LinearLight(B,L)((L < 0.5)?ChannelBlend_LinearBurn(B,(2 * L)):ChannelBlend_LinearDodge(B,(2 * (L - 0.5))))
#define ChannelBlend_VividLight(B,L) ((L < 0.5)?ChannelBlend_ColorBurn(B,(2 * L)):ChannelBlend_ColorDodge(B,(2 * (L - 0.5))))
#define ChannelBlend_PinLight(B,L)   ((L < 0.5)?ChannelBlend_Darken(B,(2 * L)):ChannelBlend_Lighten(B,(2 * (L - 0.5))))
#define ChannelBlend_HardMix(B,L)    ((ChannelBlend_VividLight(B,L) < 0.5) ? 0:1.0)
#define ChannelBlend_Reflect(B,L)    ((L == 1.0) ? L:min(1.0, (B * B / (1.0 - L))))
#define ChannelBlend_Glow(B,L)       (ChannelBlend_Reflect(L,B))
#define ChannelBlend_Phoenix(B,L)    (min(B,L) - max(B,L) + 1.0)
#define ChannelBlend_Alpha(B,L,O)    (B + (1.0 - O) * L)
#define ChannelBlend_AlphaF(B,L,F,O) (ChannelBlend_Alpha(F(B,L),L,O))

void IPLBlendImages::init()
{
    // init
    _factorA = 1.0;
    _factorB = 1.0;
    _operation = 0;
    _result     = NULL;
    _inputA     = NULL;
    _inputB     = NULL;

    // basic settings
    setClassName("IPLBlendImages");
    setTitle("Blend Images");
    setCategory(IPLProcess::CATEGORY_POINTOPERATIONS);
    setDescription("Blend two input images using various blend modes known from popular image editing software.");

    // inputs and outputs
    addInput("Image A", IPLData::IMAGE_COLOR);
    addInput("Image B", IPLData::IMAGE_COLOR);
    addOutput("Image", IPLData::IMAGE_COLOR);

    // properties
    addProcessPropertyDouble("factorA", "Weight A", "", _factorA, IPL_WIDGET_SLIDER, 0.0, 1.0);
    addProcessPropertyDouble("factorB", "Weight B", "Adjust the weight for each input image", _factorB, IPL_WIDGET_SLIDER, 0.0, 1.0);
    addProcessPropertyInt("operation", "Operation:Normal|Lighten|Darken|Multiply|Average|Add|Subtract|Difference|Negation|Screen|Exclusion|Overlay|Soft Light|Hard Light|Color Dodge|Color Burn|Linear Dodge|Linear Burn|Linear Light|Vivid light|Pin Light|Hard Mix|Reflect|Glow|Phoenix",
                             "", _operation, IPL_WIDGET_COMBOBOX);
}

void IPLBlendImages::destroy()
{
    delete _result;
    delete _inputA;
    delete _inputB;
}

bool IPLBlendImages::processInputData(IPLImage* image , int imageIndex, bool)
{
    // save the first image
    if(imageIndex == 0)
    {
        delete _inputA;
        _inputA = new IPLImage(*image);
    }

    // save the second image
    if(imageIndex == 1)
    {
        delete _inputB;
        _inputB = new IPLImage(*image);
    }

    // only continue if we have 2 valid inputs
    if(!(_inputA && _inputB))
    {
        return false;
    }

    // delete previous result
    delete _result;
    _result = NULL;

    // the result will be the max size of both inputs
    int width  = std::max(_inputA->width(), _inputB->width());
    int height = std::max(_inputA->height(), _inputB->height());

    // copy constructor doesnt work:
    // _result = new IPLImage(*image);

    // get properties
    _operation   = getProcessPropertyInt("operation");
    _factorA     = getProcessPropertyDouble("factorA");
    _factorB     = getProcessPropertyDouble("factorB");

    int maxNrOfPlanes = std::max( _inputA->getNumberOfPlanes(), _inputB->getNumberOfPlanes());
    int progress = 0;
    int maxProgress = maxNrOfPlanes*height;

    IPLData::IPLDataType type = IPLData::IMAGE_COLOR;
    if(maxNrOfPlanes == 1)
        type = IPLData::IMAGE_GRAYSCALE;

    // create result
    _result = new IPLImage(type, width, height);

    #pragma omp parallel for
    for( int planeNr=0; planeNr < maxNrOfPlanes; planeNr++ )
    {
        // prevent reading unavailable planes
        IPLImagePlane* planeA   = _inputA->plane(std::min(planeNr, _inputA->getNumberOfPlanes()-1));
        IPLImagePlane* planeB   = _inputB->plane(std::min(planeNr, _inputB->getNumberOfPlanes()-1));
        IPLImagePlane* newplane = _result->plane(planeNr);

        for(int y=0; y<height; y++)
        {
            // progress
            notifyProgressEventHandler(100*progress++/maxProgress);

            for(int x=0; x<width; x++)
            {
                float valueA = _factorA * (float) planeA->cp(x,y);
                float valueB = _factorB * (float) planeB->cp(x,y);
                float value = 0.0f;

                switch (_operation) {
                case 1:
                    value = ChannelBlend_Lighten(valueA, valueB);
                    break;
                case 2:
                    value = ChannelBlend_Darken(valueA, valueB);
                    break;
                case 3:
                    value = ChannelBlend_Multiply(valueA, valueB);
                    break;
                case 4:
                    value = ChannelBlend_Average(valueA, valueB);
                    break;
                case 5:
                    value = ChannelBlend_Add(valueA, valueB);
                    break;
                case 6:
                    value = ChannelBlend_Subtract(valueA, valueB);
                    break;
                case 7:
                    value = ChannelBlend_Difference(valueA, valueB);
                    break;
                case 8:
                    value = ChannelBlend_Negation(valueA, valueB);
                    break;
                case 9:
                    value = ChannelBlend_Screen(valueA, valueB);
                    break;
                case 10:
                    value = ChannelBlend_Exclusion(valueA, valueB);
                    break;
                case 11:
                    value = ChannelBlend_Overlay(valueA, valueB);
                    break;
                case 12:
                    value = ChannelBlend_SoftLight(valueA, valueB);
                    break;
                case 13:
                    value = ChannelBlend_HardLight(valueA, valueB);
                    break;
                case 14:
                    value = ChannelBlend_ColorDodge(valueA, valueB);
                    break;
                case 15:
                    value = ChannelBlend_ColorBurn(valueA, valueB);
                    break;
                case 16:
                    value = ChannelBlend_LinearDodge(valueA, valueB);
                    break;
                case 17:
                    value = ChannelBlend_LinearBurn(valueA, valueB);
                    break;
                case 18:
                    value = ChannelBlend_LinearLight(valueA, valueB);
                    break;
                case 19:
                    value = ChannelBlend_VividLight(valueA, valueB);
                    break;
                case 20:
                    value = ChannelBlend_PinLight(valueA, valueB);
                    break;
                case 21:
                    value = ChannelBlend_HardMix(valueA, valueB);
                    break;
                case 22:
                    value = ChannelBlend_Reflect(valueA, valueB);
                    break;
                case 23:
                    value = ChannelBlend_Glow(valueA, valueB);
                    break;
                case 24:
                    value = ChannelBlend_Phoenix(valueA, valueB);
                    break;
                default:
                    value = ChannelBlend_Normal(valueA, valueB);
                    break;
                }

                // clamp to 0.0-1.0
                value = min(1.0f, max(0.0f, value));
                newplane->p(x,y) = value;
            }
        }
    }

    //_inputA = NULL;
    //_inputB = NULL;

    return true;
}

IPLImage* IPLBlendImages::getResultData( int )
{
    return _result;
}
