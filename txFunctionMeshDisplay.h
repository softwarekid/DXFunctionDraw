#pragma once

class Box;

class txFunctionMeshDisplay
{
public:
	txFunctionMeshDisplay(ID3D10Device* pD3DDevice);
	~txFunctionMeshDisplay(void);

	void init(FLOAT xMin=-2.0f,  FLOAT xMax=2.0f, DWORD xGridCount=300,
		FLOAT yMin=-2.0f, FLOAT yMax=2.0f, DWORD yGridCount=300
		// Need to add a call back function to calculate z
		);

	void DrawFunction();

	void DrawBoundingBox();

private:
	FLOAT CalculateZ(FLOAT x, FLOAT y);

private:
	FLOAT m_XMin;
	FLOAT m_XMax;
	FLOAT m_YMin;
	FLOAT m_YMax;
	DWORD m_XGridCount;
	DWORD m_YGridCount;

	DWORD m_NumVertices;
	DWORD m_NumFaces;

	ID3D10Device* md3dDevice;
	ID3D10Buffer* mVB;
	ID3D10Buffer* mIB;

	// for debug
	Box * m_DebugBox;
};

