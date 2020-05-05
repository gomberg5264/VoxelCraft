#type vertex
#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUV;
layout (location = 3) in float aTexIndex;

out vec3 vNormal;
out vec2 vUV;
out flat float vTexIndex; 

out vec3 vVertPos;
out float vSkyIntensity;
out float vSkyDiffuseIntensity;
out vec3 vSkyLightDir;
out vec3 vSkyLightColor;

uniform mat4 aVP;
uniform mat4 aTransform;

uniform float aSkyIntensity;
uniform float aSkyDiffuseIntensity;
uniform vec3 aSkyLightDir;
uniform vec3 aSkyLightColor;

void main()
{
    vNormal = aNormal;
    vUV = aUV;
    vTexIndex = aTexIndex;
 
    vVertPos = aPos;
    vSkyIntensity = aSkyIntensity;
    vSkyDiffuseIntensity = aSkyDiffuseIntensity;
    vSkyLightDir = aSkyLightDir;
    vSkyLightColor = aSkyLightColor;

    gl_Position = aVP * aTransform * vec4(aPos ,1.0f);
}

#type fragment
#version 450 core

in vec3 vNormal;
in vec2 vUV; 
in flat float vTexIndex; 

in vec3 vVertPos;
in float vSkyIntensity;
in float vSkyDiffuseIntensity;
in vec3 vSkyLightDir; 
in vec3 vSkyLightColor; 

out vec4 vColor;

//uniform sampler2DArray sampl;
uniform sampler2D sampl;

void main()
{
    vec3 ambient = vSkyIntensity * vSkyLightColor;
    vec3 toSky = vSkyLightDir * -1.f;
    
    float diff = max(dot(toSky, vNormal), 0.0);
    vec3 diffuse = vSkyLightColor * diff * vSkyDiffuseIntensity;

    vec3 result = min(ambient + diffuse,vec3(1));
    //vColor = vec4(result,1) * texture(sampl,vec3(uv,texIndex));    
    //vColor = vec4(result,1) * texture(sampl, vUV);
    vColor = texture(sampl, vUV);
}