#!/bin/sh
# HalloTui test runner. The menu proofs are the C++ binary. TP-DOC-01 reads the README.
set -eu
ROOT="$(CDPATH= cd -- "$(dirname "$0")/.." && pwd)"
cd "$ROOT"
make test
python3 -m unittest tests.test_docs -v
