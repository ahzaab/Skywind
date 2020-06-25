Scriptname AHZ_SKWD_NotificationTest extends Quest 


Event OnInit()
	Maintenance() ; OnPlayerLoadGame will not fire the first time
EndEvent

Event OnKeyUp(Int KeyCode, Float HoldTime)
	
	If KeyCode == 26
		Debug.Notification("Left Bracket is registered and has been released after being held for " + HoldTime + " seconds")
		Skywind.SendNotification("Message from Left Bracket")
	EndIf

	If KeyCode == 27
		Debug.Notification("Right Bracket is registered and has been released after being held for " + HoldTime + " seconds")
		Skywind.SendNotification("Message from Right Bracket")
	EndIf
EndEvent 


Function Maintenance()
Debug.Notification("Skywind Test Script Started")
	RegisterForKey(26)  ;Left Bracket
	RegisterForKey(27) ;Right Bracket
EndFunction