#include "StdAfx.h"
#include "WindowProc.h"
#include "Message.h"

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	MSG_MAP *pMsg = g_msg;
	while(NULL != pMsg->m_uMsg)
	{
		if (pMsg->m_uMsg == uMsg)
		{
			return pMsg->m_pFn(hWnd, wParam, lParam);
		}
		
		pMsg++;
	}
	
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}