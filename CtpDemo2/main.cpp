#include <iostream>
#include "MyMdSpi.h"

CThostFtdcMdApi* pMdApi;

char FRONT_ADDR[] = "tcp://180.168.146.187:10031";     // ǰ�õ�ַ
//char FRONT_ADDR[] = "tcp://180.168.146.187:10010";     // ǰ�õ�ַ
TThostFtdcBrokerIDType BROKER_ID = "9999";             // ���͹�˾����
TThostFtdcInvestorIDType INVESTOR_ID = "069637";       // Ͷ���ߴ���
TThostFtdcPasswordType PASSWORD = "sim0594184520";     // �û�����

const char *a[] = { "ag1612", "rb1610", "ru1609", "TA609", "IF1609", "ni1609", "m1609", "p1609" };
int count = 8;
int iRequestID = 0;

int main()
{
    pMdApi = CThostFtdcMdApi::CreateFtdcMdApi(".\\Md\\", true); // ����UserApi
    CThostFtdcMdSpi* pMdSpi = new CMdSpi();
    pMdApi->RegisterSpi(pMdSpi); // ע���¼���
    pMdApi->RegisterFront(FRONT_ADDR);                   // connect
    pMdApi->Init();

    pMdApi->Join();
    return 0;
}
