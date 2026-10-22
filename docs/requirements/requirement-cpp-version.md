**file**: docs/requirements/requirement-cpp-version.md
**Status**: Active (Version 1.0.1)
**Area**: cpp
**Key**: `requirement-cpp-version`
**Philosophy**: CIAO **v2.10.2** / CIAO-Lite (Caution • Intentional • Anti-fragile • Over-engineered / Over-protect)

## 1. Purpose

This file owns the one product version string for HalloTui. The sibling Python program kept that string as `__version__` and printed `Name version` from the `version` verb and from `--version`. It did not keep a second integer triple. This file is that rule for the C++ ship unit.

The welcome sentence stays on `requirement-domain-videojoin`. The about page stays on `requirement-python-about`. The menu row that shows the string stays on `requirement-cpp-tui`. The typed verb is also named on `requirement-python-cli-interface`. The Version badge is `requirement-python-readme`. This file does not restate those pages.

The verb collects no fields and no secrets. There is no one-field-at-a-time walk. `hallo-tui version` and `hallo-tui --version` stay on the terminal. They do not open the text screen. Menu row **82** shows the same characters on the result page and does not call pip.

### 1.1 Human-facing

**In one sentence:** You type `hallo-tui version` or `hallo-tui --version` and the program prints `HalloTui 1.0.0` and stops.

| Box | Meaning | Example |
|-----|---------|---------|
| You / this login | Person asking which edition is built | `hallo-tui version` |
| The other role | The menu row and the welcome line, which reuse this string | Row **82**, and `hallo-tui hello` |
| Not this file | How the frame is drawn, and the pip upgrade command | Text menu and the pip verbs |

| Includes | Excludes |
|----------|----------|
| One string, the two places that must match, and the two commands that print it | A second integer triple named MAJOR, MINOR, and PATCH |
| Menu row **82** showing that same line without calling pip | Opening the text screen from `hallo-tui version` |
| The README Version badge copying the same string | A version bump as a side effect of editing this file |

| Surface | What you open | What for |
|---------|---------------|----------|
| `hallo-tui version` | terminal | The installed line |
| `hallo-tui --version` | terminal | The same line |
| `src/main/cpp/project.h` | `kVersion` | The string in the binary |
| `project.ini` | `VERSION` | The same string for the build tool |

| You do… | What it means | What you type |
|---------|---------------|---------------|
| Ask the edition | The program prints the name, one space, and the string, then a newline. It does not call pip and it does not draw the menu. | `hallo-tui version` |
| Ask with the flag | The same line. The flag does not open the menu. | `hallo-tui --version` |
| Read it on the menu | Self-management row **82** shows that line on the result page. It still does not call pip. | `hallo-tui`, then **8**, then **82** |

## 2. Core Rules / Requirements (Mandatory)

1. The product version **MUST** be one string. Display code **MUST NOT** keep a second copy of that string, including a fallback literal.
2. The ship unit **MUST NOT** add integer constants named `MAJOR`, `MINOR`, or `PATCH`. A dotted string such as `1.0.0` is still one string.
3. The binary constant and the build-file constant **MUST** be equal whenever a release is claimed.
4. The root user document's Version badge **MUST** contain that same string.
5. The typed verb `version` **MUST** print the application name, one space, and the string, then a newline. It **MUST** return 0. It **MUST NOT** call pip. It **MUST NOT** draw the text screen.
6. `--version` **MUST** print that same line and return 0. It **MUST NOT** draw the text screen. It **MUST NOT** call pip.
7. `SelfManage::localVersion` **MUST** return that same line without the newline. The `version` verb **MUST** print that return value. `--version` **MUST** print the same characters from the constants. Those two paths **MUST NOT** diverge.
8. The welcome sentence, the about identity line, the about stamp, the about star-box title, and the menu status line **MUST** use this string. Those pages stay on their own files. They **MUST NOT** keep a private version literal.
9. Menu row **82** **MUST** show `localVersion` and **MUST NOT** call pip. Choosing that row **MUST NOT** be what `hallo-tui version` does.
10. `version-check` **MUST** start its text with `localVersion` and then the pip sentence. The pip command stays on `requirement-python-cli-interface` and `requirement-cpp-tui`. This file owns only the prefix line.
11. A date stamp on the about page **MUST NOT** be treated as the version string.
12. This file **MUST NOT** require a logger line in order to print the version. The console verbs write the line themselves until a logger unit exists.
13. `version` is an operational verb. It is not a test-purpose verb. It takes no operands.

### 2.1 Implementation Notes (this project)

| Item | Value |
|------|--------|
| **Display name** | HalloTui |
| **Console command** | `hallo-tui` |
| **Current string** | `1.0.0` |
| **Binary constant** | `kVersion` in `src/main/cpp/project.h` |
| **Build-file constant** | `VERSION` in `project.ini` |
| **Method** | `SelfManage::localVersion` in `src/main/cpp/self_manage.cpp` |
| **Flag and verb** | `Cli::run` in `src/main/cpp/cli.cpp` |
| **Badge** | root `README.md` shields.io Version badge |
| **Welcome** | `Welcome to HalloTui 1.0.0.` built by `Tui::helloText` |
| **Row 82** | result text `HalloTui 1.0.0` |
| **Stamp** | `kLastUpdate` is `2026-10-04`. It is not the version |
| **Homepage constant** | `https://github.com/cloudgen/HalloTui` in `project.h` `kHomepage`. A public-home order sets this string. This file does not by itself retarget the git remote |
| **Workspace** | the RAM-drive tree for this basename when that tree exists, otherwise the hard-disk checkout. Do not record a login name |

Both commands print:

```text
HalloTui 1.0.0
```

```text
hallo-tui version
hallo-tui --version
```

The identity header in this checkout:

```cpp
constexpr const char *kAppName = "HalloTui";
constexpr const char *kVersion = "1.0.0";
constexpr const char *kConsoleName = "hallo-tui";
```

The method:

```cpp
std::string SelfManage::localVersion() const { return appName_ + " " + version_; }
```

The two console paths:

```cpp
if (args.version) {
    std::cout << kAppName << " " << kVersion << "\n";
    return 0;
}
if (args.verb == "version") {
    std::cout << selfManage.localVersion() << "\n";
    return 0;
}
```

`project.ini` holds `VERSION=1.0.0` on its own line. `./build.sh version` reads that line. It is not a second product version.

### 2.2 Why This Requirement Exists (Direct CIAO Alignment)

- **CIAO Principle 5 – SSOT**: one string, two stores that must match, many displays that copy it.
- **CIAO Principle 2 – Intentional**: the Python program's single string is kept. An integer triple is not added on the way to C++.
- **CIAO Principle 1 – Caution**: `version` does not call the network.
- **CIAO Principle 21 – Dual Policies**: the string values sit in these notes. Reusable molds keep placeholders.

## Under command line for normal user only

On Termux, Git Bash, Windows cmd, or the same class, the person runs `hallo-tui version` as the normal user. **This requirement:** printing the version does not use administrator privilege, `sudo`, `apt`, or a dedicated system user. Do not pipe a downloaded script into a shell. Type 1 and Type 2 are unused on Termux, Git Bash, and Windows cmd. Git Bash and Windows cmd do not call Termux `pkg`.

## 3. Design Principles (CIAO / CIAO-Lite)

- **Caution**: the verb returns the local string and does not query an index.
- **Intentional**: `--version` and `version` are the same line.
- **Anti-fragile**: the build file and the header are checked against each other.
- **Over-protect**: a second literal, including a fallback, is forbidden.
- **Principle 16**: the commands do not wait for a key.

## 4. Protection Rule (Sacred)

**Future editors MUST NOT**:

1. Split this string into `MAJOR`, `MINOR`, and `PATCH` integers.
2. Put a second version literal in `cli.cpp`, the about page, or the welcome line.
3. Make `hallo-tui version` or `hallo-tui --version` open the text screen.
4. Make row **82** call pip.
5. Bump `1.0.0` because this file or a glossary section changed.
6. Treat `kLastUpdate` as the version.
7. Restore a Python package so this string can live in `__version__` again.
8. Change the git remote from this file.

## 5. Related artifacts (versioned surface only)

| Artifact | Role |
|----------|------|
| `docs/requirements/index.md` | Registry |
| `docs/requirements/requirement-python-cli-interface.md` | Names the verb and the flag |
| `docs/requirements/requirement-cpp-tui.md` | Row **82** and the status line |
| `docs/requirements/requirement-domain-videojoin.md` | Welcome sentence |
| `docs/requirements/requirement-python-about.md` | Identity, stamp, star box |
| `docs/requirements/requirement-python-readme.md` | Version badge |
| `docs/requirements/requirement-cpp-folder.md` | `project.h` is in the ship unit |
| `docs/requirements/requirement-cpp-coding-style.md` | `localVersion` is lowerCamel |
| `docs/requirements/requirement-class-software-dev.md` | Residual pointer |
| `src/main/cpp/project.h` | `kVersion` |
| `src/main/cpp/cli.cpp` | Flag and verb |
| `src/main/cpp/self_manage.cpp` | `localVersion` |
| `project.ini` | `VERSION` |
| `README.md` | Version badge |

## Design-time verification

| TP-ID | Suite | Status | Note |
|-------|-------|--------|------|
| **TP-VER-01** | `tests/cpp/test_main.cpp` and `tests/test_docs.py` | have | Both commands print `HalloTui 1.0.0` and a newline. They do not open the screen. Row **82** shows the same line. `project.ini` equals `kVersion`. The header has no `MAJOR`, `MINOR`, or `PATCH` |

**Matrix:** `docs/reviews/requirement-test-matrix.md`
**Map:** `docs/reviews/test-plan.md`

## Terminologies

### Project target version

**Definition:** Project target version is the user-confirmed product version string (typically MAJOR.MINOR.PATCH, for example 1.0.0 or 1.7.0) that agents must collect from the user (or confirm from an existing product root README.md parse). After it is written into product root README.md, that file is the single source of truth for this field, displayed once in the product README header kit as the shields.io Version badge. It is not hardcoded into skills, requirements, templates, or terminology files.

**Human daily-life explanation:** Project target version is the user-confirmed version string shown once as the README Version badge. Skills do not own it.

**Daily-life example:** The edition number printed on a cookbook's cover; the inner chapters should match, but the cover is what you asked to print.

### Version align

**Definition:** Version align (also version alignment) is the evidence-based process of inventorying every in-scope product version surface, comparing them for consistency with a single current product version string, and reporting PASS, DISCREPANCY, N/A, or not probeable, including, when a PyPI upload script is present on disk, the live PyPI version from the package index. It is not a VCS commit, not a force of registry state, and not permission to invent versions.

**Human daily-life explanation:** Version align is checking that every clock in the house shows the same time as the one agreed current time, then reporting match, mismatch, not applicable, or cannot look. It does not wind the clocks for you, and it does not invent a time that was never written.

**Daily-life example:** You compare the oven clock, the hallway clock, and the posted menu date. You report "hallway is five minutes slow." You do not secretly rewrite the menu.

## 6. Status history

| Date | Status | Notes |
|------|--------|-------|
| 2026-10-22 | Active 1.0.0 | C++ owner of the single version string. Converts the sibling Python rule that lived in the package string, the version verb, and `--version`. No integer triple |
| 2026-10-22 | Active 1.0.1 | Records `kHomepage` as `https://github.com/cloudgen/HalloTui`. The version string stays `1.0.0` |

**Last Updated**: 2026-10-22
**Owner**: project maintainers
**Alignment**: Registry `docs/requirements/index.md`; **CIAO** (https://github.com/cloudgen/ciao); CIAO-Lite (https://github.com/cloudgen/ciao-lite).
