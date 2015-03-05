How to implement IPLProcess
===================================================

\tableofcontents

\section Introduction


\section Class Declaration
Each IPLProcess must inherit from IPLClonableProcess and implement the methods init, processInputData,
getResultData. It usually needs an instance of IPLData to save the resulting data.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
class IPLSHARED_EXPORT IPLExample : public IPLClonableProcess<IPLExample>
{
public:
                            IPLExample        		();
                            ~IPLExample       		();
    void                    init();
    virtual bool            processInputData        (IPLImage*, int, bool);
    virtual IPLImage*       getResultData           (int);
protected:
    IPLImage*               _result;
};
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

\section Implementation
Your methods are called according to the following protocol:
- Constructor: is called exactly once when starting ImagePlay.
- init(): is called once for every instance of this process step which is created by the user.
- processInputData: is called whenever the input or any of the parameters for the process step has changed.
- getResultData: is called when the following process step is requesting input data or for displaying the result to the user.

\subsection Init

This method is used to define inputs and outputs of the process step and their types.
It is also the place to define process properties, which result in GUI options for user changeable parameters.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
void IPLExample::init()
{
    // inputs and outputs
    addInput("Image", IPLData::IMAGE_COLOR);
    addOutput("Image", IPLImage::IMAGE_COLOR);

    // properties
    addProcessPropertyDouble("threshold", "Threshold", "0.0 < threshold < 1.0", IPL_DOUBLE_SLIDER, 0.5, 0.0, 1.0);
}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~