
// Game.h : ������� ���� ��������� ��� ���������� PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�������� stdafx.h �� ��������� ����� ����� � PCH"
#endif

#include "resource.h"		// �������� �������


// CGameApp:
// � ���������� ������� ������ ��. Game.cpp
//

class CGameApp : public CWinAppEx
{
public:
	CGameApp();

// ���������������
	public:
	virtual BOOL InitInstance();

// ����������

	DECLARE_MESSAGE_MAP()
};

extern CGameApp theApp;