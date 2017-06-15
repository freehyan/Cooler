#version 450 core

layout (location = 0) out vec4 FragColor_;

in vec2 g_TexCoords;
uniform sampler2D uTexSampler;

void main()
{
	FragColor_ = texture2D(uTexSampler, g_TexCoords)
}