#include <kernel/types.h>
#include <user/user.h>

int main() {
    int upd = uptime();
    if (upd < 0) {
        fprintf(2, "uptime: error retrieving uptime\n");
        exit(1);
    }
    fprintf(1, "%d\n", upd);
    exit(0);
}