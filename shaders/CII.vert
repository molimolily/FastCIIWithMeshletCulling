#version 460
#extension GL_ARB_shader_viewport_layer_array: require
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 Normal;
out vec2 TexCoords;
flat out int drawID;

layout (std140, binding = 0) uniform VPBuffer
{
	mat4 vp[16];
};

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	Normal = (aNormal + 1.0f) / 2.0f;
	TexCoords = aTexCoords;
	int instanceID = gl_InstanceID;
	gl_Position = vp[instanceID] * model * vec4(aPos, 1.0);
	gl_ViewportIndex = instanceID;
	drawID = gl_DrawID;
}
