#include <iostream>
#include "MyMdSpi.h"

#ifdef _linux
#include <string.h>
#endif

using std::cerr;
using std::endl;
using std::cout;

// USER_API参数
extern CThostFtdcMdApi* pUserApiMd;

// 配置参数
extern char FRONT_ADDR[];                 // 前置地址
extern char BROKER_ID[];                  // 经纪公司代码
extern char INVESTOR_ID[];                // 投资者代码
extern char PASSWORD[];                   // 用户密码
extern char INSTRUMENT_ID[];              // 合约代码
extern TThostFtdcPriceType LIMIT_PRICE;   // 价格
extern TThostFtdcDirectionType DIRECTION; // 买卖方向

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
    int iResult = pUserApiMd->ReqUserLogin(&req, ++iRequestID);
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
        cerr << "--->>> 获取当前交易日 = " << pUserApiMd->GetTradingDay() << endl;
        // 投资者结算结果确认
        //ReqSettlementInfoConfirm();
    }
    if (nullptr != pRspInfo)
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

///订阅行情应答
void OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
        std::cout << __FILE__ << std::endl;

}

void CMdSpi::ShowRspUserLoginField(const CThostFtdcRspUserLoginField* const pRspUserLogin) const
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

void CMdSpi::SubscribeMarketData()
{
	int iResult = pUserApiMd->SubscribeMarketData(a, count);
	cerr << "--->>> 发送行情订阅请求: " << ((iResult == 0) ? "成功" : "失败") << endl;

}