# Change Log

## 6.0.0-beta.4 - 2015-08-14
### Fixed
- Allow empty convolution kernel.
- Crash while zooming empty viewer.
- Mac OS X, gesture on viewer crash.
- Typos ...

## 6.0.0-beta.3 - 2015-06-26
### Added
- New button to go back from process propeties [ < ]
- New property widget: IPL_WIDGET_GROUP which enables/disables groups of properties (see IPLResize)
- A few new OpenCV implementations
- New events are now called: beforeProcessing and afterProcessing
- New progress indicator for indefinitely long processes, use notifyProgressEventHandler(-1)
- File dialog allows to deactivate file filters.
- Added OpenCV support to the binary morphology process.

### Changed
- Play mode is now automatically started when launching the application

### Fixed
- Fixed IPL_WIDGET_SLIDER, IPL_WIDGET_SLIDER_ODD, IPL_WIDGET_SLIDER_EVEN which failed to update the image
- Added a check for empty kernels to the binary morphology process.
- General stability improvements

### Known Issues
- Mac OS X: Building ImagePlay may produce errors after macdeployqt ../_bin/Release/macx/ImagePlay.app/ -dmg
 - those errors only affect the creation of the dmg file, you can still run ImagePlay.app
- Mac OS X: The Image Viewer might only display a small portion of an image.
 - Workaround: Load a large image
- Histogram and Zoom might not update correctly.
- While using a IPLCamera, the UI can be pretty unresponsive. Pause to adjust the UI.
- The application may still be unstable, expect crashes (please report them on https://github.com/cpvrlab/ImagePlay/issues)


## 6.0.0-beta.2 - 2015-04-02
### Added
- A first version of the plugin SDK is supplied in /plugin_development/
 - To set up your plugin, use "ImagePlay -> Tools -> Generate Plugin Code"
- Keyboard shortcut for quickly searching in the process library: [CTRL + F] or [CMD + F]
- New property widgets: IPL_WIDGET_SLIDER_ODD and IPL_WIDGET_SLIDER_EVEN
- OpenCV implementation for 2D convolutions
- FFT, Filtering and IFFT

### Changed
- Switched the property GUI code to RTTI based dynamic_cast calls

### Fixed
- Process items in the graph can't be placed over other items anymore
- Fixed a severe threading issue which occurred when using IPLProcess::addMessage
- General stability improvements

### Known Issues
- Mac OS X: Building ImagePlay may produce errors after macdeployqt ../_bin/Release/macx/ImagePlay.app/ -dmg
 - those errors only affect the creation of the dmg file, you can still run ImagePlay.app
- Histogram and Zoom might not update correctly. Histogram channels don't workd yet (RGB, etc...)
- While using a IPLCamera, the UI can be pretty unresponsive. Pause to adjust the UI.
- The application may still be unstable, expect crashes (please report them on https://github.com/cpvrlab/ImagePlay/issues)


## 6.0.0-beta.1 - 2015-03-18
### Added
- Added example images in /images/

### Changed
- Mac OS X: ImageViewer now stays on top of the MainWindow

### Fixed
- IPLCamera now works better with up to 30fps and should now terminate all threads on exit.
- IPLCanny now works with opencv enabled.
- General stability improvements

### Known Issues
- Mac OS X: Building ImagePlay may produce errors after macdeployqt ../_bin/Release/macx/ImagePlay.app/ -dmg
 - those errors only affect the creation of the dmg file, you can still run ImagePlay.app
- The application may still be unstable, expect crashes (please report them on https://github.com/cpvrlab/ImagePlay/issues)
