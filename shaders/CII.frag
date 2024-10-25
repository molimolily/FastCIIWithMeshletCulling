#version 450

in vec3 Normal;
in vec2 TexCoords;
flat in int drawID;
out vec4 FragColor;

vec3 hsv2rgb(vec3 c) {
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

vec3 intToColor(int value, float period) {
    // value �� 0-360 �x�͈̔͂ɐ��K�� (�����I�ɌJ��Ԃ�)
    float hue = mod(float(value),period);
    // �F���� 0-1 �͈̔͂ɕϊ�
    float h = hue / period;
    // �ʓx�Ɩ��x���Œ�l�ɐݒ�
    float s = 1.0;
    float v = 1.0;
    // HSV ���� RGB �ɕϊ�
    return hsv2rgb(vec3(h, s, v));
}

void main()
{
	// vec3 color = Normal;
    vec3 color = intToColor(drawID, 10.0f);
    FragColor = vec4(color, 1.0f);
}
