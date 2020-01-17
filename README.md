# Replay #

[![Build Status](https://travis-ci.org/ltjax/replay.svg?branch=master)](https://travis-ci.org/ltjax/replay)
[![Build status](https://ci.appveyor.com/api/projects/status/0b8i5chwamnpfmxp/branch/master?svg=true)](https://ci.appveyor.com/project/ltjax/replay/branch/master)

## Overview ##

**Replay** is a lightweight game development utility library. It contains data structures for 3D maths, such as vectors, quaternions and matrices. It also contains common algorithms that operate on these data structures and other utility data structures that are useful in game development.

## Features ##
  * 2,3 and 4 dimensional vector classes and math functions.
  * 2x2, 3x3 and 4x4 matrix classes.
  * Quaternions and affine maps.
  * Raster image loading and modification.
  * 2 dimensional object containers.
  * Geometry algorithms, such as minimal bounding spheres for points or convex hulls.
  * Bounding boxes for 2 and 3 dimensions.

## Install ##

They easiest way to use replay is to use conan. Add the remote via:
```
conan remote add ltjax https://api.bintray.com/conan/ltjax/conan 
```
Then add a dependency to your project:
```
replay/2.4@ltjax/stable
```

Alternatively, replay can be build using CMake.

## History ##

Replay has been used in game development related code since it's inception in 2006. Since then, it has been steadily updated. It has proven it's reliability and has matured quite a lot, so it was decided to release it as open-source.

## Dependencies ##
Replay depends (only) on the following libraries:
  * The C++ Standard Library.
  * The **Boost** C++ Libraries (http://www.boost.org/).

And optionally:
  * **stb_image** (https://github.com/nothings/stb)
  * **libpng** (http://www.libpng.org/).
  * **zlib** (http://www.zlib.net/).
