## Overview ##

**Replay** is a lightweight game development utility library. It contains data structures for 3D maths, such as vectors, quaternions and matrices. It also contains common algorithms that operate on these data structures and other utility data structures that are useful in game development.

It is thoroughly documented with Doxygen (http://www.doxygen.org) comments. The API reference can be found in the link section on the right.

The implementation of critical algorithms is checked via unit testing (using Boost.Test).

## Features ##
  * 2,3 and 4 dimensional vector classes and math functions.
  * 2x2, 3x3 and 4x4 matrix classes.
  * Quaternions and affine maps.
  * Raster image loading and modification.
  * 2 dimensional object containers.
  * Geometry algorithms, such as minimal bounding spheres for points or convex hulls.
  * Bounding boxes for 2 and 3 dimensions.

## Install ##

To use replay, just check out the source for the subversion server. There are currently build files for:
  * Visual Studio 2010
  * CMake
Linux is supported via CMake. Build file contributions for other systems are very welcome.

## History ##

Replay has been used in game development related code since it's inception in 2006. Since then, it has been steadily updated. It has proven it's reliability and has matured quite a lot, so it was decided to release it as open-source.

## Dependencies ##
Replay depends (only) on the following libraries:
  * The C++ Standard Library.
  * The **Boost** C++ Libraries (http://www.boost.org/).

And optionally:
  * **libpng** (http://www.libpng.org/).
  * **zlib** (http://www.zlib.net/).
