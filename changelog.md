# Change Log

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
