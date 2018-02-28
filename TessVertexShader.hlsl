cbuffer externalData	: register(b0)
{
	matrix world;
}

struct VertexShaderInput
{ 
	
	float3 position		: POSITION;     // XYZ position
	//float4 color		: COLOR;        // RGBA color
	float2 uv			: TEXCOORD;
	float3 normal		: NORMAL;
	float3 tangent		: TANGENT;
};


struct VertexToHull
{

	float3 worldPos		: POSITION;		// XYZW position (System Value Position)
	//float4 color		: COLOR;        // RGBA color
	float3 normal		: NORMAL;
	float3 tangent		: TANGENT;
	float2 uv			: TEXCOORD;
};

VertexToHull main(VertexShaderInput input)
{

	VertexToHull output;

	//matrix worldViewProj = mul(mul(world, view), projection);
	
	//output.position = mul(float4(input.position, 1.0f), worldViewProj);
	//output.position = mul(float4(input.position, 1.0f), world).xyz;

	

	//output.color = input.color;

	output.normal = mul(input.normal, (float3x3)world);
	output.tangent = mul(input.tangent, (float3x3)world);
	output.worldPos = mul(float4(input.position, 1.0f), world).xyz;
	output.uv = input.uv;


	return output;
}