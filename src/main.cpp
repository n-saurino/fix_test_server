#include "Server.hpp"

int main(int argc, char *argv[])
{
    std::cout << "Launching FIX Server...\n";
    Server server{};
    return 0;
}

/*
int main(int argc, char **argv)
{
  FIX::SessionSettings settings("server.cfg");
  FixServer application;
  FIX::FileStoreFactory storeFactory(settings);
  FIX::ScreenLogFactory logFactory(settings);
  FIX::SocketAcceptor acceptor(application, storeFactory, settings, logFactory);
  acceptor.start();
  std::cout << "FIX server started..." << std::endl;
  while (true)
  {
    // Keep the server running
  }
  acceptor.stop();
  return 0;
}
*/
