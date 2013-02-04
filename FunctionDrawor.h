#pragma once
#include <vector>
#include "Vertex.h"

class txFunctionDrawor
{
public:

	txFunctionDrawor(const std::vector<D3DXVECTOR3> &vlist_, int m_, int n_, ID3D10Device* pD3DDevice);
	~txFunctionDrawor(void);

	void Draw();

private:
	void ConstructDrawBuffer();


private:
	const std::vector<D3DXVECTOR3> &m_Vlist;
	const int m_N;
	const int m_M;

	int m_NumVertices;
	int m_NumFaces; // Triangle faces

	ID3D10Device* m_pD3DDevice;
	ID3D10Buffer* mVB;
	ID3D10Buffer* mIB;
};

