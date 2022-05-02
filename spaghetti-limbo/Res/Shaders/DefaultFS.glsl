#version 460 core
out vec4 Output;

in VS_OUT
{

	vec3 Color;
	vec2 UV;

} Input;

uniform sampler2D Diffuse;

void main()
{
	Output = texture(Diffuse, Input.UV);
	Output.rgb *= Input.Color;
}