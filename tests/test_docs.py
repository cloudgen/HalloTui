# TP-DOC-01 — user document structure. Primary law: requirement-python-readme.
# The badge matches kVersion and project.ini VERSION.
# TP-VER-01 — those two strings match, and project.h has no integer triple.
# TP-DOC-03 (catalog paragraph equality) is not this file yet.
import re
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
README = (ROOT / "README.md").read_text(encoding="utf-8")
IMAGE_BASE = "https://raw.githubusercontent.com/cloudgen/HalloTui/main/screenshots/"
RELATED = (
    "https://github.com/cloudgen/HalloTui",
    "https://pypi.org/project/HalloTui/",
    "https://github.com/Wilgat/AnimeDlp",
    "https://github.com/Wilgat/ChronicleLogger",
    "https://github.com/Wilgat/VideoSpeed",
    "https://github.com/cloudgen/ciao",
    "https://github.com/cloudgen/ciao-lite",
    "https://github.com/cloudgen/safe-rm",
)
HEADINGS = (
    "Features",
    "Advantages",
    "Quick Installation",
    "Usage",
    "Screenshots",
    "Examples",
    "Platform Compatibility",
    "Related Projects",
    "Contributing",
    "License",
    "Last Update",
)


def _package_version():
    project_ini = (ROOT / "project.ini").read_text(encoding="utf-8")
    header = (ROOT / "src" / "main" / "cpp" / "project.h").read_text(encoding="utf-8")
    ini_match = re.search(r"(?m)^VERSION=(\S+)", project_ini)
    header_match = re.search(r'kVersion = "([^"]+)"', header)
    return ini_match.group(1), header_match.group(1)


class TestReadmeStructure(unittest.TestCase):
    """TP-DOC-01."""

    def test_tp_doc_01_badge_links_headings_and_related_projects(self):
        """TP-DOC-01: badge, absolute picture URLs, headings, clock, no setup.sh."""
        py_version, init_version = _package_version()
        self.assertEqual(py_version, init_version)
        self.assertIn("Version-" + py_version + "-blue", README)
        self.assertIn("local clock", README)
        self.assertIn("HH:MM:SS", README)
        self.assertNotIn("setup.sh", README)
        self.assertNotIn("sudo pip", README)
        self.assertNotIn("curl | sh", README)
        headings = re.findall(r"(?m)^## ([^#\n].*)$", README)
        self.assertEqual(tuple(headings), HEADINGS)
        names = sorted(path.name for path in (ROOT / "screenshots").glob("*.png"))
        self.assertGreater(len(names), 0)
        for name in names:
            self.assertIn(IMAGE_BASE + name, README, name)
        self.assertIsNone(re.search(r"!\[[^\]]*\]\(\.?/?screenshots/", README))
        related = README.split("## Related Projects", 1)[1].split("## Contributing", 1)[0]
        urls = re.findall(r"\((https://[^)]+)\)", related)
        self.assertEqual(tuple(urls), RELATED)
        video = README.split("### `video.png`", 1)[1]
        if "\n### " in video:
            video = video.split("\n### ", 1)[0]
        self.assertIn("not a text-menu capture", video)

    def test_tp_ver_01_single_string_pair(self):
        """TP-VER-01: project.ini VERSION equals kVersion. No MAJOR, MINOR, or PATCH."""
        ini_version, header_version = _package_version()
        self.assertEqual(ini_version, header_version)
        header = (ROOT / "src" / "main" / "cpp" / "project.h").read_text(encoding="utf-8")
        for name in ("MAJOR", "MINOR", "PATCH"):
            self.assertNotIn(name, header)


if __name__ == "__main__":
    unittest.main()
