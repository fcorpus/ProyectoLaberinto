#version 330 core
layout (location = 0) in vec3 tPos;
layout (location = 1) in vec3 aColor;

out vec4 ourColor;

uniform mat4 modelo;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * modelo * vec4(tPos, 1.0);
	ourColor = vec4(aColor, 1.0f);
}