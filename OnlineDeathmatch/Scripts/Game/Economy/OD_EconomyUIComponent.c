[ComponentEditorProps(category: "OnlineDeatmatch", description: "Responsible for managing the economy UI")]
class OD_EconomyUIComponentClass : ScriptComponentClass {};

class OD_EconomyUIComponent : ScriptComponent
{
	[Attribute("", uiwidget: UIWidgets.ResourcePickerThumbnail, "Layout for rendering player economy information", category: "OnlineDeatmatch")]
	ResourceName EconomyLayout;
	[Attribute("", uiwidget: UIWidgets.ResourcePickerThumbnail, "Layout for rendering economy shop", category: "OnlineDeatmatch")]
	ResourceName ShopLayout;

	[Attribute("", uiwidget: UIWidgets.EditBox, "Money text id in the economy layout", category: "OnlineDeatmatch")]
	string MoneyTextId;
	[Attribute("", uiwidget: UIWidgets.EditBox, "Heal price text id in the economy layout", category: "OnlineDeatmatch")]
	string HealPriceTextId;
	[Attribute("", uiwidget: UIWidgets.EditBox, "Heal button id in the shop layout", category: "OnlineDeatmatch")]
	string HealButtonId;
	[Attribute("", uiwidget: UIWidgets.EditBox, "Close button id in the shop layout", category: "OnlineDeatmatch")]
	string ShopButtonId;

	Widget EconomyWidget;
	Widget ShopWidget;
	
	void CreateEconomyUI()
	{
		EconomyWidget = GetGame().GetWorkspace().CreateWidgets(EconomyLayout);
		OD_EconomyComponent economy = OD_EconomyComponent.Cast(GetOwner().FindComponent(OD_EconomyComponent));
		if (!EconomyWidget || !economy)
			return;

		RichTextWidget moneyText = RichTextWidget.Cast(EconomyWidget.FindAnyWidget(MoneyTextId));
		moneyText.SetText("Money: " + economy.Money + "$");
	}

	void CreateShopUI(float value, EActionTrigger trigger)
	{
		OD_EconomyComponent economy = OD_EconomyComponent.Cast(GetOwner().FindComponent(OD_EconomyComponent));
		if (!economy)
			return;
		MenuBase shopMenu = GetGame().GetMenuManager().OpenMenu(ChimeraMenuPreset.OD_ShopMenu);
		if (!shopMenu)
			return;
		ShopWidget = shopMenu.GetRootWidget();

		RichTextWidget healPrice = RichTextWidget.Cast(ShopWidget.FindAnyWidget(HealPriceTextId));
		if (healPrice)
		{
			OD_Item healItem = economy.GetItem("heal");
			if (healItem)
				healPrice.SetText(healItem.Value.ToString() + "$");
		}

		ButtonWidget healButton = ButtonWidget.Cast(ShopWidget.FindAnyWidget(HealButtonId));
		if (healButton)
			ButtonActionComponent.GetOnAction(healButton).Insert(OnHealButton);

		ButtonWidget shopButton = ButtonWidget.Cast(ShopWidget.FindAnyWidget(ShopButtonId));
		if (shopButton)
			ButtonActionComponent.GetOnAction(shopButton).Insert(OnShopCloseButton);
	}

	private void OnHealButton(Widget widget, float value, EActionTrigger actionTrigger)
	{		
		if (actionTrigger != EActionTrigger.DOWN)
			return;

		OD_EconomyComponent economy = OD_EconomyComponent.Cast(GetOwner().FindComponent(OD_EconomyComponent));
		if (!economy)
			return;

		economy.TryPurchase("heal");
	}
	
	void UpdateMoneyText(int newMoney)
	{
		if (!EconomyWidget)
		return;

		RichTextWidget moneyText = RichTextWidget.Cast(EconomyWidget.FindAnyWidget(MoneyTextId));
		if (!moneyText)
			return;
		moneyText.SetText("Money: " + newMoney + "$");
	}

	private void OnShopCloseButton(Widget widget, float value, EActionTrigger actionTrigger)
	{
		GetGame().GetMenuManager().CloseMenuByPreset(ChimeraMenuPreset.OD_ShopMenu);
		if (ShopWidget)
			ShopWidget.RemoveFromHierarchy();
		ShopWidget = null;
	}

	override void OnDelete(IEntity owner)
	{		
		if (EconomyWidget)
			EconomyWidget.RemoveFromHierarchy();

		if (ShopWidget)
		{
			GetGame().GetMenuManager().CloseMenuByPreset(ChimeraMenuPreset.OD_ShopMenu);
			ShopWidget.RemoveFromHierarchy();
		}
	}
};