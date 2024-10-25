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
    // value を 0-360 度の範囲に正規化 (周期的に繰り返す)
    float hue = mod(float(value),period);
    // 色相を 0-1 の範囲に変換
    float h = hue / period;
    // 彩度と明度を固定値に設定
    float s = 1.0;
    float v = 1.0;
    // HSV から RGB に変換
    return hsv2rgb(vec3(h, s, v));
}

void main()
{
	// vec3 color = Normal;
    vec3 color = intToColor(drawID, 10.0f);
    FragColor = vec4(color, 1.0f);
}
