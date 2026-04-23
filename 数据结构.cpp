#include <stdio.h>
#include <math.h>
#include <windows.h>
#include <tchar.h>

float f(float x, float y, float z) {
    float a = x * x + 9.0f / 4.0f * y * y + z * z - 1;
    return a * a * a - x * x * z * z * z - 9.0f / 80.0f * y * y * z * z * z;
}

float h(float x, float z) {
    for (float y = 1.0f; y >= 0.0f; y -= 0.001f)
        if (f(x, y, z) <= 0.0f)
            return y;
    return 0.0f;
}

int main() {
    HANDLE o = GetStdHandle(STD_OUTPUT_HANDLE);
    if (o == INVALID_HANDLE_VALUE) return 1;

    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(o, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(o, &cursorInfo);

    TCHAR buffer[25][80] = { _T(' ') };
    TCHAR ramp[] = _T(".:-=+*#%@");

    int frame = 0;

    for (float t = 0.0f;; t += 0.1f) {
        int sy = 0;
        float s = sinf(t);
        float a = s * s * s * s * 0.2f;

        for (float z = 1.3f; z > -1.2f; z -= 0.1f) {
            TCHAR* p = &buffer[sy++][0];
            float tz = z * (1.2f - a);
            for (float x = -1.5f; x < 1.5f; x += 0.05f) {
                float tx = x * (1.2f + a);
                float v = f(tx, 0.0f, tz);
                if (v <= 0.0f) {
                    float y0 = h(tx, tz);
                    float ny = 0.01f;
                    float nx = h(tx + ny, tz) - y0;
                    float nz = h(tx, tz + ny) - y0;
                    float nd = 1.0f / sqrtf(nx * nx + ny * ny + nz * nz);
                    float d = (nx + ny - nz) * nd * 0.5f + 0.5f;
                    *p++ = ramp[(int)(d * 5.0f)];
                }
                else {
                    *p++ = _T(' ');
                }
            }
        }

        int blink = (frame / 15) % 2;
        if (blink) {
            SetConsoleTextAttribute(o, FOREGROUND_RED | FOREGROUND_BLUE);
        }
        else {
            SetConsoleTextAttribute(o, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
        }

        for (sy = 0; sy < 25; sy++) {
            COORD coord = { 0, sy };
            SetConsoleCursorPosition(o, coord);
            DWORD written;
            WriteConsole(o, buffer[sy], 79, &written, NULL);
        }

        COORD textPos1 = { 68, 8 };
        SetConsoleCursorPosition(o, textPos1);
        WriteConsole(o, _T("【嘻嘻，你好呀】"), _tcslen(_T("【嘻嘻，你好呀】")), NULL, 0);

        COORD textPos2 = { 68, 10 };
        SetConsoleCursorPosition(o, textPos2);
        WriteConsole(o, _T("【欢迎来到所谓无人乐园】"), _tcslen(_T("【欢迎来到所谓无人乐园】")), NULL, 0);

        frame++;
        int show = (frame % 30) < 15;

        if (show) {
            COORD textPos3 = { 68, 14 };
            SetConsoleCursorPosition(o, textPos3);
            TCHAR nhzdw[] = _T("NHZDW");
            WORD colors[] = {
                FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY,
                FOREGROUND_BLUE | FOREGROUND_INTENSITY,
                FOREGROUND_RED | FOREGROUND_INTENSITY,
                FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY,
                FOREGROUND_GREEN | FOREGROUND_INTENSITY
            };
            for (int i = 0; i < 5; i++) {
                SetConsoleTextAttribute(o, colors[i]);
                WriteConsole(o, &nhzdw[i], 1, NULL, 0);
            }
        }
        else {
            COORD textPos3 = { 68, 14 };
            SetConsoleCursorPosition(o, textPos3);
            WriteConsole(o, _T("     "), 5, NULL, 0);
        }

        Sleep(33);
    }
}