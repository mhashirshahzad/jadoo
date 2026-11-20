#ifdef _WIN32
    #include <windows.h>
    #include <conio.h>
    #include <fstream>
#else
    #include <unistd.h>
    #include <termios.h>
    #include <vector>
    #include <fcntl.h>
    #include <sys/ioctl.h>
    #include <sys/select.h>
    #include <alsa/asoundef.h>
    #include <alsa/asoundlib.h>
#endif

#include <iostream>
#include <stdlib.h>
#include <cmath>
#include <thread>
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

void ConsoleSleep(float miliseconds) {
    Sleep(miliseconds);
}

void PlaceCursor(int x, int y) {
    COORD c;
    c.X = x - 1;
    c.Y = y - 1;

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

void GetMaxWindowSize(int& maxHorizontal, int& maxVertical)
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

    if (GetConsoleScreenBufferInfo(h, &csbi)) {
        maxHorizontal = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        maxVertical = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    }
    else {
        maxHorizontal = 80;
        maxVertical = 25;
    }
}

void SetWindowTitle(char Title[])
{
    SetConsoleTitle((LPCSTR)Title);
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
    c.X = x - 1;
    c.Y = y - 1;
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

int ConsoleBeep(int freq, int duration) {
    // Beep(freq, duration);
    std::thread worker(Beep, freq, duration);
    worker.detach();
    return 0; 
}
bool PrintColoredCharAtPoint(int x, int y, char c, int color) {
    
    PlaceCursor(x, y);
    cout << c;
    SetColorAtPoint(x, y, color);
    cout << "\033[0m";
    return 1;
}
// mac linux smth POSIX
#else

// ---------- ANSI helpers ----------
static void ansiMove(int x, int y) {
    cout << "\033[" << y << ";" << x << "H";
}
static void ansiClear() {
    cout << "\033[2J\033[H" << flush;
}
static void ansiColorsFromWinAttr(int attr) {
    int f = (attr & CONSOLE_RED   ? 1 : 0)
          | (attr & CONSOLE_GREEN ? 2 : 0)
          | (attr & CONSOLE_BLUE  ? 4 : 0);
    int b = (attr & CONSOLE_BG_RED   ? 1 : 0)
          | (attr & CONSOLE_BG_GREEN ? 2 : 0)
          | (attr & CONSOLE_BG_BLUE  ? 4 : 0);

    int fg = ((attr & CONSOLE_INTENSITY) ? 90 : 30) + f;

    int bg;
    if (b == 0 && !(attr & CONSOLE_BG_INTENSITY))
        bg = 49;                                   // terminal default, no box
    else
        bg = ((attr & CONSOLE_BG_INTENSITY) ? 100 : 40) + b;

    cout << "\033[0;" << fg << ";" << bg << "m";
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

void ConsoleSleep(float miliseconds) {
    usleep(miliseconds * 1000);
}
void PlaceCursor(int x, int y)
{
    ansiMove(x, y);
    cout.flush();
}

bool PrintColoredCharAtPoint(int x, int y, char c, int color) {
    SetColorAtPoint(x, y, color);
    PlaceCursor(x, y);
    cout << c << "\033[0m";
    return 1;
}
void OutputString(int x, int y, char *str)
{
    ansiMove(x, y);
    cout << str;
    cout.flush();
}

void ClearScreen()
{
    ansiClear();
}

void ClearScreen1()
{
    ansiClear();
}

// defined to mimic windows, 0 idea what tihs is
// but sends the beep signal to alsa speaker
int Beep(int freq, int duration) {
    static snd_pcm_t* pcm = nullptr;
    const unsigned rate = 44100;
    if (!pcm) {
        if (snd_pcm_open(&pcm, "default", SND_PCM_STREAM_PLAYBACK, 0) < 0) { pcm = nullptr; return 1; }
        snd_pcm_set_params(pcm, SND_PCM_FORMAT_S16_LE, SND_PCM_ACCESS_RW_INTERLEAVED,
                           1, rate, 1, 50000);
    }

    const int n    = rate * duration / 1000;
    const int fade = rate / 200;
    std::vector<int16_t> buf(n);
    for (int i = 0; i < n; ++i) {
        double env = i < fade ? double(i) / fade : (i > n - fade ? double(n - i) / fade : 1.0);
        buf[i] = freq ? int16_t(std::sin(2.0 * M_PI * freq * i / rate) * 12000 * env) : 0;
    }

    int16_t* p = buf.data();
    int left = n;
    while (left > 0) {
        snd_pcm_sframes_t r = snd_pcm_writei(pcm, p, left);
        if (r < 0) { if (snd_pcm_recover(pcm, (int)r, 1) < 0) return 1; continue; }
        p += r; left -= (int)r;
    }
    snd_pcm_drain(pcm);
    snd_pcm_prepare(pcm);
   return 0;
}


int ConsoleBeep(int freq, int duration) {
    // Beep(freq, duration);
    std::thread worker(Beep, freq, duration);
    worker.detach();
    return 0; 
}

int CheckKeyPressed(int waitTime)
{
    static RawMode raw;
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
    static RawMode raw;
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
    cout << "\033]0;" << Title << '\007' << flush;
}


bool SetColorAtPoint(int x, int y, int color)
{
    ansiMove(x, y);
    ansiColorsFromWinAttr(color);
    cout.flush();
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
