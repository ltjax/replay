/*
replay
Software Library

Copyright (c) 2010-2019 Marius Elvert

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

#include <cstdint>
#include <filesystem>
#include <replay/bstream.hpp>
#include <replay/pixbuf_io.hpp>
#include <boost/numeric/conversion/cast.hpp>

#ifdef REPLAY_USE_STBIMAGE
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#endif

#ifdef REPLAY_USE_STBIMAGE_WRITE
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>
#endif

namespace
{

int stb_read_callback(void* user, char* data, int size)
{
    std::istream* file(reinterpret_cast<std::istream*>(user));
    file->read(data, size);
    return static_cast<int>(file->gcount());
}

void stb_skip_callback(void* user, int n)
{

    std::istream* file(reinterpret_cast<std::istream*>(user));
    for (int i = 0; i < n; ++i)
        file->get();
}

int stb_eof_callback(void* user)
{
    std::istream* file(reinterpret_cast<std::istream*>(user));
    return file->eof() ? 1 : 0;
}

class tga_header
{

public:
    tga_header()
    : id_length(0)
    , colormap_type(0)
    , image_type(0)
    , width(0)
    , height(0)
    , pixeldepth(0)
    , image_descriptor(0)
    {
        colormap[0] = colormap[1] = colormap[2] = colormap[3] = colormap[4] = 0;
        origin[0] = origin[1] = 0;
    }

    replay::pixbuf load(replay::input_binary_stream& file);
    void save(replay::output_binary_stream& file, replay::pixbuf const& source);

private:
    std::uint8_t id_length;
    std::uint8_t colormap_type;
    std::uint8_t image_type;
    std::uint8_t colormap[5];
    std::uint16_t origin[2];
    std::uint16_t width;
    std::uint16_t height;
    std::uint8_t pixeldepth;
    std::uint8_t image_descriptor;

    replay::pixbuf load_type2(replay::input_binary_stream& file);
};

} // namespace

#ifdef REPLAY_USE_STBIMAGE_WRITE
/** Serialize by encoding a PNG file via stbimage_write.
    \param file The file to serialize to.
    \param source The image to serialize.
    \ingroup Imaging
*/
void replay::pixbuf_io::save_to_png_file(std::ostream& file, pixbuf const& source)
{
    auto write_callback = [](void* context, void* data, int size) {
        auto file = reinterpret_cast<std::ostream*>(context);
        file->write(reinterpret_cast<char const*>(data), size);
    };

    auto stride = boost::numeric_cast<int>(source.width() * source.channel_count());
    auto width = boost::numeric_cast<int>(source.width());
    auto height = boost::numeric_cast<int>(source.height());
    auto channel_count = boost::numeric_cast<int>(source.channel_count());

    stbi_write_png_to_func(write_callback, &file, width, height, channel_count,
                           source.ptr() + stride * (height - 1), -stride);
}
#endif

replay::pixbuf tga_header::load_type2(replay::input_binary_stream& file)
{
    using namespace replay;

    // read in colormap dummy data
    file >> colormap[0] >> colormap[1] >> colormap[2] >> colormap[3] >> colormap[4];

    // read in image specification
    file >> origin[0] >> origin[1] >> width >> height >> pixeldepth >> image_descriptor;

    // only TARGA-24 and TARGA-32 are supported
    if ((pixeldepth != 24) && (pixeldepth != 32))
        throw pixbuf_io::unrecognized_format();

    // read the freeform id
    std::uint8_t dummy;
    for (int i = 0; i < id_length; ++i)
        file >> dummy;

    using index_type = replay::pixbuf::index_type;

    // now read the image data
    replay::pixbuf result(width, height, pixeldepth == 24 ? pixbuf::color_format::rgb : pixbuf::color_format::rgba);
    auto pixelcount = index_type(width) * height;
    auto pixel = result.ptr();
    std::uint8_t buffer[4];

    if (pixeldepth == 24)
    {
        for (index_type i = 0; i < pixelcount; ++i)
        {
            file.read(buffer, 3);
            *(pixel++) = buffer[2];
            *(pixel++) = buffer[1];
            *(pixel++) = buffer[0];
        }
    }
    else // pixeldepth == 32
    {
        for (index_type i = 0; i < pixelcount; ++i)
        {
            file.read(buffer, 4);
            *(pixel++) = buffer[2];
            *(pixel++) = buffer[1];
            *(pixel++) = buffer[0];
            *(pixel++) = buffer[3];
        }
    }

    return result;
}

/** Deserialize a TGA encoded file.
    \ingroup Imaging
*/
replay::pixbuf replay::pixbuf_io::load_from_tga_file(std::istream& file)
{
    tga_header header;
    input_binary_stream binary_file(file);

    return header.load(binary_file);
}

/** Serialize by encoding a TGA file.
    \ingroup Imaging
*/
void replay::pixbuf_io::save_to_tga_file(std::ostream& file, const pixbuf& source)
{
    tga_header header;
    output_binary_stream binary_file(file);

    header.save(binary_file, source);
}

/** Save an image.
    \note Only TGA and PNG are supported right now.
    \param filename Path of the file to be saved.
    \param source The image to be saved.
    \ingroup Imaging
*/
void replay::pixbuf_io::save_to_file(std::filesystem::path const& filename, pixbuf const& source)
{
    auto const extension = filename.extension().string();

    std::ofstream file;

    file.exceptions(std::ifstream::badbit | std::ifstream::eofbit | std::ifstream::failbit);
    file.open(filename, std::ios_base::out | std::ios_base::binary);

    if (extension == ".tga")
    {
        save_to_tga_file(file, source);
    }
#ifdef REPLAY_USE_STBIMAGE_WRITE
    else if (extension == ".png")
    {
        save_to_png_file(file, source);
    }
#endif
    else
    {
        throw pixbuf_io::unrecognized_format();
    }
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS

replay::pixbuf tga_header::load(replay::input_binary_stream& file)
{
    using namespace replay;

    file >> id_length >> colormap_type >> image_type;

    // colormaps are not supported
    if (colormap_type != 0)
        throw pixbuf_io::unrecognized_format();

    replay::pixbuf result;

    switch (image_type)
    {
    // uncompressed, unmapped BGR image
    case 2:
        result = load_type2(file);
        break;

        // FIXME: add this
        // compressed, unmapped
        // case 10: load_type10( dst, file ); break;

    default:
        throw pixbuf_io::unrecognized_format();
    }

    if (image_descriptor & (1 << 5))
        result.flip();

    return result;
}

void tga_header::save(replay::output_binary_stream& file, replay::pixbuf const& source)
{
    if (!source.empty() && ((source.channel_count() == 3) || (source.channel_count() == 4)))
    {
        image_type = 2;
        width = boost::numeric_cast<std::uint16_t>(source.width());
        height = boost::numeric_cast<std::uint16_t>(source.height());
        pixeldepth = boost::numeric_cast<std::uint8_t>(source.channel_count() * 8);
    }
    else
    {
        throw replay::pixbuf_io::write_error();
    }

    file << id_length << colormap_type << image_type;

    // write colormap dummy data
    file << colormap[0] << colormap[1] << colormap[2] << colormap[3] << colormap[4];

    // write image specification
    file << origin[0] << origin[1] << width << height << pixeldepth << image_descriptor;

    unsigned int pixelcount = width * height;
    const std::uint8_t* pixel = source.ptr();
    std::uint8_t buffer[4];
    if (pixeldepth == 24)
    {
        for (unsigned int i = 0; i < pixelcount; ++i)
        {
            buffer[2] = *(pixel++);
            buffer[1] = *(pixel++);
            buffer[0] = *(pixel++);

            file.write(buffer, 3);
        }
    }
    else // pixeldepth == 32
    {
        for (unsigned int i = 0; i < pixelcount; ++i)
        {
            buffer[2] = *(pixel++);
            buffer[1] = *(pixel++);
            buffer[0] = *(pixel++);
            buffer[3] = *(pixel++);

            file.write(buffer, 4);
        }
    }
}

/** Load an image.
    The format is guessed from the filename's extension.
    \note Only TGA and PNG are supported right now.
    \param filename Path of the file to be loaded.
    \ingroup Imaging
*/
replay::pixbuf replay::pixbuf_io::load_from_file(std::filesystem::path const& filename)
{
    auto const extension = filename.extension().string();

    std::ifstream file;
    file.open(filename, std::ios_base::in | std::ios_base::binary);

    if (!file.good())
        throw read_error("Unable to open file " + filename.string());

#ifdef REPLAY_USE_STBIMAGE
    stbi_io_callbacks callbacks;
    callbacks.read = &stb_read_callback;
    callbacks.skip = &stb_skip_callback;
    callbacks.eof = &stb_eof_callback;

    int rx = 0, ry = 0, comp = 0;
    auto* data = stbi_load_from_callbacks(&callbacks, static_cast<std::istream*>(&file), &rx, &ry, &comp, 0);

    if (!data)
    {
        throw pixbuf_io::unrecognized_format();
    }

    replay::pixbuf::color_format format = pixbuf::color_format::rgba;
    if (comp != 4)
    {
        if (comp == 3)
            format = replay::pixbuf::color_format::rgb;
        else if (comp == 1)
            format = replay::pixbuf::color_format::greyscale;
        else
        {
            stbi_image_free(data);
            throw pixbuf_io::unrecognized_format();
        }
    }
    pixbuf result(rx, ry, format);
    auto target = result.ptr();

    std::size_t byte_count = rx * ry * comp;
    for (std::size_t i = 0; i < byte_count; ++i)
        target[i] = data[i];

    stbi_image_free(data);
    result.flip();
    return result;
#else
    file.exceptions(std::ifstream::badbit | std::ifstream::eofbit | std::ifstream::failbit);

    if (extension == ".tga")
    {
        return load_from_tga_file(file);
    }
#ifdef REPLAY_USE_LIBPNG
    else if (extension == ".png")
    {
        return load_from_png_file(file);
    }
#endif

    throw pixbuf_io::unrecognized_format();
#endif
}

/** Load an image.
    The format is guessed from the files contents.
    \param filename Path of the file to be loaded.
    \ingroup Imaging
*/
replay::pixbuf replay::pixbuf_io::load_from_file(std::istream& file)
{
#ifdef REPLAY_USE_STBIMAGE
    stbi_io_callbacks callbacks;
    callbacks.read = &stb_read_callback;
    callbacks.skip = &stb_skip_callback;
    callbacks.eof = &stb_eof_callback;

    int rx = 0, ry = 0, comp = 0;
    auto* data = stbi_load_from_callbacks(&callbacks, static_cast<std::istream*>(&file), &rx, &ry, &comp, 0);

    if (!data)
    {
        const char* failure = stbi_failure_reason();
        throw pixbuf_io::read_error(failure);
    }

    replay::pixbuf::color_format format = pixbuf::color_format::rgba;
    if (comp != 4)
    {
        if (comp == 3)
            format = replay::pixbuf::color_format::rgb;
        else if (comp == 1)
            format = replay::pixbuf::color_format::greyscale;
        else
        {
            stbi_image_free(data);
            throw pixbuf_io::unrecognized_format();
        }
    }
    pixbuf result(rx, ry, format);
    auto target = result.ptr();

    std::size_t byte_count = rx * ry * comp;
    for (std::size_t i = 0; i < byte_count; ++i)
        target[i] = data[i];

    stbi_image_free(data);
    result.flip();
    return result;
#else
    throw pixbuf_io::unrecognized_format();
#endif
}
#endif
