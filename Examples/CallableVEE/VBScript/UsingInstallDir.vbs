'******************************************************************************
'
' File: UsingInstallDir.vbs
'
' Description:
'
'   This Callable VEE VBScript example demonstrates the following concepts:
'
'   1. Loading library relative to the VEE install dir
'   2. Calling a VEE user function with 2 inputs and 2 outputs
'   3. Calling a VEE user function where an empty dynamic array is
'      passed for the output parameter of the Call method.
'
' Execution Instructions:
'
'   From the command line, type the following (or double-click this file in
'   the Windows Explorer)
'
'   cscript UsingInstallDir.vbs
'
' This example is provided as an illustration "as is", and
' Keysight Technologies makes no warranty of any kind with regard to this
' example.
'
' Copyright (c) 2000-2018 Keysight Technologies.
'
'******************************************************************************
Option Explicit

Dim srv
Dim lib
Dim uf
Dim libpath
Dim inputs(1)
Dim outputs()

libpath = "\examples\callablevee\lib.vee"

Set srv = CreateObject("CallableVEE.CallServer")
Set lib = srv.Libraries.Load(srv.InstallDir & libpath)
Set uf  = lib.UserFunctions("TwoInTwoOut")

inputs(0) = 1
inputs(1) = 5.5

Call uf.call(inputs, outputs)

WScript.Echo("A + B = " & CStr(outputs(0)))
WScript.Echo("The time according to VEE is " & CStr(outputs(1)))

