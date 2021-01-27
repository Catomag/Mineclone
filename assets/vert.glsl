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

//	float dist = pos.x * pos.x + pos.z * pos.z;
//	pos.y -= dist / 200;

	pos.y -= ((dist * dist) / 300.f);// - off;
	
	gl_Position = projection * view * pos;
}
