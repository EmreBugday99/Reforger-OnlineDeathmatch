[ComponentEditorProps(category: "OnlineDeatmatch", description: "Enemy Spawner")]
class OD_EnemySpawnerClass : ScriptComponentClass {};

class OD_EnemySpawner : ScriptComponent
{
	[Attribute("0", uiwidget: UIWidgets.EditBox, "What faction does the enemies belong to", category: "OnlineDeatmatch")]
	private string enemyFactionKey;
	[Attribute("1", uiwidget: UIWidgets.EditBox, "Timer for the enemy respawn checker", category: "OnlineDeatmatch")]
	private int enemyRespawnMilliseconds;
	[Attribute("2", uiwidget: UIWidgets.EditBox, "Amount of enemies this spawner will spawn", category: "OnlineDeatmatch")]
	private int enemyCount;

	ref map<string, SCR_ChimeraCharacter> SpawnedEnemies = new map<string, SCR_ChimeraCharacter>;

	override void OnPostInit(IEntity owner)
	{
		SetEventMask(owner, EntityEvent.INIT);

		super.OnPostInit(owner);
	}

	override void EOnInit(IEntity owner)
	{		
		RplComponent replication = RplComponent.Cast(owner.FindComponent(RplComponent));
		if (!replication || replication.IsProxy())
			return;

		GetGame().GetCallqueue().CallLater(SpawnEnemiesOnStart, enemyRespawnMilliseconds, false);

		super.EOnInit(owner);
	}
	
	void SpawnEnemiesOnStart()
	{
		for (int i = 0; i < enemyCount; i++)
		{
			SpawnEnemy();
		}
	}
	
	void OnEnemyDead(SCR_CharacterControllerComponent deadEnemy, IEntity instigator)
	{
		SpawnedEnemies.Remove(deadEnemy.GetOwner().GetID().ToString());

		// Immediately deleting and spawning a new enemy on death would look weird gameplay wise.
		GetGame().GetCallqueue().CallLater(DeleteDeadEnemy, 3000, false, deadEnemy);
		GetGame().GetCallqueue().CallLater(SpawnEnemy, 10000, false);
	}
	
	private void DeleteDeadEnemy(SCR_CharacterControllerComponent deadEnemy)
	{
		RplComponent replication = RplComponent.Cast(GetOwner().FindComponent(RplComponent));
		if (!replication)
			return;

		replication.DeleteRplEntity(deadEnemy.GetOwner(), false);
	}

	SCR_ChimeraCharacter SpawnEnemy()
	{
		RplComponent replication = RplComponent.Cast(GetOwner().FindComponent(RplComponent));
		if (!replication || replication.IsProxy())
			return null;

		Faction enemyFaction = GetGame().GetFactionManager().GetFactionByKey(enemyFactionKey);
		if (!enemyFaction)
			return null;

		SCR_SpawnPoint spawnPoint = SCR_SpawnPoint.Cast(GetOwner());
		if (!spawnPoint)
			return null;

		EntitySpawnParams spawnParams = new EntitySpawnParams();
		spawnParams.TransformMode = ETransformMode.WORLD;
		spawnPoint.GetWorldTransform(spawnParams.Transform);

		SCR_BasePlayerLoadout loadout = GetGame().GetLoadoutManager().GetRandomFactionLoadout(enemyFaction);
		if (!loadout)
			return null;

		SCR_ChimeraCharacter enemy = SCR_ChimeraCharacter.Cast(GetGame().SpawnEntityPrefab(Resource.Load(loadout.GetLoadoutResource()), GetOwner().GetWorld(), spawnParams));
		SpawnedEnemies.Insert(enemy.GetID().ToString(), enemy);

		SCR_CharacterControllerComponent controller = SCR_CharacterControllerComponent.Cast(enemy.FindComponent(SCR_CharacterControllerComponent));
		// Prefab probably has a wrong setup.
		if (!controller)
		{
			Print("[OnlineDeatmatch] Spawned enemy prefab doesn't have SCR_CharacterControllerComponent!", LogLevel.ERROR);
			replication.DeleteRplEntity(enemy, false);
			return null;
		}

		controller.m_OnPlayerDeathWithParam.Insert(OnEnemyDead);

		return enemy;	
	}
};