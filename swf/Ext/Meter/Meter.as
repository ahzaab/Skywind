import Shared.GlobalFunc;
import flash.geom.ColorTransform;
import flash.geom.Matrix;
import flash.geom.Transform;
import gfx.core.UIComponent;
import skyui.util.ColorFunctions;

class Meter extends UIComponent
{
	/* CONSTANTS */

	public static var FILL_DIRECTION_LEFT: String = "left";
	public static var FILL_DIRECTION_RIGHT: String = "right";
	public static var FILL_DIRECTION_BOTH: String = "both";

	private static var MAX_PCT: Number = 100;
	private static var MIN_PCT: Number = 0;


	/* PRIVATE VARIABLES */

	private var _originalWidth: Number;
	private var _originalHeight: Number;
	private var _originalCapWidth: Number;
	private var _originalCapHeight: Number;
	private var _originalMeterFillHolderWidth: Number;

	private var _meterFrameContent: MovieClip;
	private var _meterFillHolder: MovieClip;
	private var _meterFillContent: MovieClip;
	private var _meterFlashAnim: MovieClip;
	private var _meterAlphaBarAnim: MovieClip;
	private var _meterAlphaBar: MovieClip;
	private var _meterInstantBar: MovieClip;

	private var _currentPercent: Number = 0;
	private var _targetPercent: Number = 0;
	private var _emptyIdx: Number;
	private var _fullIdx: Number;

	private var _fillDirection: String;
	private var _secondaryColor: Number;
	private var _primaryColor: Number;
	private var _flashColor: Number;
	private var _flashColorAuto: Boolean = false;

	private var colorIsInvalid: Boolean = false;
	private var flashColorIsInvalid: Boolean = false;
	private var fillDirectionIsInvalid: Boolean = true;


	/* PUBLIC VARIABLES */

	public var fillDelta: Number = 2;
	public var emptyDelta: Number = 3;


	/* STAGE ELEMENTS */

	public var meterContent: MovieClip;
	public var background: MovieClip;


	/* INITIALIZATION */

	public function Meter()
	{
		super();

		//background._visible = meterContent.capBackground._visible = false;

		_meterFrameContent = meterContent.meterFrameHolder.meterFrameContent;
		_meterFillHolder = meterContent.meterFillHolder;
		_meterFillContent = _meterFillHolder.meterFillContent;
		_meterFlashAnim = _meterFrameContent.meterFlashAnim;
		_meterAlphaBarAnim = _meterFillContent.meterBarAnim;
		_meterAlphaBar = _meterAlphaBarAnim.meterAlphaBar;
		_meterInstantBar = _meterAlphaBarAnim.meterInstantBar;

		_originalWidth = background._width;
		_originalHeight = background._height;
		_originalCapWidth = meterContent.capBackground._width;
		_originalCapHeight = meterContent.capBackground._height;
		_originalMeterFillHolderWidth = _meterFillHolder._width;

		//_meterFillHolder._x = _originalCapWidth;

		// Set stage dimensions to original dimensions and invalidate size
		_width = _originalWidth;
		_height = _originalHeight;
	}


	/* PROPERTIES */

	// @override UIComponent
	public function get height(): Number
	{
		return background._height;
	}


	public function get color(): Number
	{
		return _primaryColor;
	}


	public function set color(a_primaryColor: Number): Void
	{
		var lightColor: Number = (a_primaryColor == undefined) ? 0xFFFFFF: ColorFunctions.validHex(a_primaryColor);
		if (lightColor == _primaryColor) {
			return;
		}
		_primaryColor = lightColor;

		var darkColorHSV: Array = ColorFunctions.hexToHsv(lightColor);
		darkColorHSV[2] -= 40;

		_secondaryColor = ColorFunctions.hsvToHex(darkColorHSV);

		colorIsInvalid = true;
	}


	public function setColors(a_primaryColor: Number, a_secondaryColor: Number, a_flashColor: Number): Void
	{
		flashColor = a_flashColor;

		if (a_secondaryColor == undefined || a_secondaryColor < 0x000000) {
			color = a_primaryColor;
			return;
		}

		_primaryColor = (a_primaryColor == undefined) ? 0xFFFFFF: ColorFunctions.validHex(a_primaryColor);
		_secondaryColor = ColorFunctions.validHex(a_secondaryColor);

		colorIsInvalid = true;
	}


	public function get flashColor(): Number
	{
		return _flashColor;
	}


	public function set flashColor(a_flashColor: Number): Void
	{
		var rgb: Number;
		_flashColorAuto = false;

		if ((a_flashColor < 0x000000 || a_flashColor == undefined) && _primaryColor != undefined) {
			rgb = _primaryColor;
			_flashColorAuto = true;
		} else if (a_flashColor == undefined) {
			rgb = 0xFFFFFF;
		} else {
			rgb = ColorFunctions.validHex(a_flashColor);
		}

		if (_flashColor == rgb) {
			return;
		}
		_flashColor = rgb;

		flashColorIsInvalid = true;
	}


	public function get fillDirection(): String
	{
		return _fillDirection;
	}


	public function set fillDirection(a_fillDirection: String): Void
	{
		setFillDirection(a_fillDirection)
	}


	public function setFillDirection(a_fillDirection: String): Void
	{
		var fillDirection: String = a_fillDirection.toLowerCase();
		if (_fillDirection == fillDirection) {
			return;
		}
		_fillDirection = fillDirection;

		fillDirectionIsInvalid = true;
	}


	public function get percent(): Number
	{
		return _targetPercent;
	}


	public function set percent(a_percent: Number): Void
	{
		setPercent(a_percent, false);
	}


	public function setPercent(a_percent: Number, a_force: Boolean): Void
	{
		_targetPercent = Math.min(MAX_PCT, Math.max(a_percent, MIN_PCT));
		invalidate();

		if (a_force) {
			_currentPercent = _targetPercent;
			var meterFrame: Number = Math.floor(GlobalFunc.Lerp(_emptyIdx, _fullIdx, MIN_PCT, MAX_PCT, _currentPercent, false));
			_meterAlphaBarAnim.gotoAndStop(meterFrame);
		}
	}


	public function startFlash(a_force: Boolean): Void
	{
		// meterFlashing is set on the timeline and is false once the animation has finished
		if (_meterFlashAnim.meterFlashing && !a_force) {
			return;
		}

		_meterFlashAnim.gotoAndPlay("StartFlash");
	}


	/* PRIVATE FUNCTIONS */

	// @override UIComponent
	private function draw(): Void
	{
		super.draw();

		if (sizeIsInvalid) {
			invalidateSize();
		}

		if (fillDirectionIsInvalid) {
			invalidateFillDirection();
		}

		if (colorIsInvalid) {
			invalidateColor();
		}

		if (flashColorIsInvalid) {
			invalidateFlashColor();
		}

		drawImpl();
	}


	private function drawImpl(): Void
	{
		/*if (!initialized) {
			_currentPercent = _targetPercent;
		} else*/ if (_targetPercent == _currentPercent) {
			return;
		}

		if (_currentPercent < _targetPercent) {
			_currentPercent = _currentPercent + fillDelta;
			if (_currentPercent > _targetPercent) {
				_currentPercent = _targetPercent;
			}
		} else {
			_currentPercent = _currentPercent - emptyDelta;
			if (_currentPercent < _targetPercent) {
				_currentPercent = _targetPercent;
			}
		}

		_currentPercent = Math.min(MAX_PCT, Math.max(_currentPercent, MIN_PCT));
		var meterFrame: Number = Math.floor(GlobalFunc.Lerp(_emptyIdx, _fullIdx, MIN_PCT, MAX_PCT, _currentPercent, false));
		_meterAlphaBarAnim.gotoAndStop(meterFrame);
	}


	private function invalidateSize(): Void
	{
		var safeWidth: Number = _originalCapWidth * 3; // Safe width is 3* size of cap
		var safeHeight: Number;

		if (__width < safeWidth) {
			//3 times cap width is our minumum
			__width = safeWidth;
		}

		// Safe height of meter is 80% of the max height
		safeHeight = ((_originalCapHeight/_originalCapWidth) * __width/2) * 0.80;

		if (__height > safeHeight) {
			__height = safeHeight;
		}

		background._width = __width;
		background._height = __height;

		// Calculate scaling percent of the meter based on heights
		var scalePercent: Number = __height/_originalHeight;

		// Scale the meterContent based on height so the caps AR is maintained
		meterContent._xscale = meterContent._yscale = scalePercent * 100;

		// Scale inner content
		// Scale meterFrameContent instead of meterFrameHolder due to scale9Grid
		_meterFrameContent._width = __width / scalePercent; // newWidth = oldWidth * newPercent/oldPercent /. newPercent -> 100
		_meterFillHolder._xscale = ((_meterFrameContent._width - 2*_originalCapWidth)/_originalMeterFillHolderWidth) * 100;
	}


	private function invalidateFillDirection(): Void
	{
		switch(_fillDirection) {
			case FILL_DIRECTION_LEFT:
			case FILL_DIRECTION_BOTH:
			case FILL_DIRECTION_RIGHT:
				break;
			default:
				_fillDirection = FILL_DIRECTION_LEFT;
		}

		_meterFillContent.gotoAndStop(_fillDirection);
		_meterAlphaBar = _meterAlphaBarAnim.meterAlphaBar;
		_meterInstantBar = _meterAlphaBarAnim.meterInstantBar;

		drawMeterGradients();

		_meterAlphaBarAnim.gotoAndStop("Full");
		_fullIdx = _meterAlphaBarAnim._currentframe;
		_meterAlphaBarAnim.gotoAndStop("Empty");
		_emptyIdx = _meterAlphaBarAnim._currentframe;

		setPercent(_currentPercent, true);

		fillDirectionIsInvalid = false;
	}


	// Draws the meter
	private function drawMeterGradients(): Void
	{
		var bars: Array = [_meterAlphaBar, _meterInstantBar];
		var len: Number = bars.length;
		for (var i: Number = 0; i < len; ++i) {
			var bar: MovieClip = bars[i];

			var w: Number = bar._width;
			var h: Number = bar._height;
			var meterBevel: MovieClip = bar.meterBevel;
			var meterShine: MovieClip = bar.meterShine;

			var colors: Array = [0xCCCCCC, 0xFFFFFF, 0x000000, 0x000000, 0x000000];
			var alphas: Array = [10, 60, 0, 10, 30];
			var ratios: Array = [0, 115, 128, 128, 255];
			var matrix: Matrix = new Matrix();

			if (meterShine != undefined) {
				continue;
			}

			meterShine = bar.createEmptyMovieClip("meterShine", 2);

			meterBevel.swapDepths(1);
			matrix.createGradientBox(w, h, Math.PI/2);
			meterShine.beginGradientFill("linear", colors, alphas, ratios, matrix);
			meterShine.moveTo(0,0);
			meterShine.lineTo(w, 0);
			meterShine.lineTo(w, h);
			meterShine.lineTo(0, h);
			meterShine.lineTo(0, 0);
			meterShine.endFill();
		}

		invalidateColor();
	}


	private function invalidateColor(): Void
	{
		var bars: Array = [_meterAlphaBar, _meterInstantBar];
		var len: Number = bars.length;
		for (var i: Number = 0; i < len; ++i) {
			var bar: MovieClip = bars[i];

			var colors: Array;
			var alphas: Array;
			var ratios: Array;
			var w: Number = bar._width;
			var h: Number = bar._height;
			var meterGradient: MovieClip = bar.meterGradient;
			var matrix: Matrix = new Matrix();

			if (meterGradient != undefined) {
				meterGradient.removeMovieClip();
			}

			meterGradient = bar.createEmptyMovieClip("meterGradient", 0);

			switch(_fillDirection) {
				case FILL_DIRECTION_LEFT:
					colors = [_secondaryColor, _primaryColor];
					alphas = [100, 100];
					ratios = [0, 255];
					break;
				case FILL_DIRECTION_BOTH:
					colors = [_secondaryColor, _primaryColor, _secondaryColor];
					alphas = [100, 100, 100];
					ratios = [0, 127, 255];
					break;
				case FILL_DIRECTION_RIGHT:
				default:
					colors = [_primaryColor, _secondaryColor];
					alphas = [100, 100];
					ratios = [0, 255];
			}

			matrix.createGradientBox(w, h);
			meterGradient.beginGradientFill("linear", colors, alphas, ratios, matrix);
			meterGradient.moveTo(0,0);
			meterGradient.lineTo(w, 0);
			meterGradient.lineTo(w, h);
			meterGradient.lineTo(0, h);
			meterGradient.lineTo(0, 0);
			meterGradient.endFill();
		}

		if (_flashColorAuto || !initialized) {
			if (_flashColorAuto) {
				_flashColor = _primaryColor;
			}
			invalidateFlashColor();
		}

		colorIsInvalid = false;
	}


	private function invalidateFlashColor(): Void
	{
		var tf: Transform = new Transform(_meterFlashAnim);
		var colorTf: ColorTransform = new ColorTransform();
		colorTf.rgb = _flashColor;
		tf.colorTransform = colorTf;

		flashColorIsInvalid = false;
	}
}
