#version 460 core
out vec4 Output;

in VS_OUT
{

	vec3 Color;
	vec2 UV;

} Input;


void main()
{
	Output = vec4(Input.Color, 1.0f);
}