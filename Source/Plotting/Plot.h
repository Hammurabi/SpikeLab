#pragma once
#include <vector>
#include <mutex>

class FPlot
{
public:
	FPlot(const size_t& plotSize);
	// add an element to the right-most side of the plot
	void init(const size_t& numElements, const double& initialValue);
	// add an element to the right-most side of the plot
	void add(const std::vector<double>& data);
	// erase n-elements from the left-most side of the plot
	void erase(const size_t& numElements = 1);
	// return the element at index n + mDeleted
	const std::vector<double>& operator[] (const size_t& index);
	// return the size of all elements in the plot
	inline size_t getTotalElements()
	{
		return mSteps.size();
	}
	// return the size of active elements in the plot
	inline size_t getTotalActiveElements()
	{
		return mSteps.size() - mDeleted;
	}
	inline size_t getPlotSize()
	{
		return mSize;
	}
	void tick();
	void render();

	// set the range of elements to plot
	inline void setRange(const size_t& begin, const size_t& end)
	{
		mBeginElementRange = begin;
		mEndElementRange = end;
	}
private:
	std::vector<std::vector<double>>	mSteps;
	size_t								mSize;
	size_t								mDeleted;
	size_t								mBeginElementRange;
	size_t								mEndElementRange;
	std::mutex							mMutex;
};