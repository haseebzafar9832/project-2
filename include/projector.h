#ifndef FLOWDESK_PROJECTOR_H
#define FLOWDESK_PROJECTOR_H

#include <string>
#include <vector>

struct MonthForecast {
    int month = 0;
    double optimistic = 0.0;
    double base = 0.0;
    double cautious = 0.0;
};

class Projector {
public:
    Projector(double startBalance, double avgIncome, double avgExpense);

    std::vector<MonthForecast> run(int months, bool applyInterest) const;

    double startBalance() const { return startBalance_; }
    double avgIncome() const { return avgIncome_; }
    double avgExpense() const { return avgExpense_; }

private:
    double startBalance_;
    double avgIncome_;
    double avgExpense_;

    static double step(double balance, double income, double expense, bool applyInterest);
};

#endif
