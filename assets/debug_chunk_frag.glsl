#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec4 pos;

void main() {
	float val;
	val += float(TexCoord.x > .99f) + float(TexCoord.x < .01f);
	val += float(TexCoord.y > .99f) + float(TexCoord.y < .01f);

	if(val == 0)
		discard;

	FragColor = vec4(1.f, 0.f, 1.f, 1.f);
}
