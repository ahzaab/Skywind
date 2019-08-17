class QuestCenteredList extends Shared.CenteredScrollingList
{

	function QuestCenteredList()
	{
		super();
	}

	function SetEntryText(aEntryClip: MovieClip, aEntryObject: Object): Void
	{
		super.SetEntryText(aEntryClip, aEntryObject);
		if (aEntryClip.textField != undefined) {
			aEntryClip.textField.textColor = aEntryObject.completed == true ? 0x998D7D : 0xF9EDD5;
		}
		if (aEntryClip.EquipIcon != undefined) {
			if (aEntryObject != undefined && aEntryObject.active) {
				aEntryClip.EquipIcon.gotoAndStop("Equipped");
				return;
			}
			aEntryClip.EquipIcon.gotoAndStop("None");
		}
	}

}
