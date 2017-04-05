# dlib Changes

At first, you can see original Release notes.
http://dlib.net/release_notes.html

## 1.2.0 (use dlib 19.4)

### New Features

* `Dlib::Image.pyramid_up!` make `Dlib::Image` scale up to x2

## 1.1.5 (use dlib 19.4)

### Bug fixes

* Make GPU memory management better with ruby's GC

## 1.1.4 (use dlib 19.4)

### Bug fixes

* raise CudaError as ruby's Exception (Dlib::CudaError)

## 1.1.3 (use dlib 19.4)

### Bug fixes

* Fix compile option for cuda env

## 1.1.2 (use dlib 19.4)

### Bug fixes

* Fix build path

## 1.1.1 (use dlib 19.4)

### Bug fixes

* fix typo

## 1.1.0 (use dlib 19.4)

### Library

* Upgrade dlib v18.3 -> v19.4
  * It included deep learning libraries

### New features

* Dlib::FrontalFaceDetector - HOG based face detector
* Dlib::DNNFaceDetector - DNN based face detector (needs CUDA SDK and GPU environment)
* Dlib::Cuda - device manupilations of CUDA
