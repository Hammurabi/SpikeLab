#include "NervousSystem.h"
#include "Time.h"
#include <glad/glad.h>

FNervousSystem::FNervousSystem()
	: mPlot(500), mKeepAlive(true), mPlotTimer(0.0), mPlotSize(0)
{
}

void FNervousSystem::addNeurons(const size_t& num, const unsigned int& type, const std::set<unsigned int>& excitableTypes, const std::set<unsigned int>& inhibitableTypes)
{
	std::lock_guard<std::mutex> lock(mMutex);
	for (size_t i = 0; i < num; i++)
	{
		FNeuron neuron(type);
		neuron.setExcitableTypes(excitableTypes);
		neuron.setInhibitableTypes(inhibitableTypes);
		mNeurons.push_back(neuron);
	}
}

void FNervousSystem::init()
{
	mPlot.init(mNeurons.size(), -70.0);
	mPlot.setRange(0, 1);
}

#include <iostream>

void FNervousSystem::start()
{
	int keys[348];
	for (int i = 0; i < 348; i++)
	{
		keys[i] = false;
	}

	double timeCounter = getTime();

	for (int i = 0; i < 25; i++)
	{
		mNeurons[0].makeExcitatory(5, &mNeurons[25 + i]);
		mNeurons[25 + i].makeExcitatory(1.5, &mNeurons[50 + i]);
		mNeurons[50 + i].makeExcitatory(0.1, &mNeurons[100]);
	}

	for (int i = 0; i < 400; i++)
	{
		mNeurons[i + 100].makeExcitatory(5, &mNeurons[i + 101]);
	}

	mNeurons[150].makeExcitatory(5, &mNeurons[0]);
	mNeurons[151].makeExcitatory(5, &mNeurons[0]);
	mNeurons[152].makeExcitatory(5, &mNeurons[0]);



	mNeurons[1000].makeExcitatory(5, &mNeurons[1001]);
	mNeurons[1001].makeExcitatory(5, &mNeurons[1002]);
	mNeurons[1002].makeExcitatory(5, &mNeurons[1003]);
	mNeurons[1003].makeExcitatory(5, &mNeurons[1004]);
	mNeurons[1004].makeExcitatory(5, &mNeurons[1005]);
	mNeurons[1005].makeExcitatory(5, &mNeurons[1000]);

	mNeurons[1000].excite(25);


	while (mKeepAlive.load())
	{	
		double time = getTime();

		if (time - timeCounter >= (1.0 / 500.0))
		{
			{
				std::lock_guard<std::mutex> lock(mCommandMutex);
				for (auto& x : mExciteCommands)
				{
					mNeurons[x.index].excite(x.value);
				}

				for (auto& x : mInhibitCommands)
				{
					mNeurons[x.index].inhibit(x.value);
				}
				
				// this is too slow.
				for (auto& x : mExciteAll)
				{
					for (auto& n : mNeurons)
					{
						n.excite(x);
					}
				}

				for (auto& x : mInhibitAll)
				{
					for (auto& n : mNeurons)
					{
						n.inhibit(x);
					}
				}

				mExciteCommands.clear();
				mInhibitCommands.clear();
				mExciteAll.clear();
				mInhibitAll.clear();
			}

			std::vector<FNeuronInfo> firedRecently;
			std::vector<FNeuronInfo> notFiredRecently;

			std::vector<double> plots;
			for (auto& neuron : mNeurons)
			{
				plots.push_back(neuron.getMembranePotential());
				neuron.tick(time);
			}

			mPlot.add(plots);

			for (auto& neuron : mNeurons)
			{
				neuron.prep(time, firedRecently, notFiredRecently);
			}

			for (auto& neuron : mNeurons)
			{
				// neuron.updateSynapses(time, firedRecently, notFiredRecently);
			}

			timeCounter = time;
		}
	}
}

void FNervousSystem::tick()
{
	// erase as many elements as it takes to stay at 500 plot points
	if (mPlot.getTotalActiveElements() > 500)
	{
		mPlot.erase();
	}

	mPlotSize = mPlot.getTotalActiveElements();

	mPlotTimer = 0.0;
}

void FNervousSystem::render()
{
	mPlot.render();

	glBegin(GL_QUADS);

	double currentTime = getTime();

	for (size_t i = 0; i < mNeurons.size(); i++)
	{
		if (mNeurons[i].hasFired(currentTime))
		{
			glColor3f(0.95, 0.95, 0.35);
		}
		else if (mNeurons[i].hasFiredRecently(currentTime))
		{
			glColor3f(0.5, 0.5, 0.35);
		}
		else
		{
			glColor3f(0.35, 0.35, 0.35);
		}

		double x = (i % 50) * 0.015;
		double y = (i / 50) * 0.015;

		glVertex2f(-0.95 + (x + 0.005), -0.95 + (y + 0.005));
		glVertex2f(-0.95 + (x + -0.005), -0.95 + (y + 0.005));
		glVertex2f(-0.95 + (x + -0.005), -0.95 + (y + -0.005));
		glVertex2f(-0.95 + (x + 0.005), -0.95 + (y + -0.005));
	}

	glEnd();
}

void FNervousSystem::shutdown()
{
	mKeepAlive.store(false);
}

void FNervousSystem::excite(const size_t& neuron, const double x)
{
	std::lock_guard<std::mutex> lock(mCommandMutex);
	mExciteCommands.push_back(FCommand{neuron, x});
}

void FNervousSystem::inhibit(const size_t& neuron, const double x)
{
	std::lock_guard<std::mutex> lock(mCommandMutex);
	mInhibitCommands.push_back(FCommand{ neuron, x });
}

void FNervousSystem::exciteAll(const double x)
{
	std::lock_guard<std::mutex> lock(mCommandMutex);
	mExciteAll.push_back(x);
}

void FNervousSystem::inhibitAll(const double x)
{
	std::lock_guard<std::mutex> lock(mCommandMutex);
	mInhibitAll.push_back(x);
}

size_t FNervousSystem::getPlotSize()
{
	return mPlotSize;
}
