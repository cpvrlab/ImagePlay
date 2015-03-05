#include "IPLCombineImages.h"

using namespace std;

#define ChannelBlend_Normal(A,B)     ((uchar)(A))
#define ChannelBlend_Lighten(A,B)    ((uchar)((B > A) ? B:A))
#define ChannelBlend_Darken(A,B)     ((uchar)((B > A) ? A:B))
#define ChannelBlend_Multiply(A,B)   ((uchar)((A * B) / 255))
#define ChannelBlend_Average(A,B)    ((uchar)((A + B) / 2))
#define ChannelBlend_Add(A,B)        ((uchar)(min(255, (A + B))))
#define ChannelBlend_Subtract(A,B)   ((uchar)((A + B < 255) ? 0:(A + B - 255)))
#define ChannelBlend_Difference(A,B) ((uchar)(abs(A - B)))
#define ChannelBlend_Negation(A,B)   ((uchar)(255 - abs(255 - A - B)))
#define ChannelBlend_Screen(A,B)     ((uchar)(255 - (((255 - A) * (255 - B)) >> 8)))
#define ChannelBlend_Exclusion(A,B)  ((uchar)(A + B - 2 * A * B / 255))
#define ChannelBlend_Overlay(A,B)    ((uchar)((B < 128) ? (2 * A * B / 255):(255 - 2 * (255 - A) * (255 - B) / 255)))
#define ChannelBlend_SoftLight(A,B)  ((uchar)((B < 128)?(2*((A>>1)+64))*((float)B/255):(255-(2*(255-((A>>1)+64))*(float)(255-B)/255))))
#define ChannelBlend_HardLight(A,B)  (ChannelBlend_Overlay(B,A))
#define ChannelBlend_ColorDodge(A,B) ((uchar)((B == 255) ? B:min(255, ((A << 8 ) / (255 - B)))))
#define ChannelBlend_ColorBurn(A,B)  ((uchar)((B == 0) ? B:max(0, (255 - ((255 - A) << 8 ) / B))))
#define ChannelBlend_LinearDodge(A,B)(ChannelBlend_Add(A,B))
#define ChannelBlend_LinearBurn(A,B) (ChannelBlend_Subtract(A,B))
#define ChannelBlend_LinearLight(A,B)((uchar)(B < 128)?ChannelBlend_LinearBurn(A,(2 * B)):ChannelBlend_LinearDodge(A,(2 * (B - 128))))
#define ChannelBlend_VividLight(A,B) ((uchar)(B < 128)?ChannelBlend_ColorBurn(A,(2 * B)):ChannelBlend_ColorDodge(A,(2 * (B - 128))))
#define ChannelBlend_PinLight(A,B)   ((uchar)(B < 128)?ChannelBlend_Darken(A,(2 * B)):ChannelBlend_Lighten(A,(2 * (B - 128))))
#define ChannelBlend_HardMix(A,B)    ((uchar)((ChannelBlend_VividLight(A,B) < 128) ? 0:255))
#define ChannelBlend_Reflect(A,B)    ((uchar)((B == 255) ? B:min(255, (A * A / (255 - B)))))
#define ChannelBlend_Glow(A,B)       (ChannelBlend_Reflect(B,A))
#define ChannelBlend_Phoenix(A,B)    ((uchar)(min(A,B) - max(A,B) + 255))

IPLCombineImages::IPLCombineImages() : IPLProcess()
{
    _className  = "IPLCombineImages";
    _title      = "Combine Images";
    _result     = NULL;
}

IPLCombineImages::~IPLCombineImages()
{
    if( _result ) delete _result;
}

void IPLCombineImages::init()
{
    _result = NULL;
    _inputA = NULL;
    _inputB = NULL;
    _factorA = 1.0;
    _factorB = 1.0;
    _operation = 0;

    // inputs and outputs
    _inputs.push_back(IPLProcessIO(0, "Image A", IPLImage::COLOR));
    _inputs.push_back(IPLProcessIO(1, "Image B", IPLImage::COLOR));
    _outputs.push_back(IPLProcessIO(0, "Image", IPLImage::COLOR));

    // properties
    _properties["operation"] = new IPLProcessPropertyInt(this, 0, "Operation:Normal|Lighten|Darken|Multiply|Average|Add|Subtract|Difference|Negation|Screen|Exclusion|Overlay|Soft Light|Hard Light|Color Dodge|Color Burn|Linear Dodge|Linear Burn|Linear Light|Vivid light|Pin Light|Hard Mix|Reflect|Glow|Phoenix",
                                                         "", IPL_INT_COMBOBOX, _operation);
    _properties["factorA"] = new IPLProcessPropertyDouble(this, 1, "Weight A", "", IPL_DOUBLE_SLIDER, _factorA, 0.0, 1.0);
    _properties["factorB"] = new IPLProcessPropertyDouble(this, 2, "Weight B", "Adjust the weight for each input image",
                                                            IPL_DOUBLE_SLIDER, _factorB, 0.0, 1.0);
}

bool IPLCombineImages::processInputData(IPLImage* image , int imageIndex)
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
    _operation   = ((IPLProcessPropertyInt*)      _properties["operation"])->value();
    _factorA     = ((IPLProcessPropertyDouble*)   _properties["factorA"])->value();
    _factorB     = ((IPLProcessPropertyDouble*)   _properties["factorB"])->value();

    int maxNrOfPlanes = std::max( _inputA->getNumberOfPlanes(), _inputB->getNumberOfPlanes());
    int progress = 0;
    int maxProgress = maxNrOfPlanes*height;

    IPLImage::IPLImageType type = IPLImage::COLOR;
    if(maxNrOfPlanes == 1)
        type = IPLImage::GRAY;

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
                int valueA = _factorA * (double) planeA->cp(x,y);
                int valueB = _factorB * (double) planeB->cp(x,y);
                int value = 0;

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

                newplane->p(x,y) = (uchar) value;
            }
        }
    }

    _inputA = NULL;
    _inputB = NULL;

    // set result ready
    _resultReady = true;
    return true;
}

IPLImage* IPLCombineImages::getResultData( int )
{
    return _result;
}
