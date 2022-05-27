#version 460 core

layout(location = 0) in vec3 _VertexPosition_;

uniform mat4 LightSpace;
uniform mat4 Model;

void main()
{
	gl_Position = LightSpace * Model * vec4(_VertexPosition_, 1.0f);
}