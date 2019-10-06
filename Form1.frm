VERSION 5.00
Begin VB.Form Form1 
   AutoRedraw      =   -1  'True
   BorderStyle     =   1  'Fixed Single
   Caption         =   "Sudoku Calculator"
   ClientHeight    =   9045
   ClientLeft      =   45
   ClientTop       =   390
   ClientWidth     =   8730
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   9045
   ScaleWidth      =   8730
   StartUpPosition =   2  'ÆÁÄ»ÖÐÐÄ
   Begin VB.CommandButton Command4 
      Caption         =   "Clear"
      Height          =   375
      Left            =   4440
      TabIndex        =   4
      Top             =   8520
      Width           =   1965
   End
   Begin VB.CommandButton Command3 
      Caption         =   "Save"
      Height          =   375
      Left            =   2400
      TabIndex        =   3
      Top             =   8520
      Width           =   1965
   End
   Begin VB.CommandButton Command2 
      Caption         =   "Open"
      Height          =   375
      Left            =   360
      TabIndex        =   2
      Top             =   8520
      Width           =   1965
   End
   Begin VB.CommandButton Command1 
      Caption         =   "Calculate"
      Height          =   375
      Left            =   6480
      TabIndex        =   1
      Top             =   8520
      Width           =   1965
   End
   Begin VB.TextBox Text 
      Height          =   300
      Index           =   0
      Left            =   0
      TabIndex        =   0
      Top             =   0
      Visible         =   0   'False
      Width           =   300
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit
Dim Start_X, Start_Y, Square_Length, Distance As Integer
Private Sub Init_Object()
  Cls
  Dim x, y, i
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
Private Sub Form_Activate()
  Text(1).SetFocus
End Sub
Private Sub Form_Load()
  Dim i As Integer
  For i = 1 To 81
    Load Text(i)
    Text(i).Left = 0
    Text(i).Alignment = 2
    Text(i).Appearance = 0
    Text(i).BackColor = Form1.BackColor
    Text(i).BorderStyle = 0
    Text(i).FontSize = 40
    Text(i).MaxLength = 1
    Text(i).Visible = True
  Next i
  Start_X = 300: Start_Y = 300
  Square_Length = 800
  Distance = 100
  Call Init_Object
End Sub
Private Sub Command2_Click()
  On Error Resume Next
  Dim i As Integer, dat As String
  If Dir(App.Path & "\temp") = "" Then
    MsgBox "No Saved File"
    Exit Sub
  End If
  For i = 1 To 81
    Text(i).Text = ""
  Next i
  Open App.Path & "\temp" For Input As #1
    Line Input #1, dat
    For i = 1 To 81
      If Mid(dat, i, 1) <> "0" Then Text(i).Text = Mid(dat, i, 1)
    Next i
  Close #1
End Sub
Private Sub Command3_Click()
  On Error Resume Next
  Dim i As Integer
  Open App.Path & "\temp" For Output As #1
  For i = 1 To 81
    Print #1, Trim(Val(Text(i).Text));
  Next i
  Close #1
  If Dir(App.Path & "\temp") <> "" Then
    MsgBox "Saved Successfully"
  Else
    MsgBox "Failed Save"
  End If
End Sub
Private Sub Command4_Click()
  Dim i As Integer
  If MsgBox("Are you sure ?", vbQuestion + vbYesNo, "Tips") = vbYes Then
    For i = 1 To 81
      Text(i).Text = ""
    Next i
  End If
End Sub
Private Sub Command1_Click()
  On Error Resume Next
  Dim i As Integer
  Open App.Path & "\Data_Input.txt" For Output As #1
  For i = 1 To 81
    Print #1, Trim(Val(Text(i).Text));
  Next i
  Close #1
  Kill App.Path & "\Compete"
  If Dir(App.Path & "\Sudoku_Engine.exe") = "" Then
    MsgBox "Sudoku Engine not Found", vbCritical
    Exit Sub
  Else
    Shell App.Path & "\Sudoku_Engine.exe"
  End If
  Form2.Timer1.Enabled = True
  Form2.Show 1
End Sub
Private Sub Form_Unload(Cancel As Integer)
  On Error Resume Next
  Kill App.Path & "\Compete"
End Sub
