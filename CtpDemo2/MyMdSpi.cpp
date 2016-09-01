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

// USER_API参数
extern CThostFtdcMdApi* pMdApi;

// 配置参数
extern char BROKER_ID[];                  // 经纪公司代码
extern char INVESTOR_ID[];                // 投资者代码
extern char PASSWORD[];                   // 用户密码

extern char* a[];
extern int count;

// 请求编号
extern int iRequestID;

// 会话参数
TThostFtdcFrontIDType FRONT_ID_MD;     // 前置编号
TThostFtdcSessionIDType SESSION_ID_MD; // 会话编号
TThostFtdcOrderRefType ORDER_REF_MD;   // 报单引用

void CMdSpi::OnFrontConnected()
{
    cout << "--->>> " << "OnFrontConnected" << endl;
    ///用户登录请求
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
    cerr << "--->>> 发送用户登录请求: " << ((iResult == 0) ? "成功" : "失败") << endl;
}

bool CMdSpi::IsErrorRspInfo(const CThostFtdcRspInfoField* const pRspInfo) const
{
	// 如果0 != ErrorID, 说明收到了错误的响应
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
        // 保存会话参数
        FRONT_ID_MD = pRspUserLogin->FrontID;
        SESSION_ID_MD = pRspUserLogin->SessionID;
        int iNextOrderRef = atoi(pRspUserLogin->MaxOrderRef);
        iNextOrderRef++;
        sprintf(ORDER_REF_MD, "%d", iNextOrderRef);
        // 获取当前交易日
        cerr << "--->>> 获取当前交易日 = " << pMdApi->GetTradingDay() << endl;
        // 投资者结算结果确认
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

///订阅行情应答
void OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    std::cout << __FILE__ << " " << __FUNCTION__ << std::endl;
    cerr << "--->>> " << "订阅行情品种：" <<pSpecificInstrument->InstrumentID<< endl;
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
    cerr << "--->>> 发送行情订阅请求: " << ((iResult == 0) ? "成功" : "失败") << endl;
}

void CMdSpi::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData)
{
    cerr << __FUNCTION__ << "\t" << pDepthMarketData->TradingDay <<
        "\t" << pDepthMarketData->UpdateTime << "\t" << pDepthMarketData->InstrumentID << "\t" <<
        pDepthMarketData->LastPrice << "\t" << pDepthMarketData->OpenPrice << "\t" << pDepthMarketData->LowestPrice << "\t" << pDepthMarketData->HighestPrice
        << endl;
}
