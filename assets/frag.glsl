#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec4 pos;

uniform sampler2D ourTexture;
uniform vec3 cam_pos;

void main() {
	FragColor = texture(ourTexture, TexCoord);
//	FragColor = vec4(vec3(distance(cam_pos, pos.xyz)) / 200.f, 1);
}
