/*
@brief ��Ϣ�����ӿ�
*/
#pragma once
#include "AsyncServDefine.h"

class IMsgInterflow
{
public:
    //��Ӧ�׽��ֹر�
    virtual void OnFDClose(SOCKET sClient) = 0;

protected:
    virtual ~IMsgInterflow() {}
};