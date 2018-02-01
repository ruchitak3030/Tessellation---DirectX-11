
struct DomainToPixel
{
	float4 position		: SV_POSITION;
	float4 color		: COLOR;
};

float4 main(DomainToPixel input) : SV_TARGET
{
	
	return input.color;
}