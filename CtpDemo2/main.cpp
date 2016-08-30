#include <iostream>
#include "MyTraderSpi.h"
#include "MyMdSpi.h"

// UserApi����
CThostFtdcTraderApi* pUserApi;
CThostFtdcMdApi* pUserApiMd;

// char FRONT_ADDR[] = "tcp://180.168.146.187:10000";     // ǰ�õ�ַ
char FRONT_ADDR[] = "tcp://180.168.146.187:10030";     // ǰ�õ�ַ
TThostFtdcBrokerIDType BROKER_ID = "9999";             // ���͹�˾����
TThostFtdcInvestorIDType INVESTOR_ID = "069637";       // Ͷ���ߴ���
TThostFtdcPasswordType PASSWORD = "sim0594184520";     // �û�����

TThostFtdcInstrumentIDType INSTRUMENT_ID = "cu0909";   // ��Լ����
TThostFtdcDirectionType DIRECTION = THOST_FTDC_D_Sell; // ��������
TThostFtdcPriceType LIMIT_PRICE = 38850;               // �۸�

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
	pUserApiMd = CThostFtdcMdApi::CreateFtdcMdApi("Md"); // ����UserApi
	CMdSpi* pUserSpiMd = new CMdSpi();
	pUserApiMd->RegisterSpi((CThostFtdcMdSpi*)pUserSpiMd); // ע���¼���
// 	pUserApiMd->SubscribePublicTopic(THOST_TERT_RESTART);    // ע�ṫ����
// 	pUserApiMd->SubscribePrivateTopic(THOST_TERT_RESTART);   // ע��˽����
	pUserApiMd->RegisterFront(FRONT_ADDR);                   // connect
	pUserApiMd->Init();

    pUserApi->Join();
	pUserApiMd->Join();

    // pUserApi->Release();

    return 0;
}
