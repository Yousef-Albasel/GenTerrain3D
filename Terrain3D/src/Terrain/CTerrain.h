#pragma once
struct SHEIGHT_DATA
{
	unsigned char* m_pucData; //the height data
	int m_iSize; //the height size (power of 2)
};

class CTerrain {
protected:
	SHEIGHT_DATA m_heightData;
	float m_fHeightScale;		// Scaling Variable

public:
	int m_iSize;
	virtual void Render(void) = 0;
	bool LoadHeightMap(char* szFileName, int iSize);
	bool SaveHeightMap(char* szFileName);

	inline void SetHeightScale(float fScale) // Set the height scaling variable
	{
		m_fHeightScale = fScale;
	}

	inline void SetHeightAtPoint(unsigned char ucHeight,int iX, int iZ) // set height for a specific point
	{
		m_heightData.m_pucData[(iZ * m_iSize) + iX] = ucHeight;
	}

	inline unsigned char GetTrueHeightAtPoint(int iX, int iZ) // gets the height value at a point
	{ 
		return (m_heightData.m_pucData[(iZ * m_iSize) + iX]);
	}

	inline float GetScaledHeightAtPoint(int iX, int iZ)
	{
		return ((m_heightData.m_pucData[(iZ * m_iSize) + iX]
			) * m_fHeightScale);
	}
	CTerrain(void)
	{ };
	~CTerrain(void)
	{ };





};