/* -*- C++ -*- */

#ifndef EXAMPLES_CLIENT_APPLICATION_H_
#define EXAMPLES_CLIENT_APPLICATION_H_

#include <cmath>
#include <iomanip>
#include <iostream>
#include <string>

#include "quickfix/Application.h"
#include "quickfix/MessageCracker.h"
#include "quickfix/Mutex.h"
#include "quickfix/Values.h"

// client -> server
#include "quickfix/fix44/SecurityListRequest.h"  // < x >

// server -> client
#include "quickfix/fix44/SecurityList.h"  // < y >

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

  // server -> client
  /* y  */ void onMessage(const FIX44::SecurityList &, const FIX::SessionID &);

  // message tool
  void SetMessageHeader(FIX::Message &);
  std::string getSetting(const char *, const char *defvalue = "");
  std::string getCnt();
};

#endif  // EXAMPLES_CLIENT_APPLICATION_H_
