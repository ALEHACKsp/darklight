#include "autowall.h"
#include "../utilities/math.h"
#include "../core/variables.h"
#include "../core/interfaces.h"

CAutoWall g_AutoWall;

float CAutoWall::GetDamage(CBaseEntity* pLocal, const Vector& vecPoint, FireBulletData_t& dataOut)
{
	QAngle angView;
	Vector vecDirection;
	Vector vecDelta = vecPoint - pLocal->GetEyePosition();
	float flDamage = 0;

	FireBulletData_t data;
	data.vecPosition = pLocal->GetEyePosition();
	data.filter.pSkip = pLocal;

	M::VectorAngles(vecDelta, angView);
	M::AngleVectors(angView, &vecDirection);
	vecDirection.NormalizeInPlace();
	data.vecDir = vecDirection;

	CBaseCombatWeapon* pWeapon = pLocal->GetWeapon();
	if (pWeapon == nullptr)
		return -1.0f;

	if (SimulateFireBullet(pLocal, pWeapon, data))
		flDamage = data.flCurrentDamage;

	dataOut = data;
	return flDamage;
}

void CAutoWall::ScaleDamage(int iHitGroup, CBaseEntity* pEntity, float flWeaponArmorRatio, float& flDamage)
{
	bool bHeavyArmor = pEntity->HasHeavyArmor();
	int iArmor = pEntity->GetArmor();

	switch (iHitGroup)
	{
		case HITGROUP_HEAD:
			flDamage *= bHeavyArmor ? 2.0f : 4.0f;
			break;
		case HITGROUP_STOMACH:
			flDamage *= 1.25f;
			break;
		case HITGROUP_LEFTLEG:
		case HITGROUP_RIGHTLEG:
			flDamage *= 0.75f;
			break;
	}

	if (iArmor > 0 && ((iHitGroup == HITGROUP_HEAD && pEntity->HasHelmet()) || (iHitGroup >= HITGROUP_GENERIC && iHitGroup <= HITGROUP_RIGHTARM)))
	{
		float flModifier = 1.0f, flArmorBonusRatio = 0.5f, flArmorRatio = flWeaponArmorRatio * 0.5f;
		if (bHeavyArmor)
		{
			flArmorBonusRatio = 0.33f;
			flArmorRatio *= 0.5f;
			flModifier = 0.33f;
		}

		float flNewDamage = flDamage * flArmorRatio;

		if (bHeavyArmor)
			flNewDamage *= 0.85f;

		if (((flDamage - flDamage * flArmorRatio) * (flModifier * flArmorBonusRatio)) > iArmor)
			flNewDamage = flDamage - iArmor / flArmorBonusRatio;

		flDamage = flNewDamage;
	}
}

void CAutoWall::ClipTraceToPlayers(const Vector& vecAbsStart, const Vector& vecAbsEnd, unsigned int fMask, ITraceFilter* pFilter, Trace_t* pTrace)
{
	Trace_t trace = { };
	float flSmallestFraction = pTrace->flFraction;

	Ray_t ray = { };
	ray.Init(vecAbsStart, vecAbsEnd);

	for (int i = 1; i <= I::Globals->nMaxClients; i++)
	{
		CBaseEntity* pEntity = I::ClientEntityList->Get<CBaseEntity>(i);
		if (pEntity == nullptr || !pEntity->IsAlive() || pEntity->IsDormant())
			continue;

		if (pFilter != nullptr && pFilter->ShouldHitEntity(pEntity, fMask) == false)
			continue;

		const ICollideable* pCollideable = pEntity->GetCollideable();

		if (pCollideable == nullptr)
			continue;

		const Vector vecMin = pCollideable->OBBMins();
		const Vector vecMax = pCollideable->OBBMaxs();

		const Vector vecCenter = (vecMax + vecMin) * 0.5f;
		const Vector vecPosition = vecCenter + pEntity->GetOrigin();

		Vector vecTo = vecPosition - vecAbsStart;
		Vector vecDir = vecAbsEnd - vecAbsStart;
		float flLength = vecDir.NormalizeInPlace();

		const float flRangeAlong = vecDir.DotProduct(vecTo);
		float flRange = 0.0f;

		if (flRangeAlong < 0.0f)
			flRange = -vecTo.Length();
		else if (flRangeAlong > flLength)
			flRange = -(vecPosition - vecAbsEnd).Length();
		else
		{
			Vector vecRay = vecPosition - (vecDir * flRangeAlong + vecAbsStart);
			flRange = vecRay.Length();
		}

		if (flRange < 0.0f || flRange > 60.0f)
			continue;

		I::EngineTrace->ClipRayToEntity(ray, fMask | CONTENTS_HITBOX, pEntity, &trace);

		if (trace.flFraction < flSmallestFraction)
		{
			*pTrace = trace;
			flSmallestFraction = trace.flFraction;
		}
	}
}

bool CAutoWall::IsBreakableEntity(CBaseEntity* pEntity)
{
	if (pEntity == nullptr || pEntity->GetIndex() == 0)
		return false;

	const int iOldTakeDamage = pEntity->GetTakeDamage();
	const CBaseClient* pClientClass = pEntity->GetClientClass();

	if (pClientClass == nullptr)
		return false;

	if (pClientClass->nClassID == EClassIndex::CBreakableSurface || 
		pClientClass->nClassID == EClassIndex::CBaseDoor || 
		pClientClass->nClassID == EClassIndex::CFuncBrush)
		pEntity->GetTakeDamage() = DAMAGE_YES;

	using IsBreakableEntityFn = bool(__thiscall*)(CBaseEntity*);
	static IsBreakableEntityFn oIsBreakableEntity = nullptr;

	if (oIsBreakableEntity == nullptr)
		oIsBreakableEntity = (IsBreakableEntityFn)(MEM::FindPattern(CLIENT_DLL, _("55 8B EC 51 56 8B F1 85 F6 74 68")));

	pEntity->GetTakeDamage() = iOldTakeDamage;
	return oIsBreakableEntity(pEntity);
}

bool CAutoWall::TraceToExit(Trace_t* pEnterTrace, Trace_t* pExitTrace, Vector vecPosition, Vector vecDirection)
{
	float flDistance = 0.0f;
	int iStartContents = 0;

	while (flDistance <= 90.0f)
	{
		flDistance += 4.0f;

		Vector vecStart = vecPosition + vecDirection * flDistance;

		if (!iStartContents)
			iStartContents = I::EngineTrace->GetPointContents(vecStart, MASK_SHOT_HULL | CONTENTS_HITBOX, nullptr);

		const int iCurrentContents = I::EngineTrace->GetPointContents(vecStart, MASK_SHOT_HULL | CONTENTS_HITBOX, nullptr);

		if (!(iCurrentContents & MASK_SHOT_HULL) || iCurrentContents & CONTENTS_HITBOX && iCurrentContents != iStartContents)
		{
			const Vector vecEnd = vecStart - (vecDirection * 4.0f);
			U::TraceLine(vecStart, vecEnd, MASK_SHOT_HULL | CONTENTS_HITBOX, nullptr, pExitTrace);

			if (pExitTrace->bStartSolid && pExitTrace->surface.uFlags & SURF_HITBOX)
			{
				U::TraceLine(vecStart, vecPosition, MASK_SHOT_HULL, pExitTrace->pHitEntity, pExitTrace);

				if (pExitTrace->DidHit() && !pExitTrace->bStartSolid)
				{
					vecStart = pExitTrace->vecEnd;
					return true;
				}

				continue;
			}

			if (pExitTrace->DidHit() && !pExitTrace->bStartSolid)
			{
				if (IsBreakableEntity(pEnterTrace->pHitEntity) && IsBreakableEntity(pExitTrace->pHitEntity))
					return true;

				if ((!(pExitTrace->surface.uFlags & SURF_NODRAW) || (pEnterTrace->surface.uFlags & SURF_NODRAW && pExitTrace->surface.uFlags & SURF_NODRAW)) && pExitTrace->plane.vecNormal.DotProduct(vecDirection) <= 1.0f)
				{
					const float flMultiplier = pExitTrace->flFraction * 4.0f;
					vecStart -= vecDirection * flMultiplier;
					return true;
				}

				continue;
			}

			if (!pExitTrace->DidHit() || pExitTrace->bStartSolid)
			{
				if (pEnterTrace->pHitEntity != nullptr && pEnterTrace->pHitEntity->GetIndex() != 0 && IsBreakableEntity(pEnterTrace->pHitEntity))
				{
					pExitTrace = pEnterTrace;
					pExitTrace->vecEnd = vecStart + vecDirection;
					return true;
				}

				continue;
			}
		}
	}

	return false;
}

bool CAutoWall::HandleBulletPenetration(CBaseEntity* pLocal, surfacedata_t* pEnterSurfaceData, CCSWeaponData* pWeaponData, FireBulletData_t& data)
{
	static CConVar* ff_damage_reduction_bullets = I::ConVar->FindVar(_("ff_damage_reduction_bullets"));
	static CConVar* ff_damage_bullet_penetration = I::ConVar->FindVar(_("ff_damage_bullet_penetration"));

	const float flReductionDamage = ff_damage_reduction_bullets->GetFloat();
	const float flPenetrateDamage = ff_damage_bullet_penetration->GetFloat();

	const MaterialHandle_t hEnterMaterial = pEnterSurfaceData->game.hMaterial;
	const float flEnterPenetrationModifier = pEnterSurfaceData->game.flPenetrationModifier;

	Trace_t exitTrace = { };
	if ((!data.iPenetrateCount &&
		!(data.enterTrace.surface.uFlags >> 7 & SURF_LIGHT) && !((data.enterTrace.iContents >> 3) & CONTENTS_SOLID) &&
		hEnterMaterial != CHAR_TEX_GRATE && hEnterMaterial != CHAR_TEX_GLASS) ||
		pWeaponData->flPenetration <= 0.0f ||
		(!TraceToExit(&data.enterTrace, &exitTrace, data.enterTrace.vecEnd, data.vecDir) && !(I::EngineTrace->GetPointContents(data.enterTrace.vecEnd, MASK_SHOT_HULL, nullptr) & MASK_SHOT_HULL)))
		return false;

	const surfacedata_t* pExitSurfaceData = I::PhysicsProps->GetSurfaceData(exitTrace.surface.nSurfaceProps);
	const MaterialHandle_t hExitMaterial = pExitSurfaceData->game.hMaterial;
	const float flExitPenetrationModifier = pExitSurfaceData->game.flPenetrationModifier;

	float flDamageLostModifier = 0.16f;
	float flPenetrationModifier = 0.0f;

	if (hEnterMaterial == CHAR_TEX_GRATE || hEnterMaterial == CHAR_TEX_GLASS)
	{
		flDamageLostModifier = 0.05f;
		flPenetrationModifier = 3.0f;
	}
	else if ((data.enterTrace.iContents >> 3) & CONTENTS_SOLID || (data.enterTrace.surface.uFlags >> 7) & SURF_LIGHT)
	{
		flDamageLostModifier = 0.16f;
		flPenetrationModifier = 1.0f;
	}
	else if (hEnterMaterial == CHAR_TEX_FLESH && (pLocal->GetTeam() == data.enterTrace.pHitEntity->GetTeam() && flReductionDamage == 0.0f))
	{
		if (flPenetrateDamage == 0.0f)
			return false;

		flDamageLostModifier = 0.16f;
		flPenetrationModifier = flPenetrateDamage;
	}
	else
	{
		flDamageLostModifier = 0.16f;
		flPenetrationModifier = (flEnterPenetrationModifier + flExitPenetrationModifier) * 0.5f;
	}

	if (hEnterMaterial == hExitMaterial)
	{
		if (hExitMaterial == CHAR_TEX_CARDBOARD || hExitMaterial == CHAR_TEX_WOOD)
			flPenetrationModifier = 3.0f;
		else if (hExitMaterial == CHAR_TEX_PLASTIC)
			flPenetrationModifier = 2.0f;
	}

	const float flTraceDistance = (exitTrace.vecEnd - data.enterTrace.vecEnd).LengthSqr();

	const float flModifier = std::max<float>(0.0f, 1.0f / flPenetrationModifier);

	const float flLostDamage = (data.flCurrentDamage * flDamageLostModifier + std::max<float>(0.0f, 3.75f / pWeaponData->flPenetration) * (flModifier * 3.0f)) + ((flModifier * flTraceDistance) / 24.0f);

	if (flLostDamage > data.flCurrentDamage)
		return false;

	if (flLostDamage > 0.0f)
		data.flCurrentDamage -= flLostDamage;

	if (data.flCurrentDamage < 1.0f)
		return false;

	data.vecPosition = exitTrace.vecEnd;
	--data.iPenetrateCount;
	return true;
}

bool CAutoWall::SimulateFireBullet(CBaseEntity* pLocal, CBaseCombatWeapon* pWeapon, FireBulletData_t& data)
{
	CCSWeaponData* pWeaponData = I::WeaponSystem->GetWeaponData(*pWeapon->GetItemDefinitionIndex());

	if (pWeaponData == nullptr)
		return false;

	float flMaxRange = pWeaponData->flRange;
	float flTraceLenght = 0.0f;

	data.enterTrace = { };
	data.iPenetrateCount = 4;
	data.flCurrentDamage = (float)pWeaponData->iDamage;

	Trace_t trace = { };
	CTraceFilterSkipEntity filter(pLocal);

	while (data.iPenetrateCount > 0 && data.flCurrentDamage >= 1.0f)
	{
		flMaxRange -= flTraceLenght;

		const Vector vecEnd = data.vecPosition + data.vecDir * flMaxRange;
		U::TraceLine(data.vecPosition, vecEnd, MASK_SHOT_HULL | CONTENTS_HITBOX, pLocal, &data.enterTrace);

		ClipTraceToPlayers(data.vecPosition, vecEnd + data.vecDir * 40.0f, MASK_SHOT_HULL | CONTENTS_HITBOX, &data.filter, &data.enterTrace);

		surfacedata_t* pEnterSurfaceData = I::PhysicsProps->GetSurfaceData(data.enterTrace.surface.nSurfaceProps);
		float flEnterPenetrationModifier = pEnterSurfaceData->game.flPenetrationModifier;

		if (data.enterTrace.flFraction == 1.0f)
			break;

		flTraceLenght += data.enterTrace.flFraction * flMaxRange;
		data.flCurrentDamage *= std::powf(pWeaponData->flRangeModifier, flTraceLenght / MAX_DAMAGE);

		if (flTraceLenght > 3000.0f || flEnterPenetrationModifier < 0.1f)
			break;

		if (data.enterTrace.iHitGroup > HITGROUP_GENERIC && data.enterTrace.iHitGroup <= HITGROUP_RIGHTLEG && pLocal->IsEnemy(data.enterTrace.pHitEntity))
		{
			ScaleDamage(data.enterTrace.iHitGroup, data.enterTrace.pHitEntity, pWeaponData->flArmorRatio, data.flCurrentDamage);
			return true;
		}

		if (!HandleBulletPenetration(pLocal, pEnterSurfaceData, pWeaponData, data))
			break;
	}

	return false;
}