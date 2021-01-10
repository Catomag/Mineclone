#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;
out vec4 pos;
out float test;

uniform vec3 cam_pos;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 transform;

void main() {
	TexCoord = aTexCoord;
	pos = transform * vec4(aPos, 1.f);

	float positive = 0;
	float off = 0;
	positive += float(pos.y > -32);
	positive -= float(pos.y <= -32);

	off += 2048 * float(pos.y <= -32);

	float dist = distance(pos.xyz, vec3(cam_pos.x, cam_pos.y + float(pos.y <= -32) * -64, cam_pos.z));

	test = positive;

	pos.y -= positive * ((dist * dist) / 50.f) + off;
	pos.xz *= positive;
	gl_Position = projection * view * pos;
}
