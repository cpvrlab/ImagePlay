# Build instructions

[Windows](#windows)

[macOS](#macos)

[Linux](#linux)

## Windows
### Prerequisites
* Install [Git](http://git-scm.com/download/win/)
* Install Visual Studio 2013:
   * [Visual Studio Community Edition](https://www.visualstudio.com/en-us/products/visual-studio-community-vs/)
    or
   * [Visual Studio Express 2013 for Windows Desktop](https://www.visualstudio.com/downloads/)
* Install [Qt (Version 5.4 or greater)](http://www.qt.io/download-open-source/)
   * Make sure you select the correct version (`msvc2013 64-bit` or similar)
* Add Qt to your PATH (`C:\Qt\5.4\msvc2013_64\bin` or similar)

### Build
```sh
git clone --recursive https://github.com/cpvrlab/ImagePlay.git
cd ImagePlay
qmake -tp vc -r
```
**Visual Studio:**
 * Open **ImagePlay.sln**
 * Set *ImagePlay* as startup project.
 * Run

**Qt Creator:**
 * Open **ImagePlay.pro**
 * Disable *Shadow build* in the project settings.
 * Run

## macOS
### Prerequisites
First make sure you have a recent version of XCode.

Install [Homebrew](http://brew.sh)

```sh
brew install qt5
brew link --force qt5
brew tap homebrew/science
brew install opencv3
```
### Build
```sh
git clone --recursive https://github.com/cpvrlab/ImagePlay.git
cd ImagePlay
qmake -recursive
make 
```
__Note__
Currently the build based on homebrews qt5.rb has some difficulties to find the correct libraries. You can use [Qt Creator (Version 5.5)](http://www.qt.io/download-open-source/) where you load the *Imageplay.pro* project with the *Desktop QT 5.5* profile.



## Linux
### Prerequisites
The following packages are necessary to compile (Debian/Ubuntu).

```sh
sudo apt-get install qt5-default qt5-qmake git
sudo apt-get install libfreeimage-dev libopencv-core-dev libopencv-core-dev libopencv-imgproc-dev libopencv-highgui-dev
```

### Build
```sh
git clone --recursive https://github.com/cpvrlab/ImagePlay.git
cd ImagePlay
qmake -recursive
make 
```

### Install
```sh
sudo make install
```
The following files are installed:
- `/usr/bin/imageplay`
- `/usr/share/imageplay/*`

### Debug build
Append `CONFIG+=debug` to the `qmake -recursive` command (note: In such a case, `make` current does not rebuild `imageplay` binary automatically)