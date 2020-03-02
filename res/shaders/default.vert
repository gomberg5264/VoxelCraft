#version 330 core
layout (location = 0) in float aVertexDir;
layout (location = 1) in vec3 aVertexPos;
layout (location = 2) in vec2 aVertexUV;

layout (location = 3) in vec3 aPosOffset;
layout (location = 4) in vec4 aUVOffset0;
layout (location = 5) in vec4 aUVOffset1;
layout (location = 6) in vec4 aUVOffset2;

out vec2 uv;

uniform mat4 aVP;
uniform vec2 aAtlasSize;

void main()
{
    /*
        Dir is as follows
        0,0 - Back
        0,2 - Front
        1,0 - Left
        1,2 - Right
        2,0 - Bottom
        2,2 - Top
    */
    mat3x4 uvOffset;
    uvOffset[0] = aUVOffset0;
    uvOffset[1] = aUVOffset1;
    uvOffset[2] = aUVOffset2;

    int column = int(aVertexDir) / 2; 
    int row = (int(aVertexDir) % 2) * 2;
    vec2 uvIndex = vec2(
       uvOffset[column][row],
       uvOffset[column][row + 1]);

    uv = uvIndex + (aVertexUV / aAtlasSize);
    gl_Position = aVP * vec4(aVertexPos + aPosOffset ,1.0f);
}