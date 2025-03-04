#pragma once
#include <thread>
#include "quickfix/config.h"

#include "Application.hpp"
#include "quickfix/Session.h"

#include "quickfix/fix40/ExecutionReport.h"
#include "quickfix/fix41/ExecutionReport.h"
#include "quickfix/fix42/ExecutionReport.h"
#include "quickfix/fix43/ExecutionReport.h"
#include "quickfix/fix44/ExecutionReport.h"
#include "quickfix/fix50/ExecutionReport.h"

#include "generated/sbe/MessageHeader.h"
#include "generated/sbe/NewOrderSingle.h"

class SBESocketHandler
{
public:
    SBESocketHandler(int port);
    ~SBESocketHandler();
    void Start();
    void Stop();
    void ReceiveMessages();
    void FIXToSBE(const FIX::Message &message, const FIX::SessionID &session_id);
    void SendSBEMessage(const char *buffer, size_t length);

private:
    int m_port;
    int m_socket;
    bool m_running;
    std::thread m_thread;
};