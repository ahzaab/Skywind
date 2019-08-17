import gfx.managers.FocusHandler;
import gfx.ui.NavigationCode;
import Shared.GlobalFunc;
import gfx.controls.TextInput;

import skyui.util.DialogManager;
import skyui.util.GlobalFunctions;
import skyui.util.Translator;
import skyui.defines.Input;


class TextInputDialog extends OptionDialog
{
  /* PRIVATE VARIABLES */

  /* STAGE ELEMENTS */
  
	public var textInput: TextInput;

	
  /* PROPERTIES */
	
	public var initialText: String;

	
  /* INITIALIZATION */
  
	public function TextInputDialog()
	{
		super();
	}
	

   /* PUBLIC FUNCTIONS */
   
	// @override OptionDialog
	public function initButtons(): Void
	{
		var acceptControls: Object;
		var cancelControls: Object;
		
		if (platform == 0) {
			acceptControls = Input.Enter;
			cancelControls = Input.Tab;
		} else {
			acceptControls = Input.Accept;
			cancelControls = Input.Cancel;
		}
		
		rightButtonPanel.clearButtons();
		var cancelButton = rightButtonPanel.addButton({text: "$Cancel", controls: cancelControls});
		cancelButton.addEventListener("press", this, "onCancelPress");
		var acceptButton = rightButtonPanel.addButton({text: "$Accept", controls: acceptControls});
		acceptButton.addEventListener("press", this, "onAcceptPress");
		rightButtonPanel.updateButtons();
	}

	// @override OptionDialog
	public function initContent(): Void
	{
		FocusHandler.instance.setFocus(textInput.textField, 0);
		textInput.focused = true;
		Selection.setFocus(textInput.textField);
		
		textInput.maxChars = 30;
		textInput.text = Translator.translateNested(initialText);
		
		Selection.setSelection(0,99);
		
		skse.AllowTextInput(true);
	}
	
	// @GFx
	public function handleInput(details, pathToFocus): Boolean
	{
		var nextClip = pathToFocus.shift();
		if (nextClip.handleInput(details, pathToFocus))
			return true;
		
		if (GlobalFunc.IsKeyPressed(details, false)) {
			if (details.navEquivalent == NavigationCode.TAB) {
				onCancelPress();
				return true;
			} else if (details.navEquivalent == NavigationCode.ENTER) {
				onAcceptPress();
				return true;
			}
		}
		
		// Don't forward to higher level
		return true;
	}
	
	
  /* PRIVATE FUNCTIONS */
	
	private function onAcceptPress(): Void
	{
		skse.AllowTextInput(false);
		skse.SendModEvent("SKICP_inputAccepted", textInput.text, 0);
		DialogManager.close();
	}

	private function onCancelPress(): Void
	{
		skse.AllowTextInput(false);
		skse.SendModEvent("SKICP_dialogCanceled");
		DialogManager.close();
	}
}