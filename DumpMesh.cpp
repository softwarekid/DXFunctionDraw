#include "DXUT.h"
#include "d3dUtil.h"
#include "DumpMesh.h"
#include "MeshLoader10.h"


DumpMesh::DumpMesh(ID3D10Device* pd3dDevice, ID3DX10Mesh* pMesh)
	:m_pd3dDevice(pd3dDevice)
	,m_pMesh(pMesh)
{
	assert(m_pd3dDevice);
	assert(m_pMesh);
	m_Dumpfile.open("DumpFile.txt");
}


DumpMesh::~DumpMesh(void)
{
	m_Dumpfile.close();
}

void DumpMesh::FillCacheDataFromMesh()
{

}

void DumpMesh::DumpVertices()
{
	const D3D10_INPUT_ELEMENT_DESC *pDesc;
	UINT declCount = NULL;

	m_pMesh->GetVertexDescription(&pDesc, &declCount);

	m_Dumpfile << "---------IALayout-----------"  << std::endl;
	for (size_t i=0; i<declCount; i++)
	{
		m_Dumpfile.width(10);
		m_Dumpfile
			<< pDesc[i].SemanticName << "\t" 
			<< pDesc[i].Format << "\t"
			<< pDesc[i].AlignedByteOffset << "\t"
			<< pDesc[i].InputSlotClass << "\t"
			<< std::endl;
	}

	// Hard code the position type, should be consistances with the 
	// IALayout declaration

	ID3DX10MeshBuffer *vb = NULL;
	m_pMesh->GetVertexBuffer(0, &vb);

	VERTEX *vertices = NULL;
	SIZE_T vbufferSize;

	vb->Map( (void**)&vertices, &vbufferSize );

	m_Position.reserve(m_pMesh->GetVertexBufferCount());
	////m_Dumpfile << "---------Vertices-----------"  << std::endl;
	for (size_t i=0; i<m_pMesh->GetVertexCount(); i++)
	{
		//m_Dumpfile.width(10);
		//m_Dumpfile 
		//	<< vertices[i].position.x << "\t"
		//	<< vertices[i].position.y << "\t"
		//	<< vertices[i].position.z << "\t" 
		//	<< std::endl;
		m_Position.push_back(vertices[i].position);
	}

	vb->Unmap();
	ReleaseCOM(vb);

}

void DumpMesh::DumpIndices()
{
	ID3DX10MeshBuffer *indexBuffer = NULL;
	m_pMesh->GetIndexBuffer(&indexBuffer);

	DWORD *indices;
	SIZE_T bufferSize;
	indexBuffer->Map((void **)&indices, &bufferSize);

	size_t triangleIndexCount = m_pMesh->GetFaceCount()*3;
	m_Index.reserve(triangleIndexCount);
	m_Dumpfile << "---------Index--------------" << std::endl;
	for (size_t i=0; i<triangleIndexCount; i+=3)
	{
		//m_Dumpfile << indices[i] << "/" << indices[i+1] << "/" << indices[i+2] << std::endl;
		m_Index.push_back(indices[i]);
		m_Index.push_back(indices[i+1]);
		m_Index.push_back(indices[i+2]);
	}

	indexBuffer->Unmap();
	ReleaseCOM(indexBuffer);
}