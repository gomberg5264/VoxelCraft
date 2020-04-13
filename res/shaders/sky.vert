#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal; // This will not be used
layout (location = 2) in vec2 aUV;
layout (location = 3) in float aTexIndex;

out vec2 uv;
out flat float texIndex; 

uniform mat4 aRotProj;

void main()
{
    uv = aUV;
    texIndex = aTexIndex;

    gl_Position = aRotProj * vec4(aPos ,1.0f);
    
    //gl_Position = vec4(aPos.xy,-1.0f ,1.0f);
}