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

#include <replay/box_packer.hpp>
#include <replay/vector2.hpp>

#ifndef DOXYGEN_SHOULD_SKIP_THIS

class replay::box_packer::node
{
public:
    node(replay::box<int> const& rect = replay::box<int>())
    : rectangle(rect)
    , in_use(false)
    {
    }

    node(node const&) = delete;
    void operator=(node const&) = delete;

    node(replay::box<int> const& rect, std::unique_ptr<node> left_child, std::unique_ptr<node> right_child)
    : child{std::move(left_child), std::move(right_child)}
    , rectangle(rect)
    , in_use(false)
    {

    }

    ~node()
    {
    }

    replay::box<int> const& get_rectangle() const throw()
    {
        return rectangle;
    }

    node const* insert(replay::v2<int> const& size, int padding);

private:
    std::unique_ptr<node> child[2];
    replay::box<int> rectangle;
    bool in_use;
};

const replay::box_packer::node* replay::box_packer::node::insert(v2<int> const& size, int padding)
{
    if (in_use)
        return nullptr;

    // Try to recurse down
    if (child[0] != nullptr)
    {
        node const* result = child[0]->insert(size, padding);

        if (result == nullptr)
            result = child[1]->insert(size, padding);

        in_use = child[0]->in_use && child[1]->in_use;
        return result;
    }

    const int dw = rectangle.get_width() - size[0];
    const int dh = rectangle.get_height() - size[1];

    // Too big?
    if ((dw < 0) || (dh < 0))
        return nullptr;

    // Perfect fit?
    if ((dw == 0) && (dh == 0))
    {
        this->in_use = true;
        return this;
    }

    child[0] = std::make_unique<node>();
    child[1] = std::make_unique<node>();

    // split
    if (dw > dh)
    {
        // divide width
        child[0]->rectangle.set(rectangle.left, rectangle.bottom, rectangle.left + size[0], rectangle.top);

        child[1]->rectangle.set(rectangle.left + size[0] + (padding << 1), rectangle.bottom, rectangle.right,
                               rectangle.top);
    }
    else
    {
        // devide height
        child[0]->rectangle.set(rectangle.left, rectangle.bottom, rectangle.right, rectangle.bottom + size[1]);

        child[1]->rectangle.set(rectangle.left, rectangle.bottom + size[1] + (padding << 1), rectangle.right,
                               rectangle.top);
    }

    // first child is constructed to fit, so insert it there
    return child[0]->insert(size, padding);
}

#endif

replay::box_packer::box_packer()
: root(nullptr)
, padding(0)
{
}

/** Create a new box packer.
    \param width The width of the area to pack in
    \param height The height of the area to pack in
    \param padding Space to be left between rectangles
*/
replay::box_packer::box_packer(int width, int height, int padding)
: root(new node(box<int>(padding, padding, width - padding, height - padding)))
, padding(padding)
{
}

void replay::box_packer::enlarge(int width, int height)
{
    auto old_rectangle = root->get_rectangle();

    if (width < old_rectangle.right + padding || height < old_rectangle.top + padding)
        throw std::invalid_argument("enlarge only to larger sizes");

    auto extended = std::make_unique<node>(box<int>(old_rectangle.left + 2 * padding, old_rectangle.bottom + 2 * padding, width - padding, height - padding));
    auto new_root = std::make_unique<node>(box<int>(padding, padding, width - padding, height - padding), std::move(root), std::move(extended));

    root = std::move(new_root);
}

replay::box_packer::box_packer(box_packer&& rhs)
: root(std::move(rhs.root))
, padding(rhs.padding)
{
}

/** dtor.
*/
replay::box_packer::~box_packer()
{
}

replay::box_packer& replay::box_packer::operator=(box_packer&& rhs)
{
    if (this != &rhs)
    {
        root = std::move(rhs.root);
        padding = rhs.padding;
    }

    return *this;
}

/** Pack an item of the given size.
    If there is no more space left to pack the given item, the function
    will throw an box_packer::pack_overflow exception.
    \param width The width of the item to pack
    \param height The height of the item to pack
*/
replay::box<int> const& replay::box_packer::pack(int width, int height)
{
    if (root == nullptr)
        throw pack_overflow();

    const node* result = root->insert(v2<int>(width, height), this->padding);

    if (result == nullptr)
        throw pack_overflow();

    return result->get_rectangle();
}

/** Pack an item of the given size.
    This is the exception free variant: if there is no more space left to pack the given item, the function
    will return false.
    \param width The width of the item to pack.
    \param height The height of the item to pack.
    \param rect The result rect where the item was placed.
*/
bool replay::box_packer::pack(int width, int height, replay::box<int>* rect)
{
    if (root == nullptr)
        return false;

    const node* result = root->insert(v2<int>(width, height), this->padding);

    if (result == nullptr)
        return false;

    if (rect != nullptr)
        *rect = result->get_rectangle();

    return true;
}

/** get the width (without padding).
*/
int replay::box_packer::get_width() const
{
    return root ? root->get_rectangle().get_width() + 2 * padding : 0;
}

/** get the height (without padding).
*/
int replay::box_packer::get_height() const
{
    return root ? root->get_rectangle().get_height() + 2 * padding : 0;
}

/** get the padding between boxes.
*/
int replay::box_packer::get_padding() const
{
    return padding;
}
