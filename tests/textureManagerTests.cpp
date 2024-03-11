#include <gtest/gtest.h>

#include "../src/lib/Managers/TextureManager/TextureManager.h"

TEST(TextureManagerTest, LoadTextureCorrectly)
{
    TextureManager& tm = TextureManager::getInstance();
    EXPECT_NO_THROW(tm.loadTexture("correct_texture", ASSETS_PATH + std::string("player.png")));
}

TEST(TextureManagerTest, ThrowsWhenLoadingNonexistentTexture)
{
    TextureManager& tm = TextureManager::getInstance();
    EXPECT_THROW(tm.loadTexture("nonexistent_texture", ASSETS_PATH + std::string("wrong_player.png")), std::runtime_error);
}

TEST(TextureManagerTest, GetTextureCorrectly)
{
    TextureManager& tm = TextureManager::getInstance();
    tm.loadTexture("test_texture", ASSETS_PATH + std::string("player.png"));
    EXPECT_NO_THROW(tm.getTexture("test_texture"));
}