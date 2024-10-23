#version 460

layout (location = 0) in vec3 aPos;

uniform mat4 vp;

void main()
{
    gl_Position = vp * vec4(aPos, 1.0);
}
