cbuffer MatrixBuffer
{
	float4x4 worldMatrix;
	float4x4 viewMatrix;
	float4x4 projMatrix;
}

struct VertexIn
{
	float4 pos : POSITION;
	float4 color : COLOR;	
};

struct VertexOut
{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
};

VertexOut VS(VertexIn vin)
{
	VertexOut vout;
	vout.pos.w = 1.0f;
	vout.pos = mul(vin.pos, worldMatrix);
	vout.pos = mul(vout.pos, viewMatrix);
	vout.pos = mul(vout.pos, projMatrix);

	vout.color = vin.color;

	return vout;
}