/*
@brief �첽ͨ�ŶԻ��������
*/
#pragma once
#include "IMsgInterflow.h"
#include <map>
#include <vector>
using namespace std;

class CAsyncTalkDlg;
class CAsyncTalkMng
{
public:
    static CAsyncTalkMng* GetInstance();
    static void ReleaseInstance();

    void StartTalkDlg(SOCKET sClient, IMsgInterflow* pMsgFlow);
    void StopTalkDlg(SOCKET sClient);
    //ɾ�����ۿ�
    void DeleteTalkDlg(SOCKET sClient);

    void AddClient(StClientSock* pClientSock);
    void RemoveClient(SOCKET sClient);

    void CloseAllClient();

protected:
    CAsyncTalkMng(void);
    ~CAsyncTalkMng(void);

private:
    static CAsyncTalkMng* m_pInstance;

    map<SOCKET, StClientSock*> m_SocketMap;
    map<SOCKET, CAsyncTalkDlg*> m_TalkDlgMap;
};
