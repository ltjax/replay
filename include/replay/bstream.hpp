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

#ifndef replay_bstream_hpp
#define replay_bstream_hpp

#include <boost/numeric/conversion/cast.hpp>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>

namespace replay
{

/** Binary input stream wrapper.
    Allows wrapping of standard streams for binary reading.
*/

class input_binary_stream
{
public:
    /** Construct the stream wrapper.
     */
    explicit input_binary_stream(std::istream& rhs)
    : m_stream(rhs)
    {
    }

    /** Read the stream.
    \param pointer Pointer to the stream.
    \param s Size
    \return *this*/
    input_binary_stream& read(void* pointer, std::streamsize s)
    {
        m_stream.read(reinterpret_cast<char*>(pointer), s);
        return *this;
    }

    /** Reads the stream, size is calculated from the type.
        \param result The variable to be read to.
        \return *this.
    */
    template <class T> input_binary_stream& read(T& result)
    {
        static_assert(std::is_pod<T>::value, "Can only read PODs directly");
        m_stream.read(reinterpret_cast<char*>(&result), sizeof(T));
        return *this;
    }

    /** Reads the stream, size is calculated from the type.
        \return The data that was read.
    */
    template <class T> T read()
    {
        T result;
        m_stream.read(reinterpret_cast<char*>(&result), sizeof(T));
        return result;
    }

    /**Reading operator for unsigned ints.*/
    input_binary_stream& operator>>(boost::uint32_t& x)
    {
        return read(x);
    }
    /**Reading operator for signed ints.*/
    input_binary_stream& operator>>(boost::int32_t& x)
    {
        return read(x);
    }

    /**Binary stream reading operator for unsigned chars.*/
    input_binary_stream& operator>>(boost::uint8_t& x)
    {
        return read(x);
    }

    /**Reading operator for signed chars.*/
    input_binary_stream& operator>>(boost::int8_t& x)
    {
        return read(x);
    }

    /**Reading operator for unsigned shorts.*/
    input_binary_stream& operator>>(boost::uint16_t& x)
    {
        return read(x);
    }
    /**Reading operator for signed shorts.*/
    input_binary_stream& operator>>(boost::int16_t& x)
    {
        return read(x);
    }

    /**Reading operator for floats.*/
    input_binary_stream& operator>>(float& x)
    {
        return read(x);
    }
    /**Reading operator for doubles.*/
    input_binary_stream& operator>>(double& x)
    {
        return read(x);
    }

    /**	Reading operator for strings.
     */
    inline input_binary_stream& operator>>(std::string& x)
    {
        boost::uint32_t const length = read<boost::uint32_t>();

        if (length > 0)
        {
            x.resize(length);
            read(&(x[0]), length);
        }

        return *this;
    }

private:
    std::istream& m_stream;
};

/** Binary output stream Wrapper.
    Allows wrapping of standard streams for binary writing.
*/
class output_binary_stream
{
public:
    /** Construct the stream wrapper.
     */
    explicit output_binary_stream(std::ostream& rhs)
    : m_stream(rhs)
    {
    }

    /**Write to the stream.
    \param pointer Pointer to the stream.
    \param s Size
    \return *this*/
    output_binary_stream& write(void const* pointer, std::streamsize s)
    {
        m_stream.write(reinterpret_cast<char const*>(pointer), s);
        return *this;
    }

    /**Writes to the stream, size is calculated from the type.
    \param x The stream to write to.
    \return *this*/
    template <class T> output_binary_stream& write(T const& rhs)
    {
        m_stream.write(reinterpret_cast<char const*>(&rhs), sizeof(T));
        return *this;
    }

    /**Writing operator for unsigned ints.*/
    output_binary_stream& operator<<(boost::uint32_t rhs)
    {
        return write(rhs);
    }
    /**Writing operator for signed ints.*/
    output_binary_stream& operator<<(boost::int32_t rhs)
    {
        return write(rhs);
    }

    /**Writing operator for unsigned chars.*/
    output_binary_stream& operator<<(boost::uint8_t rhs)
    {
        return write(rhs);
    }
    /**Writing operator for signed chars.*/
    output_binary_stream& operator<<(boost::int8_t rhs)
    {
        return write(rhs);
    }

    /**Writing operator for unsigned shorts.*/
    output_binary_stream& operator<<(boost::uint16_t rhs)
    {
        return write(rhs);
    }
    /**Writing operator for signed shorts.*/
    output_binary_stream& operator<<(boost::int16_t rhs)
    {
        return write(rhs);
    }

    /**Writing operator for floats.*/
    output_binary_stream& operator<<(float rhs)
    {
        return write(rhs);
    }
    /**Writing operator for doubles.*/
    output_binary_stream& operator<<(double rhs)
    {
        return write(rhs);
    }

private:
    std::ostream& m_stream;
};

/** Writing operator for strings.
 */
inline output_binary_stream& operator<<(output_binary_stream& lhs, std::string const& rhs)
{
    lhs.write(boost::numeric_cast<boost::uint32_t>(rhs.size()));

    if (rhs.length() > 0)
    {
        lhs.write(&(rhs[0]), rhs.size());
    }

    return lhs;
}

inline input_binary_stream binary_stream(std::istream& rhs)
{
    return input_binary_stream(rhs);
}

inline output_binary_stream binary_stream(std::ostream& rhs)
{
    return output_binary_stream(rhs);
}
}

#endif // replay_bstream_hpp
