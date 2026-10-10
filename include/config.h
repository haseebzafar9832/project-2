/*
 * FlowDesk — compile-time configuration
 * Portfolio Project #2 | Language: C++17
 *
 * AI Tool 1 (Cursor): authored this header and wired it through the build.
 * AI Tool 2 (ChatGPT): reviewed the projection constants (scenario bands,
 * compound monthly rate) before they were locked in here.
 *
 * Assignment note: preprocessor directives are applied BEFORE compiling.
 * The Makefile passes -DFLOWDESK_COLOR. Version defaults to 1.0.0.
 */

#ifndef FLOWDESK_CONFIG_H
#define FLOWDESK_CONFIG_H

/* ---------- identity ---------- */
#ifndef FLOWDESK_VERSION
#define FLOWDESK_VERSION "1.0.0"
#endif

#define FLOWDESK_TITLE       "FlowDesk"
#define FLOWDESK_SUBTITLE    "Office Cash-Flow Workstation"
#define FLOWDESK_DATA_FILE   "data/ledger.csv"
#define FLOWDESK_MAX_RECORDS 500

/* ---------- projection model knobs ---------- */
#define FLOWDESK_OPTIMISTIC  0.85   /* expenses run 15% lighter */
#define FLOWDESK_CAUTIOUS    1.15   /* expenses run 15% heavier */
#define FLOWDESK_ANNUAL_RATE 0.03   /* 3% annual interest on a positive balance */

/* stringify helper: turns a macro value into a C string */
#define FLOWDESK_STR_HELPER(x) #x
#define FLOWDESK_STR(x)        FLOWDESK_STR_HELPER(x)

/* require a modern language dialect */
#if defined(__cplusplus) && (__cplusplus < 201703L)
#error "FlowDesk requires C++17 or later. Compile with -std=c++17"
#endif

/* platform: screen clear + path separator */
#if defined(_WIN32) || defined(_WIN64)
#define FLOWDESK_WINDOWS 1
#define FLOWDESK_CLEAR   "cls"
#define FLOWDESK_PATHSEP "\\"
#else
#define FLOWDESK_WINDOWS 0
#define FLOWDESK_CLEAR   "clear"
#define FLOWDESK_PATHSEP "/"
#endif

/* colour: enabled by -DFLOWDESK_COLOR in the Makefile */
#ifdef FLOWDESK_COLOR
#define C_RESET   "\033[0m"
#define C_BOLD    "\033[1m"
#define C_DIM     "\033[2m"
#define C_INK     "\033[38;5;236m"
#define C_MUTED   "\033[38;5;245m"
#define C_LINE    "\033[38;5;250m"
#define C_ACCENT  "\033[38;5;67m"    /* slate blue */
#define C_GOOD    "\033[38;5;65m"    /* sage */
#define C_WARN    "\033[38;5;136m"   /* ochre */
#define C_BAD     "\033[38;5;131m"   /* dusty rose */
#define C_TITLE   "\033[1;38;5;67m"
#else
#define C_RESET   ""
#define C_BOLD    ""
#define C_DIM     ""
#define C_INK     ""
#define C_MUTED   ""
#define C_LINE    ""
#define C_ACCENT  ""
#define C_GOOD    ""
#define C_WARN    ""
#define C_BAD     ""
#define C_TITLE   ""
#endif

/* announce the build so the preprocessor is visible in the compile log */
#ifdef FLOWDESK_COLOR
#define FLOWDESK_BUILD_LABEL FLOWDESK_TITLE " " FLOWDESK_VERSION " (color UI)"
#else
#define FLOWDESK_BUILD_LABEL FLOWDESK_TITLE " " FLOWDESK_VERSION " (plain UI)"
#endif

#ifdef FLOWDESK_ANNOUNCE_BUILD
#pragma message("Preprocessor: compiling " FLOWDESK_BUILD_LABEL)
#endif

#endif /* FLOWDESK_CONFIG_H */
