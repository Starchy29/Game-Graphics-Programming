#include "Entity.h"
#include "Camera.h"

using namespace DirectX;

Entity::Entity(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material)
{
	this->mesh = mesh;
	this->material = material;
}

void Entity::Draw(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, std::shared_ptr<Camera> camera)
{
	std::shared_ptr<SimpleVertexShader> vs = material->GetVertexShader();
	vs->SetFloat4("colorTint", material->GetTint());  
	vs->SetMatrix4x4("world", transform.GetWorldMatrix()); 
	vs->SetMatrix4x4("view", camera->GetView()); 
	vs->SetMatrix4x4("projection", camera->GetProjection()); 

	vs->CopyAllBufferData();

	/*D3D11_MAPPED_SUBRESOURCE mappedBuffer = {};
	context->Map(constantBufferVS.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedBuffer);
	memcpy(mappedBuffer.pData, &vsData, sizeof(vsData));
	context->Unmap(constantBufferVS.Get(), 0);

	context->VSSetConstantBuffers(0, // Which slot (register) to bind the buffer to?  
		1, // How many are we activating?  Can do multiple at once  
		constantBufferVS.GetAddressOf());  // Array of buffers (or the address of one)*/

	material->GetVertexShader()->SetShader();
	material->GetPixelShader()->SetShader();

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
