Texture2D Texture: register(t0);
sampler TextureSampler: register(s0);

Texture2D Normal: register(t1);
sampler NormalSampler: register(s1); 

Texture2D Texture2: register(t2);
sampler TextureSampler2: register(s2);

struct light
{
	float4 lightColor;
	float4 lightPosition;
};

cbuffer constant: register(b0)
{
	row_major float4x4 m_world;
	row_major float4x4 m_view;
	row_major float4x4 m_proj;
	float4 m_camera_position;
	light m_lights[32];
	float _Time;
};

struct PS_INPUT
{
	float4 position: SV_POSITION;
	float2 texcoord: TEXCOORD0;
	float3 world_pos: TEXCOORD1;
	row_major float3x3 tbn: TEXCOOR2;
};

struct VS_INPUT
{
	float4 position: POSITION0;
	float2 texcoord: TEXCOORD0;
	float3 normal: NORMAL0;
	float3 tangent: TANGENT0;
	float3 binormal: BINORMAL0;
};

struct VS_OUTPUT
{
	float4 position: SV_POSITION;
	float2 texcoord: TEXCOORD0;
	float3 world_pos: TEXCOORD1;
	row_major float3x3 tbn: TEXCOOR2;
};

VS_OUTPUT vsmain(VS_INPUT input)
{

	VS_OUTPUT output = (VS_OUTPUT)0;

	output.position = mul(input.position, m_world);
	output.world_pos = output.position.xyz;

	output.position = mul(output.position, m_view);
	output.position = mul(output.position, m_proj);

	output.texcoord = input.texcoord;

	output.tbn[0] = normalize(mul(input.tangent, m_world));
	output.tbn[1] = normalize(mul(input.binormal, m_world));
	output.tbn[2] = normalize(mul(input.normal, m_world));

	return output;
}

float3 LightAnalizer(float4 MainTexture, float4 NormalTexture, float3x3 tbn, float3 world_pos)
{
	float ka = 0.13;
	float3 ia = float3(1, 1, 1);
	ia *= MainTexture.rgb;
	float3 ambient_light = ka * ia;

	float3 final_light = ambient_light;

	float kd = 0.9;

	float constant_func = 1.0;
	float linear_func = 1.0;
	float quadratic_func = 22;

	float ks = 2;

	for (int i = 0; i < 32; i++)
	{
		if (m_lights[i].lightPosition.w == 0)
		{
			continue;
		}

		float dot_nl = dot(m_lights[i].lightPosition.xyz, tbn[2]);
		float3 light_dir = normalize(m_lights[i].lightPosition.xyz - world_pos);
		float distance_light_object = length(m_lights[i].lightPosition.xyz - world_pos);
		float fade_area = max(0, distance_light_object - m_lights[i].lightColor.w);

		float attenuation = constant_func + linear_func * fade_area + quadratic_func * fade_area * fade_area;
		float amount_diffuse_light = max(0, dot(light_dir.xyz, NormalTexture));
		float3 id = m_lights[i].lightColor.xyz;
		id *= MainTexture.rgb;
		float3 diffuse_light = (kd * id * amount_diffuse_light) / attenuation;

		float3 direction_to_camera = normalize(world_pos - m_camera_position.xyz);
		float3 reflected_light = reflect(light_dir, NormalTexture);
		float shininess = 44;
		float amount_specular_light = 0;
		if (dot_nl > 0)
		{
			amount_specular_light = pow(max(0, dot(reflected_light, direction_to_camera)), shininess);
		}
		float3 specular_light = (ks * amount_specular_light * MainTexture.rgb * m_lights[i].lightColor.xyz) / attenuation;

		final_light += diffuse_light + specular_light;
	}

	return final_light;
}

float4 psmain(PS_INPUT input) : SV_TARGET
{
	input.texcoord.xy *= 1;
	float4 maintexture = Texture.Sample(TextureSampler, input.texcoord);
	float4 normal = Normal.Sample(NormalSampler, input.texcoord);

	input.texcoord.x += _Time * 0.003f;
	maintexture += Texture2.Sample(TextureSampler2, input.texcoord);

	normal.xyz = (normal.xyz * 2) - 1;
	normal.xyz = mul(normal.xyz, input.tbn);

	float3 final = LightAnalizer(maintexture, normal, input.tbn, input.world_pos);

	return float4(final, 1);
}
