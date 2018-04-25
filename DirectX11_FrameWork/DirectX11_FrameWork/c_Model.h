#pragma once

class c_Model
{
private:
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT4 color;
	};

private:
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	UINT m_vertexCount, m_indexCount;

private:
	bool InitializeBuffers(ID3D11Device* device);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*deviceContext);

public:

	bool Initialize(ID3D11Device* device);
	void Shutdown();
	void Render(ID3D11DeviceContext* deviceContext);

	int GetIndexCount();

public:
	c_Model();
	~c_Model();

};

