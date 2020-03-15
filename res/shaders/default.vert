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
out flat int texIndex; 

out vec3 tex;

uniform mat4 aVP;

void main()
{
    const int intDir = int(aDir);
    //if(intDir < 3)
    //    texIndex = int(aTex1[intDir]);
    //else   
    //    texIndex = int(aTex2[intDir - 3]);
    texIndex = 
        intDir == 0 ? int(aTex1[0]) :
        intDir == 1 ? int(aTex1[1]) :
        intDir == 2 ? int(aTex1[2]) :
        intDir == 3 ? int(aTex2[0]) :
        intDir == 4 ? int(aTex2[1]) :
        intDir == 5 ? int(aTex2[2]) : 0; // TODO add error texture

    //texIndex = int(aTex1.x);
    //texIndex = intDir;
    tex = aTex1;

    uv = aUV;
    gl_Position = aVP * vec4(aPos + aPosOffset ,1.0f);
}