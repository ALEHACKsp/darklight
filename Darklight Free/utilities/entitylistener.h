#pragma once
// used: std::vector
#include <vector>

// uisng: hooks setup/remove
#include "../utilities/detourhook.h"
// uisng: cliententitylistener class
#include "../sdk/interfaces/icliententitylist.h"

class CEntityListener : public IClientEntityListener // @credits: alpine971
{
public:
	struct EntityObject_t
	{
		EntityObject_t(CBaseEntity* pEntity, int nIndex) :
			pEntity(pEntity), iIndex(nIndex) { }

		CBaseEntity* pEntity = nullptr;
		int	iIndex = 0;
	};

	// Get
	void			Setup();
	void			Destroy();

	// Override
	virtual void	OnEntityCreated(CBaseEntity* pEntity) override;
	virtual void	OnEntityDeleted(CBaseEntity* pEntity) override;

	// Values
	/* vector of listening players */
	std::vector<EntityObject_t> vecEntities;
};