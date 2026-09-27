#ifndef FLOWDESK_LEDGER_H
#define FLOWDESK_LEDGER_H

#include "transaction.h"

#include <string>
#include <vector>

class Ledger {
public:
    bool add(const Transaction& tx);
    bool update(int id, const Transaction& tx);
    bool remove(int id);
    const Transaction* find(int id) const;
    Transaction* find(int id);

    const std::vector<Transaction>& all() const { return items_; }
    std::vector<Transaction> search(const std::string& query) const;
    std::vector<Transaction> byCategory(const std::string& category) const;

    double totalIncome() const;
    double totalExpense() const;
    double balance() const;
    double averageMonthlyIncome() const;
    double averageMonthlyExpense() const;
    int nextId() const;
    std::size_t size() const { return items_.size(); }
    void clear();
    void replace(std::vector<Transaction> items);

private:
    std::vector<Transaction> items_;
    int monthsCovered() const;
};

#endif
