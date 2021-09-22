/* -*- C++ -*- */

#include "Application.h"
#include "quickfix/Session.h"

/* F  */
void Application::OrderCancelRequest(/* 41   */ const std::string& orgID) {
  FIX44::OrderCancelRequest message;
  /* 11  */ message.set(FIX::ClOrdID(getCnt()));
  /* 41  */ message.set(FIX::OrigClOrdID(orgID));

  // -- send
  SetMessageHeader(message);
  FIX::Session::sendToTarget(message, SessionTypeTRADE);
  std::cout << "--- < F > ---- OrderCancelRequest --------" << std::endl;
  std::cout << "  ID " << orgID << std::endl;
}

/*
        <message name="OrderCancelRequest" msgtype="F" msgcat="app">
41            <field name="OrigClOrdID" required="Y"/>
37            <field name="OrderID" required="N"/>
11            <field name="ClOrdID" required="Y"/>
        </message>
*/
