#pragma once
#include <vector>
#include <fstream>

struct ID3D10Device;
struct ID3DX10Mesh;
struct D3DXVECTOR3;
struct D3DXVECTOR2;

struct VERTEX;


class DumpMesh
{
public:
	DumpMesh(ID3D10Device* pd3dDevice, ID3DX10Mesh* pMesh);
	~DumpMesh(void);


public:
	void DumpVertices();
	void DumpIndices();

	std::vector<D3DXVECTOR3> &GetVertexCache(){return m_Position;};
	std::vector<size_t> &GetIndexCache(){return m_Index;};

private:
	void FillCacheDataFromMesh();
private:
	ID3D10Device* m_pd3dDevice;
	ID3DX10Mesh* m_pMesh;

	std::vector<D3DXVECTOR3> m_Position;
	std::vector<size_t> m_Index;

	std::ofstream m_Dumpfile;
};

