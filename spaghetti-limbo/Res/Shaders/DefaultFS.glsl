#version 460 core
out vec4 Output;

in VS_OUT
{

	vec3 Normal;
	vec2 UV;
	vec3 FragmentPosition;

} Input;

uniform vec3 CamPosition;
uniform vec3 Color;
uniform vec3 DirectionalLight;

void main()
{
	vec3 LightDirection		= normalize(DirectionalLight);
	vec3 Reflection			= reflect(DirectionalLight, Input.Normal);
	vec3 ViewDirection		= normalize(Input.FragmentPosition - CamPosition);
	
	float DiffuseStrength		= max(dot(DirectionalLight, Input.Normal), 0.0f);
	float SpecularHightlight	= pow(max(dot(ViewDirection, Reflection), 0.0f), 16.0f);
	float SpecularStrength		= 0.5f;

	vec3 Ambient	= Color * 0.1f;
	vec3 Diffuse	= Color * DiffuseStrength;
	vec3 Specular	= Color * SpecularHightlight * SpecularStrength;

	Output = vec4(Ambient + Diffuse + Specular, 1.0f);
}