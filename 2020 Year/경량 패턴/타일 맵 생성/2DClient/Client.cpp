﻿// Client.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//
#include "stdafx.h"
#include "Client.h"
#include"World.h"
#include"Terrain.h"
#include"Player.h"
#define MAX_LOADSTRING	100
#define WINDWS_WIDTH	672
#define WINDWS_HEIGHT	714

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.
World* g_world;
Player* g_player;

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_CLIENT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);


    // 응용 프로그램 초기화를 수행합니다:


    if (!InitInstance(hInstance, nCmdShow)) {
        return FALSE;
    }



    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CLIENT));
    MSG msg;


    // 기본 메시지 루프입니다:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}


ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CLIENT));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_CLIENT);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, WINDWS_WIDTH, WINDWS_HEIGHT, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}



// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

    switch (message) {
    case WM_CREATE: {
        g_groundTexture = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_GROUND_TITLE));
        g_waterTexture = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_WATER_TITLE));
        g_ironTexture = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_IRON_TITLE));
        g_playerTexture = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_Pawn));
        g_world = new World();
        g_world->GenerateTerrain();
        g_player = new Player(&g_playerTexture);
        break;
    }
    case WM_COMMAND: {
        int wmId = LOWORD(wParam);
        // 메뉴 선택을 구문 분석합니다:
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
        break;
    }

    case WM_KEYDOWN: {
        KeyInput(wParam);
        InvalidateRgn(hWnd, NULL, FALSE);
        break;
    }
    case WM_PAINT: {

        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        HDC hMemDc;
        hMemDc = CreateCompatibleDC(hdc);

        //Draw tiles  map
        int textureWidth = 97 / 3;

        for (int x = 0; x < WIDTH; ++x) {
            for (int y = 0; y < HEIGHT; ++y) {

                Texture texture = g_world->GetTile(x, y).GetTexture();
                SelectObject(hMemDc, texture);
                int texturePointer{ 1 };
                int xpos{ x * textureWidth }, ypos{ y * textureWidth };

                BitBlt(hdc, xpos, ypos, 32, 34, hMemDc, 0, 0, SRCCOPY);
            }
        }

        //Draw Player
        Texture texture = g_player->GetPlayerTexture();
        SelectObject(hMemDc, texture);
        int xpos{}, ypos{};
        g_player->GetPlayerPosition(xpos, ypos);
        TransparentBlt(hdc, xpos* textureWidth, ypos* textureWidth, 32, 34, hMemDc, 0, 0, 37, 62, RGB(255, 255, 255));

        DeleteDC(hMemDc);
        EndPaint(hWnd, &ps);
        break;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}


void KeyInput(WPARAM key) {
    int x, y;
    bool isWater{ false };
    g_player->GetPlayerPosition(x, y);

    switch (key) {
    case VK_LEFT: {
        if (x > 0) {
            x -= 1;
            isWater = g_world->GetTile(x, y).IsWater();
        }

        break;
    }
    case VK_RIGHT: {
        if (x < WIDTH - 1) {
            x += 1;
            isWater = g_world->GetTile(x, y).IsWater();
        }
        break;
    }
    case VK_UP: {
        if (y > 0) {
            y -= 1;
            isWater = g_world->GetTile(x, y).IsWater();
        }
        break;
    }
    case VK_DOWN: {
        if (y < HEIGHT - 1) {
            y += 1;
            isWater = g_world->GetTile(x, y).IsWater();
        }
        break;
    }
    default:
        break;
    }

    if(isWater==false)
    g_player->SetPlayerPosition(x, y);
}

