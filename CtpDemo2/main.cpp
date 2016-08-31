#include <iostream>
#include "MyTraderSpi.h"
#include "MyMdSpi.h"

// UserApi����
CThostFtdcTraderApi* pUserApi;
CThostFtdcMdApi* pUserApiMd;

// char FRONT_ADDR[] = "tcp://180.168.146.187:10000";     // ǰ�õ�ַ
char FRONT_ADDR[] = "tcp://180.168.146.187:10031";     // ǰ�õ�ַ
TThostFtdcBrokerIDType BROKER_ID = "9999";             // ���͹�˾����
TThostFtdcInvestorIDType INVESTOR_ID = "069637";       // Ͷ���ߴ���
TThostFtdcPasswordType PASSWORD = "sim0594184520";     // �û�����

TThostFtdcInstrumentIDType INSTRUMENT_ID = "cu0909";   // ��Լ����
TThostFtdcDirectionType DIRECTION = THOST_FTDC_D_Sell; // ��������
TThostFtdcPriceType LIMIT_PRICE = 38850;               // �۸�

char *a[] = { "C1606", "RB1701", "ru1701", "FG401", "CU1610" };
int count = 5;
// ������
int iRequestID = 0;

int main()
{
    // ��ʼ��UserApi
//     pUserApi = CThostFtdcTraderApi::CreateFtdcTraderApi(); // ����UserApi
//     CTraderSpi* pUserSpi = new CTraderSpi();
//     pUserApi->RegisterSpi((CThostFtdcTraderSpi*)pUserSpi); // ע���¼���
//     pUserApi->SubscribePublicTopic(THOST_TERT_RESTART);    // ע�ṫ����
//     pUserApi->SubscribePrivateTopic(THOST_TERT_RESTART);   // ע��˽����
//     pUserApi->RegisterFront(FRONT_ADDR);                   // connect
//     pUserApi->Init();

	// ��ʼ��UserApiMd
	pUserApiMd = CThostFtdcMdApi::CreateFtdcMdApi("./Md"); // ����UserApi
	CThostFtdcMdSpi* pUserSpiMd = new CMdSpi();
	pUserApiMd->RegisterSpi((CThostFtdcMdSpi*)pUserSpiMd); // ע���¼���
// 	pUserApiMd->SubscribePublicTopic(THOST_TERT_RESTART);    // ע�ṫ����
// 	pUserApiMd->SubscribePrivateTopic(THOST_TERT_RESTART);   // ע��˽����
//	TThostFtdcInstrumentIDType a[] = { "C1606", "C1607", "ZC701", "FG401", "CU1610" };
	pUserApiMd->RegisterFront(FRONT_ADDR);                   // connect
	pUserApiMd->Init();
	
    //pUserApi->Join();
	pUserApiMd->Join();
    // pUserApi->Release();

    return 0;
}
