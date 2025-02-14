#include <stdbool.h>
#include <stdio.h>

bool same_sign(int a, int b) {
    printf("%d\n", (a ^ b));
    return (a ^ b) >= 0;
}


int main() {
    int x = -5, y = -10;
    int z = 7, w = 3;

    printf("same_sign(%d, %d) = %d\n", x, y, same_sign(x, y)); // 0 (false)
    printf("same_sign(%d, %d) = %d\n", z, w, same_sign(z, w)); // 1 (true)

    return 0;
}