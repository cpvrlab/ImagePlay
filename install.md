# Build instructions

## Prerequisites

### Windows
* Install [Git](http://git-scm.com/download/win/)
* Install Visual Studio 2013 or later:
   * [Visual Studio Community Edition](https://www.visualstudio.com/en-us/products/visual-studio-community-vs/)
    or
   * [Visual Studio Express 2013 for Windows Desktop](https://www.visualstudio.com/downloads/)
* Install [Qt (Version 5.4 or greater)](http://www.qt.io/download-open-source/)
   * Make sure you select the correct version (`msvc2013 64-bit` or similar)
* Add Qt to your PATH (`C:\Qt\5.4\msvc2013_64\bin` or similar)

### OSX
First make sure you have a recent version of XCode.

Install [Homebrew](http://brew.sh)

```sh
brew install qt5
brew link --force qt5
```

### Linux
The following packages are necessary to compile (Debian/Ubuntu).

```sh
sudo apt-get install qt5-default qt5-qmake git
sudo apt-get install libfreeimage-dev libopencv-core-dev libopencv-core-dev libopencv-imgproc-dev libopencv-highgui-dev
```

## Build

### Windows
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

### OSX
```sh
git clone --recursive https://github.com/cpvrlab/ImagePlay.git
cd ImagePlay
qmake -recursive
make 
```
