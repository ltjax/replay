#pragma once

#include <cstdint>
#include <replay/aligned_allocator.hpp>
#include <stdexcept>
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

    template <bool Const> class base_iterator
    {
    public:
        using value_type = std::conditional_t<Const, std::add_const_t<index_map::mapped_type>, index_map::mapped_type>;
        using reference = value_type&;
        using pointer = value_type*;
        using difference_type = std::ptrdiff_t;
        using iterator_category = std::bidirectional_iterator_tag;

        using container_type = std::conditional_t<Const, std::add_const_t<index_map>, index_map>;

        base_iterator(container_type* parent, size_type index)
        : parent_(parent)
        , index_(index)
        {
            skip_invalid_forward();
        }

        base_iterator(base_iterator const&) = default;
        base_iterator& operator=(base_iterator const&) = default;

        base_iterator& operator++()
        {
            ++index_;
            skip_invalid_forward();
            return *this;
        }

        base_iterator operator++(int) const
        {
            auto result = *this;
            ++(*this);
            return result;
        }

        base_iterator& operator--()
        {
            --index_;
            skip_invalid_backward();
            return *this;
        }

        base_iterator operator--(int) const
        {
            auto result = *this;
            --(*this);
            return result;
        }

        reference operator*()
        {
            return (*parent_)[index_];
        }

        pointer operator->()
        {
            return &((*parent_)[index_]);
        }

        template <bool OtherConst> bool operator==(base_iterator<OtherConst> const& rhs) const
        {
            return index_ == rhs.index_;
        }

        template <bool OtherConst> bool operator<(base_iterator<OtherConst> const& rhs) const
        {
            return index_ < rhs.index_;
        }

        template <bool OtherConst> bool operator>(base_iterator<OtherConst> const& rhs) const
        {
            return index > rhs.index_;
        }

        template <bool OtherConst> bool operator<=(base_iterator<OtherConst> const& rhs) const
        {
            return index_ <= rhs.index_;
        }

        template <bool OtherConst> bool operator>=(base_iterator<OtherConst> const& rhs) const
        {
            return index >= rhs.index_;
        }

        template <bool OtherConst> bool operator!=(base_iterator<OtherConst> const& rhs) const
        {
            return index_ != rhs.index_;
        }

    private:
        void skip_invalid_forward()
        {
            while (index_ < parent_->capacity_ && !parent_->element_initialized(index_))
                ++index_;
        }

        void skip_invalid_backward()
        {
            while (index >= 1 && !parent_->element_initialized(index_))
                --index_;
        }

        container_type* parent_;
        size_type index_;
    };

    using iterator = typename base_iterator<false>;
    using const_iterator = typename base_iterator<true>;

    enum
    {
        bits_per_mask = sizeof(mask_element_type) * 8 / sizeof(std::uint8_t),
    };

    index_map()
    {
    }

    index_map(index_map const& rhs)
    {
        if (!rhs.capacity_)
            return;

        // Allocate new buffers
        auto allocator = allocator_type();
        auto new_buffer = allocator.allocate(rhs.capacity_);

        // Move all initialized elements to their new location
        for (size_type i = 0; i < rhs.capacity_; ++i)
        {
            if (!rhs.element_initialized(i))
                continue;

            new (new_buffer + i) mapped_type(rhs.buffer_[i]);
        }

        // Create and initialize a new mask
        auto const mask_size = mask_size_for(rhs.capacity_);
        auto const new_mask = new mask_element_type[mask_size];
        std::copy(rhs.mask_, rhs.mask_ + mask_size, new_mask);

        capacity_ = rhs.capacity_;
        buffer_ = new_buffer;
        mask_ = new_mask;
    }

    index_map(index_map&& rhs)
    : size_(rhs.size_)
    , capacity_(rhs.capacity_)
    , buffer_(rhs.buffer_)
    , mask_(rhs.mask_)
    {
        rhs.null_out();
    }

    ~index_map()
    {
        free_memory();
    }

    index_map& operator=(index_map const& rhs)
    {
        *this = index_map(rhs); // copy-construct and move
        return *this;
    }

    index_map& operator=(index_map&& rhs)
    {
        if (&rhs == this)
            return *this;

        free_memory();
        size_ = rhs.size_;
        capacity_ = rhs.capacity_;
        buffer_ = rhs.buffer_;
        mask_ = rhs.mask_;

        rhs.null_out();
        return *this;
    }

    bool empty() const
    {
        return size_ == 0;
    }

    size_type size() const
    {
        return size_;
    }

    void erase(key_type key)
    {
        if (key >= capacity_ || !element_initialized(key))
            return;

        --size_;
        buffer_[key].~mapped_type();
        mask_[key / bits_per_mask] &= ~((mask_element_type{ 1 } << (key % bits_per_mask)));
    }

    void insert(value_type&& value)
    {
        auto index = value.first;

        // Make sure the arrays are big enough
        size_to_include(index);

        if (element_initialized(index))
            return;

        new (buffer_ + index) mapped_type(std::move(value.second));

        ++size_;
        mask_[index / bits_per_mask] |= mask_element_type{ 1 } << (index % bits_per_mask);
    }

    void insert(value_type const& value)
    {
        auto index = value.first;

        // Make sure the arrays are big enough
        size_to_include(index);

        if (element_initialized(index))
            return;

        new (buffer_ + index) mapped_type(value.second);

        ++size_;
        mask_[index / bits_per_mask] |= mask_element_type{ 1 } << (index % bits_per_mask);
    }

    mapped_type& operator[](key_type key)
    {
        return const_cast<mapped_type&>(const_cast<index_map<T> const&>(*this)[key]);
    }

    mapped_type const& operator[](key_type key) const
    {
        return buffer_[key];
    }

    mapped_type& at(key_type key)
    {
        return const_cast<mapped_type&>(const_cast<index_map<T> const&>(*this).at(key));
    }

    mapped_type const& at(key_type key) const
    {
        if (key >= capacity_)
            throw std::out_of_range("Key past end of container");
        if (!element_initialized(key))
            throw std::out_of_range("Element not inserted");
        return (*this)[key];
    }

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

        free_memory();

        buffer_ = new_buffer;
        mask_ = new_mask;
        capacity_ = new_capacity;
    }

    iterator begin()
    {
        return iterator(this, 0);
    }

    iterator end()
    {
        return iterator(this, capacity_);
    }

    const_iterator begin() const
    {
        return const_iterator(this, 0);
    }

    const_iterator end() const
    {
        return const_iterator(this, capacity_);
    }

    size_type capacity() const
    {
        return capacity_;
    }

private:
    void free_memory()
    {
        if (buffer_ != nullptr)
        {
            allocator_type().deallocate(buffer_, capacity_);
            delete[] mask_;
        }
    }

    void null_out()
    {
        size_ = 0;
        capacity_ = 0;
        buffer_ = 0;
        mask_ = 0;
    }

    bool element_initialized(size_type index) const
    {
        return mask_[index / bits_per_mask] & (mask_element_type{ 1 } << (index % bits_per_mask));
    }

    size_type mask_size_for(size_type capacity)
    {
        auto constexpr bits_per_mask = sizeof(mask_element_type) * 8 / sizeof(std::uint8_t);
        return (capacity + bits_per_mask - 1) / bits_per_mask;
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