 /*
 * (c) Copyright 1995-1999, Igor Tarasov
 * FidoNet: 2:5020/370.2 620.20 1103.5
 * Inet: itarasov@rtuis.miem.edu.ru
 * Phone: (095)942-50-97
 */
#include <stdio.h>
#include <windows.h>
#include <math.h>


#include "gl/gl.h"
#include "gl/glu.h"
#include "gl/glaux.h"

#include "main.h"
#include "game.h"

HWND hWnd;
HWND hWnd1;
HGLRC hGLRC;
HDC hDC;



/* шрифты */
HFONT hFontNew, hFontOld;
LOGFONT lf;
GLYPHMETRICSFLOAT agmf[256];
 

float mouse_x;
float mouse_y;
int mouse_flags;
int l_mouse = 0; // 0 - не нажата, 1 - нажата, 2 - отпущена
/* размер окна в пикселях (Windows) */
float window_width;
float window_height;
/* размер области вывода внутри окна в единицах OpenGL */
float opengl_width =  480*1.25; //1024; // 938;
float opengl_height = 640; //768; // 640;


int SetWindowPixelFormat()
{
    int m_GLPixelIndex;
    PIXELFORMATDESCRIPTOR pfd;


	pfd.nSize       = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion    = 1;

    pfd.dwFlags   = PFD_DRAW_TO_WINDOW | 
                    PFD_SUPPORT_OPENGL | 
                    PFD_DOUBLEBUFFER;

    pfd.iPixelType     = PFD_TYPE_RGBA;
    pfd.cColorBits     = 32;
    pfd.cRedBits       = 8;
    pfd.cRedShift      = 16;
    pfd.cGreenBits     = 8;
    pfd.cGreenShift    = 8;
    pfd.cBlueBits      = 8;
    pfd.cBlueShift     = 0;
    pfd.cAlphaBits     = 0;
    pfd.cAlphaShift    = 0;
    pfd.cAccumBits     = 64;    
    pfd.cAccumRedBits  = 16;
    pfd.cAccumGreenBits   = 16;
    pfd.cAccumBlueBits    = 16;
    pfd.cAccumAlphaBits   = 0;
    pfd.cDepthBits        = 32;
    pfd.cStencilBits      = 8;
    pfd.cAuxBuffers       = 0;
    pfd.iLayerType        = PFD_MAIN_PLANE;
    pfd.bReserved         = 0;
    pfd.dwLayerMask       = 0;
    pfd.dwVisibleMask     = 0;
    pfd.dwDamageMask      = 0;


    m_GLPixelIndex = ChoosePixelFormat( hDC, &pfd);
    if(m_GLPixelIndex==0) // Let's choose a default index.
    {
     m_GLPixelIndex = 1;    
     if(DescribePixelFormat(hDC,m_GLPixelIndex,sizeof(PIXELFORMATDESCRIPTOR),&pfd)==0)
       return 0;
    }

    if (SetPixelFormat( hDC, m_GLPixelIndex, &pfd)==FALSE)
        return 0;

    return 1;
}


  



void display()
{
	PAINTSTRUCT ps;
	BeginPaint(hWnd, &ps);
/////
	
	wglMakeCurrent(hDC, hGLRC);
	glClearColor(0.5, 0.0, 0.5, 1.0);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	game();

/* */	
    glFinish();
    SwapBuffers(wglGetCurrentDC());
/////
	EndPaint(hWnd, &ps);


}


LRESULT CALLBACK WindowFunc(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
float pos[4] = {3,3,3,1};
float dir[3] = {-1,-1,-1};
char key;
int ret;

switch(msg)
	{

	case WM_CREATE:
	hDC = GetDC(hWnd);
	SetWindowPixelFormat();
	hGLRC = wglCreateContext(hDC);
	wglMakeCurrent(hDC, hGLRC);


  memset(&lf, 0, sizeof(LOGFONT)); 

  lf.lfHeight        =   28;
  lf.lfWeight        =   FW_NORMAL ;
  lf.lfCharSet       =   FF_DONTCARE  ;  /* ? или ANSI_CHARSET  */
  lf.lfOutPrecision  =   OUT_TT_ONLY_PRECIS ;  /*  OUT_DEFAULT_PRECIS*/ 
  lf.lfClipPrecision =   CLIP_DEFAULT_PRECIS ;
  lf.lfQuality       =   DEFAULT_QUALITY ;
  lf.lfPitchAndFamily=   FF_DONTCARE || DEFAULT_PITCH;
//   lstrcpy (lf.lfFaceName, "Arial Cyr") ;  // работает
//  lstrcpy (lf.lfFaceName, "") ; /*работает */
  lstrcpy (lf.lfFaceName, "Arial Cyr") ;

  hFontNew = CreateFontIndirect(&lf);
  hFontOld = SelectObject(hDC,hFontNew);

  wglUseFontOutlines(hDC, 0, 255, 1000, 4.0, 0.0, WGL_FONT_POLYGONS, NULL);/* работает */
  /*wglUseFontOutlines(hDC, 0, 255, 1000, 0.0, 0.0, WGL_FONT_POLYGONS, &agmf);*/
 //wglUseFontBitmaps ();
  DeleteObject(SelectObject(hDC, hFontOld));
  DeleteObject(SelectObject(hDC, hFontNew));

	SetTimer(hWnd, TIMER1, TICS_MIN, (TIMERPROC)timer);

    break; 

    
	case WM_DESTROY:
	if (hGLRC) 
	{
	    wglMakeCurrent(NULL, NULL);
	    wglDeleteContext(hGLRC);
	}
	ReleaseDC(hWnd, hDC);

	PostQuitMessage(0);
	break;


	case WM_KEYDOWN:
		key = (unsigned char) wParam;
		if (key == 27) 
		{
			DestroyWindow(hWnd);
		}
		process_key(key);
		key = '\0';

	break;	
	
	case WM_PAINT:
	display();
	break;
	
	// WM_SIZING - при восстановлении окна после минимизации перерисовывавается
	case WM_SIZING:
	InvalidateRect(hWnd, NULL, FALSE);
		break;
		
	case WM_SIZE: 
	resize( LOWORD(lParam), HIWORD(lParam), hDC, hGLRC );

 	window_width = LOWORD(lParam);
	window_height = HIWORD(lParam);
	break;  

	case WM_LBUTTONDOWN:
//	mouse_flags = wParam;        // key flags 
	mouse_x = LOWORD(lParam);  // horizontal position of cursor 
	mouse_y = HIWORD(lParam);  // vertical position of cursor 
    mouse_x = mouse_x * opengl_width / window_width ;
//    mouse_y = opengl_height - mouse_y * opengl_height / window_height ;	
	mouse_y = mouse_y * opengl_height / window_height ;	
	l_mouse = 1;
	break;

	case WM_TIMER:
	timer();
	InvalidateRect(hWnd, NULL, FALSE);

	break;

	case WM_LBUTTONUP:
//	mouse_flags = wParam;        // key flags 
//	mouse_y = 2000 - HIWORD(lParam);  // vertical position of cursor 

	l_mouse = 2;
	break;

       default:
	return DefWindowProc(hWnd,msg,wParam,lParam);
	}


return 0;
}

///////++++++++++++++++++++++++++++


int WINAPI WinMain(HINSTANCE hThisInst,
				   HINSTANCE hPrevInst,
				   LPSTR str,int nWinMode)
{
MSG msg;
WNDCLASS wcl;

/*HWND hWndDesktop;*/

int i;
//WNDCLASS wcl1;

wcl.hInstance=hThisInst;
wcl.lpszClassName = "OpenGLWinClass";
wcl.lpfnWndProc = WindowFunc;
wcl.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;

wcl.hIcon = NULL;
wcl.hCursor = LoadCursor(NULL,IDC_ARROW);
wcl.lpszMenuName = NULL;

wcl.cbClsExtra = 0;
wcl.cbWndExtra = 0;

wcl.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
RegisterClass(&wcl);


hWnd = CreateWindow("OpenGLWinClass", "много ящиков", 
					 WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS ,
					 2,
					 2,
					// 1125.6,  //480 * 1.2 + 20 * 1.2 + (146*3) * 1.2,
					// 800,
					//GetSystemMetrics(SM_CXSCREEN) - 2,
					WINDOW_WIDTH_PX,
					WINDOW_HEIGHT_PX,
					 HWND_DESKTOP, NULL,
					 hThisInst, NULL);
window_height = 800;



ShowWindow(hWnd,nWinMode);
UpdateWindow(hWnd);


	while(1)
	{
		//ct = clock();
		//InvalidateRect(hWnd, NULL, FALSE);
		
		while( PeekMessage(&msg,NULL,0,0,PM_NOREMOVE) ) 
		{
			if(GetMessage(&msg,NULL,0,0))
			{ 
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				return 0;
			}
		}
		InvalidateRect(hWnd, NULL, FALSE);
		Sleep(10);
	} 

	return 0;

}
