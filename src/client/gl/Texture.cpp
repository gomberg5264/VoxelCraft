#include "vcpch.hpp"
#include "Texture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Texture::Texture(const char* path)
{
    unsigned& tex = m_ID;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D_ARRAY, tex);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);

    unsigned char* data = stbi_load(path, &width, &height, &nrChannels, STBI_rgb_alpha);
    
    if (data)
    {
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RGBA,
            width,
            height,
            0,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

Texture::~Texture()
{
    glDeleteTextures(1, &m_ID);
}

TextureAtlas::TextureAtlas(const char* path, unsigned x, unsigned y)
{
    unsigned& tex = m_ID;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D_ARRAY, tex);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);

    // Since we put our textures in a grid like pattern, we have to convert the data
    // to be in a format that emulates all the textures in one columns
    unsigned char* data = stbi_load(path, &width, &height, &nrChannels, STBI_rgb_alpha);
    const unsigned texSize = width / x;

    if (data)
    {
        auto image{ std::make_unique<unsigned char[]>(width * height * nrChannels) };

        unsigned offset = 0;
        for (unsigned yI = 0; yI < height / texSize; yI++)
        {
            for (unsigned xI = 0; xI < width / texSize; xI++)
            {
                // Read 64 rows
                for (unsigned row = 0; row < texSize; row++)
                {
                    memcpy(
                        &image[offset],
                        &data
                        [(yI * width * texSize * nrChannels) +
                    (xI * texSize * nrChannels) +
                        row * width * nrChannels],
                        texSize * nrChannels);
                    offset += texSize * nrChannels;
                }
            }
        }

        const auto size = x * y;
        // Create the storage
        glTexStorage3D(GL_TEXTURE_2D_ARRAY, 4, GL_RGBA8, texSize, texSize, size);

        glTexSubImage3D(
            GL_TEXTURE_2D_ARRAY,
            0,
            0, 0, 0,
            texSize, texSize, size,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            image.get());

        glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

TextureAtlas::~TextureAtlas()
{
    glDeleteTextures(1, &m_ID);
}