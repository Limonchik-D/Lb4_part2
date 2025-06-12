#include <iostream>
#include <windows.h>
#include <conio.h>

int main() {
    SetConsoleOutputCP(1251); // Русская кодировка для вывода
    SetConsoleCP(1251);

    double X = 1000.0;  // Ежемесячный взнос
    double S = 0.0;     // Сумма на счету
    double percent = 1.03;
    int i;

    __asm {
        finit
        fldz                // ST(0) = 0.0, начальная сумма

        mov i, 1

    month_loop:
        cmp i, 13
        jge end_loop

        fld X              // ST(0) = X
        fadd               // ST(0) = ST(0) + ST(1)

        mov eax, i
        xor edx, edx
        mov ecx, 3
        div ecx
        cmp edx, 0
        jne skip_percent

        fld percent        // ST(0) = 1.03
        fmul               // ST(0) = сумма * 1.03

    skip_percent:
        inc i
        jmp month_loop

    end_loop:
        fstp S             // Сохраняем результат в переменную S
    }

    std::cout << "Итоговая сумма в начале января следующего года: " << S << " рублей\n";
    _getch();
    return 0;
}
