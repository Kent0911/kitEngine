struct VS_IN
{
	float4 pos : SV_Position;
	float4 col : COLOR;
	float2 tex : TEXCOORD;
};

struct VS_OUT
{
	float4 pos : SV_Position;
	float4 col : COLOR;
	float2 tex : TEXCOORD;
};

cbuffer ConstantBuffer : register(b0)
{
	float4x4 World :		packoffset(c0);
	float4x4 View :			packoffset(c4);
	float4x4 Projection :	packoffset(c8);
};

VS_OUT vs_main( VS_IN input )
{
	VS_OUT output;
	output.pos = mul(input.pos, World);
	output.pos = mul(output.pos, View);
	output.pos = mul(output.pos, Projection);

	output.col = input.col;
	output.tex = input.tex;

	return output;
}