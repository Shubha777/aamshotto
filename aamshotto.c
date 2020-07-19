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
#include<time.h>
#include <shobjidl.h> 
#include <string.h>
#include<conio.h>

int fuse = 0;
int loop = 0;

void DibFileInitialize(HWND hwnd);
BOOL DibFileOpenDlg(HWND hwnd, PTSTR pstrFileName, PTSTR pstrTitleName);
BOOL DibFileSaveDlg(HWND hwnd, PTSTR pstrFileName, PTSTR pstrTitleName);
LRESULT CALLBACK MyBitmapWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
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
     //bfh.bfType='MB';

     if (!bSuccess || (dwBytesRead != dwFileSize)         
                   //|| (pbmfh->bfType != *(WORD*)"BM")
         || (pbmfh->bfType != 'MB')
     
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
LRESULT CALLBACK WinreaderProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HWND CreateAHorizontalScrollBar(HWND hwndParent, int sbHeight);
LRESULT CALLBACK MyTextWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK MyBitmapWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);



const wchar_t* fileopen(HWND hwndle)
{
    OPENFILENAME ofn;       
    static wchar_t szFile[100];       
    HWND hwnd = hwndle;              

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwndle;
    ofn.lpstrFile = szFile;

    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof(szFile);
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = L"C:/manga";
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
    
    GetOpenFileName(&ofn);
    size_t len = wcslen(szFile);

    int i = 0;
    while (szFile[i] != '\0')
    {
        //printf("%c \n", (char)szFile[i]);
        if (szFile[i] == 92)
        {
            szFile[i] = 47;
        }
        i++;
    }
   // printf("%ls   * %ls  * %d",szFile, ofn.lpstrFile,len);
    const wchar_t *filename = szFile;
    printf("\n %ls   * %ls  * %d  \n", szFile, ofn.lpstrFile, len);
    return filename;
}


int checknet()
{
    DWORD dwsens;
    if (IsNetworkAlive(&dwsens) == FALSE)
    {
        int res = askconnectnet();
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
    HWND mgrchecker = CreateWindowExW(0,
        PROGRESS_CLASS,
        TEXT("Is Mangareader Working"),

        WS_VISIBLE | WS_CHILD | PBS_MARQUEE | WS_OVERLAPPED,

        600, 50, 200, 20, 
        hwnd, 
        (HMENU)0, 
        NULL,
        NULL);

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


LRESULT CALLBACK WinProc(HWND hwnd, UINT msg,
    WPARAM wParam, LPARAM lParam) {
    int loop = 0;
    static HWND hwndEdit;
    static HWND hwndButton;
    static HWND hwndManga;
    static HWND  hwndchapno;
    static  HWND hwndButtontwo;
    static HWND mangadownloading;
    static HWND hwndButtonread;
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

    switch (msg) {;
    case (WM_CREATE):


        hwndEdit = CreateWindowExA(WS_EX_WINDOWEDGE,A, B,
            WS_CHILD | WS_VISIBLE| WS_BORDER,
            50, 50, 500, 40, hwnd, (HMENU) ID_EDIT,NULL,
            NULL);

        hwndButton = CreateWindowW(L"button", L"SEARCH",
            WS_VISIBLE | WS_CHILD, 50, 100, 100, 40,
            hwnd, (HMENU)ID_BUTTON, NULL, NULL);
        hwndButtontwo = CreateWindowW(L"button", L"DOWNLOAD",
            WS_DISABLED | WS_CHILD, 1000, 100, 300, 40,
            hwnd, (HMENU)ID_BUTTON, NULL, NULL);
        mangadownloading = CreateWindowExW(0, PROGRESS_CLASS, TEXT("Downloading..."),
            WS_CHILD | PBS_MARQUEE | WS_DISABLED,
            1000, 150, 400, 30, hwnd, (HMENU)0, NULL, NULL);
        hwndButtonread = CreateWindowW(L"button", L"READ",
            WS_DISABLED | WS_CHILD, 1000, 200, 300, 40,
            hwnd, (HMENU)ID_BUTTON, NULL, NULL);


    case WM_PAINT :
    {     
        pbmfh[0] = DibLoadImage(L"darling.bmp");
        //pbmfh[0] = DibLoadImage(L"C:\\test\\zerotwo.bmp");
        pbmi[0] = (BITMAPINFO*)(pbmfh[0] + 1);
        pBits[0] = (BYTE*)pbmfh[0] + pbmfh[0]->bfOffBits;
        cxDib[0] = pbmi[0]->bmiHeader.biWidth;
        cyDib[0] = pbmi[0]->bmiHeader.biHeight;
            pbmfha[0] = DibLoadImage(L"C:\\test\\sample1.bmp");
            pbmia[0] = (BITMAPINFO*)(pbmfha[0] + 1);
            pBitsa[0] = (BYTE*)pbmfha[0] + pbmfha[0]->bfOffBits;
            cxDiba[0] = pbmia[0]->bmiHeader.biWidth;
            cyDiba[0] = pbmia[0]->bmiHeader.biHeight;
            hdc = BeginPaint(hwnd, &ps);
            SetDIBitsToDevice(hdc, 1000, cyClient / 4, cxDib[0], cyDib[0], 0, 0, 0, cyDib[0], pBits[0], pbmi[0], DIB_RGB_COLORS);
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
                RedrawWindow(hwnd, 0, 0, RDW_ERASE | RDW_ERASENOW | RDW_INVALIDATE | RDW_UPDATENOW);
                readmessagebox();
            }
            if ((HWND)lParam == hwndButtontwo)
            {  
                EnableWindow(hwndButtonread, TRUE);
                ShowWindow(hwndButtonread, 1);
                SendMessage(mangadownloading, (UINT)PBM_SETMARQUEE, 1, 10);
                EnableWindow(mangadownloading, TRUE);
                ShowWindow(mangadownloading, 1);
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
                
                
                CloseHandle(pio.hProcess);
                CloseHandle(pio.hThread);   
                printf("POPPPPP");
                EnableWindow(mangadownloading, FALSE);

                printf("END");
              


            }
            if ((HWND)lParam == hwndButtonread)
            {
                printf("REEEEEEEEAAAAAAAD");
                static HINSTANCE hint;
                //createreaderwindow(hint, 3);
                createreaderwindow(hint, 3);
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

int scroll = 50;
int createreaderwindow(HINSTANCE hinstano, int cmd)

{
    const wchar_t CLASS_NAMEA[] = L"SampleA Window Class";
    WNDCLASS wco = {0};
    //wco.lpfnWndProc = MyBitmapWindowProc;
    wco.lpfnWndProc=WinreaderProc;
    wco.hInstance = hinstano;
    wco.lpszClassName = CLASS_NAMEA;
    wco.hCursor = LoadCursor(NULL, IDC_HAND);
    wco.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    HBRUSH colo = CreateSolidBrush(RGB(0, 0, 0));
    wco.hbrBackground = colo;
    RegisterClass(&wco);
    HWND hwndnew = CreateWindowEx(
        0,                              // Optional window styles.
        CLASS_NAMEA,                     // Window class
        L"Aamshottoreader",    // Window text
        WS_OVERLAPPEDWINDOW| WS_VSCROLL|WS_THICKFRAME,            // Window style

        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

        NULL,       // Parent window    
        NULL,       // Menu
        hinstano,  // Instance handle
        NULL        // Additional application data
    );
    //HWND scroll = CreateAHorizontalScrollBar( hwndnew, 200);
    if (hwndnew == NULL)
    {
        return 0;
    }
    ShowWindow(hwndnew, 3);
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}

LRESULT CALLBACK WinreaderProc(HWND hwnd, UINT msg,
    WPARAM wParam, LPARAM lParam) 

{

    static HWND hwndMangaloc;
    static HWND hwndButton;
    static HWND nextbar;
    static HWND prevbar;
   


    static BITMAPFILEHEADER* pbmfh[2];
    static BITMAPINFO* pbmi[2];
    static BYTE* pBits[2];
    static int cxClient, cyClient, cxDib[2], cyDib[2];
    HDC hdc;
    PAINTSTRUCT ps;


    RECT rctMainWindowArea = { 0, 0, 1200, 800 };
    SCROLLINFO si = { 0 };

    static int xClient;   
    static int yClient;
    static int yChar;
    static int yPos;     
    static int yMinScroll;      
    static int yCurrentScroll;  
    static int yMaxScroll;
    static BITMAP bmp;
    static HDC hdcScreen;
    static HDC hdcScreenCompat;

    static BOOL fscroll;

    
    //printf(" \n MSG: %d \n", msg);
    
    switch (msg) {
        

    case (WM_CREATE):
        printf("\n RIP \n ");

        
        hwndButton = CreateWindowW(L"button", L"BROWSE",
            WS_VISIBLE | WS_CHILD, 0, 50, 200, 40,
            hwnd, (HMENU)ID_BUTTON, NULL, NULL);

        hwndMangaloc = CreateWindowExW(WS_EX_STATICEDGE, L"Static", NULL,
           WS_CHILD | WS_VISIBLE, 0, 10, 200, 40, hwnd, NULL, NULL, NULL);

        nextbar = CreateWindowExW(BS_PUSHBUTTON,L"button", L">>",
            WS_VISIBLE | WS_CHILD, 1400, 200, 50, 800,
            hwnd, (HMENU)ID_BUTTON, NULL, NULL);
  
        prevbar = CreateWindowExW(BS_PUSHBUTTON, L"button", L"<<",
            WS_VISIBLE | WS_CHILD, 150, 200, 50, 800,
            hwnd, (HMENU)ID_BUTTON, NULL, NULL);

        //CreateAHorizontalScrollBar(hwndMangaloc, 50);
    
       //break;
    case WM_COMMAND:
  
        if (HIWORD(wParam) == BN_CLICKED)

        {

            if ((HWND)lParam == hwndButton)

            {


                printf("\n CLICKED \n");
                auto location = fileopen(hwnd);
                SendMessageW(hwndMangaloc, WM_SETTEXT, 0, (LPARAM)location);
                //SendMessageW(hwndMangaloc, WM_SETTEXT, 0, L"TEXT");


                scroll = 0;
                RedrawWindow(hwnd, 0, 0, RDW_ERASE | RDW_ERASENOW | RDW_INVALIDATE | RDW_UPDATENOW);
                printf("\n DONE AGAIN\n");
            }

            if ((HWND)lParam == nextbar)

            {


                printf("\n CLICKED 2 \n");
                
                wchar_t text[100];
                wchar_t loc[100];
                wchar_t ans[10];
                int len = GetWindowTextLengthW(hwndMangaloc) + 1;
                GetWindowTextW(hwndMangaloc, text, len);
                wchar_t* pch;
                pch = wcsrchr(text, L'/');
                wchar_t* iou = wcsrchr(text, L'.');

                printf("Last occurence of 's' found at %d   %d\n", pch-text,iou-text);
                printf("TEXT : %ls  ", text);

                int up=pch - text;int down= iou - text;

                int inside = 0;
                wchar_t left[100];
                int count = 0;

                for (int num = 0; num <= up; num++)
                {
                    left[inside] = text[num];
                    inside++;
                }

                left[inside] = L'\0';

                for (int num = up + 1; num < down; num++)
                {
                    ans[count] = text[num];
                    count++;
                }
              
                ans[count] = L'\0';

               unsigned int in = wcstol(ans, 0, 10);;
                in = in + 1;
        
                printf("\n 222TEX22T : %ls  %d        %d\n", left,in,inside);

          
                wchar_t intapp[3];
                swprintf_s(intapp,3, L"%lu",in);
                wprintf(L"%s", intapp);
                wcscat(left, intapp);

                wcscat(left, L".bmp");


                printf("\n 33TEX33T : %ls      %d \n",left,in);
                    //"C:\manga\vagabond\9\1.bmp"

                SendMessageW(hwndMangaloc, WM_SETTEXT, 0,left);
                scroll = 0;
                RedrawWindow(hwnd, 0, 0, RDW_ERASE | RDW_ERASENOW | RDW_INVALIDATE | RDW_UPDATENOW);
                printf("\n DONE AGAIN\n");
            }

            if ((HWND)lParam == prevbar)

            {

                wchar_t text[100];
                wchar_t loc[100];
                wchar_t ans[10];
                int len = GetWindowTextLengthW(hwndMangaloc) + 1;
                GetWindowTextW(hwndMangaloc, text, len);
                wchar_t* pch;
                pch = wcsrchr(text, L'/');
                wchar_t* iou = wcsrchr(text, L'.');

                int up = pch - text; int down = iou - text;

                int inside = 0;
                wchar_t left[100];
                int count = 0;

                for (int num = 0; num <= up; num++)
                {
                    left[inside] = text[num];
                    inside++;
                }

                left[inside] = L'\0';

                for (int num = up + 1; num < down; num++)
                {
                    ans[count] = text[num];
                    count++;
                }

                ans[count] = L'\0';

                unsigned int in = wcstol(ans, 0, 10);;
                in = in - 1;

                wchar_t intapp[3];
                swprintf_s(intapp, 3, L"%lu", in);
                wprintf(L"%s", intapp);
                wcscat(left, intapp);

                wcscat(left, L".bmp");


                SendMessageW(hwndMangaloc, WM_SETTEXT, 0, left);

                scroll = 0;
                RedrawWindow(hwnd, 0, 0, RDW_ERASE | RDW_ERASENOW | RDW_INVALIDATE | RDW_UPDATENOW);
                printf("\n DONE AGAIN\n");
            }



 
        break;
    case WM_PAINT:
    {   printf("\n   PAINT \n");

    wchar_t text[100];
    int len = GetWindowTextLengthW(hwndMangaloc) + 1;
    GetWindowTextW(hwndMangaloc, text, len);
    if (len == 1)
    {
        printf("\n TRUE \n");
        pbmfh[0] = DibLoadImage(L"darling.bmp");
    }
    else
    {
        // wchar_t io[] = L"C:/manga/onepiece/6/1.bmp";
        printf("\n FALSE AGAIN \n");
        //LPWSTR LOC = text;
        pbmfh[0] = DibLoadImage(text);
        printf("\n FALSE AGAIN 2 \n");
    }
    //pbmfh[0] = DibLoadImage(L"C:\\test\\zerotwo.bmp");
    pbmi[0] = (BITMAPINFO*)(pbmfh[0] + 1);
    pBits[0] = (BYTE*)pbmfh[0] + pbmfh[0]->bfOffBits;
    cxDib[0] = pbmi[0]->bmiHeader.biWidth;
    cyDib[0] = pbmi[0]->bmiHeader.biHeight;


    hdc = BeginPaint(hwnd, &ps);


    int loc = cyDib[0] + scroll;
    if ((loc <= 0) | (loc >= 2 * cyDib[0]))
    {
        scroll = 0;
        loc = cyDib[0];

    }

    printf("\n LOC: %d  \n", loc);



    SetDIBitsToDevice(hdc, 200, cyClient / 4, cxDib[0], loc, 0, 0, 0, cyDib[0], pBits[0], pbmi[0], DIB_RGB_COLORS);

    EndPaint(hwnd, &ps);
    return 0;
    //break;

    }


    }

    
    case WM_VSCROLL:
    {  printf("\n SCROLL \n");

        //scroll += 50;

        switch (LOWORD(wParam))
       {

       case SB_LINEUP:
      {
        printf("\n page up \n");
        scroll += 400;
        break;
      }

       case SB_LINEDOWN:
      {
        printf("\n page down \n");
        scroll -= 400;
        break;

      }
    }
        RedrawWindow(hwnd, 0, 0, RDW_ERASE | RDW_ERASENOW | RDW_INVALIDATE | RDW_UPDATENOW);


        break;
    }
     
    case WM_KEYDOWN:
       
    {   
        printf("\nKey Key\n");

        switch (wParam)
        {
        case VK_UP:
            printf("\n page up \n");
            scroll += 400;
            break;



        case VK_DOWN:
            printf("\n page up \n");
            scroll -= 400;
            break;



        case VK_RETURN:

        {   
            wchar_t text[100];
            int len = GetWindowTextLengthW(hwndMangaloc) + 1;
            GetWindowTextW(hwndMangaloc, text, len);
            printf("\n ENTER \n");
            break;
        }


        }


        RedrawWindow(hwnd, 0, 0, RDW_ERASE | RDW_ERASENOW | RDW_INVALIDATE | RDW_UPDATENOW);


        //return 0;
        break;
        
    }

    case WM_DESTROY:
        if (pbmfh[0])
        {
            free(pbmfh[0]);
        }
    
        PostQuitMessage(0);
        break;
    }

    return DefWindowProcW(hwnd, msg, wParam, lParam);
}



int main(HINSTANCE hinstance, HINSTANCE nully, LPSTR atgs, int ncmdshow)
{

    int a = DisplayResourceNAMessageBox();

    if (a == 5)
    {
        createmainwindow(hinstance, ncmdshow);
        
    }
    //createmainwindow(hinstance, ncmdshow);
    
   //createreaderwindow(hinstance, ncmdshow);
    
    return 0;

}
