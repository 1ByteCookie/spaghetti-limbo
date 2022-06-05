#version 460 core
out vec4 Output;

in VS_OUT
{

	vec3 Normal;
	vec2 UV;
	vec3 FragmentPosition;
	vec4 FragmentLightSpace;

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
uniform sampler2D	shadowMap;

float CalculateShadow(vec4 LightSpaceFragment)
{
    // perform perspective divide
    vec3 projCoords = LightSpaceFragment.xyz / LightSpaceFragment.w;
    

	// transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    

	// get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    

	// get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    
	vec3 SurfaceNormal		= normalize(Input.Normal);
	vec3 LightDirection		= normalize(Light.Position - Input.FragmentPosition);
	float bias				= max(0.05f * ( 1.0f - dot(SurfaceNormal, LightDirection) ), 0.005f);

	// check whether current frag pos is in shadow
    float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;

    return shadow;
}

void main()
{
	vec3 SurfaceNormal			= normalize(Input.Normal);
	
	vec3 LightDirection			= normalize(Light.Position - Input.FragmentPosition);
	vec3 ViewDirection			= normalize(CamPosition - Input.FragmentPosition);
	vec3 Halfway				= normalize(LightDirection + ViewDirection);


	float DiffuseStrength		= max(dot(LightDirection, SurfaceNormal), 0.0f);
	float SpecularHightlight	= pow(max(dot(SurfaceNormal, Halfway), 0.0f), Material.Luster);


	vec3 Ambient	= Light.Ambient * 0.1f;
	vec3 Diffuse	= Light.Diffuse * DiffuseStrength;
	vec3 Specular	= Light.Specular * SpecularHightlight;
	
	
	float Shadow	= CalculateShadow(Input.FragmentLightSpace);
	vec3 Color		= (Ambient + (1.0 - Shadow) * (Diffuse + Specular)) * Material.Diffuse;

	Output = vec4(Color, 1.0f);
}