/* C++ proofs for the text menu that the Python suite already covers.
   TP-TUI-09 path label, TP-TUI-10 wide columns, TP-LANG-01, hello, and TP-VER-01. */

#include "cli.h"
#include "menu_model.h"
#include "menu_painter.h"
#include "menu_session.h"
#include "project.h"
#include "self_manage.h"
#include "text_width.h"
#include "tui.h"

#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <sys/stat.h>
#include <unistd.h>
#include <vector>

namespace {

int gFailures = 0;

void fail(const char *file, int line, const std::string &message) {
    std::cerr << "FAIL " << file << ":" << line << ": " << message << "\n";
    ++gFailures;
}

template <typename A, typename B>
void checkEq(const char *file, int line, const char *expr, const A &got, const B &want) {
    if (got == want) return;
    std::ostringstream out;
    out << expr << "\n  got:  " << got << "\n  want: " << want;
    fail(file, line, out.str());
}

void checkTrue(const char *file, int line, const char *expr, bool value) {
    if (!value) fail(file, line, expr);
}

#define CHECK_EQ(got, want) checkEq(__FILE__, __LINE__, #got " == " #want, (got), (want))
#define CHECK(cond) checkTrue(__FILE__, __LINE__, #cond, static_cast<bool>(cond))

class EnvGuard {
public:
    EnvGuard(const char *name, const char *value) : name_(name) {
        const char *current = std::getenv(name);
        if (current != nullptr) {
            had_ = true;
            old_ = current;
        }
        if (value == nullptr) ::unsetenv(name);
        else ::setenv(name, value, 1);
    }
    ~EnvGuard() {
        if (had_) ::setenv(name_.c_str(), old_.c_str(), 1);
        else ::unsetenv(name_.c_str());
    }

private:
    std::string name_;
    bool had_ = false;
    std::string old_;
};

class CwdGuard {
public:
    explicit CwdGuard(const std::string &next) {
        char buffer[4096];
        if (::getcwd(buffer, sizeof buffer) != nullptr) old_ = buffer;
        ::chdir(next.c_str());
    }
    ~CwdGuard() {
        if (!old_.empty()) ::chdir(old_.c_str());
    }

private:
    std::string old_;
};

std::string makeTemp(const char *prefix) {
    std::string pattern = std::string("/tmp/") + prefix + "XXXXXX";
    std::vector<char> buffer(pattern.begin(), pattern.end());
    buffer.push_back('\0');
    if (::mkdtemp(buffer.data()) == nullptr) {
        std::perror("mkdtemp");
        std::exit(2);
    }
    return buffer.data();
}

class StreamCapture {
public:
    StreamCapture(int fd, const std::string &path) : fd_(fd), path_(path) {
        saved_ = ::dup(fd_);
        const int out = ::open(path.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0600);
        ::dup2(out, fd_);
        ::close(out);
    }
    ~StreamCapture() { restore(); }
    std::string stop() {
        if (fd_ == STDOUT_FILENO) std::cout.flush();
        if (fd_ == STDERR_FILENO) std::cerr.flush();
        std::fflush(nullptr);
        restore();
        std::ifstream input(path_);
        std::stringstream buffer;
        buffer << input.rdbuf();
        return buffer.str();
    }

private:
    void restore() {
        if (saved_ < 0) return;
        ::dup2(saved_, fd_);
        ::close(saved_);
        saved_ = -1;
    }
    int fd_;
    std::string path_;
    int saved_ = -1;
};

std::string commit(hallo::MenuModel &model, const std::string &token) {
    model.buffer = token;
    model.cursor = static_cast<int>(token.size());
    model.focus = "input";
    return model.applyKey(10);
}

int modeOf(const std::string &path) {
    struct stat info;
    if (::stat(path.c_str(), &info) != 0) return -1;
    return static_cast<int>(info.st_mode & 0777);
}

bool existsPath(const std::string &path) {
    struct stat info;
    return ::stat(path.c_str(), &info) == 0;
}

std::string readFile(const std::string &path) {
    std::ifstream input(path);
    std::stringstream buffer;
    buffer << input.rdbuf();
    return buffer.str();
}

class RecordingScreen : public hallo::Screen {
public:
    struct Call {
        int y;
        int x;
        std::string text;
    };
    explicit RecordingScreen(int height = 24, int width = 80) : height_(height), width_(width) {}
    std::pair<int, int> getmaxyx() const override { return {height_, width_}; }
    void erase() override { calls.clear(); }
    void addstr(int y, int x, const std::string &text, int) override { calls.push_back({y, x, text}); }
    void refresh() override {}
    void cursSet(int) override {}
    void moveCursor(int, int) override {}
    bool hasTimeout() const override { return true; }
    void timeoutMs(int) override {}
    int getch() override { return -1; }
    void keypad(bool) override {}
    std::vector<Call> calls;

private:
    int height_;
    int width_;
};

int utf8Length(unsigned char lead) {
    if (lead < 0x80) return 1;
    if ((lead & 0xE0) == 0xC0) return 2;
    if ((lead & 0xF0) == 0xE0) return 3;
    if ((lead & 0xF8) == 0xF0) return 4;
    return 1;
}

void occupy(const std::vector<RecordingScreen::Call> &calls) {
    std::map<int, std::map<int, std::string>> rows;
    for (const RecordingScreen::Call &call : calls) {
        int column = call.x;
        for (std::size_t index = 0; index < call.text.size();) {
            int length = utf8Length(static_cast<unsigned char>(call.text[index]));
            if (index + static_cast<std::size_t>(length) > call.text.size()) length = 1;
            const std::string piece = call.text.substr(index, static_cast<std::size_t>(length));
            int width = hallo::displayWidth(piece);
            if (width < 1) width = 1;
            for (int offset = 0; offset < width; ++offset) {
                const int pos = column + offset;
                auto &row = rows[call.y];
                if (row.count(pos) != 0) {
                    std::ostringstream out;
                    out << "row " << call.y << " column " << pos << " already holds '" << row[pos]
                        << "' and '" << piece << "'";
                    fail(__FILE__, __LINE__, out.str());
                    return;
                }
                row[pos] = offset == 0 ? piece : "";
            }
            column += width;
            index += static_cast<std::size_t>(length);
        }
    }
}

int gapBefore(const std::string &line, const std::string &clock) {
    const std::string head = line.substr(0, line.size() - clock.size());
    const auto end = head.find_last_not_of(' ');
    if (end == std::string::npos) return static_cast<int>(head.size());
    return static_cast<int>(head.size() - end - 1);
}

void testHello() {
    CHECK(!hallo::Cli::opensTextScreen({"hello"}, true));
    CHECK(hallo::Cli::opensTextScreen({}, true));
    CHECK(!hallo::Cli::opensTextScreen({"hello"}, true));
    CHECK(hallo::Cli::opensTextScreen({"about"}, true));
    CHECK(!hallo::Cli::opensTextScreen({"join"}, true));
    CHECK(!hallo::Cli::opensTextScreen({"list-videos"}, true));
    CHECK(!hallo::Cli::opensTextScreen({}, false));

    hallo::Cli app;
    const std::string welcome = std::string("Welcome to ") + hallo::kAppName + " " + hallo::kVersion + ".\n";
    {
        StreamCapture out(STDOUT_FILENO, "/tmp/hallo-test-hello-out.txt");
        const int code = app.run({"hello"});
        CHECK_EQ(code, 0);
        CHECK_EQ(out.stop(), welcome);
    }
    for (const char *verb : {"join", "list-videos"}) {
        StreamCapture err(STDERR_FILENO, "/tmp/hallo-test-hello-err.txt");
        const int code = app.run({verb});
        const std::string text = err.stop();
        CHECK_EQ(code, 1);
        CHECK(text.find(std::string("Unknown verb '") + verb + "'") != std::string::npos);
    }
    {
        StreamCapture out(STDOUT_FILENO, "/tmp/hallo-test-hello-help.txt");
        CHECK_EQ(app.run({"help"}), 0);
        const std::string text = out.stop();
        CHECK(text.find("list-videos") == std::string::npos);
        CHECK(text.find("FFmpeg") == std::string::npos);
        CHECK(text.find("ffmpeg") == std::string::npos);
        CHECK(text.find("concatenate") == std::string::npos);
        CHECK(text.find("python -m pip index versions HalloTui") != std::string::npos);
        CHECK(text.find("python3") == std::string::npos);
    }

    const std::string home = makeTemp("vjhello_");
    EnvGuard lang("VIDEOJOIN_LANG", nullptr);
    hallo::MenuPainter painter(home);
    CHECK_EQ(painter.language().code(), std::string("en"));
    const std::vector<hallo::MenuRow> rows = painter.displayRows("front");
    CHECK_EQ(rows.at(0).number, 1);
    CHECK_EQ(rows.at(0).shortText, std::string("hello"));
    CHECK_EQ(rows.at(0).explain, std::string("show a welcome message"));
    CHECK_EQ(rows.at(0).kind, std::string("hello"));
    CHECK_EQ(static_cast<int>(rows.size()), 5);
    CHECK_EQ(rows.at(1).number, 3);
    CHECK_EQ(rows.at(2).number, 4);
    CHECK_EQ(rows.at(3).number, 8);
    CHECK_EQ(rows.at(4).number, 9);

    hallo::Tui tui(hallo::kAppName, hallo::kVersion, nullptr, nullptr, "");
    hallo::MenuSession session(
        hallo::kAppName, hallo::kVersion, &painter, []() { return std::string(); },
        []() { return std::string(); },
        [&](const std::string &kind) { return kind == "hello" ? tui.helloText() : std::string(); });
    CHECK_EQ(session.takeAction("hello"), std::string("stay"));
    CHECK_EQ(session.model.phase, std::string("result"));
    const std::string line = std::string("Welcome to ") + hallo::kAppName + " " + hallo::kVersion + ".";
    CHECK_EQ(session.model.resultText, line);
    CHECK_EQ(tui.helloText(), line);
}

void testVersion() {
    /* TP-VER-01: version and --version print the one string and do not open the screen. */
    const std::string line = std::string(hallo::kAppName) + " " + hallo::kVersion;
    hallo::SelfManage manage(hallo::kAppName, hallo::kVersion);
    CHECK_EQ(manage.localVersion(), line);
    CHECK(!hallo::Cli::opensTextScreen({"version"}, true));
    CHECK(!hallo::Cli::opensTextScreen({"--version"}, true));
    hallo::Cli app;
    {
        StreamCapture out(STDOUT_FILENO, "/tmp/hallo-test-version-verb.txt");
        CHECK_EQ(app.run({"version"}), 0);
        CHECK_EQ(out.stop(), line + "\n");
    }
    {
        StreamCapture out(STDOUT_FILENO, "/tmp/hallo-test-version-flag.txt");
        CHECK_EQ(app.run({"--version"}), 0);
        CHECK_EQ(out.stop(), line + "\n");
    }
    const std::string home = makeTemp("vjver_");
    hallo::MenuPainter painter(home);
    hallo::MenuSession session(
        hallo::kAppName, hallo::kVersion, &painter, [&]() { return manage.localVersion(); },
        []() { return std::string(); }, [](const std::string &) { return std::string(); });
    CHECK_EQ(session.takeAction("version"), std::string("stay"));
    CHECK_EQ(session.model.phase, std::string("result"));
    CHECK_EQ(session.model.resultText, line);
}

void testPathLabel() {
    CHECK_EQ(std::string(hallo::MenuPainter::kPathLabelEn), std::string("Path"));
    CHECK_EQ(std::string(hallo::MenuPainter::kPathLabelZhHant), std::string("路徑"));
    const std::string folder = makeTemp("vjclips_");
    const std::string home = makeTemp("vjhome_");
    EnvGuard langEnv("LANG", "zh_TW.UTF-8");
    EnvGuard language("LANGUAGE", "zh_TW:zh");
    EnvGuard lcAll("LC_ALL", "zh_TW.UTF-8");
    EnvGuard lcMessages("LC_MESSAGES", "zh_TW.UTF-8");
    EnvGuard homeEnv("HOME", home.c_str());
    EnvGuard videojoin("VIDEOJOIN_LANG", nullptr);
    CwdGuard cwd(folder);
    hallo::MenuPainter painter(home);
    CHECK_EQ(painter.pathLabel(), std::string("Path"));
    char currentBuffer[4096];
    CHECK(::getcwd(currentBuffer, sizeof currentBuffer) != nullptr);
    const std::string current = currentBuffer;
    const std::string clock = "14:05:09";
    painter.clockText = [&]() { return clock; };
    const std::string logical = "Path: " + current + "  " + clock;
    CHECK_EQ(painter.pathLine(), logical);
    CHECK(logical.rfind("路徑", 0) != 0);
    CHECK(logical.find("路徑") == std::string::npos);
    CHECK(logical.find(current) != std::string::npos);
    CHECK(logical.size() >= clock.size() && logical.compare(logical.size() - clock.size(), clock.size(), clock) == 0);
    const int narrow = static_cast<int>(std::string("Path: ").size()) + 4;
    const std::string shortened = painter.pathLine(narrow);
    CHECK_EQ(shortened, std::string("Path: ") + current.substr(current.size() - 4));
    CHECK(shortened.find(clock) == std::string::npos);
    CHECK(shortened.rfind("路徑", 0) != 0);

    hallo::Tui session(hallo::kAppName, hallo::kVersion, nullptr, nullptr, "");
    session.painter() = hallo::MenuPainter(home);
    session.painter().clockText = [&]() { return clock; };
    const std::string expected = "Path: " + current + "  " + clock;
    for (const std::vector<std::string> &lines :
         {session.frontLines(), session.selfLines(), session.logLines(), session.languageLines()}) {
        CHECK_EQ(lines.at(0), expected);
        CHECK(lines.at(0).rfind("路徑", 0) != 0);
        const auto split = lines.at(0).find("  ");
        CHECK(split != std::string::npos);
        CHECK(lines.at(0).substr(0, split).find("HalloTui") == std::string::npos);
    }
}

void testWideColumns() {
    CHECK_EQ(hallo::displayWidth("╭─╮"), 3);
    CHECK_EQ(hallo::displayWidth("█"), 1);
    CHECK_EQ(hallo::displayWidth("路径"), hallo::displayWidth("Path"));
    CHECK_EQ(hallo::displayWidth("路徑"), 4);
    CHECK_EQ(hallo::displayWidth("パス"), 4);
    CHECK_EQ(hallo::displayWidth("경로"), 4);

    const std::string home = makeTemp("vjwide_");
    const std::string folder = makeTemp("vjclips_");
    EnvGuard homeEnv("HOME", home.c_str());
    EnvGuard videojoin("VIDEOJOIN_LANG", nullptr);
    EnvGuard lang("LANG", nullptr);
    EnvGuard lcAll("LC_ALL", nullptr);
    CwdGuard cwd(folder);
    const std::string clock = "14:05:09";
    hallo::MenuPainter painter(home);
    painter.clockText = [&]() { return clock; };
    const int wide = 79;
    const std::string english = painter.pathLine(wide);
    CHECK_EQ(hallo::displayWidth(english), wide);
    CHECK(english.size() >= clock.size() &&
          english.compare(english.size() - clock.size(), clock.size(), clock) == 0);
    const int englishGap = gapBefore(english, clock);

    struct Sample {
        const char *label;
        const char *first;
        const char *code;
    };
    const Sample samples[] = {
        {"路径", "路", "zh-Hans"},
        {"路徑", "路", "zh-Hant"},
        {"パス", "パ", "ja"},
        {"경로", "경", "ko"},
    };
    for (const Sample &sample : samples) {
        EnvGuard forced("VIDEOJOIN_LANG", sample.code);
        hallo::MenuPainter labeled(home);
        labeled.clockText = [&]() { return clock; };
        CHECK_EQ(labeled.pathLabel(), std::string(sample.label));
        const std::string line = labeled.pathLine(wide);
        CHECK(line.rfind(std::string(sample.label) + ": ", 0) == 0);
        CHECK(line.size() >= clock.size() && line.compare(line.size() - clock.size(), clock.size(), clock) == 0);
        CHECK_EQ(hallo::displayWidth(line), wide);
        CHECK_EQ(gapBefore(line, clock), englishGap);
        CHECK_EQ(labeled.pathLine(1), std::string());
        CHECK_EQ(labeled.pathLine(2), std::string(sample.first));
        CHECK_EQ(labeled.pathLine(3), std::string(sample.first));
        CHECK_EQ(labeled.pathLine(5), std::string(sample.label) + ":");
    }

    const std::map<std::string, std::vector<std::string>> expected = {
        {"zh-Hans",
         {"1. hello   : 显示一条欢迎消息", "3. 系统日志: 查看、清空，以及日志文件夹", "4. 语言    : 此菜单的显示语言",
          "8. 自我管理: 版本、关于，以及 pip 生命周期", "9. 离开    : 离开"}},
        {"zh-Hant",
         {"1. hello   : 顯示一則歡迎訊息", "3. 系統日誌: 查看、清空，以及日誌資料夾", "4. 語言    : 這個選單的顯示語言",
          "8. 自我管理: 版本、關於，以及 pip 生命週期", "9. 離開    : 離開"}},
        {"ko",
         {"1. hello      : 환영 메시지를 보여 줍니다", "3. 시스템 로그: 보기, 비우기, 로그 폴더",
          "4. 언어       : 이 메뉴의 표시 언어", "8. 자기관리   : 버전, 정보, pip 수명 주기", "9. 종료       : 종료"}},
        {"ja",
         {"1. hello       : 歓迎のメッセージを表示する", "3. システムログ: 表示、消去、ログフォルダ",
          "4. 言語        : このメニューの表示言語", "8. 自己管理    : バージョン、概要、pip のライフサイクル",
          "9. 終了        : 終了"}},
    };
    const std::map<std::string, std::vector<std::string>> shorts = {
        {"zh-Hans", {"语言", "系统日志", "自我管理", "离开"}},
        {"zh-Hant", {"語言", "系統日誌", "自我管理", "離開"}},
        {"ko", {"언어", "시스템 로그", "자기관리", "종료"}},
        {"ja", {"言語", "システムログ", "自己管理", "終了"}},
    };
    for (const auto &item : shorts) {
        EnvGuard forced("VIDEOJOIN_LANG", item.first.c_str());
        hallo::MenuPainter boardPainter(home);
        boardPainter.clockText = [&]() { return clock; };
        CHECK_EQ(boardPainter.language().code(), item.first);
        const std::vector<hallo::MenuRow> board = boardPainter.displayRows("front");
        const std::vector<std::string> formatted = boardPainter.formatRows(board);
        const std::vector<std::string> &wanted = expected.at(item.first);
        CHECK_EQ(static_cast<int>(formatted.size()), static_cast<int>(wanted.size()));
        for (std::size_t index = 0; index < formatted.size() && index < wanted.size(); ++index) {
            CHECK_EQ(formatted.at(index), wanted.at(index));
        }
        hallo::MenuModel model(&boardPainter);
        RecordingScreen screen;
        boardPainter.paint(screen, model, hallo::kAppName, hallo::kVersion);
        occupy(screen.calls);
        const int placeable = screen.getmaxyx().second - 1;
        CHECK_EQ(screen.calls.at(0).y, 0);
        CHECK_EQ(screen.calls.at(0).x, 0);
        const std::string pathText = screen.calls.at(0).text;
        CHECK(pathText.rfind(boardPainter.pathLabel() + ": ", 0) == 0);
        CHECK(pathText.size() >= clock.size() &&
              pathText.compare(pathText.size() - clock.size(), clock.size(), clock) == 0);
        CHECK_EQ(hallo::displayWidth(pathText), placeable);
        int framed = 0;
        for (const RecordingScreen::Call &call : screen.calls) {
            if (!call.text.empty() && call.text.rfind("╭", 0) == 0) {
                ++framed;
                CHECK_EQ(hallo::displayWidth(call.text), placeable);
            }
        }
        CHECK_EQ(framed, 1);
        for (const hallo::RowPart &part : boardPainter.rowParts(board)) {
            bool wanted = false;
            for (const std::string &word : item.second) {
                if (part.shortText == word) wanted = true;
            }
            if (!wanted) continue;
            const int verbX = hallo::displayWidth(part.numberField);
            int matches = 0;
            int y = 0;
            int x = 0;
            for (const RecordingScreen::Call &call : screen.calls) {
                if (call.text == part.shortText && call.x == verbX) {
                    ++matches;
                    y = call.y;
                    x = call.x;
                }
            }
            CHECK_EQ(matches, 1);
            const int colonX = x + hallo::displayWidth(part.shortText);
            int colonMatches = 0;
            for (const RecordingScreen::Call &call : screen.calls) {
                if (call.y == y && call.x == colonX) {
                    ++colonMatches;
                    CHECK_EQ(call.text, part.verbPad + ": ");
                }
            }
            CHECK_EQ(colonMatches, 1);
            CHECK(colonX != x + static_cast<int>(part.shortText.size()));
        }
    }

    hallo::MenuPainter englishPainter(home);
    englishPainter.clockText = [&]() { return clock; };
    CHECK_EQ(englishPainter.language().code(), std::string("en"));
    const std::vector<hallo::MenuRow> langRows = englishPainter.displayRows("lang");
    const std::vector<std::string> formatted = englishPainter.formatRows(langRows);
    std::string joined;
    for (const std::string &line : formatted) joined += line + "\n";
    for (const char *endonym : {"简体中文", "繁體中文", "日本語", "한국어"}) {
        CHECK(joined.find(endonym) != std::string::npos);
    }
    hallo::MenuModel model(&englishPainter);
    model.layer = "lang";
    RecordingScreen screen(40, 80);
    englishPainter.paint(screen, model, hallo::kAppName, hallo::kVersion);
    occupy(screen.calls);
    const std::vector<std::string> endonyms = {"简体中文", "繁體中文", "日本語", "한국어"};
    for (const hallo::RowPart &part : englishPainter.rowParts(langRows)) {
        bool wanted = false;
        for (const std::string &word : endonyms) {
            if (part.shortText == word) wanted = true;
        }
        if (!wanted) continue;
        const int verbX = hallo::displayWidth(part.numberField);
        int matches = 0;
        int y = 0;
        int x = 0;
        for (const RecordingScreen::Call &call : screen.calls) {
            if (call.text == part.shortText && call.x == verbX) {
                ++matches;
                y = call.y;
                x = call.x;
            }
        }
        CHECK_EQ(matches, 1);
        const int colonX = x + hallo::displayWidth(part.shortText);
        int colonMatches = 0;
        for (const RecordingScreen::Call &call : screen.calls) {
            if (call.y == y && call.x == colonX) {
                ++colonMatches;
                CHECK_EQ(call.text, part.verbPad + ": ");
            }
        }
        CHECK_EQ(colonMatches, 1);
        CHECK(colonX != x + static_cast<int>(part.shortText.size()));
    }
}

void testLanguage() {
    const std::string home = makeTemp("vjlang_");
    EnvGuard homeEnv("HOME", home.c_str());
    EnvGuard videojoin("VIDEOJOIN_LANG", nullptr);
    EnvGuard lang("LANG", "zh_TW.UTF-8");
    EnvGuard lcAll("LC_ALL", "zh_TW.UTF-8");
    hallo::MenuPainter painter(home);
    hallo::MenuModel model(&painter);

    const std::vector<hallo::MenuRow> front = painter.displayRows("front");
    CHECK_EQ(static_cast<int>(front.size()), 5);
    CHECK_EQ(front.at(0).number, 1);
    CHECK_EQ(front.at(1).number, 3);
    CHECK_EQ(front.at(1).kind, std::string("system-log"));
    CHECK_EQ(front.at(2).number, 4);
    CHECK_EQ(front.at(2).shortText, std::string("language"));
    CHECK_EQ(front.at(2).kind, std::string("language"));
    CHECK_EQ(front.at(3).number, 8);
    CHECK_EQ(front.at(4).number, 9);
    for (const hallo::MenuRow &row : front) CHECK(row.number != 2);

    const std::vector<hallo::MenuRow> logRows = painter.displayRows("log");
    CHECK_EQ(static_cast<int>(logRows.size()), 4);
    CHECK_EQ(logRows.at(0).number, 31);
    CHECK_EQ(logRows.at(1).number, 32);
    CHECK_EQ(logRows.at(2).number, 33);
    CHECK_EQ(logRows.at(3).number, 0);
    CHECK_EQ(logRows.at(0).shortText, std::string("view-log"));
    CHECK_EQ(logRows.at(1).shortText, std::string("clear-log"));
    CHECK_EQ(logRows.at(2).shortText, std::string("log-folder"));
    const std::vector<hallo::MenuRow> selfRows = painter.displayRows("self");
    const int selfNumbers[] = {82, 83, 84, 85, 86, 87, 0};
    CHECK_EQ(static_cast<int>(selfRows.size()), 7);
    for (int index = 0; index < 7; ++index) CHECK_EQ(selfRows.at(static_cast<std::size_t>(index)).number, selfNumbers[index]);
    CHECK_EQ(selfRows.at(0).shortText, std::string("version"));
    CHECK_EQ(selfRows.at(1).shortText, std::string("about"));
    CHECK_EQ(selfRows.at(1).explain, std::string("version and this computer"));
    CHECK_EQ(selfRows.at(6).kind, std::string("back"));

    const std::vector<hallo::MenuRow> langRows = painter.displayRows("lang");
    CHECK_EQ(static_cast<int>(langRows.size()), 14);
    for (int number = 41; number <= 53; ++number) CHECK_EQ(langRows.at(static_cast<std::size_t>(number - 41)).number, number);
    CHECK_EQ(langRows.back().number, 0);
    CHECK_EQ(langRows.at(0).shortText, std::string("English"));
    CHECK_EQ(langRows.at(2).shortText, std::string("繁體中文"));
    CHECK_EQ(langRows.at(12).shortText, std::string("Ελληνικά"));
    CHECK_EQ(commit(model, "4"), std::string());
    CHECK_EQ(model.layer, std::string("lang"));

    const std::string leaf = painter.language().leafPath();
    CHECK(!existsPath(home + "/.local"));
    CHECK_EQ(commit(model, "0"), std::string());
    CHECK_EQ(model.layer, std::string("front"));
    CHECK(!existsPath(leaf));
    CHECK_EQ(commit(model, "4"), std::string());
    CHECK_EQ(commit(model, "40"), std::string());
    CHECK_EQ(model.layer, std::string("lang"));
    CHECK(model.error.find("not on this list") != std::string::npos);
    CHECK(!existsPath(leaf));
    CHECK_EQ(commit(model, "54"), std::string());
    CHECK(!existsPath(leaf));
    CHECK_EQ(commit(model, "43"), std::string("set-zh-Hant"));

    hallo::MenuSession session(
        "HalloTui", "1.0.4", &painter, []() { return std::string("1.0.4"); },
        []() { return std::string("about"); }, [](const std::string &) { return std::string(); });
    session.model = model;
    CHECK_EQ(session.takeAction("set-zh-Hant"), std::string("stay"));
    CHECK_EQ(session.model.layer, std::string("front"));
    CHECK_EQ(session.model.notice, std::string("選單語言是繁體中文"));
    CHECK_EQ(painter.pathLabel(), std::string("路徑"));
    CHECK_EQ(painter.language().code(), std::string("zh-Hant"));
    CHECK(leaf.rfind(home, 0) == 0);
    CHECK_EQ(readFile(leaf), std::string("zh-Hant\n"));
    CHECK_EQ(modeOf(leaf), 0600);
    CHECK_EQ(modeOf(leaf.substr(0, leaf.find_last_of('/'))), 0700);
    const std::vector<hallo::MenuRow> translated = painter.displayRows("front");
    CHECK_EQ(translated.at(2).shortText, std::string("語言"));
    CHECK_EQ(translated.at(0).shortText, std::string("hello"));
    CHECK_EQ(translated.at(0).kind, std::string("hello"));
    CHECK_EQ(painter.displayRows("log").at(0).shortText, std::string("view-log"));
    CHECK_EQ(painter.displayRows("lang").at(0).shortText, std::string("English"));
    {
        StreamCapture err(STDERR_FILENO, "/tmp/hallo-test-lang-verb.txt");
        hallo::Cli app;
        CHECK_EQ(app.run({"language"}), 1);
        CHECK(err.stop().find("Unknown verb 'language'") != std::string::npos);
    }

    {
        const std::string badHome = makeTemp("vjlangbad_");
        const std::string badLeaf = badHome + "/.local/HalloTui/language";
        ::mkdir((badHome + "/.local").c_str(), 0700);
        ::mkdir((badHome + "/.local/HalloTui").c_str(), 0700);
        {
            std::ofstream output(badLeaf);
            output << "nope\nzh-Hant\n";
        }
        hallo::MenuLanguage loaded(badHome);
        CHECK_EQ(loaded.code(), std::string("en"));
        CHECK_EQ(loaded.pathLabel(), std::string("Path"));
        CHECK_EQ(readFile(badLeaf), std::string("nope\nzh-Hant\n"));
        EnvGuard forced("VIDEOJOIN_LANG", "zh-Hant");
        hallo::MenuLanguage overridden(badHome);
        CHECK_EQ(overridden.code(), std::string("zh-Hant"));
        CHECK_EQ(overridden.pathLabel(), std::string(hallo::MenuPainter::kPathLabelZhHant));
        CHECK_EQ(readFile(badLeaf), std::string("nope\nzh-Hant\n"));
    }
    {
        const std::string crHome = makeTemp("vjlangcr_");
        const std::string crLeaf = crHome + "/.local/HalloTui/language";
        ::mkdir((crHome + "/.local").c_str(), 0700);
        ::mkdir((crHome + "/.local/HalloTui").c_str(), 0700);
        {
            std::ofstream output(crLeaf);
            output << "es\r\n";
        }
        hallo::MenuLanguage cr(crHome);
        CHECK_EQ(cr.code(), std::string("es"));
        CHECK_EQ(cr.pathLabel(), std::string("Ruta"));
    }

    hallo::MenuLanguage empty("");
    CHECK_EQ(empty.save("es"), std::string("Could not save the menu language"));
    CHECK_EQ(empty.code(), std::string("en"));
    painter.language().save("zh-Hant");
    const std::string local = home + "/.local";
    ::rename(local.c_str(), (local + ".kept").c_str());
    {
        std::ofstream output(local);
        output << "x";
    }
    const std::string message = painter.language().save("es");
    CHECK_EQ(painter.language().code(), std::string("zh-Hant"));
    CHECK_EQ(message, std::string("無法儲存選單語言"));
}

class ScriptScreen : public hallo::Screen {
public:
    explicit ScriptScreen(std::vector<int> keys) : keys_(std::move(keys)) {}
    std::pair<int, int> getmaxyx() const override { return {24, 80}; }
    void erase() override {}
    void addstr(int, int, const std::string &, int) override {}
    void refresh() override {}
    void cursSet(int) override {}
    void moveCursor(int, int) override {}
    bool hasTimeout() const override { return true; }
    void timeoutMs(int) override {}
    int getch() override {
        ++reads;
        if (at_ >= keys_.size()) return 10;
        return keys_[at_++];
    }
    void keypad(bool) override {}
    int reads = 0;

private:
    std::vector<int> keys_;
    std::size_t at_ = 0;
};

void testClockTimeoutIsNotExit() {
    const std::string home = makeTemp("vjclock_");
    EnvGuard videojoin("VIDEOJOIN_LANG", nullptr);
    hallo::MenuPainter painter(home);
    hallo::MenuSession session(
        hallo::kAppName, hallo::kVersion, &painter, []() { return std::string(); },
        []() { return std::string(); }, [](const std::string &) { return std::string(); });
    /* -1 on the front board is the one-second clock wait. It must not exit. */
    ScriptScreen screen({-1, '9', 10});
    CHECK_EQ(session.run(screen), 0);
    CHECK(screen.reads >= 3);
}

void testResultScroll() {
    const std::string home = makeTemp("vjscroll_");
    EnvGuard videojoin("VIDEOJOIN_LANG", nullptr);
    hallo::MenuPainter painter(home);
    hallo::MenuModel model(&painter);
    model.showResult("one line");
    CHECK_EQ(model.applyKey(hallo::kKeyDown, 24), std::string());
    CHECK_EQ(model.phase, std::string("board"));

    std::string longPage;
    for (int line = 0; line < 40; ++line) longPage += "line " + std::to_string(line) + "\n";
    model.showResult(longPage);
    CHECK_EQ(model.applyKey(hallo::kKeyDown, 24), std::string());
    CHECK_EQ(model.phase, std::string("result"));
    CHECK(model.resultOffset > 0);
    const int offset = model.resultOffset;
    CHECK_EQ(model.applyKey(hallo::kKeyUp, 24), std::string());
    CHECK(model.resultOffset < offset);
}

void testAboutPage() {
    hallo::AboutPage page(hallo::kAppName, hallo::kVersion, hallo::kConsoleName, hallo::kAuthorName,
                          hallo::kLastUpdate, hallo::kHomepage, hallo::kBasicUsage);
    const std::string home = makeTemp("vjabout_");
    EnvGuard homeEnv("HOME", home.c_str());
    const std::string text = page.frameworkAbout();
    CHECK(text.rfind(std::string(hallo::kAppName) + " " + hallo::kVersion + "\n", 0) == 0);
    CHECK(text.find("Domain: Show a welcome message\n") != std::string::npos);
    CHECK(text.find("Runtime tools: none\n") != std::string::npos);
    CHECK(text.find("Entry points: hallo-tui\n") != std::string::npos);
    CHECK(text.find("python -m") == std::string::npos);
    CHECK(text.find("curl -fsSL") == std::string::npos);
    CHECK(text.find("py-tui") == std::string::npos);
    CHECK(text.find("ffmpeg is on PATH") == std::string::npos);
    CHECK(text.find("[CHECK SYSTEM]:") != std::string::npos);
    CHECK(text.find("  Now checking your operation system!\n") != std::string::npos);
    CHECK(text.find("    Compiler: g++ ") != std::string::npos);
    CHECK(text.find("    C++ standard: C++17\n") != std::string::npos);
    CHECK(text.find("    Operation System: ") != std::string::npos);
    CHECK(text.find("    Architecture: ") != std::string::npos);
    CHECK(text.find("    Inside docker container: ") != std::string::npos);
    CHECK(text.find("    Cache folder used: ") != std::string::npos);
    CHECK(text.find("    Cache folder (preferred): /dev/shm/cache/cache-HalloTui-") != std::string::npos);
    CHECK(text.find("    Cache folder (1st fallback): /tmp/cache/cache-HalloTui-") != std::string::npos);
    CHECK(text.find("    Cache folder (2nd fallback): " + home + "/.cache/cache-HalloTui-") != std::string::npos);
    CHECK(text.find("    Persistence storage: " + home + "/.local/HalloTui") != std::string::npos);
    CHECK(text.find("Basic Usage:") != std::string::npos);
    CHECK(text.find("hallo-tui hello") != std::string::npos);
    CHECK(!existsPath(home + "/.cache"));
    CHECK(!existsPath(home + "/.local"));
    const auto stamp = text.find(" HalloTui(v1.0.0)  [CHECK SYSTEM]:");
    CHECK(stamp != std::string::npos);
    CHECK(stamp >= 26);
    const std::string prefix = text.substr(stamp - 26, 26);
    CHECK_EQ(prefix.substr(19), std::string(".000000"));
    CHECK_EQ(prefix.at(4), '-');
    CHECK_EQ(prefix.at(10), ' ');
    CHECK_EQ(prefix.at(13), ':');

    CHECK_EQ(page.installKind("/tmp/not/src/main/cpp/hallo-tui"), std::string("uninstalled"));
    const std::string tree = home + "/proj";
    ::mkdir(tree.c_str(), 0700);
    ::mkdir((tree + "/src").c_str(), 0700);
    ::mkdir((tree + "/src/main").c_str(), 0700);
    ::mkdir((tree + "/src/main/cpp").c_str(), 0700);
    {
        std::ofstream make(tree + "/Makefile");
        make << "all:\n";
    }
    CHECK_EQ(page.installKind(tree + "/target/hallo-tui"), std::string("uninstalled"));
    CHECK_EQ(page.installKind(home + "/bin/hallo-tui"), std::string("local"));
    CHECK_EQ(page.installKind("/usr/local/bin/hallo-tui"), std::string("global"));
    CHECK_EQ(page.installKind("/opt/hallo-tui"), std::string("global"));
    CHECK_EQ(page.installSentence("global"), std::string("You are using the GLOBAL INSTALLED version, location:"));
    CHECK_EQ(page.installSentence("local"), std::string("You are using the LOCAL INSTALLED version, location:"));
    CHECK_EQ(page.installSentence("uninstalled"),
             std::string("You are using an UNINSTALLED version, location:"));
    const std::vector<std::string> box =
        page.aboutBoxLines(hallo::kBasicUsage, "/usr/local/bin/hallo-tui", "global");
    CHECK(box.size() >= 3);
    CHECK_EQ(box.front().size(), box.back().size());
    for (const std::string &line : box) CHECK_EQ(line.size(), box.front().size());
    CHECK(box.at(0).find('*') != std::string::npos);
}

void testSelfCommands() {
    hallo::SelfManage self(hallo::kAppName, hallo::kVersion);
    const std::vector<std::string> check = self.argvFor("version-check");
    CHECK_EQ(static_cast<int>(check.size()), 6);
    CHECK_EQ(check.at(0), std::string("python3"));
    CHECK_EQ(check.at(1), std::string("-m"));
    CHECK_EQ(check.at(2), std::string("pip"));
    CHECK_EQ(check.at(3), std::string("index"));
    CHECK_EQ(check.at(4), std::string("versions"));
    CHECK_EQ(check.at(5), std::string("HalloTui"));
    const std::vector<std::string> uninstall = self.argvFor("self-uninstall");
    CHECK(uninstall.size() >= 2);
    CHECK_EQ(uninstall.back(), std::string("HalloTui"));
    CHECK_EQ(uninstall.at(uninstall.size() - 2), std::string("-y"));
}

}  // namespace

int main() {
    testHello();
    testVersion();
    testPathLabel();
    testWideColumns();
    testLanguage();
    testClockTimeoutIsNotExit();
    testResultScroll();
    testAboutPage();
    testSelfCommands();
    {
        hallo::Cli app;
        StreamCapture out(STDOUT_FILENO, "/tmp/hallo-test-empty-help.txt");
        CHECK_EQ(app.run({}), 0);
        const std::string text = out.stop();
        CHECK(text.find("usage: hallo-tui") != std::string::npos);
        CHECK(text.find("Choice:") == std::string::npos);
    }
    if (gFailures != 0) {
        std::cerr << gFailures << " failure(s)\n";
        return 1;
    }
    std::cout << "hallo-tui-tests ok\n";
    return 0;
}
