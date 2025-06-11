#include <iostream>
#include <windows.h>
#include <conio.h>

int main() {
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);

    double a = 1.4;
    double sum = 0.0;
    int i = 1;
    int result = 0;

    __asm {
        finit           // Инициализация сопроцессора
        fldz            // ST(0) = 0.0 — сумма

        mov i, 1

        loop_start:
        fild i          // ST(0) = i
            fld1
            fdiv            // ST(0) = 1 / i
            fadd            // сумма += 1 / i

            fld st(0)       // Копируем сумму в ST(0)
            fld a           // ST(0) = a, ST(1) = сумма
            fcomip st(0), st(1) // Сравнение: a ? сумма
            fstp st(0)      // Убираем a

            jb found

            inc i
            jmp loop_start

            found :
        mov eax, i
            mov result, eax
            fstp st(0)      // Очистка стека
    }

    std::cout << "Сумма меньше " << a << " достигается при i = " << result << "\n";
    _getch();
    return 0;
}
