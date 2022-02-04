'******************************************************************************
'
' File: GetServerInfo.vbs
'
' Description:
'
'   This Callable VEE VBScript example demonstrates the following concepts:
'
'   1. Determining which environment is hosting the Callable VEE Server:
'      VEE Pro Development (vee.exe) or VEE Pro RunTime (veerun.exe).
'   2. Determining the location of the I/O configuration file used by the 
'      server.
'   3. Determine the version of VEE that is hosting the server.
'   4. Determine the install directory used by Callable VEE Server.
'
' Execution Instructions:
'
'   From the command line, type the following (or double-click this file in
'   the Windows Explorer)
'
'   cscript GetServerInfo.vbs [hostname]
'
' This example is provided as an illustration "as is", and
' Keysight Technologies makes no warranty of any kind with regard to this
' example.
'
' Copyright (c) 2000-2018 Keysight Technologies.
'
'******************************************************************************
Option Explicit

Dim callServer
Dim scriptArgs
Dim hostname

Set scriptArgs = WScript.Arguments
If scriptArgs.Count > 1 Then
    Call Usage()
ElseIf scriptArgs.Count = 1 Then
    hostname = scriptArgs(0)
End If

If IsEmpty(hostname) Then
	Set callServer = CreateObject("CallableVEE.CallServer")
Else
	Set callServer = CreateObject("CallableVEE.CallServer", hostname)
End If

WScript.Echo("** Information on the registered Callable VEE Server **")
WScript.Echo("   Name:            " & callServer.Name)
WScript.Echo("   Version:         " & callServer.Version)
WScript.Echo("   Executable Path: " & callServer.Path)
WScript.Echo("   Install Dir:     " & callServer.InstallDir)
WScript.Echo("   I/O config path: " & callServer.IOConfigPath)



'******************************************************************************
'
' Utility subroutine that prints out usage text for this script.
'
Sub Usage()
    WScript.Echo ""
    WScript.Echo "Usage: GetServerInfo.vbs [hostname]"
    WScript.Quit
End Sub

