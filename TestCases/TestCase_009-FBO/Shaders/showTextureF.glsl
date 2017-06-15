#version 450

in vec2 _TexCoord;

uniform sampler2D uColorSampler;

layout(location = 0) out vec4 FragColor_;

void main()
{
	FragColor_ = texture2D(uColorSampler, _TexCoord);
}