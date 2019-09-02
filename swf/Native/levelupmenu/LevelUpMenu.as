import gfx.controls.Button;
import gfx.io.GameDelegate;
import skyui.util.Translator;


dynamic class LevelUpMenu extends MovieClip
{
	/* CONSTANTS */

	public static var MAX_SELECTABLE: Number = 3;

	public static var STRENGTH_ATTR: Number = 79;
	public static var ENDURANCE_ATTR: Number = 80;
	public static var INTELLIGENCE_ATTR: Number = 81;
	public static var WILLPOWER_ATTR: Number = 95;
	public static var AGILITY_ATTR: Number = 90;
	public static var SPEED_ATTR: Number = 91;
	public static var PERSONALITY_ATTR: Number = 60;
	public static var LUCK_ATTR: Number = 61;

	public static var STRENGTH_MOD: Number = 0x07FEF0;
	public static var ENDURANCE_MOD: Number = 0x07FEF1;
	public static var INTELLIGENCE_MOD: Number = 0x07FEF2;
	public static var WILLPOWER_MOD: Number = 0x07FEF3;
	public static var AGILITY_MOD: Number = 0x07FEF4;
	public static var SPEED_MOD: Number = 0x07FEF5;
	public static var PERSONALITY_MOD: Number = 0x07FEF6;
	public static var LUCK_MOD: Number = 0x000000;


	/* PRIVATE VARIABLES */

	private var _numSelected: Number = 0;
	private var _attributeMCs: Array;


	/* STAGE ELEMENTS */

	public var ContinueMC: Button;
	public var ItemTextField: TextField;
	public var LevelUpTextField: TextField;

	public var StrengthMC: AttributeMovieClip;
	public var IntelligenceMC: AttributeMovieClip;
	public var WillpowerMC: AttributeMovieClip;
	public var AgilityMC: AttributeMovieClip;
	public var SpeedMC: AttributeMovieClip;
	public var EnduranceMC: AttributeMovieClip;
	public var PersonalityMC: AttributeMovieClip;
	public var LuckMC: AttributeMovieClip;


	/* INITIALIZATION */

	public function LevelUpMenu()
	{
		super();
		_attributeMCs = [StrengthMC, IntelligenceMC, WillpowerMC, AgilityMC, SpeedMC, EnduranceMC, PersonalityMC, LuckMC];
	}


	public function init(): Void
	{
		ContinueMC.disabled = true;

		var attributeIDs: Array = [STRENGTH_ATTR, INTELLIGENCE_ATTR, WILLPOWER_ATTR, AGILITY_ATTR, SPEED_ATTR, ENDURANCE_ATTR, PERSONALITY_ATTR, LUCK_ATTR];
		var attributeMods: Array = [STRENGTH_MOD, INTELLIGENCE_MOD, WILLPOWER_MOD, AGILITY_MOD, SPEED_MOD, ENDURANCE_MOD, PERSONALITY_MOD, LUCK_MOD];

		var len: Number = _attributeMCs.length;
		for (var i: Number = 0; i < len; ++i) {
			var attribute: AttributeMovieClip = _attributeMCs[i];
			attribute.setPressCallback(this, "onPressedAttribute");
			attribute.setRollOverCallback(this, "onButtonGainFocus");
			attribute.setRollOutCallback(this, "onButtonLoseFocus");
			attribute.init(attributeIDs[i], attributeMods[i]);
		}

		ContinueMC.addEventListener("press", this, "closeCallback");
		ContinueMC.addEventListener("rollOver", this, "playFocusSound");

		getPlayerLevel();
	}


	/* PUBLIC FUNCTIONS */

	public function setLevelText(a_level: Number): Void
	{
		var toTranslate: String = "$SK_ASCENDED{" + a_level.toString() + "}";
		ItemTextField.htmlText = Translator.translateNested(toTranslate);

		var levelUpText: String = "$SK_LEVEL";
		levelUpText += a_level.toString();

		if (a_level > 1 && a_level < 21) {
			LevelUpTextField.htmlText = levelUpText;
		} else {
			LevelUpTextField.htmlText = "$SK_LEVELDEFAULT";
		}
	}


	public function onPressedAttribute(a_attribute: AttributeMovieClip): Void
	{
		var canToggleOn: Boolean = _numSelected < MAX_SELECTABLE;
		var toggleState: Number = a_attribute.toggle(canToggleOn);

		switch (toggleState) {
			case AttributeMovieClip.TOGGLE_ON:
				if (canToggleOn) {
					++_numSelected;
				}
				break;
			case AttributeMovieClip.TOGGLE_OFF:
				--_numSelected;
				break;
		}

		ContinueMC.disabled = _numSelected != MAX_SELECTABLE;
	}


	public function closeCallback(a_event: Object): Void
	{
		playSoundImpl("UIMenuOK");
		var len: Number = _attributeMCs.length;
		for (var i: Number = 0; i < len; ++i) {
			var attribute: AttributeMovieClip = _attributeMCs[i];
			attribute.onClose();
		}
		advanceLevel();
		closeMenu();
	}


	public function onButtonGainFocus(a_attribute: AttributeMovieClip): Void
	{
		a_attribute.setHighlight();
		playSoundImpl("UIMenuFocus");
	}


	public function onButtonLoseFocus(a_attribute: AttributeMovieClip): Void
	{
		a_attribute.clearHighlight();
	}


	public function playFocusSound(a_event: Object): Void
	{
		playSoundImpl("UIMenuFocus");
	}


	/* PRIVATE FUNCTIONS */

	private function advanceLevel(): Void
	{
		GameDelegate.call("AdvanceLevel", []);	// skywind
	}


	private function closeMenu(): Void
	{
		GameDelegate.call("CloseMenu", []);	// skywind
	}


	private function getPlayerLevel(): Void
	{
		GameDelegate.call("GetPlayerLevel", [], this, "setLevelText");	// skywind
	}


	private function log(a_str: String): Void
	{
		GameDelegate.call("Log", [a_str]);	// skywind
	}


	private function playSoundImpl(a_sound: String): Void
	{
		GameDelegate.call("PlaySound", [a_sound]);	// skywind
	}
}
