#version 450 core

layout (location = 0) in vec3 _VertexPostion;

void main()
{
	gl_Position = vec4(_VertexPostion, 1.0);
}