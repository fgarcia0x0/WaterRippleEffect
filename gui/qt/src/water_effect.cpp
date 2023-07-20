#include <water_effect.h>

#include <stdexcept>
#include <string_view>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

water_effect::water_effect(std::string_view image_path)
{
    int width{};
    int height{};
    int channels{};
    
    uint8_t* srcptr = stbi_load(image_path.data(), &width, &height, &channels, 0);
    size_t size = static_cast<size_t>(width * height * channels);

    if (!srcptr)
        throw std::invalid_argument{ "cannot load image" };
    
    if (!size)
        throw std::runtime_error{ "image size cannot be null" };

    m_width = width;
    m_height = height;
    m_pixel_format = (channels == 3) ? wre::pixel_format::rgb : wre::pixel_format::rgba;

    m_render_src.resize(size);
    std::copy(srcptr, srcptr + size, m_render_src.data());

    m_render_out.resize(size);
    std::copy(srcptr, srcptr + size, m_render_out.data());

    m_water_ripple = std::make_unique<wre::water_ripple>(m_width, m_height, 5);

    stbi_image_free(srcptr);
}

void water_effect::make_ripple(int32_t x, int32_t y, uint32_t radius, uint32_t blob_height)
{
    m_water_ripple->make_ripple(x, y, radius, blob_height);
}

void water_effect::clear()
{
    m_water_ripple->clear();
}

void water_effect::render()
{
    m_water_ripple->render(m_render_src, m_render_out, m_pixel_format);
}

QImage water_effect::get_render_output()
{
    QImage::Format format = QImage::Format_RGB888;

    if (m_pixel_format == wre::pixel_format::rgba)
        format = QImage::Format_RGBA8888;

    return QImage(m_render_out.data(), m_width, m_height, format);
}