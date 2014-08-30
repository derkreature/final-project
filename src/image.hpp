#ifndef IMAGE_H
#define IMAGE_H

#include <FreeImage.h>
#include <glm/glm.hpp>

#include <string>

//<< Describes a set of channels (out of R, G, B)
enum channels
{
    R   = 1 << 0,
    G   = 1 << 1,
    B   = 1 << 2,
    RG  = R | G,
    RB  = R | B,
    GB  = G | B,
    RGB = R | G | B,
};

inline channels operator|(const channels& x, const channels& y)
{
    return (channels)((unsigned)x | (unsigned)y);
}

//<< General purpose image class, backed by a FreeImage bitmap
//<< Pixels are represented as an OpenGL-compatible glm::vec4
class image
{
public:
    image& operator=(const image &other);
    image(size_t width, size_t height);
    image(const std::string& path);
    image(const image &other);
    image(FIBITMAP *dib);
    ~image();

    //<< Loads a new image file, or saves this image to an EXR file
    void load(const std::string& path);
    void save(const std::string& path) const;

    //<< Pointwise addition/etc of channels of another image to this one
    void add(const image& other, const channels& which = channels::RGB);
    void sub(const image& other, const channels& which = channels::RGB);
    void mul(const image& other, const channels& which = channels::RGB);

    //<< Fills channels of the image with a given background color
    void fill(const glm::vec4& bkgd, const channels& which = channels::RGB);

    //<< Normalizes channels of this image such that:
    //<<  - the channel sums up to 1, if local is false
    //<<  - each pixel channel value is at most 1, otherwise
    void normalize(bool local, const channels& which = channels::RGB);

    //<< Resizes this image to new dimensions with a given filter, and returns the result
    image resize(size_t newWidth, size_t newHeight, FREE_IMAGE_FILTER filter = FILTER_CATMULLROM) const;

    //<< Returns the subregion of this image specified by a given rectangle
    image subregion(size_t rectX, size_t rectY, size_t rectW, size_t rectH) const;

    //<< Returns this image, padded on the sides with a black background
    image zero_pad(size_t left, size_t top, size_t right, size_t bottom) const;

    //<< Accesses the scanline pointer for a given row
    glm::vec4* operator[](size_t y);
	const glm::vec4* operator[](size_t y) const;

    //<< Accesses the pixel at a given point
    glm::vec4& operator()(size_t x, size_t y);
	const glm::vec4& operator()(size_t x, size_t y) const;

    //<< Accesses the width and height of the image
    size_t width() const;
    size_t height() const;
private:
    FIBITMAP *dib;
};

#endif