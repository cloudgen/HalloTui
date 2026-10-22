**file**: docs/requirements/requirement-class-software-dev.md
**Status**: Active (Version 1.2.2 – public home is `https://github.com/cloudgen/HalloTui`)
**Area**: class
**Key**: `requirement-class-software-dev`
**Philosophy**: CIAO **v2.10.2** / CIAO-Lite (Caution • Intentional • Anti-fragile • Over-engineered / Over-protect)

## 1. Purpose

Declare this workspace as a software-development project class and hold the residual collection of software-engineering stack facts not already owned by more specific Active peer requirements: primary language, toolchain policy, package and build tooling, and runtime OS family.

This file is class law plus the residual SSOT. It is not a second copy of domain join features, FFmpeg concat ops, the CLI surface, the text menu, the logger construct, the class map, packaging tables, or error-handling tables. Those stay on peer requirements.

### 1.1 Human-facing

**In one sentence:** This file records that HalloTui is C++, built with g++ and GNU Make, and it points at the requirement that owns each other topic.

| Box | Meaning | Example |
|-----|---------|---------|
| You / this login | Maintainer checking which file owns a topic | The menu picture is `requirement-cpp-tui` |
| The other role | Peer requirements | CLI, TUI, domain, about |
| Not this file | The frame glyphs and the pip command strings | Their own requirements. This file only points |

| Includes | Excludes |
|----------|----------|
| Language, interpreter, package tool, and the residual pointer table | A second copy of the menu, the logger construct, or the class map |
| Shell install and root elevation marked absent | A pip lifecycle marked absent. That lifecycle is owned by the CLI and the TUI |
| One human operator. No dest approver | A new actor-requirement file |

| Surface | What you open | What for |
|---------|---------------|----------|
| This file | Residual table | Which requirement owns the topic |
| `project.ini` and `Makefile` | Build facts | The binary path. The folder requirement owns the source list |
| `hallo-tui` | The program | You run it as the normal user |

| You do… | What it means | What you type |
|---------|---------------|---------------|
| Find the menu law | Open `requirement-cpp-tui`. Do not expect the picture in this file. | `hallo-tui` |
| Build | GNU Make. There is no shell installer. | `make` |
| Ask for the version | The binary prints the local version. It does not call pip. | `hallo-tui version` |

## 2. Core Rules (Mandatory)

### 2.0 Project class membership

1. **MUST** treat this workspace as software-development (shippable software), not genesis-template and not server-maintenance.
2. **MUST** use basename `requirement-class-software-dev.md` as the sole Active class-law file for this class.
3. **MUST NOT** register an Active `requirement-class-server-maintenance.md` while the class is software-development.
4. **MUST** retain portable harness knowledge when present. Specialized product knowledge lives in this and peer `requirement-*.md` files.
5. **MUST** apply the software-development SSOT and gate posture when claimed (identity, package ship surface, and a pre-commit check when git is used — as applicable).
6. **MUST NOT** invent hollow product docs solely to look specialized.

### 2.1 Residual collection principle (SSOT hygiene)

7. **MUST** treat this file as the default home for software-stack facts not owned by another Active requirement.
8. **MUST NOT** duplicate full normative tables that already live in a more specific Active requirement. Prefer a one-line pointer to the peer requirement key.
9. When a new specialized requirement takes ownership of a topic previously only listed here, **MUST** update this file in the same change: shrink the residual entry and point to the new owner.
10. **MUST NOT** leave contradictory stack facts across this file and peer requirements.

### 2.2 Programming language(s)

11. **MUST** declare at least one primary programming language for the ship unit.
12. **SHOULD** list secondary languages only when they are real product law.
13. **MUST** state whether the product is primarily interpreted, compiled, polyglot, or package-multi-language.
14. **MUST NOT** freeze a marketing product name as if it were the language name.

### 2.3 Compilers, interpreters, and toolchains

15. **MUST** declare the target toolchain class used to build or run the product.
16. **MUST** state version policy as one of: unconstrained, minimum version, range, or pinned.
17. **SHOULD** record whether cross-compilation is in scope.
18. **MUST** fail closed in docs claims: do not claim “supports all interpreters” without tests or an explicit unconstrained policy.

### 2.4 Project / package / build tools

19. **MUST** declare the primary project or package tool used for dependencies and builds.
20. **MUST** declare how dependencies are resolved when the ecosystem supports lockfiles.
21. **SHOULD** name the test runner and linter or formatter classes when they are project law.
22. **MUST NOT** require a secret token or a private registry password in this file.

### 2.5 Runtime and platform (residual)

23. **MUST** declare the intended primary runtime and OS family when not fully owned by another architecture requirement.
24. **SHOULD** declare minimum CPU or architecture support only when it is real product law.
25. **MUST** separate developer-machine toolchain requirements from end-user runtime requirements when they differ.

### 2.6 No-hardcode / dual policy (class file)

26. **MUST NOT** hard-code a single product brand, one organization’s production hostname, or a personal owner identity as universal core law.
27. **MUST** put the live product name, the repo slug, and the concrete stack choices in Implementation Notes.
28. **MUST NOT** store secrets, PATs, or toy credentials in this file.

### 2.7 Implementation Notes (this project)

| Field | Value (HalloTui) |
|-------|---------------------|
| **Project display name** | HalloTui |
| **Project class** | software-development |
| **Class requirement basename** | `requirement-class-software-dev.md` |
| **Primary language(s)** | C++ |
| **Language role** | primary for the ship unit under `src/main/cpp` |
| **Execution model** | compiled binary |
| **Toolchain / interpreter** | `g++ -std=c++17` |
| **Toolchain version policy** | C++17. Re-verify before advertising a newer standard |
| **Cross-compile in scope?** | no |
| **Primary project/package tool** | GNU Make. One `Makefile`. No Autotools |
| **Lockfile policy** | not used as product law |
| **Test runner** | `make test` runs `target/exe/debug/hallo-tui-tests`. `tests/run.sh` also runs the README proof |
| **Linter/formatter** | none as project law. The build uses `-Wall -Wextra -Wpedantic` |
| **Primary runtime / OS family** | Linux with ncurses. No FFmpeg |
| **Architectures supported** | the architecture this `g++` targets |
| **Git surface** | used — remote `https://github.com/cloudgen/HalloTui` |
| **Ship surface** | `target/exe/debug/hallo-tui`. Console command `hallo-tui`. Not `python -m HalloTui` |
| **Product version SSOT** | owned by `requirement-cpp-version`. `src/main/cpp/project.h` `kVersion` and `project.ini` `VERSION` (current: **1.0.0**) |
| **Install mode** | GNU Make. Not a shell online-install product. Not a pip ship unit |
| **Type 1 elevation** | intentionally absent. No root or sudo product surface |
| **Actor / role / subject / approver** | **Considered — no dest approver.** The human operator of `hallo-tui` is the only actor. No dest approval machine. **None** is valid. Do not add an actor requirement file |
| **Dest fence conditions** | **Considered — no dest fence conditions.** No dest inbound, approve, or reject queue |
| **Author contact (non-secret)** | Wilgat Wong · `wilgat.wong@gmail.com` |

**Residual ownership table:**

| Topic | Owner | Notes |
|-------|-------|--------|
| Project class membership | **this file** | Fixed |
| Primary language + toolchain policy | **this file** | C++ / `g++` |
| Build tool | **this file** + `requirement-cpp-folder` | GNU Make. `./build.sh` is not the ship command |
| Project layout | `requirement-cpp-folder` | `src/main/cpp` |
| Naming | `requirement-cpp-coding-style` | UpperCamel types, lowerCamel methods |
| Product version string | `requirement-cpp-version` | `kVersion` and `project.ini` `VERSION` stay equal. This row records the current string |
| CLI entry / typed verbs | `requirement-python-cli-interface` | Empty argv and the verb table |
| Text menu | `requirement-cpp-tui` | Front board and the picture |
| Menu language | `requirement-python-cli-language` | Front row 4, the thirteen codes, and the language leaf |
| Status logger | `requirement-cpp-coding-style` | No `logger_*` unit yet. ChronicleLogger is not constructed |
| Domain surface | `requirement-domain-videojoin` | Welcome line. The about page uses the domain sentence |
| About page | `requirement-python-about` | Identity, host check, and the star box |
| FFmpeg concat / re-encode | `requirement-video-ffmpeg-pipeline` | Retired |
| Error / fail-closed user messaging | `requirement-python-error-handling` | Console sentences |
| Host runtime deps | `requirement-runtime-prerequisites` | ncurses. No encoder |
| Shell online install / shell self-update / Type O | **intentionally absent** | Not a shell channel |
| Pip lifecycle (`version-check`, `self-update`, `self-install`, `self-uninstall`) | `requirement-python-cli-interface` and `requirement-cpp-tui` | Menu sentences say `python -m pip`. Not a shell channel |
| Type 1 sudoers / root elevation | **intentionally absent** | No elevation law |
| Actor / role / subject / approver | **this file (residual)** | Considered — no dest approver |
| Dest fence conditions | **this file (residual)** | Considered — no dest fence conditions |

## 3. Why This Requirement Exists (Direct CIAO Alignment)

- **CIAO Principle 2 – Intentional**: Class and stack choices are explicit.
- **CIAO Principle 5 – SSOT**: Residual facts stay here until a peer takes the topic. The menu, the logger, and the class map have already moved.
- **CIAO Principle 1 – Caution**: This file does not invent a shell installer.
- **CIAO Principle 21 – Dual Policies**: Pointers stay short. Concrete values sit in Implementation Notes.
- **CIAO Principle 4 (O) + Principle 20**: One class file. No second stack SSOT.

## 4. Design Principles (CIAO / CIAO-Lite)

- **Caution**: FFmpeg is a host tool. This file does not install it.
- **Intentional**: The residual table points. It does not copy peer bodies.
- **Anti-fragile**: Packaging identity stays in `pyproject.toml`.
- **Over-protect**: Shell Type O stays absent while pip lifecycle is claimed on the CLI and the TUI.

## 5. Protection Rule (Sacred)

**Future AI assistants, Grok, or maintainers MUST NOT**:

1. Delete this file while the workspace remains software-development with other Active product requirements.
2. Rename the specialized basename away from `requirement-class-software-dev.md` without an explicit class-model change.
3. Hard-code secrets, personal tokens, or production host names into core rules as universal law.
4. Duplicate full peer requirement bodies into this residual section.
5. Leave Implementation Notes hollow while Status is Active.
6. Introduce a shell online-install channel, a `curl|sh` installer, or shell Type O empty-argv install-ensure. Pip lifecycle owned by the CLI and TUI requirements is not that channel.
7. Introduce Type 1 sudoers or root elevation without an explicit user order and an elevation allowlist.
8. Treat this file as server-maintenance allowlist law, or register an Active server-maintenance class file in parallel.
9. Invent a second primary language that contradicts the C++ ship unit.
10. Add an actor-requirement file while the residual says no dest approver.
11. Mark pip lifecycle absent, or mark shell Type O present.

**Violating any of these is a critical regression.**

## 6. Acceptance criteria

| ID | Criterion |
|----|-----------|
| AC-1 | The sole Active class file is `requirement-class-software-dev.md` |
| AC-2 | Primary language is C++ with `g++ -std=c++17` |
| AC-3 | The build tool is GNU Make. The binary is `target/exe/debug/hallo-tui` |
| AC-4 | The residual table points at the CLI, TUI, logging, OOP, and domain peers without copying their bodies |
| AC-5 | Shell Type O and Type 1 elevation are absent. Pip lifecycle is owned by the CLI and the TUI |
| AC-6 | Registered in `docs/requirements/index.md` with Area `class` |
| AC-7 | Actor residual stays “no dest approver”. No actor requirement file |

## 7. Related requirements (peer keys only)

| Key | Relationship |
|-----|----------------|
| `requirement-python-packaging` | Manifest, entry point, logger floor |
| `requirement-python-project-structure` | Layout |
| `requirement-python-oop` | Class map |
| `requirement-python-cli-interface` | Typed verbs and empty argv |
| `requirement-cpp-tui` | Text menu |
| `requirement-cpp-folder` | Ship-unit directory and `make` |
| `requirement-cpp-coding-style` | Naming |
| `requirement-python-cli-logging` | Logger construct |
| `requirement-domain-videojoin` | Domain four pillars |
| `requirement-video-ffmpeg-pipeline` | Processing ops |
| `requirement-python-coding-style` | Temps and `shutil.move` |
| `requirement-python-error-handling` | Console errors |
| `requirement-runtime-prerequisites` | Host tools and the logger floor |
| `docs/requirements/index.md` | Registry |

## Design-time verification

| TP family / ID | Suite | Status | Note |
|----------------|-------|--------|------|
| n/a | packaging and structure smoke | deferred | Class residual is exercised by TP-PKG, TP-STRUCT, TP-LOG, TP-TUI, and TP-OOP when those rows leave todo |

**Matrix:** `docs/reviews/requirement-test-matrix.md`
**Map:** `docs/reviews/test-plan.md`

## Under command line for normal user only

On Termux, Git Bash, Windows cmd, or the same class, VideoJoin runs as the normal user. **This requirement:** do not use administrator privilege, `sudo`, `apt`, or a dedicated system user. Do not pipe a downloaded script into a shell. Type 1 and Type 2 are unused on Termux, Git Bash, and Windows cmd. Git Bash and Windows cmd do not call Termux `pkg`. Pip lifecycle, when the operator asks for it, is `python -m pip` from the CLI and TUI requirements.

## Terminologies

### Project target version

**Definition:** Project target version is the user-confirmed product version string (typically MAJOR.MINOR.PATCH, for example 1.0.0 or 1.7.0) that agents must collect from the user (or confirm from an existing product root README.md parse). After it is written into product root README.md, that file is the single source of truth for this field, displayed once in the product README header kit as the shields.io Version badge. It is not hardcoded into skills, requirements, templates, or terminology files.

**Human daily-life explanation:** Project target version is the user-confirmed version string shown once as the README Version badge. Skills do not own it.

**Daily-life example:** The edition number printed on a cookbook's cover; the inner chapters should match, but the cover is what you asked to print.

## 8. Status history

| Date | Status | Note |
|------|--------|------|
| 2026-08-09 | Active 1.0.0 | Initial software-dev class law for VideoJoin |
| 2026-10-04 | Active 1.1.0 | Residual pointers for the text menu, the logger, the class map, and pip lifecycle. Shell Type O stays absent |
| 2026-10-04 | Active 1.1.1 | Menu language points at `requirement-python-cli-language`. No new actor file |
| 2026-10-04 | Active 1.1.2 | Residual row for the about page. Product version **1.0.5** |
| 2026-10-22 | Active 1.2.0 | Ship unit is C++ / g++ / GNU Make. Product version **1.0.0**. Menu owner is `requirement-cpp-tui` |
| 2026-10-22 | Active 1.2.1 | The version string is owned by `requirement-cpp-version`. This file points at it |
| 2026-10-22 | Active 1.2.2 | Public home is `https://github.com/cloudgen/HalloTui` |

---

**Last Updated**: 2026-10-22
**Owner**: project maintainers
**Alignment**: Registry `docs/requirements/index.md`; **CIAO** (https://github.com/cloudgen/ciao); CIAO-Lite (https://github.com/cloudgen/ciao-lite).
