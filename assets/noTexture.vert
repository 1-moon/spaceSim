#version 430

layout( location = 0 ) uniform mat4 uProjCameraWorld;
layout( location = 50) uniform mat4 uAnimationMatrix;
layout( location = 1 ) uniform mat3 uNormalMatrix;
layout( location = 88 ) uniform vec3 offsets[300];
layout( location = 6 ) uniform vec3 uPointDirs[3];

layout( location = 0 ) in vec3 iPosition;
layout( location = 1 ) in vec3 iColor;
layout( location = 2 ) in vec3 iNormal;

out vec3 v2fPosition;
out vec3 v2fColor;
out vec3 v2fNormal;

void main()
{
    v2fColor = iColor;
    v2fNormal = normalize(uNormalMatrix * iNormal);

    gl_Position = uProjCameraWorld * uAnimationMatrix * vec4( iPosition + offsets[gl_InstanceID], 1.0 );
    v2fPosition = iPosition + offsets[gl_InstanceID];
}
