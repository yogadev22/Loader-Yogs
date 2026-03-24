#include "support.h"

int main(int argc, char *argv[]) {
    pid = getPid("com.dts.freefiremax");
    if (pid == 0) {
        printf("\nGame is not running");
        Close();
        return 0;
    }

    uintptr_t base = getBase();

    while (true) {
        uintptr_t GameFacade = Read<uintptr_t>(base + 0x0);
    }
}
