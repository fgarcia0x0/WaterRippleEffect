#pragma once

#include <type_traits>
#include <cstdint>
#include <memory>
#include <span>

namespace wre 
{
    template <typename T>
    class basic_wave_map;

    using wave_map = basic_wave_map<int32_t>;

    template <typename T>
    class basic_wave_map
    {
    public:
        enum class buffer_type { current, previous };
        using value_type = std::decay_t<T>;
        using pointer = std::add_pointer_t<value_type>;

        constexpr basic_wave_map(uint32_t width, uint32_t height)
        {
            reset(width, height);
        }

        constexpr void reset(uint32_t width, uint32_t height, bool clear_field = true)
        {
            m_width = width;
            m_height = height;

            size_t storage_size{ size() };
            size_t storage_half_size{ storage_size >> 1 };

            m_storage = std::make_unique<value_type[]>(storage_size);
            pointer raw_storage{ m_storage.get() };

            m_curr_wm = std::span{ raw_storage, storage_half_size };
            m_prev_wm = std::span{ raw_storage + storage_half_size, storage_half_size };

            if (clear_field)
                clear();
        }

        constexpr void clear(value_type clear_value = {0})
        {
            std::fill_n(m_storage.get(), size(), clear_value);
        }

        constexpr uint32_t width() const
        {
            return m_width;
        }

        constexpr uint32_t height() const
        {
            return m_height;
        }

        constexpr size_t size() const
        {
            return (size_t{ m_width } * m_height) << size_t{ 1 };
        }

        constexpr size_t size_bytes() const
        {
            return size() * sizeof(value_type);
        }

        constexpr auto buffer(buffer_type type)
        { 
            return (type == buffer_type::current) ? m_curr_wm : m_prev_wm;
        }

        void swap_buffers()
        {
            std::swap(m_curr_wm, m_prev_wm);
        }

        const pointer raw_storage() const 
        {
            return m_storage.get();
        }

        void update(uint8_t damping)
        {
            uint32_t row_size = m_width;
            uint32_t row_index = row_size + 1;
            value_type new_height{};

            // for each non-edge pixels
            for (uint32_t y = (m_height - 1) * row_size; row_index < y; row_index += 2)
            {
                // iterates over each pixel in that row starting from the second pixel
                for (uint32_t x = row_index + row_size - 2; row_index < x; row_index++)
                {
                    // This sum it is our smooth function
                    // it does the 8-neighborhood method.
                    new_height = m_prev_wm[row_index + row_size]     +
                                 m_prev_wm[row_index - row_size]     +
                                 m_prev_wm[row_index + 1       ]     +
                                 m_prev_wm[row_index - 1       ]     +
                                 m_prev_wm[row_index - row_size - 1] +
                                 m_prev_wm[row_index - row_size + 1] +
                                 m_prev_wm[row_index + row_size - 1] +
                                 m_prev_wm[row_index + row_size + 1];

                    // To decrease the effect of the velocity
                    // we need a value that is 2 times average
                    // then we must divide the sum of neighbors pixels per 4
                    new_height = (new_height >> 2) - m_curr_wm[row_index];
                    m_curr_wm[row_index] = new_height - (new_height >> damping);
                }
            }
        }

        basic_wave_map(const basic_wave_map& wm)
        {
            reset(wm.m_width, wm.m_height, false);
            std::span src{ wm.raw_storage(), wm.size() };
            std::copy(src.begin(), src.end(), m_storage.get());
        }

        basic_wave_map& operator=(const basic_wave_map& other)
        {
            if (this != &other)
            {
                reset(other.m_width, other.m_height, false);
                std::span src{ other.raw_storage(), other.size() };
                std::copy(src.begin(), src.end(), m_storage.get());
            }

            return *this;
        }

        basic_wave_map(basic_wave_map&& wm) = default;
        basic_wave_map& operator=(basic_wave_map&& wm) = default;

    private:
        uint32_t m_width;
        uint32_t m_height;
        std::unique_ptr<value_type[]> m_storage;
        std::span<value_type> m_curr_wm;
        std::span<value_type> m_prev_wm;
    };
}