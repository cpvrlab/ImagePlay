#include "IPLArithmeticOperationsConstant.h"

using namespace std;

#define Arithmetic_Add(A,B)          min(1.0f, (A + B))
#define Arithmetic_Sub(A,B)          max(0.0f, (A - B))
#define Arithmetic_Mul(A,B)          min(1.0f, (A * B))
#define Arithmetic_Div(A,B)          min(1.0f, ((B > 0.0f ? (A / B) : 1.0f)))
#define Arithmetic_Max(A,B)          max(A, B)

void IPLArithmeticOperationsConstant::init()
{
    // init
    _result     = NULL;
    _operation  = 0;
    _constant   = 0;

    // basic settings
    setClassName("IPLArithmeticOperationsConstant");
    setTitle("Arithmetic Operations Constant");
    setCategory(IPLProcess::CATEGORY_ARITHMETIC);
    setDescription("Performs arithmetic operations based an input image and a constant.");

    // inputs and outputs
    addInput("Image", IPLData::IMAGE_COLOR);
    addOutput("Image", IPLData::IMAGE_COLOR);

    // properties
    addProcessPropertyInt("operation", "Operation:ADD Constant|SUB Constant|MUL Constant|DIV Constant",
                          "", IPL_INT_RADIOBUTTONS, 0);
    addProcessPropertyDouble("constant", "Constant", "", IPL_DOUBLE_SLIDER, 1.0, 0.0, 16.0);
}

void IPLArithmeticOperationsConstant::destroy()
{
    delete _result;
}

bool IPLArithmeticOperationsConstant::processInputData(IPLImage* image , int, bool useOpenCV)
{
    // delete previous result
    delete _result;
    _result = NULL;

    int width = image->width();
    int height = image->height();
    _result = new IPLImage( image->type(), width, height );

    // get properties
    _operation   = getProcessPropertyInt("operation");
    _constant    = getProcessPropertyDouble("constant");

    int nrOfPlanes = image->getNumberOfPlanes();
    int progress = 0;
    int maxProgress = nrOfPlanes*height;

    #pragma omp parallel for
    for( int planeNr=0; planeNr < nrOfPlanes; planeNr++ )
    {
        // prevent reading unavailable planes
        IPLImagePlane* inputPlane = image->plane(planeNr);
        IPLImagePlane* newplane   = _result->plane(planeNr);

        for(int y=0; y<height; y++)
        {
            // progress
            notifyProgressEventHandler(100*progress++/maxProgress);

            for(int x=0; x<width; x++)
            {
                float valueA = inputPlane->cp(x,y);
                float value = 0.0f;

                // ADD Constant|SUB Constant|MUL Constant|DIV Constant
                switch (_operation) {
                case 0:
                    value = Arithmetic_Add(valueA, _constant);
                    break;
                case 1:
                    value = Arithmetic_Sub(valueA, _constant);
                    break;
                case 2:
                    value = Arithmetic_Mul(valueA, _constant);
                    break;
                case 3:
                    value = Arithmetic_Div(valueA, _constant);
                    break;
                }

                newplane->p(x,y) = value;
            }
        }
    }

    return true;
}

IPLImage* IPLArithmeticOperationsConstant::getResultData( int )
{
    return _result;
}
