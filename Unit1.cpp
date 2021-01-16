#include <vcl.h>
#include <stdio.h>
#include <string.h>

#pragma hdrstop

#include "src/xtalw.h"

#include "Unit1.h"
#include "Unit2.h"

#pragma package(smart_init)
#pragma resource "*.dfm"

TForm1 *Form1;

struct DesktopSize {
  int Width;
  int Height;
};

struct DesktopSize DS;

int real_count;
char * SHOW_MAGIC = NULL;

__fastcall TForm1::TForm1(TComponent* Owner): TForm(Owner) {
}

void magic_generate(void) {
  for (int i = 0; i < (real_count - 2); i++) {
    if (((i + SHOW_MAGIC[i]) % 3) == 0) {
      SHOW_MAGIC[i] = (char)genrand('A', 'Z');
    }
    else {
      SHOW_MAGIC[i] = (char)genrand('a', 'z');
    }
  }

  SHOW_MAGIC[real_count - 1] = 0x00;
}

void __fastcall TForm1::Timer1Timer(TObject *Sender) {
  AnsiString TempString;

  magic_generate();
  TempString = AnsiString(SHOW_MAGIC);

  //Memo1->Clear();
  Memo1->Text = TempString;
}

void MemoryFree(void) {
  if (SHOW_MAGIC != NULL) {
    free(SHOW_MAGIC);
    SHOW_MAGIC = NULL;
  }
}

void __fastcall TForm1::FormClose(TObject *Sender, TCloseAction &Action) {
  MemoryFree();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormShow(TObject *Sender) {
  /* Hide control panel OS Windows  */
  SetWindowLong(Handle, GWL_EXSTYLE, !WS_EX_APPWINDOW);
  
  /* Initialized random number */
  srand((uint32_t)time(NULL));

  /* Check size desktop and write in global DS struct */
  DS.Width  = GetDeviceCaps(GetDC(0), HORZRES);
  DS.Height = GetDeviceCaps(GetDC(0), VERTRES);

  /* Calculate size buffer for MAGIC data and allocate memory */
  real_count = (DS.Width  / Form1->Memo1->Font->Size) *
               (DS.Height / Form1->Memo1->Font->Size);

  if ((DS.Width < 1) || (DS.Height < 1) ||
     (real_count < DS.Width) || (real_count < DS.Height)) {

    Form1->Close();
  }

  SHOW_MAGIC = (char *)calloc(real_count, 1);

  /* Close application if cannot allocate memory */
  if (SHOW_MAGIC == NULL) {
    Form1->Close();
  }

  /* Application == GLOBAL */
  SetWindowPos(Handle, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOACTIVATE |
    SWP_NOMOVE | SWP_NOSIZE);
    
  PostMessage(Handle, WM_SYSCOMMAND, SC_MAXIMIZE, 1);

  /* Show password check form */
  Form2->Show();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormDestroy(TObject *Sender)
{
  MemoryFree();        
}
//---------------------------------------------------------------------------


