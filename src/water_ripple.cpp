#include <wre/water_ripple.h>
#include <wre/wave_map.hpp>

#include <cstddef>
#include <optional>
#include <stdexcept>
#include <algorithm>
#include <numeric>
#include <assert.h>

namespace wre
{
    water_ripple::water_ripple(uint32_t width, uint32_t height, uint8_t damping)
        : m_wave_map{ width, height },
          m_damping{ damping }
    {
    }

    void water_ripple::reset(uint32_t width, uint32_t height, bool clear_field)
    {
        m_wave_map.reset(width, height, clear_field);
    }

    void water_ripple::clear()
    {
        m_wave_map.clear();
    }

    size_t water_ripple::size() const
    {
        return m_wave_map.size();
    }

    size_t water_ripple::size_bytes() const
    {
        return m_wave_map.size_bytes();
    }

    uint32_t water_ripple::width() const
    {
        return m_wave_map.width();
    }

    uint32_t water_ripple::height() const
    {
        return m_wave_map.height();
    }

    uint8_t water_ripple::damping() const
    {
        return m_damping;
    }

    void water_ripple::render(std::span<const uint8_t> src, 
                              std::span<uint8_t> dest,
                              pixel_format format)
    {
        draw_ripple(src, dest, format);
        m_wave_map.update(m_damping);
        m_wave_map.swap_buffers();
    }

    void water_ripple::make_ripple(int32_t x, int32_t y, 
                                   uint32_t radius, uint32_t blob_height)
    {
        std::span hnew = m_wave_map.buffer(wave_map::buffer_type::current);

        int32_t width = static_cast<int32_t>(m_wave_map.width());
        int32_t height = static_cast<int32_t>(m_wave_map.height());
        uint32_t hoffset{};

        int32_t radius_i32 = static_cast<int32_t>(radius);
        int32_t radius_squared = radius_i32 * radius_i32;

        int32_t cx_squared{};
        int32_t cy_squared{};
        int32_t dx{};
        int32_t dy{};

        // Perform edge clipping... 
        int32_t left   = std::max<int32_t>(-radius_i32, 1 - x);
        int32_t right  = std::min<int32_t>(radius_i32, width - 1 - x);
        int32_t top    = std::max<int32_t>(-radius_i32, 1 - y);
        int32_t bottom = std::min<int32_t>(radius_i32, height - 1 - y);

        // iterate in all pixels inside in the clipping rectangle
        for (int32_t cy = top; cy < bottom; cy++)
        {
            cy_squared = cy * cy;
            dy = y + cy;

            for (int32_t cx = left; cx < right; cx++)
            {
                cx_squared = cx * cx;
                dx = x + cx;

                // check if the pixel inside in the bubble using Pytaghoras Theorem
                // dx^2 + dy^2 < R^2, we exclude the border
                if (cx_squared + cy_squared < radius_squared)
                {
                    // update height map of this pixel
                    hoffset = static_cast<uint32_t>(width * dy + dx);
                    hnew[hoffset] += blob_height;
                }
            }
        }
    }

    [[gnu::noinline]]
    void water_ripple::draw_ripple(std::span<const uint8_t> src, 
                                   std::span<uint8_t> dest,
                                   pixel_format format)
    {
        // the size of src and dest must be same
        [[unlikely]]
        if (src.size() != dest.size())
            throw std::invalid_argument{ "src and dest must have the same size" };

        const uint8_t num_channels = (format == pixel_format::rgb) ? 3 : 4;

        [[unlikely]]
        if ((src.size() / num_channels) != (size() / 2))
            throw std::invalid_argument{ "src must have the same size of wavemap" };

        uint32_t width = m_wave_map.width();
        uint32_t height = m_wave_map.height();
        std::span hptr = m_wave_map.buffer(wave_map::buffer_type::current);

        int32_t img_size = static_cast<int32_t>(width * height);
        uint32_t offset{ width + 1 };
        int32_t dx{};
        int32_t dy{};
        int32_t light_index{};
        
        for (uint32_t last_col{ (height - 1) * width }; offset < last_col; ++offset)
        {
            for (uint32_t last_row{ offset + width - 1 }; offset < last_row; ++offset)
            {
                dx = hptr[offset] - hptr[offset + 1];
                dy = hptr[offset] - hptr[offset + width];
                light_index = offset + width * (dy >> 3) + (dx >> 3);

                if (light_index > 0 && light_index < img_size)
                {
                    for (uint8_t index = 0; index < num_channels; ++index)
                    {
                        dest[num_channels * offset + index] = src[num_channels * light_index + index];
                    }
                }
            }
        }
    }
}