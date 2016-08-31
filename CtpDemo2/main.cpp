#include <iostream>
#include "MyTraderSpi.h"
#include "MyMdSpi.h"

// UserApi对象
CThostFtdcTraderApi* pUserApi;
CThostFtdcMdApi* pUserApiMd;

// char FRONT_ADDR[] = "tcp://180.168.146.187:10000";     // 前置地址
char FRONT_ADDR[] = "tcp://180.168.146.187:10031";     // 前置地址
TThostFtdcBrokerIDType BROKER_ID = "9999";             // 经纪公司代码
TThostFtdcInvestorIDType INVESTOR_ID = "069637";       // 投资者代码
TThostFtdcPasswordType PASSWORD = "sim0594184520";     // 用户密码

TThostFtdcInstrumentIDType INSTRUMENT_ID = "cu0909";   // 合约代码
TThostFtdcDirectionType DIRECTION = THOST_FTDC_D_Sell; // 买卖方向
TThostFtdcPriceType LIMIT_PRICE = 38850;               // 价格

char *a[] = { "C1606", "RB1701", "ru1701", "FG401", "CU1610" };
int count = 5;
// 请求编号
int iRequestID = 0;

int main()
{
    // 初始化UserApi
//     pUserApi = CThostFtdcTraderApi::CreateFtdcTraderApi(); // 创建UserApi
//     CTraderSpi* pUserSpi = new CTraderSpi();
//     pUserApi->RegisterSpi((CThostFtdcTraderSpi*)pUserSpi); // 注册事件类
//     pUserApi->SubscribePublicTopic(THOST_TERT_RESTART);    // 注册公有流
//     pUserApi->SubscribePrivateTopic(THOST_TERT_RESTART);   // 注册私有流
//     pUserApi->RegisterFront(FRONT_ADDR);                   // connect
//     pUserApi->Init();

	// 初始化UserApiMd
	pUserApiMd = CThostFtdcMdApi::CreateFtdcMdApi("./Md"); // 创建UserApi
	CThostFtdcMdSpi* pUserSpiMd = new CMdSpi();
	pUserApiMd->RegisterSpi((CThostFtdcMdSpi*)pUserSpiMd); // 注册事件类
// 	pUserApiMd->SubscribePublicTopic(THOST_TERT_RESTART);    // 注册公有流
// 	pUserApiMd->SubscribePrivateTopic(THOST_TERT_RESTART);   // 注册私有流
//	TThostFtdcInstrumentIDType a[] = { "C1606", "C1607", "ZC701", "FG401", "CU1610" };
	pUserApiMd->RegisterFront(FRONT_ADDR);                   // connect
	pUserApiMd->Init();
	
    //pUserApi->Join();
	pUserApiMd->Join();
    // pUserApi->Release();

    return 0;
}
