// AsyncClientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AsyncClient.h"
#include "AsyncClientDlg.h"
#include "AsyncFunc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAsyncClientDlg dialog

#define WM_NETWORK WM_USER+100
#define DEFAULT_BUFFER 4096

const UINT DEFAULT_PORT = 5150;


CAsyncClientDlg::CAsyncClientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAsyncClientDlg::IDD, pParent)
    , m_bSocketClose(false)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CAsyncClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAsyncClientDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_BTN_CONN, &CAsyncClientDlg::OnBnClickedBtnConn)
    ON_BN_CLICKED(IDC_BTN_CLOSE, &CAsyncClientDlg::OnBnClickedBtnClose)
    ON_BN_CLICKED(IDC_BTN_SEND, &CAsyncClientDlg::OnBnClickedBtnSend)
    ON_MESSAGE(WM_NETWORK,&CAsyncClientDlg::OnNetwork)
    ON_WM_CLOSE()
END_MESSAGE_MAP()


// CAsyncClientDlg message handlers

BOOL CAsyncClientDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
    GetDlgItem(IDC_EDIT_SEND)->EnableWindow(FALSE);
    GetDlgItem(IDC_BTN_SEND)->EnableWindow(FALSE);

    CIPAddressCtrl* pIP=(CIPAddressCtrl*)GetDlgItem(IDC_IP_CONN);
    pIP->SetAddress(127,0,0,1);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CAsyncClientDlg::OnPaint()
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
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CAsyncClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CAsyncClientDlg::OnBnClickedBtnConn()
{
    CIPAddressCtrl * pIP=(CIPAddressCtrl*)GetDlgItem(IDC_IP_CONN);
    if (pIP->IsBlank())
    {
        MessageBox(_T("����˵�ַ����Ϊ��"), _T("��ʾ"), MB_OK|MB_ICONWARNING);
        return;
    }

    //��ȡIP��ַ
    BYTE nf1,nf2,nf3,nf4;
    pIP->GetAddress(nf1,nf2,nf3,nf4);

    char szServer[128] = {0};
    sprintf_s(szServer, 128, "%d.%d.%d.%d", nf1,nf2,nf3,nf4);

    WSADATA wsd;
    if (WSAStartup(MAKEWORD(2,2), &wsd) != 0)
    {
        CAsyncFunc::SetAppendText(this, IDC_EDIT_RECV, _T("load Winsock library failed!"));
        return;
    }
    CAsyncFunc::SetAppendText(this, IDC_EDIT_RECV, _T("load Winsock library success!"));

    m_sClient = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    if (m_sClient == INVALID_SOCKET)
    {
        CAsyncFunc::SetAppendText(this, IDC_EDIT_RECV, _T("socket() failed!"));
        return;
    }
    CAsyncFunc::SetAppendText(this, IDC_EDIT_RECV, _T("socket() success!"));

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(DEFAULT_PORT);
    server.sin_addr.s_addr = inet_addr(szServer);

    if (connect(m_sClient, (struct sockaddr*)&server, sizeof(server))
        == SOCKET_ERROR)
    {
        CAsyncFunc::SetAppendText(this, IDC_EDIT_RECV, _T("connect() failed!"));
        return;
    }
    CAsyncFunc::SetAppendText(this, IDC_EDIT_RECV, _T("connect() success!"));

    //ע�����Ȥ�������¼�
    int nAsyncRet = WSAAsyncSelect(m_sClient, m_hWnd, WM_NETWORK, FD_READ|FD_CLOSE|FD_WRITE);
    if (SOCKET_ERROR == nAsyncRet)
    {
        CAsyncFunc::SetAppendText(this, IDC_EDIT_RECV, _T("ע�������¼�ʧ�ܣ�"));

        return;
    }

    GetDlgItem(IDC_EDIT_SEND)->EnableWindow(TRUE);
    GetDlgItem(IDC_BTN_SEND)->EnableWindow(TRUE);

    GetDlgItem(IDC_IP_CONN)->EnableWindow(FALSE);
    GetDlgItem(IDC_BTN_CONN)->EnableWindow(FALSE);
}

void CAsyncClientDlg::OnBnClickedBtnClose()
{
    PostMessage(WM_CLOSE);
}

void CAsyncClientDlg::OnBnClickedBtnSend()
{
    CString strSendMsg = _T("");
    GetDlgItemText(IDC_EDIT_SEND, strSendMsg);

    if (strSendMsg.IsEmpty())
    {
        MessageBox(_T("������Ϣ����Ϊ��"), _T("��ʾ"), MB_OK|MB_ICONWARNING);
        return;
    }

    string saSendMsg = CAsyncFunc::UnicodeToAnsi(wstring(strSendMsg));
    const char* szSendMsg = saSendMsg.c_str();




    int ret = send(m_sClient, szSendMsg, (int)strlen(szSendMsg), 0);
    if (ret == 0 || ret == SOCKET_ERROR)
    {
        CAsyncFunc::SetAppendText(this, IDC_EDIT_RECV, _T("send() failed!"));
        return;
    }

    CString strSendInfo = _T("");
    strSendInfo += _T("me:");
    strSendInfo += strSendMsg;

    CAsyncFunc::SetAppendText(this, IDC_EDIT_RECV, strSendInfo);
    SetDlgItemText(IDC_EDIT_SEND, _T(""));
}

LRESULT CAsyncClientDlg::OnNetwork(WPARAM wParam, LPARAM lParam)
{
    //SOCKET s = (SOCKET)wParam;
    WORD netEvent = WSAGETSELECTEVENT(lParam);
    WORD error = WSAGETSELECTERROR(lParam);
    if (error!=0)
    {
        CString strErrorInfo = _T("");
        strErrorInfo.Format(_T("Error code: %d"), error);
        CAsyncFunc::SetAppendText(this, IDC_EDIT_RECV, strErrorInfo);

        return -1;
    }

    switch(netEvent)
    {
    case FD_READ: OnFDRead(); break;
	case FD_WRITE: OnFDWrite(); break;
    case FD_CLOSE: OnFDClose(); break;
    }

    return 0;
}

void CAsyncClientDlg::OnFDRead()  
{
    char szBuff[DEFAULT_BUFFER] = {0};

    int ret = recv(m_sClient, szBuff, DEFAULT_BUFFER, 0);
    if (0 == ret)
    {
        return;
    }
    else if (SOCKET_ERROR == ret)
    {
        CAsyncFunc::SetAppendText(this, IDC_EDIT_RECV, _T("recv() failed"));
        return;
    }
    szBuff[ret] = '\0';

    wstring wsRecvInfo = CAsyncFunc::AnsiToUnicode(string(szBuff));
    CString strRecvInfo = _T("");
    strRecvInfo += _T("�����:");
    strRecvInfo += (CString)(wsRecvInfo.c_str());

    CAsyncFunc::SetAppendText(this, IDC_EDIT_RECV, strRecvInfo);
}

void CAsyncClientDlg::OnFDWrite()  
{
	//�����ӳɹ��󣬽��ܵ�дд��Ϣ��˵�����ڿ��Ե���send������Ϣ��
	//(1)����һ�ֿ����ǣ�������send��Ϣ�����ͻ�������մ��������ʧ�ܣ������ͻ��������ݷ��ͳ�ȥ��
	//	�����ٴη�������ʱ������ܵ�����Ϣ
	CAsyncFunc::SetAppendText(this, IDC_EDIT_RECV, _T("д���ݳɹ�:"));
}
void CAsyncClientDlg::OnFDClose()
{
    closesocket(m_sClient);
    m_bSocketClose = true;

    CAsyncFunc::SetAppendText(this, IDC_EDIT_RECV, _T("������ѹر�"));

    GetDlgItem(IDC_EDIT_SEND)->EnableWindow(FALSE);
    GetDlgItem(IDC_BTN_SEND)->EnableWindow(FALSE);
}

BOOL CAsyncClientDlg::PreTranslateMessage(MSG* pMsg)
{
    //�Ի�������Enter��ESC��
    if (WM_KEYDOWN == pMsg->message)
    {
        //�س�����Ӧ��Ϣ����
        if (VK_RETURN == pMsg->wParam)
        {
            if (GetDlgItem(IDC_BTN_SEND)->IsWindowEnabled())
            {
                OnBnClickedBtnSend();
            }
        }

        if (    VK_RETURN == pMsg->wParam
            || VK_ESCAPE == pMsg->wParam)
        {
            return TRUE;
        }
    }

    return CDialog::PreTranslateMessage(pMsg);
}


void CAsyncClientDlg::OnClose()
{
    // TODO: Add your message handler code here and/or call default
    if (!m_bSocketClose)
    {
        closesocket(m_sClient);
    }

    WSACleanup();
    
    CDialog::OnClose();
}
