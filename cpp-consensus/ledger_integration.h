#ifndef LEDGER_INTEGRATION_H
#define LEDGER_INTEGRATION_H

#include <string>

namespace LedgerIntegration {
    bool initLedger();
    bool applyTransaction(const std::string &sender, 
                          const std::string &receiver, 
                          const std::string &amount);
}

#endif
