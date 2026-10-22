**file**: docs/requirements/requirement-runtime-prerequisites.md
**Status**: Active (Version 1.2.0 – no encoder. The C++ binary does not import ChronicleLogger)
**Area**: runtime
**Key**: `requirement-runtime-prerequisites`
**Philosophy**: CIAO **v2.10.2** / CIAO-Lite (Caution • Intentional • Anti-fragile • Over-engineered / Over-protect)

## 1. Purpose

Declare the host and Python runtime prerequisites required to run VideoJoin. This product does not implement a privileged installer. This file is the documentation and validation SSOT for what must already be present, together with the ChronicleLogger floor also owned by `requirement-python-packaging`.

### 1.1 Human-facing

**In one sentence:** The C++ binary needs ncurses. No encoder is required. ChronicleLogger is not imported.

| Box | Meaning | Example |
|-----|---------|---------|
| You / this login | Person about to join two videos | FFmpeg is already installed by you, not by pip |
| The other role | pip | Installs VideoJoin and ChronicleLogger. It does not install FFmpeg |
| Not this file | The menu picture and the concat command | TUI and pipeline requirements |

| Includes | Excludes |
|----------|----------|
| ncurses at link time | A claim that this binary installs FFmpeg |
| No ChronicleLogger import in the C++ ship unit | A host encoder |
| A console next step when either one is missing | Root auto-install of either one |

| Surface | What you open | What for |
|---------|---------------|----------|
| PATH | `ffmpeg` | Concat and the fallback re-encode |
| pip | ChronicleLogger | Status file |
| About page | Runtime-tools line | Names FFmpeg. Does not probe PATH (`requirement-python-about`) |

| You do… | What it means | What you type |
|---------|---------------|---------------|
| Install the Python package | You get VideoJoin and the status library. | `python -m pip install VideoJoin` |
| Open the menu | No encoder check. The menu does not call FFmpeg. | `hallo-tui` |
| Start with the library missing | The console prints the pip next step and returns non-zero. | `video-join` |

## 2. Core Rules (Mandatory)

### 2.1 Scope

1. **MUST** document every external tool required at runtime.
2. **MUST NOT** claim the product auto-installs system packages via root or sudo.
3. **MUST** separate pip-installable Python dependencies from system binaries.

### 2.2 Required runtime components

| Component | Kind | Required for | Install surface |
|-----------|------|--------------|-----------------|
| ncurses | link library | the text screen | the host. The `Makefile` links it |
| `python3` | optional host binary | rows 84–87 only, when the operator asks | not required to print `hello` or to open the menu |

FFmpeg is not a runtime requirement. ChronicleLogger is not imported by the C++ binary.

### 2.3 Validation

4. The program **MUST NOT** require `ffmpeg` on PATH to open the menu or to print `hello`.
5. The about page **MUST** say `Runtime tools: none`. It **MUST NOT** report FFmpeg on PATH.
6. **MUST NOT** document FFmpeg as something this product installs.
7. There is no media format set. `requirement-video-ffmpeg-pipeline` is Retired.
8. A missing ChronicleLogger **MUST NOT** block `hallo-tui`. The C++ ship unit does not import it.

### 2.4 Privilege

9. **MUST NOT** require root to satisfy runtime prerequisites for normal use.
10. Type 1 elevation is out of scope.

### 2.5 Implementation Notes (this project)

| Item | Value |
|------|--------|
| **Build** | `make` writes `target/exe/debug/hallo-tui` |
| **Declared pip dependency (law)** | none for the C++ binary |
| **System library** | ncurses |
| **Encoder** | none |
| **Auto install command** | none. The product does not run a root install |
| **Platform notes** | Linux with `g++` and ncurses |
| **Product version** | 1.0.0 |
| **Startup check** | none for FFmpeg |
| **User docs** | Root `README.md` |

### 2.6 Why This Requirement Exists (CIAO)

- **Principle 1 – Caution**: No encoder is required. The menu does not call FFmpeg.
- **Principle 10 – Least privilege**: No root installer.
- **Principle 2 – Intentional**: The pip library and the system binary are different rows.

## Under command line for normal user only

On Termux, Git Bash, Windows cmd, or the same class, the normal user installs the pip package and supplies FFmpeg. **This requirement:** do not use administrator privilege, `sudo`, `apt`, or a dedicated system user to install ChronicleLogger or FFmpeg from inside VideoJoin. Do not pipe a downloaded script into a shell. Type 1 and Type 2 are unused on Termux, Git Bash, and Windows cmd. Git Bash and Windows cmd do not call Termux `pkg`.

## 3. Design Principles (CIAO / CIAO-Lite)

- **Caution:** Document the missing tool before the join fails.
- **Intentional:** pip does not install FFmpeg. ChronicleLogger is required.
- **Anti-fragile:** Join works wherever `ffmpeg` is already on PATH.
- **Over-protect:** No silent root install.

## 4. Protection Rule (Sacred)

**Future AI assistants, Grok, or maintainers MUST NOT**:

1. Claim FFmpeg is installed by `pip install VideoJoin`.
2. Add a root package-manager install from this product.
3. Drop FFmpeg from the table while `join` still depends on it.
4. Store secrets in this file.
5. Add OpenCV or another unused library as a requirement.
6. Call ChronicleLogger optional, or hide the manifest lag.

**Violating this rule is a critical honesty regression.**

## 5. Acceptance criteria

| ID | Criterion |
|----|-----------|
| AC-1 | FFmpeg is listed as a system binary |
| AC-2 | ChronicleLogger is a required pip dependency at `>=1.3.1`. The live manifest lag is stated |
| AC-3 | No root auto-install claim |
| AC-4 | Missing FFmpeg on `join` produces an actionable message |
| AC-5 | Missing ChronicleLogger produces the pip next step and a non-zero return |

## 6. Related requirements (peer keys only)

| Key | Relationship |
|-----|----------------|
| `requirement-python-packaging` | Floor owner for the pip dependency |
| `requirement-python-cli-logging` | Construct. Missing-import sentence |
| `requirement-video-ffmpeg-pipeline` | Uses FFmpeg |
| `requirement-python-error-handling` | Missing-tool messages |
| `requirement-domain-videojoin` | About reports FFmpeg |
| `docs/requirements/index.md` | Registry |

## Design-time verification

| TP family / ID | Suite | Status | Note |
|----------------|-------|--------|------|
| **TP-PRE-01** | `tests/test_prerequisites.py` | todo | Host `ffmpeg`, or an actionable miss on `join` |
| **TP-PRE-02** | `tests/test_prerequisites.py` | todo | ChronicleLogger imports. Law floor is `>=1.3.1` |
| **TP-ERR-01** | `tests/test_errors.py` | todo | Peer: missing FFmpeg on `join` |

**Matrix:** `docs/reviews/requirement-test-matrix.md`
**Map:** `docs/reviews/test-plan.md`

## 7. Status history

| Date | Status | Note |
|------|--------|------|
| 2026-08-09 | Active 1.0.0 | Initial runtime prerequisites law for VideoJoin |
| 2026-10-04 | Active 1.1.0 | ChronicleLogger `>=1.3.1` is required. Live manifest still `>=1.2.3`. FFmpeg stays a system binary. The front board may open without it |
| 2026-10-04 | Active 1.1.1 | Product version **1.0.4**. Manifest floor is `ChronicleLogger>=1.3.1` |
| 2026-10-04 | Active 1.1.2 | The about page names FFmpeg and does not probe PATH. Product version **1.0.5** |

---

**Last Updated**: 2026-10-04
**Owner**: project maintainers
**Alignment**: Registry `docs/requirements/index.md`; **CIAO** (https://github.com/cloudgen/ciao); CIAO-Lite (https://github.com/cloudgen/ciao-lite).
