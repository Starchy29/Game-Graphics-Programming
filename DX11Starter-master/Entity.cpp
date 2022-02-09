#include "Entity.h"
#include "BufferStructs.h"

using namespace DirectX;

Entity::Entity(std::shared_ptr<Mesh> mesh)
{
	this->mesh = mesh;
}

void Entity::Draw(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, Microsoft::WRL::ComPtr<ID3D11Buffer> constantBufferVS)
{
	VertexShaderExternalData vsData;
	vsData.colorTint = XMFLOAT4(1.0f, 0.5f, 0.5f, 1.0f);
	vsData.worldMatrix = transform.GetWorldMatrix();

	D3D11_MAPPED_SUBRESOURCE mappedBuffer = {};
	context->Map(constantBufferVS.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedBuffer);
	memcpy(mappedBuffer.pData, &vsData, sizeof(vsData));
	context->Unmap(constantBufferVS.Get(), 0);

	context->VSSetConstantBuffers(0, // Which slot (register) to bind the buffer to?  
		1, // How many are we activating?  Can do multiple at once  
		constantBufferVS.GetAddressOf());  // Array of buffers (or the address of one)

	mesh->Draw();
}

Transform* Entity::GetTransform()
{
	return &transform;
}

std::shared_ptr<Mesh> Entity::GetMesh()
{
	return mesh;
}
