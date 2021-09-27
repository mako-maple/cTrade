/* -*- C++ -*- */

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
  if (SessionTypeQUOTE == sessionID.getSessionQualifier()) {
    SYMBOL_ID = getSetting("SYMBOL", "0");
    SecurityListRequest();
  }
  if (SessionTypeTRADE == sessionID.getSessionQualifier()) {
    SIZE   = std::stol(getSetting("SIZE", "1000"));
    RANGE  = std::stoi(getSetting("RANGE", "100"));
    STOP   = std::stoi(getSetting("STOP",  "100"));
    MAXSEC = std::stol(getSetting("MAXSEC", "30"));
    SPREAD = std::stoi(getSetting("SPREAD",  "3"));
  }
}

void Application::onLogout(const FIX::SessionID &sessionID) {
  std::cout << std::endl << "Logout - " << sessionID << std::endl;
}

void Application::fromApp(const FIX::Message &message, const FIX::SessionID &sessionID)
    EXCEPT(FIX::FieldNotFound, FIX::IncorrectDataFormat, FIX::IncorrectTagValue, FIX::UnsupportedMessageType) {
  crack(message, sessionID);
  // if (FIX::MsgType_ExecutionReport == message.getHeader().getField(FIX::FIELD::MsgType)) std::cout << message.toXML() << std::endl;
}

void Application::toApp(FIX::Message &message, const FIX::SessionID &sessionID) EXCEPT(FIX::DoNotSend) {
  try {
    FIX::PossDupFlag possDupFlag;
    message.getHeader().getField(possDupFlag);
    if (possDupFlag) throw FIX::DoNotSend();
  } catch (FIX::FieldNotFound &) {
  }
  // if (FIX::MsgType_NewOrderSingle == message.getHeader().getField(FIX::FIELD::MsgType)) std::cout << message.toXML() << std::endl;
  // if (FIX::MsgType_OrderMassStatusRequest == message.getHeader().getField(FIX::FIELD::MsgType)) std::cout << message.toXML() << std::endl;
  // if (FIX::MsgType_OrderCancelRequest == message.getHeader().getField(FIX::FIELD::MsgType)) std::cout << message.toXML() << std::endl;
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
                << "x) Security List Request" << std::endl
                << "V) Market Data Request" << std::endl
                << std::endl
                << "d) New Order" << std::endl
                << "A) Order List" << std::endl
                << std::endl
                << "q) Quit" << std::endl
                << std::endl
                << "Action: ";
      std::cin >> action;

      if (action == 'q')
        break;
      else if (action == 'x')
        SecurityListRequest();
      else if (action == 'V')
        MarketDataRequest(getSetting("SYMBOL", "0"));
      else if (action == 'd') 
        setNewOrder();
      else if (action == 'A')
        OrderMassStatusRequest();
    } catch (std::exception &e) {
      std::cout << "Message Not Sent: " << e.what();
    }
  }
}

// Set Message Header
void Application::SetMessageHeader(FIX::Message &message) {
  message.getHeader().setField((FIX::SenderCompID)getSetting("SenderCompID"));
  message.getHeader().setField((FIX::TargetCompID)getSetting("TargetCompID"));
}

// Get Config String
std::string Application::getSetting(const char *key, const char *defvalue) {
  const FIX::Dictionary dic = m_settings.get();
  if (dic.has(key)) return dic.getString(key);
  return defvalue;
}

// Get Counter
std::string Application::getCnt() { 
  return std::to_string(++cnt); 
}

// Get UTC Time String
std::string Application::getUTCTimeStr() {
  struct timespec ts;
  struct tm t;
  int ret;
  char buf[32] = "yyyy/mm/dd hh:mm:ss.000";

  // Get epoch time
  ret = clock_gettime(CLOCK_REALTIME, &ts);
  if (ret < 0)  return buf;

  // Convert into local and parsed time
  gmtime_r(&ts.tv_sec, &t);

  // Create string with strftime
  ret = strftime(buf, 32, "%Y/%m/%d %H:%M:%S", &t);
  if (ret < 0)  return buf;

  // Add milli-seconds with snprintf
  char output[32];
  const int msec = ts.tv_nsec / 1000000;
  ret = snprintf(output, 32, "%s.%03d", buf, msec);
  if (ret < 0)  return buf;

  // Result
  return output;
}

// New Order 準備
void Application::setNewOrder() {
  std::cout << "set New Order [" << ORDER_ID << "] [" << ORDER_COUNT << "] [" << ORDER_SIDE << "]" << std::endl;
  // Order 可能？
  if (ORDER_ID != "") return;

  // Order 待ち状態？
  if (ORDER_COUNT > 0) return; 

  // ナノ秒取得
  struct timespec ts;
  clock_gettime(CLOCK_REALTIME, &ts);

  // 待ち時間？待ちTick？を設定
  ORDER_COUNT = ts.tv_nsec  / 100000000;

  // 方向設定（買、売）
  ORDER_SIDE = (ts.tv_nsec  / 1000000 % 2 == 0 ? "1" /* Side_BUY */ : "2" /* Side_SELL */);
  std::cout << "set New Order [" << ORDER_ID << "] [" << ORDER_COUNT << "] [" << ORDER_SIDE << "]" << std::endl;

  // 
  return;
}
