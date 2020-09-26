#include "Renderer.h"
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <iostream>
#include "../Time.h"

FRenderer::FRenderer(const unsigned int& wWidth, const unsigned int& wHeight)
	: mWindowWidth(wWidth), mWindowHeight(wHeight), mWindow(nullptr), mKeepAlive(true)
{
}

FRenderer::~FRenderer()
{
}

void FRenderer::show(FNervousSystem& nervousSystem)
{
	if (!glfwInit())
	{
		std::cout << "Unable to initialize GLFW.\n";
		nervousSystem.shutdown();
		return;
	}

	auto Primary = glfwGetPrimaryMonitor();

	GLFWmonitor* Monitor = Primary;
	int GLFWMonitorID = 0;

	const GLFWvidmode* VideoMode = glfwGetVideoMode(Monitor);

	auto WindowX = mWindowWidth;
	auto WindowY = mWindowHeight;

	auto RefreshRate = (unsigned int)VideoMode->refreshRate;


	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);

	mWindow = glfwCreateWindow(WindowX, WindowY, "SpikeLab", NULL, NULL);

	int MonitorX, MonitorY;
	glfwGetMonitorPos(Monitor, &MonitorX, &MonitorY);

	int windowWidth, windowHeight;
	glfwGetWindowSize(mWindow, &windowWidth, &windowHeight);

	glfwSetWindowPos(mWindow, MonitorX + (VideoMode->width - WindowX) / 2, MonitorY + (VideoMode->height - WindowY) / 2);

	if (!mWindow)
	{
		glfwTerminate();
		std::cout << "could not create a GLFW window with the provided parameters." << std::endl;
		nervousSystem.shutdown();
		return;
	}

	glfwSetWindowSizeCallback(mWindow, [](GLFWwindow* window, int Width, int Height) {
		glViewport(0,
			0,
			Width,
			Height);
	});
	glfwSetDropCallback(mWindow, [](GLFWwindow* window, int Count, const char** Paths) {});
	glfwSetKeyCallback(mWindow, [](GLFWwindow* window, int Key, int ScanCode, int Action, int Mods) {
		FRenderer::onKey(Key, ScanCode, Action, Mods);
	});

	glfwMakeContextCurrent(mWindow);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << ("opengl context could not be initiated.") << std::endl;
		nervousSystem.shutdown();
		return;
	}

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &vao);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);
	glEnableVertexAttribArray(5);
	glEnableVertexAttribArray(6);
	glEnableVertexAttribArray(7);

	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	//if (Debug) {
	//	glEnable(GL_DEBUG_OUTPUT);
	//	glDebugMessageCallback(MessageCallback, 0);
	//}

	glEnable(GL_DEPTH_CLAMP);

	glClearColor(1., 1., 1., 1.);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	double LastTime = getTime();
	double FrameCounter = 0;
	double UnprocessedTime = 0;
	int Frames = 0;
	bool KeepRunning = true;

	glEnable(GL_DEPTH_TEST);

	while (KeepRunning)
	{
		bool ShouldRender = false;

		double StartTime = getTime();
		double PassedTime = StartTime - LastTime;
		LastTime = StartTime;

		UnprocessedTime += PassedTime;
		FrameCounter += PassedTime;

		if (FrameCounter >= 1.0)
		{
			double TotalTime = ((1000.0 * FrameCounter) / ((double)Frames));

			printf("Total Time:                             %f ms -- total frames %i\n\n", TotalTime, nervousSystem.getPlotSize());
			Frames = 0;
			FrameCounter = 0;
		}

		while (UnprocessedTime > FrameTime)
		{
			if (shouldClose())
			{
				KeepRunning = false;
				nervousSystem.shutdown();
			}

			for (auto& x : mKeyPressAction)
			{
				if (isKeyDown(x.first) && !mKeys[x.first])
				{
					x.second(nervousSystem);
				}
			}

			for (auto& x : mKeyDownAction)
			{
				if (isKeyDown(x.first))
				{
					x.second(nervousSystem);
				}
			}

			for (auto& x : mKeyReleaseAction)
			{
				if (!isKeyDown(x.first) && mKeys[x.first])
				{
					x.second(nervousSystem);
				}
			}

			nervousSystem.tick();

			tick();
			keys.clear();

			ShouldRender = true;

			UnprocessedTime -= FrameTime;
		}

		if (ShouldRender)
		{
			render(nervousSystem);
			Frames++;
		}
	}
}

void FRenderer::tick()
{
	keys.clear();

	for (int i = 0; i < 348; i++)
	{
		mKeys[i] = glfwGetKey(mWindow, i);
	}
	glfwPollEvents();
}

void FRenderer::render(FNervousSystem& nervousSystem)
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	nervousSystem.render();
	glfwSwapBuffers(mWindow);
}

bool FRenderer::shouldClose()
{
	return (bool)glfwWindowShouldClose(mWindow);
}

void FRenderer::close()
{
	if (mWindow)
	{
		glfwDestroyWindow(mWindow);
	}

	glfwTerminate();

	mKeepAlive.store(false);
}

bool FRenderer::isKeyDown(const int& key)
{
	return glfwGetKey(mWindow, key);
}

bool FRenderer::isKeyPressed(const int& key)
{
	return isKeyDown(key) && !mKeys[key];
}

bool FRenderer::isKeyReleased(const int& key)
{
	return !isKeyDown(key) && mKeys[key];
}

std::vector<FKey> FRenderer::pollKeys()
{
	std::vector<FKey> result(keys);

	keys.clear();

	return keys;
}

std::vector<FKey> FRenderer::keys;

void FRenderer::onKey(int Key, int ScanCode, int Action, int Mods)
{
	keys.push_back(FKey{ Key, ScanCode, Action, Mods });
}

void FRenderer::onKeyPress(const int& key, std::function<void(FNervousSystem&)> action)
{
	mKeyPressAction.insert(std::pair<int, std::function<void(FNervousSystem&)>>(key, action));
}

void FRenderer::onKeyDown(const int& key, std::function<void(FNervousSystem&)> action)
{
	mKeyDownAction.insert(std::pair<int, std::function<void(FNervousSystem&)>>(key, action));
}

void FRenderer::onKeyRelease(const int& key, std::function<void(FNervousSystem&)> action)
{
	mKeyReleaseAction.insert(std::pair<int, std::function<void(FNervousSystem&)>>(key, action));
}
