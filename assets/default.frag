#version 430

in vec3 v2fPosition;
in vec3 v2fNormal;
in vec2 v2fTexCoord;

layout( location = 0 ) out vec4 oColor;

layout( location = 5 ) uniform vec3 uCameraPos;
layout( location = 2 ) uniform vec3 uLightDir;
layout( location = 3 ) uniform vec3 uLightDiffuse;
layout( location = 4 ) uniform vec3 uSceneAmbient;
layout( location = 6 ) uniform vec3 uPointDirs[3];
layout( location = 42) uniform vec3 uPointCols[3];
layout( location = 26) uniform vec3 uPointPos[3];
layout( location = 20) uniform vec3 uDiffuse;
layout( location = 23) uniform vec3 uSpecular;
layout( location = 24) uniform vec3 uEmission;
layout( location = 25) uniform float uShininess;

layout(binding = 0) uniform sampler2D uTexture;

void main()
{
    vec3 normal = normalize(v2fNormal);
    float nDoLL = max(0.0, dot( normal, uLightDir));
    vec3 color;

    color = (uSceneAmbient + nDoLL * uLightDiffuse) * texture( uTexture, v2fTexCoord ).rgb + uEmission;
    
    for(int p = 0; p < 3; p++){
        vec3 viewDir = normalize(uCameraPos - v2fPosition);
        vec3 lightDir = normalize(uPointPos[p] - v2fPosition);
        vec3 halfV = normalize(lightDir + viewDir);
        float attenuation = 1.0/(length(v2fPosition - uPointPos[p]) * 
        length(v2fPosition - uPointPos[p]) + 0.001);        

        color += max(0.0, dot( normal, lightDir)) * uPointCols[p] * uDiffuse * attenuation;
        color += pow(max(0.0, dot( normal, halfV)), uShininess) * uPointCols[p] * uSpecular * attenuation;
        color += uPointCols[p] * texture( uTexture, v2fTexCoord ).rgb * attenuation;
    }
    float alpha = 1.0;
    
    oColor = vec4(color, alpha);
}
