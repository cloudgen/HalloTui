# Requirements index

**Product:** HalloTui (C++ CLI — text menu on a terminal; `hello` prints a welcome line, menu row 1 shows it)
**Workspace state:** Specialized product law; **software-development** class; GNU Make binary (not a shell online installer).
**Product version:** **1.0.0** (`project.ini`, `src/main/cpp/project.h`, root `README.md` Version badge)
**Updated:** 2026-10-22

| ID / key | Title | Area | Status | Path | Updated |
|----------|-------|------|--------|------|---------|
| requirement-class-software-dev | Software-development class law + residual stack (C++, g++, GNU Make) | class | Active | `requirement-class-software-dev.md` | 2026-10-22 |
| requirement-domain-videojoin | Domain surface SSOT (welcome line: typed `hello`, menu row 1, help, about) | domain | Active | `requirement-domain-videojoin.md` | 2026-10-22 |
| requirement-cpp-folder | C++ ship unit `src/main/cpp` and `make` → `target/exe/debug/hallo-tui` | cpp | Active | `requirement-cpp-folder.md` | 2026-10-22 |
| requirement-cpp-coding-style | C++17 naming. Logger unit not present yet | cpp | Active | `requirement-cpp-coding-style.md` | 2026-10-22 |
| requirement-cpp-version | One version string. `version` and `--version` print `HalloTui 1.0.0` | cpp | Active | `requirement-cpp-version.md` | 2026-10-22 |
| requirement-cpp-tui | Text menu: hello, system-log, language, self-management, Exit | cpp | Active | `requirement-cpp-tui.md` | 2026-10-22 |
| requirement-python-about | About page: identity, host check, star box. C++ field list amends the Python host lines | python | Active | `requirement-python-about.md` | 2026-10-22 |
| requirement-video-ffmpeg-pipeline | Retired. No encoder, no concat, no media publish | video | Retired | `requirement-video-ffmpeg-pipeline.md` | 2026-10-05 |
| requirement-python-cli-interface | CLI entry, typed verbs, empty-argv menu on a terminal and help off a terminal | python | Active | `requirement-python-cli-interface.md` | 2026-10-22 |
| requirement-python-cli-logging | ChronicleLogger in `def main`. Not the C++ ship unit | python | Superseded | `requirement-python-cli-logging.md` | 2026-10-22 |
| requirement-python-tui | Withdrawn join picture. Active menu is `requirement-cpp-tui` | python | Superseded | `requirement-python-tui.md` | 2026-10-22 |
| requirement-python-cli-language | Menu language: front row 4, codes 41–53, leaf `~/.local/HalloTui/language` | python | Active | `requirement-python-cli-language.md` | 2026-10-22 |
| requirement-python-oop | Python class map. Not the C++ ship unit | python | Superseded | `requirement-python-oop.md` | 2026-10-22 |
| requirement-python-coding-style | Python style and shutil.publish. Not the C++ ship unit | python | Superseded | `requirement-python-coding-style.md` | 2026-10-22 |
| requirement-python-packaging | `pyproject.toml` package. Not the C++ ship SSOT | python | Superseded | `requirement-python-packaging.md` | 2026-10-22 |
| requirement-python-project-structure | `src/VideoJoin` layout. Not the C++ ship unit | python | Superseded | `requirement-python-project-structure.md` | 2026-10-22 |
| requirement-python-readme | Root user document: sections, badges, picture catalog, related projects | python | Active | `requirement-python-readme.md` | 2026-10-22 |
| requirement-python-error-handling | Fail-closed errors; console sentences stay | python | Active | `requirement-python-error-handling.md` | 2026-10-04 |
| requirement-runtime-prerequisites | No encoder. No root auto-install. C++ binary does not import ChronicleLogger | runtime | Active | `requirement-runtime-prerequisites.md` | 2026-10-22 |

## Intentionally absent (by design)

| Surface | Status on HalloTui |
|---------|----------------------|
| Shell online install / `SCRIPT_URL` / Type O empty-argv install-ensure / `curl\|sh` | **Absent** |
| Shell local `install` / `uninstall` / shell self-update | **Absent** |
| Type 1 sudoers / root elevation allowlist | **Absent** |
| Automatic companion `.sha256` channel integrity law | **Absent** |
| OpenCV / duration probe dependency | **Absent** |
| Second Active `requirement-domain-*` | **Forbidden** while domain-videojoin is Active |
| Front row 2, cut / speed / boomerang, `list-mp4`, `join`, `list-videos`, FFmpeg | **Absent**. Front row 1 is `hello` |
| Second Active text-menu file | **Forbidden**. `requirement-cpp-tui` is the picture. `requirement-python-tui` is Superseded |
| `requirement-python-dependency-management` | **Absent** |
| `requirement-python-version` | **Not created**. The sibling Python program kept one package string and had no file with this name. The owner is `requirement-cpp-version` |
| `requirement-python-json-output` | **Absent**. This product does not claim `--json` |
| `requirement-python-graceful-exit` | **Absent**. Row 9 Exit returns 0 with no confirm |
| Actor-role requirement file | **Absent**. Class residual is no dest approver |

**Install mode:** GNU Make. Binary `target/exe/debug/hallo-tui`. Console command `hallo-tui`. Not a shell installer. `./build.sh` has no `make` verb.

**Pip lifecycle:** claimed on the menu only. Rows 84–87 and the typed verbs `version-check`, `self-update`, `self-install`, and `self-uninstall` use the `python -m pip` sentences. The process runs `python3 -m pip`. They are not a shell channel. Empty argv does not run them.

**Logger:** the C++ ship unit has no `logger_*` translation unit and does not construct ChronicleLogger. Typed `hello` prints only the welcome line.

**Rules for agents:**

1. Treat rows above as the **live product-law inventory** for HalloTui. A Superseded file is history. Do not implement it.
2. **Do not invent** additional `requirement-*.md` paths — verify on disk and add a registry row in the same change when creating one.
3. Product source comments cite **only** these live requirement files.
4. This versioned surface lists **requirement rows only**.
5. Keep Status and Path in sync with each file’s header when status changes.
6. **Class gate:** software-development requires exactly one Active `requirement-class-software-dev.md`.
7. **Domain SSOT:** exactly one Active domain file (`requirement-domain-videojoin`).
8. **Do not introduce** a shell installer or Type 1 elevation without an explicit user order and a registry update.
9. **Do not** treat pip rows 84–87 as a reason to add a shell installer. **Do not** mark those pip rows absent.
10. **Do not** restore menu row 1 as `join`.

When adding a requirement: append a row, create the file under `docs/requirements/`, keep Status in sync with the file header.
