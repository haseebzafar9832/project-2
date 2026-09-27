#include "app.h"
#include "config.h"
#include "projector.h"
#include "storage.h"
#include "ui.h"

#include <iomanip>
#include <iostream>
#include <map>

namespace {
Transaction seed(int id, const char* date, TxType type, const char* category,
                 double amount, const char* note) {
    Transaction tx;
    tx.id = id;
    tx.date = date;
    tx.type = type;
    tx.category = category;
    tx.amount = amount;
    tx.note = note;
    return tx;
}
}  // namespace

int App::run() {
    loadOrSeed();

    bool running = true;
    while (running) {
        UI::clearScreen();
        const int choice = menu();
        UI::clearScreen();
        UI::banner();

        switch (choice) {
            case 1: addTransaction(); break;
            case 2: viewTransactions(); break;
            case 3: searchTransactions(); break;
            case 4: editOrDelete(); break;
            case 5: categoryReport(); break;
            case 6: projectionModel(); break;
            case 7: saveLedger(); break;
            case 8: loadLedger(); break;
            case 9: help(); break;
            case 0:
                UI::notice("Saving a closing snapshot, then leaving cleanly.");
                Storage::save(FLOWDESK_DATA_FILE, ledger_.all());
                UI::ok("Goodbye. FlowDesk finished with return 0.");
                running = false;
                break;
            default:
                UI::warn("That option is not on the menu.");
                UI::pause();
                break;
        }
    }
    return 0;
}

void App::loadOrSeed() {
    std::vector<Transaction> loaded;
    if (Storage::load(FLOWDESK_DATA_FILE, loaded) && !loaded.empty()) {
        ledger_.replace(std::move(loaded));
        return;
    }

    /* Sample office month so the first demo is not an empty book. */
    ledger_.add(seed(1, "2026-07-01", TxType::Income,  "Consulting", 4200.00, "July retainers"));
    ledger_.add(seed(2, "2026-07-04", TxType::Expense, "Software",     49.00, "Design tools"));
    ledger_.add(seed(3, "2026-07-12", TxType::Expense, "Office",      186.40, "Paper and toner"));
    ledger_.add(seed(4, "2026-08-01", TxType::Income,  "Consulting", 3900.00, "August retainers"));
    ledger_.add(seed(5, "2026-08-08", TxType::Expense, "Travel",      214.75, "Client visit"));
    ledger_.add(seed(6, "2026-08-20", TxType::Expense, "Software",     49.00, "Design tools"));
    ledger_.add(seed(7, "2026-09-01", TxType::Income,  "Consulting", 4100.00, "September retainers"));
    ledger_.add(seed(8, "2026-09-09", TxType::Expense, "Office",       72.30, "Supplies"));
    ledger_.add(seed(9, "2026-09-18", TxType::Expense, "Marketing",   160.00, "Listing refresh"));
}

int App::menu() const {
    UI::banner();
    std::cout << C_MUTED << "  Balance  " << C_RESET << C_BOLD
              << UI::money(ledger_.balance()) << C_RESET
              << C_MUTED << "   ·   " << ledger_.size() << " records\n\n"
              << C_RESET
              << "    1  Add a transaction\n"
              << "    2  View all transactions\n"
              << "    3  Search / filter\n"
              << "    4  Edit or delete a transaction\n"
              << "    5  Category report\n"
              << "    6  Cash-flow projection model\n"
              << "    7  Save ledger to file\n"
              << "    8  Load ledger from file\n"
              << "    9  Help\n"
              << "    0  Exit\n\n";
    return UI::readInt("Choose an option: ", 0, 9);
}

void App::printTable(const std::vector<Transaction>& rows) const {
    UI::rule();
    std::cout << C_MUTED
              << "  ID   Date          Type      Category        Amount         Note\n"
              << C_RESET;
    UI::rule();

    if (rows.empty()) {
        UI::notice("No rows to show.");
        UI::rule();
        return;
    }

    for (const Transaction& tx : rows) {
        const char* tone = (tx.type == TxType::Income) ? C_GOOD : C_BAD;
        std::cout << "  " << std::setw(4) << tx.id << "  "
                  << std::setw(12) << std::left << tx.date << std::right << "  "
                  << tone << std::setw(8) << std::left << tx.typeLabel() << C_RESET << "  "
                  << std::setw(14) << std::left << tx.category.substr(0, 14) << std::right
                  << tone << std::setw(12) << UI::money(tx.signedAmount()) << C_RESET
                  << "  " << tx.note << '\n';
    }
    UI::rule();
    std::cout << "  Income " << C_GOOD << UI::money(ledger_.totalIncome()) << C_RESET
              << "   Expense " << C_BAD << UI::money(ledger_.totalExpense()) << C_RESET
              << "   Balance " << C_BOLD << UI::money(ledger_.balance()) << C_RESET << '\n';
}

void App::addTransaction() {
    std::cout << C_TITLE << "  Add a transaction\n" << C_RESET;
    UI::notice("Leave the date blank to use today.");

    Transaction tx;
    tx.date = UI::readLineOrDefault("Date (YYYY-MM-DD): ", UI::today());
    const int kind = UI::readInt("Type  [1 Income / 2 Expense]: ", 1, 2);
    tx.type = (kind == 1) ? TxType::Income : TxType::Expense;
    tx.category = UI::readLineOrDefault("Category: ", "General");
    tx.amount = UI::readDouble("Amount: ", 0.01, 100000000.0);
    tx.note = UI::readLine("Note (optional): ");

    if (ledger_.add(tx)) {
        UI::ok("Recorded as #" + std::to_string(ledger_.nextId() - 1) + ".");
    } else {
        UI::warn("The ledger is full (see FLOWDESK_MAX_RECORDS).");
    }
    UI::pause();
}

void App::viewTransactions() const {
    std::cout << C_TITLE << "  All transactions\n" << C_RESET;
    printTable(ledger_.all());
    UI::pause();
}

void App::searchTransactions() const {
    std::cout << C_TITLE << "  Search / filter\n" << C_RESET;
    UI::notice("Match against date, type, category, or note.");
    const std::string query = UI::readLine("Search text: ");
    printTable(ledger_.search(query));
    UI::pause();
}

void App::editOrDelete() {
    std::cout << C_TITLE << "  Edit or delete\n" << C_RESET;
    printTable(ledger_.all());
    if (ledger_.size() == 0) {
        UI::pause();
        return;
    }

    const int id = UI::readInt("Transaction ID: ", 1, 100000);
    Transaction* found = ledger_.find(id);
    if (!found) {
        UI::warn("No transaction uses that ID.");
        UI::pause();
        return;
    }

    const int action = UI::readInt("1 Edit   2 Delete   0 Cancel: ", 0, 2);
    if (action == 0) {
        UI::notice("No change made.");
        UI::pause();
        return;
    }
    if (action == 2) {
        if (UI::confirm("Delete this row")) {
            ledger_.remove(id);
            UI::ok("Deleted.");
        } else {
            UI::notice("Kept.");
        }
        UI::pause();
        return;
    }

    Transaction edited = *found;
    edited.date = UI::readLineOrDefault("Date [" + found->date + "]: ", found->date);
    edited.category = UI::readLineOrDefault("Category [" + found->category + "]: ", found->category);
    const std::string amountText = UI::readLine("Amount [" + UI::money(found->amount) + "]: ");
    if (!amountText.empty()) {
        try {
            edited.amount = std::stod(amountText);
        } catch (...) {
            UI::warn("Amount left unchanged.");
        }
    }
    edited.note = UI::readLineOrDefault("Note [" + found->note + "]: ", found->note);
    ledger_.update(id, edited);
    UI::ok("Updated.");
    UI::pause();
}

void App::categoryReport() const {
    std::cout << C_TITLE << "  Category report\n" << C_RESET;
    UI::rule();

    std::map<std::string, double> income;
    std::map<std::string, double> expense;
    for (const Transaction& tx : ledger_.all()) {
        if (tx.type == TxType::Income) {
            income[tx.category] += tx.amount;
        } else {
            expense[tx.category] += tx.amount;
        }
    }

    std::cout << C_GOOD << "  Income\n" << C_RESET;
    if (income.empty()) {
        UI::notice("None yet.");
    } else {
        for (const auto& pair : income) {
            std::cout << "    " << std::setw(16) << std::left << pair.first
                      << std::right << UI::money(pair.second) << '\n';
        }
    }

    std::cout << C_BAD << "\n  Expense\n" << C_RESET;
    if (expense.empty()) {
        UI::notice("None yet.");
    } else {
        for (const auto& pair : expense) {
            std::cout << "    " << std::setw(16) << std::left << pair.first
                      << std::right << UI::money(pair.second) << '\n';
        }
    }
    UI::rule();
    UI::pause();
}

void App::projectionModel() const {
    std::cout << C_TITLE << "  Cash-flow projection model\n" << C_RESET;
    UI::notice("Uses average monthly income and expense from the ledger.");
    UI::notice("Optimistic = expenses x " FLOWDESK_STR(FLOWDESK_OPTIMISTIC)
               "   Cautious = expenses x " FLOWDESK_STR(FLOWDESK_CAUTIOUS));

    double income = ledger_.averageMonthlyIncome();
    double expense = ledger_.averageMonthlyExpense();
    if (ledger_.size() == 0) {
        UI::warn("The book is empty, so enter assumed monthly figures.");
        income = UI::readDouble("Assumed monthly income: ", 0.0, 100000000.0);
        expense = UI::readDouble("Assumed monthly expense: ", 0.0, 100000000.0);
    }

    std::cout << '\n'
              << "  Starting balance   " << UI::money(ledger_.balance()) << '\n'
              << "  Avg monthly income " << UI::money(income) << '\n'
              << "  Avg monthly expense " << UI::money(expense) << "\n\n";

    const int months = UI::readInt("How many months ahead (1-36)? ", 1, 36);
    const bool interest = UI::confirm("Apply " FLOWDESK_STR(FLOWDESK_ANNUAL_RATE)
                                     " annual interest on a positive balance");

    const Projector model(ledger_.balance(), income, expense);
    const std::vector<MonthForecast> rows = model.run(months, interest);

    UI::rule();
    std::cout << C_MUTED
              << "  Month    Optimistic         Base          Cautious\n"
              << C_RESET;
    UI::rule();
    for (const MonthForecast& row : rows) {
        std::cout << "   " << std::setw(4) << row.month << "    "
                  << C_GOOD << std::setw(12) << UI::money(row.optimistic) << C_RESET << "   "
                  << C_BOLD << std::setw(12) << UI::money(row.base) << C_RESET << "   "
                  << C_WARN << std::setw(12) << UI::money(row.cautious) << C_RESET << '\n';
    }
    UI::rule();
    UI::notice("This is a planning model, not a promise.");
    UI::pause();
}

void App::saveLedger() const {
    std::cout << C_TITLE << "  Save ledger\n" << C_RESET;
    if (Storage::save(FLOWDESK_DATA_FILE, ledger_.all())) {
        UI::ok(std::string("Wrote ") + FLOWDESK_DATA_FILE);
    } else {
        UI::warn("Could not write the file. Check the data folder.");
    }
    UI::pause();
}

void App::loadLedger() {
    std::cout << C_TITLE << "  Load ledger\n" << C_RESET;
    std::vector<Transaction> loaded;
    if (!Storage::load(FLOWDESK_DATA_FILE, loaded)) {
        UI::warn("Could not read " FLOWDESK_DATA_FILE);
        UI::pause();
        return;
    }
    ledger_.replace(std::move(loaded));
    UI::ok("Loaded " + std::to_string(ledger_.size()) + " records.");
    UI::pause();
}

void App::help() const {
    std::cout << C_TITLE << "  Help\n" << C_RESET
              << "\n  FlowDesk is a small office workstation.\n"
              << "  Record income and expenses, review categories, then project\n"
              << "  cash flow a few months ahead under three scenarios.\n\n"
              << "  Files live in  " << FLOWDESK_DATA_FILE << "\n"
              << "  Build label    " << FLOWDESK_BUILD_LABEL << "\n"
              << "  Language       C++17    Compiler: Clang or GCC\n"
              << "  AI tools       Cursor (code) and ChatGPT (projection review)\n\n"
              << "  Exhaust every menu item, then choose 0. The program\n"
              << "  writes a closing snapshot and returns 0.\n";
    UI::pause();
}
