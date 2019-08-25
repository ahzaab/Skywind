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
			classes.addEventListener(EventTypes.ITEM_PRESS, this, "handleClassPress");
		} else {
			log("could not get classes");
		}

		_trees = _root.trees;
		if (_trees != undefined) {
			_trees.addEventListener(EventTypes.ITEM_PRESS, this, "handleTreePress");
		} else {
			log("could not get trees");
		}

		_perks = _root.perks;
		if (_perks != undefined) {
			_perks.addEventListener(EventTypes.ITEM_PRESS, this, "handlePerkPress");
		} else {
			log("could not get perks");
		}

		_ranks = _root.ranks;
		if (_ranks != undefined) {
			_ranks.addEventListener(EventTypes.ITEM_PRESS, this, "handleRankPress");
		} else {
			log("could not get ranks");
		}

		var unlock: Button = _root.unlock;
		if (unlock != undefined) {
			unlock.addEventListener(EventTypes.PRESS, this, "handleUnlockPress");
		} else {
			log("could not get unlock");
		}
	}


	public function handleClassPress(a_event: Object): Void
	{
		var index: Number = a_event.index;
		GameDelegate.call("OnClassPress", [index]);
	}


	public function handleTreePress(a_event: Object): Void
	{
		var index: Number = a_event.index;
		GameDelegate.call("OnTreePress", [index]);
	}


	public function handlePerkPress(a_event: Object): Void
	{
		var index: Number = a_event.index;
		GameDelegate.call("OnPerkPress", [index]);
	}


	/* PRIVATE FUNCTIONS */

	public function handleRankPress(a_event: Object): Void
	{
		var rankIndex: Number = a_event.index;
		var treeIndex: Number = _trees.selectedIndex;
		GameDelegate.call("OnRankPress", [rankIndex, treeIndex]);
	}


	public function handleUnlockPress(a_event: Object): Void
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
