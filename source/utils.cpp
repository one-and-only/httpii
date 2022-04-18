#include <cstdio>

namespace HTTPii::Utils {
    void clear_screen() {
        printf("\e[1;1H\e[2J");
        printf("\x1b[4;0H");
    }
}