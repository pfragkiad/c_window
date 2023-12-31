#include <windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void myMessage(TCHAR *msg)
{
    MessageBox(NULL, msg, TEXT("Information"), MB_ICONINFORMATION | MB_OK);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    HWND hwnd;
    MSG Msg;
    WNDCLASS wc = {0};

    wc.lpszClassName = TEXT("Window");
    wc.hInstance = hInstance;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpfnWndProc = WndProc;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    if (!RegisterClass(&wc))
    {
        MessageBox(NULL, TEXT("Window Registration Failed!"), TEXT("Error"), MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    hwnd = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("Window"), TEXT("My Window"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 240, 120, NULL, NULL, hInstance, NULL);

    if (hwnd == NULL)
    {
        MessageBox(NULL, TEXT("Window Creation Failed!"), TEXT("Error"), MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    while (GetMessage(&Msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }

    return Msg.wParam;
}

HWND addCheckbox(TCHAR *text, HWND parent, int x, int y, int w, int h)
{
    return CreateWindow(TEXT("button"), text, WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, x, y, w, h,
                        parent, NULL, NULL, NULL);
}

void setCheckboxFont(HFONT hFont, HWND hCheckBox)
{
    SendMessage(hCheckBox, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE, 0));
}

HFONT createFont()
{
    return CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"));
}

BOOL isCheckboxChecked(HWND hCheckBox)
{
    LRESULT checkState = SendMessage(hCheckBox, BM_GETCHECK, 0, 0);
    // BOOL isChecked = (checkState == BST_CHECKED);
    return checkState == BST_CHECKED;
}

HWND hCheckBox = NULL;
HWND hButton2 = NULL;

HFONT hFont = NULL;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CREATE:
        // classname, window name, style, x, y, w, h, handle_parent, hmenu, hinstance,  param
        hCheckBox = addCheckbox("Check v", hwnd, 10, 10, 80, 25);

        hFont = createFont();
        if (hFont)
            setCheckboxFont(hFont, hCheckBox);

        hButton2 = CreateWindow(TEXT("button"), TEXT("Cancel"), WS_VISIBLE | WS_CHILD, 100, 10, 80, 25, hwnd, (HMENU)2, NULL, NULL);
        break;
    case WM_COMMAND:
        // HMENU hMenu = (HMENU)LOWORD(wParam); // hMenu not allowed in switch statement
        // switch (LOWORD(wParam))
        // {
        // case (HMENU)1: // this is allowed here
        // {
        //     myMessage(TEXT("Button OK Clicked!"));
        //     break;
        // }
        // case 2:
        //     myMessage(TEXT("Button CANCEL Clicked!"));
        //     break;
        // }

        BOOL isClickPressed = HIWORD(wParam) == BN_CLICKED;

        if (isClickPressed)
        {
            HWND handle = (HWND)lParam;
            // if (handle == hCheckBox)
            //     ; // myMessage("B1!");
            // else
            if (handle == hButton2)
                myMessage(TEXT((isCheckboxChecked(hCheckBox) ? "THIS IS CHECKED!" : "THIS IS UNCHECKED!")));
        }

        break;

    case WM_DESTROY:
        if (hFont)
            DeleteObject(hFont);

        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }

    return 0;
}
