#pragma once
#include "Plotting/Plot.h"
#include "Input/Key.h"
#include "Neuron.h"
#include <set>
#include <vector>
#include <map>
#include <string>
#include <mutex>
#include <atomic>
#include <functional>

struct FCommand
{
	size_t index;
	double value;
};

class FNervousSystem
{
public:
	FNervousSystem();
	/*
		num:	the number of neurons to add
		type:	the type of the added neurons
		excitableTypes: a set of types that these new neurons could excite
		excitableTypes: a set of types that these new neurons could inhibit
	*/
	void addNeurons(const size_t& num, const unsigned int& type,
		const std::set<unsigned int>& excitableTypes, const std::set<unsigned int>& inhibitableTypes);
	void init();
	void start();
	void tick();
	void render();
	void shutdown();

	// excite neuron "neuron" with "x"
	void excite(const size_t& neuron, const double x);
	// inhibit neuron "neuron" with "x"
	void inhibit(const size_t& neuron, const double x);

	// excite all neurons with "x"
	void exciteAll(const double x);
	// inhibit all neurons with "x"
	void inhibitAll(const double x);

	size_t getPlotSize();
private:
	FPlot					mPlot;
	std::vector<FNeuron>	mNeurons;
	std::map<unsigned int, std::string> mTypeNames;
	std::atomic<bool>		mKeepAlive;
	std::mutex				mMutex;
	double					mPlotTimer;
	size_t					mPlotSize;

	std::mutex				mCommandMutex;
	std::vector<FCommand>	mExciteCommands;
	std::vector<FCommand>	mInhibitCommands;
	std::vector<double>		mExciteAll;
	std::vector<double>		mInhibitAll;
};