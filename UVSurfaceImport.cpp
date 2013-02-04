#include "DXUT.h"
#include "UVSurfaceImport.h"
#include <stdlib.h>
#include <exception>
#include <assert.h>

#define BufferSize 25

txUVSurfaceImport::txUVSurfaceImport(void)
	:m_M(0)
	,m_N(0)
	,m_VList(0)
{
}


txUVSurfaceImport::~txUVSurfaceImport(void)
{
}

void txUVSurfaceImport::ConstructSurfaceFromFile(const std::string &filename)
{
	//std::ofstream fin(filename.c_str());
	FILE *rfp;
	if ((rfp=fopen(filename.c_str(),"r"))==NULL)
	{
		throw std::logic_error("read file failer!");
	}
	char buffer[BufferSize];
	int numrow;
	int numcolumn;
	
	fscanf(rfp,"%s %i %i\n",buffer,&numrow,&numcolumn);
	assert(numrow);
	assert(numcolumn);
	m_M = numrow;
	m_N = numcolumn;
	m_VList.reserve(numrow*numcolumn);

	double x,y,z;
	while(!feof(rfp))
	{
		fscanf(rfp,"%lf %lf %lf\n",&x,&y,&z);
		D3DXVECTOR3 p;
		p.x = (float)x;
		p.y = (float)y;
		p.z = (float)z;
		m_VList.push_back(p);
	}

	fclose(rfp);
}
