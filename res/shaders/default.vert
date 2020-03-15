#version 450 core
layout (location = 0) in float aDir;
layout (location = 1) in vec3 aPos;
layout (location = 2) in vec2 aUV;

/*
    Back,
    Front,
    Left,
    Right,
    Bottom,
    Top,
*/

layout (location = 3) in vec3 aPosOffset;
layout (location = 4) in vec3 aTex1;
layout (location = 5) in vec3 aTex2;

out vec2 uv;
out int texIndex; 

uniform mat4 aVP;

void main()
{
    int tex[6];
    tex[0] = int(aTex1.x);
    tex[1] = int(aTex1.y);
    tex[2] = int(aTex1.z);
    tex[3] = int(aTex2.x);
    tex[4] = int(aTex2.y);
    tex[5] = int(aTex2.z);

    uv = aUV;
    texIndex = tex[aDir];
    gl_Position = aVP * vec4(aPos + aPosOffset ,1.0f);
}