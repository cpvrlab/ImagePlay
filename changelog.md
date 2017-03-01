# Change Log

## 6.1.0 - 2017-03-01
### Added
- Added Fervor upate checker, you will be notified about future updates (> 6.1.0).
- Updated to Qt 5.6 and OpenCV 3.1.0
- Improved plugin API. We now check for API level which should make things more stable.
- New IPLEvent allows to react to process GUI events from buttons when overriding processPropertyEvents

### Fixed
- Fixed some issues with IPLLabelBlobs

### Known Issues
- While using a IPLCamera, the UI can be pretty unresponsive. Pause to adjust the UI.
- The application may still be unstable, expect crashes (please report them on https://github.com/cpvrlab/ImagePlay/issues)

## 6.0.0 - 2015-11-06
### Added
- Improved OS X retina dispay support
- You can now directly open .ipj files when associated with ImagePlay.exe

### Fixed
- Fixed some issues with IPLLabelBlobs

### Known Issues
- While using a IPLCamera, the UI can be pretty unresponsive. Pause to adjust the UI.
- The application may still be unstable, expect crashes (please report them on https://github.com/cpvrlab/ImagePlay/issues)

## 6.0.0-rc.3 - 2015-11-05
### Added
- Added examples (located under /examples/ or directly on the DMG for Mac OS X)
- Inputs/Outputs are now shown for each process right underneath the process properties
- We now have a completely new plugin system
- File paths can now be relative
- Added better event logging (needs to be enabled in settings)

### Fixed
- Improved IPPropertyKernelInt
- Improved blueprint background image
- Fixed scene clearing (could lead to crashes before)

### Known Issues
- Mac OS X: Building ImagePlay may produce errors after macdeployqt ../_bin/Release/macx/ImagePlay.app/ -dmg
 - those errors only affect the creation of the dmg file, you can still run ImagePlay.app
- Mac OS X: The Image Viewer might only display a small portion of an image.
 - Workaround: Load a large image
- While using a IPLCamera, the UI can be pretty unresponsive. Pause to adjust the UI.
- The application may still be unstable, expect crashes (please report them on https://github.com/cpvrlab/ImagePlay/issues)


## 6.0.0-rc.2 - 2015-10-16
### Added
- New button which allows to reset all process properties to their defaults

### Fixed
- Fixed a crash in IPLSaveImage

### Known Issues
- Mac OS X: Building ImagePlay may produce errors after macdeployqt ../_bin/Release/macx/ImagePlay.app/ -dmg
 - those errors only affect the creation of the dmg file, you can still run ImagePlay.app
- Mac OS X: The Image Viewer might only display a small portion of an image.
 - Workaround: Load a large image
- While using a IPLCamera, the UI can be pretty unresponsive. Pause to adjust the UI.
- The application may still be unstable, expect crashes (please report them on https://github.com/cpvrlab/ImagePlay/issues)


## 6.0.0-rc.1 - 2015-10-09
### Added
- Tabs in the image viewer are now automatically sorted by their execution order
- Compatibility for building under OS X 10.11, see QMAKE_MAC_SDK = macosx10.11

### Fixed
- Improved live reloading of plugins
- Improved IPLCamera, stability of .ipj loading, zoom widget
- Graph state gets updated when removing edges, invalidating following results
- Decreased memory usage of ImageViewer, improved behaviour ouf ZoomVie…


### Known Issues
- Mac OS X: Building ImagePlay may produce errors after macdeployqt ../_bin/Release/macx/ImagePlay.app/ -dmg
 - those errors only affect the creation of the dmg file, you can still run ImagePlay.app
- Mac OS X: The Image Viewer might only display a small portion of an image.
 - Workaround: Load a large image
- While using a IPLCamera, the UI can be pretty unresponsive. Pause to adjust the UI.
- The application may still be unstable, expect crashes (please report them on https://github.com/cpvrlab/ImagePlay/issues)

## 6.0.0-rc.0 - 2015-09-18
### Added
- New mode for loading RAW image files when dimensions and byte order are known
- New algorithms
 - Gradient Operator
 - Normalize Illumination

### Fixed
- Improved plugin generator and plugin loading stability
- Improved reliability of the process grid recomputation

### Known Issues
- Mac OS X: Building ImagePlay may produce errors after macdeployqt ../_bin/Release/macx/ImagePlay.app/ -dmg
 - those errors only affect the creation of the dmg file, you can still run ImagePlay.app
- Mac OS X: The Image Viewer might only display a small portion of an image.
 - Workaround: Load a large image
- Histogram and Zoom might not update correctly.
- While using a IPLCamera, the UI can be pretty unresponsive. Pause to adjust the UI.
- The application may still be unstable, expect crashes (please report them on https://github.com/cpvrlab/ImagePlay/issues)

## 6.0.0-beta.5 - 2015-08-28
### Added
- Added an "open recent" menu.
- Drag & drop for loading .ipj files directly.
- Added a setting which specifies where the /plugin_development/ folder is located.
- Scrollable space in the main window now automatically grows.
- New option to always show the ImageViewer on top
- New property type IPL_WIDGET_MATRIX
- New algorithms
 - Undistort
 - Warp Affine
 - Warp Perspective
- We now provide a nightly build for Windows at https://ci.appveyor.com/project/cpvrlab/imageplay/build/artifacts

### Fixed
- ImageViewer now zooms all images to the same level.
- Drifting scrollbars in ImageViewer
- Crash when using IPLCamera and closing the app
- Default value for hough transform
- Allow any input for IPLMergePlanes

### Known Issues
- Mac OS X: Building ImagePlay may produce errors after macdeployqt ../_bin/Release/macx/ImagePlay.app/ -dmg
 - those errors only affect the creation of the dmg file, you can still run ImagePlay.app
- Mac OS X: The Image Viewer might only display a small portion of an image.
 - Workaround: Load a large image
- Histogram and Zoom might not update correctly.
- While using a IPLCamera, the UI can be pretty unresponsive. Pause to adjust the UI.
- The application may still be unstable, expect crashes (please report them on https://github.com/cpvrlab/ImagePlay/issues)

## 6.0.0-beta.4 - 2015-08-14
### Added
- You can now drag & drop files or folders from your desktop directly to ImagePlay.

### Fixed
- Allow empty convolution kernel.
- Crash while zooming empty viewer.
- Mac OS X, gesture on viewer crash.
- Typos ...

### Known Issues
- Mac OS X: Building ImagePlay may produce errors after macdeployqt ../_bin/Release/macx/ImagePlay.app/ -dmg
 - those errors only affect the creation of the dmg file, you can still run ImagePlay.app
- Mac OS X: The Image Viewer might only display a small portion of an image.
 - Workaround: Load a large image
- Histogram and Zoom might not update correctly.
- While using a IPLCamera, the UI can be pretty unresponsive. Pause to adjust the UI.
- The application may still be unstable, expect crashes (please report them on https://github.com/cpvrlab/ImagePlay/issues)

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
