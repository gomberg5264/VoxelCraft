#version 450 core

in vec3 normal;
in vec2 uv; 
in flat float texIndex; 

in vec3 vertPos;
in float skyIntensity;
in float skyDiffuseIntensity;
in vec3 skyLightDir; 
in vec3 skyLightColor; 

out vec4 color;

uniform sampler2DArray sampl;

void main()
{
    vec3 ambient = skyIntensity * skyLightColor;
    vec3 toSky = skyLightDir * -1.f;
    
    float diff = max(dot(toSky, normal), 0.0);
    vec3 diffuse = skyLightColor * diff * skyDiffuseIntensity;

    vec3 result = min(ambient + diffuse,vec3(1));
    color = vec4(result,1) * texture(sampl,vec3(uv,texIndex));
    
    //color = vec4(diffuse,1);

    //color = vec4(vec3(skyIntensity),1);
    // color = vec4(vec3(float(texIndex) / 2.f),1);
    //color = vec4(tex / 5.f,1);
}