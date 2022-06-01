[BaseContainerProps(), SCR_BaseContainerCustomTitleUIInfo("m_Info")]
class OD_GiveMoneyContextAction : SCR_GeneralContextAction
{		
	override bool CanBeShown(SCR_EditableEntityComponent hoveredEntity, notnull set<SCR_EditableEntityComponent> selectedEntities, vector cursorWorldPosition, int flags)
	{
		return CanBePerformed(hoveredEntity, selectedEntities, cursorWorldPosition, flags);
	}
	
	override bool CanBePerformed(SCR_EditableEntityComponent hoveredEntity, notnull set<SCR_EditableEntityComponent> selectedEntities, vector cursorWorldPosition, int flags)
	{
		if (!hoveredEntity)
			return false;

		OD_EconomyComponent economy = OD_EconomyComponent.Cast(hoveredEntity.GetOwner().FindComponent(OD_EconomyComponent));
		return economy != null;
	}

	override void Perform(SCR_EditableEntityComponent hoveredEntity, notnull set<SCR_EditableEntityComponent> selectedEntities, vector cursorWorldPosition, int flags, int param = -1)
	{
		OD_EconomyComponent economy = OD_EconomyComponent.Cast(hoveredEntity.GetOwner().FindComponent(OD_EconomyComponent));
		if (!economy)
		{
			Print("[OnlineDeathmatch] Can't perform GiveMoney action on an entity that doesn't have an OD_EconomyComponent!", LogLevel.ERROR);
			return;
		}

		OD_GameMasterEntity gmManager = OD_GameMasterEntity.GetInstance();
		if (!gmManager)
		{
			Print("[OnlineDeathmatch] Can't change money! OD_GameMasterEntity is not present in the scene.", LogLevel.ERROR);
			return;
		}

		economy.GMChangeMoney(gmManager.GiveMoneyAmount);
	}
};