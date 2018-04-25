#include "stdafx.h"
#include "c_Model.h"

c_Model::c_Model()
	:m_vertexBuffer(NULL),
	m_indexBuffer(NULL)
{
}

c_Model::~c_Model()
{
}

bool c_Model::Initialize(ID3D11Device * device)
{
	bool result;
	// 정점 버퍼와 인덱스 버퍼를 초기화합니다. 
	result = InitializeBuffers(device);
	if (!result)
	{
		return false;
	}

	return true;
}

void c_Model::Shutdown()
{
	ShutdownBuffers();
}

void c_Model::Render(ID3D11DeviceContext * deviceContext)
{
	RenderBuffers(deviceContext);
}

int c_Model::GetIndexCount()
{
	return m_indexCount;
}

bool c_Model::InitializeBuffers(ID3D11Device * device)
{
	VertexType* vertices; 
	unsigned long* indices; 
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc; 
	D3D11_SUBRESOURCE_DATA vertexData, indexData; 
	HRESULT result;

	// 정점 배열의 길이를 설정합니다. 
	m_vertexCount = 3; 
	// 인덱스 배열의 길이를 설정합니다. 
	m_indexCount = 3; 
	// 정점 배열을 생성합니다. 
	vertices = new VertexType[m_vertexCount]; 
	if(!vertices) 
	{ 
		return false; 
	} 
	// 인덱스 배열을 생성합니다. 
	indices = new unsigned long[m_indexCount]; 
	if (!indices)
	{
		return false;
	}

	// 정점 배열에 값을 넣습니다. 
	
	vertices[0].position = XMFLOAT3(-1.0f, -1.0f, 0.0f);
	// 왼쪽 아래 
	vertices[0].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	vertices[1].position = XMFLOAT3(0.0f, 1.0f, 0.0f);
	// 상단 가운데 
	vertices[1].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	vertices[2].position = XMFLOAT3(1.0f, -1.0f, 0.0f);
	// 오른쪽 아래 
	vertices[2].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	// 인덱스 배열에 값을 넣습니다. 
	indices[0] = 0; 
	// 왼쪽 아래 
	indices[1] = 1; 
	// 상단 가운데 
	indices[2] = 2; // 오른쪽 아래

	// 정점 버퍼의 description을 작성합니다. 
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT; 
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount; 
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER; 
	vertexBufferDesc.CPUAccessFlags = 0; 
	vertexBufferDesc.MiscFlags = 0; 
	vertexBufferDesc.StructureByteStride = 0; 
	// 정점 데이터를 가리키는 보조 리소스 구조체를 작성합니다. 
	vertexData.pSysMem = vertices; 
	vertexData.SysMemPitch = 0; 
	vertexData.SysMemSlicePitch = 0; 
	// 정점 버퍼를 생성합니다. 
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer); 
	if(FAILED(result)) 
	{ 
		return false; 
	} 
	// 인덱스 버퍼의 description을 작성합니다.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT; 
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount; 
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER; 
	indexBufferDesc.CPUAccessFlags = 0; 
	indexBufferDesc.MiscFlags = 0; 
	indexBufferDesc.StructureByteStride = 0; 
	// 인덱스 데이터를 가리키는 보조 리소스 구조체를 작성합니다. 
	indexData.pSysMem = indices; 
	indexData.SysMemPitch = 0; 
	indexData.SysMemSlicePitch = 0; 
	// 인덱스 버퍼를 생성합니다. 
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer); 
	if(FAILED(result)) 
	{ 
		return false; 
	}

	// 생성되고 값이 할당된 정점 버퍼와 인덱스 버퍼를 해제합니다. 
	SAFE_DELETE_ARRAY(vertices);
	SAFE_DELETE_ARRAY(indices);
	
	return true;
}

void c_Model::ShutdownBuffers()
{
	RELEASE_COM(m_vertexBuffer);
	RELEASE_COM(m_indexBuffer);
}

void c_Model::RenderBuffers(ID3D11DeviceContext * deviceContext)
{
	unsigned int stride; 
	unsigned int offset; 
	// 정점 버퍼의 단위와 오프셋을 설정합니다. 
	stride = sizeof(VertexType); offset = 0; 
	// input assembler에 정점 버퍼를 활성화하여 그려질 수 있게 합니다. 
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset); 
	// input assembler에 인덱스 버퍼를 활성화하여 그려질 수 있게 합니다. 
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0); 
	// 정점 버퍼로 그릴 기본형을 설정합니다. 여기서는 삼각형입니다. 
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}
