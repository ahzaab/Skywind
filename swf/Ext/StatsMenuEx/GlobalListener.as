import gfx.controls.Button;
import gfx.controls.ScrollingList;
import gfx.events.EventTypes;
import gfx.io.GameDelegate;


class GlobalListener extends MovieClip
{
	/* PRIVATE VARIABLES */

	private var roots: ScrollingList;
	private var trees: ScrollingList;
	private var perks: ScrollingList;
	private var ranks: ScrollingList;


	/* INITIALIZATION */

	public function GlobalListener()
	{
		super();
	}


	public function onLoad(): Void
	{
		roots  = _parent.roots;
		if (roots != undefined) {
			roots.addEventListener(EventTypes.ITEM_PRESS, this, "handleRootsPress");
		} else {
			log("could not get classes");
		}

		trees = _parent.trees;
		if (trees != undefined) {
			trees.addEventListener(EventTypes.ITEM_PRESS, this, "handleTreesPress");
		} else {
			log("could not get trees");
		}

		perks = _parent.perks;
		if (perks != undefined) {
			perks.addEventListener(EventTypes.ITEM_PRESS, this, "handlePerksPress");
		} else {
			log("could not get perks");
		}

		ranks = _parent.ranks;
		if (ranks != undefined) {
			ranks.addEventListener(EventTypes.ITEM_PRESS, this, "handleRanksPress");
		} else {
			log("could not get ranks");
		}

		var requisites: ScrollingList = _parent.requisites;
		if (requisites != undefined) {
			requisites.addEventListener(EventTypes.ITEM_PRESS, this, "handleRequisitesPress");
		} else {
			log("could not get requisites");
		}

		var unlocks: ScrollingList = _parent.unlocks;
		if (unlocks != undefined) {
			unlocks.addEventListener(EventTypes.ITEM_PRESS, this, "handleUnlocksPress");
		} else {
			log("could not get unlocks");
		}

		var unlock: Button = _parent.unlock;
		if (unlock != undefined) {
			unlock.addEventListener(EventTypes.PRESS, this, "handleUnlockPress");
		} else {
			log("could not get unlock");
		}
	}


	/* PRIVATE FUNCTIONS */

	private function handleRootsPress(a_event: Object): Void
	{
		var index: Number = a_event.index;
		GameDelegate.call("OnRootPress", [index]);
	}


	private function handleTreesPress(a_event: Object): Void
	{
		var index: Number = a_event.index;
		GameDelegate.call("OnTreePress", [index]);
	}


	private function handlePerksPress(a_event: Object): Void
	{
		var perkIndex: Number = a_event.index;
		var treeIndex: Number = trees.selectedIndex;
		GameDelegate.call("OnPerkPress", [perkIndex, treeIndex]);
	}


	private function handleRanksPress(a_event: Object): Void
	{
		var rankIndex: Number = a_event.index;
		var treeIndex: Number = trees.selectedIndex;
		GameDelegate.call("OnRankPress", [rankIndex, treeIndex]);
	}


	private function handleRequisitesPress(a_event: Object): Void
	{
		var requisiteIndex: Number = a_event.index;
		var treeIndex: Number = trees.selectedIndex;
		GameDelegate.call("OnRequisitePress", [requisiteIndex, treeIndex]);
	}


	private function handleUnlocksPress(a_event: Object): Void
	{
		var unlockIndex: Number = a_event.index;
		var treeIndex: Number = trees.selectedIndex;
		GameDelegate.call("OnUnlockPress", [unlockIndex, treeIndex]);
	}


	private function handleUnlockPress(a_event: Object): Void
	{
		var rankIndex: Number = ranks.selectedIndex;
		var perkIndex: Number = perks.selectedIndex;
		var treeIndex: Number = trees.selectedIndex;
		GameDelegate.call("UnlockPerk", [rankIndex, perkIndex, treeIndex]);
	}


	private function log(a_message: String): Void
	{
		GameDelegate.call("Log", [a_message]);
	}
}
