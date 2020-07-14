//int main(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR atgs, int ncmdshow)
#include<windows.h>
#include<stdio.h>
#include <tchar.h>
#include <WinInet.h> 
#include <Windows.h>
#include <Sensapi.h>
#include <stdbool.h>
#include<CommCtrl.h>
#include<WinUser.h>
int fuse = 0;
int loop = 0;
void DibFileInitialize(HWND hwnd);
BOOL DibFileOpenDlg(HWND hwnd, PTSTR pstrFileName, PTSTR pstrTitleName);
BOOL DibFileSaveDlg(HWND hwnd, PTSTR pstrFileName, PTSTR pstrTitleName);
BITMAPFILEHEADER* DibLoadImage(PTSTR pstrFileName);
BITMAPFILEHEADER * DibLoadImage (PTSTR pstrFileName)
{
     BOOL               bSuccess ;
     DWORD              dwFileSize, dwHighSize, dwBytesRead ;
     HANDLE             hFile ;
     BITMAPFILEHEADER * pbmfh ;

     hFile = CreateFile (pstrFileName, GENERIC_READ, FILE_SHARE_READ, NULL,
                         OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL) ;

     if (hFile == INVALID_HANDLE_VALUE)
          return NULL ;

     dwFileSize = GetFileSize (hFile, &dwHighSize) ;

     if (dwHighSize)
     {
          CloseHandle (hFile) ;
          return NULL ;
     }

     pbmfh = malloc (dwFileSize) ;

     if (!pbmfh)
     {
          CloseHandle (hFile) ;
          return NULL ;
     }

     bSuccess = ReadFile (hFile, pbmfh, dwFileSize, &dwBytesRead, NULL) ;
     CloseHandle (hFile) ;

     if (!bSuccess || (dwBytesRead != dwFileSize)         
                   || (pbmfh->bfType != * (WORD *) "BM") 
                   || (pbmfh->bfSize != dwFileSize))
     {
          free (pbmfh) ;
          return NULL ;
     }
     return pbmfh ;
}
BOOL DibSaveImage(PTSTR pstrFileName, BITMAPFILEHEADER*);
#define CRTDBG_MAP_ALLOC 
#include <crtdbg.h>
#pragma comment(lib, "Sensapi.lib")
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#define ID_EDIT 1
#define ID_BUTTON 2
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"") 
#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#pragma comment( lib, "gdiplus.lib" ) 

LRESULT CALLBACK WinProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WinProczerotwo(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//#pragma comment(lib, "ole32.lib")


int checknet()
{
    DWORD dwsens;
    if (IsNetworkAlive(&dwsens) == FALSE)
    {
        askconnectnet();
        return 3;
    }
    else
    {
        return 1;
    }
    
}

int checkernet()
{
    DWORD dwsens;
    if (IsNetworkAlive(&dwsens) == FALSE)
    {

        return 0;
    }
    if (IsNetworkAlive(&dwsens) == TRUE)

    {
        return 1;
    }

}

int askconnectnet()
{
    int msgboxID = MessageBox(NULL, TEXT("PLEASE HAVE A WORKING CONNECTION"), TEXT("Aamshotto"), 0 | MB_ICONWARNING);
    switch (msgboxID)
    {

    case IDOK:
            break;
    default:
        break;
    }
    return 0;
}
int DisplayResourceNAMessageBox()
{
    int msgboxID = MessageBox(
        NULL,
        (LPCWSTR)L"Is your internet working?",
        (LPCWSTR)L"Aamshotto",
        MB_ICONQUESTION | MB_YESNOCANCEL
    );
    switch (msgboxID)
    {
    case IDYES:
        if (checknet() == 1)
        {
            return 5;
        }
        else

        break;
    case IDNO:
        askconnectnet();
        break;
    case IDCANCEL:

        break;
    }

    return msgboxID;
}

int readmessagebox()

{
    int msgboxID = MessageBox(
        NULL,
        (LPCWSTR)L"Do you want to download and read the Manga?",
        (LPCWSTR)L"Aamshotto",
        MB_ICONQUESTION | MB_CANCELTRYCONTINUE);
    switch (msgboxID)
    {

    default:
        break;
    }
    return 0;
}

int createmainwindow(HINSTANCE hinstan, int nCmdShow)

{
    const wchar_t CLASS_NAME[] = L"Sample Window Class";
    WNDCLASSW wc = {0};
    wc.lpfnWndProc = WinProc;
    wc.hInstance = hinstan;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(NULL,IDC_HAND);
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    HBRUSH colo = CreateSolidBrush(RGB(0, 0, 0));
    wc.hbrBackground = colo;
    RegisterClass(&wc);
    HWND hwnd=CreateWindowEx(
        0,                              // Optional window styles.
        CLASS_NAME,                     // Window class
        L"AAMSHOTTO",    // Window text
        WS_OVERLAPPEDWINDOW,            // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

        NULL,       // Parent window    
        NULL,       // Menu
        hinstan,  // Instance handle
        NULL        // Additional application data

    );

    ShowWindow(hwnd, 3);
    int io = checkernet();
    int* pi = &io;
    HWND mgrchecker = CreateWindowExW(0, PROGRESS_CLASS, TEXT("Is Mangareader Working"),
        WS_VISIBLE | WS_CHILD | PBS_MARQUEE | WS_OVERLAPPED,
         600, 50, 200, 20, hwnd, (HMENU)0, NULL, NULL);

    SendMessage(mgrchecker, (UINT)PBM_SETMARQUEE, io, 10);
    ShowWindow(mgrchecker, 1);
    UpdateWindow(mgrchecker);

    if (hwnd == NULL)
    {
        return 0;
    }

    MSG msg;
    while (GetMessageW(&msg, NULL, 0, 0))
    {
            loop = loop + 1;
            TranslateMessage(&msg);
            if (loop % 200 == 0)
            {   
                int io = checkernet();
                SendMessage(mgrchecker, (UINT)PBM_SETMARQUEE, io, 10);
                ShowWindow(mgrchecker, 1);
                UpdateWindow(mgrchecker);
            }
            DispatchMessage(&msg);
            UpdateWindow(hwnd);
    }
   
    return 0;
}

int main(HINSTANCE hinstance, HINSTANCE nully, LPSTR atgs, int ncmdshow)
{
    
    int a= DisplayResourceNAMessageBox();
    if (a == 5)
    {     
        createmainwindow(hinstance, ncmdshow);
    }
    return 0;

}

LRESULT CALLBACK WinProc(HWND hwnd, UINT msg,
    WPARAM wParam, LPARAM lParam) {
    int loop = 0;
    static HWND hwndEdit;
    static HWND hwndButton;
    static HWND hwndManga;
    static HWND  hwndchapno;
    static  HWND hwndButtontwo;
    

    LPCSTR A = "EDIT";
    LPCSTR B = "ENTER THE MANGA NAME";
    LPCWSTR null = NULL;
    int io = checkernet();
    int* pi = &io;

    static BITMAPFILEHEADER* pbmfh[2];
    static BITMAPINFO* pbmi[2];
    static BYTE* pBits[2];
    static int cxClient, cyClient, cxDib[2], cyDib[2];
    HDC hdc;
    PAINTSTRUCT ps;

    static BITMAPFILEHEADER* pbmfha[2];
    static BITMAPINFO* pbmia[2];
    static BYTE* pBitsa[2];
    static int cxClienta, cyClienta, cxDiba[2], cyDiba[2];
  
    switch (msg) {
    case (WM_CREATE):


        hwndEdit = CreateWindowExA(WS_EX_WINDOWEDGE,A, B,
            WS_CHILD | WS_VISIBLE| WS_BORDER,
            50, 50, 500, 40, hwnd, (HMENU) ID_EDIT,NULL,
            NULL);

        hwndButton = CreateWindowW(L"button", L"SEARCH",
            WS_VISIBLE | WS_CHILD, 50, 100, 100, 40,
            hwnd, (HMENU)ID_BUTTON, NULL, NULL);
        //hwndchapno = CreateWindowW( L"Edit", L"0", WS_CHILD | WS_VISIBLE| WS_EX_WINDOWEDGE, 1350, 50, 50, 30, hwnd, (HMENU)ID_EDIT, NULL, NULL);
        //hwndButtontwo = CreateWindowW(L"button", L"DOWNLOAD",
          //  WS_VISIBLE | WS_CHILD, 1000, 100, 300, 40,
           // hwnd, (HMENU)ID_BUTTON, NULL, NULL);
        hwndButtontwo = CreateWindowW(L"button", L"DOWNLOAD",
            WS_DISABLED | WS_CHILD, 1000, 100, 300, 40,
            hwnd, (HMENU)ID_BUTTON, NULL, NULL);
    case WM_PAINT :
        {      
        
        pbmfh[0] = DibLoadImage(L"darling.bmp");
        //pbmfh[0] = DibLoadImage(L"C:\\test\\zerotwo.bmp");
        pbmi[0] = (BITMAPINFO*)(pbmfh[0] + 1);
        pBits[0] = (BYTE*)pbmfh[0] + pbmfh[0]->bfOffBits;
        cxDib[0] = pbmi[0]->bmiHeader.biWidth;
        cyDib[0] = pbmi[0]->bmiHeader.biHeight;
           
            printf("1  PAINT 1");
            pbmfha[0] = DibLoadImage(L"C:\\test\\sample1.bmp");
            pbmia[0] = (BITMAPINFO*)(pbmfha[0] + 1);
            pBitsa[0] = (BYTE*)pbmfha[0] + pbmfha[0]->bfOffBits;
            cxDiba[0] = pbmia[0]->bmiHeader.biWidth;
            cyDiba[0] = pbmia[0]->bmiHeader.biHeight;
            hdc = BeginPaint(hwnd, &ps);
            SetDIBitsToDevice(hdc, 1000, cyClient / 4, cxDib[0], cyDib[0], 0, 0, 0, cyDib[0], pBits[0], pbmi[0], DIB_RGB_COLORS);

            printf("2 PAINT 2");

            SetDIBitsToDevice(hdc, 200, 100, cxDiba[0], cyDiba[0], 0, 0, 0, cyDiba[0], pBitsa[0], pbmia[0], DIB_RGB_COLORS);

            EndPaint(hwnd, &ps);
           
            return 0;
        }
    case WM_COMMAND:


        if (HIWORD(wParam) == BN_CLICKED)

        
        {
            if ((HWND)lParam == hwndButton)
            {  
                int len = GetWindowTextLengthW(hwndEdit) + 1;

                wchar_t text[200];

                GetWindowTextW(hwndEdit, text, len);
                SetWindowTextW(hwnd, text);
                hwndManga = CreateWindowExW(WS_EX_STATICEDGE, L"Static", (LPCWSTR)text, WS_CHILD | WS_VISIBLE, 1000, 50, 300, 30, hwnd, NULL, NULL, NULL);
                hwndchapno = CreateWindowW(L"Edit", L"0", WS_CHILD | WS_VISIBLE | WS_EX_WINDOWEDGE, 1350, 50, 50, 30, hwnd, (HMENU)ID_EDIT, NULL, NULL);
                int ap = IsWindowEnabled(hwndButtontwo);

                printf("%ls enable %d **", text, ap);
                EnableWindow(hwndButtontwo, TRUE);
                ShowWindow(hwndButtontwo, 1);
                FILE* fp;
                fp = fopen("c://test//aamshotto.txt", "w");
                fprintf(fp, "%ls", text);
                fclose(fp);
                STARTUPINFO si;
                PROCESS_INFORMATION pi;
                ZeroMemory(&si, sizeof(si));
                si.cb = sizeof(si);
                ZeroMemory(&pi, sizeof(pi));

                CreateProcess(
                    L"C:\\Users\\Shubhashish PC\\PycharmProjects\\aamshotto\\dist\\test\\test.exe",   // No module name (use command line)
                    NULL,        // Command line
                    NULL,           // Process handle not inheritable
                    NULL,           // Thread handle not inheritable
                    FALSE,          // Set handle inheritance to FALSE
                    0,              // No creation flags
                    NULL,           // Use parent's environment block
                    NULL,           // Use parent's starting directory 
                    &si,            // Pointer to STARTUPINFO structure
                    &pi);

                WaitForSingleObject(pi.hProcess, 20000);
                //Sleep(5000);
                CloseHandle(pi.hProcess);
                CloseHandle(pi.hThread);

                fuse = 1;

                printf("target");




                RedrawWindow(hwnd, 0, 0, RDW_ERASE | RDW_ERASENOW | RDW_INVALIDATE | RDW_UPDATENOW);
                readmessagebox();
            }
            if ((HWND)lParam == hwndButtontwo)
            {  


                int leno = GetWindowTextLengthW(hwndchapno) + 1;

                wchar_t texto[200];

                GetWindowTextW(hwndchapno, texto, leno);

                FILE* fpo;
                fpo = fopen("c://test//chapno.txt", "w");
                fprintf(fpo, "%ls", texto);
                fclose(fpo);
                STARTUPINFO sio;
                PROCESS_INFORMATION pio;
                ZeroMemory(&sio, sizeof(sio));
                sio.cb = sizeof(sio);
                ZeroMemory(&pio, sizeof(pio));

                CreateProcess(
                    L"C:\\Users\\Shubhashish PC\\PycharmProjects\\aamshotto\\dist\\download\\download.exe",   // No module name (use command line)
                    NULL,        // Command line
                    NULL,           // Process handle not inheritable
                    NULL,           // Thread handle not inheritable
                    FALSE,          // Set handle inheritance to FALSE
                    0,              // No creation flags
                    NULL,           // Use parent's environment block
                    NULL,           // Use parent's starting directory 
                    &sio,            // Pointer to STARTUPINFO structure
                    &pio);

                WaitForSingleObject(pio.hProcess, 20000);
                //Sleep(5000);
                CloseHandle(pio.hProcess);
                CloseHandle(pio.hThread);
                printf("************TURTURUUUUUUUUUUUUU***********");
            }
        }

        break;

    case WM_DESTROY:
        fuse = 0;
        if (pbmfha[0])
        {
            free(pbmfha[0]);
        }
        if (pbmfh[0])
        {
            free(pbmfh[0]);
        }
        PostQuitMessage(0);
        break;
    }
    return DefWindowProcW(hwnd, msg, wParam, lParam);
}
