#pragma once
#include "Transform.h"
#include "Mesh.h"
#include <memory>
#include "Camera.h"

class Entity
{
public:
	Entity(std::shared_ptr<Mesh> mesh);

	void Draw(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, Microsoft::WRL::ComPtr<ID3D11Buffer> constantBufferVS, std::shared_ptr<Camera> camera);

	Transform* GetTransform();
	std::shared_ptr<Mesh> GetMesh();

private:
	Transform transform;
	std::shared_ptr<Mesh> mesh;
};

