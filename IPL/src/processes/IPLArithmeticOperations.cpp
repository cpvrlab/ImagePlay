#include "IPLArithmeticOperations.h"

using namespace std;

#define Arithmetic_Add(A,B)          min(1.0f, (A + B))
#define Arithmetic_Sub(A,B)          max(0.0f, (A - B))
#define Arithmetic_Mul(A,B)          (A * B) / 1.0f
#define Arithmetic_Div(A,B)          ((B > 0.0f ? ((A / B) / 1.0f) : 0.0f))
#define Arithmetic_Max(A,B)          max(A, B)
#define Arithmetic_Min(A,B)          min(A, B)
#define Arithmetic_And(A,B)          (A == 1.0f && B == 1.0f ? 1.0f : 0.0f)
#define Arithmetic_Or(A,B)           (A == 1.0f || B == 1.0f ? 1.0f : 0.0f)
#define Arithmetic_Xor(A,B)          ((A == 1.0f) != (B == 1.0f) ? 1.0f : 0.0f)
#define Arithmetic_Not(A)            (A == 1.0f ? 0.0f : 1.0f)
#define Arithmetic_Atan2(A,B)        atan2(A,B);

void IPLArithmeticOperations::init()
{
    // init
    _result     = NULL;
    _inputA     = NULL;
    _inputB     = NULL;
    _operation  = 0;

    // basic settings
    setClassName("IPLArithmeticOperations");
    setTitle("Arithmetic Operations");
    setKeywords("add, substract, multiply, divide, minimum, maximum, and, or, xor, exclusive or, not, atan, arcus tangens");
    setCategory(IPLProcess::CATEGORY_ARITHMETIC);
    setDescription("Performs arithmetic operations based on two input images.");

    // inputs and outputs
    addInput("Image A", IPLData::IMAGE_COLOR);
    addInput("Image B", IPLData::IMAGE_COLOR);
    addOutput("Image", IPLData::IMAGE_COLOR);

    // properties
    addProcessPropertyInt("operation", "Operation:ADD|SUB|MUL|DIV|MIN|MAX|AND|OR|XOR|NOT|ATAN2",
                          "Boolean operations need binary images as input",
                          IPL_INT_RADIOBUTTONS, 0);
}

void IPLArithmeticOperations::destroy()
{
    delete _result;
}

bool IPLArithmeticOperations::processInputData(IPLImage* image, int imageIndex, bool useOpenCV)
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
                float valueA = planeA->cp(x,y);
                float valueB = planeB->cp(x,y);
                float value = 0;

                //ADD|SUB|MUL|DIV|MIN|MAX|AND|OR|XOR|NOT|ADD Constant|SUB Constant|MUL Constant|DIV Constant
                switch (_operation) {
                case 0:
                    value = Arithmetic_Add(valueA, valueB);
                    break;
                case 1:
                    value = Arithmetic_Sub(valueA, valueB);
                    break;
                case 2:
                    value = Arithmetic_Mul(valueA, valueB);
                    break;
                case 3:
                    value = Arithmetic_Div(valueA, valueB);
                    break;
                case 4:
                    value = Arithmetic_Min(valueA, valueB);
                    break;
                case 5:
                    value = Arithmetic_Max(valueA, valueB);
                    break;
                case 6:
                    value = Arithmetic_And(valueA, valueB);
                    break;
                case 7:
                    value = Arithmetic_Or(valueA, valueB);
                    break;
                case 8:
                    value = Arithmetic_Xor(valueA, valueB);
                    break;
                case 9:
                    value = Arithmetic_Not(valueA);
                    break;
                case 10:
                    value = Arithmetic_Atan2(valueA, valueB);
                    break;
                }

                value = value > 1.0 ? 1.0 : value;
                newplane->p(x,y) = value;
            }
        }
    }

    //_inputA = NULL;
    //_inputB = NULL;

    return true;
}

IPLImage* IPLArithmeticOperations::getResultData( int )
{
    return _result;
}
