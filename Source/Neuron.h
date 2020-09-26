#pragma once
#include <vector>
#include <map>
#include <set>

enum ENeuroTransmitter
{
	Excitatory,
	Inhibitory,
};

class FNeuron;

struct FSynapse
{
	ENeuroTransmitter			mType;
	double						mStrength;
	unsigned int				mChanges;
	FNeuron*					mNeuron;

	FSynapse(const ENeuroTransmitter& type = Inhibitory, const double& strength = 0, FNeuron* neuron = nullptr);
};

struct FReceptor
{
	unsigned int	mNumReceptors;
	double			mPercentInactive;
	FReceptor(const unsigned int& numReceptors, const double& percentInactive);
};

struct FNeuronInfo
{
	FNeuron*	mNeuron;
	double		mLastFired;
};

class FNeuron
{
public:
	FNeuron(const unsigned int& type = 0);
	inline void setExcitableTypes(const std::set<unsigned int>& types) { mExcitable = types; }
	inline void setInhibitableTypes(const std::set<unsigned int>& types) { mInhibitable = types; }

	void tick(const double& currentTime);
	void prep(const double& currentTime, std::vector<FNeuronInfo>& firedRecently, std::vector<FNeuronInfo>& notFiredRecently);
	void updateSynapses(const double& currentTime, std::vector<FNeuronInfo>& firedRecently, std::vector<FNeuronInfo>& notFiredRecently);

	// returns true if this neuron fired within the last 5 milliseconds
	bool hasFired(const double& currentTime);
	// returns true if this neuron fired within the last 50 milliseconds
	bool hasFiredRecently(const double& currentTime);
	// creates an excitatory synapse of "strength" to "neuron"
	void makeExcitatory(const double& strength, FNeuron* neuron);
	// creates an inhibitory synapse of "strength" to "neuron"
	void makeInhibitory(const double& strength, FNeuron* neuron);

	inline unsigned int getType() { return mType; }
	inline size_t getId() { return mId; }
	inline double getPositiveVG() { return mPositiveVoltageGate; }
	inline double getNegativeVG() { return mNegativeVoltageGate; }
	inline double getMembranePotential() { return mMembranePotential; }
	// returns the number of excitatory pressure this neuron is under
	double getExcites();
	// returns the number of inhibitory pressure this neuron is under
	double getInhibits();
	// returns the last time this neuron fired in ms
	inline double getLastFired() { return mLastFired; }

	inline void excite(const double &num = 2) { mExcites += num; }
	inline void inhibit(const double & num = 2) { mInhibits += num; }
private:
	static size_t					TOTAL_NEURONS;
	size_t							mId;
	unsigned int					mType;
	double							mLastTimeStep;
	double							mPositiveVoltageGate;
	double							mNegativeVoltageGate;

	double							mMembranePotential;
	double							mNextPotential;
	int								mStage;
	double							mLastFired;
	double							mExcites;
	double							mInhibits;
	std::map<size_t, FSynapse>		mSynapses;

	std::set<unsigned int>			mExcitable;
	std::set<unsigned int>			mInhibitable;
};