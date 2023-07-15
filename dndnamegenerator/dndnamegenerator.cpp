// dndnamegenerator.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include <CommCtrl.h>
#include <windowsx.h>
#include "dndnamegenerator.h"
#include <iostream>
#include <string>
#include "namedatabase.h"
#include "utils.h"
#include <iostream>
#include <fstream>
#include <sstream>

void debugOutputToFile(std::string str) {
    std::ofstream myfile;
    myfile.open("debug.txt");
    myfile << str;
    myfile.close();
    return;
}
void debugOutputToFile(LPCWSTR str) {
    std::ofstream myfile;
    myfile.open("debug.txt");
    myfile << str;
    myfile.close();
    return;
}

#define MAX_LOADSTRING 100

#define FILE_MENU_DING 1
#define FILE_MENU_EXIT 2
#define FILE_MENU_CHANGETITLE 3
#define IDC_COMBOBOX_TEXT 1000
#define GWL_HINSTANCE -6
#define BUTTON_GENERATE_NAME 4

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

HMENU hMenu;
HWND hEdit;
HWND hNationalitySelector;
HWND hTypeSelector;
HWND hGenderSelector;
HWND hGenerate;
HWND hNameOutputField;
HWND hInfoField;
namedatabase nameDB;

void AddMenus(HWND);
void AddControls(HWND);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_DNDNAMEGENERATOR, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DNDNAMEGENERATOR));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DNDNAMEGENERATOR));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
        AddMenus(hWnd);
        AddControls(hWnd);
    case WM_COMMAND:
        switch (wParam) {
            case FILE_MENU_DING:
                MessageBox(hWnd, TEXT("Ding"), TEXT("ding"), MB_OK);
                MessageBeep(MB_OK);
                break;
            case FILE_MENU_EXIT:
                DestroyWindow(hWnd);
                break;
            case FILE_MENU_CHANGETITLE:
                wchar_t text[100];
                GetWindowTextW(hEdit,text,100);
                SetWindowTextW(hWnd, text);
                break;
            case BUTTON_GENERATE_NAME:
                wchar_t input_nationality[32], input_gender[32];
                GetWindowTextW(hNationalitySelector, input_nationality, 32);
                GetWindowTextW(hGenderSelector, input_gender, 32);
                std::string nationality = toStdString(input_nationality);
                std::string gender = toStdString(input_gender);
                std::string result = nameDB.generate(nationality, gender);
                if (nationality == "korean") {
                    std::string familyname;
                    std::istringstream namelist;
                    namelist.str(result);
                    std::getline(namelist, familyname, ' ');
                    std::string info = "In this Korean name, the family name is " + familyname + ".";
                    SetWindowTextA(hInfoField, (LPCSTR)info.c_str());
                }
                else {
                    std::string info = "";
                    SetWindowTextA(hInfoField, (LPCSTR)info.c_str());
                }
                debugOutputToFile(result);
                SetWindowTextA(hNameOutputField, (LPCSTR)result.c_str());
                break;
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            TCHAR title_message[] = _T("DnD Name Generator v0.1 by woodchuck321");
            TextOut(hdc,
                5, 5,
                title_message, _tcslen(title_message));
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
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

void AddMenus(HWND hWnd) {
    hMenu = CreateMenu();

    HMENU hFileMenu = CreateMenu();

    AppendMenu(hFileMenu, MF_STRING, FILE_MENU_CHANGETITLE, L"Change Title");
    AppendMenu(hFileMenu, MF_STRING, FILE_MENU_DING, L"Ding");
    AppendMenu(hFileMenu, MF_SEPARATOR, NULL, NULL);
    AppendMenu(hFileMenu, MF_STRING, FILE_MENU_EXIT, L"Exit");

    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hFileMenu , L"File");
    AppendMenu(hMenu, MF_STRING, 1, L"Help");

    SetMenu(hWnd, hMenu);
}

void AddControls(HWND hWnd) {
    
    // TEXT INPUT EXAMPLE
    //CreateWindowW(L"static", L"static field", WS_VISIBLE | WS_BORDER | SS_CENTER | WS_CHILD, 10, 30, 100, 20, hWnd, nullptr, nullptr, nullptr);
    //hEdit = CreateWindowW(L"edit", L"edit field", WS_VISIBLE | WS_BORDER | SS_LEFT | WS_CHILD, 10, 60, 100, 20, hWnd, nullptr, nullptr, nullptr);

    hNationalitySelector = CreateWindowW(L"combobox", TEXT("english"), WS_VISIBLE | WS_BORDER | WS_CHILD | CBS_HASSTRINGS | CBS_DROPDOWN | WS_VSCROLL, 10, 30, 100, 500, hWnd, nullptr, nullptr, nullptr);

    //ComboBox_AddString(hNationalitySelector, TEXT("random"));
    ComboBox_AddString(hNationalitySelector, TEXT("english"));
    ComboBox_AddString(hNationalitySelector, TEXT("french"));
    ComboBox_AddString(hNationalitySelector, TEXT("spanish"));
    ComboBox_AddString(hNationalitySelector, TEXT("russian"));
    ComboBox_AddString(hNationalitySelector, TEXT("italian"));
    ComboBox_AddString(hNationalitySelector, TEXT("scottish"));
    ComboBox_AddString(hNationalitySelector, TEXT("indian"));
    ComboBox_AddString(hNationalitySelector, TEXT("greek"));
    ComboBox_AddString(hNationalitySelector, TEXT("welsh"));
    ComboBox_AddString(hNationalitySelector, TEXT("armenian"));
    ComboBox_AddString(hNationalitySelector, TEXT("chinese"));
    ComboBox_AddString(hNationalitySelector, TEXT("korean"));
    ComboBox_AddString(hNationalitySelector, TEXT("cornish"));
    ComboBox_AddString(hNationalitySelector, TEXT("german"));
    ComboBox_AddString(hNationalitySelector, TEXT("dutch"));
    ComboBox_AddString(hNationalitySelector, TEXT("finnish"));
    ComboBox_AddString(hNationalitySelector, TEXT("hungarian"));
    ComboBox_AddString(hNationalitySelector, TEXT("japanese"));
    ComboBox_AddString(hNationalitySelector, TEXT("jewish"));
    ComboBox_AddString(hNationalitySelector, TEXT("norwegian"));
    ComboBox_AddString(hNationalitySelector, TEXT("polish"));
    ComboBox_AddString(hNationalitySelector, TEXT("portuguese"));
    ComboBox_AddString(hNationalitySelector, TEXT("romani"));

    ComboBox_SetCurSel(hNationalitySelector, 0);

    hGenderSelector = CreateWindowW(L"combobox", TEXT("any"), WS_VISIBLE | WS_BORDER | WS_CHILD | CBS_HASSTRINGS | CBS_DROPDOWN | WS_VSCROLL, 10, 60, 100, 500, hWnd, nullptr, nullptr, nullptr);

    ComboBox_AddString(hGenderSelector, TEXT("any"));
    ComboBox_AddString(hGenderSelector, TEXT("male"));
    ComboBox_AddString(hGenderSelector, TEXT("female"));

    ComboBox_SetCurSel(hGenderSelector, 0);


    hGenerate = CreateWindowW(L"button", TEXT("Generate"), WS_VISIBLE | WS_BORDER | WS_CHILD, 10, 90, 100, 50, hWnd, (HMENU)BUTTON_GENERATE_NAME, nullptr, nullptr);

    hNameOutputField = CreateWindowW(L"edit", nullptr, WS_VISIBLE | SS_LEFT | WS_CHILD, 10, 150, 300, 200, hWnd, nullptr, nullptr, nullptr);

    hInfoField = CreateWindowW(L"edit", nullptr, WS_VISIBLE | SS_LEFT | WS_CHILD, 120, 30, 600, 50, hWnd, nullptr, nullptr, nullptr);
}