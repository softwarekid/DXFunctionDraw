#include "DXUT.h"
#include "FunctionUVBase.h"
#include <assert.h>


txFunctionUVBase::txFunctionUVBase(float xstart_, float xend_, float ystart_, float yend_, int m, int n)
{
	assert(m);
	assert(n);
	m_XStart = xstart_;
	m_XEnd = xend_;
	m_YStart =ystart_ ;
	m_YEnd = yend_;
	m_M = m;
	m_N = n;
	m_VList.reserve(m*n);
}


txFunctionUVBase::~txFunctionUVBase(void)
{
}

void txFunctionUVBase::Discrete(Fun_p f)
{
	float x = m_XStart;
	float y = m_YStart;
	float stepx = (m_XEnd - m_XStart)/(m_M-1);
	float stepy = (m_YEnd - m_YStart)/(m_N-1);

	for (int i=0; i<m_N; i++)
	{
		x = m_XStart;
		for (int j=0; j<m_M; j++)
		{
			D3DXVECTOR3 p;
			p.x = x;
			p.y = y;
			p.z = f(x,y);
			m_VList.push_back(p);
			x+=stepx;
		}
		y+=stepy;
	}

}
