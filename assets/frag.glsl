#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec4 pos;

uniform sampler2D ourTexture;
uniform vec3 cam_pos;

void main() {
	FragColor = vec4((distance(cam_pos, pos.xyz)) / 100.f * texture(ourTexture, TexCoord).xyz, 1);
}
