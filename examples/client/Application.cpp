/* -*- C++ -*- */
// Copyright 2021 Maplelinks,inc. All rights reserved.

#include "Application.h"

#include "quickfix/Session.h"
#include "quickfix/config.h"

void Application::onLogon(const FIX::SessionID &sessionID) {
  std::cout << std::endl << "Logon - " << sessionID << std::endl;
  std::cout << std::endl
            << "BEGIN    :" << sessionID.getBeginString() << std::endl
            << "SENDER   :" << sessionID.getSenderCompID() << std::endl
            << "TARGET   :" << sessionID.getTargetCompID() << std::endl
            << "QUALIFIER:" << sessionID.getSessionQualifier() << std::endl
            << std::endl;
}

void Application::onLogout(const FIX::SessionID &sessionID) {
  std::cout << std::endl << "Logout - " << sessionID << std::endl;
}

void Application::fromApp(const FIX::Message &message, const FIX::SessionID &sessionID)
    EXCEPT(FIX::FieldNotFound, FIX::IncorrectDataFormat, FIX::IncorrectTagValue, FIX::UnsupportedMessageType) {
  crack(message, sessionID);
}

void Application::toApp(FIX::Message &message, const FIX::SessionID &sessionID) EXCEPT(FIX::DoNotSend) {
  try {
    FIX::PossDupFlag possDupFlag;
    message.getHeader().getField(possDupFlag);
    if (possDupFlag) throw FIX::DoNotSend();
  } catch (FIX::FieldNotFound &) {
  }
}

void Application::toAdmin(FIX::Message &message, const FIX::SessionID &sessionID) {
  if (FIX::MsgType_Logon == message.getHeader().getField(FIX::FIELD::MsgType)) {
    const FIX::Dictionary &dic = m_settings.get(sessionID);
    if (dic.has("TargetSubID")) message.getHeader().setField(FIX::TargetSubID(dic.getString("TargetSubID")));
    if (dic.has("Username")) message.getHeader().setField(FIX::Username(dic.getString("Username")));
    if (dic.has("Password")) message.getHeader().setField(FIX::Password(dic.getString("Password")));
  }
}

void Application::run() {
  while (true) {
    try {
      char action;
      std::cout << std::endl 
        << "q) Quit" << std::endl 
        << std::endl 
        << "Action: ";
      std::cin >> action;

      if (action == 'q') break;
    } catch (std::exception &e) {
      std::cout << "Message Not Sent: " << e.what();
    }
  }
}