#pragma once

#include <cstddef>
#include <ostream>

/**Replay namespace*/

namespace replay
{

/** A one dimensional interval.
    \ingroup Container
*/
template <class T = float> class interval
{

public:
    /** Empty constructor.
        Leave the elements value-initialized.
    */
    constexpr interval()
    : data_{}
    {
    }

    /**	Constructor.
        \param a first value.
        \param b second value.
        \see replay::make_interval
    */
    constexpr interval(T const& a, T const& b)
    : data_{ a, b }
    {
    }

    /** Get an element from the couple.
     */
    template <typename I> constexpr T& operator[](I i)
    {
        return data_[i];
    }

    /** Get an element from a couple.
     */
    template <typename I> constexpr T const& operator[](I i) const
    {
        return data_[i];
    }

    /** Get the first element.
     */
    constexpr T const& left() const
    {
        return data_[0];
    }

    /** Get the first element.
     */
    constexpr T& left()
    {
        return data_[0];
    }

    /** Get the second element.
     */
    constexpr T const& right() const
    {
        return data_[1];
    }

    /** Get the second element.
     */
    constexpr T const& right()
    {
        return data_[1];
    }

    /** Get a pointer to the array.
     */
    inline T const* ptr() const
    {
        return data_;
    }

    /** Get a pointer to the array.
     */
    inline T* ptr()
    {
        return data_;
    }

    /** Updates the values in the interval.
        \param a the first value.
        \param b the seconf value.
    */
    inline interval& set(T const& a, T const& b)
    {
        data_[0] = a;
        data_[1] = b;
        return *this;
    }

    /** Swap the values in the couple.
     */
    inline interval& swap_sides()
    {
        auto x = std::move(data_[0]);
        data_[0] = std::move(data_[1]);
        data_[1] = std::move(x);
        return *this;
    }

    /** Inequantity.
     */
    bool operator!=(interval const& other) const
    {
        return array[0] != other.array[0] || array[1] != other.array[1];
    }

    /** Equantity.
     */
    bool operator==(interval const& other) const
    {
        return array[0] == other.array[0] && array[1] == other.array[1];
    }

private:
    T data_[2];
};

template <class T>
interval(T left, T right)->interval<T>;

/** Make an interval without providing template parameters
    \param a The first value.
    \param b The second value.
    \return The new couple object.
*/
template <class T> inline interval<T> make_couple(T const& a, T const& b)
{
    return interval<T>(a, b);
}

/** Print the interval to an output stream.
*/
template <class T> inline std::ostream& operator<<(std::ostream& out, interval<T> const& rhs)
{
    return out << '{' << rhs.left() << ".." << rhs.right() << '}';
}

} // namespace replay
