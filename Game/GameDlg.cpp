
// GameDlg.cpp : ���� ����������
//

#include "stdafx.h"
#include "Game.h"
#include "GameDlg.h"
#include "akBufferedDC.h"
//#include <algorithm>
//#include <functional>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ���������� ���� CAboutDlg ������������ ��� �������� �������� � ����������

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// ������ ����������� ����
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // ��������� DDX/DDV

// ����������
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// ���������� ���� CGameDlg




CGameDlg::CGameDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGameDlg::IDD, pParent), m_stopTimer(true)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CGameDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
	ON_WM_KEYDOWN()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// ����������� ��������� CGameDlg

// *** ����� �������� ������ ������������
BOOL CGameDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���������� ������ "� ���������..." � ��������� ����.

	// IDM_ABOUTBOX ������ ���� � �������� ��������� �������.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ������ ������ ��� ����� ����������� ����. ����� ������ ��� �������������,
	//  ���� ������� ���� ���������� �� �������� ����������
	SetIcon(m_hIcon, TRUE);			// ������� ������
	SetIcon(m_hIcon, FALSE);		// ������ ������

	// *** ��� �������� ��� ��������� �����������: ����������� ���� TVisibleObject,
	// ��������� ��������, ��������� ������, ��������� ������� � ��������.
	// ������ ���� � ����� ������� �����������
	
	GetClientRect(&m_client_rectangle);
	TVisibleObject::InitScreenSize(m_client_rectangle.Width(), m_client_rectangle.Height());
	TVisibleObject::InitBitmaps(::AfxGetInstanceHandle());
	m_hMemDC  = ::CreateCompatibleDC(NULL);

	gun = TGun(m_hMemDC);
	airfield = new TAirfield(list_of_aircraft, m_hMemDC);
	SetTimer(ID_TIMER, TimerInterval, NULL);

	return TRUE;  // ������� �������� TRUE, ���� ����� �� ������� �������� ����������
}

void CGameDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// ��� ���������� ������ ����������� � ���������� ���� ����� ��������������� ����������� ���� �����,
//  ����� ���������� ������. ��� ���������� MFC, ������������ ������ ���������� ��� �������������,
//  ��� ������������� ����������� ������� ��������.


// *** ����� ����������� ���� �� �����
void CGameDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �������� ���������� ��� ���������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ������������ ������ �� ������ ����������� ��������������
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ��������� ������
		dc.DrawIcon(x, y, m_hIcon);
	}
	else	// *** ��� ��������� �� ����� ��'����
	{
		// ����������� ��������, �� ������� ������� ����������� ���������
		CakBufferedDC dc(this);
		// ����������� ���
		CBrush bk_brush(GetSysColor(COLOR_BTNFACE));
		dc.FillRect(&m_client_rectangle, &bk_brush);
		// ��������� ����� ��'����
		gun.Show(dc);
		ShowAll(list_of_bullet, dc);
		ShowAll(list_of_aircraft, dc);
		//ShowObj showFunc(dc);
		//std::for_each(list_of_bullet.begin(), list_of_bullet.end(), showFunc);
		//std::for_each(list_of_bullet.begin(), list_of_bullet.end(), [&dc](TMovedObject* obj) { obj->show(dc); });
		//std::for_each(list_of_aircraft.begin(), list_of_aircraft.end(), showFunc);
	}
}

// ������� �������� ��� ������� ��� ��������� ����������� ������� ��� �����������
//  ���������� ����.
HCURSOR CGameDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// *** ����������� �� ������� ������ ��������:
void CGameDlg::OnTimer(UINT_PTR nIDEvent)
{
	airfield->Spent(TimerInterval);		// ��������
	MoveAndGarbage(list_of_aircraft);		// ����� ������ � �������
	MoveAndGarbage(list_of_bullet);		// ��� ������ � ��������������
	list_of_bullet.remove_if(m_hiter);	    // ��� ��������

	// ���� ���� ��������� ��'���� ���� ����� ������������!!!
	Invalidate();

	CDialog::OnTimer(nIDEvent);
}

// *** �������� ����� ����� ���� ���: ����� - ���
void CGameDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_stopTimer) KillTimer(ID_TIMER);
	else SetTimer(ID_TIMER, TimerInterval, NULL);
	m_stopTimer = !m_stopTimer;

	CDialog::OnLButtonDown(nFlags, point);
}

// *** ���������, ��� ������ ���������
void CGameDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar)
	{
	case VK_SPACE: list_of_bullet.push_back(this->gun.Fire()); break;
	case K_LEFT: gun.MoveLeft(); break;		// [ < | , ]
	case K_RIGHT: gun.MoveRight(); break;	// [ > | . ]
	case 'Q': ::PostQuitMessage(0);		// ��������� ���������
	}

	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}
// ��������� ����� �� ������ ����� � ��������, ���� ����������
BOOL CGameDlg::PreTranslateMessage(MSG* pMsg) 
{
    if(pMsg->message == WM_KEYDOWN)
    {
		if (pMsg->wParam == VK_LEFT) pMsg->wParam = K_LEFT;
		else if (pMsg->wParam == VK_RIGHT) pMsg->wParam = K_RIGHT;
    }	
    return CDialog::PreTranslateMessage(pMsg);
}

BOOL CGameDlg::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}
