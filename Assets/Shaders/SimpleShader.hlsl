Texture2D Texture: register(t0);
sampler TextureSampler: register(s0);

struct light
{
	float radius;
	float4 lightColor;
	float4 lightPosition;
};

cbuffer constant: register(b0)
{
	row_major float4x4 m_world;
	row_major float4x4 m_view;
	row_major float4x4 m_proj;
	light m_lights[32];
	float4 m_camera_position;
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

	return output;
}

float4 psmain(PS_INPUT input) : SV_TARGET
{
	float4 maintexture = Texture.Sample(TextureSampler, input.texcoord);
	return float4(maintexture.rgb, 0.0f);
}
