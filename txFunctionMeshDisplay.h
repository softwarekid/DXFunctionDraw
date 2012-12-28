#pragma once
class txFunctionMeshDisplay
{
public:
	txFunctionMeshDisplay(void);
	~txFunctionMeshDisplay(void);

	void init(FLOAT xMin=-10.0f,  FLOAT xMax=10.0f, DWORD xGridCount=100,
		FLOAT yMin=-10.0f, FLOAT yMax=10.0f, DWORD yGridCount=100
		// Need to add a call back function to calculate z
		);

	void DrawFunction();

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
};

