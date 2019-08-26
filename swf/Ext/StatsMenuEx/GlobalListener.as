import gfx.controls.Button;
import gfx.controls.ScrollingList;
import gfx.events.EventTypes;
import gfx.io.GameDelegate;


class GlobalListener extends MovieClip
{
	/* PRIVATE VARIABLES */

	private var _trees: ScrollingList;
	private var _perks: ScrollingList;
	private var _ranks: ScrollingList;


	/* INITIALIZATION */

	public function GlobalListener()
	{
		super();
	}


	public function onLoad(): Void
	{
		var classes: ScrollingList = _root.classes;
		if (classes != undefined) {
			classes.addEventListener(EventTypes.ITEM_PRESS, this, "handleClassesPress");
		} else {
			log("could not get classes");
		}

		_trees = _root.trees;
		if (_trees != undefined) {
			_trees.addEventListener(EventTypes.ITEM_PRESS, this, "handleTreesPress");
		} else {
			log("could not get trees");
		}

		_perks = _root.perks;
		if (_perks != undefined) {
			_perks.addEventListener(EventTypes.ITEM_PRESS, this, "handlePerksPress");
		} else {
			log("could not get perks");
		}

		_ranks = _root.ranks;
		if (_ranks != undefined) {
			_ranks.addEventListener(EventTypes.ITEM_PRESS, this, "handleRanksPress");
		} else {
			log("could not get ranks");
		}

		var requisites: ScrollingList = _root.requisites;
		if (requisites != undefined) {
			requisites.addEventListener(EventTypes.ITEM_PRESS, this, "handleRequisitesPress");
		} else {
			log("could not get requisites");
		}

		var unlocks: ScrollingList = _root.unlocks;
		if (unlocks != undefined) {
			unlocks.addEventListener(EventTypes.ITEM_PRESS, this, "handleUnlocksPress");
		} else {
			log("could not get unlocks");
		}

		var unlock: Button = _root.unlock;
		if (unlock != undefined) {
			unlock.addEventListener(EventTypes.PRESS, this, "handleUnlockPress");
		} else {
			log("could not get unlock");
		}
	}


	/* PRIVATE FUNCTIONS */

	private function handleClassesPress(a_event: Object): Void
	{
		var index: Number = a_event.index;
		GameDelegate.call("OnClassPress", [index]);
	}


	private function handleTreesPress(a_event: Object): Void
	{
		var index: Number = a_event.index;
		GameDelegate.call("OnTreePress", [index]);
	}


	private function handlePerksPress(a_event: Object): Void
	{
		var perkIndex: Number = a_event.index;
		var treeIndex: Number = _trees.selectedIndex;
		GameDelegate.call("OnPerkPress", [perkIndex, treeIndex]);
	}


	private function handleRanksPress(a_event: Object): Void
	{
		var rankIndex: Number = a_event.index;
		var treeIndex: Number = _trees.selectedIndex;
		GameDelegate.call("OnRankPress", [rankIndex, treeIndex]);
	}


	private function handleRequisitesPress(a_event: Object): Void
	{
		var requisiteIndex: Number = a_event.index;
		var treeIndex: Number = _trees.selectedIndex;
		GameDelegate.call("OnRequisitePress", [requisiteIndex, treeIndex]);
	}


	private function handleUnlocksPress(a_event: Object): Void
	{
		var unlockIndex: Number = a_event.index;
		var treeIndex: Number = _trees.selectedIndex;
		GameDelegate.call("OnUnlockPress", [unlockIndex, treeIndex]);
	}


	private function handleUnlockPress(a_event: Object): Void
	{
		var rankIndex: Number = _ranks.selectedIndex;
		var perkIndex: Number = _perks.selectedIndex;
		var treeIndex: Number = _trees.selectedIndex;
		GameDelegate.call("UnlockPerk", [rankIndex, perkIndex, treeIndex]);
	}


	private function log(a_message: String): Void
	{
		GameDelegate.call("Log", [a_message]);
	}
}
