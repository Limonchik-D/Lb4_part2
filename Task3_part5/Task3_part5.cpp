#include <iostream>
#include <windows.h>
#include <conio.h>

int main() {
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);

    double x = -1.1;
    double sum = 0.0;
    int k = 1;

    __asm {
        finit           // Инициализация FPU
        fldz            // ST(0) = 0.0 — начальная сумма

        mov k, 1

    loop_start:
        cmp k, 6
        jge end_loop

        // Вычисляем x^(2k)
        fld x              // ST(0) = x
        fmul st(0), st(0)  // ST(0) = x^2

        mov ecx, 1

    power_loop:
        inc ecx
        cmp ecx, k
        jg end_power
        fmul st(0), st(0)  // ST(0) = x^(2*ecx)
        jmp power_loop

    end_power:
        // Делим на 2k
        mov eax, k
        shl eax, 1           // eax = 2k
        push eax
        fild dword ptr [esp] // ST(0) = 2k (int -> float)
        add esp, 4
        fdiv                // ST(0) = x^(2k) / 2k

        fadd                // sum += ...

        inc k
        jmp loop_start

    end_loop:
        fstp sum            // sum = ST(0)
    }

    std::cout << "Сумма ряда при x = -1.1: " << sum << "\n";
    _getch();
    return 0;
}
