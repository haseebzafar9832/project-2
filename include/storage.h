#ifndef FLOWDESK_STORAGE_H
#define FLOWDESK_STORAGE_H

#include "transaction.h"

#include <string>
#include <vector>

class Storage {
public:
    static bool save(const std::string& path, const std::vector<Transaction>& items);
    static bool load(const std::string& path, std::vector<Transaction>& items);
};

#endif
