// SpikerLab.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Source/Rendering/Renderer.h"
#include <thread>
#include <GLFW/glfw3.h>
#include "SpikerLab.h"
#include "Source/NervousSystem.h"

std::atomic<bool> keepAlive(true);

std::vector<FNeuron> neurons(2500);

FPlot plot(500);

int main()
{
	std::set<unsigned int> blank;

	FNervousSystem system;
	system.addNeurons(2500, 0, blank, blank);
	system.init();

	std::thread secondaryThread(&FNervousSystem::start, &system);

	FRenderer renderer(1920, 1080);
	renderer.onKeyPress(GLFW_KEY_UP, [](FNervousSystem& system) { system.excite(0, 25); });
	renderer.onKeyPress(GLFW_KEY_SPACE, [](FNervousSystem& system) { system.inhibitAll(50); });
	renderer.show(system);
	renderer.close();
	secondaryThread.join();
	return 0;
}