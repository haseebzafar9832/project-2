#ifndef FLOWDESK_UI_H
#define FLOWDESK_UI_H

#include <string>

class UI {
public:
    static void clearScreen();
    static void banner();
    static void rule();
    static void pause();
    static void notice(const std::string& text);
    static void warn(const std::string& text);
    static void ok(const std::string& text);

    static std::string readLine(const std::string& prompt);
    static std::string readLineOrDefault(const std::string& prompt, const std::string& fallback);
    static int readInt(const std::string& prompt, int min, int max);
    static double readDouble(const std::string& prompt, double min, double max);
    static bool confirm(const std::string& prompt);

    static std::string money(double value);
    static std::string today();
};

#endif
