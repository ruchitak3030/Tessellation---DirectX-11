cbuffer externalData : register(b0)
{
	matrix view;
	matrix projection;
};

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

struct DomainToPixel
{
	float4 position		: SV_POSITION;
	//float4 color		: COLOR;
	float3 normal		: NORMAL;
	float3 tangent		: TANGENT;
	float3 worldPos		: POSITION;
	float2 uv			: TEXCOORD;
};

#define NUM_CONTROL_POINTS 3

[domain("tri")]
DomainToPixel main(HS_CONSTANT_DATA_OUTPUT input, float3 domain : SV_DomainLocation, const OutputPatch<HS_CONTROL_POINT_OUTPUT, NUM_CONTROL_POINTS> patch)
{
	float3 vertexPosition;
	DomainToPixel Output;

	//vertexPosition = float4(patch[0].position*domain.x+patch[1].position*domain.y+patch[2].position*domain.z,1);

	/*vertexPosition = domain.x*patch[0].position + domain.y*patch[1].position + domain.z*patch[2].position;

	Output.position = mul(float4(vertexPosition, 1.0f), world);
	Output.position = mul(Output.position, view);
	Output.position = mul(Output.position, projection);*/

	Output.normal = domain.x*patch[0].normal + domain.y*patch[1].normal + domain.z*patch[2].normal;
	Output.tangent = domain.x*patch[0].tangent + domain.y*patch[1].tangent + domain.z*patch[2].tangent;
	Output.worldPos = domain.x*patch[0].worldPos + domain.y*patch[1].worldPos + domain.z*patch[2].worldPos;
	Output.uv = domain.x*patch[0].uv + domain.y*patch[1].uv + domain.z*patch[2].uv;
	
	//Output.color = patch[0].color;

	Output.normal = normalize(Output.normal);

	matrix viewProj = mul(view, projection);

	Output.position = mul(float4(Output.worldPos, 1.0f), viewProj);

	return Output;
}
