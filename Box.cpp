//=======================================================================================
// Box.cpp by Frank Luna (C) 2008 All Rights Reserved.
//=======================================================================================
#include "DXUT.h"
#include "Box.h"
#include "Vertex.h"
#include "d3dUtil.h"

Box::Box()
: mNumVertices(0), mNumFaces(0), md3dDevice(0), mVB(0), mIB(0)
{
}
 
Box::~Box()
{
	ReleaseCOM(mVB);
	ReleaseCOM(mIB);
}

void Box::init(ID3D10Device* device, float scale)
{
	md3dDevice = device;
 
	mNumVertices = 24;
	mNumFaces    = 12; // 2 per quad

	// Create vertex buffer
    Vertex vertices[] =
    {
        Vertex( D3DXVECTOR3( -1.0f, 1.0f, -1.0f ), D3DXVECTOR3( 0.0f, 1.0f, 0.0f ) ,D3DXVECTOR2( 0.0f, 0.0f ) ),
        Vertex(  D3DXVECTOR3( 1.0f, 1.0f, -1.0f ), D3DXVECTOR3( 0.0f, 1.0f, 0.0f ) ,D3DXVECTOR2( 1.0f, 0.0f )),
        Vertex(  D3DXVECTOR3( 1.0f, 1.0f, 1.0f ), D3DXVECTOR3( 0.0f, 1.0f, 0.0f ) ,D3DXVECTOR2( 1.0f, 1.0f )),
        Vertex(  D3DXVECTOR3( -1.0f, 1.0f, 1.0f ), D3DXVECTOR3( 0.0f, 1.0f, 0.0f ) ,D3DXVECTOR2( 0.0f, 1.0f )),

        Vertex(  D3DXVECTOR3( -1.0f, -1.0f, -1.0f ), D3DXVECTOR3( 0.0f, -1.0f, 0.0f ) ,D3DXVECTOR2( 0.0f, 0.0f )),
        Vertex(  D3DXVECTOR3( 1.0f, -1.0f, -1.0f ), D3DXVECTOR3( 0.0f, -1.0f, 0.0f ) ,D3DXVECTOR2( 1.0f, 0.0f )),
        Vertex(  D3DXVECTOR3( 1.0f, -1.0f, 1.0f ), D3DXVECTOR3( 0.0f, -1.0f, 0.0f ) ,D3DXVECTOR2( 1.0f, 1.0f )),
        Vertex(  D3DXVECTOR3( -1.0f, -1.0f, 1.0f ), D3DXVECTOR3( 0.0f, -1.0f, 0.0f ) ,D3DXVECTOR2( 0.0f, 1.0f )),

        Vertex( D3DXVECTOR3( -1.0f, -1.0f, 1.0f ), D3DXVECTOR3( -1.0f, 0.0f, 0.0f ) ,D3DXVECTOR2( 0.0f, 0.0f )),
        Vertex(  D3DXVECTOR3( -1.0f, -1.0f, -1.0f ), D3DXVECTOR3( -1.0f, 0.0f, 0.0f ) ,D3DXVECTOR2( 1.0f, 0.0f )),
        Vertex(  D3DXVECTOR3( -1.0f, 1.0f, -1.0f ), D3DXVECTOR3( -1.0f, 0.0f, 0.0f ) ,D3DXVECTOR2( 1.0f, 1.0f )),
        Vertex(  D3DXVECTOR3( -1.0f, 1.0f, 1.0f ), D3DXVECTOR3( -1.0f, 0.0f, 0.0f ) ,D3DXVECTOR2( 0.0f, 1.0f ) ),

        Vertex(  D3DXVECTOR3( 1.0f, -1.0f, 1.0f ), D3DXVECTOR3( 1.0f, 0.0f, 0.0f ) ,D3DXVECTOR2( 0.0f, 0.0f )),
        Vertex(  D3DXVECTOR3( 1.0f, -1.0f, -1.0f ), D3DXVECTOR3( 1.0f, 0.0f, 0.0f ) ,D3DXVECTOR2( 1.0f, 0.0f )),
        Vertex(  D3DXVECTOR3( 1.0f, 1.0f, -1.0f ), D3DXVECTOR3( 1.0f, 0.0f, 0.0f ) ,D3DXVECTOR2( 1.0f, 1.0f )),
        Vertex(  D3DXVECTOR3( 1.0f, 1.0f, 1.0f ), D3DXVECTOR3( 1.0f, 0.0f, 0.0f ) ,D3DXVECTOR2( 0.0f, 1.0f )),

        Vertex(  D3DXVECTOR3( -1.0f, -1.0f, -1.0f ), D3DXVECTOR3( 0.0f, 0.0f, -1.0f ) ,D3DXVECTOR2( 0.0f, 0.0f )),
        Vertex(  D3DXVECTOR3( 1.0f, -1.0f, -1.0f ), D3DXVECTOR3( 0.0f, 0.0f, -1.0f ) ,D3DXVECTOR2( 1.0f, 0.0f )),
        Vertex(  D3DXVECTOR3( 1.0f, 1.0f, -1.0f ), D3DXVECTOR3( 0.0f, 0.0f, -1.0f ) ,D3DXVECTOR2( 1.0f, 1.0f )),
        Vertex(  D3DXVECTOR3( -1.0f, 1.0f, -1.0f ), D3DXVECTOR3( 0.0f, 0.0f, -1.0f ) ,D3DXVECTOR2( 0.0f, 1.0f )),

        Vertex(  D3DXVECTOR3( -1.0f, -1.0f, 1.0f ), D3DXVECTOR3( 0.0f, 0.0f, 1.0f ) ,D3DXVECTOR2( 0.0f, 0.0f ) ),
        Vertex(  D3DXVECTOR3( 1.0f, -1.0f, 1.0f ), D3DXVECTOR3( 0.0f, 0.0f, 1.0f ) ,D3DXVECTOR2( 1.0f, 0.0f )),
        Vertex(  D3DXVECTOR3( 1.0f, 1.0f, 1.0f ), D3DXVECTOR3( 0.0f, 0.0f, 1.0f ) ,D3DXVECTOR2( 1.0f, 1.0f )),
        Vertex(  D3DXVECTOR3( -1.0f, 1.0f, 1.0f ), D3DXVECTOR3( 0.0f, 0.0f, 1.0f ) ,D3DXVECTOR2( 0.0f, 1.0f )),
    };

	// Scale the box.
	for(DWORD i = 0; i < mNumVertices; ++i)
		vertices[i].pos *= scale;


    D3D10_BUFFER_DESC vbd;
    vbd.Usage = D3D10_USAGE_IMMUTABLE;
    vbd.ByteWidth = sizeof(Vertex) * mNumVertices;
    vbd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags = 0;
    vbd.MiscFlags = 0;
    D3D10_SUBRESOURCE_DATA vinitData;
    vinitData.pSysMem = vertices;
    HR(md3dDevice->CreateBuffer(&vbd, &vinitData, &mVB));


	// Create the index buffer

	DWORD indices[] = {
		// front face
		0, 1, 2,
		0, 2, 3,

		// back face
		4, 6, 5,
		4, 7, 6,

		// left face
		4, 5, 1,
		4, 1, 0,

		// right face
		3, 2, 6,
		3, 6, 7,

		// top face
		1, 5, 6,
		1, 6, 2,

		// bottom face
		4, 0, 3, 
		4, 3, 7
	};

		//unsigned int indices[36] = { 	2,0,3,3,1,0,
		//					3,1,7,7,5,1,
		//					6,4,2,2,0,4,
		//					7,5,6,6,4,5,
		//					0,4,1,1,5,4,
		//					6,2,7,7,3,2 };

	D3D10_BUFFER_DESC ibd;
    ibd.Usage = D3D10_USAGE_IMMUTABLE;
    ibd.ByteWidth = sizeof(DWORD) * mNumFaces*3;
    ibd.BindFlags = D3D10_BIND_INDEX_BUFFER;
    ibd.CPUAccessFlags = 0;
    ibd.MiscFlags = 0;
    D3D10_SUBRESOURCE_DATA iinitData;
    iinitData.pSysMem = indices;
    HR(md3dDevice->CreateBuffer(&ibd, &iinitData, &mIB));
}

void Box::customizeInit(ID3D10Device* device,const D3DXVECTOR3 &minAABB, const D3DXVECTOR3 &maxAABB)
{
	md3dDevice = device;
 
	mNumVertices = 24;
	mNumFaces    = 12; // 2 per quad

	D3DXVECTOR3 temp = maxAABB - minAABB;
	
	FLOAT w = temp.x;
	FLOAT l = temp.y;
	FLOAT h = temp.z;

	// Create vertex buffer
    Vertex vertices[] =
    {
        Vertex(  minAABB , D3DXVECTOR3(0.0,-1.0,0.0),D3DXVECTOR2( 0.0f, 0.0f ) ),
        Vertex(  minAABB+D3DXVECTOR3( w,0.0,0.0 ), D3DXVECTOR3(0.0,-1.0,0.0) ,D3DXVECTOR2( 1.0f, 0.0f )),
        Vertex(  minAABB+D3DXVECTOR3( w,0.0,h ), D3DXVECTOR3(0.0,-1.0,0.0) ,D3DXVECTOR2( 1.0f, 1.0f )),
        Vertex(  minAABB+D3DXVECTOR3( 0.0f, 0.0f, h ), D3DXVECTOR3(0.0,-1.0,0.0) ,D3DXVECTOR2( 0.0f, 1.0f )),

        Vertex(  minAABB+D3DXVECTOR3( 0.0f, l, 0.0f ), D3DXVECTOR3( 0.0f, 1.0f, 0.0f ) ,D3DXVECTOR2( 0.0f, 0.0f )),
        Vertex(  minAABB+D3DXVECTOR3( w, l, 0.0f ), D3DXVECTOR3( 0.0f, 1.0f, 0.0f ) ,D3DXVECTOR2( 1.0f, 0.0f )),
        Vertex(  minAABB+D3DXVECTOR3( w, l, h ), D3DXVECTOR3( 0.0f, 1.0f, 0.0f ) ,D3DXVECTOR2( 1.0f, 1.0f )),
        Vertex(  minAABB+D3DXVECTOR3( 0, l, h ), D3DXVECTOR3( 0.0f, 1.0f, 0.0f ) ,D3DXVECTOR2( 0.0f, 1.0f )),

        Vertex(  minAABB, D3DXVECTOR3( -1.0f, 0.0f, 0.0f ) ,D3DXVECTOR2( 0.0f, 0.0f )),
        Vertex(  minAABB+D3DXVECTOR3( 0.0f, l, 0.0f ), D3DXVECTOR3( -1.0f, 0.0f, 0.0f ) ,D3DXVECTOR2( 1.0f, 0.0f )),
        Vertex(  minAABB+D3DXVECTOR3( 0.0f, l, h ), D3DXVECTOR3( -1.0f, 0.0f, 0.0f ) ,D3DXVECTOR2( 1.0f, 1.0f )),
        Vertex(  minAABB+D3DXVECTOR3( 0.0f, 0.0f, h ), D3DXVECTOR3( -1.0f, 0.0f, 0.0f ) ,D3DXVECTOR2( 0.0f, 1.0f ) ),

        Vertex(  minAABB+D3DXVECTOR3( w, 0.0f, 0.0f ), D3DXVECTOR3( 1.0f, 0.0f, 0.0f ) ,D3DXVECTOR2( 0.0f, 0.0f )),
        Vertex(  minAABB+D3DXVECTOR3( w, l, 0.0f ), D3DXVECTOR3( 1.0f, 0.0f, 0.0f ) ,D3DXVECTOR2( 1.0f, 0.0f )),
        Vertex(  minAABB+D3DXVECTOR3( w, l, h ), D3DXVECTOR3( 1.0f, 0.0f, 0.0f ) ,D3DXVECTOR2( 1.0f, 1.0f )),
        Vertex(  minAABB+D3DXVECTOR3( w, 0, h ), D3DXVECTOR3( 1.0f, 0.0f, 0.0f ) ,D3DXVECTOR2( 0.0f, 1.0f )),

        Vertex(  minAABB+D3DXVECTOR3( 0.0f, 0.0f, h ), D3DXVECTOR3( 0.0f, 0.0f, 1.0f ) ,D3DXVECTOR2( 0.0f, 0.0f )),
        Vertex(  minAABB+D3DXVECTOR3( w, 0.0f, h ), D3DXVECTOR3( 0.0f, 0.0f, 1.0f ) ,D3DXVECTOR2( 1.0f, 0.0f )),
        Vertex(  minAABB+D3DXVECTOR3( w, l, h ), D3DXVECTOR3( 0.0f, 0.0f, 1.0f ) ,D3DXVECTOR2( 1.0f, 1.0f )),
        Vertex(  minAABB+D3DXVECTOR3( 0.0f, l, h ), D3DXVECTOR3( 0.0f, 0.0f, 1.0f ) ,D3DXVECTOR2( 0.0f, 1.0f )),

        Vertex(  minAABB, D3DXVECTOR3( 0.0f, 0.0f, -1.0f ) ,D3DXVECTOR2( 0.0f, 0.0f ) ),
        Vertex(  minAABB+D3DXVECTOR3( w, 0.0f, 0.0f ), D3DXVECTOR3( 0.0f, 0.0f, -1.0f ) ,D3DXVECTOR2( 1.0f, 0.0f )),
        Vertex(  minAABB+D3DXVECTOR3( w, l, 0.0f ), D3DXVECTOR3( 0.0f, 0.0f, -1.0f ) ,D3DXVECTOR2( 1.0f, 1.0f )),
        Vertex(  minAABB+D3DXVECTOR3( 0.0f, l, 0.0f ), D3DXVECTOR3( 0.0f, 0.0f, -1.0f ) ,D3DXVECTOR2( 0.0f, 1.0f )),
    };


	FLOAT scale =1.0;
	// Scale the box.
	for(DWORD i = 0; i < mNumVertices; ++i)
		vertices[i].pos *= scale;


    D3D10_BUFFER_DESC vbd;
    vbd.Usage = D3D10_USAGE_IMMUTABLE;
    vbd.ByteWidth = sizeof(Vertex) * mNumVertices;
    vbd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags = 0;
    vbd.MiscFlags = 0;
    D3D10_SUBRESOURCE_DATA vinitData;
    vinitData.pSysMem = vertices;
    HR(md3dDevice->CreateBuffer(&vbd, &vinitData, &mVB));


	// Create the index buffer

	DWORD indices[] = {
		// front face
		1,3,0,
		1,2,3,

		// back face
		7,6,5,
		7,5,4,

		// left face
		11,9,8,
		10,9,11,

		// right face
		12,13,15,
		13,14,15,

		// top face
		16,17,19,
		17,18,19,

		// bottom face
		20,23,21,
		23,22,21
	};

		//unsigned int indices[36] = { 	2,0,3,3,1,0,
		//					3,1,7,7,5,1,
		//					6,4,2,2,0,4,
		//					7,5,6,6,4,5,
		//					0,4,1,1,5,4,
		//					6,2,7,7,3,2 };

	D3D10_BUFFER_DESC ibd;
    ibd.Usage = D3D10_USAGE_IMMUTABLE;
    ibd.ByteWidth = sizeof(DWORD) * mNumFaces*3;
    ibd.BindFlags = D3D10_BIND_INDEX_BUFFER;
    ibd.CPUAccessFlags = 0;
    ibd.MiscFlags = 0;
    D3D10_SUBRESOURCE_DATA iinitData;
    iinitData.pSysMem = indices;
    HR(md3dDevice->CreateBuffer(&ibd, &iinitData, &mIB));
}
	

void Box::draw()
{
	UINT stride = sizeof(Vertex);
    UINT offset = 0;
	if (mIB){
    md3dDevice->IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
	md3dDevice->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);
	md3dDevice->DrawIndexed(mNumFaces*3, 0, 0);
	}
}