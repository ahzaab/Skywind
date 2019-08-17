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
	private var _av: Number;
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
		
		_base = getPlayerAV(_av);
		baseMC.htmlText = _base.toString();
		
		_mod = getGlobal(a_modGlobalFormID);
		if (_mod == undefined) {
			_mod = DF_INCR;
		}
		raiseMC.htmlText = "+" + _mod.toString();
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
	
	
	// @override Button
	function handleMouseRollOver(controllerIdx)
	{
		if (_disabled) {
			return undefined;
		}
		
		if ((!_focused && !_displayFocus) || focusIndicator != null) {
			setState("over");
		}
		
		dispatchEventAndSound({type: "rollOver", controllerIdx: controllerIdx, sender: this});
	}
	
	
	// @override Button
	function handleMouseRollOut(controllerIdx)
	{
		if (_disabled) {
			return undefined;
		}
		
		if ((!_focused && !_displayFocus) || focusIndicator != null) {
			setState("out");
		}
		
		dispatchEventAndSound({type: "rollOut", controllerIdx: controllerIdx, sender: this});
	}
	

	// @override Button
	public function handleMousePress(controllerIdx, keyboardOrMouse, button): Void
	{
		if (!_disableFocus) {
			Selection.setFocus(this, controllerIdx);
		}
		
		setState("down");
		dispatchEventAndSound({type: "press", controllerIdx: controllerIdx, button: button, sender: this});
		
		if (autoRepeat) {
			buttonRepeatInterval = setInterval(this, "beginButtonRepeat", buttonRepeatDelay, controllerIdx, button);
		}
	}
	

	// @override Object
	public function toString(): String
	{
		return name.text;
	}
	
	
	/* PRIVATE FUNCTIONS */
	
	public function getGlobal(a_formID: Number, a_plugin: String): Number
	{
		if (a_plugin == undefined) {
			a_plugin = "SkyWind.esm";
		}
		
		return _global.skse.plugins.SkyWind.getGlobal(a_formID, a_plugin);
	}
	
	
	public function getPlayerAV(a_av: Number): Number
	{
		return _global.skse.plugins.SkyWind.getPlayerAV(a_av);
	}
	
	
	private function close(): Void
	{
		GameDelegate.call("Close", []);	// skywind
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
