#pragma once


/**
 * The base class that textures inherit from.
 * It supplies the bind functions, you should not use this since 
 * the texture won't be filled with data
 *
 * texType is the OpenGL texture enum type
 */
template <int texType>
class TextureBase
{
public:
    TextureBase() { glGenTextures(1, &m_ID); }
    ~TextureBase() { glDeleteTextures(1, &m_ID); }

    /**
     * @param textureType the enum in opengl that defines the texture type
     */
    inline void Bind() const noexcept { glBindTexture(texType, m_ID); }
    inline void Unbind() const noexcept { glBindTexture(texType, 0); }

private:
    unsigned m_ID;
};

/**
 * Creates a texture object for one texture.
 * Assumes that textures are square.
 */
class Texture : public TextureBase<GL_TEXTURE_2D>
{
public:
    Texture(const char* path);
};

/**
 * Creates a texture array object.
 * Assumes that textures are square.
 */
class TextureAtlas : public TextureBase<GL_TEXTURE_2D_ARRAY>
{
public:
    TextureAtlas(const char* path, unsigned x, unsigned y);
};

class TextureCubemap : public TextureBase<GL_TEXTURE_CUBE_MAP>
{
public:
    /**
     * @param dir the directory in which the images are located.
     * @param extension the extension of the image files.
     */
    TextureCubemap(const char* dir, const char* extension);
};