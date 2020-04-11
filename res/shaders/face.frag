#version 450 core

in vec3 normal;
in vec2 uv; 
in flat float texIndex; 

in vec3 vertPos;
in vec3 skyLightDir; 
in vec3 skyLightColor; 

out vec4 color;

uniform sampler2DArray sampl;

void main()
{
    float lamb = max(dot(-skyLightDir, normal), 0.0);
    
    vec3 ambient = vec3(0.1);
    vec3 skyColor = skyLightColor * lamb;
    
    if(lamb == 0.0)
    {
        skyColor = ambient;
    }

    color = vec4(skyColor,1) * texture(sampl,vec3(uv,texIndex));
    
    //color = vec4(1);
    // color = vec4(vec3(float(texIndex) / 2.f),1);
    //color = vec4(tex / 5.f,1);
}