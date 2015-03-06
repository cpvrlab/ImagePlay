# Build instructions

## Prerequisites

### Windows

### OSX
First make sure you have a recent version of XCode.

Install [Homebrew](http://brew.sh)

```sh
   brew install qt5
   brew link --force qt5
```

### Linux


## Build

### Windows
```sh
   git clone --recursive https://github.com/cpvrlab/ImagePlay.git
   cd ImagePlay
   qmake -tp vc -r
   
   # use ImagePlay.sln to compile in Visual Studio
```

### OSX
```sh
   git clone --recursive https://github.com/cpvrlab/ImagePlay.git
   cd ImagePlay
   qmake -recursive
   make 
```
