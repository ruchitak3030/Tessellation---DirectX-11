#include "Material.h"

Material::Material(SimplePixelShader * pixelShader, SimpleVertexShader * vertexShader, ID3D11ShaderResourceView * materialSRV, ID3D11ShaderResourceView * normalSRV, ID3D11SamplerState * materialSampler)
{
	_pixelShader = pixelShader;
	_vertexShader = vertexShader;
	_materialSRV = materialSRV;
	_normalSRV = normalSRV;
	_materialSampler = materialSampler;
}

Material::~Material()
{
}

SimplePixelShader * Material::GetPixelShader()
{
	return _pixelShader;
}

SimpleVertexShader * Material::GetVertexShader()
{
	return _vertexShader;
}

ID3D11ShaderResourceView * Material::GetMaterialSRV()
{
	return _materialSRV;
}

ID3D11ShaderResourceView * Material::GetNormalSRV()
{
	return _normalSRV;
}

ID3D11SamplerState * Material::GetMaterialSampler()
{ 
	return _materialSampler;
}
