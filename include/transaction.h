#ifndef FLOWDESK_TRANSACTION_H
#define FLOWDESK_TRANSACTION_H

#include <string>

enum class TxType { Income, Expense };

struct Transaction {
    int id = 0;
    std::string date;      /* YYYY-MM-DD */
    TxType type = TxType::Expense;
    std::string category;
    double amount = 0.0;
    std::string note;

    double signedAmount() const;
    std::string typeLabel() const;
    std::string toCsv() const;
    static Transaction fromCsv(const std::string& line);
};

#endif
