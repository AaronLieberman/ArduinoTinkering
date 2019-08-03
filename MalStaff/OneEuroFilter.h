float smoothing_factor(float t_e, float cutoff)
{
  float r = 2 * PI * cutoff * t_e;
  return r / (r + 1);
}

float exponential_smoothing(float a, float x, float x_prev)
{
  return a * x + (1 - a) * x_prev;
}

class OneEuroFilter
{
private:
  float m_x_prev;
  float m_dx_prev;
  float m_t_prev;
  float m_min_cutoff;
  float m_d_cutoff;
  float m_beta;

public:

  OneEuroFilter(float t0, float x0, float dx0 = 0.0f, float min_cutoff = 1.0f, float beta = 0.0f, float d_cutoff = 1.0f)
  {
    m_min_cutoff = float(min_cutoff);
    m_beta = float(beta);
    m_d_cutoff = float(d_cutoff);
    m_x_prev = float(x0);
    m_dx_prev = float(dx0);
    m_t_prev = float(t0);
  }

  float sample(float t, float x)
  {
    float t_e = t - m_t_prev;

    // The filtered derivative of the signal.
    float a_d = smoothing_factor(t_e, m_d_cutoff);
    float dx = (x - m_x_prev) / t_e;
    float dx_hat = exponential_smoothing(a_d, dx, m_dx_prev);

    // The filtered signal.
    float cutoff = m_min_cutoff + m_beta* abs(dx_hat);
    float a = smoothing_factor(t_e, cutoff);
    float x_hat = exponential_smoothing(a, x, m_x_prev);

    // Memorize the previous values.
    m_x_prev = x_hat;
    m_dx_prev = dx_hat;
    m_t_prev = t;
    return x_hat;
  }
  
};