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
	// ���� ���ۿ� �ε��� ���۸� �ʱ�ȭ�մϴ�. 
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

	// ���� �迭�� ���̸� �����մϴ�. 
	m_vertexCount = 3; 
	// �ε��� �迭�� ���̸� �����մϴ�. 
	m_indexCount = 3; 
	// ���� �迭�� �����մϴ�. 
	vertices = new VertexType[m_vertexCount]; 
	if(!vertices) 
	{ 
		return false; 
	} 
	// �ε��� �迭�� �����մϴ�. 
	indices = new unsigned long[m_indexCount]; 
	if (!indices)
	{
		return false;
	}

	// ���� �迭�� ���� �ֽ��ϴ�. 
	
	vertices[0].position = XMFLOAT3(-1.0f, -1.0f, 0.0f);
	// ���� �Ʒ� 
	vertices[0].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	vertices[1].position = XMFLOAT3(0.0f, 1.0f, 0.0f);
	// ��� ��� 
	vertices[1].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	vertices[2].position = XMFLOAT3(1.0f, -1.0f, 0.0f);
	// ������ �Ʒ� 
	vertices[2].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	// �ε��� �迭�� ���� �ֽ��ϴ�. 
	indices[0] = 0; 
	// ���� �Ʒ� 
	indices[1] = 1; 
	// ��� ��� 
	indices[2] = 2; // ������ �Ʒ�

	// ���� ������ description�� �ۼ��մϴ�. 
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT; 
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount; 
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER; 
	vertexBufferDesc.CPUAccessFlags = 0; 
	vertexBufferDesc.MiscFlags = 0; 
	vertexBufferDesc.StructureByteStride = 0; 
	// ���� �����͸� ����Ű�� ���� ���ҽ� ����ü�� �ۼ��մϴ�. 
	vertexData.pSysMem = vertices; 
	vertexData.SysMemPitch = 0; 
	vertexData.SysMemSlicePitch = 0; 
	// ���� ���۸� �����մϴ�. 
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer); 
	if(FAILED(result)) 
	{ 
		return false; 
	} 
	// �ε��� ������ description�� �ۼ��մϴ�.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT; 
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount; 
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER; 
	indexBufferDesc.CPUAccessFlags = 0; 
	indexBufferDesc.MiscFlags = 0; 
	indexBufferDesc.StructureByteStride = 0; 
	// �ε��� �����͸� ����Ű�� ���� ���ҽ� ����ü�� �ۼ��մϴ�. 
	indexData.pSysMem = indices; 
	indexData.SysMemPitch = 0; 
	indexData.SysMemSlicePitch = 0; 
	// �ε��� ���۸� �����մϴ�. 
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer); 
	if(FAILED(result)) 
	{ 
		return false; 
	}

	// �����ǰ� ���� �Ҵ�� ���� ���ۿ� �ε��� ���۸� �����մϴ�. 
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
	// ���� ������ ������ �������� �����մϴ�. 
	stride = sizeof(VertexType); offset = 0; 
	// input assembler�� ���� ���۸� Ȱ��ȭ�Ͽ� �׷��� �� �ְ� �մϴ�. 
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset); 
	// input assembler�� �ε��� ���۸� Ȱ��ȭ�Ͽ� �׷��� �� �ְ� �մϴ�. 
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0); 
	// ���� ���۷� �׸� �⺻���� �����մϴ�. ���⼭�� �ﰢ���Դϴ�. 
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}
