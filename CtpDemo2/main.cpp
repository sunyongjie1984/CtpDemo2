#include <iostream>
#include "MyMdSpi.h"

CThostFtdcMdApi* pMdApi;

char FRONT_ADDR[] = "tcp://180.168.146.187:10031";     // 前置地址
//char FRONT_ADDR[] = "tcp://180.168.146.187:10010";     // 前置地址
TThostFtdcBrokerIDType BROKER_ID = "9999";             // 经纪公司代码
TThostFtdcInvestorIDType INVESTOR_ID = "069637";       // 投资者代码
TThostFtdcPasswordType PASSWORD = "sim0594184520";     // 用户密码

const char *a[] = { "ag1612", "rb1610", "ru1609", "TA609", "IF1609", "ni1609", "m1609", "p1609" };
int count = 8;
int iRequestID = 0;

int main()
{
    pMdApi = CThostFtdcMdApi::CreateFtdcMdApi(".\\Md\\", true); // 创建UserApi
    CThostFtdcMdSpi* pMdSpi = new CMdSpi();
    pMdApi->RegisterSpi(pMdSpi); // 注册事件类
    pMdApi->RegisterFront(FRONT_ADDR);                   // connect
    pMdApi->Init();

    pMdApi->Join();
    return 0;
}
