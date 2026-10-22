# HalloTui - Welcome message on the command line and in the text menu

![Version](https://img.shields.io/badge/Version-1.0.0-blue?style=flat-square)
![License](https://img.shields.io/badge/License-MIT-green?style=flat-square)
[![CIAO](https://img.shields.io/badge/Philosophy-CIAO%20(Caution%20%E2%80%A2%20Intentional%20%E2%80%A2%20Anti--fragile%20%E2%80%A2%20Over--engineered)-purple.svg)](https://github.com/cloudgen/ciao)
[![Stars](https://img.shields.io/github/stars/cloudgen/HalloTui?style=flat-square)](https://github.com/cloudgen/HalloTui)
[![Python](https://img.shields.io/badge/Python-2.7%2B%20(3.x%20recommended)-blue?style=flat-square)]()
[![PyPI](https://img.shields.io/pypi/v/HalloTui?style=flat-square)](https://pypi.org/project/HalloTui/)

HalloTui prints a welcome message. The typed verb `hello` prints that line in the terminal. Menu row **1 hello** shows the same line on the text menu.

On a terminal, starting it with no arguments opens a text menu. The menu does not print the welcome line by itself, and it does not call pip. It does not join media, call an external encoder, or list files in the folder.

## Features

- Text menu on a terminal: **hello**, **system-log**, **language**, **self-management**, and **Exit**
- The first row shows the current folder and a local clock (`HH:MM:SS`) when the row has room
- Numbered rows line up. A rounded input box sits along the bottom, as wide as the terminal, with the name and version on the status line under the box
- **hello** (1) shows a welcome message on the menu. The typed verb `hello` prints that message in the terminal and does not draw the menu
- **system-log** (3) opens view-log, clear-log, and log-folder. **language** (4) picks the menu language. **self-management** (8) opens version, about, and the pip lifecycle rows
- Console command `hallo-tui`, built with GNU Make to `target/exe/debug/hallo-tui`
- Typed verbs: `help`, `version`, `about`, `hello`, `self-install`, `version-check`, `self-update`, `self-uninstall`

## Advantages

1. **Two ways to see the welcome.** `hallo-tui hello` prints one line in the terminal and does not draw the menu. On the menu, row **1 hello** shows that same line. With no arguments on a terminal, the text menu opens and waits. With no terminal and no verb, the program prints help and returns 0. There is no `--json`.
2. **Built-in languages.** Row **4** lists thirteen languages: English, Simplified Chinese, Traditional Chinese, Spanish, Arabic, French, Portuguese, Russian, German, Japanese, Korean, Dutch, and Greek. The choice is saved for the next run. The welcome line stays English. The note on row 1 follows the menu language.
3. **Lifecycle and diagnostics.** `version-check`, `self-update`, `self-install`, and `self-uninstall` are menu rows **84**–**87** and typed verbs. `self-uninstall` on the command line needs `--force`. They call pip and do not use root. **system-log** (**3**) views a log, clears a log, and shows the log folder. **about** (**83**) stays in English.

## Quick Installation

This checkout is a C++ program. No encoder is required. `./build.sh` does not accept a `make` verb.

### Build this checkout

```bash
make
./target/exe/debug/hallo-tui hello
```

`make` writes `target/exe/debug/hallo-tui`. The command name is `hallo-tui`. This tree is **1.0.0**.

### Registry

The PyPI badge above is the public index. Installing that index is not how this checkout is built.

```bash
pip install HalloTui
```

### Local install (checkout)

```bash
git clone https://github.com/cloudgen/HalloTui.git
cd HalloTui
make
```

### Main menu

After install, on a terminal, `hallo-tui` with no arguments prints this screen. The verb is bold and the note after the colon is italic. `9` leaves. `0` on a submenu goes back. After a command finishes, the main menu is shown again. A number that is not on the list prints an error and lets you choose again.

Picture of the English menu. Current directory `/tmp/clips`. The rows are the text the menu paints. The frame is the 80-column box.

```text
$ hallo-tui
Path: /tmp/clips                                                       12:06:45

1. **hello**          : *show a welcome message*
3. **system-log**     : *view, clear, and the log folder*
4. **language**       : *display language for this menu*
8. **self-management**: *version, about, and pip lifecycle*
9. **Exit**           : *leave*




╭─────────────────────────────────────────────────────────────────────────────╮
│ >                                                                           │
╰─────────────────────────────────────────────────────────────────────────────╯
  HalloTui 1.0.0  │  main menu  │  Up/Down  •  Enter
```

**system-log** (3):

```text
Path: /tmp/clips                                                       12:06:45

31. **view-log**  : *list a log file and show it*
32. **clear-log** : *empty one log file*
33. **log-folder**: *show the log folder*
 0. **Back**      : *return to the main menu*




╭─────────────────────────────────────────────────────────────────────────────╮
│ >                                                                           │
╰─────────────────────────────────────────────────────────────────────────────╯
  HalloTui 1.0.0  │  system-log  │  Up/Down  •  Enter
```

**language** (4). The short on each language row is that language’s own name. Numbers 40 and 54–59 are not printed. `0` goes back and does not save.

```text
Path: /tmp/clips                                                       12:06:46

41. **English**   : *use English for this menu*
42. **简体中文**      : *use 简体中文 for this menu*
43. **繁體中文**      : *use 繁體中文 for this menu*
44. **Español**   : *use Español for this menu*
45. **العربية**   : *use العربية for this menu*
46. **Français**  : *use Français for this menu*
47. **Português** : *use Português for this menu*
48. **Русский**   : *use Русский for this menu*
49. **Deutsch**   : *use Deutsch for this menu*
50. **日本語**       : *use 日本語 for this menu*
51. **한국어**       : *use 한국어 for this menu*
52. **Nederlands**: *use Nederlands for this menu*
53. **Ελληνικά**  : *use Ελληνικά for this menu*
 0. **Back**      : *return to the main menu*

╭─────────────────────────────────────────────────────────────────────────────╮
│ >                                                                           │
╰─────────────────────────────────────────────────────────────────────────────╯
  HalloTui 1.0.0  │  language  │  Up/Down  •  Enter
```

**self-management** (8):

```text
Path: /tmp/clips                                                       14:05:09

82. **version**       : *show the installed version*
83. **about**         : *version and this computer*
84. **version-check** : *compare this install with pip*
85. **self-update**   : *upgrade this package with pip*
86. **self-uninstall**: *remove this package with pip*
87. **self-install**  : *install this package with pip*
 0. **Back**          : *return to the main menu*

╭─────────────────────────────────────────────────────────────────────────────╮
│ >                                                                           │
╰─────────────────────────────────────────────────────────────────────────────╯
  HalloTui 1.0.0  │  self-management  │  Up/Down  •  Enter
```

Choose a number, or type the command name in the box. The block caret appears in that box while it is focused.

## Usage

```bash
hallo-tui
```

On a terminal, that opens the main menu above. It does not show the welcome line yet, and it does not call pip. With no terminal, the same command prints help and returns 0.

**hello** (1) stays on the menu and shows `Welcome to HalloTui 1.0.0.` Any key returns to the main menu. The typed verb prints that same line in the terminal and does not draw the menu:

```bash
hallo-tui hello
```

```bash
hallo-tui help
hallo-tui version
hallo-tui about
hallo-tui hello
hallo-tui version-check
hallo-tui self-update
hallo-tui self-install
hallo-tui self-uninstall --force
```

`version` prints `HalloTui 1.0.0` and does not call pip. `about` shows one English page: the product identity, a host check of this computer, and a star box. It does not call pip. `hello` prints `Welcome to HalloTui 1.0.0.` and does not draw the menu. `help` prints usage.

`version-check` runs `python -m pip index versions HalloTui`. `self-update` runs `python -m pip install --upgrade HalloTui`. `self-install` runs `python -m pip install HalloTui`. `self-uninstall` runs `python -m pip uninstall -y HalloTui` and needs `--force` on the command line. Those pip verbs do not use sudo. Empty arguments do not install or update.

Exit codes: non-zero for an unknown verb, a pip failure, or a menu that cannot open. Menu **Exit** (9) returns 0. `hello` returns 0.

## Screenshots

Each heading is the file name. The paragraph is what that picture shows: the words on the screen, the characters in the input box, or the scene. Package **1.0.0**. Menu pictures captured before the about page still show **1.0.4** where the paragraph says so. Main-menu pictures and the join pictures were captured when row 1 was join. The current board is the fenced menu in Quick Installation: row 1 is **hello**. The package index can fetch a picture only after that file is on the public `main` branch.

### `language-menu.png`

Row **4** has opened the language list. **41 English** is highlighted, with the note "use English for this menu." The other rows are **42 简体中文**, **43 繁體中文**, **44 Español**, **45 العربية**, **46 Français**, **47 Português**, **48 Русский**, **49 Deutsch**, **50 日本語**, **51 한국어**, **52 Nederlands**, and **53 Ελληνικά**. Each note says to use that language for this menu. **0 Back** says "return to the main menu." The path label is `Path`. The clock is on the right of that row. The status line says `language`. VideoJoin 1.0.4.

![Language list, 41 English highlighted](https://raw.githubusercontent.com/cloudgen/HalloTui/main/screenshots/language-menu.png)

### `main-menu-en.png`

English main menu. There is no saved-language line above the box. The path label is `Path`. The clock is on the right of that row. **1 join** is highlighted: "pick two videos in this folder and concatenate." Then **3 system-log** "view, clear, and the log folder," **4 language** "display language for this menu," **8 self-management** "version, about, and pip lifecycle," and **9 Exit** "leave." The status line says `main menu`.

![English main menu](https://raw.githubusercontent.com/cloudgen/HalloTui/main/screenshots/main-menu-en.png)

### `main-menu-zh-hans.png`

Simplified Chinese main menu. The line above the box says `菜单语言是简体中文`. The path label is `路径`. The clock is on the right of that row. **1 join** is highlighted and stays `join`. **3** is `系统日志`, **4** is `语言`, **8** is `自我管理`, and **9** is `离开`. The status line says `主菜单`.

![Simplified Chinese main menu](https://raw.githubusercontent.com/cloudgen/HalloTui/main/screenshots/main-menu-zh-hans.png)

### `main-menu-zh-hant.png`

Traditional Chinese main menu. The line above the box says `選單語言是繁體中文`. The path label is `路徑`. The clock is on the right of that row. **1 join** is highlighted and stays `join`. **3** is `系統日誌`, **4** is `語言`, **8** is `自我管理`, and **9** is `離開`. The status line says `主選單`.

![Traditional Chinese main menu](https://raw.githubusercontent.com/cloudgen/HalloTui/main/screenshots/main-menu-zh-hant.png)

### `main-menu-es.png`

Spanish main menu. The line above the box says `El idioma del menú es español`. The path label is `Ruta`. The clock is on the right of that row. **1 join** is highlighted and stays `join`. **3** is `registro`, **4** is `idioma`, **8** is `autogestión`, and **9** is `Salir`. The status line says `menú principal`.

![Spanish main menu](https://raw.githubusercontent.com/cloudgen/HalloTui/main/screenshots/main-menu-es.png)

### `main-menu-ar.png`

Arabic main menu. The line above the box says `لغة القائمة هي العربية`. The path label is `المسار`. The clock is on the right of that row. The numbers stay on the left. Arabic words on each row are shaped and read right to left. **1 join** is highlighted and stays `join`. **3** is `سجل النظام`, **4** is `لغة`, **8** is `إدارة ذاتية`, and **9** is `خروج`. The status line says `القائمة الرئيسية`.

![Arabic main menu](https://raw.githubusercontent.com/cloudgen/HalloTui/main/screenshots/main-menu-ar.png)

### `main-menu-fr.png`

French main menu. The line above the box says `La langue du menu est le français`. The path label is `Chemin`. The clock is on the right of that row. **1 join** is highlighted and stays `join`. **3** is `journal`, **4** is `langue`, **8** is `autogestion`, and **9** is `Quitter`. The status line says `menu principal`.

![French main menu](https://raw.githubusercontent.com/cloudgen/HalloTui/main/screenshots/main-menu-fr.png)

### `main-menu-pt.png`

Portuguese main menu. The line above the box says `O idioma do menu é português`. The path label is `Caminho`. The clock is on the right of that row. **1 join** is highlighted and stays `join`. **3** is `registo`, **4** is `idioma`, **8** is `autogestão`, and **9** is `Sair`. The status line says `menu principal`.

![Portuguese main menu](https://raw.githubusercontent.com/cloudgen/HalloTui/main/screenshots/main-menu-pt.png)

### `main-menu-ru.png`

Russian main menu. The line above the box says `Язык меню — русский`. The path label is `Путь`. The clock is on the right of that row. **1 join** is highlighted and stays `join`. **3** is `системный журнал`, **4** is `язык`, **8** is `самоуправление`, and **9** is `Выход`. The status line says `главное меню`.

![Russian main menu](https://raw.githubusercontent.com/cloudgen/HalloTui/main/screenshots/main-menu-ru.png)

### `main-menu-de.png`

German main menu. The line above the box says `Die Menüsprache ist Deutsch`. The path label is `Pfad`. The clock is on the right of that row. **1 join** is highlighted and stays `join`. **3** is `Systemprotokoll`, **4** is `Sprache`, **8** is `Selbstverwaltung`, and **9** is `Beenden`. The status line says `Hauptmenü`.

![German main menu](https://raw.githubusercontent.com/cloudgen/HalloTui/main/screenshots/main-menu-de.png)

### `main-menu-ja.png`

Japanese main menu. The line above the box says `メニューの言語は日本語`. The path label is `パス`. The clock is on the right of that row. **1 join** is highlighted and stays `join`. **3** is `システムログ`, **4** is `言語`, **8** is `自己管理`, and **9** is `終了`. The status line says `メインメニュー`.

![Japanese main menu](https://raw.githubusercontent.com/cloudgen/HalloTui/main/screenshots/main-menu-ja.png)

### `main-menu-ko.png`

Korean main menu. The line above the box says `메뉴 언어는 한국어`. The path label is `경로`. The clock is on the right of that row. **1 join** is highlighted and stays `join`. **3** is `시스템 로그`, **4** is `언어`, **8** is `자기관리`, and **9** is `종료`. The status line says `주 메뉴`.

![Korean main menu](https://raw.githubusercontent.com/cloudgen/HalloTui/main/screenshots/main-menu-ko.png)

### `main-menu-nl.png`

Dutch main menu. The line above the box says `De menutaal is Nederlands`. The path label is `Pad`. The clock is on the right of that row. **1 join** is highlighted and stays `join`. **3** is `systeemlog`, **4** is `taal`, **8** is `zelfbeheer`, and **9** is `Afsluiten`. The status line says `hoofdmenu`.

![Dutch main menu](https://raw.githubusercontent.com/cloudgen/HalloTui/main/screenshots/main-menu-nl.png)

### `main-menu-el.png`

Greek main menu. The line above the box says `Η γλώσσα του μενού είναι ελληνικά`. The path label is `Διαδρομή`. The clock is on the right of that row. **1 join** is highlighted and stays `join`. **3** is `αρχείο καταγραφής`, **4** is `γλώσσα`, **8** is `αυτοδιαχείριση`, and **9** is `Έξοδος`. The status line says `κύριο μενού`.

![Greek main menu](https://raw.githubusercontent.com/cloudgen/HalloTui/main/screenshots/main-menu-el.png)

### `join-first.png`

Join, first question. The title is `VideoJoin (1.0.4) — join`. The page says "Join two videos in this folder." and "Esc returns to the menu." The list is `1. clip-a.mp4` and `2. clip-b.mp4`. The prompt is "Choose FIRST video →". The input box contains `1` and the block caret. The status line says `join`. There is no clock on this page.

![Choose the first video, 1 typed](https://raw.githubusercontent.com/cloudgen/HalloTui/main/screenshots/join-first.png)

### `join-second.png`

Join, second question. The title is `VideoJoin (1.0.4) — join`. The remaining list is `1. clip-b.mp4`. The prompt is "Choose SECOND video →". The input box contains `1` and the block caret. The status line says `join`.

![Choose the second video, 1 typed for clip-b.mp4](https://raw.githubusercontent.com/cloudgen/HalloTui/main/screenshots/join-second.png)

### `join-output.png`

Join, output name. The page says "Joining:", then `clip-a.mp4`, then `+ clip-b.mp4`, then "Output filename [clip-a + clip-b.mp4]:". The input box is empty except for the block caret, so the bracketed default stands. The status line says `join`.

![Output name, default clip-a + clip-b.mp4, box empty](https://raw.githubusercontent.com/cloudgen/HalloTui/main/screenshots/join-output.png)

### `join-done.png`

The join has finished by stream copy. The page says "Joining with perfect audio sync:", lists `clip-a.mp4` and `clip-b.mp4`, then "→ clip-a + clip-b.mp4", then "Staging dir → .", then "Running ffmpeg (stream copy – no quality loss)…", then "SUCCESS! Perfectly joined with original sound → clip-a + clip-b.mp4". The footer says "Press a key to return to the main menu." The input box is empty. The status line says `join`.

![Stream-copy join of clip-a.mp4 and clip-b.mp4 succeeded](https://raw.githubusercontent.com/cloudgen/HalloTui/main/screenshots/join-done.png)

### `self-management.png`

Row **8** has opened self-management. **82 version** is highlighted: "show the installed version." Then **83 about** "version, FFmpeg, and this computer", **84 version-check** "compare this install with pip", **85 self-update** "upgrade this package with pip", **86 self-uninstall** "remove this package with pip", **87 self-install** "install this package with pip", and **0 Back** "return to the main menu." The path label is `Path`. The path is `/tmp/clips`. The clock is `14:05:09` on the right of that row. The status line says `VideoJoin 1.0.5` and `self-management`.

![Self-management, 82 version highlighted](https://raw.githubusercontent.com/cloudgen/HalloTui/main/screenshots/self-management.png)

### `tui-about.png`

**about** (83) on the result page. The title is `VideoJoin (1.0.5) — result`. The page prints `VideoJoin 1.0.5`, `Domain: Concatenate two local videos with FFmpeg (copy, then fallback)`, `Runtime tools: FFmpeg (copy, then re-encode)`, and `Entry points: video-join, python -m VideoJoin`. The host check is stamped `2026-10-04 21:14:59.669782` and headed `[CHECK SYSTEM]:`. Visible lines include Python 3.12.11, C Library GCC 13.3.0, Ubuntu 24.04.5 LTS, amd64, the current user, the shell, the Python executable, python2 location, python3 location, conda location, pyenv location, `Inside docker container: False`, and `Cython String: cpython-312-x86_64-linux-gnu`. The footer says `Up/Down scrolls this page.` and `Press a key to return to the main menu.` There is no input box and no clock. The page stays in English.

![About host check, English](https://raw.githubusercontent.com/cloudgen/HalloTui/main/screenshots/tui-about.png)

### `system-log.png`

Row **3** has opened system-log. **31 view-log** is highlighted: "list a log file and show it." Then **32 clear-log** "empty one log file", **33 log-folder** "show the log folder", and **0 Back** "return to the main menu." The path label is `Path`. The clock is on the right of that row. The status line says `system-log`.

![System log, 31 view-log highlighted](https://raw.githubusercontent.com/cloudgen/HalloTui/main/screenshots/system-log.png)

### `video.png`

A still picture, not a text-menu capture. A woman with long brown hair, in a grey shirt with a small blue mark, sits at a round wooden table and holds a white cup. An open book with a worn cover and a metal clasp lies on the table. Behind her are a beige sofa, a wide window onto trees, a potted plant, and a wooden floor in daylight.

![Woman at a table with an open book and a cup](https://raw.githubusercontent.com/cloudgen/HalloTui/main/screenshots/video.png)

## Examples

```bash
cd /path/to/folder/with/clips
hallo-tui
```

```bash
hallo-tui version
```

`version` prints `HalloTui 1.0.0` and does not call pip. `hello` prints only the welcome line.

```bash
hallo-tui hello
```

That prints `Welcome to HalloTui 1.0.0.` and does not draw the menu. On a terminal, row **1** shows the same line.

## Platform Compatibility

| Platform | Status |
|----------|--------|
| Linux | Primary; tested development path |
| macOS | Supported when CPython is on PATH |
| Windows | Supported when CPython is on PATH (venv activate differs) |
| Architectures | Any with CPython |

The text menu needs a terminal. With no terminal and no verb, the program prints help and returns 0. It does not wait.

## Related Projects

- [HalloTui on GitHub](https://github.com/cloudgen/HalloTui) — this program’s source
- [HalloTui on PyPI](https://pypi.org/project/HalloTui/) — this program on PyPI
- [AnimeDlp](https://github.com/Wilgat/AnimeDlp) — command-line downloader for anime video sites
- [ChronicleLogger](https://github.com/Wilgat/ChronicleLogger) — status logger this program depends on (`ChronicleLogger>=1.3.1`)
- [VideoSpeed](https://github.com/Wilgat/VideoSpeed) — cuts, changes speed, and boomerangs a clip. That program is not this one
- [CIAO](https://github.com/cloudgen/ciao) — Caution, Intentional, Anti-fragile, Over-engineered
- [CIAO-Lite](https://github.com/cloudgen/ciao-lite) — short agent contract
- [safe-rm](https://github.com/cloudgen/safe-rm) — guarded `rm`

## Contributing

1. Keep product law under `docs/requirements/` in sync when behavior changes.
2. Prefer small, CIAO-safe changes. Do not put media joining, an external encoder, or a file listing back without an explicit design.
3. Version dual SSOT: bump `kVersion` in `src/main/cpp/project.h` and `VERSION` in `project.ini` together.
4. Open issues and pull requests on GitHub.

## License

MIT — see [`LICENSE.md`](./LICENSE.md). Also declared in `pyproject.toml`.

## Last Update

2026-10-22 — **1.0.0**. The public repository is [https://github.com/cloudgen/HalloTui](https://github.com/cloudgen/HalloTui). The console command is `hallo-tui`. `hello` prints `Welcome to HalloTui 1.0.0.` in the terminal. Menu row 1 **hello** shows that line. Join, file listing, and the external encoder are gone. Menu pictures captured earlier still show row 1 as join and the previous name. Version badge matches `kVersion` in `src/main/cpp/project.h` and `VERSION` in `project.ini`. `ChronicleLogger>=1.3.1` is required.
