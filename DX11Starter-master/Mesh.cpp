#include "Mesh.h"

Microsoft::WRL::ComPtr<ID3D11Buffer> Mesh::GetVertexBuffer() {
	return vertexBuffer;
}

Microsoft::WRL::ComPtr<ID3D11Buffer> Mesh::GetIndexBuffer() {
	return indexBuffer;
}

int Mesh::GetIndexCount() {
	return numIndices;
}

void Mesh::Draw() {
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
	context->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	context->DrawIndexed(
		numIndices,     // The number of indices to use (we could draw a subset if we wanted)
		0,     // Offset to the first index we want to use
		0      // Offset to add to each index when looking up vertices
	);
}

Mesh::Mesh(Vertex* vertices, int numVertices, unsigned int* indices, int numIndices, Microsoft::WRL::ComPtr<ID3D11Device> device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> context) {
	this->numIndices = numIndices;
	this->context = context;

	// Create the VERTEX BUFFER description
	D3D11_BUFFER_DESC vbd = {};
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex) * numVertices; 
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER; // Tells DirectX this is a vertex buffer
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;

	// Create the proper struct to hold the initial vertex data
	D3D11_SUBRESOURCE_DATA initialVertexData = {};
	initialVertexData.pSysMem = vertices;

	// Actually create the buffer with the initial data
	device->CreateBuffer(&vbd, &initialVertexData, vertexBuffer.GetAddressOf());

	// Create the INDEX BUFFER description
	D3D11_BUFFER_DESC ibd = {};
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(unsigned int) * numIndices;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;	// Tells DirectX this is an index buffer
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;

	// Create the proper struct to hold the initial index data
	D3D11_SUBRESOURCE_DATA initialIndexData = {};
	initialIndexData.pSysMem = indices;

	// Actually create the buffer with the initial data
	device->CreateBuffer(&ibd, &initialIndexData, indexBuffer.GetAddressOf());
}

Mesh::~Mesh() {}