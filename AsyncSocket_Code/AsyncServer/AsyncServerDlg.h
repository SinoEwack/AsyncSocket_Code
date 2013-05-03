// AsyncServerDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "IMsgInterflow.h"

// CAsyncServerDlg dialog
class CAsyncServerDlg : public CDialog, public IMsgInterflow
{
// Construction
public:
	CAsyncServerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_ASYNCSERVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

    virtual void OnFDClose(SOCKET sClient);

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
    virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
    afx_msg void OnClose();
	afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnBnClickedBtnListen();
    afx_msg void OnBnClickedBtnTalk();
    afx_msg void OnBnClickedBtnQuit();

    LRESULT OnNetwork(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

private:
    //��ӦSocket�����¼�
    void OnAccept();
    void OnFDRead(SOCKET sClient);

private:
    SOCKET m_sListen;//����˼����׽���
    CListBox m_ClientList;
};
