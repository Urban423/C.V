Texture2D Texture: register(t0);
sampler TextureSampler: register(s0);

cbuffer constant: register(b0)
{
	row_major float4x4 m_world;
	row_major float4x4 m_view;
	row_major float4x4 m_proj;
	float4 m_camera_position;
	float2 pos;
	float2 letter;
};

struct PS_INPUT
{
	float4 position: SV_POSITION;
	float2 texcoord: TEXCOORD0;
	float3 world_pos: TEXCOORD1;
};

struct VS_INPUT
{
	float4 position: POSITION0;
	float2 texcoord: TEXCOORD0;
};

struct VS_OUTPUT
{
	float4 position: SV_POSITION;
	float2 texcoord: TEXCOORD0;
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
	input.texcoord.x *= 0.14;
	input.texcoord.y *= 0.08;
	input.texcoord.x += 0.05;
	input.texcoord.y += 0.064;

	input.texcoord.x += 0.155 * letter.x;
	input.texcoord.y += 0.164 * letter.y;
	float4 maintexture = Texture.Sample(TextureSampler, input.texcoord);

	return 1 - maintexture;
}