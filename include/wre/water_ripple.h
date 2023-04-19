#pragma once

#include <wre/wave_map.hpp>

#include <memory>
#include <cstdint>
#include <span>
#include <random>

namespace wre 
{
    class water_ripple
    {
    public:
        water_ripple(uint32_t width, uint32_t height, uint8_t damping = 5);

        void reset(uint32_t width, uint32_t height, bool clear_field = true);
        void clear();

        uint32_t width() const;
        uint32_t height() const;

        size_t size() const;
        size_t size_bytes() const;

        void render(std::span<const uint32_t> src, std::span<uint32_t> dest);
        void make_bubble(int32_t x, int32_t y, uint32_t radius, uint32_t blob_height);

    private:
        void draw_ripple(std::span<const uint32_t> src, std::span<uint32_t> dest);
    private:
        wave_map m_wave_map;
        uint8_t  m_damping;
    };
}