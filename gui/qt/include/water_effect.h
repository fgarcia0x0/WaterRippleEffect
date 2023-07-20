#pragma once

#include <wre/water_ripple.h>

#include <memory>
#include <string_view>

#include <QImage>

class water_effect
{
public:
    water_effect(std::string_view image_path);

    int32_t width() { return m_width; }
    int32_t height() { return m_height; }
    int32_t damping() { return 5; }

    void make_ripple(int32_t x, int32_t y, uint32_t radius, uint32_t blob_height);
    void clear();
    void render();
    QImage get_render_output();

private:
    int32_t m_width;
    int32_t m_height;
    wre::pixel_format m_pixel_format;
    std::vector<uint8_t> m_render_src;
    std::vector<uint8_t> m_render_out;
    std::unique_ptr<wre::water_ripple> m_water_ripple;
};
