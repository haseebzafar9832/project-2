# Two AI tools used on this assignment

The brief asks for two AI tools. This project used both as follows.

## Tool 1 — Cursor (agentic coding)

**Role:** wrote the C++ source, preprocessor header, Makefile, sample ledger, and presentation notes. Compiled with Clang, then replayed every menu option to confirm a clean `return 0`.

**Prompts used (summary):**
- Build Portfolio Project #2 in C++ with a user interface, preprocessor directives, and a compiler from the approved list.
- Use Clang by default and keep GCC as a second target.
- Include a practical office ledger plus a cash-flow projection model.
- The user must be able to exhaust every menu item and leave with `return 0`.

## Tool 2 — ChatGPT

**Role:** reviewed the projection model before the constants were locked into `include/config.h`.

**Prompt used:**
> Review a three-scenario monthly cash-flow model for a small office ledger. Starting balance plus average monthly income minus average monthly expense. Optimistic expenses at 85 percent, cautious expenses at 115 percent. Optional compound interest of 3 percent annual on a positive balance only. Flag anything misleading for a student presentation.

**What stayed after that review:**
- Scenario bands live as `#define FLOWDESK_OPTIMISTIC` and `#define FLOWDESK_CAUTIOUS`.
- Interest is opt-in and never applied to a negative balance.
- The on-screen copy says the table is a planning model, not a promise.

## Credit

Original programme written for this assignment. No third-party source was copied. If you later borrow from this tree, keep the header comment in `src/main.cpp`.
