#version 430

in vec3 v2fPosition;
in vec3 v2fNormal;
in vec2 v2fTexCoord;

layout( location = 0 ) out vec4 oColor;
layout( location = 5 ) uniform vec3 uCameraPos;
layout(binding = 0) uniform sampler2D uTexture;

void main()
{
    vec3 normal = normalize(v2fNormal);

    oColor = texture( uTexture, v2fTexCoord );
}