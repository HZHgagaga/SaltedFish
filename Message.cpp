#include "StdAfx.h"
#include "Message.h"
#include "List.h"
#include "resource.h"

#define STAY 0
#define MOVE 1

int g_nMoney = 0;
Door *g_pHead;
int g_nEnTime = 80;
Door *g_pEnd;
Door *g_LHead;
Door *g_LEnd;
Door *g_RHead;
Door *g_REnd;
Enemy *g_eHead;
Enemy *g_eEnd;
Enemy *g_pBoss;
Enemy *g_cBoss;
int nLife = 1;
int XStime = 0;
int HStime = 0;
int Xtime = 0;
int Htime = 0;
int nBoss = 0;
RECT g_reRect;
HBITMAP hbmpB;
int nB = 0;
int g_nHard = 3;
int g_nBlife = 1000;
int g_nTime = 6000;
int nnn = 0;
Fish FishKing;
Fish *FishL;
Fish *FishR;
BITMAP bmp;
int nMove;
int g_nX = 900;
int g_nY = 900;
int g_ncX = 900;
int g_ncY = 900;
BOOL g_bL = FALSE;
BOOL g_Skill = FALSE;

void PaintBackground(HDC hMem, HDC hmap);
void PaintFish(HDC hMem, HDC hmap);
void PaintDoor(HDC hMem, HDC hmap);
void PaintEnemy(HDC hMem, HDC hmap);
void PaintMoney(HDC hMem, HDC hmap);
void PaintBoss(HDC hMem, HDC hmap);
void PaintChuan(HDC hMem, HDC hmap);

MSG_MAP g_msg[] = { {WM_CREATE, OnCREATE},
					{WM_COMMAND, OnCOMMAND},
					{WM_KEYUP, OnKEYUP},
					{WM_KEYDOWN, OnKEYDOWN},
					{WM_MOUSEMOVE, OnMOUSEMOVE},
					{WM_PAINT, OnPAINT},
					{WM_CLOSE, OnCLOSE},
					{WM_DESTROY, OnDESTROY},
					{WM_NULL, NULL}};


void CreateLe(int nX)
{
	Door *dNew = new Door;
	dNew->m_pLast = NULL;
	dNew->m_pNext = NULL;
	g_LHead = dNew;
	g_LEnd = dNew;		
	dNew->m_nLeft = nX;
	dNew->m_nRight = dNew->m_nLeft + 30;
	dNew->m_nTop = 480;
	dNew->m_nBottom = 510;
}

void CreateRi(int nX)
{
	Door *dNew1 = new Door;
	dNew1->m_pLast = NULL;
	dNew1->m_pNext = NULL;
	g_RHead = dNew1;
	g_REnd = dNew1;		
	dNew1->m_nLeft = nX;
	dNew1->m_nRight = dNew1->m_nLeft + 30;
	dNew1->m_nTop = 480;
	dNew1->m_nBottom = 510;
}

void AddSkill()
{
	if (NULL == FishL && NULL == FishR)
	{
		Fish *pNewL = new Fish;
		Fish *pNewR = new Fish;
		pNewL->m_X = FishKing.m_X - 30;
		pNewR->m_X = FishKing.m_X + 100;
		FishL = pNewL;
		FishR = pNewR;

		CreateLe(pNewL->m_X);
		CreateRi(pNewR->m_X);
	}
}

void LeftDoorAdd(HWND hWnd)
{
	Door *dNew = new Door;
	dNew->m_pNext = NULL;
	dNew->m_pLast = g_LEnd;
	g_LEnd->m_pNext = dNew;
	g_LEnd = dNew;
	dNew->m_nLeft = FishL->m_X;
	dNew->m_nRight = dNew->m_nLeft + 30;
	dNew->m_nTop = 480;
	dNew->m_nBottom = 510;
	
	InvalidateRect(hWnd, NULL, FALSE);
}

void RightDoorAdd(HWND hWnd)
{
	Door *dNew = new Door;
	dNew->m_pNext = NULL;
	dNew->m_pLast = g_REnd;
	g_REnd->m_pNext = dNew;
	g_REnd = dNew;
	dNew->m_nLeft = FishR->m_X;
	dNew->m_nRight = dNew->m_nLeft + 30;
	dNew->m_nTop = 480;
	dNew->m_nBottom = 510;
	
	InvalidateRect(hWnd, NULL, FALSE);
}

void ClearLeft()
{
	if (NULL != g_LHead)
	{
		Door *pTemp = g_LHead;
		Door *pTempNext= NULL;
		
		while (NULL != pTemp)
		{
			pTempNext= pTemp->m_pNext;
			delete pTemp;
			pTemp = pTempNext;
		}
		
		g_LHead = NULL;
		g_LEnd = NULL;
	}
}

void ClearRight()
{
	if (NULL != g_RHead)
	{
		Door *pTemp = g_RHead;
		Door *pTempNext= NULL;
		
		while (NULL != pTemp)
		{
			pTempNext= pTemp->m_pNext;
			delete pTemp;
			pTemp = pTempNext;
		}
		
		g_RHead = NULL;
		g_REnd = NULL;
	}
}

void ClearEnemy()
{
	Enemy *pTemp = g_eHead;
	Enemy *pTempNext = NULL;
	
	while (NULL != pTemp)
	{
		pTempNext= pTemp->m_pNext;
		delete pTemp;
		pTemp = pTempNext;
	}
	
	g_eHead = NULL;
	g_eEnd = NULL;
}

void ClearAll()
{
	Door *pTemp = g_pHead;
	Door *pTempNext= NULL;
	
	while (NULL != pTemp)
	{
		pTempNext= pTemp->m_pNext;
		delete pTemp;
		pTemp = pTempNext;
	}
	
	g_pHead = NULL;
	g_pEnd = NULL;
	
	ClearLeft();
	ClearRight();
	ClearEnemy();
}

void CreateOur(HWND hWnd)
{
	FishKing.m_X = 30;
	
	Door *dNew = new Door;
	dNew->m_pLast = NULL;
	dNew->m_pNext = NULL;
	g_pHead = dNew;
	g_pEnd = dNew;
	
	dNew->m_nLeft = FishKing.m_X + 30;
	dNew->m_nRight = dNew->m_nLeft;
	dNew->m_nTop = 480;
	dNew->m_nBottom = 510;
}

void CreateEnemy(HWND hWnd)
{
	if (NULL == g_eHead)
	{
		RECT reRect = {0};
		GetClientRect(hWnd, &reRect);
		Enemy *pNew = new Enemy;
		pNew->m_nLife = (rand() % 30) + nLife;
		pNew->m_pNext = NULL;
		pNew->m_pLast = NULL;
		g_eHead = pNew;
		g_eEnd = pNew;
		pNew->m_nX = (rand() % (((reRect.right - 70) - reRect.left) / 50)) * 50;
		pNew->m_nY = 0;
	}
}

void DoorAdd(HWND hWnd)
{
	Door *dNew = new Door;
	dNew->m_pNext = NULL;
	dNew->m_pLast = g_pEnd;
	g_pEnd->m_pNext = dNew;
	g_pEnd = dNew;
	dNew->m_nLeft = FishKing.m_X + 30;
	dNew->m_nRight = dNew->m_nLeft + 10;
	dNew->m_nTop = 480;
	dNew->m_nBottom = 510;
	
	InvalidateRect(hWnd, NULL, FALSE);
}

void BossAdd(HWND hWnd)
{
	Enemy *pNew = new Enemy;
	pNew->m_nX = 380;
	pNew->m_nY = -20;
	pNew->m_pLast = NULL;
	pNew->m_pNext = NULL;
	pNew->m_nLife = 5 * g_nBlife;
	g_pBoss = pNew;
	hbmpB = (HBITMAP)LoadImage(NULL, "xiangjiaojiao.bmp", IMAGE_BITMAP, 200, 200, LR_LOADFROMFILE|LR_CREATEDIBSECTION);
}

void ChuanpuAdd(HWND hWnd)
{
	Enemy *pNew = new Enemy;
	pNew->m_nX = 380;
	pNew->m_nY = -20;
	pNew->m_pLast = NULL;
	pNew->m_pNext = NULL;
	pNew->m_nLife = 5 * g_nBlife;
	g_cBoss = pNew;
}

void EnemyAdd(HWND hWnd)
{
	if (NULL == g_eHead)
	{
		CreateEnemy(hWnd);
		return;
	}

	RECT reRect = {0};
	GetClientRect(hWnd, &reRect);
	Enemy *pNew = new Enemy;
	pNew->m_nLife = 1;
	pNew->m_pNext = NULL;
	pNew->m_pLast = NULL;
	pNew->m_pLast = g_eEnd;
	pNew->m_pNext = NULL;
	g_eEnd->m_pNext = pNew;
	g_eEnd = pNew;
	pNew->m_nX = (rand() % (reRect.right - 50) - reRect.left);
	pNew->m_nY = 0;
}

void CALLBACK EnemoProc(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	Enemy *pTemp = g_eHead;
	Enemy *pDe = NULL;
	
	while (NULL != pTemp)
	{
		pTemp->m_nY += g_nHard;

		if (pTemp->m_nY > 650)
		{
			nnn++;
		}

		pDe = pTemp;
		pTemp = pTemp->m_pNext;
	}
	
	InvalidateRect(hWnd, NULL, FALSE);
}

void DoorMove(HWND hWnd, Door *pTemp)
{
	Door *pDe = NULL;

	while (NULL != pTemp)
	{
		pTemp->m_nTop -= 10;
		pTemp->m_nBottom -= 10;
		Enemy *eTemp = g_eHead;

		if (NULL != g_pBoss && pTemp->m_nTop < (g_pBoss->m_nY + 200) && pTemp->m_nLeft > g_pBoss->m_nX - 25 
			&& pTemp->m_nRight < (g_pBoss->m_nX + 200))
		{
			g_pBoss->m_nLife = g_pBoss->m_nLife - 1;
			g_nX = g_pBoss->m_nX;
			g_nY = g_pBoss->m_nY;
			
			if (0 > g_pBoss->m_nLife)
			{
				g_nMoney += 100;
				delete g_pBoss;
				g_pBoss = NULL;
				g_nHard += 2;
				g_nTime = 6000;
				g_nBlife = g_nBlife * 2;
				g_bL = TRUE;
				nB = 0;

				if (g_nEnTime > 20)
				{
					g_nEnTime -= 10;
				}

				if (!g_Skill && NULL == g_RHead && NULL == g_REnd)
				{
					AddSkill();
					g_Skill = TRUE;
				}

				break;
			}
		}

		if (NULL != g_cBoss && pTemp->m_nTop < (g_cBoss->m_nY + 250) && pTemp->m_nLeft > g_cBoss->m_nX - 25 
			&& pTemp->m_nRight < (g_cBoss->m_nX + 250))
		{
			g_cBoss->m_nLife = g_cBoss->m_nLife - 1;
			g_ncX = g_cBoss->m_nX;
			g_ncY = g_cBoss->m_nY;
			
			if (0 > g_cBoss->m_nLife)
			{
				g_nMoney += 100;
				delete g_cBoss;
				g_cBoss = NULL;
				break;
			}
		}

		while (NULL != eTemp)
		{
			if (pTemp->m_nTop < eTemp->m_nY + 50 && pTemp->m_nLeft > eTemp->m_nX - 25 && pTemp->m_nRight < (eTemp->m_nX + 50) )
			{
				g_nX = pTemp->m_nLeft;
				g_nY = pTemp->m_nTop;
				g_nMoney++;
				nBoss++;

				if (eTemp == g_eHead)
				{
					if (g_eHead == g_eEnd)
					{
						g_eHead = NULL;
						g_eEnd = NULL;
						delete eTemp;

						if (pTemp == g_pHead)
						{
							g_pHead = pTemp->m_pNext;
						}
						else
						{
							pTemp->m_pLast->m_pNext = pTemp->m_pNext;
							
							if (pTemp == g_pEnd)
							{
								pTemp->m_pLast->m_pNext = NULL;
								g_pEnd = pTemp->m_pLast;
								break;
							}
							
							pTemp->m_pNext->m_pLast = pTemp->m_pLast;
						}

						break;
					}

					eTemp->m_pNext->m_pLast = NULL;
					g_eHead = eTemp->m_pNext;
					delete eTemp;

					if (pTemp == g_pHead)
					{
						g_pHead = pTemp->m_pNext;
					}
					else
					{
						pTemp->m_pLast->m_pNext = pTemp->m_pNext;
						
 						if (pTemp == g_pEnd)
 						{
							pTemp->m_pLast->m_pNext = NULL;
 							g_pEnd = pTemp->m_pLast;
 							break;
 						}
						
						pTemp->m_pNext->m_pLast = pTemp->m_pLast;
					}

					break;
				}
				else
				{
					eTemp->m_pLast->m_pNext = eTemp->m_pNext;
					
					if (eTemp == g_eEnd)
					{
						eTemp->m_pLast->m_pNext = NULL;
						g_eEnd = eTemp->m_pLast;
						delete eTemp;

						if (pTemp == g_pHead)
						{
							g_pHead = pTemp->m_pNext;
						}
						else
						{
							pTemp->m_pLast->m_pNext = pTemp->m_pNext;
							
							if (pTemp == g_pEnd)
							{
								pTemp->m_pLast->m_pNext = NULL;
								g_pEnd = pTemp->m_pLast;
								break;
							}
							
							pTemp->m_pNext->m_pLast = pTemp->m_pLast;
						}
	
						break;
					}

					eTemp->m_pNext->m_pLast = eTemp->m_pLast;
					delete eTemp;

					if (pTemp == g_pHead)
					{
						g_pHead = pTemp->m_pNext;
					}
					else
					{
						pTemp->m_pLast->m_pNext = pTemp->m_pNext;

 						if (pTemp == g_pEnd)
 						{
							pTemp->m_pLast->m_pNext = NULL;
 							g_pEnd = pTemp->m_pLast;
 							break;
 						}

						pTemp->m_pNext->m_pLast = pTemp->m_pLast;
					}

					break;
				}
			}

			eTemp = eTemp->m_pNext;
		}

		pTemp = pTemp->m_pNext;
	}
}

void DeleteTop(Door *pTempTemp)
{
	while (NULL != pTempTemp)
	{
		if (pTempTemp->m_nBottom < 0)
		{
			if (pTempTemp == g_pHead)
			{
				g_pHead = pTempTemp->m_pNext;
			}
			else
			{
				pTempTemp->m_pLast->m_pNext = pTempTemp->m_pNext;
				
				if (pTempTemp == g_pEnd)
				{
					pTempTemp->m_pLast->m_pNext = NULL;
					g_pEnd = pTempTemp->m_pLast;
					delete pTempTemp;
					break;
				}
				
				pTempTemp->m_pNext->m_pLast = pTempTemp->m_pLast;
			}
		}

		pTempTemp = pTempTemp->m_pNext;
	}

	delete pTempTemp;
}

void LeftDeleteTop(Door *pTempTemp)
{
	while (NULL != pTempTemp)
	{
		if (pTempTemp->m_nBottom < 0)
		{
			if (pTempTemp == g_LHead)
			{
				g_LHead = pTempTemp->m_pNext;
			}
			else
			{
				pTempTemp->m_pLast->m_pNext = pTempTemp->m_pNext;
				
				if (pTempTemp == g_LEnd)
				{
					pTempTemp->m_pLast->m_pNext = NULL;
					g_LEnd = pTempTemp->m_pLast;
					delete pTempTemp;
					break;
				}
				
				pTempTemp->m_pNext->m_pLast = pTempTemp->m_pLast;
			}
		}
		
		pTempTemp = pTempTemp->m_pNext;
	}
	
	delete pTempTemp;
}

void RightDeleteTop(Door *pTempTemp)
{
	while (NULL != pTempTemp)
	{
		if (pTempTemp->m_nBottom < 0)
		{
			if (pTempTemp == g_RHead)
			{
				g_RHead = pTempTemp->m_pNext;
			}
			else
			{
				pTempTemp->m_pLast->m_pNext = pTempTemp->m_pNext;
				
				if (pTempTemp == g_REnd)
				{
					pTempTemp->m_pLast->m_pNext = NULL;
					g_REnd = pTempTemp->m_pLast;
					delete pTempTemp;
					break;
				}
				
				pTempTemp->m_pNext->m_pLast = pTempTemp->m_pLast;
			}
		}
		
		pTempTemp = pTempTemp->m_pNext;
	}
	
	delete pTempTemp;
}

void CALLBACK DoorProc(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	Door *pTemp = g_pHead;
	Door *LTemp = g_LHead;
	Door *RTemp = g_RHead;
	DoorMove(hWnd, pTemp);	

	while (NULL != LTemp)
	{
		LTemp->m_nTop -= 10;
		LTemp->m_nBottom -= 10;
		Enemy *eTemp = g_eHead;

		if (NULL != g_pBoss && LTemp->m_nTop < (g_pBoss->m_nY + 200) && LTemp->m_nLeft > g_pBoss->m_nX - 25 
			&& LTemp->m_nRight < (g_pBoss->m_nX + 200))
		{
			g_pBoss->m_nLife = g_pBoss->m_nLife - 1;
			g_nX = g_pBoss->m_nX;
			g_nY = g_pBoss->m_nY;
			
			if (0 > g_pBoss->m_nLife)
			{
				g_nMoney += 100;
				delete g_pBoss;
				g_pBoss = NULL;
				g_nHard += 1;
				g_nTime = 6000;
				g_nBlife = g_nBlife * 2;
				g_bL = TRUE;
				nB = 0;
				break;
			}
		}

		if (NULL != g_cBoss && LTemp->m_nTop < (g_cBoss->m_nY + 250) && LTemp->m_nLeft > g_cBoss->m_nX - 25 
			&& LTemp->m_nRight < (g_cBoss->m_nX + 250))
		{
			g_cBoss->m_nLife = g_cBoss->m_nLife - 1;
			g_ncX = g_cBoss->m_nX;
			g_ncY = g_cBoss->m_nY;
			
			if (0 > g_cBoss->m_nLife)
			{
				g_nMoney += 100;
				delete g_cBoss;
				g_cBoss = NULL;
				break;
			}
		}

		while (NULL != eTemp)
		{
			if (LTemp->m_nTop < eTemp->m_nY + 50 && LTemp->m_nLeft > eTemp->m_nX - 25 && LTemp->m_nRight < (eTemp->m_nX + 50) )
			{
				g_nX = LTemp->m_nLeft;
				g_nY = LTemp->m_nTop;
				g_nMoney++;
				nBoss++;

				if (eTemp == g_eHead)
				{
					if (g_eHead == g_eEnd)
					{
						g_eHead = NULL;
						g_eEnd = NULL;
						delete eTemp;

						if (LTemp == g_LHead)
						{
							g_LHead = LTemp->m_pNext;
						}
						else
						{
							LTemp->m_pLast->m_pNext = LTemp->m_pNext;
							
							if (LTemp == g_LEnd)
							{
								LTemp->m_pLast->m_pNext = NULL;
								g_LEnd = LTemp->m_pLast;
								break;
							}
							
							LTemp->m_pNext->m_pLast = LTemp->m_pLast;
						}

						break;
					}

					eTemp->m_pNext->m_pLast = NULL;
					g_eHead = eTemp->m_pNext;
					delete eTemp;

					if (LTemp == g_LHead)
					{
						g_LHead = LTemp->m_pNext;
					}
					else
					{
						LTemp->m_pLast->m_pNext = LTemp->m_pNext;
						
 						if (LTemp == g_LEnd)
 						{
							LTemp->m_pLast->m_pNext = NULL;
 							g_LEnd = LTemp->m_pLast;
 							break;
 						}
						
						LTemp->m_pNext->m_pLast = LTemp->m_pLast;
					}

					break;
				}
				else
				{
					eTemp->m_pLast->m_pNext = eTemp->m_pNext;
					
					if (eTemp == g_eEnd)
					{
						eTemp->m_pLast->m_pNext = NULL;
						g_eEnd = eTemp->m_pLast;
						delete eTemp;

						if (LTemp == g_LHead)
						{
							g_LHead = LTemp->m_pNext;
						}
						else
						{
							LTemp->m_pLast->m_pNext = LTemp->m_pNext;
							
							if (LTemp == g_LEnd)
							{
								LTemp->m_pLast->m_pNext = NULL;
								g_LEnd = LTemp->m_pLast;
								break;
							}
							
							LTemp->m_pNext->m_pLast = LTemp->m_pLast;
						}

						break;
					}

					eTemp->m_pNext->m_pLast = eTemp->m_pLast;
					delete eTemp;

					if (LTemp == g_LHead)
					{
						g_LHead = LTemp->m_pNext;
					}
					else
					{
						LTemp->m_pLast->m_pNext = LTemp->m_pNext;

						if (LTemp == g_LEnd)
						{
							LTemp->m_pLast->m_pNext = NULL;
							g_LEnd = LTemp->m_pLast;
							break;
						}

						LTemp->m_pNext->m_pLast = LTemp->m_pLast;
					}

					break;
				}
			}

			eTemp = eTemp->m_pNext;
		}

		LTemp = LTemp->m_pNext;
	}

	while (NULL != RTemp)
	{
		RTemp->m_nTop -= 10;
		RTemp->m_nBottom -= 10;
		Enemy *eTemp = g_eHead;

		if (NULL != g_pBoss && RTemp->m_nTop < (g_pBoss->m_nY + 200) && RTemp->m_nLeft > g_pBoss->m_nX - 25 
			&& RTemp->m_nRight < (g_pBoss->m_nX + 200))
		{
			g_pBoss->m_nLife = g_pBoss->m_nLife - 1;
			g_nX = g_pBoss->m_nX;
			g_nY = g_pBoss->m_nY;
			
			if (0 > g_pBoss->m_nLife)
			{
				g_nMoney += 100;
				delete g_pBoss;
				g_pBoss = NULL;
				g_nHard += 1;
				g_nTime = 6000;
				g_nBlife = g_nBlife * 2;
				g_bL = TRUE;
				nB = 0;
				break;
			}
		}

		if (NULL != g_cBoss && RTemp->m_nTop < (g_cBoss->m_nY + 250) && RTemp->m_nLeft > g_cBoss->m_nX - 25 
			&& RTemp->m_nRight < (g_cBoss->m_nX + 250))
		{
			g_cBoss->m_nLife = g_cBoss->m_nLife - 1;
			g_ncX = g_cBoss->m_nX;
			g_ncY = g_cBoss->m_nY;
			
			if (0 > g_cBoss->m_nLife)
			{
				g_nMoney += 100;
				delete g_cBoss;
				g_cBoss = NULL;
				break;
			}
		}

		while (NULL != eTemp)
		{
			if (RTemp->m_nTop < eTemp->m_nY + 50 && RTemp->m_nLeft > eTemp->m_nX - 25 && RTemp->m_nRight < (eTemp->m_nX + 50) )
			{
				g_nX = RTemp->m_nLeft;
				g_nY = RTemp->m_nTop;
				g_nMoney++;
				nBoss++;

				if (eTemp == g_eHead)
				{
					if (g_eHead == g_eEnd)
					{
						g_eHead = NULL;
						g_eEnd = NULL;
						delete eTemp;

						if (RTemp == g_RHead)
						{
							g_RHead = RTemp->m_pNext;
						}
						else
						{
							RTemp->m_pLast->m_pNext = RTemp->m_pNext;
							
							if (RTemp == g_REnd)
							{
								RTemp->m_pLast->m_pNext = NULL;
								g_REnd = RTemp->m_pLast;
								break;
							}
							
							RTemp->m_pNext->m_pLast = RTemp->m_pLast;
						}

						break;
					}

					eTemp->m_pNext->m_pLast = NULL;
					g_eHead = eTemp->m_pNext;
					delete eTemp;

					if (RTemp == g_RHead)
					{
						g_RHead = RTemp->m_pNext;
					}
					else
					{
						RTemp->m_pLast->m_pNext = RTemp->m_pNext;
						
						if (RTemp == g_REnd)
						{
							RTemp->m_pLast->m_pNext = NULL;
							g_REnd = RTemp->m_pLast;
							break;
						}
						
						RTemp->m_pNext->m_pLast = RTemp->m_pLast;
					}

					break;
				}
				else
				{
					eTemp->m_pLast->m_pNext = eTemp->m_pNext;
					
					if (eTemp == g_eEnd)
					{
						eTemp->m_pLast->m_pNext = NULL;
						g_eEnd = eTemp->m_pLast;
						delete eTemp;

						if (RTemp == g_RHead)
						{
							g_RHead = RTemp->m_pNext;
						}
						else
						{
							RTemp->m_pLast->m_pNext = RTemp->m_pNext;
							
							if (RTemp == g_REnd)
							{
								RTemp->m_pLast->m_pNext = NULL;
								g_REnd = RTemp->m_pLast;
								break;
							}
							
							RTemp->m_pNext->m_pLast = RTemp->m_pLast;
						}

						break;
					}

					eTemp->m_pNext->m_pLast = eTemp->m_pLast;
					delete eTemp;

					if (RTemp == g_RHead)
					{
						g_RHead = RTemp->m_pNext;
					}
					else
					{
						RTemp->m_pLast->m_pNext = RTemp->m_pNext;

						if (RTemp == g_REnd)
						{
							RTemp->m_pLast->m_pNext = NULL;
							g_REnd = RTemp->m_pLast;
							break;
						}

						RTemp->m_pNext->m_pLast = RTemp->m_pLast;
					}

					break;
				}
			}

			eTemp = eTemp->m_pNext;
		}

		RTemp = RTemp->m_pNext;
	}

	Door *pTempTemp = g_pHead;
	DeleteTop(pTempTemp);
	pTempTemp = g_LHead;
	LeftDeleteTop(pTempTemp);
	pTempTemp = g_RHead;
	RightDeleteTop(pTempTemp);
}

void CALLBACK ALLTimerProc(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	Htime += 2;
	Xtime += (rand() % 10 + 1) * 3;
	
	if (NULL != g_pBoss || NULL != g_cBoss)
	{
		g_nTime -= 2;
	}
	
	static BOOL bBFl = FALSE;
	static BOOL bBFr = TRUE;
	static BOOL bCFl = TRUE;
	static BOOL bCFr = FALSE;
	
	if (NULL != g_cBoss)
	{
		if (g_cBoss->m_nX > 600)
		{
			bCFl = TRUE;
			bCFr = FALSE;
		}
		
		if (g_cBoss->m_nX < 0)
		{
			bCFr = TRUE;
			bCFl = FALSE;
		}
		
		if (bCFl)
		{
			g_cBoss->m_nX -= 10;
		}
		
		if (bCFr)
		{
			g_cBoss->m_nX += 10;
		}

		if (g_cBoss->m_nLife <= 3 * g_nBlife)
		{	
			if (g_cBoss->m_nX > 650)
			{
				bCFl = TRUE;
				bCFr = FALSE;
			}
			
			if (g_cBoss->m_nX < 0)
			{
				bCFr = TRUE;
				bCFl = FALSE;
			}
			
			if (bCFl)
			{
				g_cBoss->m_nX -= 30;
			}
			
			if (bCFr)
			{
				g_cBoss->m_nX += 30;
			}
		}
	}
	
	if (NULL != g_pBoss)
	{
		if (g_pBoss->m_nLife > 3 * g_nBlife)
		{	
			nB = 1;

			if (g_pBoss->m_nX > 650)
			{
				bBFl = TRUE;
				bBFr = FALSE;
			}
			
			if (g_pBoss->m_nX < 0)
			{
				bBFr = TRUE;
				bBFl = FALSE;
			}
			
			if (bBFl)
			{
				g_pBoss->m_nX -= 3;
			}
			
			if (bBFr)
			{
				g_pBoss->m_nX += 3;
			}
		}

		if (g_pBoss->m_nLife <= 3 * g_nBlife && g_pBoss->m_nLife > g_nBlife)
		{	
			nB = 2;
			hbmpB = (HBITMAP)LoadImage(NULL, "xiangjiao2.bmp", IMAGE_BITMAP, 200, 200, LR_LOADFROMFILE|LR_CREATEDIBSECTION);

			if (g_pBoss->m_nX > 650)
			{
				bBFl = TRUE;
				bBFr = FALSE;
			}
			
			if (g_pBoss->m_nX < 0)
			{
				bBFr = TRUE;
				bBFl = FALSE;
			}
			
			if (bBFl)
			{
				g_pBoss->m_nX -= 20;
			}
			
			if (bBFr)
			{
				g_pBoss->m_nX += 20;
			}
		}
		
		if (g_pBoss->m_nLife <= g_nBlife)
		{	
			nB = 3;
			g_pBoss->m_nX = (rand() % 8) * 90;
		}
	}

	static HMENU hMenu = GetMenu(hWnd);
	
	if (nnn > 3)
	{
		nMove = 2;
		nnn = 0;
		KillTimer(hWnd, 1111);
		KillTimer(hWnd, 111);
		KillTimer(hWnd, 333);
		MessageBox(hWnd, "ÄãÅ×ÆúÁËÍ¬×å", "Á¹Á¹", MB_OK);
		ClearAll();
		EnableMenuItem(hMenu, IDM_RE, MF_ENABLED);
		XStime = 0;
		HStime = 0;
		Xtime = 0;
		Htime = 0;
		nBoss = 0;
	}
	
	if (0 == g_nTime)
	{
		nMove = 2;
		nnn = 0;
		KillTimer(hWnd, 1111);
		KillTimer(hWnd, 111);
		KillTimer(hWnd, 333);
		MessageBox(hWnd, "Á¹Í¸ÁË", "Á¹Á¹", MB_OK);
		ClearAll();
		EnableMenuItem(hMenu, IDM_RE, MF_ENABLED);
		g_pBoss = NULL;
		g_cBoss = NULL;
		XStime = 0;
		HStime = 0;
		Xtime = 0;
		Htime = 0;
		nBoss = 0;
	}
}

void Paint(HWND hWnd)
{
	static RECT reRect = {0};
	GetClientRect(hWnd, &reRect);
    HDC hDC = GetDC(hWnd);
    HDC hMem = CreateCompatibleDC(hDC);
    HDC hMap = CreateCompatibleDC(hDC);
    HBITMAP hBitmap = CreateCompatibleBitmap(hDC, reRect.right - reRect.left, reRect.bottom - reRect.top);
	
    SelectObject(hMem, hBitmap);
    PaintBackground(hMem, hMap);
	PaintMoney(hMem, hMap);
    PaintFish(hMem, hMap);
    PaintDoor(hMem, hMap);
	PaintEnemy(hMem, hMap);
	PaintBoss(hMem, hMap);
	PaintChuan(hMem, hMap);
    BitBlt(hDC, 0, 0, reRect.right - reRect.left, reRect.bottom - reRect.top, hMem, 0, 0, SRCCOPY);
	
    DeleteObject(hBitmap);
    DeleteDC(hMem);
    DeleteDC(hMap);
    ReleaseDC(hWnd, hDC);
}

void PaintBackground(HDC hMem, HDC hMap)
{
	static HBITMAP hbmpg3 = (HBITMAP)LoadImage(NULL, "yuzhou1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE|LR_CREATEDIBSECTION);
	static HBITMAP hbmpg1 = (HBITMAP)LoadImage(NULL, "hahaha.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE|LR_CREATEDIBSECTION);
	static HBITMAP hbmpg2 = (HBITMAP)LoadImage(NULL, "yuzhou2.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE|LR_CREATEDIBSECTION);
	static HBITMAP hbmpg = (HBITMAP)LoadImage(NULL, "bei.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE|LR_CREATEDIBSECTION);

	if (NULL != g_pBoss)
	{
		if (1 == nB)
		{
			GetObject(hbmpg1, sizeof(BITMAP), &bmp);
			SelectObject(hMap, hbmpg1);
			SetStretchBltMode(hMem, COLORONCOLOR);
			StretchBlt(hMem, 0, 0, g_reRect.right, g_reRect.bottom, hMap, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);
		}
		
		if (2 == nB)
		{
			GetObject(hbmpg2, sizeof(BITMAP), &bmp);
			SelectObject(hMap, hbmpg2);
			SetStretchBltMode(hMem, COLORONCOLOR);
			StretchBlt(hMem, 0, 0, g_reRect.right, g_reRect.bottom, hMap, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);
		}

		if (3 == nB)
		{
			GetObject(hbmpg3, sizeof(BITMAP), &bmp);
			SelectObject(hMap, hbmpg3);
			SetStretchBltMode(hMem, COLORONCOLOR);
			StretchBlt(hMem, 0, 0, g_reRect.right, g_reRect.bottom, hMap, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);
		}
	}
	else
	{
		GetObject(hbmpg, sizeof(BITMAP), &bmp);
		SelectObject(hMap, hbmpg);
		SetStretchBltMode(hMem, COLORONCOLOR);
		StretchBlt(hMem, 0, 0, g_reRect.right, g_reRect.bottom, hMap, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);
	}
}

void PaintFish(HDC hMem, HDC hMap)
{
	if (NULL != FishL && NULL != FishR && g_Skill)
	{
		FishL->m_X = FishKing.m_X - 30;
		static HBITMAP hbmpL = (HBITMAP)LoadImage(NULL, "Left.bmp", IMAGE_BITMAP, 30, 30, LR_LOADFROMFILE|LR_CREATEDIBSECTION);
		GetObject(hbmpL, sizeof(BITMAP), &bmp);
		SelectObject(hMap, hbmpL);
		TransparentBlt(hMem, FishL->m_X, 510, 30, 30, hMap, 0, 0, bmp.bmWidth, bmp.bmHeight, RGB(255, 255, 255));

		FishR->m_X = FishKing.m_X + 100;
		static HBITMAP hbmpR = (HBITMAP)LoadImage(NULL, "Right.bmp", IMAGE_BITMAP, 30, 30, LR_LOADFROMFILE|LR_CREATEDIBSECTION);
		GetObject(hbmpR, sizeof(BITMAP), &bmp);
		SelectObject(hMap, hbmpR);
		TransparentBlt(hMem, FishR->m_X, 510, 30, 30, hMap, 0, 0, bmp.bmWidth, bmp.bmHeight, RGB(255, 255, 255));
	}

	static HBITMAP hbmpF = (HBITMAP)LoadImage(NULL, "xianyu2.bmp", IMAGE_BITMAP, 150, 150, LR_LOADFROMFILE|LR_CREATEDIBSECTION);
	GetObject(hbmpF, sizeof(BITMAP), &bmp);
    SelectObject(hMap, hbmpF);
	TransparentBlt(hMem, FishKing.m_X, 470, 150, 150, hMap, 0, 0, bmp.bmWidth, bmp.bmHeight, RGB(255, 255, 255));
}

void PaintDoor(HDC hMem, HDC hMap)
{
	Door *pTemp = g_pHead;
	Door *LTemp = g_LHead;
	Door *RTemp = g_RHead;
	static HBITMAP hbmpD = (HBITMAP)LoadImage(NULL, "huaji.bmp", IMAGE_BITMAP, 30, 30, LR_LOADFROMFILE|LR_CREATEDIBSECTION);
    GetObject(hbmpD, sizeof(BITMAP), &bmp);
	SelectObject(hMap, hbmpD);
	
	while (NULL != LTemp && g_Skill)
	{
		TransparentBlt(hMem, LTemp->m_nLeft, LTemp->m_nTop, 30, 30, hMap, 0, 0, bmp.bmWidth, bmp.bmHeight, RGB(255, 255, 255));
		LTemp = LTemp->m_pNext;
	}

	while (NULL != RTemp && g_Skill)
	{
		TransparentBlt(hMem, RTemp->m_nLeft, RTemp->m_nTop, 30, 30, hMap, 0, 0, bmp.bmWidth, bmp.bmHeight, RGB(255, 255, 255));
		RTemp = RTemp->m_pNext;
	}

	while (NULL != pTemp)
	{
		TransparentBlt(hMem, pTemp->m_nLeft, pTemp->m_nTop, 30, 30, hMap, 0, 0, bmp.bmWidth, bmp.bmHeight, RGB(255, 255, 255));
		pTemp = pTemp->m_pNext;
	}
}

void PaintEnemy(HDC hMem, HDC hMap)
{
	if (NULL != g_eHead)
	{
		Enemy *eTemp = g_eHead;
		static HBITMAP hbmpE = (HBITMAP)LoadImage(NULL, "xianyu.bmp", IMAGE_BITMAP, 50, 50, LR_LOADFROMFILE|LR_CREATEDIBSECTION);
		GetObject(hbmpE, sizeof(BITMAP), &bmp);
		SelectObject(hMap, hbmpE);
		
		while (NULL != eTemp)
		{
			TransparentBlt(hMem, eTemp->m_nX, eTemp->m_nY, 50, 100, hMap, 0, 0, bmp.bmWidth, bmp.bmHeight, RGB(255, 255, 255));
			eTemp = eTemp->m_pNext;
		}

		static HBITMAP hbmpBoom = (HBITMAP)LoadImage(NULL, "Boom.bmp", IMAGE_BITMAP, 50, 50, LR_LOADFROMFILE|LR_CREATEDIBSECTION);
		GetObject(hbmpBoom, sizeof(BITMAP), &bmp);
		SelectObject(hMap, hbmpBoom);
		TransparentBlt(hMem, g_nX, g_nY, 50, 50, hMap, 0, 0, bmp.bmWidth, bmp.bmHeight, RGB(255, 255, 255));
		g_nX = 900;
		g_nY = 900;
	}
}

void PaintBoss(HDC hMem, HDC hMap)
{
	if (NULL != g_pBoss)
	{
		GetObject(hbmpB, sizeof(BITMAP), &bmp);
		SelectObject(hMap, hbmpB);
		TransparentBlt(hMem, g_pBoss->m_nX, g_pBoss->m_nY, 200, 200, hMap, 0, 0, bmp.bmWidth, bmp.bmHeight, RGB(255, 255, 255));

		static HBITMAP hbmpBoom = (HBITMAP)LoadImage(NULL, "Boom.bmp", IMAGE_BITMAP, 200, 200, LR_LOADFROMFILE|LR_CREATEDIBSECTION);
		GetObject(hbmpBoom, sizeof(BITMAP), &bmp);
		SelectObject(hMap, hbmpBoom);
		TransparentBlt(hMem, g_nX, g_nY, 200, 200, hMap, 0, 0, bmp.bmWidth, bmp.bmHeight, RGB(255, 255, 255));
		g_nX = 900;
		g_nY = 900;
	}
}

void PaintChuan(HDC hMem, HDC hMap)
{
	if (NULL != g_cBoss)
	{
		static HBITMAP hbmpC = (HBITMAP)LoadImage(NULL, "huaji.bmp", IMAGE_BITMAP, 250, 250, LR_LOADFROMFILE|LR_CREATEDIBSECTION);
		GetObject(hbmpC, sizeof(BITMAP), &bmp);
		SelectObject(hMap, hbmpC);
		TransparentBlt(hMem, g_cBoss->m_nX, g_cBoss->m_nY, 250, 250, hMap, 0, 0, bmp.bmWidth, bmp.bmHeight, RGB(255, 255, 255));

		static HBITMAP hbmpBoom1 = (HBITMAP)LoadImage(NULL, "Boom.bmp", IMAGE_BITMAP, 250, 250, LR_LOADFROMFILE|LR_CREATEDIBSECTION);
		GetObject(hbmpBoom1, sizeof(BITMAP), &bmp);
		SelectObject(hMap, hbmpBoom1);
		TransparentBlt(hMem, g_ncX, g_ncY, 250, 250, hMap, 0, 0, bmp.bmWidth, bmp.bmHeight, RGB(255, 255, 255));
		g_ncY = 900;
		g_ncX = 900;
	}
}

void PaintMoney(HDC hMem, HDC hMap)
{
	static int i = 0;

	if (NULL != g_pBoss)
	{
		i = g_pBoss->m_nLife;
	}

	if (NULL == g_pBoss)
	{
		i = 0;
	}

	TCHAR str[255];

	if (NULL == g_pBoss && !g_bL)
	{
		wsprintf(str, "Óã»ê:  %d   Ïã½¶¾ý¼´½«À´Ï®!!!   ¡á   ", g_nMoney);
	}
	else if (NULL == g_pBoss && g_bL)
	{
		wsprintf(str, "Óã»ê:  %d   Ïã½¶¾ý¼´½«ÔÙ´ÎÀ´Ï®!!!Ïã½¶¾ýµÄÑªÁ¿¼´½«·­±¶!!!   ¡á   ", g_nMoney);
	}
	else
	{
		wsprintf(str, "Óã»ê:  %d   Ïã½¶¾ýÑªÁ¿:  %d   Á¹Á¹µ¹¼ÆÊ±:  %d   Ê×´Î»÷É±Ïã½¶¾ý½«»ñµÃ³¬¼¶ÎäÆ÷¡á   ", g_nMoney, i, g_nTime / 100);
	}
	
	TextOut(hMem, 0, 572, str, strlen(str));
}

LRESULT OnCREATE(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	if (STAY == nMove)
	{
		CreateOur(hWnd);
		CreateEnemy(hWnd);
		SetTimer(hWnd, 1111, 10, ALLTimerProc);
	}

	return 0;
}

LRESULT OnKEYDOWN(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	return 0;
}

LRESULT OnKEYUP(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	return 0;
}

LRESULT OnPAINT(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	GetClientRect(hWnd, &g_reRect);
	PAINTSTRUCT ps;
	HDC	hdc = BeginPaint(hWnd, &ps);
	static BOOL bEnemy = TRUE;
	static BOOL bTow = FALSE;
	static int i = 0;
	
	if (STAY == nMove)
	{
		Paint(hWnd);

		if (0 == i)
		{
			MessageBox(hWnd, "ÓÃ°®Õü¾ÈÄãµÄÍ¬°û!!!  Äã½«¸úËæÊó±êÒÆ¶¯¡ý", "ÏÌÓã", MB_OK);
			SetTimer(hWnd, 111, 10, DoorProc);
			SetTimer(hWnd, 333, 10, EnemoProc);
			i++;
		}

		nMove = 1;
	}

	Paint(hWnd);
	
	if (MOVE == nMove)
	{
		if (Xtime > g_nEnTime && bEnemy)
		{
			EnemyAdd(hWnd);
			Xtime = 0;
		}

		if (Htime > 6)
		{
			if (NULL == g_pHead)
			{
				Door *dNew = new Door;
				dNew->m_pLast = NULL;
				dNew->m_pNext = NULL;
				g_pHead = dNew;
				g_pEnd = dNew;
				
				dNew->m_nLeft = FishKing.m_X + 30;
				dNew->m_nRight = dNew->m_nLeft;
				dNew->m_nTop = 480;
				dNew->m_nBottom = 510;
			}

			DoorAdd(hWnd);

			if (g_Skill)
			{
				if (NULL == g_LHead)
				{
					CreateLe(FishKing.m_X - 30);
				}

				LeftDoorAdd(hWnd);

				if (NULL == g_RHead)
				{
					CreateRi(FishKing.m_X + 100);
				}

				RightDoorAdd(hWnd);
			}
			
			Htime = 0;
		}

		if (nBoss > 100)
		{
			bEnemy = FALSE;
			ClearEnemy();
			BossAdd(hWnd);
			ChuanpuAdd(hWnd);
			nBoss = 0;
			bTow = TRUE;
		}

		if (nBoss > 150 && bTow)
		{
			bEnemy = FALSE;
			ClearEnemy();
			BossAdd(hWnd);
			ChuanpuAdd(hWnd);
			nBoss = 0;
			bTow = FALSE;
		}

		if (NULL == g_pBoss)
		{
			CreateEnemy(hWnd);
			bEnemy = TRUE;
		}
	}

	EndPaint(hWnd, &ps);

	return 0;
}

LRESULT OnMOUSEMOVE(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	if (STAY != nMove)
	{
		FishKing.m_X = LOWORD(lParam) - 50;
	}

	return 0;
}

LRESULT OnCOMMAND(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	HMENU hMenu = GetMenu(hWnd);

	switch (LOWORD(wParam))
	{
		case IDM_RE:
			{
				CreateOur(hWnd);
				CreateEnemy(hWnd);
				SetTimer(hWnd, 1111, 10, ALLTimerProc);
				SetTimer(hWnd, 111, 10, DoorProc);
				SetTimer(hWnd, 333, 10, EnemoProc);
				EnableMenuItem(hMenu, IDM_RE, MF_GRAYED);
				InvalidateRect(hWnd, NULL, FALSE);
				nMove = MOVE;
				g_nTime = 6000;
				nnn = 0;
				nBoss = 0;
				g_nMoney = 0;
				g_nBlife = 1000;
				g_nHard = 3;
				g_bL = FALSE;
				g_Skill = FALSE;
				g_nEnTime = 80;
			}
			break;
		default:
			break;
	}

	return 0;
}

LRESULT OnCLOSE(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	DestroyWindow(hWnd);

	return 0;
}

LRESULT OnDESTROY(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	PostQuitMessage(0);

	return 0;
}