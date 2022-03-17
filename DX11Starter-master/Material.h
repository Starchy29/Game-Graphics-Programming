#pragma once
#include <DirectXMath.h>
#include <memory>
#include "SimpleShader.h"

class Material
{
public:
	Material(DirectX::XMFLOAT4 tint, std::shared_ptr<SimpleVertexShader> vertexShader, std::shared_ptr<SimplePixelShader> pixelShader, float roughness);

	DirectX::XMFLOAT4 GetTint();
	std::shared_ptr<SimpleVertexShader> GetVertexShader();
	std::shared_ptr<SimplePixelShader> GetPixelShader();
	float GetRoughness();

private:
	DirectX::XMFLOAT4 tint;
	std::shared_ptr<SimpleVertexShader> vertexShader;
	std::shared_ptr<SimplePixelShader> pixelShader;
	float roughness; // 0 -1
};

