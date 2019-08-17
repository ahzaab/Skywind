import gfx.io.GameDelegate;
import gfx.managers.FocusHandler;
import Components.CrossPlatformButtons
					   
class StartMenu extends MovieClip
{
   static var PRESS_START_STATE: String = "PressStart";
   static var MAIN_STATE: String = "Main";
   static var MAIN_CONFIRM_STATE: String = "MainConfirm";
   static var CHARACTER_LOAD_STATE: String = "CharacterLoad";
   static var CHARACTER_SELECTION_STATE: String = "CharacterSelection";
   static var SAVE_LOAD_STATE: String = "SaveLoad";
   static var SAVE_LOAD_CONFIRM_STATE: String = "SaveLoadConfirm";
   static var DELETE_SAVE_CONFIRM_STATE: String = "DeleteSaveConfirm";
   static var DLC_STATE: String = "DLC";
   static var MARKETPLACE_CONFIRM_STATE: String = "MarketplaceConfirm";
   static var START_ANIM_STR: String = "StartAnim";
   static var END_ANIM_STR: String = "EndAnim";
   static var CONTINUE_INDEX: Number = 0;
   static var NEW_INDEX: Number = 1;
   static var LOAD_INDEX: Number = 2;
   static var DLC_INDEX: Number = 3;
   static var MOD_INDEX: Number = 4;
   static var CREDITS_INDEX: Number = 5;
   static var QUIT_INDEX: Number = 6;
   static var HELP_INDEX: Number = 7;
   static var LOADING_ICON_OFFSET: Number = 50;
   static var PLATFORM_PC_KBMOUSE: Number = 0;
   static var PLATFORM_PC_GAMEPAD: Number = 1;
   static var PLATFORM_DURANGO: Number = 2;
   static var PLATFORM_ORBIS: Number = 3;
   static var ALPHA_AVAILABLE: Number = 100;
   static var ALPHA_DISABLED: Number = 50;
   
    var ButtonRect: MovieClip;
	var ConfirmPanel_mc: MovieClip;
	var DLCList_mc: MovieClip;
	var DLCPanel: MovieClip;
	var DeleteButton: CrossPlatformButtons;
	var DeleteSaveButton: CrossPlatformButtons;
	var DeleteMouseButton: CrossPlatformButtons;
	var ChangeUserButton: CrossPlatformButtons;
	var LoadingContentMessage: MovieClip;
	var CharacterSelectionHint: MovieClip;
	var Logo_mc: MovieClip;
	var MainList: MovieClip;
	var MainListHolder: MovieClip;
	var MarketplaceButton: CrossPlatformButtons;
	var SaveLoadConfirmText: TextField;
	var SaveLoadListHolder: MovieClip;
	var SaveLoadPanel_mc: MovieClip;
	var GamerIcon_mc: MovieClip;
	var GamerTagWidget_mc: MovieClip;
	var GamerTag_mc: MovieClip;
	var VersionText: TextField;
	var fadeOutParams: Array;
	var iLoadDLCContentMessageTimerID: Number;
	var iLoadDLCListTimerID: Number;
	var iPlatform: Number;
	var strCurrentState: String;
	var strFadeOutCallback: String;
	var hasContinueButton: Boolean;
   
   function StartMenu()
   {
      super();
      hasContinueButton = false;
      MainList = MainListHolder.List_mc;
      SaveLoadListHolder = SaveLoadPanel_mc;
      DLCList_mc = DLCPanel.DLCList;
      DeleteSaveButton = DeleteButton;
      ChangeUserButton = ChangeUserButton;
      MarketplaceButton = DLCPanel.MarketplaceButton;
      MarketplaceButton._visible = false;
      CharacterSelectionHint = SaveLoadListHolder.CharacterSelectionHint_mc;
      ShowCharacterSelectionHint(false);
   }
   
   function InitExtensions()
   {
      Shared.GlobalFunc.SetLockFunction();
      _parent.Lock("BR");
      Logo_mc.Lock("BL");
      Logo_mc._y = Logo_mc._y - 80;
      GamerTagWidget_mc.Lock("TL");
      GamerTag_mc = GamerTagWidget_mc.GamerTag_mc;
      GamerIcon_mc = GamerTagWidget_mc.GamerIcon_mc;
      //GamerIconSize = GamerIcon_mc._width;
     // GamerIconLoader = new MovieClipLoader();
     // GamerIconLoader.addListener(this);
      GameDelegate.addCallBack("sendMenuProperties",this, "setupMainMenu");
      GameDelegate.addCallBack("ConfirmNewGame",this, "ShowConfirmScreen");
      GameDelegate.addCallBack("ConfirmContinue",this, "ShowConfirmScreen");
      GameDelegate.addCallBack("FadeOutMenu",this, "DoFadeOutMenu");
      GameDelegate.addCallBack("FadeInMenu",this, "DoFadeInMenu");
      GameDelegate.addCallBack("onProfileChange",this, "onProfileChange");
      GameDelegate.addCallBack("StartLoadingDLC",this, "StartLoadingDLC");
      GameDelegate.addCallBack("DoneLoadingDLC",this, "DoneLoadingDLC");
      GameDelegate.addCallBack("ShowGamerTagAndIcon",this,"ShowGamerTagAndIcon");
      GameDelegate.addCallBack("OnDeleteSaveUISanityCheck",this, "OnDeleteSaveUISanityCheck");
      GameDelegate.addCallBack("OnSaveDataEventLoadSUCCESS",this,"OnSaveDataEventLoadSUCCESS");
      GameDelegate.addCallBack("OnSaveDataEventLoadCANCEL",this,"OnSaveDataEventLoadCANCEL");
      GameDelegate.addCallBack("onStartButtonProcessFinished",this,"onStartButtonProcessFinished");
      MainList.addEventListener("itemPress",this,"onMainButtonPress");
      MainList.addEventListener("listPress",this,"onMainListPress");
      MainList.addEventListener("listMovedUp",this,"onMainListMoveUp");
      MainList.addEventListener("listMovedDown",this,"onMainListMoveDown");
      MainList.addEventListener("selectionChange",this,"onMainListMouseSelectionChange");
      ButtonRect.handleInput = function()
      {
         return false;
      };
      ButtonRect.AcceptMouseButton.addEventListener("click",this,"onAcceptMousePress");
      ButtonRect.CancelMouseButton.addEventListener("click",this,"onCancelMousePress");
      ButtonRect.AcceptMouseButton.SetPlatform(0,false);
      ButtonRect.CancelMouseButton.SetPlatform(0,false);
      SaveLoadListHolder.addEventListener("loadGameSelected",this,"ConfirmLoadGame");
      SaveLoadListHolder.addEventListener("saveListPopulated",this,"OnSaveListOpenSuccess");
      SaveLoadListHolder.addEventListener("saveListCharactersPopulated",this,"OnsaveListCharactersOpenSuccess");
      SaveLoadListHolder.addEventListener("saveListOnBatchAdded",this,"OnSaveListBatchAdded");
      SaveLoadListHolder.addEventListener("OnCharacterSelected",this,"OnCharacterSelected");
      SaveLoadListHolder.addEventListener("saveHighlighted",this,"onSaveHighlight");
      SaveLoadListHolder.addEventListener("OnSaveLoadPanelBackClicked",(this,OnSaveLoadPanelBackClicked));
      SaveLoadListHolder.List_mc.addEventListener("listPress",this,"onSaveLoadListPress");
      DeleteSaveButton._alpha = StartMenu.ALPHA_AVAILABLE;
      DeleteMouseButton._alpha = StartMenu.ALPHA_AVAILABLE;
      MarketplaceButton._alpha = StartMenu.ALPHA_DISABLED;
      DeleteSaveButton._x = - DeleteSaveButton.textField.textWidth - StartMenu.LOADING_ICON_OFFSET;
      DeleteMouseButton._x = DeleteSaveButton._x;
      ChangeUserButton._x = - ChangeUserButton.textField.textWidth - StartMenu.LOADING_ICON_OFFSET;
      DLCList_mc._visible = false;
      CharacterSelectionHint.addEventListener("OnMousePressCharacterChange",(this,OnMousePressCharacterChange));
   }
   
   function setupMainMenu(): Void
   {
      var _loc8_ = 0;
      var _loc5_ = 1;
      var _loc6_ = 2;
      var _loc10_ = 3;
      var _loc11_ = 4;
      var _loc9_ = 5;
      var _loc7_ = 6;
      var _loc4_ = StartMenu.NEW_INDEX;
      if(MainList.entryList.length > 0) {
         _loc4_ = MainList.centeredEntry.index;
      }
      MainList.ClearList();
      if(arguments[_loc5_])
      {
         hasContinueButton = true;
         MainList.entryList.push({text: "$CONTINUE",index:StartMenu.CONTINUE_INDEX,disabled:false});
         if(_loc4_ == StartMenu.NEW_INDEX) {
            _loc4_ = StartMenu.CONTINUE_INDEX;
         }
      }
      MainList.entryList.push({text: "$NEW",index:StartMenu.NEW_INDEX,disabled:false});
      MainList.entryList.push({text: "$LOAD",disabled: !arguments[_loc5_],index:StartMenu.LOAD_INDEX});
      if(arguments[_loc11_] == true)
      {
         MainList.entryList.push({text: "$DOWNLOADABLE CONTENT" ,index:StartMenu.DLC_INDEX,disabled:false});
      }
      if(arguments[_loc7_])
      {
         MainList.entryList.push({text:"$MOD MANAGER",disabled:false,index:StartMenu.MOD_INDEX});
      }
      MainList.entryList.push({text:"$CREDITS",index:StartMenu.CREDITS_INDEX,disabled:false});
      if(arguments[_loc8_])
      {
         MainList.entryList.push({text:"$QUIT",index:StartMenu.QUIT_INDEX,disabled:false});
      }
      if(arguments[_loc9_])
      {
         MainList.entryList().push({text:"$HELP",index:StartMenu.HELP_INDEX,disabled:false});
      }
      var _loc3_ = 0;
      while(_loc3_ < MainList.entryList().length)
      {
         if(MainList.entryList()[_loc3_].index == _loc4_)
         {
            MainList.RestoreScrollPosition(_loc3_,false);
         }
         _loc3_ = _loc3_ + 1;
      }
      MainList.InvalidateData();
      if(currentState == undefined) {
         if(arguments[_loc10_])
         {
            StartState(StartMenu.PRESS_START_STATE);						 
         } else {
            StartState(StartMenu.MAIN_STATE);
         }
      }
      else if(currentState() == StartMenu.SAVE_LOAD_STATE || currentState() == StartMenu.SAVE_LOAD_CONFIRM_STATE || currentState() == StartMenu.DELETE_SAVE_CONFIRM_STATE)
      {
         StartState(StartMenu.MAIN_STATE);
      }
      if(arguments[_loc6_] != undefined)
      {
         VersionText.SetText("v " + arguments[_loc6_]);
      }
      else
      {
         VersionText.SetText(" ");
      }
   }
   function ShowGamerTagAndIcon(strGamerTag: String): Void
   {
      if(strGamerTag.length > 0)
      {
         //Shared.GlobalFunc.MaintainTextFormat();
         //GamerTag_mc.GamerTagText_tf.text = strGamerTag;
         //GamerTag_mc.visible = true;
         //GamerIconRect = GamerIcon_mc.createEmptyMovieClip("GamerIconRect",getNextHighestDepth());
         //GamerIconLoader.loadClip("img://BGSUserIcon",GamerIconRect);
      }
      else
      {
         //GamerTag_mc.visible = false;
         //GamerIcon_mc.visible = false;
      }
   }
   function onLoadInit(aTargetClip)
   {
      //aTargetClip._width = GamerIconSize;
      //aTargetClip._height = GamerIconSize;
   }
   function OnDeleteSaveUISanityCheck(aHasRecentSave: Boolean, aCanLoadGame: Boolean): Void
   {
      var _loc7_ = false;
      if(hasContinueButton)
      {
         if(!aHasRecentSave)
         {
            if(MainList.entryList[0].index == StartMenu.CONTINUE_INDEX)
            {
               MainList.entryList.shift();
            }
            MainList.RestoreScrollPosition(1,true);
            _loc7_ = true;
         }
      }
      if(!aCanLoadGame)
      {
         var _loc2_ = 0;
         while(_loc2_ < MainList.maxEntries())
         {
            if(MainList.entryList[_loc2_].index == StartMenu.LOAD_INDEX)
            {
               MainList.entryList.splice(_loc2_,1,{text:"$LOAD",disabled:true,index:StartMenu.LOAD_INDEX,textColor:6316128});
               _loc7_ = true;
               MainList.RestoreScrollPosition(0,false);
               break;
            }
            _loc2_ = _loc2_ + 1;
         }
      }
      if(_loc7_)
      {
         MainList.InvalidateData();
      }
   }
   function ShowCharacterSelectionHint(abFlag: Boolean): Void
   {
      if(iPlatform == StartMenu.PLATFORM_ORBIS)
      {
         CharacterSelectionHint._visible = false;
      }
      else
      {
         CharacterSelectionHint._visible = abFlag;
      }
   }
   function OnSaveDataEventLoadSUCCESS(): Void
   {
      ShowCharacterSelectionHint(false);
      if(iPlatform == StartMenu.PLATFORM_ORBIS)
      {
         onCancelPress();
      }
   }
   function OnSaveDataEventLoadCANCEL(): Void
   {
      if(iPlatform == StartMenu.PLATFORM_ORBIS)
      {
         RequestCharacterListLoad();
      }
   }
   function get currentState(): String
   {
      return strCurrentState;
   }
   function set currentState(strNewState): Void
   {
      if(strNewState == StartMenu.MAIN_STATE) {
         MainList.disableSelection = false;
      }
      if(strNewState != strCurrentState)
      {
         ShouldProcessInputs(true);
      }
      if(iPlatform == StartMenu.PLATFORM_ORBIS)
      {
         ShowDeleteButtonHelp(strNewState == StartMenu.CHARACTER_SELECTION_STATE);
      }
      else
      {
         ShowDeleteButtonHelp(strNewState == StartMenu.SAVE_LOAD_STATE);
      }
      ShowChangeUserButtonHelp(strNewState == StartMenu.MAIN_STATE);
      ShowCharacterSelectionHint(strNewState == StartMenu.SAVE_LOAD_STATE);
      SaveLoadListHolder.ShowSelectionButtons(strNewState == StartMenu.SAVE_LOAD_STATE || strNewState == StartMenu.CHARACTER_SELECTION_STATE);
      strCurrentState = strNewState;
      ChangeStateFocus(strNewState);
      //return currentState();
   }
   function get ShouldProcessInputs(): Boolean
   {
      return ShouldProcessInputs;
   }
   function set ShouldProcessInputs(abFlag: Boolean): Void
   {
      ShouldProcessInputs = abFlag;
      //return ShouldProcessInputs();
   }
   function handleInput(details: gfx.ui.InputDetails, pathToFocus: Array): Boolean
   {
      if(currentState() == StartMenu.PRESS_START_STATE && iPlatform == StartMenu.PLATFORM_ORBIS)
      {
         if(Shared.GlobalFunc.IsKeyPressed(details))
         {
            GameDelegate.call("EndPressStartState",[]);
         }
      }
      else if(pathToFocus.length > 0 && !pathToFocus[0].handleInput(details,pathToFocus.slice(1)))
      {
         if(Shared.GlobalFunc.IsKeyPressed(details) && ShouldProcessInputs())
         {
            if(details.navEquivalent == gfx.ui.NavigationCode.ENTER)
            {
               onAcceptPress();
            }
            else if(details.navEquivalent == gfx.ui.NavigationCode.TAB)
            {
               onCancelPress();
            }
            else if((details.navEquivalent == gfx.ui.NavigationCode.GAMEPAD_X || details.code == 88) && DeleteSaveButton._visible && DeleteSaveButton._alpha == StartMenu.ALPHA_AVAILABLE)
            {
               if(iPlatform == StartMenu.PLATFORM_ORBIS)
               {
                  var _loc5_ = SaveLoadListHolder.selectedEntry();
                  if(_loc5_ != undefined)
                  {
                     var _loc4_ = _loc5_.flags;
                     if(_loc4_ == undefined)
                     {
                        _loc4_ = 0;
                     }
                     var _loc3_ = _loc5_.id;
                     if(_loc3_ == undefined)
                     {
                        _loc3_ = 4294967295;
                     }
                  }
                  GameDelegate.call("ORBISDeleteSave",[_loc3_,_loc4_]);
               }
               else
               {
                  ConfirmDeleteSave();
               }
            }
            else if((details.navEquivalent == gfx.ui.NavigationCode.GAMEPAD_Y || details.code == 84) && strCurrentState == StartMenu.SAVE_LOAD_STATE && !SaveLoadListHolder.isSaving())
            {
               GameDelegate.call("PlaySound",["UIMenuCancel"]);
               EndState();
            }
            else if(details.navEquivalent == gfx.ui.NavigationCode.GAMEPAD_Y && currentState() == StartMenu.DLC_STATE && MarketplaceButton._visible && MarketplaceButton._alpha == StartMenu.ALPHA_AVAILABLE)
            {
               SaveLoadConfirmText.textField.SetText("$Open Xbox LIVE Marketplace?");
               SetPlatform(iPlatform);
               StartState(StartMenu.MARKETPLACE_CONFIRM_STATE);
            }
            else if(details.navEquivalent == gfx.ui.NavigationCode.GAMEPAD_Y && currentState() == StartMenu.MAIN_STATE && ChangeUserButton._visible)
            {
               GameDelegate.call("ChangeUser",[]);
            }
         }
      }
      return true;
   }
   function onMouseButtonDeleteSaveClick(): Void
   
   {
      if(DeleteSaveButton._alpha == StartMenu.ALPHA_AVAILABLE)
      {
         ConfirmDeleteSave();
      }
   }
   function onMouseButtonDeleteRollOver(): Void
   {
      GameDelegate.call("PlaySound",["UIMenuFocus"]);
   }
   function onStartButtonProcessFinished(): Void
   {
      EndState(StartMenu.PRESS_START_STATE);
   }
   function onAcceptPress(): Void
   {
      switch(strCurrentState) {
         case StartMenu.MAIN_CONFIRM_STATE:
            if(MainList.selectedEntry.index == StartMenu.NEW_INDEX) {
               GameDelegate.call("PlaySound",["UIStartNewGame"]);
               FadeOutAndCall("StartNewGame");
           } else if(MainList.selectedEntry.index == StartMenu.CONTINUE_INDEX) {
               GameDelegate.call("PlaySound",["UIMenuOK"]);
               FadeOutAndCall("ContinueLastSavedGame");
            } else if(MainList.selectedEntry.index == StartMenu.QUIT_INDEX) {
               GameDelegate.call("PlaySound", ["UIMenuOK"]);
               GameDelegate.call("QuitToDesktop", []);
            }
            break;
         case StartMenu.CHARACTER_SELECTION_STATE:
            GameDelegate.call("PlaySound",["UIMenuOK"]);
            break;
         case StartMenu.SAVE_LOAD_CONFIRM_STATE:
            GameDelegate.call("PlaySound",["UIMenuOK"]);
            FadeOutAndCall("LoadGame", [SaveLoadListHolder.selectedIndex]);
            break;
         case StartMenu.DELETE_SAVE_CONFIRM_STATE:
            SaveLoadListHolder.DeleteSelectedSave();
            if(SaveLoadListHolder.numSaves == 0) {
               GameDelegate.call("DoDeleteSaveUISanityCheck",[]);
               StartState(StartMenu.MAIN_STATE);
            }
            else {
               EndState();
            }
            break;
			
         case StartMenu.MARKETPLACE_CONFIRM_STATE:
            GameDelegate.call("PlaySound",["UIMenuOK"]);
            GameDelegate.call("OpenMarketplace",[]);
            StartState(StartMenu.MAIN_STATE);
      }
   }
   
   function isConfirming(): Boolean
   {
      return strCurrentState == StartMenu.SAVE_LOAD_CONFIRM_STATE || strCurrentState == StartMenu.DELETE_SAVE_CONFIRM_STATE || strCurrentState == StartMenu.MARKETPLACE_CONFIRM_STATE || strCurrentState == StartMenu.MAIN_CONFIRM_STATE;
   }
   
   function onAcceptMousePress(): Void
   {
      if(isConfirming()) {
         onAcceptPress();
      }
   }
   
   function OnMousePressCharacterChange(): Void
   {
      GameDelegate.call("PlaySound",["UIMenuCancel"]);
      EndState();
   }
   
   function onCancelMousePress(): Void
   {
      if(isConfirming()) {
         onCancelPress();
      }
   }
   
   function onCancelPress(): Void 
   {
      switch(strCurrentState) {
		case StartMenu.MAIN_CONFIRM_STATE:							 
         case StartMenu.SAVE_LOAD_STATE:
            currentState(StartMenu.CHARACTER_SELECTION_STATE);
            EndState();
            SaveLoadListHolder.ForceStopLoading();
            break;
         case StartMenu.CHARACTER_SELECTION_STATE:
         case StartMenu.SAVE_LOAD_CONFIRM_STATE:
         case StartMenu.DELETE_SAVE_CONFIRM_STATE:
         case StartMenu.DLC_STATE:
         case StartMenu.MARKETPLACE_CONFIRM_STATE:
            GameDelegate.call("PlaySound",["UIMenuCancel"]);
            EndState();
      }
   }
   
   function onMainButtonPress(event: Object): Void
   {
      if(strCurrentState == StartMenu.MAIN_STATE || iPlatform == 0) {
         switch(event.entry.index) {
            case StartMenu.CONTINUE_INDEX:
               GameDelegate.call("CONTINUE",[]);
               GameDelegate.call("PlaySound",["UIMenuOK"]);
               break;
			   
            case StartMenu.NEW_INDEX:
               GameDelegate.call("NEW",[]);
               GameDelegate.call("PlaySound",["UIMenuOK"]);
               break;
			   
            case StartMenu.QUIT_INDEX:
               ShowConfirmScreen("$Quit to desktop?  Any unsaved progress will be lost.");
               GameDelegate.call("PlaySound",["UIMenuOK"]);
               break;
			   
            case StartMenu.LOAD_INDEX:
               if(!event.entry.disabled) {
                  SaveLoadListHolder.isSaving(false);
                  RequestCharacterListLoad();
				  
              } else {
                  GameDelegate.call("OnDisabledLoadPress", []);
               }
               break;
			   
            case StartMenu.DLC_INDEX:
               StartState(StartMenu.DLC_STATE);
               break;
			   
            case StartMenu.CREDITS_INDEX:
               FadeOutAndCall("OpenCreditsMenu");
               break;
            case StartMenu.HELP_INDEX:
               GameDelegate.call("HELP",[]);
               GameDelegate.call("PlaySound", ["UIMenuOK"]);
               break;
            case StartMenu.MOD_INDEX:
               GameDelegate.call("MOD",[]);
               GameDelegate.call("PlaySound",["UIMenuOK"]);
               break;
            default:
               GameDelegate.call("PlaySound",["UIMenuCancel"]);
         }
      }
   }
   
   function RequestCharacterListLoad(): Void
   {
      GameDelegate.call("PopulateCharacterList",[SaveLoadListHolder.List_mc.entryList,SaveLoadListHolder.batchSize()]);
      StartState(StartMenu.CHARACTER_LOAD_STATE);
   }
   
   function onMainListPress(event: Object): Void
   {
      onCancelPress();
   }
   
   function onPCQuitButtonPress(event: Object): Void
   {
      if(event.index == 0) {
         GameDelegate.call("QuitToMainMenu",[]);
      } else if(event.index == 1) {
         GameDelegate.call("QuitToDesktop",[]);
      }
   }
   
   function onSaveLoadListPress(): Void
   {
      onAcceptPress();
   }
   
   function onMainListMoveUp(event: Object): Void
   {
      GameDelegate.call("PlaySound",["UIMenuFocus"]);
      if(event.scrollChanged == true) {
         MainList._parent.gotoAndPlay("moveUp");
      }
   }
   
   function onMainListMoveDown(event: Object): Void
   {
      GameDelegate.call("PlaySound",["UIMenuFocus"]);
      if(event.scrollChanged == true) {
         MainList._parent.gotoAndPlay("moveDown");
      }
   }
   
   function onMainListMouseSelectionChange(event: Object): Void
   {
      if(event.keyboardOrMouse == 0 && event.index != -1) {
         GameDelegate.call("PlaySound",["UIMenuFocus"]);
      }
   }
   
   function SetPlatform(aiPlatform: Number, abPS3Switch: Boolean): Void
   {
      ButtonRect.AcceptGamepadButton._visible = aiPlatform != 0;
      ButtonRect.CancelGamepadButton._visible = aiPlatform != 0;
      ButtonRect.AcceptMouseButton._visible = aiPlatform == 0;
      ButtonRect.CancelMouseButton._visible = aiPlatform == 0;
      var _loc4_ = DeleteSaveButton._visible;
      if(aiPlatform == StartMenu.PLATFORM_PC_KBMOUSE)
      {
         DeleteSaveButton._visible = false;
         DeleteMouseButton.label = DeleteSaveButton.label;
         DeleteMouseButton._x = DeleteButton._x;
         DeleteMouseButton.trackAsMenu = true;
         DeleteSaveButton = DeleteMouseButton;
         DeleteSaveButton.onPress = (this,onMouseButtonDeleteSaveClick);
         DeleteSaveButton.addEventListener("rollOver",(this,onMouseButtonDeleteRollOver));
      }
      else if(aiPlatform == StartMenu.PLATFORM_PC_GAMEPAD && DeleteSaveButton == DeleteMouseButton)
      {
         DeleteSaveButton._visible = false;
         DeleteSaveButton = DeleteButton;
         DeleteSaveButton.onPress = undefined;
         DeleteMouseButton.removeEventListeners("rollOver",(this,onMouseButtonDeleteRollOver));
      }
      else
      {
         DeleteMouseButton._visible = false;
      }
      ShowDeleteButtonHelp(_loc4_);
      DeleteSaveButton.SetPlatform(aiPlatform,abPS3Switch);
      ChangeUserButton.SetPlatform(aiPlatform,abPS3Switch);
      MarketplaceButton.SetPlatform(aiPlatform,abPS3Switch);
      MainListHolder.SelectionArrow._visible = aiPlatform != 0;
      if(aiPlatform != 0)
      {
         ButtonRect.AcceptGamepadButton.SetPlatform(aiPlatform,abPS3Switch);
         ButtonRect.CancelGamepadButton.SetPlatform(aiPlatform,abPS3Switch);
      }
      CharacterSelectionHint.SetPlatform(aiPlatform);
      MarketplaceButton._visible = false;
      if(iPlatform == undefined)
      {
         DLCPanel.warningText.SetText("$Loading downloadable content..." + (iPlatform != StartMenu.PLATFORM_ORBIS?"":"_PS3"));
         LoadingContentMessage.Message_mc.textField.SetText("$Loading extra content." + (iPlatform != StartMenu.PLATFORM_ORBIS?"":"_PS3"));
      }
      iPlatform = aiPlatform;
      SaveLoadListHolder.platform(iPlatform);
      MainList.SetPlatform(aiPlatform,abPS3Switch);
   }
   
   function DoFadeOutMenu(): Void
   {
      FadeOutAndCall();
   }
   
   function DoFadeInMenu(): Void
   {
      _parent.gotoAndPlay("fadeIn");
      EndState();
   }
   
   function FadeOutAndCall(strCallback: String, paramList: Array): Void
   {
      strFadeOutCallback = strCallback;
      fadeOutParams = paramList;
      _parent.gotoAndPlay("fadeOut");
      GameDelegate.call("fadeOutStarted",[]);
   }
   
   function onFadeOutCompletion(): Void
   {
      if(strFadeOutCallback != undefined && strFadeOutCallback.length > 0) {
         if(fadeOutParams != undefined) {
            GameDelegate.call(strFadeOutCallback,fadeOutParams);
         }
         else
         {
            GameDelegate.call(strFadeOutCallback,[]);
         }
      }
   }
   function StartState(strStateName : String): Void
   {
      ShouldProcessInputs(false);
      if(strStateName == StartMenu.CHARACTER_SELECTION_STATE)
      {
         SaveLoadListHolder.isShowingCharacterList(true);
      }
      else if(strStateName == StartMenu.SAVE_LOAD_STATE)
      {
         SaveLoadListHolder.isShowingCharacterList(false);
      }
      else if(strStateName == StartMenu.DLC_STATE)
      {
         ShowMarketplaceButtonHelp(false);
      }
      if(strCurrentState == StartMenu.MAIN_STATE)
      {
         MainList.disableSelection(true);
      }
      ShowDeleteButtonHelp(false);
      ShowChangeUserButtonHelp(false);
      SaveLoadListHolder.ShowSelectionButtons(false);
      strCurrentState = strStateName + StartMenu.START_ANIM_STR;
      gotoAndPlay(strCurrentState);
      gfx.managers.FocusHandler.instance().setFocus(this,0);
   }
   function EndState()
   {
      if(strCurrentState == StartMenu.DLC_STATE)
      {
         ShowMarketplaceButtonHelp(false);
      }
      if(strCurrentState != StartMenu.MAIN_STATE)
      {
         strCurrentState = strCurrentState + StartMenu.END_ANIM_STR;
         gotoAndPlay(strCurrentState);
      }
      if(strCurrentState == StartMenu.SAVE_LOAD_CONFIRM_STATE || strCurrentState == StartMenu.DELETE_SAVE_CONFIRM_STATE)
      {
         SaveLoadListHolder.ShowSelectionButtons(true);
      }
   }
   
   function ChangeStateFocus(strNewState: String): Void
   {
      switch(strNewState) {
         case StartMenu.MAIN_STATE:
            FocusHandler.instance.setFocus(MainList,0);
            break;
			
         case StartMenu.CHARACTER_SELECTION_STATE:
         case StartMenu.SAVE_LOAD_STATE:
            FocusHandler.instance.setFocus(SaveLoadListHolder.List_mc,0);
            SaveLoadListHolder.List_mc.disableSelection = false;
            break;
			
         case StartMenu.DLC_STATE:
            iLoadDLCListTimerID = setInterval(this,"DoLoadDLCList",500);
            FocusHandler.instance.setFocus(DLCList_mc,0);
            break;
			
         case StartMenu.MAIN_CONFIRM_STATE:
         case StartMenu.SAVE_LOAD_CONFIRM_STATE:
         case StartMenu.DELETE_SAVE_CONFIRM_STATE:
         case StartMenu.PRESS_START_STATE:
         case StartMenu.MARKETPLACE_CONFIRM_STATE:
            FocusHandler.instance.setFocus(ButtonRect,0);
      }
   }
   
   function ShowConfirmScreen(astrConfirmText: String): Void
   {
      ConfirmPanel_mc.textField.SetText(astrConfirmText);
      SetPlatform(iPlatform);
      StartState(StartMenu.MAIN_CONFIRM_STATE);
   }
   
   function OnSaveListOpenSuccess(): Void
   {
      if(SaveLoadListHolder.numSaves() > 0 && strCurrentState.indexOf(StartMenu.SAVE_LOAD_STATE) == -1) {
         GameDelegate.call("PlaySound", ["UIMenuOK"]);
         StartState(StartMenu.SAVE_LOAD_STATE);
      }
      else
      {
         GameDelegate.call("PlaySound", ["UIMenuCancel"]);
      }
   }
   function OnsaveListCharactersOpenSuccess(): Void
   {
      if(strCurrentState == StartMenu.CHARACTER_LOAD_STATE || strCurrentState == "CharacterLoadStartAnim")
      {
         SaveLoadListHolder.isShowingCharacterList(true);
         ShowCharacterSelectionHint(false);
         GameDelegate.call("PlaySound", ["UIMenuOK"]);
         StartState(StartMenu.CHARACTER_SELECTION_STATE);
      }
      else
      {
         GameDelegate.call("PlaySound",["UIMenuCancel"]);
      }
   }
 
   function OnSaveListBatchAdded(): Void
   {
      if(SaveLoadListHolder.numSaves() > 0 && strCurrentState == StartMenu.SAVE_LOAD_STATE)
      {
         ShowCharacterSelectionHint(true);
      }
   }
   function OnCharacterSelected(): Void
   {
      if(iPlatform != StartMenu.PLATFORM_ORBIS)
      {
         StartState(StartMenu.SAVE_LOAD_STATE);
      }
   }
   function onSaveHighlight(event: Object): Void
   {
      DeleteSaveButton._alpha = event.index != -1?StartMenu.ALPHA_AVAILABLE:StartMenu.ALPHA_DISABLED;
      if(iPlatform == 0)
      {
         GameDelegate.call("PlaySound",["UIMenuFocus"]);
      }
   }
   
   function ConfirmLoadGame(event: Object): Void
   {
      SaveLoadListHolder.List_mc.disableSelection = true;
      SaveLoadConfirmText.textField.SetText("$Load this game?");
      SetPlatform(iPlatform);
      StartState(StartMenu.SAVE_LOAD_CONFIRM_STATE);
   }
   
   function ConfirmDeleteSave(): Void
   {
      SaveLoadListHolder.List_mc.disableSelection = true;
      SaveLoadConfirmText.textField.SetText("$Delete this save?");
      SetPlatform(iPlatform);
      StartState(StartMenu.DELETE_SAVE_CONFIRM_STATE);
   }
   
   function ShowDeleteButtonHelp(abFlag: Boolean): Void
   {
      DeleteSaveButton.disabled(!abFlag);
      DeleteSaveButton._visible = abFlag;
      VersionText._visible = !abFlag;
   }
   function ShowChangeUserButtonHelp(abFlag: Boolean): Void
   {
      if(iPlatform == StartMenu.PLATFORM_DURANGO)
      {
         ChangeUserButton.disabled(!abFlag);
         ChangeUserButton._visible = abFlag;
         VersionText._visible = !abFlag;
      }
      else
      {
         ChangeUserButton.disabled(true);
         ChangeUserButton._visible = false;
      }
   }
   function ShowMarketplaceButtonHelp(abFlag: Boolean): Void
   {
      if(iPlatform == StartMenu.PLATFORM_DURANGO)
      {
         MarketplaceButton._visible = abFlag;
         VersionText._visible = !abFlag;
      }
      else
      {
         MarketplaceButton._visible = false;
      }
   }
   function ShowPressStartState(): Void
   {
      if(strCurrentState != StartMenu.PRESS_START_STATE)
      {
         StartState(StartMenu.PRESS_START_STATE);
      }
   }
   function StartLoadingDLC(): Void
   {
      LoadingContentMessage.gotoAndPlay("startFadeIn");
      clearInterval(iLoadDLCContentMessageTimerID);
      iLoadDLCContentMessageTimerID = setInterval(this,"onLoadingDLCMessageFadeCompletion",1000);
   }
   
   function onLoadingDLCMessageFadeCompletion(): Void
   {
      clearInterval(iLoadDLCContentMessageTimerID);
      GameDelegate.call("DoLoadDLCPlugins",[]);
   }
   
   function DoneLoadingDLC(): Void
   {
      LoadingContentMessage.gotoAndPlay("startFadeOut");
   }
   
   function DoLoadDLCList(): Void
   {
      clearInterval(iLoadDLCListTimerID);
      DLCList_mc.entryList.splice(0,DLCList_mc.entryList.length);
      GameDelegate.call("LoadDLC", [DLCList_mc.entryList],this, "UpdateDLCPanel");
   }
   
   function UpdateDLCPanel(abMarketplaceAvail: Boolean, abNewDLCAvail: Boolean): Void
   {
      if(DLCList_mc.entryList.length > 0) {
         DLCList_mc._visible = true;
         DLCPanel.warningText.SetText(" ");
         if(iPlatform != 0) {
            DLCList_mc.selectedIndex = 0;
         }
         DLCList_mc.InvalidateData();
      } else {
         DLCList_mc._visible = false;
         DLCPanel.warningText.SetText("$No content downloaded" + (iPlatform != StartMenu.PLATFORM_ORBIS?"":"_PS3"));
      }
      MarketplaceButton._visible = false;
      if(abNewDLCAvail == true)
      {
         DLCPanel.NewContentAvail.SetText("$New content available");
      }
   }
   function OnSaveLoadPanelSelectClicked(): Void
   {
      onAcceptPress();
   }
   function OnSaveLoadPanelBackClicked(): Void
   {
      onCancelPress();
   }
}
