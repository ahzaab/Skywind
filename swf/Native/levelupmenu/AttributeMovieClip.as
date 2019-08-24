import gfx.events.EventTypes;
import gfx.io.GameDelegate;


[InspectableList("label")]
 class AttributeMovieClip extends gfx.controls.Button
{
	/* CONSTANTS */

	public static var TOGGLE_NOP: Number = 0;
	public static var TOGGLE_ON: Number = 1;
	public static var TOGGLE_OFF: Number = 2;

	private static var DF_INCR: Number = 1;


	/* PROPERTIES */

	[Inspectable(type="String", defaultValue="attributeName")]
	public var label;


	/* PRIVATE VARIABLES */

	private var _toggled = false;
	private var _av: Number = 0;
	private var _mod: Number = 0;
	private var _rollOverCallback: Object;
	private var _rollOutCallback: Object;
	private var _pressCallback: Object;


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

	public function init(a_av: Number, a_modGlobalFormID: Number): Void
	{
		addEventListener(EventTypes.ROLL_OVER, this, "rollOverHandler");
		addEventListener(EventTypes.ROLL_OUT, this, "rollOutHandler");
		addEventListener(EventTypes.PRESS, this, "pressHandler");

		_av = a_av;

		getPlayerAV(_av);
		getGlobal(a_modGlobalFormID);
	}


	public function setRollOverCallback(a_context: Object, a_method: String): Void
	{
		if (_rollOverCallback == undefined) {
			_rollOverCallback = new Object();
		}
		_rollOverCallback.context = a_context;
		_rollOverCallback.method = a_method;
	}


	public function setRollOutCallback(a_context: Object, a_method: String): Void
	{
		if (_rollOutCallback == undefined) {
			_rollOutCallback = new Object();
		}
		_rollOutCallback.context = a_context;
		_rollOutCallback.method = a_method;
	}


	public function setPressCallback(a_context: Object, a_method: String): Void
	{
		if (_pressCallback == undefined) {
			_pressCallback = new Object();
		}
		_pressCallback.context = a_context;
		_pressCallback.method = a_method;
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
			modPlayerAV(_av, _mod);
		}
	}


	public function setMod(a_value: Number): Void
	{
		_mod = a_value;
		if (_mod == undefined) {
			_mod = DF_INCR;
		}
		raiseMC.htmlText = "+" + _mod.toString();
	}


	public function setBase(a_value: Number): Void
	{
		baseMC.htmlText = a_value.toString();
	}


	// @override Object
	public function toString(): String
	{
		return name.text;
	}


	/* PRIVATE FUNCTIONS */

	private function rollOverHandler(a_event: Object): Void
	{
		invokeCallback(_rollOverCallback);
	}


	private function rollOutHandler(a_event: Object): Void
	{
		invokeCallback(_rollOutCallback);
	}


	private function pressHandler(a_event: Object): Void
	{
		invokeCallback(_pressCallback);
	}


	private function invokeCallback(a_info: Object): Void
	{
		var context: Object = a_info.context;
		var method: String = a_info.method;
		if (context != undefined && method != undefined) {
			context[method](this);
		}
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


	private function modPlayerAV(a_av: Number, a_newVal: Number): Void
	{
		GameDelegate.call("ModPlayerAV", [a_av, a_newVal]);	// skywind
	}
}
