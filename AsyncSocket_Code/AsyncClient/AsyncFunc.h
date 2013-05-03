#pragma once
#include <string>
using std::wstring;
using std::string;

class CAsyncFunc
{
public:
    CAsyncFunc(void);
    ~CAsyncFunc(void);

    static wstring AnsiToUnicode(const string& str);
    static string UnicodeToAnsi(const wstring& str);
    /*
    @brief ��׷��ģʽ���ÿؼ���Ϣ
    @param[in] pWnd ������
    @param[in] nID �ؼ�ID
    @param[in] strInfo �ؼ���Ϣ
    */
    static void SetAppendText(CWnd* pWnd, int nID, const CString& strInfo);
};
