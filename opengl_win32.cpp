#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>

HWND    hWnd;
HDC     hDC;
HGLRC   hRC;

// Set up pixel format for graphics initialization
void SetupPixelFormat()
{
    PIXELFORMATDESCRIPTOR pfd, *ppfd;
    int pixelformat;

    ppfd = &pfd;

    ppfd->nSize = sizeof(PIXELFORMATDESCRIPTOR);
    ppfd->nVersion = 1;
    ppfd->dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    ppfd->dwLayerMask = PFD_MAIN_PLANE;
    ppfd->iPixelType = PFD_TYPE_COLORINDEX;
    ppfd->cColorBits = 16;
    ppfd->cDepthBits = 16;
    ppfd->cAccumBits = 0;
    ppfd->cStencilBits = 0;

    pixelformat = ChoosePixelFormat(hDC, ppfd);
    SetPixelFormat(hDC, pixelformat, ppfd);
}

// Initialize OpenGL graphics
void InitGraphics()
{
    hDC = GetDC(hWnd);

    SetupPixelFormat();

    hRC = wglCreateContext(hDC);
    wglMakeCurrent(hDC, hRC);

    glClearColor(0, 0, 0, 0.5);
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);
}

// Resize graphics to fit window
void ResizeGraphics()
{
    // Get new window size
    RECT rect;
	int width, height;
	GLfloat aspect;

    GetClientRect(hWnd, &rect);
    width = rect.right;
    height = rect.bottom;
    aspect = (GLfloat)width / height;

    // Adjust graphics to window size
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, aspect, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

// Draw frame
void DrawGraphics()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Set location in front of camera
    glLoadIdentity();
    glTranslated(0, 0, -10);

    // Draw a square
    glBegin(GL_QUADS);
    glColor3d(1, 0, 0);
    glVertex3d(-2, 2, 0);
    glVertex3d(2, 2, 0);
    glVertex3d(2, -2, 0);
    glVertex3d(-2, -2, 0);
    glEnd();

    // Show the new scene
    SwapBuffers(hDC);
}

// Handle window events and messages
LONG WINAPI MainWndProc (HWND hWnd, UINT uMsg, WPARAM  wParam, LPARAM  lParam)
{
    switch (uMsg)
    {
    case WM_SIZE:
        ResizeGraphics();
        break;

    case WM_CLOSE: 
        DestroyWindow(hWnd);
        break;
 
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
 
    // Default event handler
    default: 
        return DefWindowProc (hWnd, uMsg, wParam, lParam); 
        break; 
    } 
 
    return 1; 
}

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

    const LPCWSTR appname = TEXT("OpenGL Sample");

    WNDCLASS wndclass;
    MSG      msg;
 
    // Define the window class
    wndclass.style         = 0;
    wndclass.lpfnWndProc   = (WNDPROC)MainWndProc;
    wndclass.cbClsExtra    = 0;
    wndclass.cbWndExtra    = 0;
    wndclass.hInstance     = hInstance;
    wndclass.hIcon         = LoadIcon(hInstance, appname);
    wndclass.hCursor       = LoadCursor(NULL,IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wndclass.lpszMenuName  = appname;
    wndclass.lpszClassName = appname;
 
    // Register the window class
    if (!RegisterClass(&wndclass)) return FALSE;
 
    // Create the window
    hWnd = CreateWindow(
            appname,
            appname,
            WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            800,
            600,
            NULL,
            NULL,
            hInstance,
            NULL);
 
    if (!hWnd) return FALSE;

    // Initialize OpenGL
    InitGraphics();

    // Display the window
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    // Event loop
    while (1)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE) == TRUE)
        {
            if (!GetMessage(&msg, NULL, 0, 0)) return TRUE;

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        DrawGraphics();
    }

    wglDeleteContext(hRC);
    ReleaseDC(hWnd, hDC);
}
