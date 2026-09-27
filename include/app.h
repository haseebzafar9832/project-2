#ifndef FLOWDESK_APP_H
#define FLOWDESK_APP_H

#include "ledger.h"
#include "transaction.h"

#include <vector>

class App {
public:
    int run();

private:
    Ledger ledger_;

    void loadOrSeed();
    int menu() const;

    void addTransaction();
    void viewTransactions() const;
    void searchTransactions() const;
    void editOrDelete();
    void categoryReport() const;
    void projectionModel() const;
    void saveLedger() const;
    void loadLedger();
    void help() const;

    void printTable(const std::vector<Transaction>& rows) const;
};

#endif
