#ifndef %CLASSNAME%_H
#define %CLASSNAME%_H

#include <QObject>

#include "IPL_plugininterface.h"
#include "IPL_global.h"
#include "IPLProcess.h"

/*!
 * \The plugin container class
 */
class %CLASSNAME%Plugin : public QObject, PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "ch.bfh.cpvrlab.ImagePlay.PluginInterface/1.0")
    Q_INTERFACES(PluginInterface)
public:
    QString name()      { return QString("%CLASSNAME%");}
    QString author()    { return QString("%AUTHOR%");}
    QString version()   { return QString("1.0");}
    IPLProcess* getProcess()
    {
        _process = new %CLASSNAME%Plugin::%CLASSNAME%;
        return _process;
    }
private:
    IPLProcess* _process;

    /*!
     * \ The process implementation
     */

    class %CLASSNAME% : public IPLClonableProcess<%CLASSNAME%>
    {
    public:
        %CLASSNAME%() : IPLClonableProcess() { init(); }
        ~%CLASSNAME%()  { destroy(); }

        void init()
        {
            // init
            _result     = NULL;

            // basic settings
            setClassName("%CLASSNAME%Plugin::%CLASSNAME%");
            setTitle("%TITLE%");
            setCategory(IPLProcess::%CATEGORY%);
			setOpenCVSupport(IPLOpenCVSupport::%OPENCV%);
            setDescription("%DESCRIPTION%");
            setKeywords("%KEYWORDS%");

            // inputs and outputs
            addInput("Image", IPLData::IMAGE_COLOR);
            addOutput("Gray Image", IPLImage::IMAGE_BW);

            // properties
            addProcessPropertyDouble("threshold", "Threshold", "0.0 < threshold < 1.0", 0.5, IPL_WIDGET_SLIDER, 0.0, 1.0);
        }

        void destroy()
        {
            delete _result;
        }

        bool processInputData(IPLImage* input, int, bool useOpenCV)
        {
            // delete previous result
            delete _result;
            _result = NULL;

            int width = image->width();
            int height = image->height();
            if( image->type() == IPLData::IMAGE_GRAYSCALE )
                _result = new IPLImage( IPLImage::IMAGE_BW, width, height );
            else
                _result = new IPLImage( image->type(), width, height );

            // get properties
            float threshold = getProcessPropertyDouble("threshold");

            int progress = 0;
            int maxProgress = image->height() * image->getNumberOfPlanes();
            int nrOfPlanes = image->getNumberOfPlanes();

            #pragma omp parallel for
            for( int planeNr=0; planeNr < nrOfPlanes; planeNr++ )
            {
                IPLImagePlane* plane = image->plane( planeNr );
                IPLImagePlane* newplane = _result->plane( planeNr );
                for(int y=0; y<height; y++)
                {
                    // progress
                    notifyProgressEventHandler(100*progress++/maxProgress);
                    for(int x=0; x<width; x++)
                    {
                        newplane->p(x,y) = (plane->p(x,y) < threshold) ? 0.0f : 1.0f;
                    }
                }
            }
            return true;
        }

        IPLData* getResultData(int)
        {
            return _result;
        }

    private:
        IPLData*  _result;
    };
};

#endif // %CLASSNAME%_H

