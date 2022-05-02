#version 460 core

layout(location = 0) in vec3 _VertexPosition_;
layout(location = 1) in vec3 _VertexColor_;
layout(location = 2) in vec2 _UV_;

out VS_OUT
{

	vec3 Color;
	vec2 UV;

} Output;

void main()
{
	Output.Color	= _VertexColor_;
	Output.UV		= _UV_;

	gl_Position		= vec4(_VertexPosition_, 1.0f);
}