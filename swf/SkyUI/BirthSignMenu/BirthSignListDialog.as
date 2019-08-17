import gfx.io.GameDelegate;

import gfx.managers.FocusHandler;

import gfx.ui.InputDetails;
import gfx.ui.NavigationCode;

import Shared.GlobalFunc;

import skyui.defines.Input;

import skyui.util.GlobalFunctions;
import skyui.util.Translator;


class BirthSignListDialog extends MovieClip
{
  	/* PRIVATE VARIABLES */

	static private var PLTFRM_PC: Number = 0;
	static private var PLTFRM_PC_CONTROLLER: Number = 1;

	static private var NONE_ID: Number = 0;
	static private var APPRENTICE_ID: Number = 1;
	static private var ATRONACH_ID: Number = 2;
	static private var LADY_ID: Number = 3;
	static private var LORD_ID: Number = 4;
	static private var LOVER_ID: Number = 5;
	static private var MAGE_ID: Number = 6;
	static private var RITUAL_ID: Number = 7;
	static private var SERPENT_ID: Number = 8;
	static private var SHADOW_ID: Number = 9;
	static private var STEED_ID: Number = 10;
	static private var THIEF_ID: Number = 11;
	static private var TOWER_ID: Number = 12;
	static private var WARRIOR_ID: Number = 13;
	
	private var _acceptControls: Object;
	private var _acceptKey: Number = -1;
	private var _cancelControls: Object;
	private var _chooseKey: Number = -1;
	private var _birthsigns: Array;
	private var _currentSign : Number = NONE_ID;
	
  	/* STAGE ELEMENTS */
	
	public var titleTextField: TextField;	// bsWindow
	public var descriptionTextField: TextField;	// bsWindow
	public var buttonAccept: ButtonPanel;	// bsWindow
	public var buttonCancel: ButtonPanel;	// bsWindow
	public var bsWindow: MovieClip;
	public var net: MovieClip;


	/* INITIALIZATION */
	
	public function BirthSignListDialog()
	{
		super();
	}
	
	
  	/* PUBLIC FUNCTIONS */
	
	public function onLoad()
	{
		super.onLoad();
		
		_visible = true;
				
		titleTextField = bsWindow.titleTextField;
		descriptionTextField = bsWindow.descriptionTextField;
		buttonAccept = bsWindow.buttonAccept;
		buttonCancel = bsWindow.buttonCancel;
		descriptionTextField._visible = false;
		buttonAccept.setVisible(false);
		buttonCancel.setVisible(false);
		_birthsigns = Array("None", "Apprentice", "Atronach", "Lady", "Lord", "Lover", "Mage", "Ritual", "Serpent", "Shadow", "Steed", "Thief", "Tower", "Warrior");

		gotoSign(_currentSign);
		
		this.initListData();
		this.initListParams("Choose a Birthsign", 0);
	}
	
	
	public function changeMenu(a_menuIdx: Number): Void
	{
		_currentSign = a_menuIdx;
		gotoSign(_currentSign);
		formatDescription();

		if (_currentSign != NONE_ID) {
			buttonAccept.setVisible(true);
		} else {
			buttonAccept.setVisible(false);
		}
	}


	public function setInputs(a_acceptText: String, a_acceptIcon: Number, a_cancelText: String, a_cancelIcon: Number): Void
	{
		buttonAccept.setButtonText(a_acceptText);
		buttonAccept.updateButtonIcon(a_acceptIcon);

		buttonCancel.setButtonText(a_cancelText);
		buttonCancel.updateButtonIcon(a_cancelIcon);
		buttonCancel.setVisible(true);
	}


	public function getCurrentSign(): Number
	{
		return _currentSign;
	}

	
  	/* PRIVATE FUNCTIONS */

	private function initListData(): Void
	{
		for (var i = 1; i < _birthsigns.length; ++i) {
			var str = _birthsigns[i];
			var pic: MovieClip = this["ico_" + str];
			pic.hideDescription();
			pic.setDescription(Translator.translate("$" + str));
			pic.setID(i);
		}
	}


	private function initListParams(a_titleText: String, a_startIdx: Number): Void
	{
		FocusHandler.instance.setFocus(this, 0);
		titleTextField.SetText(a_titleText);
		_visible = true;
	}

	
	private function formatDescription(): Void
	{
		if (_currentSign == NONE_ID) {
			titleTextField._visible = false;
			descriptionTextField._visible = false;
		} else {
			var str = "$" + _birthsigns[_currentSign];
			
			titleTextField.SetText(Translator.translate(str));
			titleTextField._visible = true;
			
			var lines: Array = Translator.translate(str + "DESC").split("<");
			var desc = "";
			for (var i = 0; i < lines.length; ++i) {
				desc += lines[i] + "\n";	// newlines don't get treated as special characters, so this is a workaround
			}

			descriptionTextField.SetText(desc);
			descriptionTextField._visible = true;
		}
	}


	private function sendAcceptEvent(a_id: Number): Void
	{
		GameDelegate.call("SendAcceptEvent", [a_id]);
	}


	private function gotoSign(a_index: Number): Void
	{
		bsWindow.gotoAndStop(a_index + 1);
	}
}
