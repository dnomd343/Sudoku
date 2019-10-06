VERSION 5.00
Begin VB.Form Form3 
   AutoRedraw      =   -1  'True
   BorderStyle     =   1  'Fixed Single
   Caption         =   "Sudoku Displayer"
   ClientHeight    =   8985
   ClientLeft      =   45
   ClientTop       =   375
   ClientWidth     =   8730
   LinkTopic       =   "Form3"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   8985
   ScaleWidth      =   8730
   StartUpPosition =   2  'ÆÁÄ»ÖÐÐÄ
   Begin VB.CommandButton Command3 
      Caption         =   "-"
      Height          =   375
      Left            =   3600
      TabIndex        =   3
      Top             =   8520
      Width           =   1455
   End
   Begin VB.CommandButton Command2 
      Caption         =   ">"
      Height          =   375
      Left            =   5160
      TabIndex        =   0
      Top             =   8520
      Width           =   3015
   End
   Begin VB.CommandButton Command1 
      Caption         =   "<"
      Height          =   375
      Left            =   480
      TabIndex        =   2
      ToolTipText     =   "Try to Press A or D..."
      Top             =   8520
      Width           =   3015
   End
   Begin VB.Timer Timer1 
      Interval        =   5000
      Left            =   0
      Top             =   0
   End
   Begin VB.TextBox Text 
      Height          =   300
      Index           =   0
      Left            =   0
      TabIndex        =   1
      Top             =   0
      Visible         =   0   'False
      Width           =   300
   End
End
Attribute VB_Name = "Form3"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Dim Input_Data() As String
Dim Start_X, Start_Y, Square_Length, Distance As Integer
Dim use As Long
Private Sub Init_Object()
  On Error Resume Next
  Cls
  For x = 1 To 9
    For y = 1 To 9
      i = x + y * 9 - 9
      Text(i).Text = ""
      Text(i).FontSize = 40
      Text(i).MaxLength = 1
      Text(i).Width = Square_Length
      Text(i).Height = Square_Length
      Text(i).Left = Start_X + Distance * (x - 0.5) + Square_Length * (x - 1)
      Text(i).Top = Start_Y + Distance * (y - 0.5) + Square_Length * (y - 1)
    Next y
  Next x
  For i = 0 To 9
    If Int(i / 3) = i / 3 Then DrawWidth = 3 Else DrawWidth = 1
    Line (Start_X, Start_Y + (Square_Length + Distance) * i)-(Start_X + (Square_Length + Distance) * 9, Start_Y + (Square_Length + Distance) * i)
    Line (Start_X + (Square_Length + Distance) * i, Start_Y)-(Start_X + (Square_Length + Distance) * i, Start_Y + (Square_Length + Distance) * 9)
  Next i
End Sub
Private Sub Form_Load()
  On Error Resume Next
  For i = 1 To 81
    Load Text(i)
    Text(i).Left = 0
    Text(i).Alignment = 2
    Text(i).Appearance = 0
    Text(i).BackColor = Form3.BackColor
    Text(i).BorderStyle = 0
    Text(i).FontSize = 40
    Text(i).MaxLength = 1
    Text(i).Locked = True
    Text(i).Visible = True
  Next i
  Start_X = 300: Start_Y = 300
  Square_Length = 800
  Distance = 100
  Call Init_Object
  Call Data_Input
  Call Data_Output(0)
  use = 0
End Sub
Private Sub Data_Input()
  Open App.Path & "\Data_Input.txt" For Input As #1
    Line Input #1, dat
    For i = 1 To 81
      If Mid(dat, i, 1) <> "0" Then Text(i).ForeColor = vbRed
    Next i
  Close #1
  Open App.Path & "\Data_Output.txt" For Input As #1
    ReDim Input_Data(0)
    Do While Not EOF(1)
      Line Input #1, Input_Data(UBound(Input_Data))
      ReDim Preserve Input_Data(UBound(Input_Data) + 1)
    Loop
  Close #1
End Sub
Private Sub Data_Output(Num As Long)
  dat = Input_Data(Num)
  Form3.Caption = "Sudoku Displayer(" & Trim(Num + 1) & "/" & Trim(UBound(Input_Data)) & ")"
  Call Init_Object
  For i = 1 To 81
    Text(i).FontSize = 40
    Text(i).MaxLength = 1
    a = Mid(dat, i, 1)
    If a = "0" Then a = ""
    Text(i).Text = a
  Next i
End Sub
Private Sub Command1_Click()
  If use > 0 Then use = use - 1
  Call Data_Output(use)
End Sub
Private Sub Command2_Click()
  If use < UBound(Input_Data) - 1 Then use = use + 1
  Call Data_Output(use)
End Sub
Private Sub Command3_Click()
  On Error Resume Next
  dat = Int(InputBox("Please input a number", "Tips")) - 1
  If dat >= 0 And dat < UBound(Input_Data) Then use = dat: Data_Output (use)
End Sub
Private Sub Command1_KeyPress(KeyAscii As Integer)
  If KeyAscii = 97 Then Call Command1_Click
  If KeyAscii = 100 Then Call Command2_Click
End Sub
Private Sub Command2_KeyPress(KeyAscii As Integer)
  If KeyAscii = 97 Then Call Command1_Click
  If KeyAscii = 100 Then Call Command2_Click
End Sub
Private Sub Command3_KeyPress(KeyAscii As Integer)
  If KeyAscii = 97 Then Call Command1_Click
  If KeyAscii = 100 Then Call Command2_Click
End Sub
Private Sub Form_DblClick()
  Call Data_Input
  Call Data_Output(use)
End Sub
Private Sub Form_Unload(Cancel As Integer)
  Form1.Visible = True
End Sub
