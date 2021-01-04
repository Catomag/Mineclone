#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
uniform sampler2D tex;

void main() {
	//FragColor = texture(tex, TexCoord);
	FragColor = vec4(TexCoord.xy, 0, 1);
}
