**file**: docs/requirements/requirement-cpp-folder.md
**Status**: Active (Version 1.0.1)
**Area**: cpp
**Key**: `requirement-cpp-folder`
**Philosophy**: CIAO / CIAO-Lite

## 1. Purpose

This file owns the C++ ship-unit directory and the GNU Make source list for HalloTui.

The text menu is `requirement-cpp-tui`. Naming is `requirement-cpp-coding-style`. The welcome line is `requirement-domain-videojoin`. Typed verbs are `requirement-python-cli-interface`. `kVersion` in `project.h` is `requirement-cpp-version`.

**In one sentence:** `make` writes `target/exe/debug/hallo-tui` from `src/main/cpp`.

| Includes | Excludes |
|----------|----------|
| One source root, the translation units, and the debug binary path | A second root at `src/main/c` |
| `g++ -std=c++17` and one `Makefile` | Autotools, a `make` verb on `./build.sh`, GTK, WebKit, Lua, SQLite, sol2 |

## 2. Core Rules (Mandatory)

1. The ship-unit source directory **MUST** be `src/main/cpp`.
2. There **MUST** be one ship-unit source root. Do not keep sources under `src/main/c`.
3. A translation unit in that directory **MUST** be `.cpp` unless this file names a `.c` file. This file names none.
4. A header named here **MUST** stay `.h`.
5. Shell scripts, the `Makefile`, and Markdown **MUST NOT** go in `src/main/cpp`.
6. The build source list **MUST** be the translation units in the table below. `main.cpp` links the program. It **MUST NOT** link into the test binary.
7. `make` from the project root **MUST** write `target/exe/debug/hallo-tui`. `project.ini` sets `PRJ=hallo-tui` and `BUILD_DEBUG=target/exe/debug`.
8. The compiler **MUST** be `g++` with `-std=c++17`. The link line **MUST** include ncurses. It **MUST NOT** add GTK, WebKit, Lua, SQLite, or sol2.
9. `./build.sh` **MUST NOT** gain a `make` or `gnumake` verb from this file. The ship command is `make`.

### Translation units

| Unit | Role |
|------|------|
| `main.cpp` | Process entry. Not in the test link |
| `cli.cpp` / `cli.h` | Verbs and empty argv |
| `tui.cpp` / `tui.h` | Menu session for the operator |
| `menu_painter.cpp` / `menu_painter.h` | Frame, rows, path line |
| `menu_model.cpp` / `menu_model.h` | Highlight, buffer, commit |
| `menu_session.cpp` / `menu_session.h` | Key loop and clock wait |
| `menu_language.cpp` / `menu_language.h` | Thirteen codes and the leaf |
| `menu_text_data.cpp` / `menu_text_data.h` | Generated words. Do not hand-edit the tables |
| `text_width.cpp` / `text_width.h` | Display columns |
| `screen.h` | Surface the painter draws on |
| `curses_screen.cpp` / `curses_screen.h` | ncurses surface |
| `about_page.cpp` / `about_page.h` | About page |
| `self_manage.cpp` / `self_manage.h` | Pip argv for rows 84–87 |
| `system_log.cpp` / `system_log.h` | Log folder reads |
| `project.h` | Identity constants. `kVersion` is `requirement-cpp-version` |

## 3. Protection Rule

Future editors **MUST NOT** move this directory rule into the coding-style file as a second owner, and **MUST NOT** put the ship unit back under a Python package path.

## Design-time verification

| TP-ID | Suite | Status | Note |
|-------|-------|--------|------|
| TP-CPP-01 | `make` then `make test` | have | Binary `target/exe/debug/hallo-tui`. Tests do not link `main.cpp` |

## Terminologies

### Project target version

**Definition:** Project target version is the user-confirmed product version string (typically MAJOR.MINOR.PATCH, for example 1.0.0 or 1.7.0) that agents must collect from the user (or confirm from an existing product root README.md parse). After it is written into product root README.md, that file is the single source of truth for this field, displayed once in the product README header kit as the shields.io Version badge. It is not hardcoded into skills, requirements, templates, or terminology files.

**Human daily-life explanation:** Project target version is the user-confirmed version string shown once as the README Version badge. Skills do not own it.

**Daily-life example:** The edition number printed on a cookbook's cover; the inner chapters should match, but the cover is what you asked to print.

## Status history

| Date | Status | Notes |
|------|--------|-------|
| 2026-10-22 | Active 1.0.0 | Ship-unit directory and GNU Make binary |
| 2026-10-22 | Active 1.0.1 | `project.h` `kVersion` points at `requirement-cpp-version` |

**Last Updated**: 2026-10-22
