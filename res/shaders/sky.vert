#version 450 core
layout (location = 0) in vec3 aPos;

out vec3 texPos; 

uniform mat4 aRotProj;

void main()
{
    texPos = aPos;
    
    vec4 pos = aRotProj * vec4(aPos ,1.0f);
    gl_Position = pos.xyww;
}