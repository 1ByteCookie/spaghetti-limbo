#version 460 core

layout(location = 0) in vec3 _VertexPosition_;
layout(location = 1) in vec3 _Normal_;
layout(location = 2) in vec2 _UV_;

out VS_OUT
{

	vec3 Normal;
	vec2 UV;
	vec3 FragmentPosition;
	vec4 FragmentLightSpace;

} Output;

uniform mat4 Projection;
uniform mat4 View;
uniform mat4 Model;
uniform mat4 LightSpace;

void main()
{
	Output.Normal				= _Normal_;
	Output.UV					= _UV_;
	Output.FragmentPosition		= vec3(Model * vec4(_VertexPosition_, 1.0f));
	Output.FragmentLightSpace	= LightSpace * vec4(Output.FragmentPosition, 1.0f);

	gl_Position		= Projection * View * vec4(Output.FragmentPosition, 1.0f);
}