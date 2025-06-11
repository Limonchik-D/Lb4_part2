#include <iostream>
#include <windows.h>

using namespace std;

double computeSum(int n) {
    // Предварительно вычисляем факториалы для k от 0 до n: fact[k] = k!
    double* fact = new double[n + 1];
    fact[0] = 1.0;
    for (int i = 1; i <= n; ++i) {
        fact[i] = fact[i - 1] * i;
    }

    double sum = 0.0;

    // Суммирование по k от 0 до n
    for (int k = 0; k <= n; ++k) {
        double term = 0.0;
        // Вычисляем (-1)^k: если k четное, то 1, иначе -1.
        int sign = (k % 2 == 0) ? 1 : -1;
        double numerator = sign * (k + 1); // (k+1) * (-1)^k
        double denominator = fact[k];      // k!

        // Используем встроенный ассемблер для вычисления: numerator / denominator
        __asm {
            finit                   // Инициализация FPU
            fld  numerator         // st0 = numerator
            fld  denominator       // st0 = denominator, st1 = numerator
            fdivp st(1), st        // st0 = numerator / denominator; удаляем st1 из стека
            fstp term             // сохраняем результат в term
        }
        sum += term;
    }

    delete[] fact;
    return sum;
}

int main() {
    // Устанавливаем кодовую страницу консоли для корректного отображения русского текста
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    int n;
    cout << "Введите натуральное число n: ";
    cin >> n;

    double result = computeSum(n);
    cout << "Результат суммы S(n) = " << result << endl;

    return 0;
}