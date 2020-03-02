#version 330 core
layout (location = 0) in vec3 aVertex;
layout (location = 1) in vec2 aUV;
layout (location = 2) in vec3 aPos;
layout (location = 3) in vec2 aVertexUV;
out vec2 uv;

uniform mat4 aVP;
uniform vec2 aAtlasSize;

void main()
{
    uv = aUV + (aVertexUV / aAtlasSize);
    gl_Position = aVP * vec4(aVertex + aPos ,1.0f);
}