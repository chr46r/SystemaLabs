#include "ledger_integration.h"
#include <cstdlib>
#include <iostream>

namespace {
    bool runLedgerCommand(const std::string &func,
                          const std::string &arg1,
                          const std::string &arg2,
                          const std::string &arg3)
    {
        // Example command:
        // ./ledger APPLY_TX Alice Bob 100
        std::string cmd = "./ledger " + func + " " + arg1 + " " + arg2 + " " + arg3;
        std::cout << "[Ledger CMD] " << cmd << std::endl;

        int ret = std::system(cmd.c_str());
        return (ret == 0);
    }
}

namespace LedgerIntegration {

bool initLedger() {
    return runLedgerCommand("INIT", "", "", "");
}

bool applyTransaction(const std::string &sender, 
                      const std::string &receiver, 
                      const std::string &amount)
{
    return runLedgerCommand("APPLY_TX", sender, receiver, amount);
}

}
