[ComponentEditorProps(category: "OnlineDeatmatch", description: "Online Deatmatch GameMode")]
class OD_GameModeComponentClass : SCR_BaseGameModeComponentClass {};

class OD_GameModeComponent : SCR_BaseGameModeComponent
{
	[Attribute("0", uiwidget: UIWidgets.EditBox, "What faction does the players belong to", category: "OnlineDeatmatch")]
	string playerFactionName;

	[Attribute("0", uiwidget: UIWidgets.EditBox, "What faction does the players belong to", category: "OnlineDeatmatch")]
	string playerFactionKey;

	override void OnPlayerRegistered(int playerId)
	{
		RplComponent replication = RplComponent.Cast(GetOwner().FindComponent(RplComponent));
		if (!replication || replication.IsProxy())
			return;

		GetGame().GetCallqueue().CallLater(SpawnPlayer, 5000, false, playerId);
	}

	override void OnPlayerKilled(int playerId, IEntity player, IEntity killer)
	{
		Print("[OnlineDeatmatch] Player [" + playerId + "] died. Respawning...");

		RplComponent replication = RplComponent.Cast(GetOwner().FindComponent(RplComponent));
		if (!replication || replication.IsProxy())
			return;

		if (!killer)
		{
			OD_EconomyComponent economy = OD_EconomyComponent.Cast(killer.FindComponent(OD_EconomyComponent));
			if (!economy)
				economy.ChangeMoney(40);
		}

		replication.DeleteRplEntity(player, false);
		GetGame().GetCallqueue().CallLater(SpawnPlayer, 5000, false, playerId);
	}
	
	override void OnPlayerSpawned(int playerId, IEntity controlledEntity)
	{
	}

	private void SpawnPlayer(int playerId)
	{
		RplComponent replication = RplComponent.Cast(GetOwner().FindComponent(RplComponent));
		if (!replication || replication.IsProxy())
			return;

		SCR_RespawnSystemComponent respawnSystem = SCR_RespawnSystemComponent.Cast(GetOwner().FindComponent(SCR_RespawnSystemComponent));
		if (!respawnSystem)
			return;

		Faction playerFaction = GetGame().GetFactionManager().GetFactionByKey(playerFactionKey);
		if (!playerFaction)
			return;
		
		SCR_SpawnPoint spawnPoint = SCR_SpawnPoint.GetRandomSpawnPointForFaction(playerFactionKey);
		if (!spawnPoint)
			return;

		vector spawnPos = vector.Zero;
		vector spawnRot = vector.Zero;
		spawnPoint.GetPositionAndRotation(spawnPos, spawnRot);

		SCR_BasePlayerLoadout loadout = GetGame().GetLoadoutManager().GetRandomFactionLoadout(playerFaction);
		if (!loadout)
			return;

		PlayerController pc = GetGame().GetPlayerManager().GetPlayerController(playerId);
		if (!pc)
			return;

		EntitySpawnParams spawnParams = new EntitySpawnParams();
		spawnParams.TransformMode = ETransformMode.WORLD;
		spawnPoint.GetWorldTransform(spawnParams.Transform);

		respawnSystem.SetPlayerFaction(playerId, GetGame().GetFactionManager().GetFactionIndex(playerFaction));
		respawnSystem.SetPlayerLoadout(playerId, GetGame().GetLoadoutManager().GetLoadoutIndex(loadout));
		respawnSystem.SetPlayerSpawnPoint(playerId, SCR_SpawnPoint.GetSpawnPointRplId(spawnPoint));

		GenericEntity playerEntity = respawnSystem.CustomRespawn(playerId, loadout.GetLoadoutResource(), spawnPos, spawnRot);
		RplComponent playerReplication = RplComponent.Cast(playerEntity.FindComponent(RplComponent));

		pc.SetControlledEntity(playerEntity);
		replication.Give(pc.GetRplIdentity());
	}
};