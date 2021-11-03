#include <iostream>
#include <string>
#include "file.h"
#include "ui.h"


int main() {
    UI ui;
    while (ui.startMenu() != -1);
    return 0;
}