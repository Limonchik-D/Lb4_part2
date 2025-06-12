#include <iostream>
#include <cmath>
#include <windows.h>

using namespace std;

double computeExpression(int n) {
    double result = 0.0;
    double product = 1.0; // Хранит произведение sin(1)*sin(2)*...*sin(k)

    for (int k = 1; k <= n; k++) {
        product *= sin(k);
        result += 1.0 / product;
    }

    return result;
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    int n;
    cout << "Введите натуральное число n (n >= 1): ";
    cin >> n;
    if (n < 1) {
        cout << "Ошибка: n должно быть не менее 1." << endl;
        return 1;
    }

    double res = computeExpression(n);
    cout << "Результат: " << res << endl;
    return 0;
}
