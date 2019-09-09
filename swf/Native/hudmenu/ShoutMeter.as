import Shared.GlobalFunc;


class ShoutMeter
{
	/* PUBLIC VARIABLES */

	public var FlashClip: MovieClip;
	public var MeterEmtpy: Number;
	public var MeterFull: Number;
	public var ProgressClip: MovieClip;


	/* INITIALIZATION */

	function ShoutMeter(aProgressClip: MovieClip, aFlashClip: MovieClip)
	{
		ProgressClip = aProgressClip;
		FlashClip = aFlashClip;
		ProgressClip.gotoAndStop("Empty");
		MeterEmtpy = ProgressClip._currentframe;
		ProgressClip.gotoAndStop("Full");
		MeterFull = ProgressClip._currentframe;
		ProgressClip.gotoAndStop("Normal");
	}


	/* PUBLIC FUNCTIONS */

	function SetPercent(aPercent: Number): Void
	{
		if (aPercent >= 100) {
			ProgressClip.gotoAndStop("Normal");
			return;
		}
		aPercent = Math.min(100, Math.max(aPercent, 0));
		var aPercentFrame: Number = Math.floor(GlobalFunc.Lerp(MeterEmtpy, MeterFull, 0, 100, aPercent));
		ProgressClip.gotoAndStop(aPercentFrame);
	}


	function FlashMeter(): Void
	{
		FlashClip.gotoAndPlay("StartFlash");
	}
}
