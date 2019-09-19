import gfx.controls.Button;
import gfx.events.EventTypes;
import gfx.io.GameDelegate;


 class AttributeMovieClip extends Button
{
	/* CONSTANTS */

	public static var TOGGLE_NOP: Number = 0;
	public static var TOGGLE_ON: Number = 1;
	public static var TOGGLE_OFF: Number = 2;

	private static var MAX_AV: Number = 100;
	private static var MAX_INCR: Number = 5;
	private static var DF_INCR: Number = 1;
	private static var DF_PLUGIN: String = "Skywind.esm";


	/* PRIVATE VARIABLES */

	private var _toggled = false;
	private var _av: Number = 0;
	private var _modGlobal: Number = 0;
	private var _modVal: Number = DF_INCR;
	private var _dfIncr: Number = DF_INCR;
	private var _doDec: Boolean = false;
	private var _rollOverCallback: Object;
	private var _rollOutCallback: Object;
	private var _pressCallback: Object;


	/* STAGE ELEMENTS */

	public var raiseMC: TextField;
	public var baseMC: TextField;
	public var indicatorMC: MovieClip;


	/* INITIALIZATION */

	public function AttributeMovieClip()
	{
		super();
		indicatorMC._alpha = 0;
	}


	/* PUBLIC FUNCTIONS */

	public function init(a_av: Number, a_modForm: Number, a_dfIncrForm: Number): Void
	{
		if (!initialized) {
			onLoad();
		}

		addEventListener(EventTypes.ROLL_OVER, this, "rollOverHandler");
		addEventListener(EventTypes.ROLL_OUT, this, "rollOutHandler");
		addEventListener(EventTypes.PRESS, this, "pressHandler");

		_av = a_av;
		_modGlobal = a_modForm;

		getPlayerAV(_av, "setBase");
		getGlobal(a_dfIncrForm, DF_PLUGIN, "setDfIncr");
		getGlobal(_modGlobal, DF_PLUGIN, "setMod");
	}


	public function setRollOverCallback(a_context: Object, a_method: String): Void
	{
		setCallback(_rollOverCallback, a_context, a_method);
	}


	public function setRollOutCallback(a_context: Object, a_method: String): Void
	{
		setCallback(_rollOutCallback, a_context, a_method);
	}


	public function setPressCallback(a_context: Object, a_method: String): Void
	{
		setCallback(_pressCallback, a_context, a_method);
	}


	public function doToggle(a_canToggleOn: Boolean): Number
	{
		if (!_toggled && a_canToggleOn) {
			playSoundImpl("UIMenuOK");
			indicatorMC._alpha = 100;
			_toggled = true;
			return TOGGLE_ON;
		} else if (_toggled) {
			playSoundImpl("UIMenuOK");
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
			modPlayerAV(_av, _modVal);
			if (_doDec) {
				modGlobal(_modGlobal, -1 * _modVal, DF_PLUGIN);
			}
		}
	}


	/* PRIVATE FUNCTIONS */

	private function setBase(a_value: Number): Void
	{
		baseMC.htmlText = a_value.toString();

		if (a_value >= MAX_AV) {
			disabled = true;
		}
	}


	private function setDfIncr(a_value: Number): Void
	{
		_dfIncr = a_value != undefined ? a_value : DF_INCR;
	}


	private function setMod(a_value: Number): Void
	{
		_modVal = _dfIncr;
		_doDec = false;
		if (a_value != undefined && a_value > 0) {
			_modVal = Math.min(a_value, MAX_INCR);
			_doDec = true;
		}

		raiseMC.htmlText = "+" + _modVal.toString();
	}


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


	private function setCallback(a_callback: Object, a_context: Object, a_method: String): Void
	{
		if (a_callback == undefined) {
			a_callback = new Object();
		}
		a_callback.context = a_context;
		a_callback.method = a_method;
	}


	private function invokeCallback(a_info: Object): Void
	{
		var context: Object = a_info.context;
		var method: String = a_info.method;
		if (context != undefined && method != undefined) {
			context[method](this);
		}
	}


	private function getGlobal(a_formID: Number, a_plugin: String, a_callback: String): Void
	{
		GameDelegate.call("GetGlobal", [a_formID, a_plugin], this, a_callback);
	}


	private function getPlayerAV(a_av: Number, a_callback: String): Void
	{
		GameDelegate.call("GetPlayerAV", [a_av], this, a_callback);
	}


	private function log(a_str: String): Void
	{
		GameDelegate.call("Log", [a_str]);
	}


	private function modGlobal(a_formID: Number, a_mod: Number, a_plugin: String): Void
	{
		GameDelegate.call("ModGlobal", [a_formID, a_mod, a_plugin]);
	}


	private function modPlayerAV(a_av: Number, a_mod: Number): Void
	{
		GameDelegate.call("ModPlayerAV", [a_av, a_mod]);
	}


	private function playSoundImpl(a_sound: String): Void
	{
		GameDelegate.call("PlaySound", [a_sound]);
	}
}
