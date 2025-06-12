#include <iostream>
#include <cmath>
#include <windows.h>

using namespace std;

double computeExpression(int n) {
    double sum_sin = 0.0;
    double term1 = 0.0;
    double term2 = 0.0;
    double term3 = 0.0;
    double one = 1.0;
    double neg_one = -1.0;

    // Считаем сумму sin(i)
    for (int i = 1; i <= n; i++) {
        sum_sin += sin(i);
    }

    // sin(1) и sin(1)+sin(2)
    double s1 = sin(1.0);
    double s12 = sin(1.0) + sin(2.0);

    __asm {
        finit

        // term1 = 1 / sin(1)
        fld     one
        fld     s1
        fdivp   st(1), st
        fstp    term1

        // term2 = 1 / (sin(1) + sin(2))
        fld     one
        fld     s12
        fdivp   st(1), st
        fstp    term2

        // term3 = -1 / sum_sin
        fld     neg_one
        fld     sum_sin
        fdivp   st(1), st
        fstp    term3
    }

    double result = 0.0;
    __asm {
        finit
        fld     term1
        fadd    term2
        fsub    term3
        fstp    result
    }

    return result;
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    int n;
    cout << "Введите натуральное число n (n >= 2): ";
    cin >> n;
    if (n < 2) {
        cout << "Ошибка: n должно быть не менее 2." << endl;
        return 1;
    }

    double res = computeExpression(n);
    cout << "Результат: " << res << endl;
    return 0;
}
