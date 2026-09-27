#include "projector.h"
#include "config.h"

#include <algorithm>

Projector::Projector(double startBalance, double avgIncome, double avgExpense)
    : startBalance_(startBalance), avgIncome_(avgIncome), avgExpense_(avgExpense) {}

double Projector::step(double balance, double income, double expense, bool applyInterest) {
    balance += income - expense;
    if (applyInterest && balance > 0.0) {
        const double monthly = FLOWDESK_ANNUAL_RATE / 12.0;
        balance *= (1.0 + monthly);
    }
    return balance;
}

std::vector<MonthForecast> Projector::run(int months, bool applyInterest) const {
    months = std::max(1, months);
    std::vector<MonthForecast> rows;
    rows.reserve(static_cast<std::size_t>(months));

    double optimistic = startBalance_;
    double base = startBalance_;
    double cautious = startBalance_;

    for (int month = 1; month <= months; ++month) {
        optimistic = step(optimistic, avgIncome_, avgExpense_ * FLOWDESK_OPTIMISTIC, applyInterest);
        base = step(base, avgIncome_, avgExpense_, applyInterest);
        cautious = step(cautious, avgIncome_, avgExpense_ * FLOWDESK_CAUTIOUS, applyInterest);
        rows.push_back({month, optimistic, base, cautious});
    }
    return rows;
}
