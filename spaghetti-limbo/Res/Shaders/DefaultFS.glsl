#version 460 core
out vec4 Output;

in VS_OUT
{

	vec3 Normal;
	vec2 UV;
	vec3 FragmentPosition;

} Input;

struct MATERIAL
{
	vec3	Diffuse;
	vec3	Specular;
	float	Luster;
};

struct LIGHT
{
	vec3 Position;
	vec3 Ambient;
	vec3 Diffuse;
	vec3 Specular;
};



uniform MATERIAL	Material;
uniform LIGHT		Light;
uniform vec3		CamPosition;

void main()
{
	vec3 SurfaceNormal			= normalize(Input.Normal);
	
	vec3 LightDirection			= normalize(Light.Position - Input.FragmentPosition);
	vec3 ViewDirection			= normalize(CamPosition - Input.FragmentPosition);
	vec3 Halfway				= normalize(LightDirection + ViewDirection);


	float DiffuseStrength		= max(dot(LightDirection, SurfaceNormal), 0.0f);
	float SpecularHightlight	= pow(max(dot(SurfaceNormal, Halfway), 0.0f), Material.Luster);



	vec3 Ambient	= Material.Diffuse * Light.Ambient;
	vec3 Diffuse	= Material.Diffuse * Light.Diffuse * DiffuseStrength;
	vec3 Specular	= Material.Specular * Light.Specular * SpecularHightlight;

	Output = vec4(Ambient + Diffuse + Specular, 1.0f);
}