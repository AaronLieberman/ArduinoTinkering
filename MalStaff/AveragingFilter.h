#include <deque>

class AveragingFilter
{
private:
	int m_maxSampleCount;
	std::deque<float> m_samples;
	float m_currentValue = 0;

public:
	
	AveragingFilter(int maxSampleCount)
		: m_maxSampleCount(maxSampleCount)
	{
	}

	float addSample(float value)
	{
		if (m_samples.size() >= m_maxSampleCount)
		{
			m_samples.pop_front();
		}
	
		m_samples.push_back(value);

		float result = 0;
		for (auto v : m_samples)
		{
			result += v;
		}
		
		m_currentValue = result / m_samples.size();
	}
	
	float getCurrent() const
	{
		return m_currentValue;
	}

};
