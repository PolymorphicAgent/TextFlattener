; Handy for finding the nth char in a txt file

#Requires AutoHotkey v2.0

; Press F8 to start
F8::
{
    Loop 625
    {
        Send "{Right}"
	Sleep 1
    }
}