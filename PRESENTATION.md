# FlowDesk — short presentation script

Use this as a 5–8 minute walk-through of the official brief. Compile first so the preprocessor message is on screen.

```bash
make clean && make
./flowdesk
```

While Clang runs, point at this line in the compile log:

```text
warning: Preprocessor: compiling FlowDesk 1.0.0 (color UI)
```

That comes from `#pragma message` in `include/config.h`. It proves the preprocessor ran **before** the compiler built the programme (Step 3).

---

## 1. Why C++ (Step 1)

C is small and fast, but large programmes are hard to keep tidy. FlowDesk is split into classes (`Ledger`, `Projector`, `Storage`, `UI`, `App`) so each file has one job. That is the C++ reason from the brief.

## 2. Why this programme (Step 2)

The brief allows practical office use, model projections, or a game. This one does the first two:

- **Office ledger** — add, view, search, edit, delete, category totals, save, load.
- **Model projection** — the next 1–36 months under optimistic, base, and cautious expense bands. Optional 3% annual interest applies only to a positive balance.

## 3. Build and preprocessor (Step 3)

Original programme (no borrowed source). Preprocessor directives are in `include/config.h` and run before compiling:

| Directive | What it does here |
| --- | --- |
| `#ifndef` / `#define` / `#endif` | Header guard |
| `#define` | Title, version, file path, record cap, scenario bands |
| `#if` / `#else` | Windows `cls` vs Mac/Linux clear |
| `#ifdef FLOWDESK_COLOR` | Colour UI when the Makefile passes `-DFLOWDESK_COLOR` |
| `#error` | Stop the build if the dialect is older than C++17 |
| `#pragma message` | Print the compile-time banner in the log |

Build pipeline to mention: write source → **preprocessor** → compiler → linker → run. FlowDesk’s visible proof is the pragma line, then `./flowdesk`.

## 4. Compiler (Step 4)

Default is **Clang**. `make gcc` builds the same sources with **GCC**. Both are on the approved list (GCC, Clang, MinGW, Visual C++, …).

```bash
make        # Clang
make gcc    # GCC
```

## 5. Live demo (Step 5)

Walk every menu item, then leave on **0**. The user can exhaust all options; the programme finishes with `return 0`.

1. **Add** — income, category `Bonus`, any amount.  
2. **View** — the new row is in the table.  
3. **Search** — type `Consult`.  
4. **Edit or delete** — edit a note, then try delete and answer `n` so the row stays.  
5. **Category report** — income and expense grouped.  
6. **Projection** — 12 months, interest off (or on). Read the three columns.  
7. **Save** — writes `data/ledger.csv`.  
8. **Load** — reads it back.  
9. **Help** — version, file path, build label.  
0. **Exit** — “finished with return 0”.

Open another terminal and show:

```bash
echo $?
```

after a normal quit. It should print `0`.

---

## Code walk (same order as a live tour)

| File | What to say |
| --- | --- |
| `include/config.h` | Header guard, `#define` limits and scenario bands, `#if` for Windows vs Mac, `#ifdef FLOWDESK_COLOR` for colour, `#error` if the dialect is older than C++17, `#pragma message` at compile time. |
| `src/main.cpp` | Creates `App` and `return desk.run();` — never a crash-exit. |
| `src/app.cpp` | `switch` on 0–9. Case 0 saves and sets `running = false`, then `return 0`. |
| `src/ui.cpp` | Banner, safe `getline` input, money formatting, today's date. |
| `src/ledger.cpp` | Add / update / remove / search / monthly averages. Caps at `FLOWDESK_MAX_RECORDS`. |
| `src/projector.cpp` | One month step: income − expense, then optional interest. Three balances walk forward together. |
| `src/storage.cpp` | CSV read and write. |
| `src/transaction.cpp` | One row: sign, label, CSV. |

---

## If someone asks “what happens with bad input?”

`UI::readInt` and `UI::readDouble` reject anything outside the allowed range and ask again. The programme does not exit until the user chooses 0.

Optional extra (not a brief step): Cursor wrote the code; ChatGPT reviewed the projection bands. See `AI_TOOLS.md`.
