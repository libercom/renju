#include <Windows.h>
#include <time.h>
#include "random.h"
#include "vector.h"
#include "wstring.h"

enum Directions
{
    UP = 0,
    RIGHT = 1,
    DOWN = 2,
    LEFT = 3
};

int main()
{
	HANDLE console = CreateConsoleScreenBuffer(GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(console);
	SetConsoleTitle(TEXT("Renju Game"));
    CONSOLE_FONT_INFOEX cfi = { sizeof(cfi) };
    cfi.dwFontSize.X = 36;
    cfi.dwFontSize.Y = 72;
    wstring_copy(cfi.FaceName, L"Consolas");
    SetCurrentConsoleFontEx(console, FALSE, &cfi);
    SetConsoleDisplayMode(console, CONSOLE_FULLSCREEN_MODE, 0);
    DWORD dwBytesWritten = 0;
    COORD window_size = GetLargestConsoleWindowSize(console);
    SHORT WIDTH = window_size.X;
    SHORT HEIGHT = window_size.Y;
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(console, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(console, &cursorInfo);
    LCGrand random = get_seed(time(NULL));
    POINT p;
    wchar_t* screen = (wchar_t*)calloc(HEIGHT * WIDTH, sizeof(wchar_t));
    int score = 0;

    for (int i = 0; i < WIDTH * HEIGHT; i++)
    {
        screen[i] = L' ';
    }

    bool clicked = false, esc_key = false;
    wsprintf(&screen[(HEIGHT / 2 - 1) * WIDTH + (WIDTH - 17) / 2], L"WELCOME TO RENJU");
    wsprintf(&screen[(HEIGHT / 2) * WIDTH + (WIDTH - 30) / 2], L"PRESS SPACE TO START THE GAME");
    WriteConsoleOutputCharacter(console, screen, WIDTH * HEIGHT, { 0, 0 }, &dwBytesWritten);

    while (!(GetKeyState(0x20) & 0x8000)) {
        esc_key = (GetKeyState(0x1B) & 0x8000);

        if (esc_key)
        {
            exit(0);
        }
    }

    while (1)
    {
        bool game_over = false, won = false;
        bool moved = true;
        int bot_x = get_random(&random, WIDTH);
        int bot_y = get_random(&random, HEIGHT);
        int new_x = bot_x, new_y = bot_y, x, y;
        struct vector moves;
        vector_initialize(&moves);
        vector_push(&moves, { bot_x, bot_y });

        for (int i = 0; i < WIDTH * HEIGHT; i++)
        {
            screen[i] = L'.';
        }

        screen[bot_y * WIDTH + bot_x] = L'O';

        while (!game_over)
        {
            esc_key = (GetKeyState(0x1B) & 0x8000);
            clicked = (GetKeyState(0x01) & 0x8000);

            if (esc_key)
            {
                exit(0);
            }

            GetCursorPos(&p);

            if (clicked)
            {
                x = p.x / 36;
                y = p.y / 72;
                
                if (screen[y * WIDTH + x] == L'.' && x != WIDTH)
                {
                    if (screen[y * WIDTH + x + 1] != L'.' || screen[y * WIDTH + x - 1] != L'.' || screen[(y + 1) * WIDTH + x] != L'.' ||
                        screen[(y - 1) * WIDTH + x] != L'.' || screen[(y + 1) * WIDTH + x + 1] != L'.' || screen[(y + 1) * WIDTH + x - 1] != L'.' ||
                        screen[(y - 1) * WIDTH + x + 1] != L'.' || screen[(y - 1) * WIDTH + x - 1] != L'.')
                    {
                        screen[y * WIDTH + x] = L'@';
                        vector_push(&moves, { x, y });
                        moved = false;
                    }
                }
            }

            int count = 1;
            
            if (!moved)
            {
                // Horizontal check if won

                for (int i = 1; i < 5; i++)
                {
                    if ((x + i) < WIDTH)
                    {
                        if (screen[y * WIDTH + x + i] == L'@')
                        {
                            count++;
                            if (count == 5)
                            {
                                won = true;
                                score++;
                                game_over = true;
                                break;
                            }
                        }
                        else
                        {
                            break;
                        }
                    }
                }

                for (int i = 1; i < 5; i++)
                {
                    if ((x - i) >= 0)
                    {
                        if (screen[y * WIDTH + x - i] == L'@')
                        {
                            count++;
                            if (count == 5)
                            {
                                won = true;
                                score++;
                                game_over = true;
                                break;
                            }
                        }
                        else
                        {
                            break;
                        }
                    }
                }

                count = 1;

                // Vertical check if it won

                for (int i = 1; i < 5; i++)
                {
                    if ((y + i) < HEIGHT)
                    {
                        if (screen[(y + i) * WIDTH + x] == L'@')
                        {
                            count++;
                            if (count == 5)
                            {
                                won = true;
                                score++;
                                game_over = true;
                                break;
                            }
                        }
                        else
                        {
                            break;
                        }
                    }
                }

                for (int i = 1; i < 5; i++)
                {
                    if ((y - i) >= 0)
                    {
                        if (screen[(y - i) * WIDTH + x] == L'@')
                        {
                            count++;
                            if (count == 5)
                            {
                                won = true;
                                score++;
                                game_over = true;
                                break;
                            }
                        }
                        else
                        {
                            break;
                        }
                    }
                }

                count = 1;

                // First diagonal check if it won

                for (int i = 1; i < 5; i++)
                {
                    if ((y + i) < HEIGHT && (x - i) >= 0)
                    {
                        if (screen[(y + i) * WIDTH + (x - i)] == L'@')
                        {
                            count++;
                            if (count == 5)
                            {
                                won = true;
                                score++;
                                game_over = true;
                                break;
                            }
                        }
                        else
                        {
                            break;
                        }
                    }
                }

                for (int i = 1; i < 5; i++)
                {
                    if ((y - i) >= 0 && (x + i) < WIDTH)
                    {
                        if (screen[(y - i) * WIDTH + (x + i)] == L'@')
                        {
                            count++;
                            if (count == 5)
                            {
                                won = true;
                                score++;
                                game_over = true;
                                break;
                            }
                        }
                        else
                        {
                            break;
                        }
                    }
                }

                count = 1;

                // Second diagonal check if it won

                for (int i = 1; i < 5; i++)
                {
                    if ((y - i) >= 0 && (x - i) >= 0)
                    {
                        if (screen[(y - i) * WIDTH + (x - i)] == L'@')
                        {
                            count++;
                            if (count == 5)
                            {
                                won = true;
                                score++;
                                game_over = true;
                                break;
                            }
                        }
                        else
                        {
                            break;
                        }
                    }
                }

                for (int i = 1; i < 5; i++)
                {
                    if ((y + i) < HEIGHT && (x + i) < WIDTH)
                    {
                        if (screen[(y + i) * WIDTH + (x + i)] == L'@')
                        {
                            count++;
                            if (count == 5)
                            {
                                won = true;
                                score++;
                                game_over = true;
                                break;
                            }
                        }
                        else
                        {
                            break;
                        }
                    }
                }
            }

            if (game_over)
            {
                WriteConsoleOutputCharacter(console, screen, WIDTH * HEIGHT, { 0, 0 }, &dwBytesWritten);
                break;
            }

            clock_t start_time = clock();
            while (clock() < start_time + 20);

            while (!moved)
            {
                int bot_direction = get_random(&random, 4);
                int i = (moves.size == 1) ? 0 : get_random(&random, moves.size);

                new_x = moves.arr[i].x;
                new_y = moves.arr[i].y;

                switch (bot_direction)
                {
                case UP:
                    if (new_y != 0)
                    {
                        new_y -= 1;
                    }
                    break;
                case RIGHT:
                    if (new_x + 1 != WIDTH)
                    {
                        new_x += 1;
                    }
                    break;
                case DOWN:
                    if (new_y + 1 != HEIGHT)
                    {
                        new_y += 1;
                    }
                    break;
                case LEFT:
                    if (new_x != 0)
                    {
                        new_x -= 1;
                    }
                }

                if (screen[new_y * WIDTH + new_x] == L'.')
                {
                    screen[new_y * WIDTH + new_x] = L'O';
                    vector_push(&moves, { new_x, new_y });
                    moved = true;
                }
            }

            int bot_count = 1;

            // Horizontal check if won

            for (int i = 1; i < 5; i++)
            {
                if ((new_x + i) < WIDTH)
                {
                    if (screen[new_y * WIDTH + new_x + i] == L'O')
                    {
                        bot_count++;
                        if (bot_count == 5)
                        {
                            won = false;
                            game_over = true;
                            break;
                        }
                    }
                    else
                    {
                        break;
                    }
                }
            }

            for (int i = 1; i < 5; i++)
            {
                if ((new_x - i) >= 0)
                {
                    if (screen[new_y * WIDTH + new_x - i] == L'O')
                    {
                        bot_count++;
                        if (bot_count == 5)
                        {
                            won = false;
                            game_over = true;
                            break;
                        }
                    }
                    else
                    {
                        break;
                    }
                }
            }

            bot_count = 1;

            // Vertical check if it won

            for (int i = 1; i < 5; i++)
            {
                if ((new_y + i) < HEIGHT)
                {
                    if (screen[(new_y + i) * WIDTH + new_x] == L'O')
                    {
                        bot_count++;
                        if (bot_count == 5)
                        {
                            won = false;
                            game_over = true;
                            break;
                        }
                    }
                    else
                    {
                        break;
                    }
                }
            }

            for (int i = 1; i < 5; i++)
            {
                if ((new_y - i) >= 0)
                {
                    if (screen[(new_y - i) * WIDTH + new_x] == L'O')
                    {
                        bot_count++;
                        if (bot_count == 5)
                        {
                            won = false;
                            game_over = true;
                            break;
                        }
                    }
                    else
                    {
                        break;
                    }
                }
            }

            bot_count = 1;

            // First diagonal check if it won

            for (int i = 1; i < 5; i++)
            {
                if ((new_y + i) < HEIGHT && (new_x - i) >= 0)
                {
                    if (screen[(new_y + i) * WIDTH + (new_x - i)] == L'O')
                    {
                        bot_count++;
                        if (bot_count == 5)
                        {
                            won = false;
                            game_over = true;
                            break;
                        }
                    }
                    else
                    {
                        break;
                    }
                }
            }

            for (int i = 1; i < 5; i++)
            {
                if ((new_y - i) >= 0 && (new_x + i) < WIDTH)
                {
                    if (screen[(new_y - i) * WIDTH + (new_x + i)] == L'O')
                    {
                        bot_count++;
                        if (bot_count == 5)
                        {
                            won = false;
                            game_over = true;
                            break;
                        }
                    }
                    else
                    {
                        break;
                    }
                }
            }

            bot_count = 1;

            // Second diagonal check if it won

            for (int i = 1; i < 5; i++)
            {
                if ((new_y - i) >= 0 && (new_x - i) >= 0)
                {
                    if (screen[(new_y - i) * WIDTH + (new_x - i)] == L'O')
                    {
                        bot_count++;
                        if (bot_count == 5)
                        {
                            won = false;
                            game_over = true;
                            break;
                        }
                    }
                    else
                    {
                        break;
                    }
                }
            }

            for (int i = 1; i < 5; i++)
            {
                if ((new_y + i) < HEIGHT && (new_x + i) < WIDTH)
                {
                    if (screen[(new_y + i) * WIDTH + (new_x + i)] == L'O')
                    {
                        bot_count++;
                        if (bot_count == 5)
                        {
                            won = false;
                            game_over = true;
                            break;
                        }
                    }
                    else
                    {
                        break;
                    }
                }
            }
                
            WriteConsoleOutputCharacter(console, screen, WIDTH * HEIGHT, { 0, 0 }, &dwBytesWritten);
        }

        clock_t start_time = clock();
        while (clock() < start_time + 1000);

        for (int i = 0; i < WIDTH * HEIGHT; i++)
        {
            screen[i] = L' ';
        }

        wsprintf(&screen[(HEIGHT / 2 - 1) * WIDTH + (WIDTH - 27) / 2], (won) ? L"YOU WON. YOUR SCORE IS %d" : L"YOU LOST. YOUR SCORE IS %d", score);
        wsprintf(&screen[(HEIGHT / 2) * WIDTH + (WIDTH - 26) / 2], L"PRESS SPACE TO PLAY AGAIN");
        WriteConsoleOutputCharacter(console, screen, WIDTH * HEIGHT, { 0, 0 }, & dwBytesWritten);

        while (!(0x8000 & GetKeyState(0x20)))
        {
            esc_key = (GetKeyState(0x1B) & 0x8000);

            if (esc_key)
            {
                exit(0);
            }
        }
    }

	return 0;
}