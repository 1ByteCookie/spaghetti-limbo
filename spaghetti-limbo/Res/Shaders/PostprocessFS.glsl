#version 460 core
out vec4 Output;

in VS_OUT
{

	vec2 UV;

} Input;

uniform sampler2D Scene;

void main()
{
	Output = texture(Scene, Input.UV);
	Output.rgb = pow( Output.rgb, vec3(1/2.2) );

	//float DepthValue = texture(Scene, Input.UV).r;
	//Output = vec4(vec3(DepthValue), 1.0f);
}