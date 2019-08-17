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
	
	public var HealthButton: Button;
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
	

	public function InitExtensions(): Void
	{
		HealthButton.disabled = true;
		
		var attributeIDs: Array = [STRENGTH_ATTR, INTELLIGENCE_ATTR, WILLPOWER_ATTR, AGILITY_ATTR, SPEED_ATTR, ENDURANCE_ATTR, PERSONALITY_ATTR, LUCK_ATTR];
		var attributeMods: Array = [STRENGTH_MOD, INTELLIGENCE_MOD, WILLPOWER_MOD, AGILITY_MOD, SPEED_MOD, ENDURANCE_MOD, PERSONALITY_MOD, LUCK_MOD];
				
		for (var i: Number = 0; i < _attributeMCs.length; ++i) {
			_attributeMCs[i]["addEventListener"]("press", this, "onPressedAttribute");
			_attributeMCs[i]["addEventListener"]("rollOver", this, "onButtonGainFocus");
			_attributeMCs[i]["addEventListener"]("rollOut", this, "onButtonLoseFocus");
			_attributeMCs[i].setInfo(attributeIDs[i], attributeMods[i]);
		}
		
		HealthButton["addEventListener"]("press", this, "closeCallback");
		HealthButton["addEventListener"]("rollOver", this, "playFocusSound");
		
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
	

	public function onPressedAttribute(a_event: Object): Void
	{
		var toggleState: Number = AttributeMovieClip.TOGGLE_NOP;
		var canToggleOn: Boolean = _numSelected < MAX_SELECTABLE;
		if (a_event.sender != undefined) {
			var attributeMC: AttributeMovieClip = a_event.sender;
			toggleState = attributeMC.toggle(canToggleOn);
		}

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
		
		HealthButton.disabled = _numSelected != MAX_SELECTABLE;
	}
	

	public function closeCallback(a_event: Object): Void
	{
		playSound("UIMenuOK");
		for (var i: Number = 0; i < _attributeMCs.length; ++i) {
			_attributeMCs[i].onClose();
		}
		closeMenu();
	}
	
	
	public function onButtonGainFocus(a_event: Object): Void
	{
		a_event.sender.setHighlight();
		playFocusSound(a_event);
	}
	
	
	public function onButtonLoseFocus(a_event: Object): Void
	{
		a_event.sender.clearHighlight();
	}
	

	public function playFocusSound(a_event: Object): Void
	{
		playSound("UIMenuFocus");
	}
	
	
	public function getPlayerLevelCallback(a_level: Number): Void
	{
		
	}
	
	
	/* PRIVATE FUNCTIONS */
	
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
	
	
	private function playSound(a_sound: String): Void
	{
		GameDelegate.call("PlaySound", [a_sound]);	// skywind
	}
}
