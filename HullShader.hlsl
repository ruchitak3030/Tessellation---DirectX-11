cbuffer externalData : register(b0)
{
	float tessellationAmount;
}
// Input control point
struct VertexToHull
{
	float3 normal		: NORMAL;
	float3 tangent		: TANGENT;
	float3 worldPos		: POSITION;
	float2 uv			: TEXCOORD;

};

// Output control point
struct HullToDomain
{
	float3 normal		: NORMAL;
	float3 tangent		: TANGENT;
	float3 worldPos		: POSITION;
	float2 uv			: TEXCOORD;
};

// Output patch constant data.
struct HS_CONSTANT_DATA_OUTPUT
{
	float EdgeTessFactor[3]			: SV_TessFactor; // e.g. would be [4] for a quad domain
	float InsideTessFactor : SV_InsideTessFactor; // e.g. would be Inside[2] for a quad domain
												  // TODO: change/add other stuff
};

#define NUM_CONTROL_POINTS 3

// Patch Constant Function
HS_CONSTANT_DATA_OUTPUT CalcHSPatchConstants(
	InputPatch<VertexToHull, NUM_CONTROL_POINTS> ip,
	uint PatchID : SV_PrimitiveID)
{
	HS_CONSTANT_DATA_OUTPUT Output;

	// Insert code to compute Output here
	Output.EdgeTessFactor[0] = tessellationAmount;
	Output.EdgeTessFactor[1] = tessellationAmount;
	Output.EdgeTessFactor[2] = tessellationAmount;
	Output.InsideTessFactor = tessellationAmount; // e.g. could calculate dynamic tessellation factors instead

	return Output;
}

[domain("tri")]
[partitioning("fractional_odd")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("CalcHSPatchConstants")]
HullToDomain main(
	InputPatch<VertexToHull, NUM_CONTROL_POINTS> ip,
	uint i : SV_OutputControlPointID,
	uint PatchID : SV_PrimitiveID)
{
	HullToDomain Output;

	// Insert code to compute Output here
	//Output.vPosition = ip[i].vPosition;
	Output.normal = ip[i].normal;
	Output.tangent = ip[i].tangent;
	Output.worldPos = ip[i].worldPos;
	Output.uv = ip[i].uv;

	return Output;
}