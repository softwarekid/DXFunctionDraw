#include "d3dUtil.h"

#include <vector>
#include "../GeometryUtility/vector.h"

void D3DXVECTOR32txVec3(const std::vector<D3DXVECTOR3> &in, std::vector<txVec3> &out )
{
	out.reserve(in.size());
	txVec3 v;
	for (int i=0; i<in.size(); i++)
	{
		v.x = (double)in[i].x;
		v.y = (double)in[i].y;
		v.z = (double)in[i].z;
		out.push_back(v);
	}
}
