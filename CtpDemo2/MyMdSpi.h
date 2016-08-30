#pragma once
#include "ThostFtdcMdApi.h"


class CMdSpi : public CThostFtdcMdSpi
{
public:
    // ���ͻ����뽻�׺�̨������ͨ������ʱ����δ��¼ǰ�����÷��������á�
    virtual void OnFrontConnected();
    // ��¼������Ӧ
    virtual void OnRspUserLogin(CThostFtdcRspUserLoginField*, CThostFtdcRspInfoField*, int, bool);

private:
    // �û���¼����
    void ReqUserLogin();
	// �Ƿ��յ��ɹ�����Ӧ
	bool IsErrorRspInfo(const CThostFtdcRspInfoField* const pRspInfo) const;

    void ShowRspUserLoginField(const CThostFtdcRspUserLoginField* const) const;
};