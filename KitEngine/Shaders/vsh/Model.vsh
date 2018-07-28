cbuffer VSMatrix : register(b0)
{
	float4x4 World :			packoffset(c0);
	float4x4 View  :			packoffset(c4);
	float4x4 Projection :		packoffset(c8);
};

struct VSInput
{
	float3 Position :		SV_Position;
	float3 Normal :			NORMAL;
	float2 Texture :		TEXCOORD0;
};

struct VSOutput
{
	float4 Position :		SV_POSITION;
	float3 Normal :			NORMAL;
	float2 Texture :		TEXCOORD;
};

VSOutput vs_main ( VSInput input )
{
	VSOutput output = (VSOutput)0;
	output.Position = mul(float4(input.Position, 1.0f), World);
	output.Position = mul(output.Position, View);
	output.Position = mul(output.Position, Projection);

	output.Normal = normalize(mul(input.Normal, (float3x3)World));

	output.Texture = input.Texture;

	return output;
}
