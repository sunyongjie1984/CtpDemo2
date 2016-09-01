#include <iostream>
#include "MyMdSpi.h"

#ifdef _linux
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#endif

using std::cerr;
using std::endl;
using std::cout;

// USER_API����
extern CThostFtdcMdApi* pMdApi;

// ���ò���
extern char BROKER_ID[];                  // ���͹�˾����
extern char INVESTOR_ID[];                // Ͷ���ߴ���
extern char PASSWORD[];                   // �û�����

extern char* a[];
extern int count;

// ������
extern int iRequestID;

// �Ự����
TThostFtdcFrontIDType FRONT_ID_MD;     // ǰ�ñ��
TThostFtdcSessionIDType SESSION_ID_MD; // �Ự���
TThostFtdcOrderRefType ORDER_REF_MD;   // ��������

void CMdSpi::OnFrontConnected()
{
    cout << "--->>> " << "OnFrontConnected" << endl;
    ///�û���¼����
    ReqUserLogin();
}

void CMdSpi::ReqUserLogin()
{
    CThostFtdcReqUserLoginField req;
    memset(&req, 0, sizeof(req));
    strcpy(req.BrokerID, BROKER_ID);
    strcpy(req.UserID, INVESTOR_ID);
    strcpy(req.Password, PASSWORD);
    int iResult = pMdApi->ReqUserLogin(&req, ++iRequestID);
    cerr << "--->>> �����û���¼����: " << ((iResult == 0) ? "�ɹ�" : "ʧ��") << endl;
}

bool CMdSpi::IsErrorRspInfo(const CThostFtdcRspInfoField* const pRspInfo) const
{
	// ���0 != ErrorID, ˵���յ��˴������Ӧ
	bool bResult = ((pRspInfo) && (pRspInfo->ErrorID != 0));
	if (bResult)
		cerr << "--->>> ErrorID=" << pRspInfo->ErrorID << ", ErrorMsg=" << pRspInfo->ErrorMsg << endl;
	return bResult;
}

void CMdSpi::OnRspUserLogin(CThostFtdcRspUserLoginField* pRspUserLogin, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
    cerr << "--->>> " << "OnRspUserLogin" << endl;
    if (bIsLast && !IsErrorRspInfo(pRspInfo))
    {
        // ����Ự����
        FRONT_ID_MD = pRspUserLogin->FrontID;
        SESSION_ID_MD = pRspUserLogin->SessionID;
        int iNextOrderRef = atoi(pRspUserLogin->MaxOrderRef);
        iNextOrderRef++;
        sprintf(ORDER_REF_MD, "%d", iNextOrderRef);
        // ��ȡ��ǰ������
        cerr << "--->>> ��ȡ��ǰ������ = " << pMdApi->GetTradingDay() << endl;
        // Ͷ���߽�����ȷ��
        //ReqSettlementInfoConfirm();
    }
    if (NULL != pRspInfo)
    {
        if (0 == pRspInfo->ErrorID)
        {
            std::cout << "log in successfully" << std::endl;
			SubscribeMarketData();
        }
        else
        {
            std::cout << "log in failed" << std::endl;
            std::cout << "Error ID = " << pRspInfo->ErrorID << std::endl;
            std::cout << "Error Msg = " << pRspInfo->ErrorMsg << std::endl;
        }
}
    return;
}

///��������Ӧ��
void OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    std::cout << __FILE__ << " " << __FUNCTION__ << std::endl;
    cerr << "--->>> " << "��������Ʒ�֣�" <<pSpecificInstrument->InstrumentID<< endl;
}

void CMdSpi::OnRspError(CThostFtdcRspInfoField *pRspInfo,
		int nRequestID, bool bIsLast)
{
	cerr << "--->>> "<< __FUNCTION__ << endl;
	IsErrorRspInfo(pRspInfo);
}

void CMdSpi::ShowRspUserLoginField(const CThostFtdcRspUserLoginField* const pRspUserLogin) const
{
    if (NULL != pRspUserLogin)
    {
        std::cout << "date: " << pRspUserLogin->TradingDay << std::endl;
        std::cout << "login time: " << pRspUserLogin->LoginTime << std::endl;
        std::cout << "broker: " << pRspUserLogin->BrokerID << std::endl;
        std::cout << "user: " << pRspUserLogin->UserID << std::endl;
        std::cout << "system name: " << pRspUserLogin->SystemName << std::endl;
    }
    else
    {
        std::cout << "date: " << pRspUserLogin->TradingDay << std::endl;
        std::cout << "login time: " << pRspUserLogin->LoginTime << std::endl;
        std::cout << "broker: " << pRspUserLogin->BrokerID << std::endl;
        std::cout << "user: " << pRspUserLogin->UserID << std::endl;
        std::cout << "system name: " << pRspUserLogin->SystemName << std::endl;
    }
    return;
}

void CMdSpi::SubscribeMarketData()
{
    int iResult = pMdApi->SubscribeMarketData(a, count);
    cerr << "--->>> �������鶩������: " << ((iResult == 0) ? "�ɹ�" : "ʧ��") << endl;
}

void CMdSpi::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData)
{
    cerr << __FUNCTION__ << "\t" << pDepthMarketData->TradingDay <<
        "\t" << pDepthMarketData->UpdateTime << "\t" << pDepthMarketData->InstrumentID << "\t" <<
        pDepthMarketData->LastPrice << "\t" << pDepthMarketData->OpenPrice << "\t" << pDepthMarketData->LowestPrice << "\t" << pDepthMarketData->HighestPrice
        << endl;
}
