#pragma once
#include "client/gl/Buffer.hpp"
#include "common/BlockData.hpp"

namespace Primitive
{
    namespace Face
    {
        struct Vertex
        {
            Vertex(float x, float y, float z, float nX, float nY, float nZ, float u, float v, unsigned texIndex)
                : x(x)
                , y(y)
                , z(z)
                , nX(nX)
                , nY(nY)
                , nZ(nZ)
                , u(u)
                , v(v)
                , texIndex(static_cast<float>(texIndex)) // For some reason opengl doen't work if this is unsigned
            {
            }


            float x;
            float y;
            float z;

            float nX;
            float nY;
            float nZ;
        
            float u;
            float v;

            float texIndex;
        };

        struct Buffer
        {
            std::vector<Primitive::Face::Vertex> vertices;
        };

        Buffer MakeBuffer(BlockFace dir, float x, float y, float z, unsigned texIndex);
        std::vector<unsigned> MakeIndices(unsigned faceCount);
        inline VBO MakeVBO() 
        {
            return VBO({ 
                    { 0, 3, GL_FLOAT, GL_FALSE, sizeof(Primitive::Face::Vertex), 0u * sizeof(float) },
                    { 1, 3, GL_FLOAT, GL_TRUE,  sizeof(Primitive::Face::Vertex), 3u * sizeof(float) },
                    { 2, 2, GL_FLOAT, GL_TRUE,  sizeof(Primitive::Face::Vertex), 6u * sizeof(float) },
                    { 3, 1, GL_FLOAT, GL_FALSE, sizeof(Primitive::Face::Vertex), 8u * sizeof(float) }});
        };
    }
}