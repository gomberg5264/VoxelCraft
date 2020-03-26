#pragma once

namespace Primitive
{
    namespace Face
    {
        struct Vertex
        {
            Vertex(float x, float y, float z, float u, float v, unsigned texIndex)
                : x(x)
                , y(y)
                , z(z)
                , u(u)
                , v(v)
                , texIndex(static_cast<float>(texIndex)) // For some reason opengl doen't work if this is unsigned
            {}


            float x;
            float y;
            float z;
        
            float u;
            float v;

            float texIndex;
        };

        struct Buffer
        {
            std::vector<Primitive::Face::Vertex> vertices;
        };

        Buffer MakeBuffer(BlockFace dir, float x, float y, float z, unsigned texIndex);
    }
}