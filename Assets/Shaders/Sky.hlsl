Texture2D Texture: register(t0);
sampler TextureSampler: register(s0);

struct PS_INPUT
{
	float4 position: SV_POSITION;
	float2 texcoord: TEXCOORD0;
	float3 normal: TEXCOORD1;
	float3 direction_to_camera: TEXCOORD2;
};

float4 psmain(PS_INPUT input) : SV_TARGET
{ 
	return Texture.Sample(TextureSampler, input.texcoord);
}