#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <replay/byte_rgba.hpp>

namespace replay
{

/** Pixel based image.
    \note The image data is stored row-wise without padding, beginning with the bottom-most. This is different from,
   e.g., the Windows API, where images are stored with the top-most row first.
    \ingroup Imaging
*/
class pixbuf
{

public:
    /** A shared (reference-counted) pointer to a pixbuf.
     */
    using shared_pixbuf = std::shared_ptr<pixbuf>;

    /** Color-Format.
     */
    enum class color_format
    {
        greyscale, /**< Greyscale (8-bit). */
        rgb,       /**< Red, Green, Blue (24-bit). */
        rgba       /**< Red, Green, Blue and Alpha (32-bit). */
    };

    using byte = std::uint8_t;
    using index_type = std::size_t;
    using iterator = byte*; // Warning: This is an implementation detail
    using const_iterator = byte const*; // Warning: This is an implementation detail 

    pixbuf();
    pixbuf(index_type w, index_type h, color_format format);
    pixbuf(index_type w, index_type h, index_type channel_count);
    pixbuf(pixbuf&& rhs) noexcept;
    pixbuf(pixbuf const& rhs);
    ~pixbuf();

    pixbuf& operator=(pixbuf&& rhs) noexcept;
    pixbuf& operator=(pixbuf const& rhs);

    index_type width() const;
    index_type height() const;
    index_type channel_count() const;

    color_format pixel_format() const;

    byte const* ptr() const;
    byte* ptr();

    byte* ptr(index_type i);
    byte const* ptr(index_type i) const;
    byte* ptr(index_type x, index_type y);
    byte const* ptr(index_type x, index_type y) const;

    const_iterator begin() const;
    iterator begin();

    const_iterator end() const;
    iterator end();

    /** The size in bytes.
    */
    index_type size() const;

    bool empty() const;

    void blit_from(
        index_type dx, index_type dy, pixbuf const& source, index_type w, index_type h, index_type sx, index_type sy);

    void blit_from(index_type dx, index_type dy, pixbuf const& source);

    void fill(byte_rgba rgba);
    void fill(byte r, byte g, byte b, byte a = 255);
    void fill(byte grey);

    void flip();

    void assign_pixel(index_type x, index_type y, byte_rgba rgba);
    void assign_pixel(index_type x, index_type y, byte r, byte g, byte b, byte a);
    void assign_pixel(index_type x, index_type y, byte grey);

    byte_rgba read_pixel(index_type x, index_type y) const;

    void convert_to_rgba();

    pixbuf crop(index_type x, index_type y, index_type w, index_type h) const;

private:
    byte* data_;
    index_type width_;
    index_type height_;
    index_type channel_count_;
};

/** A shared (reference-counted) pointer to a pixbuf.
 */
using shared_pixbuf = pixbuf::shared_pixbuf;
} // namespace replay

