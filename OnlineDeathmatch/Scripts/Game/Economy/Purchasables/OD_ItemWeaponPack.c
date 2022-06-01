class OD_ItemWeaponPack : OD_Item
{
	void OD_ItemWeaponPack()
	{
		Identifier = "weapon_pack";
		Value = 50;
	}

	override void OnTryPurchase(IEntity buyer)
	{
		ResourceName weaponPackResourceName = "{382E8CF4DF062D01}Prefabs/ShopItems/OD_WeaponShopItem.et";
		Resource weaponPackResource = Resource.Load(weaponPackResourceName);
		if (!weaponPackResource)
		{
			Print("[OnlineDeathmatch] Failed to load Weapon Pack shop item as a Resource!", LogLevel.ERROR);
			return;
		}

		SCR_ChimeraCharacter character = SCR_ChimeraCharacter.Cast(buyer);
		if (!character || !CanBePurchased(buyer))
			return;

		// No need to check for null. Execution order guarantees null safety.
		OD_EconomyComponent economy = OD_EconomyComponent.Cast(buyer.FindComponent(OD_EconomyComponent));
		economy.ChangeMoney(-Value);

		EntitySpawnParams spawnParams = new EntitySpawnParams();
		spawnParams.TransformMode = ETransformMode.WORLD;
		character.GetWorldTransform(spawnParams.Transform);

		IEntity weaponPackEntity = GetGame().SpawnEntityPrefab(weaponPackResource, buyer.GetWorld(), spawnParams);
		if(!weaponPackEntity)
			return;

		// Destroy the weapon pack after 30 seconds.
		GetGame().GetCallqueue().CallLater(DestroyItemEntity, 30000, false, weaponPackEntity);
	}
	
	void DestroyItemEntity(IEntity itemEntity)
	{
		Print("[OnlineDeathmatch] Removing Weapon Pack shop item due to timeout.");
		
		// At OnTryPurchase() we were safe from any null exceptions due to execution order.
		// This time however since this method gets called by the CallQueue it's not guaranteed anymore.
		RplComponent replication = RplComponent.Cast(itemEntity.FindComponent(RplComponent));
		if (!replication)
		{
			Print("[OnlineDeathmatch] Failed to remove WeaponPack due to missing RplComponent on the entity.", LogLevel.ERROR);
			return;
		}

		replication.DeleteRplEntity(itemEntity, false);
	}
}