VERSION 5.00
Begin VB.Form Form1 
   Caption         =   "CallVEE Example written in Visual Basic"
   ClientHeight    =   2925
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   8820
   LinkTopic       =   "Form1"
   ScaleHeight     =   2925
   ScaleWidth      =   8820
   StartUpPosition =   3  'Windows Default
   Begin VB.CommandButton btnQuit 
      Caption         =   "Quit"
      Height          =   435
      Left            =   1680
      TabIndex        =   8
      Top             =   2340
      Width           =   1335
   End
   Begin VB.CommandButton btnExecuteTest 
      Caption         =   "Execute Test"
      Height          =   435
      Left            =   120
      TabIndex        =   7
      Top             =   2340
      Width           =   1335
   End
   Begin VB.TextBox txtDelay 
      Height          =   315
      Left            =   2280
      TabIndex        =   5
      Text            =   ".35"
      Top             =   1680
      Width           =   615
   End
   Begin VB.ListBox lstResults 
      Height          =   2595
      Left            =   3180
      TabIndex        =   4
      Top             =   180
      Width           =   5475
   End
   Begin VB.CheckBox chkDebugMode 
      Caption         =   "Debug Mode"
      Height          =   315
      Left            =   240
      TabIndex        =   3
      Top             =   1200
      Width           =   2055
   End
   Begin VB.TextBox txtHostname 
      Enabled         =   0   'False
      Height          =   315
      Left            =   240
      TabIndex        =   2
      Top             =   540
      Width           =   2175
   End
   Begin VB.CheckBox chkUseRemoteHost 
      Caption         =   "Use Remote Host"
      CausesValidation=   0   'False
      Height          =   375
      Left            =   240
      TabIndex        =   0
      Top             =   120
      Width           =   1575
   End
   Begin VB.Frame Frame1 
      Height          =   855
      Left            =   120
      TabIndex        =   1
      Top             =   180
      Width           =   2415
   End
   Begin VB.Label Label1 
      Caption         =   "Delay Value (Microseconds):"
      Height          =   255
      Left            =   180
      TabIndex        =   6
      Top             =   1740
      Width           =   2115
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'******************************************************************************
'
' File: CallVEE.bas
'
' Description:
'
'   This example shows how to do in Visual Basic what the callVEE.c program
'   does in C using the "C to VEE" API.
'
' Execution Instructions:
'
'   From the command line, type the following (or double-click this file in
'   the Windows Explorer)
'
'   CallVEE.exe
'
' This example is provided as an illustration "as is", and
' Keysight Technologies makes no warranty of any kind with regard to this
' example.
'
' Copyright (c) 2000-2018 Keysight Technologies.
'
'******************************************************************************
Option Explicit

' Globals
Dim server As CallableVEE.callServer
Dim lib As CallableVEE.Library
Dim synthesizePulseUF As CallableVEE.UserFunction
Dim displayTestUF As CallableVEE.UserFunction
Dim libFile As String
Dim progID As String
Dim upperLimit(10, 1) As Double
Dim lowerLimit(12, 1) As Double
Dim useRemoteHostChkBoxLastVal As Integer
Dim hostnameLastVal As String
Dim initializeServer As Boolean

Private Sub btnQuit_Click()
    ' Exit the program
    End
End Sub

Private Sub chkUseRemoteHost_Click()
    If chkUseRemoteHost.Value = vbChecked Then
        txtHostname.Enabled = True
    Else
        txtHostname.Enabled = False
    End If
End Sub

Private Sub Form_Load()
    ' Initialize globals
    progID = "CallableVEE.CallServer"
    libFile = "~installDir\examples\CallableVEE\lib.vee"
    initializeServer = True
    
    ' Initialize upper and lower limit masks, only need to
    ' do this once.
    upperLimit(0, 0) = 0#
    upperLimit(0, 1) = 0.05
    upperLimit(1, 0) = 0.00000025
    upperLimit(1, 1) = 0.05
    upperLimit(2, 0) = 0.000000325
    upperLimit(2, 1) = 0.8
    upperLimit(3, 0) = 0.000000326
    upperLimit(3, 1) = 1.15
    upperLimit(4, 0) = 0.000000425
    upperLimit(4, 1) = 1.15
    upperLimit(5, 0) = 0.0000005
    upperLimit(5, 1) = 1.05
    upperLimit(6, 0) = 0.000000675
    upperLimit(6, 1) = 1.05
    upperLimit(7, 0) = 0.000000725
    upperLimit(7, 1) = -0.07
    upperLimit(8, 0) = 0.0000011
    upperLimit(8, 1) = 0.05
    upperLimit(9, 0) = 0.00000125
    upperLimit(9, 1) = 0.05
    upperLimit(10, 0) = 0.00000175
    upperLimit(10, 1) = 0.05

    lowerLimit(0, 0) = 0#
    lowerLimit(0, 1) = -0.05
    lowerLimit(1, 0) = 0.00000035
    lowerLimit(1, 1) = -0.05
    lowerLimit(2, 0) = 0.000000351
    lowerLimit(2, 1) = 0.5
    lowerLimit(3, 0) = 0.0000004
    lowerLimit(3, 1) = 0.95
    lowerLimit(4, 0) = 0.0000005
    lowerLimit(4, 1) = 0.95
    lowerLimit(5, 0) = 0.0000006
    lowerLimit(5, 1) = 0.9
    lowerLimit(6, 0) = 0.00000065
    lowerLimit(6, 1) = 0.5
    lowerLimit(7, 0) = 0.000000651
    lowerLimit(7, 1) = -0.45
    lowerLimit(8, 0) = 0.0000008
    lowerLimit(8, 1) = -0.45
    lowerLimit(9, 0) = 0.000000925
    lowerLimit(9, 1) = -0.2
    lowerLimit(10, 0) = 0.0000011
    lowerLimit(10, 1) = -0.05
    lowerLimit(11, 0) = 0.00000125
    lowerLimit(11, 1) = -0.05
    lowerLimit(12, 0) = 0.00000175
    lowerLimit(12, 1) = -0.05
    
    ' Initialize GUI
    chkUseRemoteHost.Value = vbUnchecked
    useRemoteHostChkBoxLastVal = vbUnchecked
    txtHostname.Enabled = False
    hostnameLastVal = ""
    chkDebugMode.Value = vbUnchecked
        
End Sub

Private Sub btnExecuteTest_Click()
    Dim upperLimitVDC As CallableVEE.VeeDataContainer
    Dim lowerLimitVDC As CallableVEE.VeeDataContainer
    Dim pulseVDC As CallableVEE.VeeDataContainer
    Dim errorsVDC As CallableVEE.VeeDataContainer
    Dim inputs() As Variant
    Dim outputs() As Variant
    Dim delay As Double
    
    ' If Use Remote Host check box has changed or we have a new
    ' hostname, shut down current session
    If chkUseRemoteHost.Value <> useRemoteHostChkBoxLastVal Or _
       txtHostname.Text <> hostnameLastVal Then
        Set upperLimitVDC = Nothing
        Set lowerLimitVDC = Nothing
        Set pulseVDC = Nothing
        Set errorsVDC = Nothing
        Set synthesizePulseUF = Nothing
        Set displayTestUF = Nothing
        Set lib = Nothing
        Set server = Nothing
        useRemoteHostChkBoxLastVal = chkUseRemoteHost.Value
        hostnameLastVal = txtHostname.Text
        initializeServer = True
    End If
        
    ' If server has never been created, then create it
    If initializeServer Then
        ' Check to see if this is a local or remote create
        If chkUseRemoteHost.Value = vbChecked And txtHostname.Text <> "" Then
            Set server = CreateObject(progID, txtHostname.Text)
        Else
            Set server = CreateObject(progID)
        End If
        
        ' Have the server load the library of VEE UserFunctions
        Set lib = server.Libraries.Load(libFile)
        
        Set synthesizePulseUF = lib.UserFunctions("Synthesize_Pulse")
        Set displayTestUF = lib.UserFunctions("Display_Test")
    End If
    
    ' Get the Reference delay
    delay = CDbl(txtDelay.Text) * 0.000001
               
    ' Check for proper range, if not then bail out of sub
    If ((delay < 0.00000025) Or (delay > 0.00000045)) Then
        MsgBox "Valid delay values are between .25 and .45 microseconds", _
               vbOKOnly, "CallVEE Example"
        Exit Sub
    End If
    
    ' Set the debug mode on the server
    server.Debug = chkDebugMode.Value

    ' Set up the input arguments to the "Synthesize_Pulse" UserFunction.
    ' The UserFUnction has 1 input and 1 output
    ReDim inputs(0)
    ReDim outputs(0)
    inputs(0) = delay

    ' Call the Synthesize_Pulse UserFunction to create
    synthesizePulseUF.Call inputs, outputs

    ' The first parameter to "DisplaY_Test" is the synthesized pulse that was
    ' returned from the userFunction call to "Synthesize_Pulse", we just made.
    Set pulseVDC = outputs(0)

    ' The second parameter to "DisplaY_Test" is an array of 2D Coords that
    ' specify the upperLimit on our waveform mask.
    Set upperLimitVDC = server.CreateVeeDataContainer()
    Call upperLimitVDC.Create2DCoord1DArray(upperLimit)

    ' The third parameter to "DisplaY_Test" is an array of 2D Coords that
    ' specify the lowerLimit on our waveform mask.
    Set lowerLimitVDC = server.CreateVeeDataContainer()
    Call lowerLimitVDC.Create2DCoord1DArray(lowerLimit)

    ' Set up the input arguments for the "Display_Test" UserFunction
    ' The UserFUnction has 3 inputs and 1 output
    ReDim inputs(2)
    ReDim outputs(0)
    Set inputs(0) = pulseVDC
    Set inputs(1) = upperLimitVDC
    Set inputs(2) = lowerLimitVDC

    ' Call the "Display_Test" UserFunction
    displayTestUF.Call inputs, outputs

    ' Get the returned value. It is a 1D array of 2D coords
    Set errorsVDC = outputs(0)
    
    ' If the array is not empty then some points fell outside the mask
    If errorsVDC.NumElements > 0 Then
        lstResults.AddItem Time & ": Test failed with delay of " & _
                           txtDelay.Text & " microseconds on " & HostTested()
    Else
        lstResults.AddItem Time & ": Test passed with delay of " & _
                           txtDelay.Text & " microseconds on " & HostTested()
    End If
    
    ' As long as we are using the same call server, so need to re-establish
    ' the connection
    initializeServer = False
    
End Sub

Function HostTested() As String
    If chkUseRemoteHost.Value = vbChecked And txtHostname.Text <> "" Then
        HostTested = txtHostname.Text
    Else
        HostTested = "localhost"
    End If
End Function

    


