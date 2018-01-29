


struct VertexShaderInput
{ 
	
	float3 position		: POSITION;     // XYZ position
	float4 color		: COLOR;        // RGBA color
};


struct VertexToHull
{

	float3 position		: SV_POSITION;	// XYZW position (System Value Position)
	float4 color		: COLOR;        // RGBA color
};

VertexToHull main(VertexShaderInput input)
{

	VertexToHull output;

	//matrix worldViewProj = mul(mul(world, view), projection);
	
	//output.position = mul(float4(input.position, 1.0f), worldViewProj);
	output.position = input.position;

	output.color = input.color;

	return output;
}