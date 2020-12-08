#pragma once
#include "config.h"

struct Variables_t
{
	C_ADDVARIABLE(bool, bAimbot, false);
	C_ADDVARIABLE(int, iAimbotKey, VK_LBUTTON);
	C_ADDVARIABLE(int, iAimbotKeyMethod, 1);
	C_ADDVARIABLE(int, iHitbox, 0);
	C_ADDVARIABLE(int, iRCS, 50);
	C_ADDVARIABLE(float, flFOV, 5.0f);
	C_ADDVARIABLE(float, flSmoothing, 10.0f);
	C_ADDVARIABLE(bool, bAimBacktrack, false);

	C_ADDVARIABLE(bool, bEnable, false);
	C_ADDVARIABLE(bool, bSilentEnable, false);
	C_ADDVARIABLE(float, fRegularFOV, 0.0f);
	C_ADDVARIABLE(float, fSilentFOV, 0.0f);
	C_ADDVARIABLE(int, iHitboxSelection, 0);
	C_ADDVARIABLE(float, fRCS, 0.0f);
	C_ADDVARIABLE(float, fSmoothing, 1.0f);

	C_ADDVARIABLE(bool, bBacktrack, false);
	C_ADDVARIABLE(int, iBacktrackMax, 12);

	C_ADDVARIABLE(bool, bTriggerbot, false);
	C_ADDVARIABLE(int, iTriggerbotKey, VK_XBUTTON1);
	C_ADDVARIABLE(int, iTriggerbotKeyMethod, 1);
	C_ADDVARIABLE(bool, bTriggerbotOnKey, true);
	C_ADDVARIABLE(bool, bTriggerbotAutowall, false);
	C_ADDVARIABLE(float, flTriggerbotAutowallMinDmg, 100.0f);
	C_ADDVARIABLE(int, iTriggerbotDelay, 0);

	C_ADDVARIABLE(bool, bTriggerbotHitChance, false);
	C_ADDVARIABLE(float, flTriggerbotHitchance, 100.0f);

	C_ADDVARIABLE(bool, bChams, false);
	C_ADDVARIABLE(bool, bChamsXQZ, false);
	C_ADDVARIABLE(bool, bChamsShine, false);
	C_ADDVARIABLE(Color, colChamsShine, Color(255, 255, 255));
	C_ADDVARIABLE(bool, bExtendChams, false);
	C_ADDVARIABLE(bool, bFlat, 0);
	C_ADDVARIABLE(float, flPearlescent, 0);
	C_ADDVARIABLE(bool, bGlow, false);
	C_ADDVARIABLE(bool, bBox, false);

	C_ADDVARIABLE(bool, bName, false);
	C_ADDVARIABLE(bool, bWeapon, false);
	C_ADDVARIABLE(bool, bAmmo, false);
	C_ADDVARIABLE(bool, bHealth, false);
	C_ADDVARIABLE(float, flDormancy, 5.0f);


	C_ADDVARIABLE(bool, bFlashlight, false);
	C_ADDVARIABLE(int, iFlashlightKey, 0x46);
	C_ADDVARIABLE(int, iFlashlightKeyMethod, 2);

	C_ADDVARIABLE(bool, bNightmode, false);
	C_ADDVARIABLE(float, flWorldBrightness, 100.0f);

	C_ADDVARIABLE(bool, bGrenadePrediction, 0);
	C_ADDVARIABLE(Color, colGrenadePrediction, Color(125, 195, 255));

	C_ADDVARIABLE(bool, bSpectatorList, false);

	C_ADDVARIABLE(bool, bHitmarker, false);

	C_ADDVARIABLE_VECTOR(bool, 4, vecEventLogs, false);

	C_ADDVARIABLE(bool, bSniperCrosshair, false);
	C_ADDVARIABLE(bool, bRecoilCrosshair, false);

	C_ADDVARIABLE(bool, bDroppedWeaponName, 0);
	C_ADDVARIABLE(Color, colDroppedWeaponName, Color(255, 255, 255));
	C_ADDVARIABLE(bool, bDroppedWeaponAmmo, 0);
	C_ADDVARIABLE(Color, colDroppedWeaponAmmo, Color(255, 255, 255));

	C_ADDVARIABLE(bool, bGrenades, 0);
	C_ADDVARIABLE(Color, colGrenades, Color(255, 255, 255));

	C_ADDVARIABLE(bool, bGrenadeTimer, 0);
	C_ADDVARIABLE(Color, colGrenadeTimer, Color(255, 255, 255));

	C_ADDVARIABLE(bool, bBombTimer, false);

	C_ADDVARIABLE(Color, colBox, Color(255, 255, 255));
	C_ADDVARIABLE(Color, colName, Color(255, 255, 255));
	C_ADDVARIABLE(Color, colWeapon, Color(255, 255, 255));
	C_ADDVARIABLE(Color, colAmmo, Color(125, 125, 255));

	C_ADDVARIABLE(Color, colGlow, Color(230, 20, 60, 128));
	C_ADDVARIABLE(Color, colChams, Color(0, 200, 0, 255));
	C_ADDVARIABLE(Color, colChamsXQZ, Color(0, 125, 255, 255));

	C_ADDVARIABLE(bool, bPhong, false);
	C_ADDVARIABLE(Color, colPhong, Color(255, 255, 255, 125));

	C_ADDVARIABLE(bool, bReflectivity, false);
	C_ADDVARIABLE(Color, colReflectivity, Color(255, 255, 255, 125));

	C_ADDVARIABLE(int, iBunnyHop, 0);
	C_ADDVARIABLE(float, flBunnyhopHitchance, 100.0f);
	C_ADDVARIABLE(bool, bClantagChanger, false);
	C_ADDVARIABLE(bool, bAnimatedClantag, false);

	C_ADDVARIABLE(bool, bNullStrafe, false);

	C_ADDVARIABLE(bool, bFastDuck, false);

	C_ADDVARIABLE(int, iEdgeJumpKey, 0);
	C_ADDVARIABLE(int, iEdgeJumpKeyMethod, 1);
	C_ADDVARIABLE(bool, bEdgeJump, false);

	C_ADDVARIABLE(int, iJumpBugKey, 0);
	C_ADDVARIABLE(int, iJumpBugKeyMethod, 1);
	C_ADDVARIABLE(bool, bJumpBug, 0);

	C_ADDVARIABLE(int, iEdgeBugKey, 0);
	C_ADDVARIABLE(int, iEdgeBugKeyMethod, 1);
	C_ADDVARIABLE(bool, bEdgeBug, 0);

	C_ADDVARIABLE(int, iBlockbotKey, 0);
	C_ADDVARIABLE(int, iBlockbotKeyMethod, 1);
	C_ADDVARIABLE(int, iBlockbot, 0);

	C_ADDVARIABLE(bool, bVelocityGraph, false);
	C_ADDVARIABLE(int, iVelocityGraphWidth, 250);
	C_ADDVARIABLE(float, flVelocityGraphCompression, 1.0f);

	C_ADDVARIABLE(bool, bVelocityIndicators, false);
	C_ADDVARIABLE(bool, bBugIndicators, false);

	C_ADDVARIABLE(int, iYAdditive, 0);

	C_ADDVARIABLE(int, iKnifeModel, 0);
	C_ADDVARIABLE(int, iGloveModel, 0);

	C_ADDVARIABLE_VECTOR(std::string, 8, vecGloveSkins, "0");
	C_ADDVARIABLE_VECTOR(float, 8, vecGloveWear, 0.0001f);
	C_ADDVARIABLE_VECTOR(int, 8, vecGloveSeed, 0);

	C_ADDVARIABLE_VECTOR(std::string, 526, vecSkinIDs, "0");
	C_ADDVARIABLE_VECTOR(float, 526, vecSkinWear, 0.0001f);
	C_ADDVARIABLE_VECTOR(int, 526, vecSkinSeed, 0);

	C_ADDVARIABLE(int, iPlayerCTModel, 0);
	C_ADDVARIABLE(int, iPlayerTModel, 0);
	
	C_ADDVARIABLE(bool, bWatermark, false);
	C_ADDVARIABLE_VECTOR(bool, 4, vecMatchmaking, false);
	C_ADDVARIABLE(bool, bAutoShoot, false);

	C_ADDVARIABLE(bool, bCustomMenuCol, false);

	C_ADDVARIABLE(int, iMenuX, 400);
	C_ADDVARIABLE(int, iMenuY, 200);

	C_ADDVARIABLE(int, iSpectatorX, 200);
	C_ADDVARIABLE(int, iSpectatorY, 200);
};

inline Variables_t Vars;
