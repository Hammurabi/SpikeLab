#pragma once
#include <vector>
#include <mutex>
#include <atomic>
#include <map>
#include "../NervousSystem.h"

class GLFWwindow;
class FRenderer
{
public:
	FRenderer(const unsigned int &wWidth = 1920, const unsigned int &wHeight = 1080);
	~FRenderer();
	void show(FNervousSystem& nervousSystem);
	void tick();
	void render(FNervousSystem& nervousSystem);
	bool shouldClose();
	void close();
	bool isKeyDown(const int& key);
	bool isKeyPressed(const int& key);
	bool isKeyReleased(const int& key);
	static std::vector<FKey> pollKeys();
	static void onKey(int Key, int ScanCode, int Action, int Mods);
	void onKeyPress(const int& key, std::function<void(FNervousSystem&)> action);
	void onKeyDown(const int& key, std::function<void(FNervousSystem&)> action);
	void onKeyRelease(const int& key, std::function<void(FNervousSystem&)> action);
private:
	bool						mKeys[500];
	static std::vector<FKey>	keys;
	GLFWwindow*					mWindow;
	unsigned int				mWindowWidth;
	unsigned int				mWindowHeight;
	std::mutex					mMutex;
	std::atomic<bool>			mKeepAlive;
	std::map<int, std::function<void(FNervousSystem&)>>	mKeyPressAction;
	std::map<int, std::function<void(FNervousSystem&)>>	mKeyDownAction;
	std::map<int, std::function<void(FNervousSystem&)>>	mKeyReleaseAction;
};