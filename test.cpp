#include "pch.h"

#include <iostream>

#include <assert.h>
#include <gtest/gtest.h>

typedef long long INT_PTR;
typedef unsigned short WORD;
typedef unsigned char BYTE;
class CWordArray
{
public:
	CWordArray();
	INT_PTR Add(WORD newElement);
	INT_PTR GetSize() const;
	WORD GetAt(INT_PTR nIndex) const;
	void RemoveAll();
	void SetAtGrow(INT_PTR nIndex, WORD newElement);
	void SetSize(INT_PTR nNewSize, INT_PTR nGrowBy = -1);
	INT_PTR min(INT_PTR a, INT_PTR b);
	INT_PTR max(INT_PTR a, INT_PTR b);


private:

	WORD* m_pData;    
	INT_PTR m_nSize;    
	INT_PTR m_nMaxSize; 
	INT_PTR m_nGrowBy;  
};

CWordArray::CWordArray()
{
	m_pData = NULL;
	m_nSize = m_nMaxSize = m_nGrowBy = 0;
}

INT_PTR CWordArray::Add(WORD newElement)
{
	INT_PTR nIndex = m_nSize;
	SetAtGrow(nIndex, newElement);
	return nIndex;
}

INT_PTR CWordArray::GetSize() const
{
	return m_nSize;
}

WORD CWordArray::GetAt(INT_PTR nIndex) const
{
	assert(nIndex >= 0 && nIndex < m_nSize);
	return m_pData[nIndex];
}

void CWordArray::SetAtGrow(INT_PTR nIndex, WORD newElement)
{
	assert(nIndex >= 0);

	if (nIndex >= m_nSize)
		SetSize(nIndex + 1);
	m_pData[nIndex] = newElement;

}

void CWordArray::RemoveAll()
{
	SetSize(0);
}


void CWordArray::SetSize(INT_PTR nNewSize, INT_PTR nGrowBy )
{
	if (nNewSize < 0)
		return;

	if (nGrowBy >= 0)

		m_nGrowBy = nGrowBy;

	if (nNewSize == 0)
	{
		delete[] m_pData;
		m_pData = NULL;
		m_nSize = m_nMaxSize = 0;
	}

	else if (m_pData == NULL)
	{
		m_pData = (WORD*) new BYTE[nNewSize * sizeof(WORD)];
		memset(m_pData, 0, nNewSize * sizeof(WORD));
		m_nSize = m_nMaxSize = nNewSize;
	}

	else if (nNewSize <= m_nMaxSize)
	{
		if (nNewSize > m_nSize)
		{
			memset(&m_pData[m_nSize], 0, (nNewSize - m_nSize) * sizeof(WORD));
			m_nSize = nNewSize;
		}
		else
			return;
		
	}

	else
	{
		INT_PTR nGrowArrayBy = m_nGrowBy;
		if (nGrowArrayBy == 0)
		{
			nGrowArrayBy = min(1024, max(4, m_nSize / 8));
		}

		INT_PTR nNewMax;
		if (nNewSize < m_nMaxSize + nGrowArrayBy)
			nNewMax = m_nMaxSize + nGrowArrayBy;
		else
			nNewMax = nNewSize;
		//assert(nNewMax >= m_nMaxSize);

		//assert(nNewSize > m_nSize);

		WORD* pNewData = (WORD*) new BYTE[nNewMax * sizeof(WORD)];

		memcpy(pNewData, m_pData, m_nSize * sizeof(WORD));
		memset(&pNewData[m_nSize], 0, (nNewSize - m_nSize) * sizeof(WORD));

		delete[] m_pData;
		m_pData = pNewData;
		m_nSize = nNewSize;
		m_nMaxSize = nNewMax;
	}
}



INT_PTR CWordArray::min(INT_PTR a, INT_PTR b)
{
	return a < b ? a : b;
}

INT_PTR CWordArray::max(INT_PTR a, INT_PTR b)
{
	return a > b ? a : b;
}

TEST(CWordArray, setSizeLT0)
{
	CWordArray cwa;
	cwa.SetSize(-5); // newSize < 0
	EXPECT_EQ(0, cwa.GetSize());
	
}
TEST(CWordArray, setSizeGTMaxSize)
{
	CWordArray cwa;
	EXPECT_EQ(0, cwa.GetSize());
	cwa.SetSize(7); // newSize > maxSize
	EXPECT_EQ(0, cwa.GetAt(1));
	EXPECT_EQ(7, cwa.GetSize());
}

TEST(CWordArray, setSizeLTSize)
{
	CWordArray cwa;
	EXPECT_EQ(0, cwa.GetSize());
	cwa.SetSize(7); // newSize > maxSize
	EXPECT_EQ(0, cwa.GetAt(1));
	EXPECT_EQ(7, cwa.GetSize());
	cwa.SetSize(5); // newSize < Size
	EXPECT_EQ(7, cwa.GetSize());
}

TEST(CWordArray, setSizeEQ0)
{
	CWordArray cwa;
	EXPECT_EQ(0, cwa.GetSize());
	cwa.SetSize(7); // newSize > maxSize
	cwa.SetSize(0);// newSize = 0
	EXPECT_EQ(0, cwa.GetSize());
}

TEST(CWordArray, setSizeGrow)
{
	CWordArray cwa;
	cwa.SetSize(7, 4);
	EXPECT_EQ(7, cwa.GetSize());
	cwa.SetSize(9); // Size < newSize < maxSize
	EXPECT_EQ(9, cwa.GetSize());
	EXPECT_EQ(0, cwa.GetAt(8));
}


TEST(CWordArray, setAtGrowLTESize)
{
	CWordArray cwa;
	cwa.SetSize(7);
	cwa.SetAtGrow(6, 8); // index < size
	EXPECT_EQ(8, cwa.GetAt(6));
	EXPECT_EQ(7, cwa.GetSize()); 
	
}

TEST(CWordArray, setAtGrowGTSize)
{
	CWordArray cwa;
	cwa.SetSize(7);
	cwa.SetAtGrow(10, 9);// index >= size
	EXPECT_EQ(0, cwa.GetAt(8));
	EXPECT_EQ(9, cwa.GetAt(10));
}

TEST(CWordArray, Add)
{
	CWordArray cwa;
	cwa.Add(10);
	cwa.Add(5);
	cwa.Add(20);
	EXPECT_EQ(3, cwa.Add(15));
}

TEST(CWordArray, getSize)
{
	CWordArray cwa;
	EXPECT_EQ(0, cwa.GetSize()); // Constructor
	cwa.Add(10);
	cwa.Add(5);
	cwa.Add(20);
	EXPECT_EQ(3, cwa.GetSize());
}

TEST(CWordArray, removeAll)
{
	CWordArray cwa;
	cwa.Add(10);
	cwa.Add(5);
	cwa.Add(20);
	cwa.RemoveAll();
	EXPECT_EQ(0, cwa.GetSize());
}


int main(int argc, char** argv)
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();

}

