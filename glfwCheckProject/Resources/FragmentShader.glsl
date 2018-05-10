#version 330 core

out vec4 outputColor;

// in vec3 ourColor;
in vec2 texCoord;

uniform float fFaceAlpha;

uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;

void main()
{
    // outputColor = vec4(ourColor, 1.0);
	// outputColor = texture(ourTexture1, texCoord);

	vec2 modifyTexCoord = 2 * texCoord;

	outputColor = mix(texture(ourTexture1, texCoord), texture(ourTexture2, modifyTexCoord), fFaceAlpha);
}