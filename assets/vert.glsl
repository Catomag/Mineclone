#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;
out vec4 pos;

uniform vec3 cam_pos;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 transform;

void main() {

	TexCoord = aTexCoord;
	pos = transform * vec4(aPos, 1.f);

	float dist = distance(pos.xyz, vec3(cam_pos.x, cam_pos.y, cam_pos.z));

	//pos.y -= ((dist * dist) / 400.f);// - off;
	gl_Position = projection * view * pos;
}
