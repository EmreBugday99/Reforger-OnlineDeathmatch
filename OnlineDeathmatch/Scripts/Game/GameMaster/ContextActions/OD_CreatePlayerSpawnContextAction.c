[BaseContainerProps(), SCR_BaseContainerCustomTitleUIInfo("m_Info")]
class OD_CreatePlayerSpawnContextAction : SCR_GeneralContextAction
{
	[Attribute("{1E608666DA978AB1}Prefabs/SpawnPoints/Player_SpawnPoint.et")]
	private ResourceName PlayerSpawnPointPrefab;
	
	override bool CanBeShown(SCR_EditableEntityComponent hoveredEntity, notnull set<SCR_EditableEntityComponent> selectedEntities, vector cursorWorldPosition, int flags)
	{
		return CanBePerformed(hoveredEntity, selectedEntities, cursorWorldPosition, flags);
	}
	
	override bool CanBePerformed(SCR_EditableEntityComponent hoveredEntity, notnull set<SCR_EditableEntityComponent> selectedEntities, vector cursorWorldPosition, int flags)
	{
		return cursorWorldPosition != vector.Zero;
	}

	override void Perform(SCR_EditableEntityComponent hoveredEntity, notnull set<SCR_EditableEntityComponent> selectedEntities, vector cursorWorldPosition, int flags, int param = -1)
	{
		Resource resource = Resource.Load(PlayerSpawnPointPrefab);
		if (!resource)
			return;

		EntitySpawnParams spawnParams = new EntitySpawnParams;
		spawnParams.TransformMode = ETransformMode.WORLD;
		spawnParams.Transform[3] = cursorWorldPosition;

		GetGame().SpawnEntityPrefab(resource, GetGame().GetWorld(), spawnParams);
	}
};