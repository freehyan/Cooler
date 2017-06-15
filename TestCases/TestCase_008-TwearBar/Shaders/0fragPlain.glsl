#version 450 core

layout (location = 0) out vec4 FragColor_;

in vec4 g_Colors;

void main()
{
	FragColor_ = g_Colors;
}