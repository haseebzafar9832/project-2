#include "ledger.h"
#include "config.h"

#include <algorithm>
#include <cctype>
#include <set>
#include <string>

namespace {
std::string lower(std::string text) {
    for (char& ch : text) {
        ch = static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
    }
    return text;
}

std::string yearMonth(const std::string& date) {
    if (date.size() >= 7) {
        return date.substr(0, 7);
    }
    return date;
}

bool contains(const std::string& haystack, const std::string& needle) {
    return lower(haystack).find(lower(needle)) != std::string::npos;
}
}  // namespace

bool Ledger::add(const Transaction& tx) {
    if (items_.size() >= FLOWDESK_MAX_RECORDS) {
        return false;
    }
    Transaction copy = tx;
    if (copy.id <= 0) {
        copy.id = nextId();
    }
    items_.push_back(copy);
    return true;
}

bool Ledger::update(int id, const Transaction& tx) {
    Transaction* found = find(id);
    if (!found) {
        return false;
    }
    *found = tx;
    found->id = id;
    return true;
}

bool Ledger::remove(int id) {
    auto it = std::remove_if(items_.begin(), items_.end(),
                             [id](const Transaction& tx) { return tx.id == id; });
    if (it == items_.end()) {
        return false;
    }
    items_.erase(it, items_.end());
    return true;
}

const Transaction* Ledger::find(int id) const {
    for (const Transaction& tx : items_) {
        if (tx.id == id) {
            return &tx;
        }
    }
    return nullptr;
}

Transaction* Ledger::find(int id) {
    return const_cast<Transaction*>(static_cast<const Ledger*>(this)->find(id));
}

std::vector<Transaction> Ledger::search(const std::string& query) const {
    std::vector<Transaction> hits;
    for (const Transaction& tx : items_) {
        if (contains(tx.category, query) || contains(tx.note, query) ||
            contains(tx.date, query) || contains(tx.typeLabel(), query)) {
            hits.push_back(tx);
        }
    }
    return hits;
}

std::vector<Transaction> Ledger::byCategory(const std::string& category) const {
    std::vector<Transaction> hits;
    for (const Transaction& tx : items_) {
        if (lower(tx.category) == lower(category)) {
            hits.push_back(tx);
        }
    }
    return hits;
}

double Ledger::totalIncome() const {
    double sum = 0.0;
    for (const Transaction& tx : items_) {
        if (tx.type == TxType::Income) {
            sum += tx.amount;
        }
    }
    return sum;
}

double Ledger::totalExpense() const {
    double sum = 0.0;
    for (const Transaction& tx : items_) {
        if (tx.type == TxType::Expense) {
            sum += tx.amount;
        }
    }
    return sum;
}

double Ledger::balance() const {
    return totalIncome() - totalExpense();
}

int Ledger::monthsCovered() const {
    std::set<std::string> months;
    for (const Transaction& tx : items_) {
        months.insert(yearMonth(tx.date));
    }
    return months.empty() ? 1 : static_cast<int>(months.size());
}

double Ledger::averageMonthlyIncome() const {
    return totalIncome() / static_cast<double>(monthsCovered());
}

double Ledger::averageMonthlyExpense() const {
    return totalExpense() / static_cast<double>(monthsCovered());
}

int Ledger::nextId() const {
    int high = 0;
    for (const Transaction& tx : items_) {
        if (tx.id > high) {
            high = tx.id;
        }
    }
    return high + 1;
}

void Ledger::clear() {
    items_.clear();
}

void Ledger::replace(std::vector<Transaction> items) {
    items_ = std::move(items);
}
