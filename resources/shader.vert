#version 330 core

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_color;

out vec3 v_color;
out vec3 v_position;

uniform float u_movementX;
uniform float u_movementY;

void main() {
	gl_Position = vec4(in_position.x + u_movementX, in_position.y + u_movementY, in_position.z, 1.0);

	v_color = in_color;
	v_position = in_position;
}