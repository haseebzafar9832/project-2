#include "transaction.h"

#include <sstream>
#include <string>

namespace {
std::string csvEscape(const std::string& text) {
    std::string out;
    out.reserve(text.size());
    for (char ch : text) {
        if (ch == ',' || ch == '"') {
            out.push_back('"');
        }
        out.push_back(ch);
    }
    return out;
}

std::string csvUnescape(const std::string& text) {
    std::string out;
    bool skip = false;
    for (char ch : text) {
        if (skip) {
            out.push_back(ch);
            skip = false;
            continue;
        }
        if (ch == '"') {
            skip = true;
            continue;
        }
        out.push_back(ch);
    }
    return out;
}
}  // namespace

double Transaction::signedAmount() const {
    return (type == TxType::Income) ? amount : -amount;
}

std::string Transaction::typeLabel() const {
    return (type == TxType::Income) ? "Income" : "Expense";
}

std::string Transaction::toCsv() const {
    std::ostringstream row;
    row << id << ','
        << date << ','
        << (type == TxType::Income ? "IN" : "EX") << ','
        << csvEscape(category) << ','
        << amount << ','
        << csvEscape(note);
    return row.str();
}

Transaction Transaction::fromCsv(const std::string& line) {
    Transaction tx;
    std::string fields[6];
    std::size_t field = 0;
    std::string current;
    bool quoted = false;

    for (char ch : line) {
        if (ch == '"' && !quoted) {
            quoted = true;
            current.push_back(ch);
            continue;
        }
        if (ch == '"' && quoted) {
            quoted = false;
            current.push_back(ch);
            continue;
        }
        if (ch == ',' && !quoted) {
            if (field < 6) {
                fields[field++] = current;
            }
            current.clear();
            continue;
        }
        current.push_back(ch);
    }
    if (field < 6) {
        fields[field] = current;
    }

    try {
        tx.id = std::stoi(fields[0]);
        tx.amount = std::stod(fields[4]);
    } catch (...) {
        tx.id = 0;
        tx.amount = 0.0;
    }
    tx.date = fields[1];
    tx.type = (fields[2] == "IN") ? TxType::Income : TxType::Expense;
    tx.category = csvUnescape(fields[3]);
    tx.note = csvUnescape(fields[5]);
    return tx;
}
