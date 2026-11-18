#include "raylib.h"
#include "config.h"

#include <stdint.h>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN

typedef void *HMODULE;
typedef short SHORT;
typedef void *FARPROC;

__declspec(dllimport) HMODULE __stdcall LoadLibraryA(const char *);
__declspec(dllimport) FARPROC __stdcall GetProcAddress(HMODULE, const char *);
__declspec(dllimport) int __stdcall FreeLibrary(HMODULE);

typedef SHORT (__stdcall *GetAsyncKeyStateFn)(int);

static HMODULE user32;
static GetAsyncKeyStateFn asyncKeyState;

static void InitGlobalInput(void)
{
    user32 = LoadLibraryA("user32.dll");

    if (user32) {
        FARPROC proc = GetProcAddress(user32, "GetAsyncKeyState");

        /* Copy function pointer without incompatible cast */
        if (proc)
            __builtin_memcpy(&asyncKeyState, &proc, sizeof(asyncKeyState));
    }
}

static bool IsKeyPressedGlobally(int key)
{
    return asyncKeyState && (asyncKeyState(key) & 0x8000);
}

static void ShutdownGlobalInput(void)
{
    if (user32)
        FreeLibrary(user32);
}

#else

static void InitGlobalInput(void) {}
static bool IsKeyPressedGlobally(int key) { (void)key; return false; }
static void ShutdownGlobalInput(void) {}

#endif

typedef struct {
    const char *label;
    int key;
    Rectangle rect;
} OverlayKey;

static void DrawOverlayKey(OverlayKey key)
{
    Color color = IsKeyPressedGlobally(key.key)
        ? KEY_PRESSED_COLOR
        : WHITE;

    DrawRectangleRounded(key.rect, 0.18f, 8, color);
    DrawRectangleRoundedLines(key.rect, 0.18f, 8, BLACK);

    int fontSize = 20;
    int textWidth = MeasureText(key.label, fontSize);

    DrawText(
        key.label,
        (int)(key.rect.x + (key.rect.width - textWidth) / 2),
        (int)(key.rect.y + (key.rect.height - fontSize) / 2),
        fontSize,
        BLACK
    );
}

int main(void)
{
    SetConfigFlags(FLAG_WINDOW_TOPMOST);
    InitWindow(320, 205, "Key Overlay");
    SetTargetFPS(60);

    InitGlobalInput();

    OverlayKey keys[] = {
        { "Q", 'Q', { 94, 10, 55, 45 } },
        { "W", 'W', { 154, 10, 55, 45 } },
        { "E", 'E', { 214, 10, 55, 45 } },

        { "A", 'A', { 94, 60, 55, 45 } },
        { "S", 'S', { 154, 60, 55, 45 } },
        { "D", 'D', { 214, 60, 55, 45 } },

        { "CTRL", 0xA2, { 34, 115, 90, 45 } },
        { "SPACE", 0x20, { 130, 115, 155, 45 } }
    };

    const int keyCount = sizeof(keys) / sizeof(keys[0]);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground((Color){ 0, 255, 0, 255 });

        for (int i = 0; i < keyCount; i++)
            DrawOverlayKey(keys[i]);

        EndDrawing();
    }

    CloseWindow();
    ShutdownGlobalInput();

    return 0;
}