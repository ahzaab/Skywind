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
	private var _decVal: Number = 0;
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

	public function init(a_av: Number, a_modGlobalFormID: Number): Void
	{
		if (!initialized) {
			onLoad();
		}

		addEventListener(EventTypes.ROLL_OVER, this, "rollOverHandler");
		addEventListener(EventTypes.ROLL_OUT, this, "rollOutHandler");
		addEventListener(EventTypes.PRESS, this, "pressHandler");

		_av = a_av;
		_modGlobal = a_modGlobalFormID;

		getPlayerAV(_av);
		getGlobal(_modGlobal);
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
			if (_decVal < 0) {
				modGlobal(_modGlobal, _decVal);
			}
		}
	}


	/* PRIVATE FUNCTIONS */

	private function setMod(a_value: Number): Void
	{
		_modVal = DF_INCR;
		_decVal = 0;
		if (a_value != undefined && a_value > 0) {
			_modVal = a_value;
			_decVal = -1 * a_value;
		}

		raiseMC.htmlText = "+" + _modVal.toString();
	}


	private function setBase(a_value: Number): Void
	{
		baseMC.htmlText = a_value.toString();

		if (a_value >= MAX_AV) {
			disabled = true;
		}
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


	private function invokeCallback(a_info: Object): Void
	{
		var context: Object = a_info.context;
		var method: String = a_info.method;
		if (context != undefined && method != undefined) {
			context[method](this);
		}
	}


	private function getGlobal(a_formID: Number, a_plugin: String): Void
	{
		if (a_plugin == undefined) {
			a_plugin = DF_PLUGIN;
		}

		GameDelegate.call("GetGlobal", [a_formID, a_plugin], this, "setMod");
	}


	private function getPlayerAV(a_av: Number): Void
	{
		GameDelegate.call("GetPlayerAV", [a_av], this, "setBase");
	}


	private function log(a_str: String): Void
	{
		GameDelegate.call("Log", [a_str]);
	}


	private function modGlobal(a_formID: Number, a_mod: Number, a_plugin: String): Void
	{
		if (a_plugin == undefined) {
			a_plugin = DF_PLUGIN;
		}

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
