#include "SBESocketHandler.hpp"

void SBESocketHandler::Start()
{
    m_running = true;
}

void SBESocketHandler::Stop()
{
}

void SBESocketHandler::ReceiveMessages()
{
    const int kBufferSize{2048};
    char buffer[kBufferSize];

    while (m_running)
    {
        int nbytes_r = recv(m_socket, buffer, sizeof(buffer), 0);
        if (nbytes_r > 0)
        {
            sbe::MessageHeader hdr;
            sbe::NewOrderSingle order;

            hdr.wrap(buffer, 0, 0, sizeof(buffer));
            order.wrapForDecode(buffer, hdr.encodedLength(), hdr.blockLength(), hdr.version(), sizeof(buffer));

            // Translate SBE message to FIX message
            FIX44::NewOrderSingle fixMessage;
            fixMessage.setField(FIX::ClOrdID(std::to_string(order.clOrdId())));
            fixMessage.setField(FIX::Account(std::to_string(order.account())));
            fixMessage.setField(FIX::Symbol(std::to_string(order.symbol())));
            fixMessage.setField(FIX::Side(order.side()));
            fixMessage.setField(FIX::TransactTime(order.transactTime()));
            fixMessage.setField(FIX::OrderQty(order.orderQty().mantissa()));
            fixMessage.setField(FIX::Price(order.price().mantissa()));

            // Set the appropriate SessionID
            FIX::SessionID sessionID("FIX.4.4", "SENDER", "TARGET");

            // Pass the FIX message to QuickFIX
            FIX::Session::sendToTarget(fixMessage, sessionID);
        }
    }
}

void SBESocketHandler::FIXToSBE(const FIX::Message &message, const FIX::SessionID &session_id)
{
    // Translate FIX message to SBE message
    const int kBufferSize{2048};
    char buffer[kBufferSize];
    sbe::MessageHeader hdr;
    sbe::NewOrderSingle order;
    FIX::ClOrdID clOrdID;
    FIX::Account account;
    FIX::Symbol symbol;
    FIX::Side side;
    FIX::Price price;
    FIX::OrderQty orderQty;

    message.getField(clOrdID);
    message.getField(account);
    message.getField(symbol);
    message.getField(side);
    message.getField(price);
    message.getField(orderQty);

    hdr.wrap(buffer, 0, 0, sizeof(buffer))
        .blockLength(order.sbeBlockLength())
        .templateId(order.sbeTemplateId())
        .schemaId(order.sbeSchemaId())
        .version(order.sbeSchemaVersion());

    order.wrapForEncode(buffer, hdr.encodedLength(), sizeof(buffer))
        .clOrdId(std::stoll(clOrdID.getValue()))
        .account(std::stoll(account.getValue()))
        .symbol(std::stoll(symbol.getValue()))
        .side(static_cast<sbe::SideEnum::Value>(side.getValue()))
        .transactTime(0)
        .orderQty()
        .mantissa(orderQty.getValue());

    order.price().mantissa(price.getValue());

    // Send the SBE message over the network
    SendSBEMessage(buffer, hdr.encodedLength() + order.encodedLength());
}

void SBESocketHandler::SendSBEMessage(const char *buffer, size_t length)
{
    send(m_socket, buffer, length, 0);
}
