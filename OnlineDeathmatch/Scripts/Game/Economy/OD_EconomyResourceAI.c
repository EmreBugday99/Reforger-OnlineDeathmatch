[ComponentEditorProps(category: "OnlineDeatmatch", description: "Responsible for handling this resource entity in the economy")]
class OD_EconomyResourceAIClass : ScriptComponentClass {};

class OD_EconomyResourceAI : ScriptComponent
{
	[Attribute("10", uiwidget: UIWidgets.EditBox, "Value of this economy resource", category: "OnlineDeatmatch")]
	int resourceValue;
	
	override void OnPostInit(IEntity owner)
	{
		SetEventMask(owner, EntityEvent.INIT);
	}
	
	override void EOnInit(IEntity owner)
	{
		RplComponent replication = RplComponent.Cast(owner.FindComponent(RplComponent));
		if (!replication || replication.IsProxy())
			return;
		
		SCR_CharacterControllerComponent controller = SCR_CharacterControllerComponent.Cast(owner.FindComponent(SCR_CharacterControllerComponent));
		if (!controller)
			return;
		controller.m_OnPlayerDeathWithParam.Insert(OnAiDead);
	}
	
	override void OnDelete(IEntity owner)
	{
		SCR_CharacterControllerComponent controller = SCR_CharacterControllerComponent.Cast(owner.FindComponent(SCR_CharacterControllerComponent));
		if (!controller)
			return;
		
		controller.m_OnPlayerDeathWithParam.Remove(OnAiDead);
	}
	
	private void OnAiDead(SCR_CharacterControllerComponent deadAi, IEntity instigator)
	{				
		OD_EconomyComponent economy = OD_EconomyComponent.Cast(instigator.FindComponent(OD_EconomyComponent));
		if (!economy)
			return;
		
		economy.ChangeMoney(resourceValue);
	}
};