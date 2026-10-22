# Tests — HalloTui

Executable proof for product law. **Design map:** `docs/reviews/test-plan.md`.
**RTM:** `docs/reviews/requirement-test-matrix.md`.

## Status (2026-10-22, product 1.0.0)

| Item | State |
|------|--------|
| Design TP map | **present** under `docs/reviews/test-plan.md` |
| Automated suites | `tests/cpp/test_main.cpp` covers the hello line, `TP-TUI-02`, `TP-TUI-04`, `TP-TUI-05`, `TP-TUI-06`, `TP-TUI-08`, `TP-TUI-09`, `TP-TUI-10`, `TP-LANG-01`, and the C++ about rows that stay have. `tests/test_docs.py` covers **TP-DOC-01**. **TP-DOC-03** stays todo. **TP-TUI-01** stays todo. Media rows are n/a |
| Runner | `tests/run.sh` (`make test`, then `tests.test_docs`) |
| Last run | 2026-10-22 `make test` — `hallo-tui-tests ok` |

## Layout

| File | TP families |
|------|-------------|
| `tests/cpp/test_main.cpp` | Menu, language, hello, about page shape |
| `test_docs.py` | TP-DOC-01 (have). TP-DOC-03 stays todo |

## Rules

1. Assert messages / test names include the **TP-ID** when the case is a named TP row.
2. Core cases do not require the public network.
3. Flip `todo` → `have` in `docs/reviews/test-plan.md` only after a green run.
4. Do not place executable tests under `docs/templates/`.

## Run

```bash
./tests/run.sh
```
