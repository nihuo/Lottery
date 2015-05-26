
// choujiangDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CchoujiangDlg dialog
class CChouJiangDlg : public CDialogEx
{
// Construction
public:
	CChouJiangDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_CHOUJIANG_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	CFont m_EngNameFont;
	CArray< CStringArray >	m_aNameList;
	CStringArray			m_aDisplayName;
	int						m_iCurGenLoopIdx;
	int						m_iCurNameIdx;
	int						m_iDisplayCount;
	int						m_iCurDisplayIdx;
	CBitmap					m_BkBmp;


	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnWindowPosChanging(WINDOWPOS* lpwndpos);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	void InitSelList();
	void FillSelList(UINT nIdx);
	void LoadNameList();
	void BuildDisplayName();
	CString GetDisplayName(UINT nIdx);

	CEdit m_EngNameEdit;
	CString m_sEngName;
	CListCtrl m_SelList;
	CEdit m_CountEdit;
	UINT m_iCount;
	CSpinButtonCtrl m_CountSpin;
	afx_msg void OnBnClickedOk();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBnClickedEndRoll();
	CButton m_BeginBtn;
};
