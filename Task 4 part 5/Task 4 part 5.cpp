#include <iostream>
#include <cmath>     // Для функции sin()
#include <windows.h> // Для SetConsoleOutputCP и SetConsoleCP
#include <iomanip>   // Для форматирования вывода (setprecision)

// Используем using namespace std; для удобства, но в больших проектах обычно избегают
using namespace std;

// Функция для вычисления выражения ряда
double computeExpression(int n) {
    // Переменные для работы с FPU и общие для логики
    double total_sum_of_series_fpu_result = 0.0; // Итоговая сумма ряда
    double current_partial_sin_sum = 0.0;       // Сумма sin(i) от 1 до текущего k

    __asm {
        finit           // Инициализация сопроцессора (FPU)
        fldz            // ST(0) = 0.0 — инициализация общей суммы ряда (S_total)

        mov ecx, 1      // Инициализация счетчика цикла k = 1

    loop_start:
        cmp ecx, n      // Сравниваем k с n
        jg  end_loop    // Если k > n, выходим из цикла

        // --- Вычисление sin(k) и обновление current_partial_sin_sum ---
        // FPU не имеет встроенной функции sin. Вычисляем sin(k) с помощью C++ std::sin.
        // Чтобы передать значение k в sin(), нужно его преобразовать к double.
        // Можно загрузить k в регистр FPU, а затем использовать FPU для добавления.
        
        // Временная переменная для sin(k)
        double sin_k_val;
        
        // Вычисляем sin(k) на стороне C++
        sin_k_val = sin(static_cast<double>(ecx)); 

        // Добавляем sin_k_val к current_partial_sin_sum, используя FPU
        fld current_partial_sin_sum // ST(0) = current_partial_sin_sum
        fld sin_k_val               // ST(0) = sin_k_val, ST(1) = current_partial_sin_sum
        faddp st(1), st             // ST(0) = current_partial_sin_sum + sin_k_val
        fstp current_partial_sin_sum // Сохраняем обновленную сумму синусов обратно в переменную

        // --- Проверка на деление на ноль (current_partial_sin_sum) ---
        // Загружаем абсолютное значение знаменателя для проверки
        fld current_partial_sin_sum // ST(0) = current_partial_sin_sum
        fabs                        // ST(0) = |current_partial_sin_sum|

        // Сравниваем |current_partial_sin_sum| с очень маленьким числом (эпсилон)
        // Если он слишком близок к нулю, это может вызвать ошибку деления на ноль.
        // Вместо FLD_EPSILON, можно использовать константу double.
        fld      0.0000000001 // ST(0) = epsilon, ST(1) = |current_partial_sin_sum|
        fcomip   st(0), st(1) // Сравниваем |sum| с epsilon. Устанавливает флаги.
                              // fcomip удаляет обе операнды из стека.
        // Результаты сравнения:
        // CF=0, ZF=0 (больше)
        // CF=1, ZF=0 (меньше)
        // CF=0, ZF=1 (равно)
        // В данном случае jbe - Jump Below or Equal (если |sum| <= epsilon)
        jbe error_division_by_zero // Если |current_partial_sin_sum| <= epsilon, переходим к ошибке

        // --- Вычисление текущего члена ряда: -1 / current_partial_sin_sum ---
        fld     -1.0            // ST(0) = -1.0
        fld     current_partial_sin_sum // ST(0) = current_partial_sin_sum, ST(1) = -1.0
        fdivp   st(1), st       // ST(0) = ST(1) / ST(0) = -1.0 / current_partial_sin_sum (текущий член ряда)

        // --- Добавление текущего члена к общей сумме ряда ---
        // Общая сумма ряда находится в ST(1)
        faddp   st(1), st       // ST(0) = ST(1) + ST(0) = S_total_old + текущий_член (новая S_total)

        inc ecx                 // Увеличиваем счетчик k
        jmp loop_start          // Повторяем цикл

    error_division_by_zero:
        // В случае деления на ноль, можно вернуть специальное значение,
        // например, NaN (Not a Number), или просто вывести ошибку и выйти.
        // Для простоты, здесь мы выводим сообщение и завершаем.
        fld     qword ptr [total_sum_of_series_fpu_result] // Загружаем текущее значение (может быть не инициализировано)
        fdiv    qword ptr [total_sum_of_series_fpu_result] // Приведет к NaN, если 0/0
        fstp    total_sum_of_series_fpu_result // Сохраняем NaN (или некорректное значение)
        jmp     end_loop_and_error // Переходим к завершению

    end_loop:
    end_loop_and_error:
        fstp total_sum_of_series_fpu_result // Сохраняем итоговую сумму (или NaN/ошибку)
    }

    return total_sum_of_series_fpu_result;
}

int main() {
    // Устанавливаем кодировку консоли для корректного отображения русского текста
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
    
    // Форматируем вывод, чтобы показать больше знаков после запятой
    cout << fixed << setprecision(10); 
    
    // Проверяем, является ли результат NaN (может быть результатом деления на ноль)
    if (isnan(res)) {
        cout << "Ошибка: Произошло деление на ноль во время вычислений." << endl;
    } else {
        cout << "Результат: " << res << endl;
    }
    
    // Не используем _getch() здесь, чтобы программа сразу завершилась после вывода.
    // Если нужно, чтобы консоль не закрывалась:
    // cout << "Нажмите любую клавишу для выхода...";
    // _getch(); 

    return 0;
}
