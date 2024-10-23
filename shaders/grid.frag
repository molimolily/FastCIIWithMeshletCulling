#version 330 core

in vec2 TexCoord;

out vec4 FragColor;

uniform mat4 vp;

uniform vec3 pos;
uniform vec3 front;
uniform vec3 up;
uniform vec3 right;
uniform vec4 rect; // left, right, bottom, top
uniform float near;
uniform float far;

uniform float lineWidth;
uniform float pitch;

void main()
{
	vec2 xy = vec2((rect.y - rect.x) * TexCoord.x + rect.x, (rect.w - rect.z) * TexCoord.y + rect.z);
	vec3 rayDir = normalize(xy.x * right + xy.y * up + near * front);
	if(rayDir.y * pos.y < 0.0f)
	{
		float t = -pos.y / rayDir.y;
		vec3 hitPos = pos + t * rayDir;
		vec2 grid = mod(hitPos.xz, lineWidth + pitch);
		if((grid.x < lineWidth || grid.y < lineWidth) && (t < far))
		{
			FragColor = vec4(1.0f);
			vec4 clipPos = vp * vec4(hitPos, 1.0f);
			float ndcDepth = clipPos.z / clipPos.w;
			gl_FragDepth = ndcDepth * 0.5f + 0.5f;
		}
		else
		{
			discard;
		}
	}
	else
	{
		discard;
	}
}