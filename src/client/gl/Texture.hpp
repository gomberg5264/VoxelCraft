#pragma once

/**
 * Creates a texture object for one texture.
 * Assumes that textures are square.
 */
class Texture
{
public:
    Texture(const char* path);
    ~Texture();

    inline void Bind() const noexcept { glBindTexture(GL_TEXTURE_2D, m_ID); } 
    inline void Unbind() const noexcept { glBindTexture(GL_TEXTURE_2D, m_ID); }

private:
    unsigned m_ID;
};

/**
 * Creates a texture array object.
 * Assumes that textures are square.
 * Could inherit from texture but I don't see the benefit in it
 */
class TextureAtlas
{
public:
    TextureAtlas(const char* path, unsigned x, unsigned y);
    ~TextureAtlas();

    inline void Bind() const noexcept { glBindTexture(GL_TEXTURE_2D_ARRAY, m_ID); }
    inline void Unbind() const noexcept { glBindTexture(GL_TEXTURE_2D_ARRAY, m_ID); }

private:
    unsigned m_ID;
};