#pragma once

#include <cstdint>
#include <replay/aligned_allocator.hpp>
#include <utility>

namespace replay
{

template <class T> class index_map
{
public:
    using size_type = std::size_t;
    using key_type = size_type;
    using mapped_type = T;
    using value_type = std::pair<const key_type, mapped_type>;
    using allocator_type = replay::aligned_allocator<mapped_type>;
    using mask_element_type = std::uint64_t;

    enum
    {
        bits_per_mask = sizeof(mask_element_type) * 8 / sizeof(std::uint8_t),
    };

    index_map()
    {
    }

    bool empty() const
    {
        return true;
    }

    size_type size() const
    {
        return size_;
    }

    void erase(key_type key)
    {
    }

    void insert(value_type&& value)
    {
        store<value_type&&>(std::move(value));
    }

    void insert(value_type const& value)
    {
        store<value_type const>(value);
    }

    mapped_type& operator[](key_type key)
    {
        return buffer_[key];
    }

    mapped_type const& operator[](key_type key) const;

    mapped_type& at(key_type key);
    mapped_type const& at(key_type key) const;

    void reserve(size_type new_capacity)
    {
        // Can only increase
        if (new_capacity <= capacity_)
            return;

        // Allocate new buffers
        auto allocator = allocator_type();
        auto new_buffer = allocator.allocate(new_capacity);

        // Move all initialized elements to their new location
        for (size_type i = 0; i < capacity_; ++i)
        {
            if (!element_initialized(i))
                continue;

            new (new_buffer + i) mapped_type(std::move(buffer_[i]));
            (buffer_[i]).~mapped_type();
        }

        // Create and initialize a new mask
        auto const current_mask_size = mask_size_for(capacity_);
        auto const new_mask_size = mask_size_for(new_capacity);

        auto const new_mask = new mask_element_type[new_mask_size];
        std::copy(mask_, mask_ + current_mask_size, new_mask);
        std::fill(new_mask + current_mask_size, new_mask + new_mask_size, mask_element_type{ 0 });

        if (buffer_)
        {
            allocator.deallocate(buffer_, capacity_);
            delete[] mask_;
        }

        buffer_ = new_buffer;
        mask_ = new_mask;
        capacity_ = new_capacity;
    }

private:
    bool element_initialized(size_type index)
    {
        return mask_[index / bits_per_mask] & (mask_element_type{ 1 } << (index % bits_per_mask));
    }

    size_type mask_size_for(size_type capacity)
    {
        auto constexpr bits_per_mask = sizeof(mask_element_type) * 8 / sizeof(std::uint8_t);
        return (capacity + bits_per_mask - 1) / bits_per_mask;
    }

    template <class P>
    void store(P value)
    {
        auto index = value.first;

        // Make sure the arrays are big enough
        size_to_include(index);

        if (element_initialized(index))
            return;

        buffer_[index] = value.second;

        ++size_;
    }

    void size_to_include(size_type key)
    {
        // Exponentially grow the buffers
        auto next_capacity = capacity_ > 0 ? capacity_ : 1;
        while (next_capacity <= key)
        {
            next_capacity = 2 * next_capacity;
        }

        reserve(next_capacity);
    }

    size_type size_ = 0;
    size_type capacity_ = 0;

    mapped_type* buffer_ = nullptr;
    mask_element_type* mask_ = nullptr;
};

} // namespace replay