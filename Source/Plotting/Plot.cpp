#include "Plot.h"
#include <glad/glad.h>
#include "../Time.h"

FPlot::FPlot(const size_t& plotSize)
	: mSize(plotSize), mDeleted(0), mBeginElementRange(0), mEndElementRange(1)
{
}

void FPlot::init(const size_t& numElements, const double& initialValue)
{
	std::vector<double> data;

	for (size_t i = 0; i < numElements; i++)
	{
		data.push_back(initialValue);
	}

	std::lock_guard<std::mutex> lock(mMutex);
	for (size_t i = 0; i < mSize; i++)
	{
		mSteps.push_back(data);
	}
}

void FPlot::add(const std::vector<double>& data)
{
	std::lock_guard<std::mutex> lock(mMutex);
	mSteps.push_back(data);
}

void FPlot::erase(const size_t& numElements)
{
	std::lock_guard<std::mutex> lock(mMutex);
	mSteps.erase(mSteps.begin(), mSteps.begin() + numElements);
}

const std::vector<double>& FPlot::operator[](const size_t& index)
{
	std::lock_guard<std::mutex> lock(mMutex);
	return mSteps[index + mDeleted];
}

void FPlot::tick()
{
	std::lock_guard<std::mutex> lock(mMutex);
	if (mSteps.size() > mSize)
	{
		//TODO: save elements to file
		mSteps.erase(mSteps.begin());
	}
}

void FPlot::render()
{
	double x = -1.f;
	double w = 1.f;
	double y = 1.f;
	double h = -1.f;

	glBegin(GL_LINES);

	glColor3f(0.012f, 0.135f, 0.012f);

	glColor3f(0.12f, 1.0f, 0.12f);
	size_t currentPlotSize = getTotalActiveElements();

	for (int i = 1; i < getPlotSize(); i++)
	{
		if (i >= currentPlotSize)
		{
			break;
		}

		auto previousPlot = (*this)[i - 1];
		auto currentPlot = (*this)[i];

		for (int x = mBeginElementRange; x < mEndElementRange; x++)
		{
			double xOfPrevPlt = ((i - 1) / 1000.0) * 2.0 - 1.0;
			double xOfCurrPlt = (i / 1000.0) * 2.0 - 1.0;

			double yOfPrevPlt = (previousPlot[x] + 100.0) * 0.0066666666666667;
			double yOfCurrPlt = (currentPlot[x] + 100.0) * 0.0066666666666667;

			glVertex3f(xOfPrevPlt, yOfPrevPlt, 0.0f);
			glVertex3f(xOfCurrPlt, yOfCurrPlt, 0.0f);
		}
	}


	glColor3f(0.012f, 0.035f, 0.012f);

	glVertex3f(0.0f, y, 0.0f);
	glVertex3f(0.0f, 0, 0.0f);

	glColor3f(0.5f, 0.5f, 0.012f);

	glVertex3f(x, 50.0 * 0.0066666666666667, 0.0f);
	glVertex3f(0, 50.0 * 0.0066666666666667, 0.0f);

	glColor3f(0.125f, 0.45f, 0.85f);
	glVertex3f(x, 10.0 * 0.0066666666666667, 0.0f);
	glVertex3f(0, 10.0 * 0.0066666666666667, 0.0f);

	glColor3f(0.125f, 0.45f, 0.5f);
	glVertex3f(x, 30.0 * 0.0066666666666667, 0.0f);
	glVertex3f(0, 30.0 * 0.0066666666666667, 0.0f);

	glColor3f(0.85f, 0.25f, 0.012f);
	glVertex3f(x, 100.0 * 0.0066666666666667, 0.0f);
	glVertex3f(0, 100.0 * 0.0066666666666667, 0.0f);

	glColor3f(0.85f, 0.45f, 0.012f);
	glVertex3f(x, 110.0 * 0.0066666666666667, 0.0f);
	glVertex3f(0, 110.0 * 0.0066666666666667, 0.0f);

	glColor3f(0.45f, 0.5f, 0.012f);
	glVertex3f(x, 120.0 * 0.0066666666666667, 0.0f);
	glVertex3f(0, 120.0 * 0.0066666666666667, 0.0f);

	glColor3f(0.45f, 0.85f, 0.012f);
	glVertex3f(x, 130.0 * 0.0066666666666667, 0.0f);
	glVertex3f(0, 130.0 * 0.0066666666666667, 0.0f);

	glColor3f(0.12f, 0.35f, 0.12f);

	glVertex3f(x, 0.0f, 0.0f);
	glVertex3f(0, 0.0f, 0.0f);

	glEnd();
}
