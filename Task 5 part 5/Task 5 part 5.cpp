#include <iostream>
#include <windows.h>

using namespace std;

double computeSum(int n) {
    if (n < 0) return 0.0;

    // Вычисляем факториалы от 0 до n
    double* fact = new double[n + 1];
    fact[0] = 1.0;
    for (int i = 1; i <= n; ++i) {
        fact[i] = fact[i - 1] * i;
    }

    double sum = 0.0;

    for (int k = 0; k <= n; ++k) {
        double term = 0.0;
        int sign = (k % 2 == 0) ? 1 : -1;
        double numerator = sign * (k + 1);
        double denominator = fact[k];

        __asm {
            finit
            fld     numerator         // st0 = numerator
            fld     denominator       // st0 = denominator, st1 = numerator
            fdivp   st(1), st         // st0 = numerator / denominator
            fstp    term              // term = result, стек очищен
        }

        sum += term;
    }

    delete[] fact;
    return sum;
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    int n;
    cout << "Введите натуральное число n: ";
    cin >> n;

    if (n < 0) {
        cout << "Ошибка: n должно быть неотрицательным." << endl;
        return 1;
    }

    double result = computeSum(n);
    cout << "Результат суммы S(n) = " << result << endl;

    return 0;
}
