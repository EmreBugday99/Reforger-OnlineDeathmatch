class OD_Item
{
	string Identifier;
	int Value;

	void OD_Item() {}

	bool CanBePurchased(IEntity buyer) 
	{
		OD_EconomyComponent economy = OD_EconomyComponent.Cast(buyer.FindComponent(OD_EconomyComponent));
		if (!economy || economy.Money < Value)
			return false;

		return true;
	}

	void OnUsed(IEntity user, IEntity usedAgainst) {}
	void OnTryPurchase(IEntity buyer) {}
};