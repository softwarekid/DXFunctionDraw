#pragma once
#include <vector>


class Box;

struct D3DXVECTOR3;

const static FLOAT txEPSILON = 0.001;

template<typename T>
static T txABS(const T &v){
	return v>0.0?v:-v;
};

typedef enum txSplitAxis
{
	XAXIS=0,
	YAXIS=1,
	ZAXIS=2
}txSplitAxis;

typedef enum txTrianglePlaneRelation
{
	TOP=0,
	MIDDLE=1,
	BOTTOM=2
}txTrianglePlaneRelation;

typedef struct txAABB
{
	D3DXVECTOR3 min;
	D3DXVECTOR3 max;

	//static void ComputeAABB();
}txAABB;

typedef struct txBoundingBoxNode
{
	std::vector<size_t> *m_pIndices;
	std::vector<size_t> *m_pMiddleIndices;
	txAABB aabb;
	txBoundingBoxNode *left;
	txBoundingBoxNode *right;
}txBoundingBoxNode;

class txAABBConstructor
{
public:
	txAABBConstructor(
		ID3D10Device* device,
		const std::vector<D3DXVECTOR3> &vertices,
		const std::vector<size_t> &indices);
	~txAABBConstructor(void);

	void GetDXDevice(ID3D10Device* device){m_PD3Device = device;};

	void DrawLevel(size_t i);

	void DrawAllAABBDetial();

	size_t GetAABBLevelCount(){return m_AABBTreeLevelsOffset.size();}

private:
	void ConstructAABBTree(txBoundingBoxNode *&root, 
		std::vector<size_t> *&indices);

	void ConstructAABBTreeLevel();

	void ComputeAABB(const std::vector<size_t> &indices,
		txAABB &aabb);

	void ComputeSplitPlane(const txAABB &aabb, 
		txSplitAxis &axis, FLOAT &value);

	D3DXVECTOR3 GetCenterPosition(size_t faceIndex, const std::vector<size_t> &indices){
		return (m_Vertices[m_Indices[3*indices[faceIndex]]]+m_Vertices[m_Indices[3*indices[faceIndex]+1]]+m_Vertices[m_Indices[3*indices[faceIndex]+2]])/3.0;
	}

	void SplitTriangleList(const std::vector<size_t> &indices,size_t faceIndex, txSplitAxis &axis, FLOAT &value, txTrianglePlaneRelation &triPlaneRelation);

	void DrawInitialize();

	
	

private:
	const std::vector<D3DXVECTOR3> m_Vertices;
	const std::vector<size_t> m_Indices;

	txBoundingBoxNode *mRoot;

	std::vector<txBoundingBoxNode* > m_AABBTreeLevels;
	std::vector<size_t> m_AABBTreeLevelsOffset;

	std::vector<Box*> m_pAABBDrawLevels;
	std::vector<size_t> m_AABBDrawLevelOffset;

	ID3D10Device* m_PD3Device;
};

