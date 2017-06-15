#version 450 core

layout (location = 0) in vec3 _VertexPosition;
layout (location = 2) in vec2 _VertexTexCoords;

uniform mat4 uModelMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uProjectionMatrix;

out vec2 g_TexCoords;

void main()
{
	gl_Position = uProjectionMatrix * uViewMatrix * uModelMatrix * vec4(_VertexPosition, 1.0);
	g_TexCoords = vec2(_VertexTexCoords.x, 1.0f - _VertexTexCoords.y);
}