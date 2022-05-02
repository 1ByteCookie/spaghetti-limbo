#version 460 core

layout(location = 0) in vec3 _VertexPosition_;

void main()
{
	gl_Position = vec4(_VertexPosition_, 1.0f);
}