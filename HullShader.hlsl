cbuffer TessellatorBuffer
{
	float tessellationAmount;
	float3 padding;
};

struct VertexToHull
{

	float3 worldPos		: POSITION;		// XYZW position (System Value Position)
										//float4 color		: COLOR;        // RGBA color
	float3 normal		: NORMAL;
	float3 tangent		: TANGENT;
	float2 uv			: TEXCOORD;
};

// Input control point
//struct VS_CONTROL_POINT_OUTPUT
//{
//	float3 vPosition : WORLDPOS;
//	// TODO: change/add other stuff
//};

// Output patch constant data.
struct HS_CONSTANT_DATA_OUTPUT
{
	float EdgeTessFactor[3]			: SV_TessFactor; // e.g. would be [4] for a quad domain
	float InsideTessFactor			: SV_InsideTessFactor; // e.g. would be Inside[2] for a quad domain
												  // TODO: change/add other stuff
};

// Output control point
struct HS_CONTROL_POINT_OUTPUT
{
	float3 worldPos	    : POSITION; 
	//float4 color		: COLOR;
	float3 normal		: NORMAL;
	float3 tangent		: TANGENT;
	float2 uv			: TEXCOORD;
};





#define NUM_CONTROL_POINTS 3

// Patch Constant Function
HS_CONSTANT_DATA_OUTPUT CalcHSPatchConstants(InputPatch<VertexToHull, NUM_CONTROL_POINTS> ip, uint PatchID : SV_PrimitiveID)
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

HS_CONTROL_POINT_OUTPUT main( 
	InputPatch<VertexToHull, NUM_CONTROL_POINTS> ip, 
	uint i : SV_OutputControlPointID,
	uint PatchID : SV_PrimitiveID )
{
	HS_CONTROL_POINT_OUTPUT Output;

	// Insert code to compute Output here
	Output.worldPos = ip[i].worldPos;
	Output.normal = ip[i].normal;
	Output.tangent = ip[i].tangent;
	Output.uv = ip[i].uv;

	//Output.color = ip[i].color;

	return Output;
}
