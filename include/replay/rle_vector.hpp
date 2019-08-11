#pragma once

#include <stdexcept>
#include <vector>

namespace replay
{

template <typename T> class rle_vector
{
public:
    using backing_container = std::vector<std::pair<T, std::size_t>>;
    using backing_iterator = typename backing_container::const_iterator;

    using size_type = std::size_t;
    using value_type = T;

    class iterator
    {
    public:
        using value_type = rle_vector::value_type;
        using reference = value_type const&;
        using pointer = value_type const*;
        using difference_type = std::ptrdiff_t;
        using iterator_category = std::forward_iterator_tag;

        iterator(backing_iterator backing, size_type index)
        : backing_(backing)
        , index_(index)
        {
        }

        bool operator==(iterator const& rhs) const
        {
            return backing_ == rhs.backing_ && index_ == rhs.index_;
        }

        bool operator!=(iterator const& rhs) const
        {
            return backing_ != rhs.backing_ || index_ != rhs.index_;
        }

        iterator& operator++()
        {
            ++index_;
            if (index_ == backing_->second)
            {
                ++backing_;
                index_ = 0;
            }
            return *this;
        }

        iterator const operator++(int)
        {
            auto result = *this;
            ++(*this);
            return result;
        }

        iterator& operator+=(size_type rhs)
        {
            auto new_index = index_ + rhs;
            while (new_index > 0 && new_index >= backing_->second)
            {
                new_index -= backing_->second;
                ++backing_;
            }
            index_ = new_index;
            return *this;
        }

        reference operator*() const
        {
            return backing_->first;
        }

        pointer operator->() const
        {
            return &backing_->first;
        }

        size_type repetition_count() const
        {
            return backing_->second - index_;
        }

        iterator operator+(size_type rhs) const
        {
            auto result = *this;
            return result += rhs;
        }
    private:
        backing_iterator backing_;
        size_type index_;
    };

    rle_vector()
    {
        size_ = 0;
    }

    rle_vector(size_type count, T value)
    : values_(1, std::make_pair(value, count))
    , size_(count)
    {
    }

    rle_vector(std::initializer_list<std::pair<T, std::size_t>> list)
    : values_(list)
    , size_(0)
    {
        for (auto const& each : values_)
            size_ += each.second;
    }

    void push(T value, size_type count = 1)
    {
        if (count == 0)
        {
            throw std::invalid_argument("Cannot add element without repetitions");
        }

        values_.push_back(std::make_pair(value, count));
        size_ += count;
    }

    iterator begin() const
    {
        return iterator(values_.begin(), 0);
    }

    iterator end() const
    {
        return iterator(values_.end(), 0);
    }

    size_type size() const
    {
        return size_;
    }

    bool empty() const
    {
        return values_.empty();
    }

    bool operator==(rle_vector const& rhs) const
    {
        return size_ == rhs.size_ && values_ == rhs.values_;
    }

    bool operator!=(rle_vector const& rhs) const
    {
        return !(*this == rhs);
    }

    typename backing_container::value_type const* data() const
    {
        return values_.data();
    }

private:
    backing_container values_;
    size_type size_;
};

} // namespace replay