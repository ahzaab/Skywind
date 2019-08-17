import gfx.io.GameDelegate;


[InspectableList("label")]
 class AttributeMovieClip extends gfx.controls.Button
{
	/* CONSTANTS */
	
	public static var TOGGLE_NOP: Number = 0;
	public static var TOGGLE_ON: Number = 1;
	public static var TOGGLE_OFF: Number = 2;
	
	public static var DF_INCR: Number = 1;
	
	/* PROPERTIES */
	
	[Inspectable(type="String", defaultValue="attributeName")]
	public var label;
	
	/* PRIVATE VARIABLES */
	
	private var _toggled = false;
	private var _av: Number = 0;
	private var _base: Number = 0;
	private var _mod: Number = 0;
	
	/* STAGE ELEMENTS */
	
	public var name: TextField;
	public var raiseMC: TextField;
	public var baseMC: TextField;
	public var indicatorMC: MovieClip;
	
	
	/* INITIALIZATION */
	
	public function AttributeMovieClip()
	{
		super();
		indicatorMC._alpha = 0;
		name.htmlText = label;
	}
	
	
	/* PUBLIC FUNCTIONS */
	
	public function setInfo(a_av: Number, a_modGlobalFormID: Number): Void
	{
		_av = a_av;
		getPlayerAV(_av);
		getGlobal(a_modGlobalFormID);
	}
	

	public function toggle(a_canToggleOn: Boolean): Number
	{
		if (!_toggled && a_canToggleOn) {
			playSound("UIMenuOK");
			indicatorMC._alpha = 100;
			_toggled = true;
			return TOGGLE_ON;
		} else if (_toggled) {
			playSound("UIMenuOK");
			indicatorMC._alpha = 50;
			_toggled = false;
			return TOGGLE_OFF;
		} else {
			return TOGGLE_NOP;
		}
	}
	
	
	public function setHighlight(): Void
	{
		if (!_toggled && !_disabled) {
			indicatorMC._alpha = 50;
		}
	}
	
	
	public function clearHighlight(): Void
	{
		if (!_toggled && !_disabled) {
			indicatorMC._alpha = 0;
		}
	}
	
	
	public function onClose(): Void
	{
		if (_toggled) {
			updatePlayerAV(_av, _base + _mod);
		}
	}
	
	
	public function setMod(a_value: Number): Void
	{
		log("setMod");
		_mod = a_value;
		if (_mod == undefined) {
			_mod = DF_INCR;
		}
		raiseMC.htmlText = "+" + _mod.toString();
	}
	
	
	public function setBase(a_value: Number): Void
	{
		log("setBase");
		_base = a_value;
		baseMC.htmlText = _base.toString();
	}
	

	// @override Object
	public function toString(): String
	{
		return name.text;
	}
	
	
	/* PRIVATE FUNCTIONS */
	
	// @override Button
	private function handleMouseRollOver(controllerIdx: Number): Void
	{
		if (_disabled) {
			return;
		}
		
		if ((!_focused && !_displayFocus) || focusIndicator != null) {
			setState("over");	// Otherwise it is focused, and has no focusIndicator, so do nothing.
		}
		
		dispatchEventAndSound({type: "rollOver", controllerIdx: controllerIdx, sender: this});
	}
	
	
	// @override Button
	private function handleMouseRollOut(controllerIdx: Number): Void
	{
		if (_disabled) {
			return;
		}
		
		if ((!_focused && !_displayFocus) || focusIndicator != null) {
			setState("out");	// Otherwise it is focused, and has no focusIndicator, so do nothing.
		}
		
		dispatchEventAndSound({type: "rollOut", controllerIdx: controllerIdx, sender: this});
	}
	

	// @override Button
	private function handleMousePress(controllerIdx: Number, keyboardOrMouse: Number, button: Number): Void
	{
		if (_disabled) {
			return;
		}
		
		if (!_disableFocus) {
			Selection.setFocus(this, controllerIdx);
		}
		
		setState("down"); // Focus changes in the setState will override those in the changeFocus (above)
		dispatchEventAndSound({type: "press", controllerIdx: controllerIdx, button: button, sender: this});		
		if (autoRepeat) {
			buttonRepeatInterval = setInterval(this, "beginButtonRepeat", buttonRepeatDelay, controllerIdx, button);
		}
	}
	
	
	private function closeMenu(): Void
	{
		GameDelegate.call("CloseMenu", []);	// skywind
	}
	
	
	public function getGlobal(a_formID: Number, a_plugin: String): Void
	{
		if (a_plugin == undefined) {
			a_plugin = "SkyWind.esm";
		}
		
		GameDelegate.call("GetGlobal", [a_formID, a_plugin], this, "setMod");	// skywind
	}
	
	
	public function getPlayerAV(a_av: Number): Void
	{
		GameDelegate.call("GetPlayerAV", [a_av], this, "setBase");	// skywind
	}
	
	
	private function log(a_str: String): Void
	{
		GameDelegate.call("Log", [a_str]);	// skywind
	}
	
	
	private function playSound(a_sound: String): Void
	{
		GameDelegate.call("PlaySound", [a_sound]);	// skywind
	}
	
	
	private function updatePlayerAV(a_av: Number, a_newVal: Number): Void
	{
		GameDelegate.call("UpdatePlayerAV", [a_av, a_newVal]);	// skywind
	}
}
