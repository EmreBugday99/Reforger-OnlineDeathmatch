class OD_GameMasterEntityClass : GenericEntityClass 
{}

class OD_GameMasterEntity : GenericEntity
{
	[Attribute("50", uiwidget: UIWidgets.ResourcePickerThumbnail, "How much money will the Give Money action give to the player?", category: "OnlineDeatmatch")]
	int GiveMoneyAmount;

	private static OD_GameMasterEntity singleton;

	void OD_GameMasterEntity(IEntitySource src, IEntity parent)
	{
		if (singleton)
			Print("[OnlineDeathmatch] Multiple instances of OD_GameMasterEntity detected! There can only one instance of OD_GameMasterEntity. Be cautious!", LogLevel.ERROR);
		
		singleton = this;

		SetEventMask(EntityEvent.INIT | EntityEvent.FRAME);
		SetFlags(EntityFlags.ACTIVE, false);
	}

	void ~OD_GameMasterEntity() { singleton = null }

	override void EOnInit(IEntity owner)
	{
		OD_GMSettingsUIComponent gmSettingsUI = OD_GMSettingsUIComponent.Cast(FindComponent(OD_GMSettingsUIComponent));
		if (!gmSettingsUI)
		{
			Print("[OnlineDeathmatch] Can't initialize inputs for Game Master Settings Menu. Missing Component: OD_GMSettingsUIComponent", LogLevel.ERROR);
			return;		
		}

		GetGame().GetInputManager().AddActionListener("OD_GMSettings", EActionTrigger.DOWN, gmSettingsUI.CreateSettingsUI);
	}

	override void EOnFrame(IEntity owner, float timeSlice)
	{
		SCR_EditorManagerEntity editorManager = SCR_EditorManagerEntity.GetInstance();
		if (editorManager && !editorManager.IsLimited())
			GetGame().GetInputManager().ActivateContext("OD_GameMaster");
	}

	static OD_GameMasterEntity GetInstance() { return singleton; }
};