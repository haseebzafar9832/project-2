#include "storage.h"

#include <fstream>
#include <sstream>

bool Storage::save(const std::string& path, const std::vector<Transaction>& items) {
    std::ofstream out(path);
    if (!out) {
        return false;
    }
    out << "id,date,type,category,amount,note\n";
    for (const Transaction& tx : items) {
        out << tx.toCsv() << '\n';
    }
    return static_cast<bool>(out);
}

bool Storage::load(const std::string& path, std::vector<Transaction>& items) {
    std::ifstream in(path);
    if (!in) {
        return false;
    }

    std::vector<Transaction> loaded;
    std::string line;
    bool header = true;
    while (std::getline(in, line)) {
        if (line.empty()) {
            continue;
        }
        if (header) {
            header = false;
            if (line.rfind("id,", 0) == 0) {
                continue;
            }
        }
        Transaction tx = Transaction::fromCsv(line);
        if (tx.id > 0) {
            loaded.push_back(tx);
        }
    }
    items = std::move(loaded);
    return true;
}
