#include <iostream>
#include <cmath>
#include <windows.h>

using namespace std;

double computeExpression(int n) {
    double sum_sin = 0.0;      // сумма sin(1) + sin(2) + ... + sin(n)
    double term1 = 0.0;        // 1/sin(1)
    double term2 = 0.0;        // 1/(sin(1) + sin(2))
    double term3 = 0.0;        // (-1)^(2*n+1) / sum_sin  (но (-1)^(2*n+1) всегда = -1)
    double one = 1.0;
    double neg_one = -1.0;

    // Вычисляем сумму синусов от 1 до n с помощью C++ (без ассемблера)
    for (int i = 1; i <= n; i++) {
        sum_sin += sin(i);
    }

    // Для term1 и term2 нам нужны sin(1) и sin(1) + sin(2).
    // Если n меньше 2, то term2 вычислить невозможно; поэтому требуем n>=2.
    double s1 = sin(1.0);
    double s12 = sin(1.0) + sin(2.0);

    __asm {
        finit                        // инициализация FPU

        // term1 = 1/sin(1)
        fld     one                // st(0)= 1.0
        fld     s1                 // st(0)= s1, st(1)= 1.0
        fdivp   st(1), st          // st(0)= 1.0/s1
        fstp    term1              // term1 <- 1/s1

        // term2 = 1/(sin(1)+sin(2))
        fld     one                // st(0)= 1.0
        fld     s12                // st(0)= s12, st(1) = 1.0
        fdivp   st(1), st          // st(0)= 1.0/s12
        fstp    term2              // term2 <- 1/s12

        // term3 = (-1)^(2*n+1) / sum_sin.
        // Заметим, что (-1)^(2*n+1) = -1 при любом n.
        fld     neg_one            // st(0)= -1.0
        fld     sum_sin            // st(0)= sum_sin, st(1)= -1.0
        fdivp   st(1), st          // st(0)= (-1.0) / sum_sin
        fstp    term3              // term3 <- (-1)/ sum_sin
    }

    double result;
    __asm {
        finit
        fld     term1              // st0 = term1
        fadd    term2              // st0 = term1 + term2
        fsub    term3              // st0 = term1 + term2 - term3
        fstp    result             // result <- (term1 + term2 - term3)
    }

    return result;
}

int main() {
    // Установка кодовой страницы для корректного отображения русского текста в консоли
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