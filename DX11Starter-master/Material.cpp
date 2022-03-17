#include "Material.h"

Material::Material(DirectX::XMFLOAT4 tint, std::shared_ptr<SimpleVertexShader> vertexShader, std::shared_ptr<SimplePixelShader> pixelShader, float roughness)
{
    this->tint = tint;
    this->pixelShader = pixelShader;
    this->vertexShader = vertexShader;
    this->roughness = roughness;
}

DirectX::XMFLOAT4 Material::GetTint()
{
    return tint;
}

std::shared_ptr<SimplePixelShader> Material::GetPixelShader()
{
    return pixelShader;
}

float Material::GetRoughness()
{
    return roughness;
}

std::shared_ptr<SimpleVertexShader> Material::GetVertexShader()
{
    return vertexShader;
}
