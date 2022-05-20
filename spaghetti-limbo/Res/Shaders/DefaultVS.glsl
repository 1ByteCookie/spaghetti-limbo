#version 460 core

layout(location = 0) in vec3 _VertexPosition_;
layout(location = 1) in vec3 _Normal_;
layout(location = 2) in vec2 _UV_;

out VS_OUT
{

	vec3 Color;
	vec2 UV;

} Output;

uniform mat4 Projection;
uniform mat4 View;
uniform mat4 Model;

void main()
{
	Output.Color	= _Normal_;
	Output.UV		= _UV_;

	gl_Position		= Projection * View * Model * vec4(_VertexPosition_, 1.0f);
}