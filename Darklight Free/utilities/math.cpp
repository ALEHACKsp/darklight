#include "math.h"

// used: modules definitons, convar, globals interfaces
#include "../core/interfaces.h"

bool M::Setup()
{
	RandomSeed = (RandomSeedFn)GetProcAddress(GetModuleHandle(VSTDLIB_DLL), _("RandomSeed"));
	if (RandomSeed == nullptr)
		return false;

	RandomFloat = (RandomFloatFn)GetProcAddress(GetModuleHandle(VSTDLIB_DLL), _("RandomFloat"));
	if (RandomFloat == nullptr)
		return false;

	RandomFloatExp = (RandomFloatExpFn)GetProcAddress(GetModuleHandle(VSTDLIB_DLL), _("RandomFloatExp"));
	if (RandomFloatExp == nullptr)
		return false;

	RandomInt = (RandomIntFn)GetProcAddress(GetModuleHandle(VSTDLIB_DLL), _("RandomInt"));
	if (RandomInt == nullptr)
		return false;

	RandomGaussianFloat = (RandomGaussianFloatFn)GetProcAddress(GetModuleHandle(VSTDLIB_DLL), _("RandomGaussianFloat"));
	if (RandomGaussianFloat == nullptr)
		return false;

	return true;
}

void M::VectorAngles(const Vector& vecForward, QAngle& angView)
{
	float flPitch, flYaw;

	if (vecForward[PITCH] == 0.f && vecForward[YAW] == 0.f)
	{
		flPitch = (vecForward[ROLL] > 0.f) ? 270.f : 90.f;
		flYaw = 0.f;
	}
	else
	{
		flPitch = std::atan2f(-vecForward[ROLL], vecForward.Length2D()) * 180.f / M_PI;

		if (flPitch < 0.f)
			flPitch += 360.f;

		flYaw = std::atan2f(vecForward[YAW], vecForward[PITCH]) * 180.f / M_PI;

		if (flYaw < 0.f)
			flYaw += 360.f;
	}

	angView[PITCH] = flPitch;
	angView[YAW] = flYaw;
	angView[ROLL] = 0.f;
}

void M::AngleVectors(const QAngle& angView, Vector* pForward, Vector* pRight, Vector* pUp)
{
	float sp, sy, sr, cp, cy, cr;

	DirectX::XMScalarSinCos(&sp, &cp, M_DEG2RAD(angView[PITCH]));
	DirectX::XMScalarSinCos(&sy, &cy, M_DEG2RAD(angView[YAW]));
	DirectX::XMScalarSinCos(&sr, &cr, M_DEG2RAD(angView[ROLL]));

	if (pForward != nullptr)
	{
		pForward->x = cp * cy;
		pForward->y = cp * sy;
		pForward->z = -sp;
	}

	if (pRight != nullptr)
	{
		pRight->x = -1 * sr * sp * cy + -1 * cr * -sy;
		pRight->y = -1 * sr * sp * sy + -1 * cr * cy;
		pRight->z = -1 * sr * cp;
	}

	if (pUp != nullptr)
	{
		pUp->x = cr * sp * cy + -sr * -sy;
		pUp->y = cr * sp * sy + -sr * cy;
		pUp->z = cr * cp;
	}
}

void M::MatrixGetColumn(const matrix3x4_t& matrix, int nColumn, Vector& vecOut)
{
	vecOut.x = matrix[0][nColumn];
	vecOut.y = matrix[1][nColumn];
	vecOut.z = matrix[2][nColumn];
}

void M::MatrixSetColumn(const Vector& vecColumn, int nColumn, matrix3x4_t& matrix)
{
	matrix[0][nColumn] = vecColumn.x;
	matrix[1][nColumn] = vecColumn.y;
	matrix[2][nColumn] = vecColumn.z;
}

void M::MatrixSetOrigin(matrix3x4_t& matrix, const Vector& vecOrigin, const Vector& vecNewOrigin)
{
	Vector vecMatrixOrigin(matrix[0][3], matrix[1][3], matrix[2][3]);
	M::MatrixSetColumn(vecNewOrigin + (vecMatrixOrigin - vecOrigin), 3, matrix);
}

void M::AngleMatrix(const QAngle& angView, matrix3x4_t& matrix)
{
	float sp, sy, sr, cp, cy, cr;

	DirectX::XMScalarSinCos(&sp, &cp, M_DEG2RAD(angView[PITCH]));
	DirectX::XMScalarSinCos(&sy, &cy, M_DEG2RAD(angView[YAW]));
	DirectX::XMScalarSinCos(&sr, &cr, M_DEG2RAD(angView[ROLL]));

	matrix[0][0] = cp * cy;
	matrix[1][0] = cp * sy;
	matrix[2][0] = -sp;

	float crcy = cr * cy;
	float crsy = cr * sy;
	float srcy = sr * cy;
	float srsy = sr * sy;

	matrix[0][1] = sp * srcy - crsy;
	matrix[1][1] = sp * srsy + crcy;
	matrix[2][1] = sr * cp;

	matrix[0][2] = (sp * crcy + srsy);
	matrix[1][2] = (sp * crsy - srcy);
	matrix[2][2] = cr * cp;

	matrix[0][3] = 0.f;
	matrix[1][3] = 0.f;
	matrix[2][3] = 0.f;
}

Vector2D M::AnglePixels(float flSensitivity, float flPitch, float flYaw, const QAngle& angBegin, const QAngle& angEnd)
{
	QAngle angDelta = angBegin - angEnd;
	angDelta.Normalize();

	float flPixelMovePitch = (-angDelta.x) / (flYaw * flSensitivity);
	float flPixelMoveYaw = (angDelta.y) / (flPitch * flSensitivity);

	return Vector2D(flPixelMoveYaw, flPixelMovePitch);
}

QAngle M::PixelsAngle(float flSensitivity, float flPitch, float flYaw, const Vector2D& vecPixels)
{
	float flAngleMovePitch = (-vecPixels.x) * (flYaw * flSensitivity);
	float flAngleMoveYaw = (vecPixels.y) * (flPitch * flSensitivity);

	return QAngle(flAngleMoveYaw, flAngleMovePitch, 0.f);
}

QAngle M::CalcAngle(Vector vecStart, Vector vecEnd)
{
	/*QAngle angView;
	VectorAngles(vecEnd - vecStart, angView);
	return angView;*/

	QAngle delta = QAngle(
		(vecStart.x - vecEnd.x), 
		(vecStart.y - vecEnd.y), 
		(vecStart.z - vecEnd.z)), 
		angle;

	double hyp = sqrt(delta.x * delta.x + delta.y * delta.y);

	angle.x = float(atanf(float(delta.z / hyp)) * RADPI);
	angle.y = float(atanf(float(delta.y / delta.x)) * RADPI);
	angle.z = 0.0f;

	if (delta.x >= 0.0) {
		angle.y += 180.0f;
	}

	return angle;
}

Vector M::VectorTransform(const Vector& vecTransform, const matrix3x4_t& matrix)
{
	return Vector(vecTransform.DotProduct(matrix[0]) + matrix[0][3],
		vecTransform.DotProduct(matrix[1]) + matrix[1][3],
		vecTransform.DotProduct(matrix[2]) + matrix[2][3]);
}

Vector M::ExtrapolateTick(Vector p0, Vector v0)
{
	// position formula: p0 + v0t
	return p0 + (v0 * I::Globals->flIntervalPerTick);
}

float M::FloatNormalize(float flValue)
{
	if (!std::isfinite(flValue))
		flValue = 0.f;

	return std::remainderf(flValue, 360.0f);
}

float M::AngleNormalize(float flValue)
{
	float rot;
	float flNewValue = flValue;

	// bad number.
	if (!std::isfinite(flNewValue)) {
		flNewValue = 0.f;
		return flValue;
	}

	// no need to normalize this angle.
	if (flNewValue >= -180.f && flNewValue <= 180.f)
		return flValue;

	// get amount of rotations needed.
	rot = std::round(std::abs(flNewValue / 360.f));

	// normalize.
	flNewValue = (flNewValue < 0.f) ? flNewValue + (360.f * rot) : flNewValue - (360.f * rot);
	return flNewValue;
}

float M::GetFOV(const QAngle& vViewAngle, const QAngle& vAimAngle)
{
	auto AngleVector = [](QAngle ang) -> Vector
	{
		return Vector(
			cos(DEG2RAD(ang.x)) * cos(DEG2RAD(ang.y)),
			cos(DEG2RAD(ang.x)) * sin(DEG2RAD(ang.y)),
			-sin(DEG2RAD(ang.x)));
	};

	Vector vAim = AngleVector(vViewAngle),
		   vAng = AngleVector(vAimAngle);

	return RAD2DEG(acos(vAim.DotProduct(vAng) / vAim.LengthSqr()));
}