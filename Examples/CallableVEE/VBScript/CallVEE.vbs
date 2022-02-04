'******************************************************************************
'  
' File: CallVEE.vbs
'
' Description:
'
'   This example shows how to do in VBScript what the callVEE.c program
'   does in C using the "C to VEE" API.
'
' Execution Instructions:
'
'   From the command line, type the following (or double-click this file in
'	the Windows Explorer)
'
'	cscript CallVEE.vbs [hostname]
'
' This example is provided as an illustration "as is", and
' Keysight Technologies makes no warranty of any kind with regard to this
' example.
'
' Copyright (c) 2000-2018 Keysight Technologies.
'
'******************************************************************************
Option Explicit

Dim callServer, lib, scriptArgs
Dim szInstallDir, szLibFile
Dim hostname
Dim result
Dim delay
Dim pulseVDC
Dim errorsVDC
Dim upperLimitVDC, lowerLimitVDC
Dim upperLimit(10,1)
Dim lowerLimit(12,1)
Dim inputs()
Dim outputs()

Set scriptArgs = WScript.Arguments

If scriptArgs.Count > 1 Then
	Call Usage()
ElseIf scriptArgs.Count = 1 Then
	hostname = scriptArgs(0)
End If

Do
	' Create top-level object in CallableVEE Object model.  This starts the 
	' CallableVEE server
	If IsEmpty(hostname) Then
		Set callServer = CreateObject("CallableVEE.CallServer")
	Else
		Set callServer = CreateObject("CallableVEE.CallServer", hostname)
	End If

	' Ask user if they want to run CallableVEE in debug mode
	If (MsgBox("Debug Mode On?", vbYesNo + vbQuestion) = vbYes) Then
	    callServer.Debug = True
	End If

	' Get the installation directory of the CallableVEE server
	szInstallDir = callServer.InstallDir
	szLibFile = szInstallDir & "\examples\CallableVEE\lib.vee"

	' Have the server load a library of VEE UserFunctions
	Set lib = callServer.Libraries.Load(szLibFile)

	' Get the Reference delay from the user
	result = InputBox("Please enter a reference delay value." & _
	                  vbCrLf & vbCrLf & _
					  "Valid numbers are between .25 and .45 microseconds.", _
	                  "CallVEE", ".30")
	delay = CDbl(result) * 1E-6

	' Set up the input arguments to the "Synthesize_Pulse" UserFunction.
	' The UserFUnction has 1 input and 1 output
	ReDim inputs(0)
	ReDim outputs(0)
	inputs(0) = delay

	' Call the Synthesize_Pulse UserFunction to create
	Call lib.UserFunctions("Synthesize_Pulse").Call(inputs, outputs)

	' The first parameter to "DisplaY_Test" is the synthesized pulseVDC that was
	' returned from the userFunction call to "Synthesize_Pulse", we just made.
	Set pulseVDC = outputs(0)

	' The second parameter to "DisplaY_Test" is an array of 2D Coords that 
	' specify the upperLimit on our waveform mask.
	upperLimit(0,0)  =  0.0
	upperLimit(0,1)  =  0.05
	upperLimit(1,0)  =  2.5E-07
	upperLimit(1,1)  =  0.05
	upperLimit(2,0)  =  3.25E-07
	upperLimit(2,1)  =  0.8
	upperLimit(3,0)  =  3.26E-07
	upperLimit(3,1)  =  1.15
	upperLimit(4,0)  =  4.25E-07
	upperLimit(4,1)  =  1.15
	upperLimit(5,0)  =  5.00E-07
	upperLimit(5,1)  =  1.05
	upperLimit(6,0)  =  6.75E-07
	upperLimit(6,1)  =  1.05
	upperLimit(7,0)  =  7.25E-07
	upperLimit(7,1)  = -0.07
	upperLimit(8,0)  =  1.1E-06
	upperLimit(8,1)  =  0.05
	upperLimit(9,0)  =  1.25E-06
	upperLimit(9,1)  =  0.05
	upperLimit(10,0) =  1.75E-06
	upperLimit(10,1) =  0.05

	Set upperLimitVDC = callServer.CreateVeeDataContainer()
	upperLimitVDC.Create2DCoord1DArray(upperLimit)

	' The third parameter to "DisplaY_Test" is an array of 2D Coords that 
	' specify the lowerLimit on our waveform mask.
	lowerLimit(0,0)  =  0.0
	lowerLimit(0,1)  = -0.05
	lowerLimit(1,0)  =  3.50E-07
	lowerLimit(1,1)  = -0.05
	lowerLimit(2,0)  =  3.51E-07
	lowerLimit(2,1)  =  0.5
	lowerLimit(3,0)  =  4E-07
	lowerLimit(3,1)  =  0.95
	lowerLimit(4,0)  =  5.00E-07
	lowerLimit(4,1)  =  0.95
	lowerLimit(5,0)  =  6.00E-07
	lowerLimit(5,1)  =  0.9
	lowerLimit(6,0)  =  6.5E-07
	lowerLimit(6,1)  =  0.5
	lowerLimit(7,0)  =  6.51E-07
	lowerLimit(7,1)  = -0.45
	lowerLimit(8,0)  =  8.00E-07
	lowerLimit(8,1)  = -0.45
	lowerLimit(9,0)  =  9.25E-07
	lowerLimit(9,1)  = -0.2
	lowerLimit(10,0) =  1.1E-06
	lowerLimit(10,1) = -0.05
	lowerLimit(11,0) =  1.25E-06
	lowerLimit(11,1) = -0.05
	lowerLimit(12,0) =  1.75E-06
	lowerLimit(12,1) = -0.05

	Set lowerLimitVDC = callServer.CreateVeeDataContainer()
	lowerLimitVDC.Create2DCoord1DArray(lowerLimit)

	' Set up the input arguments for the "Display_Test" UserFunction
	' The UserFUnction has 3 inputs and 1 output
	ReDim inputs(2)
	ReDim outputs(0)
	Set inputs(0) = pulseVDC
	Set inputs(1) = upperLimitVDC 
	Set inputs(2) = lowerLimitVDC 

	' Call the "Display_Test" UserFunction
	Call lib.UserFunctions("Display_Test").Call(inputs, outputs)

    ' Get the returned value. It is a 1D array of 2D coords
	Set errorsVDC = outputs(0)

    ' If the array is not empty then some points fell outside the mask
	If errorsVDC.NumElements > 0 Then
		WScript.Echo "Test failed"
	Else
		WScript.Echo "Test passed"
	End If
	
Loop Until (MsgBox("Run again?", vbYesNo + vbQuestion) = vbNo) 



'******************************************************************************
'
' Utility subroutine that prints out usage text for this script.
'
Sub Usage()
	WScript.Echo ""
	WScript.Echo "Usage: CallVEE.vbs [hostname]"
	WScript.Quit
End Sub
