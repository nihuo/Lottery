
// choujiangDlg.cpp : implementation file
//

#include "stdafx.h"
#include "choujiang.h"
#include "choujiangDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define TIMER_ELAPSE	100

extern TCHAR* name_list[][6];
extern int i_name_list_count;

// CAboutDlg dialog used for App About

/*const CStringArray& fn_as_ExplodeString(const CString& s, const CString& sep)
{
	int i = 0;
	CStringArray saItems;
	for (CString sItem = s.Tokenize(sep, i); i >= 0; sItem = s.Tokenize(sep, i))
	{
		saItems.Add(sItem);
	}

	return saItems;
}
*/

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CChouJiangDlg dialog




CChouJiangDlg::CChouJiangDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CChouJiangDlg::IDD, pParent)
	, m_sEngName(_T(""))
	, m_iCount(1)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CChouJiangDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_NAME_EDIT, m_EngNameEdit);
	DDX_Text(pDX, IDC_NAME_EDIT, m_sEngName);
	DDX_Control(pDX, IDC_SEL_LIST, m_SelList);
	DDX_Control(pDX, IDC_COUNT_EDIT, m_CountEdit);
	DDX_Text(pDX, IDC_COUNT_EDIT, m_iCount);
	DDV_MinMaxUInt(pDX, m_iCount, 1, 10);
	DDX_Control(pDX, IDC_COUNT_SPIN, m_CountSpin);
	DDX_Control(pDX, IDOK, m_BeginBtn);
}

BEGIN_MESSAGE_MAP(CChouJiangDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_WINDOWPOSCHANGING()
	ON_WM_TIMER()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDOK, &CChouJiangDlg::OnBnClickedOk)
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(ID_END_ROLL, &CChouJiangDlg::OnBnClickedEndRoll)
END_MESSAGE_MAP()


// CChouJiangDlg message handlers

BOOL CChouJiangDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	m_CountSpin.SetBuddy(&m_CountEdit);
	m_CountSpin.SetRange(1, 10);

	m_EngNameFont.CreatePointFont(600, _T("Arial"));
	m_EngNameEdit.SetFont(&m_EngNameFont);
	m_EngNameEdit.ModifyStyle(0, ES_CENTER | ES_READONLY);

	InitSelList();
	LoadNameList();
	m_BkBmp.LoadBitmapW(MAKEINTRESOURCE(IDB_BK_BMP));


	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CChouJiangDlg::InitSelList()
{
	m_SelList.InsertColumn(0, _T("No"), LVCFMT_LEFT, 150, 0);
	m_SelList.InsertColumn(1, _T("Name"), LVCFMT_LEFT, 150, 0);
	m_SelList.InsertColumn(2, _T("Chn Name"), LVCFMT_LEFT, 150, 0);
	m_SelList.InsertColumn(3, _T("Tel"), LVCFMT_LEFT, 100, 0);
	m_SelList.InsertColumn(4, _T("Club"), LVCFMT_LEFT, 150, 0);

	m_SelList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP | LVS_EX_INFOTIP);
}



void CChouJiangDlg::LoadNameList()
{
	CFileDialog openDlg(TRUE, _T( "csv" ), NULL, 0, _T( "*.csv|*.csv||" ) );

	if (openDlg.DoModal() != IDOK)
	{
		PostQuitMessage(0);
		return;
	}


	FILE *fStream;
	errno_t e = _tfopen_s(&fStream, openDlg.GetPathName() , _T("rt,ccs=UTF-8"));

	//if (!nameFile.Open(openDlg.GetPathName(), CFile::modeRead )) 
	if(e != 0)
	{
		AfxMessageBox(_T("Failed to open file " + openDlg.GetPathName()));
		PostQuitMessage(0);
		return;
	}

	CStdioFile nameFile(fStream);

	CString sLine;
	CString sep = _T(",");

	m_aNameList.SetSize(0,1024);
	
	int iLineCount = 0;

	while (nameFile.ReadString(sLine))
	{
		iLineCount++;

		if (iLineCount == 1)
			continue;	//跳过标题行

		int ic = m_aNameList.GetSize();
		m_aNameList.SetSize(ic + 1, 1024);

		int i = 0;
		for (CString sItem = sLine.Tokenize(sep, i); i >= 0; sItem = sLine.Tokenize(sep, i))
		{
			m_aNameList[ ic ].Add(sItem);
		}

		//m_aNameList.Add(CStringArray());
	}

	nameFile.Close();
}

void CChouJiangDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CChouJiangDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CChouJiangDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CChouJiangDlg::OnWindowPosChanging(WINDOWPOS* lpwndpos)
{
	CDialogEx::OnWindowPosChanging(lpwndpos);

	/*if (lpwndpos->cx < 780)
		lpwndpos->cx = 780;

	if (lpwndpos->cy < 465)
		lpwndpos->cy = 465;*/
}


void CChouJiangDlg::OnTimer(UINT_PTR nIDEvent)
{
	CDialogEx::OnTimer(nIDEvent);

	if (m_iCurNameIdx < 0)
	{
		m_iCurDisplayIdx = rand() % m_aNameList.GetSize();
		m_EngNameEdit.SetWindowTextW(GetDisplayName(m_iCurDisplayIdx));
	}
	else
	{
		m_EngNameEdit.SetWindowTextW(GetDisplayName(m_iCurNameIdx));
		MSG						msg;

		while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}

		KillTimer(1);
		Sleep(2000);
		SetTimer(1, TIMER_ELAPSE, NULL);

		FillSelList(m_iCurNameIdx);
		m_aNameList.RemoveAt(m_iCurNameIdx);

		m_iCurNameIdx = -1;

		if (m_iCurGenLoopIdx < m_iCount)
		{
			m_iCurGenLoopIdx++;
		}
		else
		{
			if (m_iCount > 1)
				m_EngNameEdit.SetWindowTextW(_T(""));

			KillTimer(1);
			m_BeginBtn.EnableWindow(TRUE);
		}
	}
}


void CChouJiangDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	//
}


void CChouJiangDlg::OnBnClickedOk()
{
	if (!UpdateData())
		return;

	m_BeginBtn.EnableWindow(FALSE);

	m_SelList.DeleteAllItems();

	srand(GetTickCount());

	m_iCurGenLoopIdx = 1;
	//m_iCurNameIdx = rand() % m_aNameList.GetSize();
	m_iCurNameIdx = -1;
	BuildDisplayName();
	SetTimer(1, TIMER_ELAPSE, NULL);
}


void CChouJiangDlg::BuildDisplayName()
{
	m_iCurDisplayIdx = rand() % m_aNameList.GetSize();
	/*m_iDisplayCount = 20;

	CArray<BOOL> aFlags;
	int i;

	aFlags.SetSize(m_aNameList.GetSize());

	for (i = 0; i < aFlags.GetSize(); i++)
	{
		aFlags[i] = FALSE;
	}

	m_aDisplayName.SetSize(m_iDisplayCount);

	for (i = 0; i < m_iDisplayCount; i++)
	{
		int idx = rand() % m_aNameList.GetSize();

		if (aFlags[idx] || idx == m_iCurNameIdx )
		{
			i--;
			continue;
		}

		aFlags[idx] = TRUE;

		m_aDisplayName[i] = GetDisplayName(idx);
	}*/
}

CString CChouJiangDlg::GetDisplayName(UINT nIdx)
{
	if (nIdx >= m_aNameList.GetSize())
		return _T("");

	if (m_aNameList[nIdx][1].GetLength() <= 0)
		return m_aNameList[nIdx][0] + _T(" ") + m_aNameList[nIdx][2];
	else
		return m_aNameList[nIdx][0] + _T(" ") + m_aNameList[nIdx][1];
}


void CChouJiangDlg::FillSelList(UINT nIdx)
{
	if (nIdx >= m_aNameList.GetSize())
		return ;

	int iRow, i ;

	iRow = m_SelList.InsertItem(m_SelList.GetItemCount(), m_aNameList[nIdx][0]);

	for (i = 1; i < 5; i++)
	{
		if (i >= m_aNameList[nIdx].GetSize() )
			continue;

		if (i == 4 && m_aNameList[nIdx].GetSize() < 5)
			continue;

		if (m_aNameList[nIdx][i] == _T("VIP"))
			continue;

		if ( i == 4 )
			m_SelList.SetItemText(iRow, i, m_aNameList[nIdx][5]);
		else
			m_SelList.SetItemText(iRow, i, m_aNameList[nIdx][i]);
	}
}


BOOL CChouJiangDlg::OnEraseBkgnd(CDC* pDC)
{
	CRect rc;

	GetClientRect(rc);

	BITMAP bm;
	m_BkBmp.GetObject(sizeof(BITMAP), &bm);

	CDC dcMemory;
	CBitmap *pOldBmp; 

	dcMemory.CreateCompatibleDC(pDC);
	pOldBmp = dcMemory.SelectObject(&m_BkBmp);

	pDC->StretchBlt(0, 0, rc.right, rc.bottom, &dcMemory, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
		
	dcMemory.SelectObject(pOldBmp);

	return TRUE;
	//return CDialogEx::OnEraseBkgnd(pDC);
}


void CChouJiangDlg::OnBnClickedEndRoll()
{
	m_iCurNameIdx = m_iCurDisplayIdx;
}
