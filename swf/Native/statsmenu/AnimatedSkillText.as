import Shared.GlobalFunc;
import Components.Meter;
import skyui.util.Debug;

class AnimatedSkillText extends MovieClip
{
   var SKILLS: Number = 18;
   var SKILL_ANGLE: Number = 20;
   var LocationsA: Array = [-150, -10, 130, 270, 410, 640, 870, 1010, 1150, 1290, 1430];
   var ThisInstance: AnimatedSkillText;

   function AnimatedSkillText()
   {
      super();
      ThisInstance = this;
   }

   function InitAnimatedSkillText(aSkillTextA: Array): Void
   {
      Debug.dump("aSkillTextA", aSkillTextA);
      GlobalFunc.MaintainTextFormat();
      var arrayStride: Number = 5;
      for (var i: Number = 0; i < aSkillTextA.length; i += arrayStride) {
         var SkillText = attachMovie("SkillText_mc", "SkillText" + i / arrayStride, getNextHighestDepth());
         SkillText.LabelInstance.html = true;
         SkillText.LabelInstance.htmlText = aSkillTextA[i + 1].toString().toUpperCase() + " <font face=\'$EverywhereBoldFont\' size=\'24\' color=\'" + aSkillTextA[i + 3].toString() + "\'>" + aSkillTextA[i].toString() + "</font>";
         var ShortBar: Meter = new Meter(SkillText.ShortBar);
         ShortBar.SetPercent(aSkillTextA[i + 2]);
         if(aSkillTextA[i + 4] > 0)
         {
            SkillText.LegendaryIconInfoInstance._alpha = 100;
            if(aSkillTextA[i + 4] > 1)
            {
               SkillText.LegendaryIconInfoInstance.LegendaryCountText.SetText(aSkillTextA[i + 4].toString(),false);
            }
            else
            {
               SkillText.LegendaryIconInfoInstance.LegendaryCountText.SetText("");
            }
         }
         else
         {
            SkillText.LegendaryIconInfoInstance._alpha = 0;
         }
         SkillText._x = LocationsA[0];
      }
   }

   function HideRing(): Void
   {
      for (var i: Number = 0; i < SKILLS; i++) {
         ThisInstance["SkillText" + i]._x = LocationsA[0];
      }
   }
   //This is mindboggling to make readable. So I kept it the same ~Greavesy
   function SetAngle(aAngle: Number): Void
   {
      var _loc6_ = Math.floor(aAngle / this.SKILL_ANGLE);
      var _loc10_ = aAngle % this.SKILL_ANGLE / this.SKILL_ANGLE;
      var _loc2_ = 0;
      while(_loc2_ < this.SKILLS)
      {
         var _loc11_ = this.LocationsA.length - 2;
         var _loc5_ = Math.floor(_loc11_ / 2) + 1;
         var _loc4_ = _loc6_ - _loc5_ < 0?_loc6_ - _loc5_ + this.SKILLS:_loc6_ - _loc5_;
         var _loc8_ = _loc6_ + _loc5_ >= this.SKILLS?_loc6_ + _loc5_ - this.SKILLS:_loc6_ + _loc5_;
         var _loc7_ = _loc4_ > _loc8_;
         if(!_loc7_ && (_loc2_ > _loc4_ && _loc2_ <= _loc8_) || _loc7_ && (_loc2_ > _loc4_ || _loc2_ <= _loc8_))
         {
            var _loc3_ = 0;
            if(!_loc7_)
            {
               _loc3_ = _loc2_ - _loc4_;
            }
            else
            {
               _loc3_ = _loc2_ <= _loc4_?_loc2_ + (this.SKILLS - _loc4_):_loc2_ - _loc4_;
            }
            _loc3_ = _loc3_ - 1;
            this.ThisInstance["SkillText" + _loc2_]._x = Shared.GlobalFunc.Lerp(this.LocationsA[_loc3_],this.LocationsA[_loc3_ + 1],1,0,_loc10_);
            var _loc9_ = (_loc3_ != 4?_loc10_ * 100:100 - _loc10_ * 100) * 0.75 + 100;
            this.ThisInstance["SkillText" + _loc2_]._xscale = !(_loc3_ == 5 || _loc3_ == 4)?100:_loc9_;
            this.ThisInstance["SkillText" + _loc2_]._yscale = !(_loc3_ == 5 || _loc3_ == 4)?100:_loc9_;
            this.ThisInstance["SkillText" + _loc2_].ShortBar._yscale = !(_loc3_ == 5 || _loc3_ == 4)?100:100 - (_loc9_ - 100) / 2.5;
         }
         else
         {
            this.ThisInstance["SkillText" + _loc2_]._x = this.LocationsA[0];
         }
         _loc2_ = _loc2_ + 1;
      }
   }

}