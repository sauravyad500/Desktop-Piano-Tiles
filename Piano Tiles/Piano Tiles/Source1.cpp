/*#ifndef UNICODE
#define UNICODE
#endif 

#define IDM_PAUSE 1
#define ID_TIMER 10

#include <windows.h>
#include "resource.h"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
wchar_t temp[58] = L" ",tem[5]=L" ";
HINSTANCE hInst;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
	// Register the window class.
	const wchar_t CLASS_NAME[]  = L"Sample Window Class";

	hInst = hInstance;
	WNDCLASS wc = { };

	wc.lpfnWndProc   = WindowProc;
	wc.hInstance     = hInstance;
	wc.lpszClassName = CLASS_NAME;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);

	RegisterClass(&wc);

	// Create the window.

	HWND hwnd = CreateWindowEx(
		0,                              // Optional window styles.
		CLASS_NAME,                     // Window class
		L"Piano Tiles",    // Window text
		WS_OVERLAPPEDWINDOW,            // Window style

										// Size and position
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

		NULL,       // Parent window    
		NULL,       // Menu
		hInstance,  // Instance handle
		NULL        // Additional application data
	);

	if (hwnd == NULL)
	{
		return 0;
	}

	ShowWindow(hwnd, nCmdShow);

	// Run the message loop.

	MSG msg = { };
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

int gx , gy , mx , my , mm , mn , n , arr[7] , k , tm = 0 , gt = 0 , x , y , xx , yy , yyy , xh = 0 , yh = 0 , i , j;
HDC hdcMem , hdc;
TEXTMETRIC t;
HWND no_sqedit,button;
HBITMAP hBmp , hBmp1,bmp,bold,htemp;
BOOL clicked = FALSE,is_clicked[7],paused,game_over=FALSE;
HBRUSH whiteb , blackb , back;
HFONT h , hf , hfont;
PAINTSTRUCT ps;
HCURSOR arrow , hand;
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch ( uMsg )
	{
		case WM_DESTROY:

			DeleteObject(bmp);
			DeleteObject(hBmp);
			DeleteObject(hBmp1);
			DeleteDC(hdcMem);
			PostQuitMessage(0);
			return 0;

		case WM_CREATE:
			no_sqedit = CreateWindow(L"edit" , L"Enter the number of squares" , WS_VISIBLE | WS_CHILD | WS_BORDER , 10 , 10 , 150 , 20 , hwnd , (HMENU)100 , NULL , NULL);
			GetTextMetrics(GetDC(no_sqedit) , &t);
			button = CreateWindow(L"STATIC" , L"Go" , SS_BITMAP | WS_CHILD | WS_VISIBLE , 50 , 50 , 0 , 0 , hwnd , (HMENU)101 , NULL , NULL);

			arrow = LoadCursor(NULL , IDC_ARROW);
			hand = LoadCursor(NULL , IDC_HAND);
			hBmp = LoadBitmap(hInst , MAKEINTRESOURCE(IDB_BITMAP1));
			if ( hBmp == NULL ) MessageBox(NULL , L"Error while loading image" , L"Error" , MB_OK | MB_ICONERROR);
			hBmp1 = LoadBitmap(hInst , MAKEINTRESOURCE(IDB_BITMAP2));
			if ( hBmp1 == NULL ) MessageBox(NULL , L"Error while loading image" , L"Error" , MB_OK | MB_ICONERROR);
			SendMessage(button , STM_SETIMAGE , (WPARAM)IMAGE_BITMAP , (LPARAM)hBmp);

			for (i = 0; i < 7; i++ ) arr[i] = -1;
			whiteb = CreateSolidBrush(RGB(255 , 255 , 255));
			blackb = CreateSolidBrush(RGB(0 , 0 , 0));
			back = CreateSolidBrush(RGB(200 , 25 , 25));
			h = CreateFont(100 , 50 , 5 , 0 , FW_BOLD , FALSE , FALSE , FALSE , DEFAULT_CHARSET , OUT_DEFAULT_PRECIS , CLIP_DEFAULT_PRECIS , DEFAULT_QUALITY , VARIABLE_PITCH , TEXT("Impact"));
			hf = CreateFont(48 , 0 , 0 , 0 , FW_DONTCARE , FALSE , TRUE , FALSE , DEFAULT_CHARSET , OUT_OUTLINE_PRECIS , CLIP_DEFAULT_PRECIS , CLEARTYPE_QUALITY , VARIABLE_PITCH , TEXT("Impact"));
			HMENU hMenuBar , hPause;
			hMenuBar = CreateMenu();
			hPause = CreateMenu();
			AppendMenu(hPause , MF_STRING , IDM_PAUSE , L"Pause");
			SetMenu(hwnd , hPause);
			return 0;

		case WM_SIZE:
			if ( !game_over ){
				gx = LOWORD(lParam);
				gy = HIWORD(lParam);
				if ( !clicked ){
					SetWindowPos(no_sqedit , NULL , 50 , gy / 2 - 50 , gx - 100 , t.tmHeight + 5 , SWP_DRAWFRAME | SWP_SHOWWINDOW);
					SetWindowPos(button , NULL , gx / 2 - 34 , gy / 2 - 25 , 67 , 58 , SWP_SHOWWINDOW);
				}
			}
			InvalidateRect(hwnd , NULL , FALSE);
			break;

		case WM_COMMAND:
		{
			switch ( LOWORD(wParam) )
			{
				case IDM_PAUSE:
					if(paused ) paused = FALSE;
					else paused = TRUE;
					break;

				case 100:
					if ( HIWORD(wParam) == EN_SETFOCUS )
					{
						GetWindowText(no_sqedit , temp , 28);
						if ( lstrcmp(temp , L"Enter the number of squares") == 0 )   SetWindowText(no_sqedit , L"");
					}
					break;
			}break;
		}

		case WM_MOUSEMOVE:
			if ( !game_over ){
				mm = LOWORD(lParam);
				mn = HIWORD(lParam);
				if ( !clicked ){
					if ( mm > gx / 2 - 34 && mm < gx / 2 - 34 + 67 && mn > gy / 2 - 25 && mn < gy / 2 - 25 + 58 )
					{
						htemp = (HBITMAP)SendMessage(button , STM_GETIMAGE , (WPARAM)IMAGE_BITMAP , (LPARAM)NULL);
						if ( htemp != hBmp1 )  SendMessage(button , STM_SETIMAGE , (WPARAM)IMAGE_BITMAP , (LPARAM)hBmp1);
						if ( GetCursor() != hand ) SetCursor(hand);
					}
					else {
						htemp = (HBITMAP)SendMessage(button , STM_GETIMAGE , (WPARAM)IMAGE_BITMAP , (LPARAM)NULL);
						if ( htemp != hBmp )  SendMessage(button , STM_SETIMAGE , (WPARAM)IMAGE_BITMAP , (LPARAM)hBmp);
					}
				}
			}
			break;

		case WM_LBUTTONDOWN:
			if ( !game_over ){
				mx = LOWORD(lParam);
				my = HIWORD(lParam);
				if ( !clicked ){
					if ( !( mx > 50 && mx<50 + gx - 100 && my>gy / 2 - 30 && my < gy / 2 - 30 + t.tmHeight + 5 ) )
					{
						SetFocus(hwnd);
						GetWindowText(no_sqedit , temp , 28);
						if ( lstrcmp(temp , L"") == 0 )  SetWindowText(no_sqedit , L"Enter the number of squares");
					}
					if ( mx > gx / 2 - 34 && mx < gx / 2 - 34 + 67 && my > gy / 2 - 25 && my < gy / 2 - 25 + 58 )
					{
						GetWindowText(no_sqedit , temp , 28);
						n = _wtoi(temp);
						if ( n == 0 ){ MessageBox(hwnd , L"Enter again." , L"Error" , MB_OK); break; }
						SetWindowPos(no_sqedit , NULL , 0 , 0 , 0 , 0 , SWP_HIDEWINDOW);
						SetWindowPos(button , NULL , 0 , 0 , 0 , 0 , SWP_HIDEWINDOW);
						SetTimer(hwnd , ID_TIMER , 0 , (TIMERPROC)NULL);
						clicked = TRUE;
						InvalidateRect(hwnd , NULL , FALSE);
					}
				}
				else
				{
					for (i = 0; i < 7; i++ )
					{
						j = arr[i];
						if ( !is_clicked[i] )
						{
							yy = ( gt + gy - ( gy*i ) / 6 ) % ( ( 7 * gy ) / 6 ) - gy / 6;
							xx = ( gx*j ) / n;
							if ( mx > xx && my > yy && mx < xx + gx / n && my < yy + gy / 6 )
							{
								is_clicked[i] = TRUE;
								InvalidateRect(hwnd , NULL , FALSE);
							}
						}
					}
				}
			}
			break;

		case WM_TIMER:
			if ( !paused ){
				InvalidateRect(hwnd , NULL , FALSE);
				gt = gt + 2;

				for (i = 0; i < 7; i++ )
				{
					yyy = ( gt + gy - ( gy*i ) / 6 ) % ( ( 7 * gy ) / 6 ) - gy / 6;
					if ( yyy == (5*gy)/6 && !is_clicked[i] && arr[i]!=-1 )
					{
						KillTimer(hwnd , ID_TIMER);
						MessageBox(hwnd , L"Sorry you lost the game" , L"Alert" , MB_OK);
						game_over = TRUE;
						InvalidateRect(hwnd , NULL , FALSE);
					}
				}
			}
			break;

		case WM_PAINT:
		{
			hdc= BeginPaint(hwnd, &ps);

			if ( !game_over ){
				if ( clicked ){
					hdcMem = CreateCompatibleDC(hdc);
					bmp = CreateCompatibleBitmap(hdc , gx , gy);
					bold = (HBITMAP)SelectObject(hdcMem , bmp);

					for ( i = 0; i < 7; i++ )
					{
						y = ( gt + gy - ( gy*i ) / 6 ) % ( ( 7 * gy ) / 6 ) - gy / 6;

						if (y == -gy / 6 ){ arr[i] = rand() % n; is_clicked[i] = FALSE; }

						for ( j = 0; j < n; j++ )
						{
							x = ( gx*j ) / n;
							if ( arr[i] == j && !is_clicked[i] )  SelectObject(hdcMem , blackb);
							Rectangle(hdcMem , x , y , x + gx / n , y + gy / 6);
							if ( arr[i] == j && !is_clicked[i] )  SelectObject(hdcMem , whiteb);
						}
					}
					BitBlt(hdc , 0 , 0 , gx , gy , hdcMem , 0 , 0 , SRCCOPY);
					SelectObject(hdc , bold);
					DeleteObject(bmp);
					DeleteDC(hdcMem);
				}
				else FillRect(hdc , &ps.rcPaint , (HBRUSH)( COLOR_WINDOW + 1 ));
			}
			else{
				FillRect(hdc , &ps.rcPaint , back);
				SetBkColor(hdc , RGB(200 , 25 , 25));// background color
				SelectObject(hdc , h);
				SetTextColor(hdc , RGB(255 , 255 , 255));
				TextOut(hdc , 100 , 100 , L"You lost the game" , 17);
				SelectObject(hdc , h);
				SetTextColor(hdc , RGB(25 , 25 , 255));
				lstrcpy(temp , L"Points    ");
				_itow(gt , tem , 10);
				lstrcat(temp , tem);
				TextOut(hdc , 300 , 300 , temp , lstrlen(temp));
			}

			EndPaint(hwnd, &ps);
		}return 0;

	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}*/