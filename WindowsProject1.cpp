#include "WindowsProject1.h"
#include "Movable.h"

#define MAX_LOADSTRING 100

TCHAR szTitle[MAX_LOADSTRING];
TCHAR szWindowClass[MAX_LOADSTRING];

HINSTANCE hInst;
HBITMAP objBitmap;
HICON objIcon;
int winWidth, winHeight;
Movable* movable;

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg,
    WPARAM wParam, LPARAM lParam);


int APIENTRY _tWinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR lpCmdLine,
    _In_ int nCmdShow)
{
    HWND hWnd;
    MSG lpMsg;
    WNDCLASSEX wcex;

    LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadString(hInstance, IDC_WINDOWSPROJECT1, szWindowClass, MAX_LOADSTRING);

    // Заполняем структуру класса окна
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSPROJECT1));
    wcex.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL));
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = GetSysColorBrush(COLOR_BACKGROUND);
    wcex.lpszMenuName = MAKEINTRESOURCE(IDC_WINDOWSPROJECT1);
    wcex.lpszClassName = szWindowClass;
    

    // Регистрируем класс окна
    if (!RegisterClassEx(&wcex))
    {
        MessageBox(NULL, _T("Не удалось зарегестрировать класс окна!"), _T("Ошибка"), MB_OK);
        return 0;
    }

    hInst = hInstance;
    // Создаем основное окно приложения
    hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

    if (!hWnd)
    {
        MessageBox(NULL, _T("Не удалось создать главное окно!"), _T("Ошибка"), MB_OK);
        return 0;
    }

    // Показываем наше окно
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);


    // Выполняем цикл обработки сообщений до закрытия приложения
    while (GetMessage(&lpMsg, NULL, 0, 0))
    {
        TranslateMessage(&lpMsg);
        DispatchMessage(&lpMsg);
    }

    return (int)lpMsg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg,
    WPARAM wParam, LPARAM lParam)
{
    //Цикл обработки сообщений
    switch (msg)
    {
        case WM_CREATE: {
            movable = new Movable();
            objIcon = (HICON)LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_ICON2), IMAGE_ICON, movable->width, movable->height, LR_LOADTRANSPARENT);
            SetTimer(hWnd, 1, 1000 / 60, NULL);
        }
        case WM_PAINT: {
            BITMAP bitmap;
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);

            if (objIcon) {
                DrawIconEx(hdc, movable->posX, movable->posY, objIcon, movable->width, movable->height, 0, NULL, DI_NORMAL);
            }
            else {
                Rectangle(hdc, movable->posX, movable->posY, movable->width, movable->height);
                //Ellipse(hdc, movable->posX, movable->posY, movable->posX + movable->width, movable->posY + movable->height);
            }

            EndPaint(hWnd, &ps);

            break;
        }
        case WM_SIZE: {
            winWidth = LOWORD(lParam);
            winHeight = HIWORD(lParam);
            if (movable->posX + movable->width > winWidth) {
                movable->posX = winWidth - movable->width;
            }
            if (movable->posY + movable->height > winHeight) {
                movable->posY = winHeight - movable->height;
            }
            InvalidateRgn(hWnd, NULL, true);

            break;
        }
        case WM_KEYDOWN: {
            switch (wParam)
            {
                case VK_A:
                case VK_LEFT: {
                    movable->SetMove(MoveDirection::Left);
                    break;
                }
                case VK_D:
                case (VK_RIGHT): {
                    movable->SetMove(MoveDirection::Right);
                    break;
                }
                case VK_W:
                case (VK_UP): {
                    movable->SetMove(MoveDirection::Up);
                    break;
                }
                case VK_S:
                case (VK_DOWN): {
                    movable->SetMove(MoveDirection::Down);
                    break;
                }
            }
            break;
        }
        case WM_KEYUP: {
            switch (wParam)
            {
                case VK_A:
                case VK_LEFT: {
                    movable->UnsetMove(MoveDirection::Left);
                    movable->xDirection = 1;
                    movable->xSpeed = 0;
                    break;
                }
                case VK_D:
                case (VK_RIGHT): {
                    movable->UnsetMove(MoveDirection::Right);
                    movable->xDirection = 1;
                    movable->xSpeed = 0;
                    break;
                }
                case VK_W:
                case (VK_UP): {
                    movable->UnsetMove(MoveDirection::Up);
                    movable->yDirection = 1;
                    movable->ySpeed = 0;
                    break;
                }
                case VK_S:
                case (VK_DOWN): {
                    movable->UnsetMove(MoveDirection::Down);
                    movable->yDirection = 1;
                    movable->ySpeed = 0;
                    break;
                }
            }
            break;
        }
        case WM_MOUSEWHEEL: {
            if (GetKeyState(VK_SHIFT) < 0) {
                movable->posX += GET_WHEEL_DELTA_WPARAM(wParam) / 60;
                if (movable->posX < 0) {
                    movable->posX = 0;
                }
                else if (movable->posX + movable->width > winWidth) {
                    movable->posX = winWidth - movable->width;
                }
            }
            else {
                movable->posY -= GET_WHEEL_DELTA_WPARAM(wParam) / 60;
                if (movable->posY < 0) {
                    movable->posY = 0;
                }
                else if (movable->posY + movable->height > winHeight) {
                    movable->posY = winHeight - movable->height;
                }
            }

            InvalidateRect(hWnd, NULL, true);
            break;
        }
        case WM_LBUTTONDOWN: {
            POINT mousePos;

            GetCursorPos(&mousePos);
            ScreenToClient(hWnd, &mousePos);
            if (mousePos.x >= movable->posX && mousePos.y >= movable->posY &&
                mousePos.x <= movable->posX + movable->width && mousePos.y <= movable->posY + movable->height) {
                SetCapture(hWnd);
                movable->isMouseMoving = true;
            }
            break;
        }
        case WM_LBUTTONUP: {
            if (movable->isMouseMoving) {
                ReleaseCapture();
                movable->isMouseMoving = false;
            }
            break;
        }
        case WM_TIMER: {
            switch (wParam) {
                case (1): {
                    int currentX = movable->posX;
                    int currentY = movable->posY;
                    if (movable->isMouseMoving) {
                        POINT mousePos;
                        GetCursorPos(&mousePos);
                        ScreenToClient(hWnd, &mousePos);

                        if (mousePos.x < 0) {
                            movable->posX = -mousePos.x % winWidth;
                        }
                        else if (mousePos.x + movable->width > winWidth) {
                            movable->posX = winWidth - (mousePos.x + 2 * movable->width - winWidth) % winWidth;
                        }
                        else {
                            movable->posX = mousePos.x;
                        }
                        if (mousePos.y < 0) {
                            movable->posY = -mousePos.y % winHeight;
                        }
                        else if (mousePos.y + movable->height > winHeight) {
                            movable->posY = winHeight - (mousePos.y + 2 * movable->height - winHeight) % winHeight;
                        }
                        else {
                            movable->posY = mousePos.y;
                        }
                    }
                    else {
                        if (movable->CheckIfMove(MoveDirection::Left)) {
                            if (movable->xSpeed < 60) {
                                movable->xSpeed+=0.5;
                            }

                            movable->posX -= movable->xDirection * movable->xSpeed;

                            if (movable->posX < 0) {
                                movable->posX = 0;
                                movable->xDirection = -1;
                            }
                            else if (movable->posX + movable->width > winWidth) {
                                movable->posX = winWidth - movable->width;
                                movable->xDirection = 1;
                            }
                        }
                        if (movable->CheckIfMove(MoveDirection::Right)) {
                            if (movable->xSpeed < 60) {
                                movable->xSpeed += 0.5;
                            }

                            movable->posX += movable->xDirection * movable->xSpeed;

                            if (movable->posX < 0) {
                                movable->posX = 0;
                                movable->xDirection = 1;
                            }
                            else if (movable->posX + movable->width > winWidth) {
                                movable->posX = winWidth - movable->width;
                                movable->xDirection = -1;
                            }

                        }
                        if (movable->CheckIfMove(MoveDirection::Up)) {
                            if (movable->ySpeed < 60) {
                                movable->ySpeed += 0.5;
                            }

                            movable->posY -= movable->yDirection * movable->ySpeed;

                            if (movable->posY < 0)
                            {
                                movable->posY = 0;
                                movable->yDirection = -1;
                            }
                            else if (movable->posY + movable->height > winHeight) {
                                movable->posY = winHeight - movable->height;
                                movable->yDirection = 1;
                            }
                        }
                        if (movable->CheckIfMove(MoveDirection::Down)) {
                            if (movable->ySpeed < 60) {
                                movable->ySpeed += 0.5;
                            }

                            movable->posY += movable->yDirection * movable->ySpeed;

                            if (movable->posY < 0)
                            {
                                movable->posY = 0;
                                movable->yDirection = 1;
                            }
                            else if (movable->posY + movable->height > winHeight) {
                                movable->posY = winHeight - movable->height;
                                movable->yDirection = -1;
                            }
                        }
                    }

                    if (currentX != movable->posX || currentY != movable->posY) {
                        InvalidateRect(hWnd, NULL, true);
                    }
                    break;
                }
            }
            break;
        }
        case WM_CLOSE: {
            if (MessageBox(hWnd, _T("Вы действительно хотите закрыть приложение?"), _T("Подтверждение"), MB_YESNO | MB_APPLMODAL) == IDYES) {
                DestroyIcon(objIcon);
                DestroyWindow(hWnd);
            }
            return 0;
        }
        case WM_DESTROY: {
            KillTimer(hWnd, 1);
            PostQuitMessage(0);
            break;
        }
        default: {
            return DefWindowProc(hWnd, msg, wParam, lParam);
        }
    }
    return 0;
}
