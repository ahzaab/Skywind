import Shared.ButtonChange;
import Components.Meter;
import gfx.io.GameDelegate;
import Shared.GlobalFunc;
import mx.utils.Delegate;

class StatsMenu extends MovieClip
{
   //StatMenus (Statics).
   static var StatsMenuInstance: StatsMenu = null;
   //Integers (Statics).
   static var MAGICKA_METER: Number = 0;
   static var HEALTH_METER: Number = 1;
   static var STAMINA_METER: Number = 2;
   static var CURRENT_METER_TEXT: Number = 0;
   static var MAX_METER_TEXT: Number = 1;
   static var ALTERATION: Number = 0;
   static var CONJURATION: Number = 1;
   static var DESTRUCTION: Number = 2;
   static var MYSTICISM: Number = 3;
   static var RESTORATION: Number = 4;
   static var ENCHANTING: Number = 5;
   static var LIGHT_ARMOR: Number = 6;
   static var PICKPOCKET: Number = 7;
   static var LOCKPICKING: Number = 8;
   static var SNEAK: Number = 9;
   static var ALCHEMY: Number = 10;
   static var SPEECHCRAFT: Number = 11;
   static var ONE_HANDED_WEAPONS: Number = 12;
   static var TWO_HANDED_WEAPONS: Number = 13;
   static var MARKSMAN: Number = 14;
   static var BLOCK: Number = 15;
   static var SMITHING: Number = 16;
   static var HEAVY_ARMOR: Number = 17;
   static var BeginAnimation: Number = 0;
   static var EndAnimation: Number = 1000;
   static var STATS: Number = 0;
   static var LEVEL_UP: Number = 1;
   static var MaxPerkNameHeight: Number = 115;
   static var MaxPerkNameHeightLevelMode: Number = 175;
   static var MaxPerkNamesDisplayed: Number = 64;
   //Arrays (Statics).   
   static var SkillStatsA = new Array();
   static var PerkNamesA = new Array();
   static var SkillsA: Array;
   static var MeterText: Array;
   //Meters (Statics).
   static var MagickaMeter: Meter;
   static var HealthMeter: Meter;
   static var StaminaMeter: Meter;  
   //Movieclips (Statics).
   static var SkillRing_mc: MovieClip;
   static var MagickaMeterBase: MovieClip;
   static var HealthMeterBase: MovieClip;
   static var StaminaMeterBase: MovieClip;
   //Numbers.
   var CameraUpdateInterval: Number = 0;
   var iCurrentSkillLevel: Number = 15;
   var CurrentPerkFrame: Number;
   var PerkEndFrame: Number;
   var PerksLeft: Number;
   var Platform: Number;
   var State: Number;
   //Booleans.
   var bLegendaryInstalled: Boolean = false;
   var bInPerkMode: Boolean = false;
   //Movieclips.
   var AddPerkButtonInstance: MovieClip;
   var AddPerkTextInstance: MovieClip;
   var BottomBarInstance: MovieClip;
   var CameraMovementInstance: MovieClip;  
   var PerkName0: MovieClip;
   var TopPlayerInfo: MovieClip;
   var SkillsListInstance: MovieClip;
   var DescriptionCardInstance: MovieClip;
   //Meters.
   var DescriptionCardMeter: Meter;
   var LevelMeter: Meter;
   //AnimatedSkillText
   var AnimatingSkillTextInstance: AnimatedSkillText;

   function StatsMenu()
   {
      super();
      StatsMenu.StatsMenuInstance = this;
      DescriptionCardMeter = new Meter(StatsMenu.StatsMenuInstance.DescriptionCardInstance.animate);
      StatsMenu.SkillsA = new Array();
      StatsMenu.SkillRing_mc = SkillsListInstance;
      SetDirection(0);
      var playerInfoCard:MovieClip = BottomBarInstance.BottomBarPlayerInfoInstance.PlayerInfoCardInstance;
      StatsMenu.MagickaMeterBase = playerInfoCard.MagickaMeterInstance;
      StatsMenu.HealthMeterBase = playerInfoCard.HealthMeterInstance;
      StatsMenu.StaminaMeterBase = playerInfoCard.StaminaMeterInstance;
      StatsMenu.MagickaMeter = new Components.Meter(StatsMenu.MagickaMeterBase.MagickaMeter_mc);
      StatsMenu.HealthMeter = new Components.Meter(StatsMenu.HealthMeterBase.HealthMeter_mc);
      StatsMenu.StaminaMeter = new Components.Meter(StatsMenu.StaminaMeterBase.StaminaMeter_mc);
      StatsMenu.MagickaMeterBase.Magicka.gotoAndStop("Pause");
      StatsMenu.HealthMeterBase.Health.gotoAndStop("Pause");
      StatsMenu.StaminaMeterBase.Stamina.gotoAndStop("Pause");
      StatsMenu.MeterText = [playerInfoCard.magicValue,playerInfoCard.healthValue,playerInfoCard.enduranceValue];
      SetMeter(StatsMenu.MAGICKA_METER, 50, 100);
      SetMeter(StatsMenu.HEALTH_METER, 75, 100);
      SetMeter(StatsMenu.STAMINA_METER, 25, 100);
      Platform = ButtonChange.PLATFORM_PC;
      AddPerkButtonInstance._alpha = 0;
      for (var i: Number = 0; i < StatsMenu.MaxPerkNamesDisplayed; i++) 
      {
         var perkName: MovieClip = attachMovie("PerkName", "PerkName" + i, getNextHighestDepth());
         perkName._x = -100 - _x;
      }
      TopPlayerInfo.swapDepths(getNextHighestDepth());
      SetStatsMode(true, 0);
      CurrentPerkFrame = 0;
      PerkName0.gotoAndStop("Visible");
      PerkEndFrame = PerkName0._currentFrame;
      PerkName0.gotoAndStop("Invisible");
   }
   function GetSkillClip(aSkillName: String): TextField
   {
      return SkillsListInstance.BaseRingInstance[aSkillName].Val.ValText;
   }
   function UpdatePerkText(abShow: Boolean): Void
   {
      if (abShow == true || abShow == undefined) {
         var perkIdx: Number = 0;
         var perkAdded: Boolean = false;
         for (var i: Number = 0; i < StatsMenu.PerkNamesA.length; i += 3) {
            
            if (StatsMenu.PerkNamesA[i] == undefined) {
               if (!perkAdded && this["PerkName" + perkIdx] != undefined) {
                  this["PerkName" + perkIdx].gotoAndStop("Invisible");
               }
            } else if (GlobalFunc.Lerp(0, 720, 0, 1, StatsMenu.PerkNamesA[i + 2]) > (State == StatsMenu.LEVEL_UP ? StatsMenu.MaxPerkNameHeightLevelMode : StatsMenu.MaxPerkNameHeight)) {
               this["PerkName" + perkIdx].PerkNameClipInstance.NameText.html = true;
               this["PerkName" + perkIdx].PerkNameClipInstance.NameText.SetText(StatsMenu.PerkNamesA[i], true);
               this["PerkName" + perkIdx]._xscale = StatsMenu.PerkNamesA[i + 2] * 165 + 10;
               this["PerkName" + perkIdx]._yscale = StatsMenu.PerkNamesA[i + 2] * 165 + 10;
               this["PerkName" + perkIdx]._x = GlobalFunc.Lerp(0, 1280, 0, 1, StatsMenu.PerkNamesA[i + 1]) - _x;
               this["PerkName" + perkIdx]._y = GlobalFunc.Lerp(0, 720, 0, 1, StatsMenu.PerkNamesA[i + 2]) - _y;
               this["PerkName" + perkIdx].bPlaying = true;
               if (this["PerkName" + perkIdx] != undefined) {
                  this["PerkName" + perkIdx].gotoAndStop(CurrentPerkFrame);
               }
               perkIdx++;
               perkAdded = true;
            }

         }

         for (var j: Number = perkIdx; j <= StatsMenu.MaxPerkNamesDisplayed; j++) {
            if (this["PerkName" + j] != undefined) {
               this["PerkName" + j].gotoAndStop("Invisible");
            }
         }

         if (CurrentPerkFrame <= PerkEndFrame) {
            CurrentPerkFrame++;
         }

         return;
      }



      if (abShow == false) {
         CurrentPerkFrame = 0;
         for (var i: Number = 0; i < StatsMenu.MaxPerkNamesDisplayed; i++) {
            if (this["PerkName" + i] != undefined) {
               this["PerkName" + i].gotoAndStop("Invisible");
            }
         }
      }
   }
   function InitExtensions(): Void
   {
      GlobalFunc.SetLockFunction();
      GlobalFunc.MaintainTextFormat();
      GameDelegate.addCallBack("SetDescriptionCard", this, "SetDescriptionCard");
      GameDelegate.addCallBack("SetPlayerInfo", this, "SetPlayerInfo");
      GameDelegate.addCallBack("UpdateSkillList", this, "UpdateSkillList");
      GameDelegate.addCallBack("SetDirection", this, "SetDirection");
      GameDelegate.addCallBack("HideRing", this, "HideRing");
      GameDelegate.addCallBack("SetStatsMode", this, "SetStatsMode");
      GameDelegate.addCallBack("SetPerkCount", this, "SetPerkCount");
   }
   function SetStatsMode(abStats: Boolean, aPerkCount: Number): Void
   {
      State = abStats ? StatsMenu.STATS : StatsMenu.LEVEL_UP;
      PerksLeft = aPerkCount;
      if (aPerkCount != undefined) {
         SetPerkCount(aPerkCount);
      }
   }
   function ShowLegendaryButtonHint()
   {
      bLegendaryInstalled = true;
      AddPerkTextInstance._alpha = 100;
      AddPerkTextInstance.gotoAndStop("Legendary");
      AddPerkTextInstance.LegendaryButtonInstance.SetPlatform(StatsMenu.StatsMenuInstance.Platform,false);
   }
   function SetPerkCount(aPerkCount: Number): Void
   {
      if (aPerkCount > 0) {
         AddPerkTextInstance._alpha = 100;
         AddPerkTextInstance.AddPerkTextField.text = _root.PerksInstance.text + " " + aPerkCount;
         return;
      }
      AddPerkTextInstance._alpha = 0;
   }
   function UpdateCamera(): Void
   {
      if (StatsMenu.StatsMenuInstance.CameraMovementInstance._currentFrame < 100) {
         var nextFrame: Number = StatsMenu.StatsMenuInstance.CameraMovementInstance._currentFrame + 8;
         if (nextFrame > 100) {
            nextFrame = 100;
         }
         GameDelegate.call("MoveCamera", [CameraMovementInstance.CameraPositionAlpha._alpha / 100]);
         return;
      }
      clearInterval(CameraUpdateInterval);
      CameraUpdateInterval = 0;
   }
   function UpdateSkillList()
   {
      StatsMenu.StatsMenuInstance.AnimatingSkillTextInstance.InitAnimatedSkillText(StatsMenu.SkillStatsA);
   }
   function HideRing()
   {
      StatsMenu.StatsMenuInstance.AnimatingSkillTextInstance.HideRing();
   }
   function SetDirection(aAngle)
   {
      StatsMenu.StatsMenuInstance.AnimatingSkillTextInstance.SetAngle(aAngle);
   }
   function SetPlayerInfo(): Void
   {
      StatsMenu.StatsMenuInstance.TopPlayerInfo.FirstLastLabel.textAutoSize = "shrink";
      StatsMenu.StatsMenuInstance.TopPlayerInfo.FirstLastLabel.SetText(arguments[0]);
      StatsMenu.StatsMenuInstance.TopPlayerInfo.LevelNumberLabel.SetText(arguments[1]);
      if (LevelMeter == undefined) {
         LevelMeter = new Meter(StatsMenu.StatsMenuInstance.TopPlayerInfo.animate);
      }
      LevelMeter.SetPercent(arguments[2]);
      StatsMenu.StatsMenuInstance.TopPlayerInfo.RacevalueLabel.SetText(arguments[3]);
      SetMeter(0, arguments[4], arguments[5], arguments[6]);
      SetMeter(1, arguments[7], arguments[8], arguments[9]);
      SetMeter(2, arguments[10], arguments[11], arguments[12]);
   }
   function SetMeter(aMeter: Number, aCurrentValue: Number, aMaxValue: Number, aColor: Number): Void
   {
      if (aMeter >= StatsMenu.MAGICKA_METER && aMeter <= StatsMenu.STAMINA_METER) {
         var meterPercent: Number = 100 * (Math.max(0, Math.min(aCurrentValue, aMaxValue)) / aMaxValue);
         
         switch (aMeter) {
            case StatsMenu.MAGICKA_METER:
               StatsMenu.MagickaMeter.SetPercent(meterPercent);
               break;
            case StatsMenu.HEALTH_METER:
               StatsMenu.HealthMeter.SetPercent(meterPercent);
               break;
            case StatsMenu.STAMINA_METER:
               StatsMenu.StaminaMeter.SetPercent(meterPercent);
               break;
         }
         
         StatsMenu.MeterText[aMeter].html = true;
         if (aColor == undefined) {
            StatsMenu.MeterText[aMeter].SetText(aCurrentValue + "/" + aMaxValue, true);
         } else {
            StatsMenu.MeterText[aMeter].SetText("<font color=\'" + aColor + "\'>" + aCurrentValue + "/" + aMaxValue + "</font>", true);
         }
         StatsMenu.MagickaMeter.Update();
         StatsMenu.HealthMeter.Update();
         StatsMenu.StaminaMeter.Update();
      }
   }
   function SetDescriptionCard(abPerkMode: Boolean, aName: String, aMeterPercent: Number, aDescription: String, aRequirements: String, aSkillLevel: String, aSkill: String): Void
   {
      if (StatsMenu.StatsMenuInstance != undefined) {
         StatsMenu.StatsMenuInstance.gotoAndStop(abPerkMode ? "Perks" : "Skills");
      }
      var descriptionCard: MovieClip = StatsMenu.StatsMenuInstance.DescriptionCardInstance;
      descriptionCard.CardDescriptionTextInstance.SetText(aDescription);
      AddPerkButtonInstance._alpha = State == StatsMenu.LEVEL_UP && abPerkMode && Platform != ButtonChange.PLATFORM_PC ? 100 : 0;
      if (!abPerkMode) {
         descriptionCard.CardNameTextInstance.html = true;
         descriptionCard.CardNameTextInstance.htmlText = aName.toUpperCase() + " <font face=\'$EverywhereBoldFont\' size=\'32\' color=\'#FFFFFF\'>" + aSkillLevel + "</font>";
         StatsMenu.StatsMenuInstance.DescriptionCardMeter.SetPercent(aMeterPercent);
         return;
      }
      descriptionCard.CardNameTextInstance.SetText("");
      descriptionCard.SkillRequirementText.html = true;
      descriptionCard.SkillRequirementText.htmlText = aSkill.toUpperCase() + "          " + aRequirements.toUpperCase(); //10 spaces
      if (PerksLeft != undefined) {
         SetPerkCount(PerksLeft);
      }
      descriptionCard.Perktype.SetText(aSkill);
   }
   static function SetPlatform(aiPlatformIndex: Number, abPS3Switch: Boolean): Void
   {
      StatsMenu.StatsMenuInstance.AddPerkButtonInstance.SetPlatform(aiPlatformIndex, abPS3Switch);
      StatsMenu.StatsMenuInstance.Platform = aiPlatformIndex;
   }
   static function StartCameraAnimation(): Void
   {
      clearInterval(StatsMenu.StatsMenuInstance.CameraUpdateInterval);
      GameDelegate.call("MoveCamera", [0]);
      StatsMenu.StatsMenuInstance.CameraUpdateInterval = setInterval(Delegate.create(StatsMenu.StatsMenuInstance, StatsMenu.StatsMenuInstance.UpdateCamera), 41);
   }
}
