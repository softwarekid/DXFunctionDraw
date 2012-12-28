#include "DXUT.h"
#include "txFunctionMeshDisplay.h"
#include "d3dUtil.h"
#include <vector>
#include "Vertex.h"
#include "Box.h"

txFunctionMeshDisplay::txFunctionMeshDisplay(ID3D10Device* pD3DDevice)
	:md3dDevice(pD3DDevice)
{
	m_DebugBox = new Box();
	init();
}


txFunctionMeshDisplay::~txFunctionMeshDisplay(void)
{
	ReleaseCOM(mVB);
	ReleaseCOM(mIB);
	delete m_DebugBox;
	m_DebugBox = NULL;
}

void txFunctionMeshDisplay::init(FLOAT xMin,  FLOAT xMax, DWORD xGridCount,
	FLOAT yMin, FLOAT yMax, DWORD yGridCount
	// Need to add a call back function to calculate z
	)
{
	m_XMin = xMin;
	m_XMax = xMax;
	m_XGridCount = xGridCount;
	m_YMin = yMin;
	m_YMax = yMax;
	m_YGridCount = yGridCount;

	D3DXVECTOR3 maxAABB(m_XMin,m_YMin,0);
	D3DXVECTOR3 minAABB(m_XMax,m_YMax,CalculateZ(m_XMin,m_YMin));
	m_DebugBox->customizeInit(md3dDevice,minAABB,maxAABB);

	D3DXVECTOR2 tempTec(0.0f,0.0f);
	// calculate z
	FLOAT x = m_XMin;
	FLOAT y = m_YMin;
	FLOAT deltaX = (m_XMax - m_XMin)/m_XGridCount;
	FLOAT deltaY = (m_YMax - m_YMin)/m_YGridCount;
	m_NumVertices = (xGridCount-1)*(yGridCount-1)*6;
	m_NumFaces = (xGridCount-1)*(yGridCount-1)*2;
	std::vector<Vertex> verticesBuffer((xGridCount-1)*(yGridCount-1)*6);
	std::vector<DWORD> indexBuffer;
	indexBuffer.reserve((xGridCount-1)*(yGridCount-1)*6);
	D3DXVECTOR3 p0,p1,p2,p3;
	D3DXVECTOR3 normal0;
	D3DXVECTOR3 normal1;
	D3DXVECTOR3 r0;
	D3DXVECTOR3 r1;
	D3DXVECTOR3 r2;
	DWORD meshIndex=0;


	for (DWORD i=0; i<m_YGridCount-1; i++){
		x=m_XMin;
		for (DWORD j=0; j<(m_XGridCount-1)*6; j+=6){
			meshIndex = i*(m_YGridCount-1)*6+j;
			p0=D3DXVECTOR3(x,y,CalculateZ(x,y));
			p1=D3DXVECTOR3(x+deltaX,y,CalculateZ(x+deltaX,y));
			p2=D3DXVECTOR3(x+deltaX,y+deltaY,CalculateZ(x+deltaX,y+deltaY));
			p3=D3DXVECTOR3(x,y+deltaY,CalculateZ(x,y+deltaY));
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
			x+=deltaX;
		}
		y+=deltaY;
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
    HR(md3dDevice->CreateBuffer(&vbd, &vinitData, &mVB));

	D3D10_BUFFER_DESC ibd;
    ibd.Usage = D3D10_USAGE_IMMUTABLE;
    ibd.ByteWidth = sizeof(DWORD) * m_NumFaces*3;
    ibd.BindFlags = D3D10_BIND_INDEX_BUFFER;
    ibd.CPUAccessFlags = 0;
    ibd.MiscFlags = 0;
    D3D10_SUBRESOURCE_DATA iinitData;
    iinitData.pSysMem = &indexBuffer[0];
    HR(md3dDevice->CreateBuffer(&ibd, &iinitData, &mIB));
	

}

FLOAT txFunctionMeshDisplay::CalculateZ(FLOAT x, FLOAT y)
{
	return x*x + y*y;
}

void txFunctionMeshDisplay::DrawFunction(){
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	md3dDevice->IASetVertexBuffers(0,1,&mVB,&stride,&offset);
	md3dDevice->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);
	md3dDevice->DrawIndexed(m_NumFaces*3,0,0);

	m_DebugBox->draw();
}

void txFunctionMeshDisplay::DrawBoundingBox(){
	m_DebugBox->draw();
}

