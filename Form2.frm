VERSION 5.00
Begin VB.Form Form2 
   BorderStyle     =   0  'None
   Caption         =   "Please Wait..."
   ClientHeight    =   660
   ClientLeft      =   0
   ClientTop       =   0
   ClientWidth     =   3240
   LinkTopic       =   "Form2"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   660
   ScaleWidth      =   3240
   ShowInTaskbar   =   0   'False
   StartUpPosition =   2  'ÆÁÄ»ÖÐÐÄ
   Begin VB.Timer Timer1 
      Interval        =   300
      Left            =   0
      Top             =   0
   End
   Begin VB.Label Label1 
      AutoSize        =   -1  'True
      Caption         =   "Please Wait..."
      BeginProperty Font 
         Name            =   "ËÎÌå"
         Size            =   26.25
         Charset         =   134
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   690
      Left            =   0
      TabIndex        =   0
      ToolTipText     =   "Double Click to Exit..."
      Top             =   0
      Width           =   3240
   End
End
Attribute VB_Name = "Form2"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Sub Form_DblClick()
  If MsgBox("Are you sure to exit?", vbQuestion + vbOKCancel, "Tips") = vbOK Then
    Shell "taskkill /f /im Sudoku_Engine.exe"
    Timer1.Enabled = False
    Form2.Hide
  End If
End Sub
Private Sub Label1_DblClick()
  Call Form_DblClick
End Sub
Private Sub Timer1_Timer()
  On Error Resume Next
  If Dir(App.Path & "\Compete") <> "" Then
    Open App.Path & "\Compete" For Input As #1
      Line Input #1, dat
      If Trim(dat) = "Error!!!" Then
        MsgBox "Error or No-Solution", vbCritical
        Timer1.Enabled = False
        Form2.Hide
        Close #1
        Exit Sub
      End If
    Close #1
    Timer1.Enabled = False
    Form1.Visible = False
    Form2.Hide
    Form3.Show
  End If
End Sub
