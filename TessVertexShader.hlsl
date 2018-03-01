cbuffer externalData	: register(b0)
{
	matrix world;
}

struct VertexSaderInput
{
	float3 position		: POSITION;
	float2 uv			: TEXCOORD;
	float3 normal		: NORMAL;
	float3 tangent		: TANGENT;
};

struct VertexToHull
{
	float3 normal		: NORMAL;
	float3 tangent		: TANGENT;
	float3 worldPos		: POSITION;
	float2 uv			: TEXCOORD;
};

VertexToHull main(VertexSaderInput input)
{
	VertexToHull output;

	output.normal = mul(input.normal, (float3x3)world);
	output.tangent = mul(input.tangent, (float3x3)world);
	output.worldPos = mul(float4(input.position, 1.0f), world).xyz;
	output.uv = input.uv;

	return output;

}