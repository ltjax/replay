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

#pragma once

#include <deque>
#include <mutex>
#include <condition_variable>
#include <optional>

namespace replay
{

/** Single-producer, single-consumer concurrent queue.
 */
template <class T> class concurrent_queue
{
public:
    void push(T value)
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_queue.push_back(std::move(value));
        m_push_signal.notify_one();
    }

    void push(T value, std::size_t max_size)
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        if (m_queue.size() >= max_size)
        {
            m_pop_signal.wait(lock, [this, max_size] { return m_queue.size() < max_size; });
        }
        m_queue.push_back(std::move(value));
        m_push_signal.notify_one();
    }

    T pop()
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        if (m_queue.empty())
        {
            m_push_signal.wait(lock, [this] { return !m_queue.empty(); });
        }

        T result = std::move(m_queue.front());
        m_queue.pop_front();
        m_pop_signal.notify_one();
        return result;
    }

    std::optional<T> pop_optional()
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        if (m_queue.empty())
        {
            return {};
        }

        auto value = std::move(m_queue.front());
        m_queue.pop_front();
        m_pop_signal.notify_one();
        return std::make_optional(std::move(value));
    }

private:
    std::mutex m_mutex;
    std::condition_variable m_push_signal;
    std::condition_variable m_pop_signal;
    std::deque<T> m_queue;
};
} // namespace replay
