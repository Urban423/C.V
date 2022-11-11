Texture2D Texture: register(t0);
sampler TextureSampler: register(s0);

Texture2D UV: register(t1);
sampler UVSampler: register(s1);

struct PS_INPUT
{
	float4 position: SV_POSITION;
	float2 texcoord: TEXCOORD0;
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
};

VS_OUTPUT vsmain(VS_INPUT input)
{

	VS_OUTPUT output = (VS_OUTPUT)0;

	output.position = input.position;
	output.texcoord = input.texcoord;

	return output;
}

float4 psmain(PS_INPUT input) : SV_TARGET
{
	float4 maintexture = Texture.Sample(TextureSampler, input.texcoord);
	float4 uvtexture = UV.Sample(UVSampler, input.texcoord);

	float4 final = lerp(maintexture, uvtexture, uvtexture.a);

	return final;
}
