#include "DXUT.h"
#include "d3dUtil.h"
#include "FunctionDrawor.h"
#include <assert.h>


txFunctionDrawor::txFunctionDrawor(const std::vector<D3DXVECTOR3> &vlist_, int m_, int n_, ID3D10Device* pD3DDevice)
	: m_Vlist(vlist_)
	, m_N(n_)
	, m_M(m_)
{
	assert(pD3DDevice);
	assert(m_);
	assert(n_);
	m_pD3DDevice = pD3DDevice;

	m_NumVertices = (m_M-1)*(m_N-1)*6;
	m_NumFaces = (m_M-1)*(m_N-1)*2; // quadrilateral consists by two triangle faces

	ConstructDrawBuffer();
}


txFunctionDrawor::~txFunctionDrawor(void)
{
	ReleaseCOM(mVB);
	ReleaseCOM(mIB);
}

void txFunctionDrawor::ConstructDrawBuffer()
{

	D3DXVECTOR2 tempTec(0.0f,0.0f);

	std::vector<Vertex> verticesBuffer(m_NumFaces*3);
	std::vector<DWORD> indexBuffer;
	indexBuffer.reserve(m_NumFaces*3);

	D3DXVECTOR3 p0,p1,p2,p3;
	D3DXVECTOR3 normal0;
	D3DXVECTOR3 normal1;
	D3DXVECTOR3 r0;
	D3DXVECTOR3 r1;
	D3DXVECTOR3 r2;


	DWORD meshIndex=0;
	for (int i=0; i<m_N-1; i++)
	{
		for (int j=0; j<m_M-1; j++)
		{
			
			p0 = m_Vlist[i*m_M+j];
			p1 = m_Vlist[i*m_M+j+1];
			p2 = m_Vlist[(i+1)*m_M+j+1];
			p3 = m_Vlist[(i+1)*m_M+j];

			r0 = p3-p0;
			r1 = p2-p0;
			r2 = p1-p0;
			D3DXVec3Cross(&normal0, &r1,&r0);
			D3DXVec3Normalize(&normal0,&normal0);
			D3DXVec3Cross(&normal1, &r2,&r1);
			D3DXVec3Normalize(&normal1,&normal1);

			verticesBuffer[meshIndex].pos = p0;
			verticesBuffer[meshIndex].normal = normal1;
			verticesBuffer[meshIndex].texC = tempTec;
			indexBuffer.push_back(meshIndex);
			verticesBuffer[meshIndex+1].pos = p2;
			verticesBuffer[meshIndex+1].normal = normal1;
			verticesBuffer[meshIndex+1].texC = tempTec;
			indexBuffer.push_back(meshIndex+1);
			verticesBuffer[meshIndex+2].pos = p3;
			verticesBuffer[meshIndex+2].normal = normal1;
			verticesBuffer[meshIndex+2].texC = tempTec;
			indexBuffer.push_back(meshIndex+2);
			verticesBuffer[meshIndex+3].pos = p0;
			verticesBuffer[meshIndex+3].normal = normal0;
			verticesBuffer[meshIndex+3].texC = tempTec;
			indexBuffer.push_back(meshIndex+3);
			verticesBuffer[meshIndex+4].pos = p1;
			verticesBuffer[meshIndex+4].normal = normal0;
			verticesBuffer[meshIndex+4].texC = tempTec;
			indexBuffer.push_back(meshIndex+4);
			verticesBuffer[meshIndex+5].pos = p2;
			verticesBuffer[meshIndex+5].normal = normal0;
			verticesBuffer[meshIndex+5].texC = tempTec;
			indexBuffer.push_back(meshIndex+5);


			meshIndex+=6;
		}
	}


	// Set vertex buffer
	D3D10_BUFFER_DESC vbd;
    vbd.Usage = D3D10_USAGE_IMMUTABLE;
    vbd.ByteWidth = sizeof(Vertex) * m_NumVertices;
    vbd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags = 0;
    vbd.MiscFlags = 0;
    D3D10_SUBRESOURCE_DATA vinitData;
    vinitData.pSysMem = &verticesBuffer[0];
    HR(m_pD3DDevice->CreateBuffer(&vbd, &vinitData, &mVB));

	D3D10_BUFFER_DESC ibd;
    ibd.Usage = D3D10_USAGE_IMMUTABLE;
    ibd.ByteWidth = sizeof(DWORD) * m_NumFaces*3;
    ibd.BindFlags = D3D10_BIND_INDEX_BUFFER;
    ibd.CPUAccessFlags = 0;
    ibd.MiscFlags = 0;
    D3D10_SUBRESOURCE_DATA iinitData;
    iinitData.pSysMem = &indexBuffer[0];
    HR(m_pD3DDevice->CreateBuffer(&ibd, &iinitData, &mIB));
}


void txFunctionDrawor::Draw()
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	m_pD3DDevice->IASetVertexBuffers(0,1,&mVB,&stride,&offset);
	m_pD3DDevice->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);
	m_pD3DDevice->DrawIndexed(m_NumFaces*3,0,0);
}