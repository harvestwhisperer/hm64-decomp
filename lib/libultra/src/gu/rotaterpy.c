#include "guint.h"

 /*
  *  Return rotation matrix given roll, pitch, and yaw in degrees
  *
  */

void guRotateRPYF(float mf[4][4], float r, float p, float h)
{
	static float	dtor = 3.1415926 / 180.0;
	float	sinr, sinp, sinh;
	float	cosr, cosp, cosh;

	r *= dtor;
	p *= dtor;
	h *= dtor;
	sinr = sinf(r);
	cosr = cosf(r);
	sinp = sinf(p);
	cosp = cosf(p);
	sinh = sinf(h);
	cosh = cosf(h);

	guMtxIdentF(mf);

	mf[0][0] = cosp * cosh;
	mf[0][1] = cosp * sinh;
	mf[0][2] = -sinp;

	mf[1][0] = sinr * sinp * cosh - cosr * sinh;
	mf[1][1] = sinr * sinp * sinh + cosr * cosh;
	mf[1][2] = sinr * cosp;

	mf[2][0] = cosr * sinp * cosh + sinr * sinh;
	mf[2][1] = cosr * sinp * sinh - sinr * cosh;
	mf[2][2] = cosr * cosp;
}

void guRotateRPY(Mtx* m, float r, float p, float h)
{
	Matrix	mf;

	guRotateRPYF(mf, r, p, h);

	guMtxF2L(mf, m);
}
