#pragma once
#include "ThostFtdcMdApi.h"


class CMdSpi : public CThostFtdcMdSpi
{
public:
    // 当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
    virtual void OnFrontConnected();
    // 登录请求响应
    virtual void OnRspUserLogin(CThostFtdcRspUserLoginField*, CThostFtdcRspInfoField*, int, bool);
	///订阅行情应答
	virtual void OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

private:
    // 用户登录请求
    void ReqUserLogin();
	// 是否收到成功的响应
	bool IsErrorRspInfo(const CThostFtdcRspInfoField* const pRspInfo) const;
	void SubscribeMarketData();
    void ShowRspUserLoginField(const CThostFtdcRspUserLoginField* const) const;
};
