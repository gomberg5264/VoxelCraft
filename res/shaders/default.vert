#version 330 core
layout (location = 0) in vec3 aVertex;
layout (location = 1) in vec2 aUV;
layout (location = 2) in vec3 aPos;
out vec2 uv;

uniform mat4 aVP;

void main()
{
    uv = aUV;
    gl_Position = aVP * vec4(aVertex + aPos ,1.0f);
}