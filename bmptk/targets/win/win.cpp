#include "bmptk.h"

// not user level include files, hence not included by bmptk.h
// #include "bmptk_win_graphics.hx"
#include "windows.h"
#include <iostream>
#include <iomanip>

#ifndef XSIZE
   #define XSIZE 256
#endif
#ifndef YSIZE
   #define YSIZE 192
#endif
#ifndef SCALE
   #define SCALE 1
#endif   

// derived by trial-and-error :(
#define X_MARGIN 20
#define Y_MARGIN 42

bmptk::time bmptk::current_time(){
   return bmptk::time( GetTickCount() * 1000ULL );
}

/*
void bmptk::fatal_error_detected( const char *msg  ){
   std::cout << "fatal error : " << msg << "\n" << std::flush;
   for(;;);
}
*/

#ifdef BMPTK_GRAPHICS

#include <windows.h>

/* This is where all the input to the window goes to */
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	switch(Message) {
		
		/* trap the WM_CLOSE (clicking X) message, and actually tell the window to close */
		case WM_CLOSE: {
			DestroyWindow(hwnd);
			break;
		}
		
		/* Upon destruction, tell the main thread to stop */
		case WM_DESTROY: {
			PostQuitMessage(0);
			break;
		}
		
		/* All other messages (a lot of them) are processed using default procedures */
		default:
			return DefWindowProc(hwnd, Message, wParam, lParam);
	}
	return 0;
}

HINSTANCE hInstance;
WNDCLASSEX wc; /* A properties struct of our window */
HWND hwnd; /* A 'HANDLE', hence the H, or a pointer to our window */
MSG Msg; /* A temporary location for all messages */
HDC hdc;
	
/* The 'main' function of Win32 GUI programs: this is where execution starts */
int WINAPI WinMain(HINSTANCE xhInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
   hInstance = xhInstance;
   extern int main();
   main();
   return 0;
}

bmptk::graphics::target_screen::target_screen( void ):
   frame( vector( XSIZE, YSIZE ))
{

	/* zero out the struct and set the stuff we want to modify */
	memset(&wc,0,sizeof(wc));
	wc.cbSize		 = sizeof(WNDCLASSEX);
	wc.lpfnWndProc	 = WndProc; /* This is where we will send messages to */
	wc.hInstance	 = hInstance;
	wc.hCursor		 = LoadCursor(NULL, IDC_ARROW);
	
	/* White, COLOR_WINDOW is just a #define for a system color, try Ctrl+Clicking it */
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wc.lpszClassName = "WindowClass";
	wc.hIcon		 = LoadIcon(NULL, IDI_APPLICATION); /* Load a standard icon */
	wc.hIconSm		 = LoadIcon(NULL, IDI_APPLICATION);

	if(!RegisterClassEx(&wc)) {
		MessageBox(NULL, "Window Registration Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		for(;;);
	}

	hwnd = CreateWindowEx(
	    WS_EX_CLIENTEDGE,
		"WindowClass",
		"Caption",
		WS_VISIBLE | WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, /* x */
		CW_USEDEFAULT, /* y */
		XSIZE + X_MARGIN, /* width */
		YSIZE + Y_MARGIN, /* height */
		NULL,NULL,hInstance,NULL);

	if(hwnd == NULL) {
		MessageBox(NULL, "Window Creation Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		for(;;);
	}
	
   hdc = GetDC(hwnd);
   for( int x = 0; x < 200; x++ ){
      SetPixel( hdc, 10 + x , 10 + x, RGB( 10+x, 100, 0 ) );
   }

	/*
		This is the heart of our program where all input is processed and 
		sent to WndProc. Note that GetMessage blocks code flow until it receives something, so
		this loop will not produre unreasonably CPU usage
	*/
	if(0)while(GetMessage(&Msg, NULL, 0, 0) > 0) { /* If no error is received... */
		TranslateMessage(&Msg); /* Translate keycodes to chars if present */
		DispatchMessage(&Msg); /* Send it to WndProc */
	}
	// return Msg.wParam;
}

void bmptk::graphics::target_screen::checked_write( 
   const vector v,
   const color c 
){
   if( ! c.is_transparent() ){ 
      for( int x = 0; x < SCALE; x++ ){
         for( int y = 0; y < SCALE; y++ ){
            SetPixel( 
		        hdc, 
   			  SCALE * v.x_get() + x , 
	   		  SCALE * v.y_get() + y,
		   	  RGB( c.red_get(), c.green_get(), c.blue_get() )
            );
         }            
      }
   }      
}

#endif