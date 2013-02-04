#pragma once
#include <vector>
#include <string>

class txUVSurfaceImport
{
public:
	txUVSurfaceImport(void);
	~txUVSurfaceImport(void);

	void ConstructSurfaceFromFile(const std::string &filename);

	const int M() { return m_M; };
	const int N() { return m_N; };
	const std::vector<D3DXVECTOR3> & GetVList() { return m_VList; };

private:
	int m_M;
	int m_N;
	std::vector<D3DXVECTOR3> m_VList;
};

