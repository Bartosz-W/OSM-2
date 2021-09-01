#ifndef INCLUDE_MOJE_KLASY_H_
#define INCLUDE_MOJE_KLASY_H_

class UAR
{
public:
	float m_out;
	float m_in;
	void setInput (float input)
	{
		m_in = input;
	}
	void Calculate ()
	{
		m_out = m_in;
	}
	float getOutput ()
	{
		return m_out;
	}
};

class PID : public UAR
{
	float m_Tp, m_Kp, m_Ki;
	float m_sum, m_Smin, m_Smax;
	float m_prevIn;
public:
	PID (float Kp, float Ti, float Tp, float Smin, float Smax)
	{
		m_Tp = Tp;
		m_Kp = Kp;
		m_Ki = Kp * Tp / Ti;
		m_in = 0;
		m_prevIn = 0;
		m_out = 0;
		m_sum = 0;
		m_Smin = Smin;
		m_Smax = Smax;
	}
	void Calculate ()
	{
		//float deltaIn = m_in - m_prevIn;
		m_sum += m_in * m_Ki;
		if (m_sum > m_Smax) m_sum = m_Smax;
		if (m_sum < m_Smin) m_sum = m_Smin;
		m_out = m_sum + m_in * m_Kp;
		/*m_sum = m_sum + m_in;
		m_prevIn = m_Kp * (m_in + m_Tp / m_Ki * m_sum);
		if (m_prevIn<0)
		{ m_prevIn = -m_prevIn; }
		if (m_prevIn > m_Smax) m_prevIn = m_Smax;
		if (m_prevIn < m_Smin) m_prevIn = m_Smin;
		m_out = m_prevIn;*/
	}
};

class CalkModel : public UAR
{
	float m_ster;
	float m_zaklo;
public:
	CalkModel (float temp)
	{
		m_in = 0;
		m_out = temp;
		m_ster = 0;
		m_zaklo = 0;
	}
	void setInput (float syg_ster,float zaklo)
	{
		m_ster = syg_ster;
		m_zaklo = zaklo;
	}
	void Calculate ()
	{
		m_out = m_out - 0.5*m_ster + m_zaklo;
	}
};

#endif /* INCLUDE_MOJE_KLASY_H_ */
