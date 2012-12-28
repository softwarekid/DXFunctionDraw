#include "DXUT.h"
#include "txAABBConstructor.h"
#include "d3dUtil.h"
#include <math.h>

#include "Box.h"


txAABBConstructor::	txAABBConstructor(
		ID3D10Device* device,
		const std::vector<D3DXVECTOR3> &vertices,
		const std::vector<size_t> &indices)
		:m_Vertices(vertices)
		,m_Indices(indices)
		,mRoot(NULL)
		,m_AABBTreeLevels(0)
		,m_AABBTreeLevelsOffset(0)
		,m_pAABBDrawLevels(0)
		,m_AABBDrawLevelOffset(0)
		,m_PD3Device(device)
{
	assert(vertices.size());
	assert(indices.size());
	assert(indices.size()%3==0);

	std::vector<size_t> *initialIndices = new std::vector<size_t>(0);
	size_t faceCount = indices.size()/3;
	initialIndices->reserve(faceCount);
	for (size_t i=0; i<faceCount; i++){
		initialIndices->push_back(i);
	}

	ConstructAABBTree(mRoot, initialIndices);

	ConstructAABBTreeLevel();

	DrawInitialize();
}


txAABBConstructor::~txAABBConstructor(void)
{
	// Release the AABB tree node

	// Release the AABB draw Box
	for (size_t i=0; i<m_pAABBDrawLevels.size(); i++){
		delete m_pAABBDrawLevels[i];
		m_pAABBDrawLevels[i] = NULL;
	}
}


void txAABBConstructor::ConstructAABBTree(txBoundingBoxNode *&root, 
	std::vector<size_t> *&indices)
{
	if (indices->size()==0){
		root = NULL;
		return;
	}
	if (indices->size()==1){
		root = new txBoundingBoxNode;
		root->m_pIndices = indices;
		ComputeAABB(*indices, root->aabb);
		root->left = NULL;
		root->right = NULL;
		return;
	}

	root = new txBoundingBoxNode;
	root->m_pIndices = indices;
	ComputeAABB(*indices, root->aabb);
	txSplitAxis axis = XAXIS;
	FLOAT value = 0.0;
	ComputeSplitPlane(root->aabb, axis, value);

	std::vector<size_t> *leftIndices = new std::vector<size_t>(0);
	std::vector<size_t> *rightIndices = new std::vector<size_t>(0);
	std::vector<size_t> *middleIndices = new std::vector<size_t>(0);
	
	size_t axisi = axis;
	
	for (size_t casei=0; casei<3; casei++){
		for (size_t i=0; i<indices->size(); i++){
			if (txABS(GetCenterPosition(i,*indices)[axisi] - value)<txEPSILON){
				middleIndices->push_back((*indices)[i]);
			}else if (GetCenterPosition(i,*indices)[axisi]>value){
				rightIndices->push_back((*indices)[i]);
			}else{
				leftIndices->push_back((*indices)[i]);
			}
		}

		if ( (leftIndices->size()>1 && rightIndices->size()==0) ||
			(leftIndices->size()==0 && rightIndices->size()>1) ){
				axisi = (axisi+1)%3;
				value = (root->aabb.max[axisi]+root->aabb.min[axisi])/2.0f;
				leftIndices->clear();
				rightIndices->clear();
				middleIndices->clear();
		}else {
			break;
		}

	}

	root->m_pMiddleIndices = middleIndices;

	ConstructAABBTree(root->left, leftIndices);
	ConstructAABBTree(root->right, rightIndices);

}

void txAABBConstructor::ComputeAABB(const std::vector<size_t> &indices,
	txAABB &aabb)
{
	aabb.min = D3DXVECTOR3(+INFINITY, +INFINITY, +INFINITY);
	aabb.max = D3DXVECTOR3(-INFINITY, -INFINITY, -INFINITY);

	for (size_t i=0; i<indices.size(); i++)
	{
		// The following are using the middle point to compute the 
		// AABB
		//D3DXVECTOR3 centerPosition = GetCenterPosition(i, indices);
		//D3DXVec3Minimize(&aabb.min, &aabb.min, &centerPosition);
		//D3DXVec3Maximize(&aabb.max, &aabb.max, &centerPosition);

		// The following code is using the Triangle as the basement 
		// to calculate the AABB
		D3DXVECTOR3 p0 = m_Vertices[m_Indices[3*indices[i]]];
		D3DXVECTOR3 p1 = m_Vertices[m_Indices[3*indices[i]+1]];
		D3DXVECTOR3 p2 = m_Vertices[m_Indices[3*indices[i]+2]];
		D3DXVec3Minimize(&aabb.min, &aabb.min, &p0);
		D3DXVec3Maximize(&aabb.max, &aabb.max, &p0);
		D3DXVec3Minimize(&aabb.min, &aabb.min, &p1);
		D3DXVec3Maximize(&aabb.max, &aabb.max, &p1);
		D3DXVec3Minimize(&aabb.min, &aabb.min, &p2);
		D3DXVec3Maximize(&aabb.max, &aabb.max, &p2);
	}
}

void txAABBConstructor::ComputeSplitPlane(const txAABB &aabb, 
	txSplitAxis &axis, FLOAT &value)
{
	FLOAT xLength = aabb.max.x - aabb.min.x;
	FLOAT yLength = aabb.max.y - aabb.min.y;
	FLOAT zLength = aabb.max.z - aabb.min.z;

	
	if (xLength>yLength && xLength>zLength){
		axis  = XAXIS;
		value = (aabb.max.x+aabb.min.x)/2.0f;
		return;
	} 

	if (yLength>xLength && yLength>zLength){
		axis = YAXIS;
		value = (aabb.max.y+aabb.min.y)/2.0f;
		return;
	}

	// Then the z
	axis  = ZAXIS;
	value = (aabb.max.z+aabb.min.z)/2.0f;

}

void txAABBConstructor::ConstructAABBTreeLevel()
{
	
	std::vector<txBoundingBoxNode*> pre;
	std::vector<txBoundingBoxNode*> *pPre = &pre;
	std::vector<txBoundingBoxNode*> current;
	std::vector<txBoundingBoxNode*> *pCurrent = &current;

	pre.push_back(mRoot);

	size_t offsetCount = 0;
	m_AABBTreeLevelsOffset.push_back(offsetCount);


	while(pPre->size()){
		
		for (std::vector<txBoundingBoxNode*>::iterator it=pPre->begin();
			it!=pPre->end(); it++){
				m_AABBTreeLevels.push_back(*it);
		}
		offsetCount+=pPre->size();
		m_AABBTreeLevelsOffset.push_back(offsetCount);

		for (size_t i=0; i<pPre->size(); i++){
			if ((*pPre)[i]->left) {
				pCurrent->push_back((*pPre)[i]->left);
			}
			if ((*pPre)[i]->right) {
				pCurrent->push_back((*pPre)[i]->right);
			}
		}

		std::swap(pPre,pCurrent);
		pCurrent->clear();
	}

}

void txAABBConstructor::DrawLevel(size_t index)
{
	assert(index>=0);
	assert(index<m_AABBTreeLevelsOffset.size());
	for (size_t i=m_AABBTreeLevelsOffset[index]; i<m_AABBTreeLevelsOffset[index+1]; i++){
		m_pAABBDrawLevels[i]->draw();
	}

}

void txAABBConstructor::DrawInitialize()
{
	m_pAABBDrawLevels.reserve(m_AABBTreeLevels.size());
	m_AABBDrawLevelOffset.reserve(m_AABBTreeLevelsOffset.size());
	//std::copy(m_AABBTreeLevelsOffset.begin,m_AABBTreeLevelsOffset.end,
	//	std::back_insert(m_AABBDrawLevelOffset));
	
	for (size_t i=0; i<m_AABBTreeLevels.size(); i++){
		Box *pCurrentBox = new Box;
		pCurrentBox->customizeInit(m_PD3Device,m_AABBTreeLevels[i]->aabb.min,
			m_AABBTreeLevels[i]->aabb.max);
		m_pAABBDrawLevels.push_back(pCurrentBox);
	}

}

void txAABBConstructor::SplitTriangleList(const std::vector<size_t> &indices,size_t faceIndex, txSplitAxis &axis, FLOAT &value, txTrianglePlaneRelation &triPlaneRelation)
{
	//D3DXVECTOR3 p0,p1,p2;
	//p0 = m_Vertices[m_Indices[3*indices[faceIndex]]];
	//p1 = m_Vertices[m_Indices[3*indices[faceIndex]+1]];
	//p2 = m_Vertices[m_Indices[3*indices[faceIndex]+2]];


}

void txAABBConstructor::DrawAllAABBDetial()
{
	for (size_t i=0; i<m_pAABBDrawLevels.size(); i++){
		if (m_AABBTreeLevels[i]->left==NULL&&m_AABBTreeLevels[i]->right==NULL){
			m_pAABBDrawLevels[i]->draw();
		}
	}
}



