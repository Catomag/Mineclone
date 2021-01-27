#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec4 pos;

uniform sampler2D ourTexture;
uniform vec3 cam_pos;
uniform float test;

void main() {
	FragColor = vec4(clamp(1.f / (distance(cam_pos, pos.xyz)) * 20.f, 0.f, 1.6f) * (texture(ourTexture, TexCoord).xyz), 1);
}
