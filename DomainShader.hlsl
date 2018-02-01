cbuffer externalData : register(b0)
{
	matrix world;
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
	float3 position	    : POSITION;
	float4 color		: COLOR;
};

struct DomainToPixel
{
	float4 position		: SV_POSITION;
	float4 color		: COLOR;
};

#define NUM_CONTROL_POINTS 3

[domain("tri")]
DomainToPixel main(HS_CONSTANT_DATA_OUTPUT input, float3 domain : SV_DomainLocation, const OutputPatch<HS_CONTROL_POINT_OUTPUT, NUM_CONTROL_POINTS> patch)
{
	float3 vertexPosition;
	DomainToPixel Output;

	//vertexPosition = float4(patch[0].position*domain.x+patch[1].position*domain.y+patch[2].position*domain.z,1);

	vertexPosition = domain.x*patch[0].position + domain.y*patch[1].position + domain.z*patch[2].position;

	Output.position = mul(float4(vertexPosition, 1.0f), world);
	Output.position = mul(Output.position, view);
	Output.position = mul(Output.position, projection);
	
	Output.color = patch[0].color;

	return Output;
}
