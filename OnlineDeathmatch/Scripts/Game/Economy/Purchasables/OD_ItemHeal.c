class OD_ItemHeal : OD_Item
{
	void OD_ItemHeal()
	{
		Identifier = "heal";
		Value = 50;
	}

	override void OnTryPurchase(IEntity buyer)
	{
		SCR_ChimeraCharacter character = SCR_ChimeraCharacter.Cast(buyer);
		if (!character || !CanBePurchased(buyer))
			return;

		// No need to check for null. Execution order guarantees null safety.
		OD_EconomyComponent economy = OD_EconomyComponent.Cast(buyer.FindComponent(OD_EconomyComponent));
		economy.ChangeMoney(-Value);
		character.GetDamageManager().FullHeal();
	}
}