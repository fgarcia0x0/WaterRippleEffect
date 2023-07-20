#pragma once

#include <wre/wave_map.hpp>

#include <memory>
#include <cstdint>
#include <span>
#include <random>

namespace wre 
{
    enum class pixel_format
    {
        rgb,
        rgba
    };

    class water_ripple
    {
    public:
        /**
         * @brief Construct a new water_ripple object
         * 
         * @param width The width of the output buffer
         * @param height The height of the output buffer
         * @param damping The damping factor of the ripple in range 
                          0 <= damping <= 31
         * @details The damping factor is a log2(2^x) where x is the damping factor
         *                 
         */
        water_ripple(uint32_t width, uint32_t height, uint8_t damping = 5);

        /**
         * @brief Reallocate the WaveMap with a new width and height
         * 
         * @param width A new width value
         * @param height A new height value
         * @param clear_field if true fill all buffer with zeroes
         */
        void reset(uint32_t width, uint32_t height, bool clear_field = true);

        /**
         * @brief Clear the WaveMap
         */
        void clear();

        /**
         * @brief Get the width of WaveMap
         * 
         * @return the width of WaveMap
         */
        uint32_t width() const;

        /**
         * @brief Get the height of WaveMap
         * 
         * @return the height of WaveMap 
         */
        uint32_t height() const;

        /**
         * @brief Get damping factor of the ripple
         * 
         * @return the damping factor of the ripple 
         */
        uint8_t damping() const;

        /**
         * @brief Get the size of the WaveMap
         * 
         * @return The size of the WaveMap
         */
        size_t size() const;

        /**
         * @brief Get the size of the WaveMap in bytes
         * 
         * @return The size of the WaveMap in bytes
         */
        size_t size_bytes() const;

        /**
         * @brief Renderize the water ripple
         * @note The buffers must be RGB or RGBA
         * @note The src and dest must have the same size and cannot be aliased
         * 
         * @param src The input buffer (surface)
         * @param dest The output buffer where changes will be applied
         */
        void render(std::span<const uint8_t> src, std::span<uint8_t> dest, pixel_format format = pixel_format::rgb);

        /**
         * @brief Creates a disturbance in the wave_map generating a ripple on the surface
         * 
         * @param x The x position of the image rectangle
         * @param y The y position of the image rectangle
         * @param radius The radius of the ripple
         * @param blob_height The height of the ripple disturbance
         */
        void make_ripple(int32_t x, int32_t y, uint32_t radius, uint32_t blob_height);

        /**
         * @brief Get a pointer to the raw storage of the wave_map
         * 
         * @return a pointer to wave_map storage
         */
        wave_map::value_type* raw_storage() const 
        {
            return m_wave_map.raw_storage();
        }

    private:
        void draw_ripple(std::span<const uint8_t> src, 
                         std::span<uint8_t> dest, 
                         pixel_format format);
    private:
        wave_map m_wave_map;
        uint8_t  m_damping;
    };
}