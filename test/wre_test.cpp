#include <wre/water_ripple.h>

#include <algorithm>
#include <gtest/gtest.h>

static inline bool wre_cmp(const wre::water_ripple& a, 
                           const wre::water_ripple& b)
{
    std::span a_span{ a.raw_storage(), a.size() };
    std::span b_span{ b.raw_storage(), b.size() };

    return (a.width()      == b.width())        &&
           (a.height()     == b.height())       &&
           (a.size()       == b.size())         &&
           (a.size_bytes() == b.size_bytes())   &&
           (a.damping()    == b.damping())      &&
           (std::equal(a_span.begin(), a_span.end(), b_span.begin()));
}

TEST(wre, ctor)
{
    wre::water_ripple water_surface{ 1920, 1080, 5 };
    ASSERT_EQ(water_surface.width(), 1920);
    ASSERT_EQ(water_surface.height(), 1080);
    ASSERT_EQ(water_surface.damping(), 5);
}

TEST(wre, wave_map_size)
{
    uint32_t width{ 560 };
    uint32_t height{ 420 };
    uint8_t damping{ 7 };
    size_t new_size = size_t{ 2 } * width * height;

    wre::water_ripple water_surface{ width, height, damping };
    ASSERT_EQ(water_surface.size(), new_size);
    ASSERT_EQ(water_surface.size_bytes(), new_size * sizeof(wre::wave_map::value_type));
}

TEST(wre, reset)
{
    uint32_t new_width{ 560 };
    uint32_t new_height{ 420 };
    size_t new_size = size_t{ 2 } * new_width * new_height;

    wre::water_ripple water_surface{ 1920, 1080, 5 };
    water_surface.reset(new_width, new_height, true);

    ASSERT_EQ(water_surface.width(), new_width);
    ASSERT_EQ(water_surface.height(), new_height);
    ASSERT_EQ(water_surface.size(), new_size);
    ASSERT_EQ(water_surface.size_bytes(), new_size * sizeof(wre::wave_map::value_type));
}

TEST(wre, render)
{
    constexpr size_t kImageWidth{ 256 };
    constexpr size_t kImageHeight{ 256 };

    uint32_t img_src[kImageWidth * kImageHeight] = {};
    uint32_t img_dest[kImageWidth * kImageHeight] = {};

    wre::water_ripple water_surface{ kImageWidth, kImageHeight, 5 };
    
    water_surface.render(img_src, img_dest);
    water_surface.make_ripple(50, 50, 5, 50);
    water_surface.render(img_src, img_dest);
}

TEST(wre, ctor_copy)
{
    wre::water_ripple water_surface{ 1920, 1080, 5 };
    wre::water_ripple ws_copy{ water_surface };
    ASSERT_TRUE(wre_cmp(water_surface, ws_copy));
}

TEST(wre, copy_attr)
{
    wre::water_ripple water_surface{ 1920, 1080, 5 };
    wre::water_ripple ws_copy{ 560, 720, 4 };
    ws_copy = water_surface;
    
    ASSERT_TRUE(wre_cmp(water_surface, ws_copy));
}

TEST(wre, ctor_move)
{
    wre::water_ripple surface{ wre::water_ripple{ 1920, 1080, 5 } };
    ASSERT_EQ(surface.width(), 1920);
    ASSERT_EQ(surface.height(), 1080);
    ASSERT_EQ(surface.damping(), 5);
}

TEST(wre, move_attr)
{
    wre::water_ripple surface{ 1920, 1080, 5 };
    wre::water_ripple new_surface = std::move(surface);
    ASSERT_EQ(new_surface.width(), 1920);
    ASSERT_EQ(new_surface.height(), 1080);
    ASSERT_EQ(new_surface.damping(), 5);
}