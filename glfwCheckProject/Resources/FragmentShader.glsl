#version 330 core

out vec4 outputColor;

in vec3 ourColor;
in vec2 texCoord;

uniform sampler2D ourTexture;

void main()
{
    // outputColor = vec4(ourColor, 1.0);
	outputColor = texture(ourTexture, texCoord);
}