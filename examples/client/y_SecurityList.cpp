/* -*- C++ -*- */

#include "Application.h"
#include "quickfix/Session.h"

/* y  */
void Application::onMessage(const FIX44::SecurityList &message, const FIX::SessionID &sessionID) {
  /* INIT Message Data */
  /* 52   */ FIX::FieldBase respDateTime(FIX::FIELD::SendingTime, "");
  /* 320  */ FIX::FieldBase reqID(FIX::FIELD::SecurityReqID, "0");
  /* 322  */ FIX::FieldBase respID(FIX::FIELD::SecurityResponseID, "0");
  /* 560  */ FIX::FieldBase result(FIX::FIELD::SecurityRequestResult, "0");
  /* 146  */ FIX::FieldBase symbolCount(FIX::FIELD::NoRelatedSym, "0");

  /* Get Message Data */
  /* 52   */ message.getHeader().getFieldIfSet(respDateTime);
  /* 320  */ message.getFieldIfSet(reqID);
  /* 322  */ message.getFieldIfSet(respID);
  /* 560  */ message.getFieldIfSet(result);
  /* 146  */ message.getFieldIfSet(symbolCount);

  /* check result
   * 0 = Valid request
   * 1 = Invalid or unsupported request
   * 2 = No instruments found that match selection criteria
   * 3 = Not authorized to retrieve instrument data
   * 4 = <Instrument> data temporarily unavailable
   * 5 = Request for instrument data not supported
   **/
  if (result.getString() != "0") {
    std::cout << "<y> SecurityList: ERROR(" << result.getString() << ")" << std::endl;
    return;
  }

  /* get SYMBOL config file */
  std::string symbol_id = getSetting("SYMBOL", "0");

  /* Loop: Currency Pair */
  std::cout << std::endl << "Currency Pair List" << std::endl;
  for (int i = 1; i <= std::stoi(symbolCount.getString()); i++) {
    /* 146  */ FIX44::SecurityList::NoRelatedSym r;
    /* 146  */ message.getGroup(i, r);
    /* 55   */ FIX::FieldBase symbol(FIX::FIELD::Symbol, "0");
    /* 1007 */ FIX::FieldBase name(FIX::FIELD::SymbolName, "");
    /* 1008 */ FIX::FieldBase digits(FIX::FIELD::SymbolDigits, "0");

    /* 55   */ r.getFieldIfSet(symbol);
    /* 1007 */ r.getFieldIfSet(name);
    /* 1008 */ r.getFieldIfSet(digits);

    std::cout << "\tID: " << symbol << "\t " << name << "  digit(" << digits << ")" << std::endl;

    /* set symbol data */
    if (symbol_id == symbol.getString()) {
      SYMBOL_ID = symbol.getString();
      SYMBOL_NAME = name.getString();
      SYMBOL_DIGIT = std::stod(digits.getString());
      std::cout << std::endl
                << "\t*********************************************************************" << std::endl
                << "\tID: " << SYMBOL_ID << "\t " << SYMBOL_NAME << "  digit(" << SYMBOL_DIGIT << ")" << std::endl
                << "\t*********************************************************************" << std::endl
                << std::endl;
      MarketDataRequest(SYMBOL_ID);
    }
  }
}
