**file**: docs/requirements/requirement-cpp-coding-style.md
**Status**: Active (Version 1.0.1)
**Area**: cpp
**Key**: `requirement-cpp-coding-style`
**Philosophy**: CIAO / CIAO-Lite

## 1. Purpose

This file owns C++ naming and linkage for HalloTui. The directory list stays in `requirement-cpp-folder`.

**In one sentence:** Types are UpperCamel, methods are lowerCamel, and headers stay `.h`.

## 2. Core Rules (Mandatory)

1. The primary language **MUST** be C++17, compiled with `g++`.
2. A type, class, struct, or enum **MUST** be UpperCamel. A method or a local **MUST** be lowerCamel.
3. An `extern "C"` function **MUST** be snake_case. This ship unit does not yet have a logger translation unit. When one is added, its functions **MUST** be `logger_info`, `logger_warn`, `logger_error`, `logger_fatal`, `logger_debug`, `logger_log`, `logger_output_init`, `logger_set_quiet`, `logger_get_quiet`, and `logger_get_debug`, with level tokens `LOGGER_LEVEL_*`. There **MUST NOT** be `logger_set_debug`.
4. Until that logger unit exists, console verbs **MAY** write with the C++ stream library. That is a recorded deviation from the logger mold. The text-screen path **MUST NOT** print those status lines while the menu is open. Typed `hello` **MUST** print only the welcome line and a newline.
5. Do not add GLib or GTK types. Do not rename a header in `requirement-cpp-folder` to `.hpp`.
6. Display columns **MUST NOT** use `wcwidth`. Wide and Fullwidth count as two. Every other width, including Ambiguous, counts as one.
7. `localVersion` **MUST** be lowerCamel. It returns the application name, one space, and the version string. That string is `requirement-cpp-version`. This file **MUST NOT** keep a second version literal.

## 3. Protection Rule

Future editors **MUST NOT** put a hyphen in a C++ identifier, and **MUST NOT** treat this file as the menu picture.

## Design-time verification

| TP-ID | Suite | Status | Note |
|-------|-------|--------|------|
| n/a | naming is reviewed with the ship unit | n/a | No separate style executable. Column rule is TP-TUI-10 |

## Terminologies

### Project target version

**Definition:** Project target version is the user-confirmed product version string (typically MAJOR.MINOR.PATCH, for example 1.0.0 or 1.7.0) that agents must collect from the user (or confirm from an existing product root README.md parse). After it is written into product root README.md, that file is the single source of truth for this field, displayed once in the product README header kit as the shields.io Version badge. It is not hardcoded into skills, requirements, templates, or terminology files.

**Human daily-life explanation:** Project target version is the user-confirmed version string shown once as the README Version badge. Skills do not own it.

**Daily-life example:** The edition number printed on a cookbook's cover; the inner chapters should match, but the cover is what you asked to print.

## Status history

| Date | Status | Notes |
|------|--------|-------|
| 2026-10-22 | Active 1.0.0 | C++17 naming. Logger unit not present |
| 2026-10-22 | Active 1.0.1 | `localVersion` is lowerCamel and points at `requirement-cpp-version` |

**Last Updated**: 2026-10-22
