#pragma once

constexpr int next_prime(int x) {
    if (x == 2 || x == 1) {
        return 2;
    }
    int y = x + 1;
    bool flag = false;
    while (true) {
        for (int i = 2; i < y / 2; ++i) {
            if (y % i == 0) {
                flag = true;
                break;
            }
        }
        if (!flag) {
            return y;
        }
        y += 1;
        flag = false;
    }
}
