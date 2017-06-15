#version 450 core

layout (location = 0) in vec3 _VertexPosition;
layout (location = 2) in vec2 _VertexTexCoords;

uniform mat4 uModelMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uProjectionMatrix;

out vec4 g_Colors;

void main()
{
	gl_Position = uProjectionMatrix * uViewMatrix * uModelMatrix * vec4(_VertexPosition, 1.0);
    g_Colors = vec4(_VertexPosition.x * 0.5 + 0.5, _VertexPosition.y * 0.5 + 0.5, 0.0, 1.0);
}