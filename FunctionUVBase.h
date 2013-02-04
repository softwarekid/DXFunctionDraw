#pragma once
#include <vector>

typedef float (*Fun_p)(float , float)  ;

class txFunctionUVBase
{
public:
	txFunctionUVBase(float xstart_, float xend_, float ystart_, float yend_, int m, int n);
	virtual ~txFunctionUVBase(void);

	//virtual float Function(float x, float y) = 0;

	void Discrete(Fun_p f);

	const std::vector<D3DXVECTOR3> & GetVList() { return m_VList; };
	const int M() { return m_M; }
	const int N() { return m_N; }

private:
	float m_XStart;
	float m_XEnd;
	float m_YStart;
	float m_YEnd;
	int m_M;
	int m_N;

	std::vector<D3DXVECTOR3> m_VList;
};

