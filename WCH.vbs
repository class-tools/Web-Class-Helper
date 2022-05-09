'Web Class Helper Post Install Program Runtime Environment Configuration Module 2.0.0
'This source code file is under MIT License.
'Copyright (c) 2022 Class Tools Develop Team
'Contributors: jsh-jsh ren-yc
Set WshShell = WScript.CreateObject("WScript.Shell")
If WScript.Arguments.Length = 0 Then
    Set ObjShell = CreateObject("Shell.Application")
    ObjShell.ShellExecute "wscript.exe" _
    , """" & WScript.ScriptFullName & """ RunAsAdministrator", , "runas", 1
    WScript.Quit
End if
CurPath = left(WScript.ScriptFullName, instrrev(WScript.ScriptFullName, "\"))
Set WshShell = CreateObject("WScript.Shell")
WshShell.run "cmd.exe"
WScript.sleep 50
s = "cacls """ & CurPath
s1 = StrReverse(s)
s2 = mid(s1, 2)
s = StrReverse(s2)
If InStr(s, "(") <> 0 Then
    tmp = s & """ /T /G Everyone:F" & "{enter}"
    tmp1 = split(tmp, "(")
    tmp2 = split(tmp1(1), ")")
    WshShell.sendkeys tmp1(0)
    WshShell.sendkeys "{(}"
    WshShell.sendkeys tmp2(0)
    WshShell.sendkeys "{)}"
    WshShell.sendkeys tmp2(1)
Else
    WshShell.sendkeys s & """ /T /G Everyone:F" & "{enter}"
End If
WScript.sleep 50
WshShell.sendkeys "Y{enter}"
WScript.sleep 50
WshShell.sendkeys "%{F4}"