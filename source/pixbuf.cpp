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

#include <boost/cstdint.hpp>
#include <replay/pixbuf.hpp>
#include <vector>

#ifndef DOXYGEN_SHOULD_SKIP_THIS

// Private implementation that holds the image data
class replay::pixbuf::internal_t
{
public:
    typedef boost::uint8_t uint8;
    typedef boost::uint16_t uint16;
    typedef unsigned int uint;

    unsigned char* data;
    unsigned int width;
    unsigned int height;
    unsigned int channels;

    internal_t();
    ~internal_t();

    void create(unsigned int w, unsigned int h, unsigned int c);
    void free();

    void flip();

    uint8* operator()(const uint x, const uint y)
    {
        return data + (y * width + x) * channels;
    }
    const uint8* operator()(const uint x, const uint y) const
    {
        return data + (y * width + x) * channels;
    }
    uint8* operator()(const unsigned int n)
    {
        return data + channels * n;
    }
    const uint8* operator()(const unsigned int n) const
    {
        return data + channels * n;
    }

    uint get_pixel_count() const
    {
        return width * height;
    }

    void set_pixel(unsigned int x, unsigned int y, const replay::pixbuf::byte* color);

    bool blit(unsigned int dx,
              unsigned int dy,
              unsigned int w,
              unsigned int h,
              unsigned int sx,
              unsigned int sy,
              const internal_t& source);

    void fill(const uint8* color);
};

replay::pixbuf::internal_t::internal_t()
: data(0)
, width(0)
, height(0)
, channels(0)
{
}

replay::pixbuf::internal_t::~internal_t()
{
    free();
}

void replay::pixbuf::internal_t::set_pixel(unsigned int x, unsigned int y, const byte* color)
{
    byte* target = data + (y * width + x) * channels;

    for (unsigned int i = 0; i < channels; ++i)
        target[i] = color[i];
}

void replay::pixbuf::internal_t::fill(const uint8* color)
{
    const uint pixelcount = width * height;

    for (uint i = 0; i < pixelcount; ++i)
        for (uint c = 0; c < channels; ++c)
            (*this)(i)[c] = color[c];
}

bool replay::pixbuf::internal_t::blit(unsigned int dx,
                                      unsigned int dy,
                                      unsigned int w,
                                      unsigned int h,
                                      unsigned int sx,
                                      unsigned int sy,
                                      const internal_t& source)
{
    uint8* dst_pixel = 0;
    const uint8* src_pixel = 0;

    if (channels != source.channels)
        return false;

    uint i;
    unsigned int y0;
    unsigned int y1;
    unsigned int ex = std::min(sx + w, source.width);
    unsigned int ey = std::min(sy + h, source.height);

    for (; (sx < ex) && (dx < width); ++sx, ++dx)
    {
        for (y0 = sy, y1 = dy; (y0 < ey) && (y1 < height); ++y0, ++y1)
        {
            dst_pixel = (*this)(dx, y1);
            src_pixel = source(sx, y0);

            for (i = 0; i < channels; ++i)
                dst_pixel[i] = src_pixel[i];
        }
    }

    return true;
}

void replay::pixbuf::internal_t::flip()
{
    uint const half_height = height >> 1;

    // Temporary swap space
    uint row_byte_count = width * channels;
    std::vector<uint8> buffer(row_byte_count);

    for (uint y = 0; y < half_height; ++y)
    {
        uint8* row0 = this->data + (y * width) * channels;
        uint8* row1 = this->data + ((height - 1 - y) * width) * channels;

        // Swap rows
        std::copy(row0, row0 + row_byte_count, buffer.begin());
        std::copy(row1, row1 + row_byte_count, row0);
        std::copy(buffer.begin(), buffer.end(), row1);
    }
}

void replay::pixbuf::internal_t::create(unsigned int w, unsigned int h, unsigned int c)
{
    free();

    width = w;
    height = h;
    channels = c;
    data = new uint8[width * height * channels];
}

void replay::pixbuf::internal_t::free()
{
    delete[] data;
    width = height = channels = 0;
    data = 0;
}

#endif

/** Create a new invalid pixbuf.
*/
replay::pixbuf::pixbuf()
: data(new internal_t)
{
}

/** Free the image data.
*/
replay::pixbuf::~pixbuf()
{
}

/** Get the image width.
*/
unsigned int replay::pixbuf::get_width() const
{
    return data->width;
}

/** Get the image height.
*/
unsigned int replay::pixbuf::get_height() const
{
    return data->height;
}
/** Get the image number of channels.
*/
unsigned int replay::pixbuf::get_channels() const
{
    return data->channels;
}

/** Get a pointer to the pixel data.
*/
unsigned char* replay::pixbuf::get_data()
{
    return data->data;
}

/** Get a constant pointer to the pixel data.
*/
const unsigned char* replay::pixbuf::get_data() const
{
    return data->data;
}

/** Get a pointer to a specific pixel.
    \param i Index of the pixel to get.
*/
const unsigned char* replay::pixbuf::get_pixel(unsigned int i) const
{
    return (*data)(i);
}

/** Get a pointer to a specific pixel.
    \param x Column of the pixel to get.
    \param y Column of the pixel to get.
*/
const unsigned char* replay::pixbuf::get_pixel(unsigned int x, unsigned int y) const
{
    return (*data)(x, y);
}

/** Get a pointer to a specific pixel.
    \param x Column of the pixel to get.
    \param y Column of the pixel to get.
*/
unsigned char* replay::pixbuf::get_pixel(unsigned int x, unsigned int y)
{
    return (*data)(x, y);
}

/** Set the pixel.
*/
void replay::pixbuf::set_pixel(
    const unsigned int x, const unsigned int y, const byte r, const byte g, const byte b, const byte a)
{
    const byte color[4] = { r, g, b, a };

    data->set_pixel(x, y, color);
}

/** Set the pixel.
*/
void replay::pixbuf::set_pixel(const unsigned int x, const unsigned int y, const byte grey)
{
    data->set_pixel(x, y, &grey);
}

/** Create an image with the given parameters.
*/
boost::shared_ptr<replay::pixbuf> replay::pixbuf::create(unsigned int x, unsigned int y, color_format format)
{
    boost::shared_ptr<replay::pixbuf> result(new replay::pixbuf);

    switch (format)
    {
    case rgb:
        result->data->create(x, y, 3);
        break;

    case rgba:
        result->data->create(x, y, 4);
        break;

    default: // greyscale
        result->data->create(x, y, 1);
        break;
    };

    return result;
}

/** Return a section of this image.
    \param x Source offset x-coordinate.
    \param y Source offset y-coordinate.
    \param w Width of the part to copy.
    \param h Height of the part to copy.
*/
boost::shared_ptr<replay::pixbuf>
replay::pixbuf::get_sub_image(unsigned int x, unsigned int y, unsigned int w, unsigned int h)
{
    boost::shared_ptr<replay::pixbuf> result(new replay::pixbuf);

    result->data->create(w, h, this->data->channels);

    result->blit(0, 0, w, h, x, y, *this);

    return result;
}

/**	Copy a part of one image to another.
    \param dx Destination x-coordinate.
    \param dy Destination y-coordinate.
    \param w Width of the part to copy.
    \param h Height of the part to copy.
    \param sx Source offset x-coordinate.
    \param sy Source offset y-coordinate.
    \param source Image to source the data from.
*/

bool replay::pixbuf::blit(unsigned int dx,
                          unsigned int dy,
                          unsigned int w,
                          unsigned int h,
                          unsigned int sx,
                          unsigned int sy,
                          const pixbuf& source)
{
    return this->data->blit(dx, dy, w, h, sx, sy, *source.data);
}

/** Simplified blit. Copies the whole source image to the given coordinates.
    \param dx Destination x-coordinate.
    \param dy Destination y-coordinate.
    \param source Image to source the data from.
*/
bool replay::pixbuf::blit(unsigned int dx, unsigned int dy, const pixbuf& source)
{
    return this->data->blit(dx, dy, source.get_width(), source.get_height(), 0, 0, *source.data);
}

/** Fill the whole image with the given pixel value.
*/
void replay::pixbuf::fill(const byte r, const byte g, const byte b, const byte a)
{
    const byte array[] = { r, g, b, a };
    this->data->fill(array);
}

/** Fill the whole image with the given greyscale value.
*/
void replay::pixbuf::fill(const byte grey)
{
    const byte array[] = { grey, grey, grey, grey };
    this->data->fill(array);
}

/** Flip the image vertically.
*/
void replay::pixbuf::flip()
{
    data->flip();
}

/** Convert this image to 4-channel RGBA format.
*/
void replay::pixbuf::convert_to_rgba()
{
    // Nothing to do
    if (data->channels == 4)
        return;

    std::auto_ptr<internal_t> new_data(new internal_t);

    new_data->create(data->width, data->height, 4);

    internal_t::uint pixel_count = data->get_pixel_count();
    internal_t::uint8* src = data->data;
    internal_t::uint8* dst = new_data->data;

    if (data->channels == 3)
    {
        for (unsigned int i = 0; i < pixel_count; ++i)
        {
            dst[0] = src[0];
            dst[1] = src[1];
            dst[2] = src[2];
            dst[3] = 255;

            src += 3;
            dst += 4;
        }
    }
    else if (data->channels == 1)
    {
        for (unsigned int i = 0; i < pixel_count; ++i)
        {

            dst[0] = src[0];
            dst[1] = src[0];
            dst[2] = src[0];
            dst[3] = 255;

            ++src;
            dst += 4;
        }
    }

    this->data.reset();
    this->data = new_data;
}

/** \defgroup Imaging Image manipulation, loading and saving.
*/
