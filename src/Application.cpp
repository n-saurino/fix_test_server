#ifdef _MSC_VER
#pragma warning(disable : 4503 4355 4786)
#endif

#include "quickfix/config.h"

#include "Application.hpp"
#include "quickfix/Session.h"

#include "quickfix/fix44/ExecutionReport.h"

void Application::onCreate(const FIX::SessionID &sessionID) {}

void Application::onLogon(const FIX::SessionID &sessionID)
{
  std::cout << "Received Log on message from client" << "\n";
}

void Application::onLogout(const FIX::SessionID &sessionID) {}
void Application::toAdmin(FIX::Message &message, const FIX::SessionID &sessionID) {}
void Application::toApp(FIX::Message &message, const FIX::SessionID &sessionID) EXCEPT(FIX::DoNotSend) {}

void Application::fromAdmin(const FIX::Message &message, const FIX::SessionID &sessionID)
    EXCEPT(FIX::FieldNotFound, FIX::IncorrectDataFormat, FIX::IncorrectTagValue, FIX::RejectLogon) {}

void Application::fromApp(const FIX::Message &message, const FIX::SessionID &sessionID)
    EXCEPT(FIX::FieldNotFound, FIX::IncorrectDataFormat, FIX::IncorrectTagValue, FIX::UnsupportedMessageType)
{
  crack(message, sessionID);
}

void Application::onMessage(const FIX44::NewOrderSingle &message, const FIX::SessionID &sessionID)
{
  FIX::Symbol symbol;
  FIX::Side side;
  FIX::OrdType ordType;
  FIX::OrderQty orderQty;
  FIX::Price price;
  FIX::ClOrdID clOrdID;
  FIX::Account account;

  message.get(ordType);

  if (ordType != FIX::OrdType_LIMIT)
  {
    throw FIX::IncorrectTagValue(ordType.getTag());
  }

  message.get(symbol);
  message.get(side);
  message.get(orderQty);
  message.get(price);
  message.get(clOrdID);

  FIX44::ExecutionReport executionReport = FIX44::ExecutionReport(
      FIX::OrderID(genOrderID()),
      FIX::ExecID(genExecID()),
      FIX::ExecType(FIX::ExecType_TRADE),
      FIX::OrdStatus(FIX::OrdStatus_FILLED),
      side,
      FIX::LeavesQty(0),
      FIX::CumQty(orderQty),
      FIX::AvgPx(price));

  executionReport.set(clOrdID);
  executionReport.set(symbol);
  executionReport.set(orderQty);
  executionReport.set(FIX::LastQty(orderQty));
  executionReport.set(FIX::LastPx(price));

  if (message.isSet(account))
  {
    executionReport.setField(message.get(account));
  }

  try
  {
    FIX::Session::sendToTarget(executionReport, sessionID);
  }
  catch (FIX::SessionNotFound &)
  {
  }
}

void Application::onMessage(const FIX44::OrderCancelRequest &message, const FIX::SessionID &sessionID)
{
  FIX::OrigClOrdID origClOrdID;
  FIX::Symbol symbol;
  FIX::Side side;
  FIX::Account account;

  message.get(origClOrdID);
  message.get(symbol);
  message.get(side);
  message.get(account);

  // FIX42::ExecutionReport fixOrder(
  //   FIX::OrderID(order.getClientID()),
  //   FIX::ExecID(m_generator.genExecutionID()),
  //   FIX::ExecTransType(FIX::ExecTransType_NEW),
  //   FIX::ExecType(status),
  //   FIX::OrdStatus(status),
  //   FIX::Symbol(order.getSymbol()),
  //   FIX::Side(convert(order.getSide())),
  //   FIX::LeavesQty(order.getOpenQuantity()),
  //   FIX::CumQty(order.getExecutedQuantity()),
  //   FIX::AvgPx(order.getAvgExecutedPrice()));

  //   ExecutionReport(
  //     const FIX::OrderID& aOrderID,
  //     const FIX::ExecID& aExecID,
  //     const FIX::ExecType& aExecType,
  //     const FIX::OrdStatus& aOrdStatus,
  //     const FIX::Side& aSide,
  //     const FIX::LeavesQty& aLeavesQty,
  //     const FIX::CumQty& aCumQty,
  //     const FIX::AvgPx& aAvgPx )

  FIX44::ExecutionReport executionReport = FIX44::ExecutionReport(
      FIX::OrderID(genOrderID()),
      FIX::ExecID(genExecID()),
      FIX::ExecType(FIX::ExecType_CANCELED),
      FIX::OrdStatus(FIX::OrdStatus_CANCELED),
      side,
      FIX::LeavesQty(0),
      FIX::CumQty(0),
      FIX::AvgPx(0));

  executionReport.set(origClOrdID);
  executionReport.set(symbol);

  if (message.isSet(account))
  {
    executionReport.setField(message.get(account));
  }

  try
  {
    FIX::Session::sendToTarget(executionReport, sessionID);
  }
  catch (FIX::SessionNotFound &)
  {
  }
}

void Application::onMessage(const FIX44::OrderCancelReplaceRequest &message, const FIX::SessionID &sessionID)
{
  FIX::OrigClOrdID origClOrdId;
  FIX::Symbol symbol;
  FIX::Side side;
  FIX::OrdType ordType;
  FIX::OrderQty orderQty;
  FIX::Price price;
  FIX::ClOrdID clOrdID;
  FIX::Account account;

  message.get(ordType);

  if (ordType != FIX::OrdType_LIMIT)
  {
    throw FIX::IncorrectTagValue(ordType.getTag());
  }

  message.get(origClOrdId);
  message.get(symbol);
  message.get(side);
  message.get(orderQty);
  message.get(price);
  message.get(clOrdID);

  // FIX42::ExecutionReport fixOrder(
  //   FIX::OrderID(order.getClientID()),
  //   FIX::ExecID(m_generator.genExecutionID()),
  //   FIX::ExecTransType(FIX::ExecTransType_NEW),
  //   FIX::ExecType(status),
  //   FIX::OrdStatus(status),
  //   FIX::Symbol(order.getSymbol()),
  //   FIX::Side(convert(order.getSide())),
  //   FIX::LeavesQty(order.getOpenQuantity()),
  //   FIX::CumQty(order.getExecutedQuantity()),
  //   FIX::AvgPx(order.getAvgExecutedPrice()));

  //   ExecutionReport(
  //     const FIX::OrderID& aOrderID,
  //     const FIX::ExecID& aExecID,
  //     const FIX::ExecType& aExecType,
  //     const FIX::OrdStatus& aOrdStatus,
  //     const FIX::Side& aSide,
  //     const FIX::LeavesQty& aLeavesQty,
  //     const FIX::CumQty& aCumQty,
  //     const FIX::AvgPx& aAvgPx )

  FIX44::ExecutionReport executionReport = FIX44::ExecutionReport(
      FIX::OrderID(genOrderID()),
      FIX::ExecID(genExecID()),
      FIX::ExecType(FIX::ExecType_REPLACED),
      FIX::OrdStatus(FIX::OrdStatus_REPLACED),
      side,
      FIX::LeavesQty(orderQty),
      FIX::CumQty(orderQty),
      FIX::AvgPx(price));

  executionReport.set(clOrdID);
  executionReport.set(symbol);

  if (message.isSet(account))
  {
    executionReport.setField(message.get(account));
  }

  try
  {
    FIX::Session::sendToTarget(executionReport, sessionID);
  }
  catch (FIX::SessionNotFound &)
  {
  }
}
