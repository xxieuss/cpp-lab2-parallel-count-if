#pragma once

struct IsEven {
    bool operator()(int value) const noexcept {
        return value % 2 == 0;
    }
};

struct IsPrime {
    bool operator()(int value) const noexcept {
        if (value < 2) return false;
        if (value % 2 == 0) return value == 2;
        for (int divisor = 3; divisor <= value / divisor; divisor += 2) {
            if (value % divisor == 0) return false;
        }
        return true;
    }
};