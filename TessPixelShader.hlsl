
Texture2D textureSRV : register(t0);
Texture2D normalMapSRV : register(t1);
SamplerState basicSampler : register(s0);


struct DomainToPixel
{
	float4 vPosition	: SV_POSITION;
	float3 normal		: NORMAL;
	float3 tangent		: TANGENT;
	float3 worldPos		: POSITION;
	float2 uv			: TEXCOORD;
};

float4 main(DomainToPixel input) : SV_TARGET
{
	input.normal = normalize(input.normal);
	input.tangent = normalize(input.tangent);

	float3 normalFromMap = normalMapSRV.Sample(basicSampler, input.uv).xyz * 2 - 1;

	// Transform from tangent to world space
	float3 N = input.normal;
	float3 T = normalize(input.tangent - N * dot(input.tangent, N));
	float3 B = cross(T, N);

	float3x3 TBN = float3x3(T, B, N);
	input.normal = normalize(mul(normalFromMap, TBN));

	float4 surfaceColor = textureSRV.Sample(basicSampler, input.uv);

	return surfaceColor;
}