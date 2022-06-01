// Should be attached to an OD_GMSettingsEntity

[ComponentEditorProps(category: "OnlineDeatmatch", description: "Responsible for managing the UI for GameMaster settings")]
class OD_GMSettingsUIComponentClass : ScriptComponentClass {};

class OD_GMSettingsUIComponent : ScriptComponent
{
	[Attribute("", uiwidget: UIWidgets.ResourcePickerThumbnail, "Layout for rendering Game Master settings", category: "OnlineDeatmatch")]
	private ResourceName GameMasterSettingsLayout;

	[Attribute("", uiwidget: UIWidgets.EditBox, "Give Money Amount Text Id", category: "OnlineDeatmatch")]
	private string GiveMoneyAmountTextId;
	[Attribute("", uiwidget: UIWidgets.EditBox, "Increment button id for the Give Money setting", category: "OnlineDeatmatch")]
	private string GiveMoneyIncrementButtonId;
	[Attribute("", uiwidget: UIWidgets.EditBox, "Decrement button id for the Give Money setting", category: "OnlineDeatmatch")]
	private string GiveMoneyDecrementButtonId;
	[Attribute("", uiwidget: UIWidgets.EditBox, "Close button for closing the GM Settings Menu", category: "OnlineDeatmatch")]
	private string SettingsCloseButtonId;

	private Widget GMSettingsWidget;

	void CreateSettingsUI()
	{
		if (GMSettingsWidget)
			return;

		OD_GameMasterEntity gmSettings = OD_GameMasterEntity.Cast(GetOwner());
		if (!gmSettings)
			return;

		MenuBase settingsMenu = GetGame().GetMenuManager().OpenMenu(ChimeraMenuPreset.OD_GMSettingsMenu);
		if (!settingsMenu)
			return;
		GMSettingsWidget = settingsMenu.GetRootWidget();

		UpdateGiveMoneyAmountText(GMSettingsWidget, gmSettings.GiveMoneyAmount.ToString());

		ButtonWidget giveMoneyIncreaseButton = ButtonWidget.Cast(GMSettingsWidget.FindAnyWidget(GiveMoneyIncrementButtonId));
		if (giveMoneyIncreaseButton)
			ButtonActionComponent.GetOnAction(giveMoneyIncreaseButton).Insert(OnGiveMoneyIncreaseButton);

		ButtonWidget giveMoneyDecreaseButton = ButtonWidget.Cast(GMSettingsWidget.FindAnyWidget(GiveMoneyDecrementButtonId));
		if (giveMoneyDecreaseButton)
			ButtonActionComponent.GetOnAction(giveMoneyDecreaseButton).Insert(OnGiveMoneyDecreaseButton);

		ButtonWidget settingsCloseButton = ButtonWidget.Cast(GMSettingsWidget.FindAnyWidget(SettingsCloseButtonId));
		if (settingsCloseButton)
			ButtonActionComponent.GetOnAction(settingsCloseButton).Insert(OnSettingsCloseButton);
	}

	private void OnGiveMoneyIncreaseButton(Widget widget, float value, EActionTrigger actionTrigger)
	{
		if (actionTrigger != EActionTrigger.DOWN)
			return;

		OD_GameMasterEntity gmSettings = OD_GameMasterEntity.Cast(GetOwner());
		if (!gmSettings)
			return;

		gmSettings.GiveMoneyAmount++;
		UpdateGiveMoneyAmountText(widget, gmSettings.GiveMoneyAmount.ToString());
	}

	private void OnGiveMoneyDecreaseButton(Widget widget, float value, EActionTrigger actionTrigger)
	{
		if (actionTrigger != EActionTrigger.DOWN)
			return;

		OD_GameMasterEntity gmSettings = OD_GameMasterEntity.Cast(GetOwner());
		if (!gmSettings)
			return;

		gmSettings.GiveMoneyAmount--;
		UpdateGiveMoneyAmountText(widget, gmSettings.GiveMoneyAmount.ToString());
	}

	private void OnSettingsCloseButton(Widget widget, float value, EActionTrigger actionTrigger)
	{
		GetGame().GetMenuManager().CloseMenuByPreset(ChimeraMenuPreset.OD_GMSettingsMenu);
		if (GMSettingsWidget)
			GMSettingsWidget.RemoveFromHierarchy();
		GMSettingsWidget = null;
	}

	void UpdateGiveMoneyAmountText(Widget widget, string newAmount)
	{
		RichTextWidget giveMoneyAmountText = RichTextWidget.Cast(GMSettingsWidget.FindAnyWidget(GiveMoneyAmountTextId));
		if (giveMoneyAmountText)
			giveMoneyAmountText.SetText(newAmount);
	}
};