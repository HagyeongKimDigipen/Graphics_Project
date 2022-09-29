/*
* Kim Hagyeong (hakyung.kim)
* Class Project
* cs200
* FALL 2019
*/
#include "window.h"
#include "Win32OS.h"

#include "KeyboardClass.h"
#include "MouseClass.h"



struct PlatformImpl
{
	POINT mousePosition{};
	HWND windowHandle{};
	WINDOWPLACEMENT windowPlacement = { sizeof(WINDOWPLACEMENT) };
	bool shouldQuit = false;
	HINSTANCE instanceHandle{};
	void CreateAndShowWindow(const wchar_t* title, int w, int h);
	void PollEvents();
	void CloseWindow();
};

namespace
{
	const wchar_t* WindowClassName = L"Hagyeong";
	LRESULT CALLBACK WndProc(HWND hwnd, UINT message,
		WPARAM wparam, LPARAM lparam);
	bool RegisterWindowClass(HINSTANCE hinstance, LPCWSTR str, int iconid);
	MouseClass mouse;
	KeyboardClass keyboard;
	Graphics gfx;
	Timer timer;
}

void PlatformImpl::CreateAndShowWindow(const wchar_t* title, int w, int h)
{
	timer.Start();

	instanceHandle = GetModuleHandleW(nullptr);
	if (const bool registered_wndclass =
		RegisterWindowClass(instanceHandle, WindowClassName, 101);
		!registered_wndclass)
	{
		MessageBox(nullptr, L"failed to register window class",
			L"Error!", MB_ICONERROR);
	}
	const DWORD window_style = WS_OVERLAPPEDWINDOW;
	int x = 0, y = 0;
	const DWORD default_extra_style = 0L;
	const HWND no_parent_window = nullptr;
	const HMENU no_menus = nullptr;
	const LPVOID nothing_for_wm_create = nullptr;

	windowHandle =
		CreateWindowEx(default_extra_style, WindowClassName,
			title, window_style, x, y, w, h, no_parent_window, no_menus,
			instanceHandle, nothing_for_wm_create);

	SetWindowLongPtr(windowHandle, GWLP_USERDATA,
		reinterpret_cast<LONG_PTR>(this));


	ShowWindow(windowHandle, SW_SHOWDEFAULT);
	UpdateWindow(windowHandle);

	gfx.Initialize(windowHandle, w, h);
}

void PlatformImpl::PollEvents()
{
	MSG msg{};
	while (PeekMessage(&msg, nullptr,
		0,
		0, PM_REMOVE) != FALSE)
	{
		if (msg.message == WM_QUIT)
		{
			shouldQuit = true;
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

void PlatformImpl::CloseWindow()
{
	PostMessage(windowHandle, WM_CLOSE, 0, 0);
	shouldQuit = true;
}


Window::Window() :pimpl(new PlatformImpl)
{
	static bool raw_input_initialized = false;
	if (raw_input_initialized == false)
	{
		RAWINPUTDEVICE rid;

		rid.usUsagePage = 0x01;
		rid.usUsage = 0x02;
		rid.dwFlags = 0;
		rid.hwndTarget = NULL;
		raw_input_initialized = true;
	}
}

Window::~Window()
{
}

bool Window::CreateAndShowWindow(const wchar_t* title, int w, int h)
{
	pimpl->CreateAndShowWindow(title, w, h);
	return true;
}

void Window::PollEvents()
{
	pimpl->PollEvents();
}

void Window::CloseWindow()
{
	pimpl->CloseWindow();
}

bool Window::ShouldQuit()
{
	return pimpl->shouldQuit;
}


namespace
{
	bool RegisterWindowClass(HINSTANCE hinstance, LPCWSTR /*str*/, int iconid)
	{
		WNDCLASSEXW wcex;

		wcex.cbSize = sizeof(WNDCLASSEX);

		wcex.style = CS_OWNDC;
		wcex.lpfnWndProc = WndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = hinstance;
		wcex.hIcon = LoadIcon(hinstance, MAKEINTRESOURCE(iconid));
		wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wcex.lpszMenuName = nullptr;
		wcex.lpszClassName = WindowClassName;
		wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(iconid));

		return RegisterClassExW(&wcex);
	}

	LRESULT CALLBACK WndProc(HWND hwnd, UINT message,
		WPARAM wparam, LPARAM lparam)
	{
		switch (message)
		{
		case WM_SIZE:
			break;
		case WM_SETFOCUS:
			break;
		case WM_KILLFOCUS:
			break;
		case WM_CLOSE:
			DestroyWindow(hwnd);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		case WM_KEYDOWN:
		{
			unsigned char keycode = static_cast<unsigned char>(wparam);
			if (keyboard.IsKeyAutoRepeat())
			{
				keyboard.OnKeyPressed(keycode);
			}
			else
			{
				const bool wasPressed = lparam & 0x4000000;

				if (!wasPressed)
				{
					keyboard.OnKeyPressed(keycode);
				}
			}
			return 0;
		}
		case WM_KEYUP:
		{
			unsigned char keycode = static_cast<unsigned char>(wparam);
			keyboard.OnKeyReleased(keycode);

			return 0;
		}
		case WM_CHAR:
		{
			unsigned char ch = static_cast<unsigned char>(wparam);
			if (keyboard.IsCharsAutoRepeat())
			{
				keyboard.OnChar(ch);
			}
			else
			{
				const bool wasPressed = lparam & 0x4000000;
				if (!wasPressed)
				{
					keyboard.OnChar(ch);
				}
			}
			return 0;
		}
		// Mouse messages
		case WM_MOUSEMOVE:
		{
			int x = LOWORD(lparam);
			int y = HIWORD(lparam);
			mouse.OnMouseMove(x, y);

			return 0;
		}
		case WM_LBUTTONDOWN:
		{
			int x = LOWORD(lparam);
			int y = HIWORD(lparam);
			mouse.OnLeftPressed(x, y);
			return 0;
		}
		case WM_RBUTTONDOWN:
		{
			int x = LOWORD(lparam);
			int y = HIWORD(lparam);
			mouse.OnRightPressed(x, y);
			return 0;
		}
		case WM_MBUTTONDOWN:
		{
			int x = LOWORD(lparam);
			int y = HIWORD(lparam);
			mouse.OnMiddlePressed(x, y);
			return 0;
		}
		case WM_LBUTTONUP:
		{
			int x = LOWORD(lparam);
			int y = HIWORD(lparam);
			mouse.OnLeftReleased(x, y);
			return 0;
		}
		case WM_RBUTTONUP:
		{
			int x = LOWORD(lparam);
			int y = HIWORD(lparam);
			mouse.OnRightReleased(x, y);
			return 0;
		}
		case WM_MBUTTONUP:
		{
			int x = LOWORD(lparam);
			int y = HIWORD(lparam);
			mouse.OnMiddleReleased(x, y);
			return 0;
		}
		case WM_MOUSEWHEEL:
		{
			int x = LOWORD(lparam);
			int y = HIWORD(lparam);
			if (GET_WHEEL_DELTA_WPARAM(wparam) > 0)
			{
				mouse.OnWheelUp(x, y);
			}
			else if (GET_WHEEL_DELTA_WPARAM(wparam) < 0)
			{
				mouse.OnWheelDown(x, y);
			}
			return 0;
		}
		case WM_INPUT:
		{
			UINT dataSize;
			GetRawInputData(reinterpret_cast<HRAWINPUT>(lparam), RID_INPUT, NULL, &dataSize, sizeof(RAWINPUTHEADER));

			if (dataSize > 0)
			{
				std::unique_ptr<BYTE[]> rawdata = std::make_unique<BYTE[]>(dataSize);
				if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lparam), RID_INPUT, rawdata.get(), &dataSize, sizeof(RAWINPUTHEADER)) == dataSize)
				{
					RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(rawdata.get());
					if (raw->header.dwType == RIM_TYPEMOUSE)
					{
						mouse.OnMouseMoveRaw(raw->data.mouse.lLastX, raw->data.mouse.lLastY);
					}
				}
			}
			return DefWindowProc(hwnd, message, wparam, lparam);
		}
		case WM_MOUSEHWHEEL:
			break;
		default:
			return DefWindowProc(hwnd, message, wparam, lparam);
		}
		return 0;
	}

}

void Window::update()
{
	while (!keyboard.CharBufferIsEmpty())
	{
		unsigned char ch = keyboard.ReadChar();
		std::string outmsg = "char: ";
		outmsg += ch;
		outmsg += "\n";
		OutputDebugStringA(outmsg.c_str());
	}

	if (mouse.IsLeftDown())
	{
		std::string outmsg = "click ";
		OutputDebugStringA(outmsg.c_str());
		gfx.model2.AdjustPosition(0.0f, 0.01f, 0.0f);
	}

	while (!mouse.EventBufferIsEmpty())
	{
		MouseEvent me = mouse.ReadEvent();
		if (mouse.IsRightDown())
		{
			std::string outmsg = "x: ";
			outmsg += std::to_string(me.GetPosX());
			outmsg += " ";
			outmsg += "y: ";
			outmsg += std::to_string(me.GetPosY());
			outmsg += "\n";
			OutputDebugStringA(outmsg.c_str());
		}
	}
	
	//float dt = timer.GetMilisecondsElapsed();
	timer.Restart();
	
	const float cameraSpeed = 0.006f;

	if (keyboard.KeyIsPressed(VK_UP))
	{
		gfx.camera.AdjustPosition(0.0f, 0.05f, 0.0f);

	}
	if (keyboard.KeyIsPressed(VK_DOWN))
	{
		gfx.camera.AdjustPosition(0.0f, -0.05f, 0.0f);

	}
	if (keyboard.KeyIsPressed(VK_RIGHT))
	{
		gfx.camera.AdjustPosition(0.05f, 0.0f, 0.0f);

	}
	if (keyboard.KeyIsPressed(VK_LEFT))
	{
		gfx.camera.AdjustPosition(-0.05f, 0.0f, 0.0f);
	}
	if (keyboard.KeyIsPressed('W'))
	{
		gfx.model4.AdjustPosition(0.0f, 0.05f, 0.0f);
	}
	if (keyboard.KeyIsPressed('S'))
	{
		gfx.model4.AdjustPosition(0.0f, -0.05f, 0.0f);
	}
	if (keyboard.KeyIsPressed('D'))
	{
		gfx.model4.AdjustPosition(0.05f, 0.0f, 0.0f);
	}
	if (keyboard.KeyIsPressed('A'))
	{
		gfx.model4.AdjustPosition(-0.05f, 0.0F, 0.0f);
	}
	if (keyboard.KeyIsPressed('R'))
	{
		gfx.model3.AdjustRotation( 0.3f);
	}
	if (keyboard.KeyIsPressed('T'))
	{
		gfx.model3.AdjustScale({1.002f,1.002f,0.0f,0.0f});
	}
	if (keyboard.KeyIsPressed('Y'))
	{
		gfx.model3.AdjustScale({ 1.002f,1.002f,0.0f });
	}
	if (keyboard.KeyIsPressed(VK_SPACE))
	{
		if(gfx.vertical == true)
		{
			gfx.vertical = false;
		}
		else
		{
			gfx.vertical = true;
		}
	}
}

void Window::renderFrame()
{
	gfx.RenderFrame();
}
