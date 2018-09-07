#ifndef __LIST_H__
#define __LIST_H__

struct Fish 
{
	int m_X;
};

struct Door
{
	int m_nLeft;
	int m_nRight;
	int m_nTop;
	int m_nBottom;

	Door *m_pLast;
	Door *m_pNext;
};

struct Enemy
{
	int m_nX;
	int m_nY;
	int m_nLife;
	
	Enemy *m_pNext;
	Enemy *m_pLast;
};

#endif