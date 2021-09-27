/* -*- C++ -*- */

#ifndef EXAMPLES_CLIENT_APPLICATION_H_
#define EXAMPLES_CLIENT_APPLICATION_H_

#include <cmath>
#include <iomanip>
#include <iostream>
#include <string>
#include <ctime>

#include "quickfix/Application.h"
#include "quickfix/MessageCracker.h"
#include "quickfix/Mutex.h"
#include "quickfix/Values.h"

// client -> server
#include "quickfix/fix44/MarketDataRequest.h"              // < V >
#include "quickfix/fix44/SecurityListRequest.h"            // < x >
#include "quickfix/fix44/NewOrderSingle.h"                 // < D >
#include "quickfix/fix44/OrderMassStatusRequest.h"         // < AF >
#include "quickfix/fix44/OrderCancelRequest.h"             // < F >

// server -> client
#include "quickfix/fix44/MarketDataSnapshotFullRefresh.h"  // < W >
#include "quickfix/fix44/SecurityList.h"                   // < y >
#include "quickfix/fix44/ExecutionReport.h"                // < 8 >

// const
const char SessionTypeQUOTE[] = "QUOTE";
const char SessionTypeTRADE[] = "TRADE";

// class Application
class Application : public FIX::Application, public FIX::MessageCracker {
 public:
  explicit Application(const FIX::SessionSettings &settings) : m_settings(settings) {}

  void run();

 private:
  FIX::SessionSettings m_settings;
  int cnt = 0;
  int SYMBOL_DIGIT = 0;
  std::string SYMBOL_ID = "";
  std::string SYMBOL_NAME = "";
  std::string ORDER_ID = "";
  int ORDER_COUNT = 0;
  int TRADE_PIP = 0;
  std::string ORDER_SIDE = "";
  std::string PositionID = "";
  double TRADE_PX = 0.0;
  long SIZE = 0;
  int RANGE = 0;
  int STOP = 0;
  long MAXSEC = 0;
  int SPREAD = 0;

  void onCreate(const FIX::SessionID &) {}
  void onLogon(const FIX::SessionID &sessionID);
  void onLogout(const FIX::SessionID &sessionID);
  void toAdmin(FIX::Message &, const FIX::SessionID &);
  void toApp(FIX::Message &, const FIX::SessionID &) EXCEPT(FIX::DoNotSend);
  void fromAdmin(const FIX::Message &message, const FIX::SessionID &sessionID)
      EXCEPT(FIX::FieldNotFound, FIX::IncorrectDataFormat, FIX::IncorrectTagValue, FIX::RejectLogon) {}
  void fromApp(const FIX::Message &message, const FIX::SessionID &sessionID)
      EXCEPT(FIX::FieldNotFound, FIX::IncorrectDataFormat, FIX::IncorrectTagValue, FIX::UnsupportedMessageType);

  // client -> server
  /* x  */ void SecurityListRequest();
  /* V  */ void MarketDataRequest(/* 55  symbol */ std::string);
  /* D  */ void NewOrderSingle(/*  54 side  */ const char &,   /* Side_BUY = '1', Side_SELL = '2'; */
                               /*  38 qty   */ const long &,
                               /*  40 type  */ const char &,   /* OrdType_MARKET = '1';, OrdType_LIMIT = '2';, OrdType_STOP = '3'; */
                               /*  44 px    */ const double &,
                               /* 721 PosId */ const std::string & /* New Order is NULL, Close Order is Set */
                              );
  /* AF */ void OrderMassStatusRequest();
  /* F  */ void OrderCancelRequest(/* 41   origOrdId  */ const std::string &);

  // server -> client
  /* y  */ void onMessage(const FIX44::SecurityList &, const FIX::SessionID &);
  /* W  */ void onMessage(const FIX44::MarketDataSnapshotFullRefresh &, const FIX::SessionID &);
  /* 8  */ void onMessage(const FIX44::ExecutionReport &, const FIX::SessionID &);

  // message tool
  void SetMessageHeader(FIX::Message &);
  std::string getSetting(const char *, const char *defvalue = "");
  std::string getCnt();
  std::string getUTCTimeStr();
  void setNewOrder();
};

#endif  // EXAMPLES_CLIENT_APPLICATION_H_
