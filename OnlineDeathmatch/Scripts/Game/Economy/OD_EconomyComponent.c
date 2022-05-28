[ComponentEditorProps(category: "OnlineDeatmatch", description: "Responsible for managing the economy for the attached Entity")]
class OD_EconomyComponentClass : ScriptComponentClass {};

class OD_EconomyComponent : ScriptComponent
{
	[RplProp(onRplName: "OnMoneyChanged")]
	int Money = 0;

	private ref map<string, ref OD_Item> Purchasables = new map<string, ref OD_Item>();

	override void OnPostInit(IEntity owner)
	{
		SetEventMask(owner, EntityEvent.INIT | EntityEvent.FRAME);
		owner.SetFlags(EntityFlags.ACTIVE, false);
	}

	override void EOnInit(IEntity owner)
	{
		// For some reason Replication isn't fully setup on init. We need the replication to be fully functioning for the economy setup.
		GetGame().GetCallqueue().CallLater(SetupEconomy, 3000, false);
	}

	override void EOnFrame(IEntity owner, float timeSlice)
	{
		GetGame().GetInputManager().ActivateContext("OD_Economy");
	}

	void InitializeInputs()
	{
		OD_EconomyUIComponent economyUI = OD_EconomyUIComponent.Cast(GetOwner().FindComponent(OD_EconomyUIComponent));
		if (!economyUI)
			return;

		GetGame().GetInputManager().AddActionListener("OD_EconomyMenu", EActionTrigger.DOWN, economyUI.CreateShopUI);
	}

	private void InitializeShop()
	{
		OD_ItemHeal heal = new OD_ItemHeal();
		Purchasables.Set(heal.Identifier, heal);
	}

	OD_Item GetItem(string itemIdentifier)
	{
		return Purchasables.Get(itemIdentifier);
	}

	private void OnMoneyChanged()
	{
		OD_EconomyUIComponent economyUI = OD_EconomyUIComponent.Cast(GetOwner().FindComponent(OD_EconomyUIComponent));
		if (!economyUI)
			return;
		
		economyUI.UpdateMoneyText(Money);
	}

	void ChangeMoney(int changeAmount)
	{
		RplComponent replication = RplComponent.Cast(GetOwner().FindComponent(RplComponent));
		if (!replication || replication.IsProxy())
			return;

		Money += changeAmount;
		Replication.BumpMe();

		if (!replication.IsProxy())
			OnMoneyChanged();
	}

	void TryPurchase(string itemIdentifier)
	{
		RplComponent replication = RplComponent.Cast(GetOwner().FindComponent(RplComponent));
		if (!replication)
			return;

		Rpc(AuthorityTryPurchase, itemIdentifier);
	}
	
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	private void AuthorityTryPurchase(string itemIdentifier)
	{
		OD_Item item = Purchasables.Get(itemIdentifier);
		if (!item || item.CanBePurchased(GetOwner()) == false)
			return;

		item.OnTryPurchase(GetOwner());
	}

	// Called from GameMode after ownership setup is completed.
	void SetupEconomy()
	{
		int playerId = GetGame().GetPlayerManager().GetPlayerIdFromControlledEntity(GetOwner());
		if (playerId == 0)
			return;
		PlayerController pc = GetGame().GetPlayerManager().GetPlayerController(playerId);

		RplComponent controllerReplication = RplComponent.Cast(pc.FindComponent(RplComponent));
		if (!controllerReplication && !controllerReplication.IsOwner())
			return;

		InitializeShop();

		OD_EconomyUIComponent economyUI = OD_EconomyUIComponent.Cast(GetOwner().FindComponent(OD_EconomyUIComponent));
		if (!economyUI)
			return;

		economyUI.CreateEconomyUI();
		InitializeInputs();

		SCR_HintManagerComponent.ShowCustomHint("Welcome to Online Deathmatch! In this mod you fight against AI and the players simultaneously. You have access to an ingame shop where you can spend your hard earned money from killing adversaries. Press I to acces to the shop. For any feedback you can contact me via Discord EmreBugday#3516", "Online Deatmatch", 5000);
	}
};