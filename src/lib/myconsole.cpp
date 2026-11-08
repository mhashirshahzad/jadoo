#ifdef _WIN32
    #include <windows.h>
    #include <conio.h>
#else
    #include <unistd.h>
    #include <termios.h>
    #include <fcntl.h>
    #include <sys/ioctl.h>
    #include <sys/select.h>
    #include <cstdint>
#endif

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>

using namespace std;

#include "myconsole.h"
#include "mygraphics.h"


// Windows implementation  —  UNCHANGED
#ifdef _WIN32

void OutputString(int x,int y,char *str)
{
    COORD c;
    c.X = x;
    c.Y = y;

    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(h,c);
    cout << str;
    cout.flush();
}

void ClearScreen()
{
    CONSOLE_SCREEN_BUFFER_INFO info;

    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(h,&info);
    system("cls");
    SetConsoleCursorPosition(h,info.dwCursorPosition);
}

void PlaceCursor(int x,int y)
{
    COORD c;
    c.X = x;
    c.Y = y;

    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(h,c);
}

int CheckKeyPressed(int waitTime)
{
    HANDLE h= GetStdHandle(STD_INPUT_HANDLE);
    INPUT_RECORD r;
    DWORD w = 1;
    DWORD eventss;
    DWORD waitResult=0;
    int keypressed = false;
    int toReturn = 0;

    waitResult = WaitForSingleObject(h,waitTime);

    if (waitResult == WAIT_OBJECT_0)
    {
        keypressed = ReadConsoleInput(h,&r,1,&eventss);

        if (keypressed && r.EventType==KEY_EVENT && r.Event.KeyEvent.bKeyDown)
            toReturn = r.Event.KeyEvent.wVirtualKeyCode;
        if (toReturn == 224)
            toReturn = CheckKeyPressed(waitTime);

        FlushConsoleInputBuffer(h);
    }
    return toReturn;
}

int CheckKeyPressed1()
{
    int a=0;
    {
        a = _getch();
        if (a==224)
            return CheckKeyPressed1();
    }
    return a;
}

void GetMaxWindowSize(int &maxHorizontal,int &maxVertical)
{
    COORD c;

    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    c = GetLargestConsoleWindowSize(h);
}

void SetWindowTitle(char Title[])
{
    SetConsoleTitle((LPCWSTR)Title);
}

void GetMaxWindowCoordinates(int &x,int &y)
{
    CONSOLE_SCREEN_BUFFER_INFO info;

    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(h,&info);
    x = info.srWindow.Right;
    y = info.srWindow.Bottom;
}

void SetWindowSize(int width,int height)
{
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    bool bAbs = true;
    SMALL_RECT r;

    r.Left   = 0;
    r.Top    = 0;
    r.Right  = width;
    r.Bottom = height;

    SetConsoleWindowInfo(h,bAbs,&r);
}

bool SetColorAtPoint(int x,int y,int color)
{
    COORD c,size;
    c.X = x;
    c.Y = y;
    size.X = 1;
    size.Y = 1;

    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

    WORD w = color;
    unsigned long written = 0;

    WriteConsoleOutputAttribute(h,&w,1,c,&written);

    if (written)
        return true;

    return false;
}

// mac linux smth POSIX
#else

// ---------- ANSI helpers ----------
static void ansiMove(int x, int y) {
    printf("\033[%d;%dH", y, x);   
}
static void ansiClear() {
    printf("\033[2J\033[H");
    fflush(stdout);
}
static void ansiColorsFromWinAttr(int attr) {
    int fgBits = attr & (CONSOLE_RED | CONSOLE_GREEN | CONSOLE_BLUE);
    int bgBits = attr & (CONSOLE_BG_RED | CONSOLE_BG_GREEN | CONSOLE_BG_BLUE);
    int fg = 30 + (fgBits & CONSOLE_RED   ? 1 : 0)
                 + (fgBits & CONSOLE_GREEN ? 2 : 0)
                 + (fgBits & CONSOLE_BLUE  ? 4 : 0);
    int bg = 40 + (bgBits & CONSOLE_BG_RED   ? 1 : 0)
                 + (bgBits & CONSOLE_BG_GREEN ? 2 : 0)
                 + (bgBits & CONSOLE_BG_BLUE  ? 4 : 0);
    int bright = (attr & CONSOLE_INTENSITY) ? 1 : 0;
    printf("\033[%d;%d;%dm", bright, fg, bg);
}

// ---------- raw-mode RAII ----------
struct RawMode {
    termios saved{};
    bool    active = false;
    RawMode() {
        if (tcgetattr(STDIN_FILENO, &saved) != 0) return;
        termios t = saved;
        t.c_lflag &= ~(ICANON | ECHO);
        t.c_cc[VMIN]  = 0;
        t.c_cc[VTIME] = 0;
        active = tcsetattr(STDIN_FILENO, TCSANOW, &t) == 0;
    }
    ~RawMode() { if (active) tcsetattr(STDIN_FILENO, TCSANOW, &saved); }
};

// Translate UTF-8 arrow-key sequence (ESC [ A/B/C/D) to Windows VK codes.
static int translateArrow(const unsigned char* c) {
    if (c[0] != '[') return 0;
    switch (c[1]) {
        case 'A': return 38;  // UP
        case 'B': return 40;  // DOWN
        case 'C': return 39;  // RIGHT
        case 'D': return 37;  // LEFT
    }
    return 0;
}

// ---------- API implementations ----------
void PlaceCursor(int x, int y)
{
    ansiMove(x, y);
    fflush(stdout);
}

void OutputString(int x, int y, char *str)
{
    ansiMove(x, y);
    fputs(str, stdout);
    fflush(stdout);
}

void ClearScreen()
{
    ansiClear();
}

void ClearScreen1()
{
    ansiClear();
}

int CheckKeyPressed(int waitTime)
{
    RawMode raw;
    fd_set set;
    FD_ZERO(&set);
    FD_SET(STDIN_FILENO, &set);

    timeval tv{ waitTime / 1000, (waitTime % 1000) * 1000 };
    int r = select(STDIN_FILENO + 1, &set, nullptr, nullptr, &tv);
    if (r <= 0) return 0;

    unsigned char c = 0;
    if (read(STDIN_FILENO, &c, 1) != 1) return 0;

    if (c == 0x1B) {                                
        unsigned char seq[2] = {0,0};
        if (read(STDIN_FILENO, &seq[0], 1) == 1 &&
            read(STDIN_FILENO, &seq[1], 1) == 1) {
            int vk = translateArrow(seq);
            if (vk) return vk;
        }
        return 27;                                  
    }
    return (int)c;
}

int CheckKeyPressed1()
{
    RawMode raw;
    unsigned char c = 0;
    if (read(STDIN_FILENO, &c, 1) != 1) return 0;

    if (c == 0x1B) {
        unsigned char seq[2] = {0,0};
        if (read(STDIN_FILENO, &seq[0], 1) == 1 &&
            read(STDIN_FILENO, &seq[1], 1) == 1) {
            int vk = translateArrow(seq);
            if (vk) return vk;
        }
        return 27;
    }
    return (int)c;
}

void GetMaxWindowSize(int &maxHorizontal, int &maxVertical)
{
    winsize ws{};
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == 0 && ws.ws_col && ws.ws_row) {
        maxHorizontal = ws.ws_col;
        maxVertical   = ws.ws_row;
    } else {
        maxHorizontal = 79;
        maxVertical   = 24;
    }
}

void GetMaxWindowCoordinates(int &x, int &y)
{
    GetMaxWindowSize(x, y);
}

void SetWindowTitle(char Title[])
{
    printf("\033]0;%s\007", Title);
    fflush(stdout);
}

// Terminal window size is controlled by the emulator; cannot be set.
void SetWindowSize(int /*width*/, int /*height*/)
{
    // no-op
}

bool SetColorAtPoint(int x, int y, int color)
{
    ansiMove(x, y);
    ansiColorsFromWinAttr(color);
    fputc(' ', stdout);
    printf("\033[0m");
    fflush(stdout);
    return true;
}

#endif  // _WIN32 / POSIX split


// Shared  
void Circle(int a, int b, int radius, MYCOLORREF colVal)
{
    float x, y, change = 0;
    x = a - radius;
    for (; x <= radius + a; x += 0.005f)
    {
        y = b + sqrt((float)(radius*radius) - ((x*x) + (a*a) - (2*a*x)));
        change = y - b;
        mySetPixel(x, b - change, colVal);
        mySetPixel(x, y, colVal);
    }
}
