#version 330 core
in vec2 vUV; 

out vec4 color;

void main()
{
    color = vec4(vUV,1,1);
}