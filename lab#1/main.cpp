#include <windows.h>
#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#define IDC_MAIN_BUTTON	101			// Button identifier
#define IDC_MAIN_EDIT	102			// Edit box identifier
#define IDC_EXIT_BUTTON 103

LRESULT CALLBACK WinProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInst,HINSTANCE hPrevInst,LPSTR lpCmdLine,int nShowCmd)
{
	WNDCLASSEX wClass;
	ZeroMemory(&wClass,sizeof(WNDCLASSEX));
	wClass.cbClsExtra=NULL;
	wClass.cbSize=sizeof(WNDCLASSEX);
	wClass.cbWndExtra=NULL;
	wClass.hbrBackground=(HBRUSH)COLOR_WINDOW;
	wClass.hCursor=LoadCursor(NULL,IDC_ARROW);
	wClass.hIcon=NULL;
	wClass.hIconSm=NULL;
	wClass.hInstance=hInst;
	wClass.lpfnWndProc=(WNDPROC)WinProc;
	wClass.lpszClassName="Window Class";
	wClass.lpszMenuName=NULL;
	wClass.style=CS_HREDRAW|CS_VREDRAW;

	if(!RegisterClassEx(&wClass))
	{
		int nResult=GetLastError();
		MessageBox(NULL,
			"Window class creation failed\r\n",
			"Window Class Failed",
			MB_ICONERROR);
	}

	HWND hWnd=CreateWindowEx(NULL,
			"Window Class",
			"Windows application",
			WS_OVERLAPPEDWINDOW,
			200,
			200,
			640,
			480,
			NULL,
			NULL,
			hInst,
			NULL);

	if(!hWnd)
	{
		int nResult=GetLastError();

		MessageBox(NULL,
			"Window creation failed\r\n",
			"Window Creation Failed",
			MB_ICONERROR);
	}

    ShowWindow(hWnd,nShowCmd);

	MSG msg;
	ZeroMemory(&msg,sizeof(MSG));

	while(GetMessage(&msg,NULL,0,0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

LRESULT CALLBACK WinProc(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;
    RECT rect;
    static HWND hEdit;
    static char buffer[256];
	switch(msg)
	{
        case WM_CREATE:
        {
            // Create an edit box
            hEdit=CreateWindowEx(WS_EX_CLIENTEDGE,
                "EDIT",
                "",
                WS_CHILD|WS_VISIBLE|
                ES_MULTILINE|ES_AUTOVSCROLL,
                50,
                100,
                200,
                100,
                hWnd,
                (HMENU)IDC_MAIN_EDIT,
                GetModuleHandle(NULL),
                NULL);
            HGDIOBJ hfDefault=GetStockObject(DEFAULT_GUI_FONT);
            SendMessage(hEdit,
                WM_SETFONT,
                (WPARAM)hfDefault,
                MAKELPARAM(FALSE,0));
            SendMessage(hEdit,
                WM_SETTEXT,
                NULL,
                (LPARAM)"Enter your name here...");
            // Create a push button
            HWND hWndButton=CreateWindowEx(NULL,
                "BUTTON",
                "OK",
                WS_TABSTOP|WS_VISIBLE|
                WS_CHILD|BS_DEFPUSHBUTTON,
                50,
                220,
                100,
                24,
                hWnd,
                (HMENU)IDC_MAIN_BUTTON,
                GetModuleHandle(NULL),
                NULL);
            SendMessage(hWndButton,
                WM_SETFONT,
                (WPARAM)hfDefault,
                MAKELPARAM(FALSE,0));

            HWND hWndExit=CreateWindowEx(NULL,
                "BUTTON",
                "EXIT",
                WS_TABSTOP|WS_VISIBLE|
                WS_CHILD|BS_DEFPUSHBUTTON,
                270,
                350,
                100,
                60,
                hWnd,
                (HMENU)IDC_EXIT_BUTTON,
                GetModuleHandle(NULL),
                NULL);
            SendMessage(hWndButton,
                WM_SETFONT,
                (WPARAM)hfDefault,
                MAKELPARAM(FALSE,0));
        }break;
        case WM_COMMAND:
            switch(LOWORD(wParam))
            {

                case IDC_MAIN_BUTTON:
                {
                    SendMessage(hEdit,
                    WM_GETTEXT,
                    sizeof(buffer)/sizeof(buffer[0]),
                    reinterpret_cast<LPARAM>(buffer));
                    //MessageBox(NULL, buffer,"Your name",MB_ICONQUESTION | MB_YESNO);
                }break;

                case IDC_EXIT_BUTTON:
                {
                    PostQuitMessage(0);
                    return 0;
                }break;
        }break;

        case WM_PAINT:
        {
            hdc=BeginPaint(hWnd, &ps);
            GetClientRect(hWnd, &rect);
            DrawText(hdc, "something here",-1,&rect, DT_SINGLELINE  | DT_RIGHT );
            EndPaint(hWnd, &ps);
        }break;

        case WM_DESTROY:
        {
            PostQuitMessage(0);
            return 0;
        }break;

	}

	return DefWindowProc(hWnd,msg,wParam,lParam);
}
