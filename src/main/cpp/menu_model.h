#ifndef HALLO_MENU_MODEL_H
#define HALLO_MENU_MODEL_H

#include "menu_painter.h"

#include <map>
#include <string>
#include <vector>

namespace hallo {

class MenuModel {
public:
    explicit MenuModel(MenuPainter *painter);

    std::string layer = "front";
    int index = 0;
    std::string buffer;
    int cursor = 0;
    std::string focus = "list";
    std::string error;
    std::string notice;
    std::string phase = "board";
    std::string resultText;
    int resultOffset = 0;

    std::vector<MenuRow> rows() const;
    MenuRow highlightedRow() const;
    bool hasHighlight() const;
    void showResult(const std::string &text);
    void showFront();
    /* Return "exit", a row kind, or empty to stay. */
    std::string applyKey(int key, int screenHeight = 24);
    void insertChar(char ch);
    void backspace();

    MenuPainter *painter;

private:
    void scrollResult(int delta, int screenHeight);
    void arrow(int key);
    void slide(int key);
    void reject();
    std::string commit();
    std::string activateNumber(int number);
    std::string activate(int number, const std::string &kind);
};

}  // namespace hallo

#endif
