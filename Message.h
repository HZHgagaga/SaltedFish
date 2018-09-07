#ifndef __MESSAGE_H__
#define __MESSAGE_H__

struct MSG_MAP
{
	UINT m_uMsg;
	LRESULT (*m_pFn)(HWND, WPARAM, LPARAM);
};

struct SnakeNode
{
	int m_nX;
	int m_nY;
	SnakeNode *m_pNext;
};

LRESULT OnCREATE(HWND, WPARAM, LPARAM);
LRESULT OnCOMMAND(HWND, WPARAM, LPARAM);
LRESULT OnKEYDOWN(HWND, WPARAM, LPARAM);
LRESULT OnPAINT(HWND, WPARAM, LPARAM);
LRESULT OnCLOSE(HWND, WPARAM, LPARAM);
LRESULT OnDESTROY(HWND, WPARAM, LPARAM);
LRESULT OnKEYUP(HWND, WPARAM, LPARAM);
LRESULT OnMOUSEMOVE(HWND, WPARAM, LPARAM);

extern MSG_MAP g_msg[];

#endif