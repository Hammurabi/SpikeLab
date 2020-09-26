#include "Neuron.h"
#include "Equations.h"
#include "Time.h"
#include <algorithm>
#include <iostream>
#include <random>

size_t FNeuron::TOTAL_NEURONS = 0;


FNeuron::FNeuron(const unsigned int& type)
	: mType(0), mMembranePotential(-70.0), mLastFired(0.0),
	mLastTimeStep(getTime()),
	mPositiveVoltageGate(1.0), mNegativeVoltageGate(1.0), mId(TOTAL_NEURONS ++),
	mExcites(0), mInhibits(0), mNextPotential(-70.0), mStage(0)
{
}

void FNeuron::tick(const double& currentTime)
{
	double delta = currentTime - mLastTimeStep;
	mLastTimeStep = currentTime;

	double depolarization		= pow(std::min((mMembranePotential + 100.0) / 50.0, 1.0), 16.0);
	double hyperpolarization	= pow(std::min((mMembranePotential + 100.0) / 110.0, 1.0), 8.0);
	double repolarization		= pow(std::min(11.0 / (mMembranePotential + 100.0), 1.0), 8.0);
	double repolarizationv		= pow(std::min((mMembranePotential + 100.0) / 30.0, 1.0), 16.0);

	double total				= std::max(mExcites + mInhibits, 0.0001);
	auto positivePotential		= (mExcites / (double)total);
	auto negativePotential		= (mInhibits / (double)total);
	auto potentialChange = pow(lerp(positivePotential, 0.0, negativePotential), 4.0);

	switch (mStage)
	{
	default:
	case 0:
	{
		mNextPotential = lerp(lerp(-70.0, 50.0, depolarization), mMembranePotential + potentialChange, std::min(1.0, mExcites / std::max(1.0, mInhibits)) * (1.0 - depolarization));
		mStage = (int)lerp(0.0, 1.2, hyperpolarization);
	}
		break;
	case 1:
		mNextPotential = lerp(mMembranePotential, -95.0, std::max(0.12, repolarization));
		mStage = (int) lerp(lerp(1.0, 2.2, repolarization), 0, std::min(1.0, (std::min(1.0, mExcites / std::max(1.0, mInhibits)) * repolarization) * 1.19995));
		mLastFired = currentTime;
		break;
	case 2:
	{
		mNextPotential = lerp(mMembranePotential, -70.0, std::max(0.12, repolarizationv));
		mStage = ((int) lerp(1.0, 2.2, repolarizationv) + 1) % 3;
	}
		break;
	}

	mMembranePotential	= lerp(mMembranePotential, mNextPotential, 0.4 / std::max((double)mStage, 1.0));
	mExcites			= lerp(mExcites, 0.0, 0.08);
	mInhibits			= lerp(mInhibits, 0.0, 0.08);

	for (auto& synapse : mSynapses)
	{
		switch (synapse.second.mType)
		{
		case ENeuroTransmitter::Excitatory:
			synapse.second.mNeuron->excite(synapse.second.mStrength * hyperpolarization);
			break;
		default:
			synapse.second.mNeuron->inhibit(synapse.second.mStrength * hyperpolarization);
			break;
		}
	}
}

void FNeuron::prep(const double& currentTime, std::vector<FNeuronInfo>& firedRecently, std::vector<FNeuronInfo>& notFiredRecently)
{
	FNeuronInfo info{};
	info.mLastFired = mLastFired;
	info.mNeuron = this;

	if (hasFiredRecently(currentTime))
	{
		firedRecently.push_back(info);
	}
	else
	{
		notFiredRecently.push_back(info);
	}
}

void FNeuron::updateSynapses(const double& currentTime, std::vector<FNeuronInfo>& firedRecently, std::vector<FNeuronInfo>& notFiredRecently)
{
	if (hasFiredRecently(currentTime))
	{
		if (mSynapses.size() < 1000)
		{
			for (auto& neuron : firedRecently)
			{
				if (neuron.mNeuron != this && neuron.mNeuron->mLastFired > mLastFired && mSynapses.find(neuron.mNeuron->mId) == mSynapses.end())
				{
					makeExcitatory(std::max(0.1, abs((neuron.mLastFired / mLastFired) * 10)), neuron.mNeuron);
				}
			}

			for (auto& neuron : notFiredRecently)
			{
				if (neuron.mNeuron != this && mSynapses.find(neuron.mNeuron->mId) == mSynapses.end())
				{
					double v = abs((neuron.mLastFired / mLastFired) * 5.0);
					if (v > 0.05)
					{
						makeInhibitory(v, neuron.mNeuron);
					}
				}
			}
		}

		auto iterator = mSynapses.begin();
		while (iterator != mSynapses.end())
		{
			switch (iterator->second.mType)
			{
			case ENeuroTransmitter::Excitatory:
				if (iterator->second.mNeuron->hasFiredRecently(currentTime))
				{
					iterator->second.mStrength = lerp(iterator->second.mStrength, iterator->second.mStrength * 1.5, 0.5);
				}
				else
				{
					iterator->second.mStrength = lerp(iterator->second.mStrength, 0, 0.5);

					if (iterator->second.mStrength <= 1)
					{
						iterator->second.mChanges++;
						iterator->second.mType = Inhibitory;
					}
				}
				break;
			default:
				if (!iterator->second.mNeuron->hasFiredRecently(currentTime))
				{
					iterator->second.mStrength = lerp(iterator->second.mStrength, iterator->second.mStrength * 1.5, 0.5);
				}
				else
				{
					iterator->second.mStrength = lerp(iterator->second.mStrength, 0, 0.5);

					if (iterator->second.mStrength <= 1)
					{
						iterator->second.mChanges++;
						iterator->second.mType = Excitatory;
					}
				}
				break;
			}

			if (iterator->second.mChanges > 5)
			{
				mSynapses.erase(iterator ++);
			}
			else
			{
				iterator++;
			}
		}
	}
}

bool FNeuron::hasFired(const double& currentTime)
{
	return currentTime - mLastFired <= 0.005;
}

bool FNeuron::hasFiredRecently(const double& currentTime)
{
	return currentTime - mLastFired <= 0.05;
}

void FNeuron::makeExcitatory(const double& strength, FNeuron* neuron)
{
	mSynapses.insert(std::pair<size_t, FSynapse>(neuron->mId, FSynapse(ENeuroTransmitter::Excitatory, strength, neuron)));
}

void FNeuron::makeInhibitory(const double& strength, FNeuron* neuron)
{
	mSynapses.insert(std::pair<size_t, FSynapse>(neuron->mId, FSynapse(ENeuroTransmitter::Inhibitory, strength, neuron)));
}

double FNeuron::getExcites()
{
	return mExcites;
}

double FNeuron::getInhibits()
{
	return mInhibits;
}

FReceptor::FReceptor(const unsigned int& numReceptors, const double& percentInactive)
	: mNumReceptors(numReceptors), mPercentInactive(percentInactive)
{
}

FSynapse::FSynapse(const ENeuroTransmitter& type, const double& strength, FNeuron* neuron)
	: mType(type), mStrength(strength), mNeuron(neuron), mChanges(0)
{
}
