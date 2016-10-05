cbuffer MatrixBuffer
{
	float4x4 world;
	float4x4 view;
	float4x4 proj;
}

struct VertexIn
{
	float4 pos : POSITION;
	float2 tex : TEXCOORD0;	
};

struct VertexOut
{
	float4 pos : SV_POSITION;	
	float2 tex : TEXCOORD0;
};

VertexOut VS(VertexIn vin)
{
	VertexOut vout;
	vin.pos.w = 1.0f;
	vout.pos = mul(vin.pos, world);
	vout.pos = mul(vout.pos, view);
	vout.pos = mul(vout.pos, proj);

	vout.tex = vin.tex;

	return vout;
}