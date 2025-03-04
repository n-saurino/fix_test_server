#include "quickfix/Application.h"
#include "quickfix/MessageCracker.h"
#include "quickfix/Session.h"
#include "quickfix/SessionSettings.h"
#include "quickfix/SocketAcceptor.h"
#include "quickfix/fix44/NewOrderSingle.h"
#include "quickfix/FileStore.h"
#include "quickfix/Log.h"
#include "NewOrderSingle.h"
#include "MessageHeader.h"

class FixServer : public FIX::Application, public FIX::MessageCracker
{
public:
  void onCreate(const FIX::SessionID &) override {}
  void onLogon(const FIX::SessionID &) override {}
  void onLogout(const FIX::SessionID &) override {}
  void toAdmin(FIX::Message &, const FIX::SessionID &) override {}
  void toApp(FIX::Message &, const FIX::SessionID &) override
      EXCEPT(FIX::DoNotSend) {}
  void fromAdmin(const FIX::Message &, const FIX::SessionID &)
      override EXCEPT(FIX::FieldNotFound, FIX::IncorrectDataFormat,
                      FIX::IncorrectTagValue, FIX::RejectLogon) {}
  void fromApp(const FIX::Message &message, const FIX::SessionID &sessionID)
      override EXCEPT(FIX::FieldNotFound, FIX::IncorrectDataFormat,
                      FIX::IncorrectTagValue, FIX::UnsupportedMessageType)
  {
    crack(message, sessionID);
  }

  void onMessage(const FIX44::NewOrderSingle &message,
                 const FIX::SessionID &sessionID) override
  {
    std::cout << "Received NewOrderSingle: " << message << std::endl;

    // Create and send an SBE response message
    char buffer[1024];
    sbe::MessageHeader hdr;
    sbe::NewOrderSingle order;

    hdr.wrap(buffer, 0, 0, sizeof(buffer))
        .blockLength(order.sbeBlockLength())
        .templateId(order.sbeTemplateId())
        .schemaId(order.sbeSchemaId())
        .version(order.sbeSchemaVersion());

    order.wrapForEncode(buffer, hdr.encodedLength(), sizeof(buffer))
        .clOrdId(12345)
        .account(67890)
        .symbol(112233)
        .side(sbe::SideEnum::Buy)
        .transactTime(1617181920)
        .orderQty()
        .mantissa(100);

    order.price()
        .mantissa(10050);

    FIX::Message fixMessage;
    fixMessage.setField(FIX::FIELD::RawData, std::string(buffer, hdr.encodedLength() + order.encodedLength()));
    FIX::Session::sendToTarget(fixMessage, sessionID);
  }
};
