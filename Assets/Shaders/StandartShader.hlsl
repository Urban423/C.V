Texture2D Texture: register(t0);
sampler TextureSampler: register(s0);

cbuffer constant: register(b0)
{
	row_major float4x4 m_world;
	row_major float4x4 m_view;
	row_major float4x4 m_proj;
	float4 m_light_direction;
	float4 m_camera_position;
	float4 m_light_position;
	float m_light_radius;
};

struct PS_INPUT
{
	float4 position: SV_POSITION;
	float2 texcoord: TEXCOORD0;
	float3 normal: NORMAL0;
	float3 world_pos: TEXCOORD1;
};

struct VS_INPUT
{
	float4 position: POSITION0;
	float2 texcoord: TEXCOORD0;
	float3 normal: NORMAL0;
};

struct VS_OUTPUT
{
	float4 position: SV_POSITION;
	float2 texcoord: TEXCOORD0;
	float3 normal: NORMAL0;
	float3 world_pos: TEXCOORD1;
};

VS_OUTPUT vsmain(VS_INPUT input)
{

	VS_OUTPUT output = (VS_OUTPUT)0;

	output.position = mul(input.position, m_world);
	output.world_pos = output.position.xyz;

	output.position = mul(output.position, m_view);
	output.position = mul(output.position, m_proj);

	output.texcoord = input.texcoord;
	output.normal = normalize(mul(input.normal, m_world));
	return output;
}

float4 psmain(PS_INPUT input) : SV_TARGET
{
	float4 maintexture = Texture.Sample(TextureSampler, input.texcoord);

	float ka = 0.3;
	float3 ia = float3(1.0, 1.0, 1.0);
	ia *= maintexture.rgb;
	float3 ambient_light = ka * ia;



	float kd = 0.7;
	float3 light_dir = normalize(m_light_position.xyz - input.world_pos.xyz);
	float distance_light_object = length(m_light_position.xyz - input.world_pos.xyz);

	float fade_area = max(0, distance_light_object - m_light_radius);

	float constant_func = 1.0;
	float linear_func = 1.0;
	float quadratic_func = 22;

	float attenuation = constant_func + linear_func * fade_area + quadratic_func * fade_area * fade_area;

	float amount_diffuse_light = max(0, dot(light_dir.xyz, input.normal));
	float3 id = float3(1, 1, 1);
	id *= (maintexture.rgb);

	float3 diffuse_light = (kd * id * amount_diffuse_light) / attenuation;



	float ks = 1;
	float3 direction_to_camera = normalize(input.world_pos - m_camera_position.xyz);
	float3 reflected_light = reflect(light_dir, input.normal);
	float shininess = 77;
	float amount_specular_light = pow(max(0, dot(reflected_light, direction_to_camera)), shininess);

	float3 specular_light = (ks * amount_specular_light * maintexture.rgb) / attenuation;

	float3 final_light = ambient_light + diffuse_light + specular_light;

	return float4(final_light, 1.0);
}
