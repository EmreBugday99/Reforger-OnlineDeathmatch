[BaseContainerProps(), SCR_BaseContainerCustomTitleUIInfo("m_Info")]
class OD_CreateEnemySpawnLocationContextAction : SCR_GeneralContextAction
{
	[Attribute("{F2A99BF6981EAF93}Prefabs/SpawnPoints/Enemy_SpawnPoint.et")]
	private ResourceName EnemySpawnPointPrefab;
	
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
		Resource resource = Resource.Load(EnemySpawnPointPrefab);
		if (!resource)
			return;

		EntitySpawnParams spawnParams = new EntitySpawnParams;
		spawnParams.TransformMode = ETransformMode.WORLD;
		spawnParams.Transform[3] = cursorWorldPosition;

		GetGame().SpawnEntityPrefab(resource, GetGame().GetWorld(), spawnParams);
	}
};