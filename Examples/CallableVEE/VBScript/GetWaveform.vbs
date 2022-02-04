'******************************************************************************
'  
' File: GetWaveform.vbs
'
' Description:
'
'   This example shows how to create one of VEE unique data types, the waveform
'   data type.  It shows how to initialize the waveform and pass it to a VEE
'   UserFunction that expects a VEE Waveform as it's only input argument and 
'   returns a VEE waveform as its only output argument.
'
'   The VEE UserFunction "Display_Waveform" does nothing more than display the 
'   waveform and add the value 1 to each waveform data point and returning
'   the modified waveform. 
'
' Execution Instructions:
'
'   From the command line, type the following (or double-click this file in
'	the Windows Explorer)
'
'	cscript GetWaveform.vbs
'
' This example is provided as an illustration "as is", and
' Keysight Technologies makes no warranty of any kind with regard to this
' example.
'
' Copyright (c) 2000-2018 Keysight Technologies.
'
'******************************************************************************
Option Explicit

Dim callServer, lib
Dim inputVDC, outputVDC
Dim szInstallDir, szLibFile
Dim from, thru, mapType
Dim inputWaveform(9)
Dim outputWaveform()
Dim inputs(0)
Dim outputs(0)


' Create top-level object in CallableVEE Object model.  This starts the 
' CallableVEE server
Set callServer = CreateObject("CallableVEE.CallServer")

' Get the installation directory of the CallableVEE server
szInstallDir = callServer.InstallDir
szLibFile = szInstallDir & "\examples\CallableVEE\lib.vee"

' Have the server load a library of VEE UserFunctions
Set lib = callServer.Libraries.Load(szLibFile)

' Create a VEE Data Container
Set inputVDC = callServer.CreateVeeDataContainer()

' Initialize waveform array
inputWaveform(0) =  0.0
inputWaveform(1) =  1.0
inputWaveform(2) =  4.0
inputWaveform(3) =  9.0
inputWaveform(4) =  16.0
inputWaveform(5) =  25.0
inputWaveform(6) =  36.0
inputWaveform(7) =  49.0
inputWaveform(8) =  64.0
inputWaveform(9) =  81.0

' Create the waveform within the VEE Data Container
Call inputVDC.CreateWaveform(0, 0.1, 1, inputWaveform)

' Display the waveform data before and after the UserFunction call
WScript.Echo("This is the waveform that will be passed to the UserFunction:")
PrintWaveformData(inputWaveform)

' Set the VEE Data Container as the first and only input argument
Set inputs(0) = inputVDC

' Call the UserFunction "Display_Waveform"
WScript.Echo(vbCrLf & "Calling 'Display_Waveform' ..." & vbCrLf)
Call lib.UserFunctions("Display_Waveform").Call(inputs, outputs)

' Get the return value, which is the first and only output argument
Set outputVDC = outputs(0)

' Extract the data from the returned waveform
Call outputVDC.GetWaveform(from, thru, mapType, outputWaveform)

' Display the waveform data returned from the UserFunction call
WScript.Echo("This is the waveform that was returned from the UserFunction:")
PrintWaveformData(outputWaveform)



'******************************************************************************
'
' Utility subroutine that prints out the contents of a waveform array
'
Sub PrintWaveformData(waveformData)
	Dim szOutput
	Dim datum
	Dim bFirstPass

	bFirstPass = TRUE

	For Each datum in waveformData

		If Len(szOutput) >= 80 Then
			WScript.Echo(szOutput)
			szOutput = ""
			bFirstPass = TRUE
		End If

		If Not bFirstPass Then
			szOutput = szOutput & ", "
		Else
			bFirstPass = FALSE
		End If

		szOutput = szOutput & CStr(datum)
	Next

	WScript.Echo(szOutput)

End Sub

