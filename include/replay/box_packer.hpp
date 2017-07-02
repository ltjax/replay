/*
replay
Software Library

Copyright (c) 2010 Marius Elvert

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.

*/

#ifndef replay_box_packer_hpp
#define replay_box_packer_hpp

#include <boost/utility.hpp>
#include <replay/box.hpp>
#include <replay/couple.hpp>
#include <stdexcept>

namespace replay
{

/** A box packer for 2-dimensions.
    This algorithm can be used to position a set of axis-aligned rectangles in the plane,
    so that they do not overlap and that all rectangles together need only a small bounding box.
    It can be used to generate texture atlases.
    This uses a first fit packing algorithm.
*/
class box_packer : public boost::noncopyable
{
public:
    /** Exception class that is thrown when a rect cannot be packed.
        \see pixbuf_packer::pack
    */
    class pack_overflow : public std::exception
    {
    };

    box_packer(int width, int height, int padding = 0);
    ~box_packer();

    const box<int>& pack(int width, int height);

    bool pack(int width, int height, box<int>* rect);

    int get_width() const;
    int get_height() const;
    int get_padding() const;

private:
    class node;
    node* root;
    int padding;
};
}

#endif // replay_box_packer_hpp
