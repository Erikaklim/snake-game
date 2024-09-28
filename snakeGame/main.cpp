// Erika Klimaite IV kursas, 5 grupe, 2110576

#include <cstdio>
#include <windows.h>
#include <stdlib.h>
#include <time.h>
#include <tchar.h>

#include "resource.h"
#include "HighScore.h"


#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 400
#define CELL_SIZE 20

#define SNAKE_INITIAL_LENGTH 5
#define SNAKE_MAX_LENGTH 100
#define TIME_INTERVAL 150

typedef struct
{
    int x, y;
} Point;

HINSTANCE hInstance;
HWND hwnd;
HMENU hMenu;
HBITMAP hSnakeBmp, hFoodBmp, hBackgroundBmp;
Point snake[SNAKE_MAX_LENGTH];
int snakeLength = SNAKE_INITIAL_LENGTH;
Point food;
int direction = VK_RIGHT;
int score = 0;
int highScore = 0;
bool isPaused = false;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK ShowDialog(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
void InitializeGame();
void MoveSnake();
void GenerateFood();
void GameOver();
void LoadResources();
void DrawGame(HDC hdc);
void UnloadResources();
void ShowHighScores();
void HandleError(const char* message);

int WINAPI WinMain(HINSTANCE hInstance_, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    LoadResources();
    InitializeGame();

    hInstance = hInstance_;

    WNDCLASS wc = {0};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "SnakeGame";
    wc.hbrBackground = (HBRUSH) COLOR_BACKGROUND;
    RegisterClass(&wc);

    hwnd = CreateWindowEx(
       0,
       "SnakeGame",
       "Snake Game",
       WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX,
       CW_USEDEFAULT, CW_USEDEFAULT,
       WINDOW_WIDTH + 16, WINDOW_HEIGHT + 39,
       NULL,
       NULL,
       hInstance,
       NULL
   );


    ShowWindow(hwnd, nCmdShow);

    SetTimer(hwnd, 1, TIME_INTERVAL, NULL);

    MSG msg;
    while(GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    UnloadResources();

    return 0;
}

LRESULT WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
        case WM_PAINT:
            {
                PAINTSTRUCT ps;
                HDC hdc = BeginPaint(hwnd, &ps);
                DrawGame(hdc);
                EndPaint(hwnd, &ps);
                break;
            }
        case WM_KEYDOWN:
            {
                switch (wParam)
                {
                    case VK_UP:
                        if(direction != VK_DOWN && !isPaused)
                        {
                            direction = VK_UP;
                        }
                    break;
                    case VK_DOWN:
                        if(direction != VK_UP && !isPaused)
                        {
                            direction = VK_DOWN;
                        }
                    break;
                    case VK_LEFT:
                        if(direction != VK_RIGHT && !isPaused)
                        {
                            direction = VK_LEFT;
                        }
                    break;
                    case VK_RIGHT:
                        if(direction != VK_LEFT && !isPaused)
                        {
                            direction = VK_RIGHT;
                        }
                    break;
                    case VK_SPACE:
                        isPaused = !isPaused;
                        if(isPaused)
                        {
                            KillTimer(hwnd, 1);
                            DialogBox(hInstance, MAKEINTRESOURCE(IDD_PAUSE_DIALOG), hwnd, ShowDialog);
                        }else
                        {
                            SetTimer(hwnd, 1, TIME_INTERVAL, NULL);
                        }
                    break;
                }
                break;
            }
        case WM_TIMER:
            {
                MoveSnake();
                InvalidateRect(hwnd, NULL, FALSE);
                break;
            }
        case WM_DESTROY:
            {
                KillTimer(hwnd, 1);
                PostQuitMessage(0);
                break;
            }
        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

INT_PTR ShowDialog(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

    RECT rcGame;
    RECT rcDlg;
    int dlgWidth, dlgHeight, xPos, yPos;

    switch (uMsg)
    {
    case WM_INITDIALOG:
        hwnd = GetParent(hDlg);

        GetWindowRect(hwnd, &rcGame);

        GetWindowRect(hDlg, &rcDlg);
        dlgWidth = rcDlg.right - rcDlg.left;
        dlgHeight = rcDlg.bottom - rcDlg.top;

        xPos = rcGame.left + ((rcGame.right - rcGame.left) - dlgWidth) / 2;
        yPos = rcGame.top + ((rcGame.bottom - rcGame.top) - dlgHeight) / 2;

        SetWindowPos(hDlg, HWND_TOPMOST, xPos, yPos, dlgWidth, dlgHeight, SWP_NOZORDER | SWP_NOSIZE);

        return TRUE;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
    case ID_GAME_RESUME:
        isPaused = false;
            EndDialog(hDlg, ID_GAME_RESUME);
            SetTimer(hwnd, 1, TIME_INTERVAL, NULL);
            return TRUE;

    case ID_GAME_NEW:
        EndDialog(hDlg, ID_GAME_NEW);
            InitializeGame();
            SetTimer(hwnd, 1, TIME_INTERVAL, NULL);
            return TRUE;

    case ID_GAME_EXIT:
        EndDialog(hDlg, ID_GAME_EXIT);
            PostQuitMessage(0);
            return TRUE;

    case ID_HIGHSCORES:
        ShowHighScores();
            return TRUE;
        }
        break;
    }

    return FALSE;
}

void InitializeGame()
{
    highScore = LoadHighScore();
    isPaused = false;
    snakeLength = SNAKE_INITIAL_LENGTH;
    for (int i = 0; i < snakeLength; i++)
    {
        snake[i].x = 10 - i;
        snake[i].y = 10;
    }
    direction = VK_RIGHT;
    score = 0;
    GenerateFood();
}

void MoveSnake()
{
    for (int i = snakeLength - 1; i > 0; i--)
    {
        snake[i] = snake[i - 1];
    }

    switch (direction)
    {
        case VK_UP: snake[0].y--; break;
        case VK_DOWN: snake[0].y++; break;
        case VK_LEFT: snake[0].x--; break;
        case VK_RIGHT: snake[0].x++; break;
    }

    if (snake[0].x < 0 || snake[0].x >= WINDOW_WIDTH / CELL_SIZE ||
        snake[0].y < 0 || snake[0].y >= WINDOW_HEIGHT / CELL_SIZE)
    {
        GameOver();
    }

    for(int i = 1; i < snakeLength; i++)
    {
        if(snake[i].x == snake[0].x && snake[i].y == snake[0].y)
        {
            GameOver();
        }
    }

    if(snake[0].x == food.x && snake[0].y == food.y)
    {
        if(snakeLength < SNAKE_MAX_LENGTH)
        {
            snake[snakeLength++] = snake[snakeLength - 1];
        }

        score += 10;

        GenerateFood();
    }
}

void GenerateFood()
{
    bool isOnSnake;
    do {
        isOnSnake = false;
        food.x = rand() % (WINDOW_WIDTH / CELL_SIZE);
        food.y = rand() % (WINDOW_HEIGHT / CELL_SIZE);
        for (int i = 0; i < snakeLength; i++) {
            if (snake[i].x == food.x && snake[i].y == food.y) {
                isOnSnake = true;
                break;
            }
        }
    } while (isOnSnake);
}

void GameOver()
{
    KillTimer(hwnd, 1);
    if(score > highScore)
    {
        highScore = score;
        SaveHighScore(highScore);
    }

    DialogBox(hInstance, MAKEINTRESOURCE(IDD_GAMEOVER_DIALOG), hwnd, ShowDialog);
}

void DrawGame(HDC hdc)
{
    HDC hdcMem = CreateCompatibleDC(hdc);
    HBITMAP hOldBmp;

    if (hBackgroundBmp) {
        hOldBmp = (HBITMAP)SelectObject(hdcMem, hBackgroundBmp);
        BitBlt(hdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, hdcMem, 0 , 0, SRCCOPY);
        SelectObject(hdcMem, hOldBmp);
    } else {
        HandleError("Background bitmap is not loaded");
    }

    if (hSnakeBmp) {
        hOldBmp = (HBITMAP)SelectObject(hdcMem, hSnakeBmp);
        for (int i = 0; i < snakeLength; i++) {
            BitBlt(hdc, snake[i].x * CELL_SIZE, snake[i].y * CELL_SIZE, CELL_SIZE, CELL_SIZE, hdcMem, 0, 0, SRCCOPY);
        }
        SelectObject(hdcMem, hOldBmp);
    } else {
        HandleError("Snake bitmap is not loaded");
    }

    if (hFoodBmp) {
        hOldBmp = (HBITMAP)SelectObject(hdcMem, hFoodBmp);
        if (food.x >= 0 && food.x < WINDOW_WIDTH / CELL_SIZE && food.y >= 0 && food.y < WINDOW_HEIGHT / CELL_SIZE) {
            BitBlt(hdc, food.x * CELL_SIZE, food.y * CELL_SIZE, CELL_SIZE, CELL_SIZE, hdcMem, 0, 0, SRCCOPY);
        } else {
            HandleError("Food coordinates are out of bounds");
        }
        SelectObject(hdcMem, hOldBmp);
    } else {
        HandleError("Food bitmap is not loaded");
    }

    TCHAR scoreText[50];
    _stprintf(scoreText, _T("Score: %d"), score);
    SetTextColor(hdc, RGB(255, 255, 255));
    SetBkMode(hdc, TRANSPARENT);
    TextOut(hdc, 10, 10, _T(scoreText), _tcslen(scoreText));

    DeleteDC(hdcMem);
}

void LoadResources() {
    hSnakeBmp = (HBITMAP)LoadImage(hInstance, _T("snake.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    if (!hSnakeBmp) {
       HandleError("Failed to load snake bitmap");
    }

    hFoodBmp = (HBITMAP)LoadImage(hInstance, _T("food.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    if (!hFoodBmp) {
        HandleError("Failed to load food bitmap");
    }

    hBackgroundBmp = (HBITMAP)LoadImage(hInstance, _T("background.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    if (!hBackgroundBmp) {
        HandleError("Failed to load background bitmap");
    }
}

void UnloadResources() {
    if (hSnakeBmp) {
        DeleteObject(hSnakeBmp);
    }
    if (hFoodBmp) {
        DeleteObject(hFoodBmp);
    }
    if (hBackgroundBmp) {
        DeleteObject(hBackgroundBmp);
    }
}

void ShowHighScores()
{
    TCHAR highScoreText[50];
    _stprintf(highScoreText, _T("High Score: %d"), highScore);

    MessageBox(hwnd, highScoreText, _T("High Scores"), MB_OK | MB_ICONINFORMATION);
}

void HandleError(const char* message)
{
    DWORD error = GetLastError();
    TCHAR errorMsg[256];
    FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, error, 0, errorMsg, 256, NULL);
    MessageBox(hwnd, errorMsg, _T(message), MB_OK | MB_ICONERROR);
}