#version 330 core

layout (location = 1) in vec3 position;
layout (location = 0) in vec3 color; 

uniform float moveVariable;

out vec3 ourColor;

void main()
{
    gl_Position = vec4(position.x + moveVariable, position.y, position.z, 1.0);
	ourColor = gl_Position.xyz;
}