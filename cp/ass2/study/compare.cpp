#include <iostream>

int main() {
    char c = '0';
    int j = 3;  // Keep j as an integer
    for (int i = 0; i < 5; i++) {
        c++;
        if (c - '0' < j) {
            printf("%c, true\n", c);
        } else {
            printf("%c, false\n", c);
        }
    }

    return 0;
}
