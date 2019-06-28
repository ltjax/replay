#pragma once

namespace replay
{

template <typename T> class aligned_allocator
{
public:
    using pointer = T*;
    using size_type = size_t;

    pointer allocate(size_type n);
    void deallocate(pointer p, size_type n);

}; // class aligned_allocatortemplate <typename T>

template <typename T> typename aligned_allocator<T>::pointer aligned_allocator<T>::allocate(size_type n)
{
    size_type const alignment = std::max(alignof(ptrdiff_t), alignof(T));
    size_type const object_size = sizeof(ptrdiff_t) + sizeof(T) * n;
    size_type const buffer_size = object_size + alignment;

    void* const block = std::malloc(buffer_size);
    if (block == nullptr)
    {
        throw std::bad_alloc{};
    }

    void* storage = reinterpret_cast<char*>(block) + sizeof(ptrdiff_t);
    size_t shift = buffer_size;

    void* const body = std::align(alignment, object_size, storage, shift);
    char* const offset = reinterpret_cast<char*>(body) - sizeof(ptrdiff_t);

    *reinterpret_cast<ptrdiff_t*>(offset) = sizeof(ptrdiff_t) + shift;

    return reinterpret_cast<pointer>(body);
} // aligned_allocator<T>::allocate 

template <typename T> void aligned_allocator<T>::deallocate(pointer p, size_type)
{
    char const* header = reinterpret_cast<char*>(p) - sizeof(ptrdiff_t);
    auto offset = *reinterpret_cast<ptrdiff_t const*>(header);
    void* const block = reinterpret_cast<char*>(p) - offset;
    std::free(block);
} // aligned_allocator<T>::deallocate

} // namespace replay