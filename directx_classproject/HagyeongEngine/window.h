/*
* Kim Hagyeong (hakyung.kim)
* Class Project
* cs200
* FALL 2019
*/
#pragma once

#include <memory>
#include <functional>
#include "graphic.h"
#include "Timer.h"

struct PlatformImpl;

class Window
{
public:
	Window();
	~Window();

	bool CreateAndShowWindow(const wchar_t* title, int w, int h);
	void PollEvents();
	void CloseWindow();
	bool ShouldQuit();
	

	void update();
	void renderFrame();

	private:
	std::unique_ptr<PlatformImpl> pimpl{};

};
