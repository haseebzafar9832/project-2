/*
 * FlowDesk — Portfolio Project #2
 * Agentic Coding Begins  |  C / C++
 *
 * Language : C++17
 * Compiler : Clang (Makefile also builds with GCC)
 * Purpose  : practical office ledger + cash-flow projection model
 * Interface: numbered terminal menu; option 0 exits with return 0
 *
 * AI Tool 1 — Cursor: wrote, compiled, and exercised the programme.
 * AI Tool 2 — ChatGPT: reviewed the three-scenario projection model
 *            (optimistic / base / cautious bands and interest step).
 */

#define FLOWDESK_ANNOUNCE_BUILD
#include "app.h"
#include "config.h"

#include <iostream>

int main() {
    std::cout << C_MUTED << "Starting " << FLOWDESK_BUILD_LABEL << "...\n" << C_RESET;
    App desk;
    return desk.run();
}
