#include <d3d9.h>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

LPDIRECT3D9 g_D3D = NULL;
LPDIRECT3DDEVICE9 g_D3DDevice = NULL;

LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
        break;

    case WM_KEYUP:
        if(wParam == VK_ESCAPE) PostQuitMessage(0);
        break;
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}

bool InitializeD3D(HWND hWnd, bool fullscreen)
{
    D3DDISPLAYMODE displayMode;

    g_D3D = Direct3DCreate9(D3D_SDK_VERSION);
    if(g_D3D == NULL) return false;

    if(FAILED(g_D3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT,
        &displayMode)))
    {
        return false;
    }

    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory(&d3dpp, sizeof(d3dpp));

    if(fullscreen)
    {
        d3dpp.Windowed = FALSE;
        d3dpp.BackBufferWidth = 640;
        d3dpp.BackBufferHeight = 480;
    }
    else
    {
        d3dpp.Windowed = TRUE;
        d3dpp.BackBufferWidth = 640;
        d3dpp.BackBufferHeight = 480;
    }

    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat = displayMode.Format;

    if(FAILED(g_D3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
        D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &g_D3DDevice)))
    {
        return false;
    }

    return true;
}

void RenderScene()
{
    g_D3DDevice->Clear(0, NULL, D3DCLEAR_TARGET,
        D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
    g_D3DDevice->BeginScene();
    g_D3DDevice->EndScene();
    g_D3DDevice->Present(NULL, NULL, NULL, NULL);
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR cmd, int show)
{
    WNDCLASSEX wc = {
        sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0, 0, hInst,
        NULL, NULL, NULL, NULL, L"AppClass", NULL
    };
    RegisterClassEx(&wc);

    HWND hWnd = CreateWindow(L"AppClass", L"Blank Window", WS_OVERLAPPEDWINDOW, 100, 100,
        640, 480, NULL, NULL, hInst, NULL);

    if(InitializeD3D(hWnd, false))
    {
        ShowWindow(hWnd, SW_SHOWDEFAULT);
        UpdateWindow(hWnd);

        MSG msg;

        ZeroMemory(&msg, sizeof(msg));

        while(msg.message != WM_QUIT)
        {
            if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
            else
            {
                RenderScene();
            }
        }

        UnregisterClass(L"AppClass", wc.hInstance);
    }

    if(g_D3D != NULL)
    {
        g_D3D->Release();
        g_D3D = NULL;
    }

    if(g_D3DDevice != NULL)
    {
        g_D3DDevice->Release();
        g_D3DDevice = NULL;
    }

    return 0;
}