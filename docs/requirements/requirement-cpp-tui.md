**file**: docs/requirements/requirement-cpp-tui.md
**Status**: Active (Version 1.0.1)
**Area**: cpp
**Key**: `requirement-cpp-tui`
**Philosophy**: CIAO / CIAO-Lite

## 1. Purpose

This file is the picture of the HalloTui text menu. It replaces `requirement-python-tui` as the Active menu law.

That older file contradicts itself: its rule 2 and its picture still say row 1 is `join`, while its rule 6, the requirements index, and the domain file say row 1 is `hello`. The live menu is the hello board. Do not implement the join picture.

**In one sentence:** On a terminal, `hallo-tui` with no arguments shows hello, system-log, language, self-management, and Exit, plus a rounded input box.

| Includes | Excludes |
|----------|----------|
| Front rows **1** hello, **3** system-log, **4** language, **8** self-management, **9** Exit | Row 2. `join`. `list-videos`. An encoder. A file list |
| Self-management **82**–**87** and **0** Back. System-log **31**–**33** and **0** Back. Language **41**–**53** and **0** Back | Numbers **40** and **54**–**59** printed on the language board |

## 2. Core Rules (Mandatory)

1. **Homes.** The session **MUST** be class `Tui` in `src/main/cpp/tui.cpp`. The frame **MUST** be class `MenuPainter` in `src/main/cpp/menu_painter.cpp`. The product **MUST NOT** import another menu package and **MUST NOT** draw the menu with a `Choice:` line.
2. **Two regions.** The first row of the menu region **MUST** be the path line. It **MUST NOT** show the product name or the version. Numbered rows follow it and **MUST NOT** sit inside the box. The verb field is padded to the display width of the longest verb on that board. The explain follows the colon and one space.
3. **Columns.** East Asian Wide and Fullwidth count as two columns. Ambiguous stays one. Box drawing and the block caret `█` (U+2588) stay width one. Do not use `wcwidth`. Clip on UTF-8 code points. A wide character that does not fit whole is dropped.
4. **Frame.** The box is three rows: `╭` `─` `╮` (U+256D U+2500 U+256E), `│` (U+2502), and `╰` `─` `╯` (U+2570 U+2500 U+256F). The input mark is `> `. The caret is `█`. The status line is `  HalloTui 1.0.0  │  <title>  │  Up/Down  •  Enter`. The name and the version in that line are the string owned by `requirement-cpp-version`.
5. **Path line.** The label is `Path` until a saved language selects another word. Traditional Chinese uses `路徑`. The folder path and the clock **MUST NOT** be translated. `LANG` **MUST NOT** select the label. The clock is local `HH:MM:SS`. When the row has room, the clock ends on the last placeable column. A narrow row keeps the label and the tail of the directory and omits the clock.
6. **Front board.** The rows **MUST** be exactly: **1** `hello` — show a welcome message, kind `hello`; **3** `system-log`; **4** `language`; **8** `self-management`; **9** `Exit`, kind `exit`. Row 2 is omitted. The short `hello` stays the English token `hello`. Choosing **1** **MUST** show `Welcome to HalloTui 1.0.0.` on the result page. The typed verb `hallo-tui hello` prints that same line and a newline and **MUST NOT** draw this screen.
7. **Other boards.** System-log is **31** `view-log`, **32** `clear-log`, **33** `log-folder`, **0** Back. Self-management is **82** `version`, **83** `about` (explain `version and this computer`), **84** `version-check`, **85** `self-update`, **86** `self-uninstall`, **87** `self-install`, **0** Back. Row **82** shows the line from `requirement-cpp-version` and does not call pip. `hallo-tui version` does not open this screen. Language rows are the thirteen endonyms **41** through **53**, then **0** Back. Leaf shorts stay English. Category shorts, explains, and Back follow the selected language.
8. **Clock wait.** On the front, self, log, and language boards the session waits at most one second. A no-key **MUST** redraw the clock. It **MUST NOT** be Esc and **MUST NOT** exit. Result pages and questions use a blocking read.
9. **Pip rows.** The labels and the help sentences **MUST** name `python -m pip` with distribution `HalloTui`, the same sentences as the previous Python menu. The process runs `python3 -m pip` because this binary has no Python `sys.executable`. It **MUST NOT** use sudo or curl. Empty argv **MUST NOT** run pip.
10. **Too small or no terminal.** A screen that cannot hold the path line, one menu row, the three-row box, and the status line **MUST** fail closed. Empty argv with no terminal **MUST** print help and return 0. `join` and `list-videos` are unknown verbs and **MUST** exit non-zero.

## 3. Protection Rule

Future editors **MUST NOT** restore row 1 as `join`, and **MUST NOT** treat the superseded picture in `requirement-python-tui` as Active law.

## Design-time verification

| TP-ID | Suite | Status | Note |
|-------|-------|--------|------|
| TP-TUI-09 | `tests/cpp/test_main.cpp` | have | Path label |
| TP-TUI-10 | `tests/cpp/test_main.cpp` | have | Wide columns and the CJK rows |
| TP-TUI-02 | `tests/cpp/test_main.cpp` | have | Empty argv off a tty prints help |
| TP-TUI-04 | `tests/cpp/test_main.cpp` | have | Rows 82–87 and the pip argv |
| TP-TUI-06 | `tests/cpp/test_main.cpp` | have | No `Choice:` line |
| TP-TUI-08 | `tests/cpp/test_main.cpp` | have | Clock timeout is not Exit |
| TP-TUI-01 | live tty open | todo | `opensTextScreen` is proven; a real terminal draw is not |
| TP-TUI-03 | n/a | n/a | Join questions are retired |
| TP-TUI-05 | `tests/cpp/test_main.cpp` | have | Rows 31–33. File-list clock clear is not a separate case |
| TP-TUI-07 | n/a | n/a | `join` is an unknown verb, covered with hello |

## Terminologies

### Project target version

**Definition:** Project target version is the user-confirmed product version string (typically MAJOR.MINOR.PATCH, for example 1.0.0 or 1.7.0) that agents must collect from the user (or confirm from an existing product root README.md parse). After it is written into product root README.md, that file is the single source of truth for this field, displayed once in the product README header kit as the shields.io Version badge. It is not hardcoded into skills, requirements, templates, or terminology files.

**Human daily-life explanation:** Project target version is the user-confirmed version string shown once as the README Version badge. Skills do not own it.

**Daily-life example:** The edition number printed on a cookbook's cover; the inner chapters should match, but the cover is what you asked to print.

## Status history

| Date | Status | Notes |
|------|--------|-------|
| 2026-10-22 | Active 1.0.0 | Hello front board replaces the withdrawn join picture |
| 2026-10-22 | Active 1.0.1 | Status line and row **82** use `requirement-cpp-version` |

**Last Updated**: 2026-10-22
