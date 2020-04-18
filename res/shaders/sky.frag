#version 450 core

out vec4 color;

in vec3 texPos;

uniform samplerCube sampl;

void main()
{
    color = texture(sampl,texPos);
}