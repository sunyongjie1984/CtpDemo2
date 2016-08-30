#include <iostream>
#include "MyTraderSpi.h"

#ifdef _linux
#include <string.h>
#endif

using std::cerr;
using std::endl;
using std::cout;

// USER_API����
extern CThostFtdcTraderApi* pUserApi;

// ���ò���
extern char FRONT_ADDR[];                 // ǰ�õ�ַ
extern char BROKER_ID[];                  // ���͹�˾����
extern char INVESTOR_ID[];                // Ͷ���ߴ���
extern char PASSWORD[];                   // �û�����
extern char INSTRUMENT_ID[];              // ��Լ����
extern TThostFtdcPriceType LIMIT_PRICE;   // �۸�
extern TThostFtdcDirectionType DIRECTION; // ��������

// ������
extern int iRequestID;

// �Ự����
TThostFtdcFrontIDType FRONT_ID;     // ǰ�ñ��
TThostFtdcSessionIDType SESSION_ID; // �Ự���
TThostFtdcOrderRefType ORDER_REF;   // ��������

void CTraderSpi::OnFrontConnected()
{
    cout << "--->>> " << "OnFrontConnected" << endl;
    ///�û���¼����
    ReqUserLogin();
}

void CTraderSpi::ReqUserLogin()
{
    CThostFtdcReqUserLoginField req;
    memset(&req, 0, sizeof(req));
    strcpy(req.BrokerID, BROKER_ID);
    strcpy(req.UserID, INVESTOR_ID);
    strcpy(req.Password, PASSWORD);
    int iResult = pUserApi->ReqUserLogin(&req, ++iRequestID);
    cerr << "--->>> �����û���¼����: " << ((iResult == 0) ? "�ɹ�" : "ʧ��") << endl;
}

void CTraderSpi::OnRspUserLogin(CThostFtdcRspUserLoginField* pRspUserLogin, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
    cerr << "--->>> " << "OnRspUserLogin" << endl;
    if (bIsLast && !IsErrorRspInfo(pRspInfo))
    {
        // ����Ự����
        FRONT_ID = pRspUserLogin->FrontID;
        SESSION_ID = pRspUserLogin->SessionID;
        int iNextOrderRef = atoi(pRspUserLogin->MaxOrderRef);
        iNextOrderRef++;
        sprintf(ORDER_REF, "%d", iNextOrderRef);
        // ��ȡ��ǰ������
        cerr << "--->>> ��ȡ��ǰ������ = " << pUserApi->GetTradingDay() << endl;
        // Ͷ���߽�����ȷ��
        ReqSettlementInfoConfirm();
    }
    if (nullptr != pRspInfo)
    {
        if (0 == pRspInfo->ErrorID)
        {
            std::cout << "log in successfully" << std::endl;
        }
        else
        {
            std::cout << "log in failed" << std::endl;
            std::cout << "Error ID = " << pRspInfo->ErrorID << std::endl;
            std::cout << "Error Msg = " << pRspInfo->ErrorMsg << std::endl;
        }
        if (nullptr != pRspUserLogin)
        {
            ShowRspUserLoginField(pRspUserLogin);
        }
        else
        {
            std::cout << "Error !!! pRspUserLogin is nullptr" << std::endl;
        }
    }
    else
    {
            std::cout << "Error !!! pRspInfo is nullptr" << std::endl;
    }
    return;
}

void CTraderSpi::ReqSettlementInfoConfirm()
{
    CThostFtdcSettlementInfoConfirmField req;
    memset(&req, 0, sizeof(req));
    strcpy(req.BrokerID, BROKER_ID);
    strcpy(req.InvestorID, INVESTOR_ID);
    int iResult = pUserApi->ReqSettlementInfoConfirm(&req, ++iRequestID);
    cerr << "--->>> Ͷ���߽�����ȷ��: " << ((iResult == 0) ? "�ɹ�" : "ʧ��") << endl;
}

void CTraderSpi::OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    cerr << "--->>> " << "OnRspSettlementInfoConfirm" << endl;
    if (bIsLast && !IsErrorRspInfo(pRspInfo))
    {
        // �����ѯ��Լ
        ReqQryInstrument();
    }
}
void CTraderSpi::ReqQryInstrument()
{
    CThostFtdcQryInstrumentField req;
    memset(&req, 0, sizeof(req));
    strcpy(req.InstrumentID, INSTRUMENT_ID);
    int iResult = pUserApi->ReqQryInstrument(&req, ++iRequestID);
    cerr << "--->>> �����ѯ��Լ: " << ((iResult == 0) ? "�ɹ�" : "ʧ��") << endl;
}

void CTraderSpi::OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    cerr << "--->>> " << "OnRspQryInstrument" << endl;
    if (bIsLast && !IsErrorRspInfo(pRspInfo))
    {
        //�����ѯ��Լ
        ReqQryTradingAccount();
    }
}

void CTraderSpi::ReqQryTradingAccount()
{
    CThostFtdcQryTradingAccountField req;
    memset(&req, 0, sizeof(req));
    strcpy(req.BrokerID, BROKER_ID);
    strcpy(req.InvestorID, INVESTOR_ID);
    int iResult = pUserApi->ReqQryTradingAccount(&req, ++iRequestID);
    cerr << "--->>> �����ѯ�ʽ��˻�: " << ((iResult == 0) ? "�ɹ�" : "ʧ��") << endl;
}

void CTraderSpi::OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    cerr << "--->>> " << "OnRspQryTradingAccount" << endl;
    if (bIsLast && !IsErrorRspInfo(pRspInfo))
    {
        // �����ѯͶ���ֲ߳�
        ReqQryInvestorPosition();
    }
}

void CTraderSpi::ReqQryInvestorPosition()
{
    CThostFtdcQryInvestorPositionField req;
    memset(&req, 0, sizeof(req));
    strcpy(req.BrokerID, BROKER_ID);
    strcpy(req.InvestorID, INVESTOR_ID);
    strcpy(req.InstrumentID, INSTRUMENT_ID);
    int iResult = pUserApi->ReqQryInvestorPosition(&req, ++iRequestID);
    cerr << "--->>> �����ѯͶ���ֲ߳�: " << ((iResult == 0) ? "�ɹ�" : "ʧ��") << endl;
}

void CTraderSpi::OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    cerr << "--->>> " << "OnRspQryInvestorPosition" << endl;
    if (bIsLast && !IsErrorRspInfo(pRspInfo))
    {
        // ����¼������
        ReqOrderInsert();
    }
}

void CTraderSpi::ReqOrderInsert()
{
    CThostFtdcInputOrderField req;
    memset(&req, 0, sizeof(req));
    // ���͹�˾����
    strcpy(req.BrokerID, BROKER_ID);
    ///Ͷ���ߴ���
    strcpy(req.InvestorID, INVESTOR_ID);
    ///��Լ����
    strcpy(req.InstrumentID, INSTRUMENT_ID);
    ///��������
    strcpy(req.OrderRef, ORDER_REF);
    ///�û�����
    //  TThostFtdcUserIDType    UserID;
    ///�����۸�����: �޼�
    req.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
    ///��������: 
    req.Direction = DIRECTION;
    ///��Ͽ�ƽ��־: ����
    req.CombOffsetFlag[0] = THOST_FTDC_OF_Open;
    ///���Ͷ���ױ���־
    req.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;
    ///�۸�
    req.LimitPrice = LIMIT_PRICE;
    ///����: 1
    req.VolumeTotalOriginal = 1;
    ///��Ч������: ������Ч
    req.TimeCondition = THOST_FTDC_TC_GFD;
    ///GTD����
    //  TThostFtdcDateType  GTDDate;
    ///�ɽ�������: �κ�����
    req.VolumeCondition = THOST_FTDC_VC_AV;
    ///��С�ɽ���: 1
    req.MinVolume = 1;
    ///��������: ����
    req.ContingentCondition = THOST_FTDC_CC_Immediately;
    ///ֹ���
    //  TThostFtdcPriceType StopPrice;
    ///ǿƽԭ��: ��ǿƽ
    req.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
    ///�Զ������־: ��
    req.IsAutoSuspend = 0;
    ///ҵ��Ԫ
    //  TThostFtdcBusinessUnitType  BusinessUnit;
    ///������
    //  TThostFtdcRequestIDType RequestID;
    ///�û�ǿ����־: ��
    req.UserForceClose = 0;

    int iResult = pUserApi->ReqOrderInsert(&req, ++iRequestID);
    cerr << "--->>> ����¼������: " << ((iResult == 0) ? "�ɹ�" : "ʧ��") << endl;
}
void CTraderSpi::OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    cerr << "--->>> " << "OnRspOrderInsert" << endl;
    IsErrorRspInfo(pRspInfo);
}
void CTraderSpi::ReqOrderAction(CThostFtdcOrderField *pOrder)
{
    static bool ORDER_ACTION_SENT = false;      //�Ƿ����˱���
    if (ORDER_ACTION_SENT)
        return;

    CThostFtdcInputOrderActionField req;
    memset(&req, 0, sizeof(req));
    ///���͹�˾����
    strcpy(req.BrokerID, pOrder->BrokerID);
    ///Ͷ���ߴ���
    strcpy(req.InvestorID, pOrder->InvestorID);
    ///������������
    //  TThostFtdcOrderActionRefType    OrderActionRef;
    ///��������
    strcpy(req.OrderRef, pOrder->OrderRef);
    ///������
    //  TThostFtdcRequestIDType RequestID;
    ///ǰ�ñ��
    req.FrontID = FRONT_ID;
    ///�Ự���
    req.SessionID = SESSION_ID;
    ///����������
    //  TThostFtdcExchangeIDType    ExchangeID;
    ///�������
    //  TThostFtdcOrderSysIDType    OrderSysID;
    ///������־
    req.ActionFlag = THOST_FTDC_AF_Delete;
    ///�۸�
    //  TThostFtdcPriceType LimitPrice;
    ///�����仯
    //  TThostFtdcVolumeType    VolumeChange;
    ///�û�����
    //  TThostFtdcUserIDType    UserID;
    ///��Լ����
    strcpy(req.InstrumentID, pOrder->InstrumentID);

    int iResult = pUserApi->ReqOrderAction(&req, ++iRequestID);
    cerr << "--->>> ������������: " << ((iResult == 0) ? "�ɹ�" : "ʧ��") << endl;
    ORDER_ACTION_SENT = true;
}
void CTraderSpi::OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    cerr << "--->>> " << "OnRspOrderAction" << endl;
    IsErrorRspInfo(pRspInfo);
}

// ����֪ͨ
void CTraderSpi::OnRtnOrder(CThostFtdcOrderField *pOrder)
{
    cerr << "--->>> " << "OnRtnOrder"  << endl;
    if (IsMyOrder(pOrder))
    {
        if (IsTradingOrder(pOrder))
            ReqOrderAction(pOrder);
        else if (pOrder->OrderStatus == THOST_FTDC_OST_Canceled)
            cout << "--->>> �����ɹ�" << endl;
    }
}

// �ɽ�֪ͨ
void CTraderSpi::OnRtnTrade(CThostFtdcTradeField *pTrade)
{
    cerr << "--->>> " << "OnRtnTrade"  << endl;
}

void CTraderSpi::OnFrontDisconnected(int nReason)
{
    cerr << "--->>> " << "OnFrontDisconnected" << endl;
    cerr << "--->>> Reason = " << nReason << endl;
}

void CTraderSpi::OnHeartBeatWarning(int nTimeLapse)
{
    cerr << "--->>> " << "OnHeartBeatWarning" << endl;
    cerr << "--->>> nTimerLapse = " << nTimeLapse << endl;
}

void CTraderSpi::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    cerr << "--->>> " << "OnRspError" << endl;
    IsErrorRspInfo(pRspInfo);
}

bool CTraderSpi::IsErrorRspInfo(const CThostFtdcRspInfoField* const pRspInfo) const
{
    // ���0 != ErrorID, ˵���յ��˴������Ӧ
    bool bResult = ((pRspInfo) && (pRspInfo->ErrorID != 0));
    if (bResult)
        cerr << "--->>> ErrorID=" << pRspInfo->ErrorID << ", ErrorMsg=" << pRspInfo->ErrorMsg << endl;
    return bResult;
}

bool CTraderSpi::IsMyOrder(CThostFtdcOrderField *pOrder)
{
        return ((pOrder->FrontID == FRONT_ID) &&
                            (pOrder->SessionID == SESSION_ID) &&
                                        (strcmp(pOrder->OrderRef, ORDER_REF) == 0));
}

bool CTraderSpi::IsTradingOrder(CThostFtdcOrderField *pOrder)
{
        return ((pOrder->OrderStatus != THOST_FTDC_OST_PartTradedNotQueueing) &&
                            (pOrder->OrderStatus != THOST_FTDC_OST_Canceled) &&
                                        (pOrder->OrderStatus != THOST_FTDC_OST_AllTraded));
}

void CTraderSpi::ShowRspUserLoginField(const CThostFtdcRspUserLoginField* const pRspUserLogin) const
{
    if (nullptr != pRspUserLogin)
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