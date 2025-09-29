#include "Display.h"

const    CODE DBCS[3]={
  {{0xA1,0xC9},{0xC6,0xF9},{0x40,0xA1},{0x7E,0xFE}},      //Big5
  {{0xA1,0xB0},{0xA9,0xF7},{0xA1,0xF0},{0xEF,0xFE}},      //GBK
  {{0x81,0xE0},{0x9F,0xEF},{0x40,0x80},{0x7E,0xFC}}};     //JIS
char     STR[16];

// Display::Display(GxIO& io, int8_t rst, int8_t busy) : GxGDEW0154M09(io, rst, busy) {
// }

Display::Display(GxEPD2_154_M09 epd2_instance) :  GxEPD2_BW<GxEPD2_154_M09, GxEPD2_154_M09::HEIGHT>(epd2_instance) {
	epd2.selectSPI(SPI, SPISettings(4000000, MSBFIRST, SPI_MODE0));
}

#define DEG2RAD 0.0174532925
void Display::drawArc(int x, int y, int StartAngle, int EndAngle, int r, int w, uint8_t color){
	bool Cap=true;
	int sx1, sx2, sy1, sy2;
	if(StartAngle>EndAngle){
		int temp = StartAngle;
		StartAngle = EndAngle;
		EndAngle = temp;
	}
	if(StartAngle<-359){ StartAngle = -359; }
	if(EndAngle>360) { EndAngle = 360; }
	if(EndAngle>StartAngle){
		if((EndAngle - StartAngle)>360) {
			drawCircle(x, y, r, color);
			return;
		}
		for(int ang =StartAngle; ang<EndAngle; ang++) {
			sx1 = cos(ang * DEG2RAD)*(r+w/2)+x;
			sy1 = sin(ang * DEG2RAD)*(r+w/2)+y;
			writePixel(sx1, sy1, color);
			if(w>1){
				sx2 = cos(ang * DEG2RAD)*(r-w/2)+x;
				sy2 = sin(ang * DEG2RAD)*(r-w/2)+y;
				writePixel(sx2, sy2, color);
				if(Cap){
					drawLine(sx1, sy1, sx2, sy2, color);
					Cap=false;
				}
			}
		}
		if(w>1){ drawLine(sx1, sy1, sx2, sy2, color); }
	}
}
void Display::fillArc(int x, int y, int StartAngle, int EndAngle, int r, int w, uint8_t color){
	bool Start=false;
	int sx1, sx2, sx3, sy1, sy2, sy3;
	if(StartAngle>EndAngle){
		int temp = StartAngle;
		StartAngle = EndAngle;
		EndAngle = temp;
	}
	if(StartAngle<-359){ StartAngle = -359; }
	if(EndAngle>360) { EndAngle = 360; }
	if(EndAngle>StartAngle){
		if((EndAngle - StartAngle)>360) {
			EndAngle = StartAngle + 360;
		}
		for(int ang =StartAngle; ang<EndAngle; ang+=6) {
			sx3 = cos(ang * DEG2RAD)*(r+w/2)+x;
			sy3 = sin(ang * DEG2RAD)*(r+w/2)+y;
			if(Start){ fillTriangle(sx1, sy1, sx2, sy2, sx3, sy3, color); }
			sx1=sx3; sy1=sy3; 
			sx3 = cos(ang * DEG2RAD)*(r-w/2)+x;
			sy3 = sin(ang * DEG2RAD)*(r-w/2)+y;
			if(Start){ fillTriangle(sx1, sy1, sx2, sy2, sx3, sy3, color); }
			sx2=sx3; sy2=sy3; 
			if(!Start){ Start=true; } 
		}
	}
}
// void Display::fillArc(int x, int y, int start_angle, int seg_count, int rx, int ry, int w, uint8_t color)
// {

  // byte seg = 3; // Segments are 3 degrees wide = 120 segments for 360 degrees
  // byte inc = 3; // Draw segments every 3 degrees, increase to 6 for segmented ring

  /*Calculate first pair of coordinates for segment start*/
  // float sx = cos((start_angle - 90) * DEG2RAD);
  // float sy = sin((start_angle - 90) * DEG2RAD);
  // uint16_t x0 = sx * (rx - w) + x;
  // uint16_t y0 = sy * (ry - w) + y;
  // uint16_t x1 = sx * rx + x;
  // uint16_t y1 = sy * ry + y;

  /*Draw colour blocks every inc degrees*/
  // for (int i = start_angle; i < start_angle + seg * seg_count; i += inc) {

    /*Calculate pair of coordinates for segment end*/
    // float sx2 = cos((i + seg - 90) * DEG2RAD);
    // float sy2 = sin((i + seg - 90) * DEG2RAD);
    // int x2 = sx2 * (rx - w) + x;
    // int y2 = sy2 * (ry - w) + y;
    // int x3 = sx2 * rx + x;
    // int y3 = sy2 * ry + y;

    // fillTriangle(x0, y0, x1, y1, x2, y2, color);
    // fillTriangle(x1, y1, x2, y2, x3, y3, color);

    /*Copy segment end to sgement start for next segment*/
    // x0 = x2;
    // y0 = y2;
    // x1 = x3;
    // y1 = y3;
  // }
// }

void     Display::begin() {
   SPI.begin(INK_SPI_SCK, MISO, INK_SPI_MOSI);
}

void     Display::clear() {
	fillScreen(GxEPD_WHITE);
}
void     Display::InitClib      (uint8_t fgColor, uint8_t bgColor){
	CSys.Style.fgColor = fgColor;
	CSys.Style.bgColor = bgColor;
	CSys.Style.AttValue = 0;
	CSys.Style.W_Space  = 1;
	CSys.Style.H_Space  = 1;	
	CSys.AttValue       = 0;
	CSys.SCRN = {{0,0},{width()-1, height()-1}};
	CSys.CID 			= UTF8;
#ifdef _MING9CH_
	HookFontFile(MING9C);
#endif
	CSys.CP = {0,0};
	CSys.LTable = nullptr;
	CSys.Style.Font   = &CSys.Enu;
	CSys.Style.DBFont = &CSys.Dbc;
}
FONT*    Display::LoadFont      (FONT* F){
	uint32_t Offset=0;

	for(uint8_t t=0;t<31;t++){ F->Name[t]=F->file[t]; }		//字形名稱
	F->Name[31]=0;
	F->Att.Code  =F->file[32];								//字形編碼法
	if(F->Att.Code>1 ){ CSys.CIDUsed=F->Att.Code-2; }		// -1:Unicode, 0:BIG5, 1:GBK, 2:JIS
	if(F->Att.Code==0){ CSys.CIDUsed=-1; }
	F->Att.Prop  =F->file[33];								// 字型寬度型態
	F->Width     =F->file[34];								// 字型全型寬度
	F->Height    =F->file[35];								// 字型全型高度
	F->semiWidth =F->file[36];								// 字型半型寬度
	F->semiHeight=F->file[37];								// 字型半型高度
	F->Att.UseIndex=(F->Att.Code?0:1);						// 是否使用索引  (Unicode及變動字寬字型會用到索引)
	F->Att.UseIndex=F->Att.UseIndex|F->Att.Prop;
	if(F->Att.UseIndex){
		F->BPC  =0;											// 若使用索引就用不到每字使用位元數
		Offset=PTR_ReadLong(&F->file[38]);
		F->Index = &F->file[42];							// 字型中的字元索引區段位址
		F->Gfx   = &F->file[Offset+42];						// 字型中的字元圖像區段位址
	}else{
		F->BPC=(F->Width*F->Height+7)>>3;
		F->Index= 0;
		F->Gfx  = &F->file[40];
	}
	return F;
}
uint8_t  Display::HookFontFile  (const uint8_t* F){
  switch(F[32]){
    case 0:   // Unicode
      // if(CSys.Enu.file!=NULL){ CSys.Enu.file=NULL; }
      // if(CSys.Dbc.file!=NULL){ CSys.Dbc.file=NULL; }
      CSys.Enu.file= (uint8_t*) F;
      SetFont(0,LoadFont(&CSys.Enu));
      if(CSys.Enu.file){ return 1; }else{ return 0; }
      break;
    case 1:   // English
      // if(CSys.Enu.file!=NULL){ CSys.Enu.file=NULL; }
      CSys.Enu.file=(uint8_t*) F;
      SetFont(0,LoadFont(&CSys.Enu));
      break;
    case 2:   // Big5
    case 3:   // GBK
    case 4:   // JIS
      // if(CSys.Dbc.file!=NULL){ CSys.Dbc.file=NULL; }
      CSys.Dbc.file=(uint8_t*) F;
      SetFont(1,LoadFont(&CSys.Dbc));
      break;
  }
  return 1;
}
void     Display::SetFont       (uint8_t ID, FONT* F){
	if(ID){ 
		CSys.Style.DBFont=F;
		if (CSys.Style.DBFont->Height>CSys.Style.MaxH){
			CSys.Style.MaxH = CSys.Style.DBFont->Height;
		}
	}else{ 
		CSys.Style.Font=F;
		if (CSys.Style.Font->Height>CSys.Style.MaxH){
			CSys.Style.MaxH = CSys.Style.Font->Height;
		}		
	}
}
uint32_t Display::cPrint        (char* Str, POINT Init, uint8_t C, uint8_t bC, Lid Lang){
	char*    Text  = Str;
	POINT    Start = (POINT){Init.x, Init.y};
	BLOCK    B     = {{0,0},{width(),height()}};
	BLOCK    CBK;
	uint8_t  w,h;
	uint8_t  Output= 1;
	uint32_t Count = 0;
	uint16_t NC=0;
	
	CSys.CP = Init;

	CSys.CID   =Lang;
	while(Output){
	//===========處理換行符號區段======================================
		NC=GetNextChar(Text);
		if(NC==0x0D){
			Text+=CSys.Skip;
			continue;
		}		//return: 不處理
		if(NC==0x0A){
			switch(CSys.Att.Dir){
				case 0:		// 0度
					Start.y += (CSys.Style.MaxH+ CSys.Style.H_Space);
					Start.x  = B.Start.x;
					break;
				case 1:		// 90度
					Start.y  = B.Start.y;
					Start.x -= (CSys.Style.MaxH+ CSys.Style.H_Space);
					break;
				case 2:		// 180度
					Start.y -= (CSys.Style.MaxH+ CSys.Style.H_Space);
					Start.x  = B.End.x;
					break;
				case 3:		// 270度
					Start.y  = B.End.y;
					Start.x += (CSys.Style.MaxH+ CSys.Style.H_Space);
					break;
			}
			Text+=CSys.Skip;
			continue;
		}
		if(NC==0x00){
			break;
		}
	//==================================================================
		if(GetChrArea(Text, Start, &CBK)){
			switch(CSys.Att.NType){
				case 0:
					Start=CBK.Start;
					break;
				case 1:
					Start=(POINT){CBK.End.x,CBK.Start.y};
					break;
				case 2:
					Start=CBK.End;
					break;
				case 3:
					Start=(POINT){CBK.Start.x,CBK.End.y};
					break;
			}
	//=============字元特殊效果預處理===================================
			switch(CSys.Style.Att.Fx){
				case FX_NORMAL:
					DrawChrEx(Text, Start ,C);
					break;
				case FX_HOLLOW:
					DrawChrEx(Text, (POINT){Start.x-1,Start.y  } ,C);
					DrawChrEx(Text, (POINT){Start.x  ,Start.y-1} ,C);
					DrawChrEx(Text, (POINT){Start.x+1,Start.y  } ,C);
					DrawChrEx(Text, (POINT){Start.x  ,Start.y+1} ,C);
					DrawChrEx(Text, Start , bC);
					break;
				case FX_SHADOW:
					switch(CSys.Att.Dir){
						case 0:
							DrawChrEx(Text, (POINT){Start.x+1  ,Start.y+1} , bC);
							break;
						case 1:
							DrawChrEx(Text, (POINT){Start.x-1  ,Start.y+1} , bC);
							break;
						case 2:
							DrawChrEx(Text, (POINT){Start.x-1  ,Start.y-1} , bC);
							break;
						case 3:
							DrawChrEx(Text, (POINT){Start.x+1  ,Start.y-1} , bC);
							break;
					}
					DrawChrEx(Text, Start , C);
					break;
				case FX_BACKGROUND:
					switch(CSys.Att.Dir){
						case 0:
							w = CBK.End.x-CBK.Start.x+1+CSys.Style.W_Space+CSys.Style.Att.Bold;
							h = CBK.End.y-CBK.Start.y+1+CSys.Style.H_Space;
							break;
						case 1:
							w = CBK.End.x-CBK.Start.x+1+CSys.Style.W_Space;
							h = CBK.End.y-CBK.Start.y+1+CSys.Style.H_Space+CSys.Style.Att.Bold;
							break;
						case 2:
							w = CBK.End.x-CBK.Start.x+1+CSys.Style.W_Space+CSys.Style.Att.Bold;
							h = CBK.End.y-CBK.Start.y+1+CSys.Style.H_Space;
							break;
						case 3:
							w = CBK.End.x-CBK.Start.x+1+CSys.Style.W_Space;
							h = CBK.End.y-CBK.Start.y+1+CSys.Style.H_Space+CSys.Style.Att.Bold;
							break;
					}
					fillRect(CBK.Start.x, CBK.Start.y, w, h, bC);
					DrawChrEx(Text, Start, C);
					break;
			}
			if(IsFullChr(Text)){
				CSys.CP.x+=(CSys.Style.W_Space+CSys.Style.Font->Width);
			}else{
				CSys.CP.x+=(CSys.Style.W_Space+CSys.Style.Font->semiWidth);
			}
	//=============字元輸出=========================================================================================
			if(CSys.Style.Att.U_Line) { DrawUnderLine(CBK); }
			Text+=CSys.Skip;
	//=============下一字元超出螢幕處理=============================================================================
			switch(CSys.Att.Dir){
				case 0:		// 0度
					Start.x=CBK.End.x+1;
					if(Start.x>B.End.x){
						Start.y += (CSys.Style.MaxH+ CSys.Style.H_Space);
						Start.x  = B.Start.x;
					}
					break;
				case 1:		// 90度
					Start.y=CBK.End.y+1;
					if(Start.y>B.End.y){
						Start.y  = B.Start.y;
						Start.x -= (CSys.Style.MaxH+ CSys.Style.H_Space);
					}
					break;
				case 2:		// 180度
					Start.x=CBK.Start.x-1;
					if(Start.x<B.Start.x){
						Start.y -= (CSys.Style.MaxH+ CSys.Style.H_Space);
						Start.x  = B.End.x;
					}
					break;
				case 3:		// 270度
					Start.y=CBK.Start.y-1;
					if(Start.y<B.Start.y){
						Start.y  = B.End.y;
						Start.x += (CSys.Style.MaxH+ CSys.Style.H_Space);
					}
					break;
			}
	//=============================================================================================================
			Count++;
			if(GetNextChar(Text)==0){ Output=0; }
		}else{
			Output=0;		//超出螢幕
		}
	}
	return Count;
}
uint32_t Display::cSimPrint     (char* Str, int16_t x, int16_t y, Lid Lang){
	return cPrint(Str,(POINT){x,y}, CSys.Style.fgColor, CSys.Style.bgColor, Lang);
}
uint32_t Display::cSimPrint     (char* Str, int16_t x, int16_t y, uint8_t Color){
	return cPrint(Str,(POINT){x,y}, Color, CSys.Style.bgColor, CSys.CID);
}
uint32_t Display::cSimPrint     (int32_t Value, int16_t x, int16_t y, uint8_t Color){
	return cPrint(ValueStrN(Value),(POINT){x,y}, Color, CSys.Style.bgColor, CSys.CID);
}
uint32_t Display::cSimPrint     (int32_t Value, int16_t x, int16_t y){
	return cPrint(ValueStrN(Value),(POINT){x,y}, CSys.Style.fgColor, CSys.Style.bgColor, CSys.CID);
}
uint32_t Display::cSimPrint     (char* Str, int16_t x, int16_t y, uint8_t Color, Lid Lang){
	return cPrint(Str,(POINT){x,y}, Color, CSys.Style.bgColor, Lang);
}
uint8_t  Display::DrawChrEx     (char* Str, POINT P, uint8_t Color){
	uint8_t  DB=0;
	uint32_t Offset=GetOffset(Str, &DB);
	uint8_t  W,H;
	uint8_t  Width,Height;
	uint8_t  WOff=0,HOff=0;
	uint8_t  t=0;
	uint8_t* Gfx;
	uint8_t  SFx=0;
	uint16_t Temp;
	uint8_t  Step=0;
	uint8_t  p,m=0;
	uint8_t  Italic=0;
	uint8_t  B=CSys.Style.Att.Bold;
	POINT DP;
  
	if(CSys.CIDUsed>=0){			//雙位元用
		if(DB){
			Width = CSys.Style.DBFont->Width;
			Height= CSys.Style.DBFont->Height;
			Gfx   =&CSys.Style.DBFont->Gfx[Offset];
		}else{
			Width = CSys.Style.Font->Width;
			Height= CSys.Style.Font->Height;
			Gfx   =&CSys.Style.Font->Gfx[Offset];
		}
	}else{							//UTF用
		WOff  = CSys.Style.Font->Gfx[Offset  ]&0x7F;
		HOff = WOff &0xF;
		WOff >>=4;		
		Width = CSys.Style.Font->Gfx[Offset+1];
		Height= CSys.Style.Font->Gfx[Offset+2];
		Gfx   =&CSys.Style.Font->Gfx[Offset+3];
	}
	//if((CSys.Style.Att.Fx==FX_TRANSHADOW)&&(g->Mode==0)){ SFx=1; }
	if(CSys.Style.Att.Italic){
		Step=Height>>2;
		p=(1<<Step)+1;
		Italic=1;
	}else{
		p=0;
	}
	for(H=0;H<Height+1;H++){
		for(W=0;W<(Width+CSys.Style.W_Space+B);W++){
			if((W<Width)&&(H<Height)){
				if(Gfx[0]&(1<<(7-t))){
					
					if(SFx){
						//Temp=PickPoint(g, (POINT){P.x+W+1, P.y+H+1});
						switch(CSys.Att.Dir){
							case 0:
								DP=(POINT){P.x+W+1+p+WOff,P.y+H+1+HOff};
								break;
							case 1:
								DP=(POINT){P.x-(H+1+HOff),P.y+W+1+p+WOff};
								break;
							case 2:
								DP=(POINT){P.x-(W+1+p+WOff),P.y-(H+1+HOff)};
								break;
							case 3:
								DP=(POINT){P.x+H+1+HOff,P.y-(W+1+p+WOff)};
								break;
						}
						drawPixel(DP.x,DP.y,Color);
					}
					if(B){
						switch(CSys.Att.Dir){
							case 0:
								DP=(POINT){P.x+W+1+p+WOff,P.y+H+HOff};
								break;
							case 1:
								DP=(POINT){P.x-(H+HOff),P.y+W+1+p+WOff};
								break;
							case 2:
								DP=(POINT){P.x-(W+1+p+WOff),P.y-(H+HOff)};
								break;
							case 3:
								DP=(POINT){P.x+H+HOff,P.y-(W+1+p+WOff)};
								break;
						}
						drawPixel(DP.x,DP.y,Color);
					}
					switch(CSys.Att.Dir){
						case 0:
							DP=(POINT){P.x+W+p+WOff,P.y+H+HOff};
							break;
						case 1:
							DP=(POINT){P.x-(H+HOff),P.y+W+p+WOff};
							break;
						case 2:
							DP=(POINT){P.x-(W+p+WOff),P.y-(H+HOff)};
							break;
						case 3:
							DP=(POINT){P.x+H+HOff,P.y-(W+p+WOff)};
							break;
					}
						drawPixel(DP.x,DP.y,Color);
				}
				t++;
				if(t==8){
					t=0;
					Gfx++;
				}
			}
		}
		if(Italic){
			m++;
			if(m==Step){
				p--;
				m=0;
			}
		}
	}
	return DB;
}
uint8_t  Display::UTF_fromLocal (char* Chr, uint16_t* UTF16, const uint8_t* Table, Lid LID){
	uint32_t   Row = 0 ;
	uint32_t   Col = 0 ;
	uint32_t   Line = 0 ;
	uint8_t    Length=2;

	switch(LID){
		case UTF:
			// UTF16[0]=Chr[1];
			// UTF16[0]=(UTF16[0]<<8)+Chr[0];
			UTF16[0]=Chr[0];
			UTF16[0]=(UTF16[0]<<8)+Chr[1];
			break;
		case UTF8:
			if(Chr[0]<0x80){
				UTF16[0]=Chr[0];
				Length=1;
			}
			else{
				if(Chr[0]&0x20){
					UTF16[0]=((Chr[0]&0xF)<<4)|((Chr[1]&0x3C)>>2);
					UTF16[0]=(UTF16[0]<<8)|((Chr[1]&0x3)<<6)|(Chr[2]&0x3F);
					Length=3;
				}
				else{
					UTF16[0]=((Chr[0]&0x1C)>>2);
					UTF16[0]=(UTF16[0]<<8)|((Chr[0]&0x3)<<6)|(Chr[1]&0x3F);
				}
			}
			break;
		case BIG5:
		case GBK:
		case JIS:
            Line = DBCS[LID-2].LE[0]-DBCS[LID-2].LS[0]+DBCS[LID-2].LE[1]-DBCS[LID-2].LS[1]+2;
            if((Chr[0] >= DBCS[LID-2].HS[0])&&(Chr[0] <= DBCS[LID-2].HE[0])){
                Row = Chr[0] - DBCS[LID-2].HS[0];
                if((Chr[1] >= DBCS[LID-2].LS[0])&&(Chr[1] <= DBCS[LID-2].LE[0])){
                    Col = Chr[1] - DBCS[LID-2].LS[0];
                }else{
                    if((Chr[1] >= DBCS[LID-2].LS[1])&&(Chr[1] <= DBCS[LID-2].LE[1]))
                        Col = Chr[1] - DBCS[LID-2].LS[1] + DBCS[LID-2].LE[0] - DBCS[LID-2].LS[0] + 1;
				}
				UTF16[0]=PTR_ReadInt((uint8_t*)&Table[(Row*Line+Col)*2]);
			}else{
                if((Chr[0] >= DBCS[LID-2].HS[1])&&(Chr[0] <= DBCS[LID-2].HE[1])){
                    Row = Chr[0] - DBCS[LID-2].HS[1] + DBCS[LID-2].HE[0] - DBCS[LID-2].HS[0] + 1;
                    if((Chr[1] >= DBCS[LID-2].LS[0])&&(Chr[1] <= DBCS[LID-2].LE[0])){
						Col = Chr[1] - DBCS[LID-2].LS[0];
					}else{
						if((Chr[1] >= DBCS[LID-2].LS[1])&&(Chr[1] <= DBCS[LID-2].LE[1]))
							Col = Chr[1] - DBCS[LID-2].LS[1] + DBCS[LID-2].LE[0] - DBCS[LID-2].LS[0]+1;
					}
					UTF16[0]=PTR_ReadInt((uint8_t*)&Table[(Row*Line+Col)*2]);
				}else{
					if(Chr[0]<0x80){
						UTF16[0]=Chr[0];
						Length=1;
					}
				}
			}
            break;
	}

	return Length;
}

uint32_t Display::GetOffset     (char* StrPtr, uint8_t *DBC){
	char* Ptr   =StrPtr;
	uint32_t   LO    =0;
	uint32_t   HO    =0;
	uint32_t   Row;
	uint16_t   Temp;
	int8_t     C     =CSys.CIDUsed;
	if(C>=0){			//在使用雙位元的字型系統下
		Row=DBCS[C].LE[0]-DBCS[C].LS[0]+DBCS[C].LE[1]-DBCS[C].LS[1]+2;
		if(IsFullChr(Ptr)){
			if(CSys.CID==(C+2)){
				if((Ptr[0]>=DBCS[C].HS[0])&&(Ptr[0]<=DBCS[C].HE[0])){
					HO=Ptr[0]-DBCS[C].HS[0];
					if((Ptr[1]>=DBCS[C].LS[0])&&(Ptr[1]<=DBCS[C].LE[0])){
						LO=Ptr[1]-DBCS[C].LS[0];
					}
					else{
						if((Ptr[1]>=DBCS[C].LS[1])&&(Ptr[1]<=DBCS[C].LE[1])){
							LO=Ptr[1]-DBCS[C].LS[1]+DBCS[C].LE[0]-DBCS[C].LS[0]+1;
						}
					}
				}
				else{
					if((Ptr[0]>=DBCS[C].HS[1])&&(Ptr[0]<=DBCS[C].HE[1])){
						HO=Ptr[0]-DBCS[C].HS[1]+DBCS[C].HE[0]-DBCS[C].HS[0]+1;
						if((Ptr[1]>=DBCS[C].LS[0])&&(Ptr[1]<=DBCS[C].LE[0]))
							LO=Ptr[1]-DBCS[C].LS[0];
						else{
							if((Ptr[1]>=DBCS[C].LS[1])&&(Ptr[1]<=DBCS[C].LE[1]))
								LO=Ptr[1]-DBCS[C].LS[1]+DBCS[C].LE[0]-DBCS[C].LS[0]+1;
						}
					}
				}
				HO = HO * Row + LO;
				HO *= CSys.Style.DBFont->BPC;
				*DBC=1;
			}else{
				HO=0;
				*DBC=1;
			}
		}
		else{
			HO=(Ptr[0]-0x20) * CSys.Style.Font->BPC;
			*DBC=0;
		}
	}else{		//在使用Unicode字型系統時
		switch(CSys.CID){
			case UTF:
				LO=PTR_ReadInt((uint8_t*)StrPtr);
				break;
			case UTF8:
				LO=GetNextChar(StrPtr);
				break;
			case BIG5:
			case GBK:
			case JIS:
				UTF_fromLocal(StrPtr, &Temp, CSys.LTable, CSys.CID);
				LO=Temp-1;
				break;
		}
		LO<<=2;
		HO=PTR_ReadLong(&CSys.Style.Font->Index[LO]);	//Offset
		*DBC=1;
	}
	return HO;
} /*
uint8_t  Display::IsInArea      (BLOCK B , POINT P){
	if((P.x>=B.Start.x)&&(P.x<=B.End.x)){
		if((P.y>=B.Start.y)&&(P.y<=B.End.y)){
			return 1;
		}
	}
	return 0;
} */
uint16_t Display::GetNextChar   (char* Str){
	uint8_t *Ptr=(uint8_t*)Str;
	uint16_t Temp=0;
	
	switch(CSys.CID){
		case UTF:
			Temp= Ptr[1]<<8 ;
			Temp+=Ptr[0];
			CSys.Skip=2;
			break;
		case UTF8:
			if(Ptr[0]<0x80){
				Temp=Ptr[0];
				CSys.Skip=1;
			}else{
				if(Ptr[0]<0xE0){
					Temp=Ptr[0]&0x1F;
					Temp<<=6;
					Temp+=Ptr[1]&0x3F;
					CSys.Skip=2;				
				}else{
					Temp  =Ptr[0]&0x0F;
					Temp<<=6;
					Temp +=Ptr[1]&0x3F;
					Temp<<=6;
					Temp +=Ptr[2]&0x3F;
					CSys.Skip=3;
				}
			}
			break;
		case BIG5:
		case GBK:
		case JIS:
			if(Ptr[0]<0x80){
				Temp=Ptr[0];
				CSys.Skip=1;
			}else{
				Temp=Ptr[1];
				Temp<<=8;
				Temp+=Ptr[0];
				CSys.Skip=2;
			}
			break;
	}
	return Temp;
} 
void     Display::GetTextArea   (char* Ptr, uint16_t* Width, uint16_t* Height, BLOCK Limit){
	char* Str    =Ptr;
	uint16_t   BWidth =Limit.End.x-Limit.Start.x;
	uint16_t   BHeight=Limit.End.y-Limit.Start.y;
	uint8_t    Temp,DB;
	uint16_t   TempW=0;

	*Width =0;
	*Height=(CSys.Style.MaxH+ CSys.Style.H_Space);
	while(*Str){
		if(IsReturn(Str)){ 
			*Width  =TempW;
			*Height+=CSys.Style.MaxH;
			continue; 
		}
		DB=IsFullChr(Str);
		if(DB){
			Temp=((CSys.CIDUsed<0)?CSys.Style.Font->Width:CSys.Style.DBFont->Width) + CSys.Style.W_Space;
			if((TempW+Temp)>BWidth){
				if(*Width<TempW){
					*Width  =TempW;
					*Height+=(CSys.Style.MaxH+ CSys.Style.H_Space);
				}
				TempW=0;
			}
			Str+=2;
		}else{
			Temp=((CSys.CIDUsed<0)?CSys.Style.Font->semiWidth:CSys.Style.Font->Width) + CSys.Style.W_Space;
			Str++;
		}
		TempW+=Temp;
	}
	if(*Width<TempW){ *Width  =TempW; }
	if(*Height>BHeight){ *Height=BHeight; }
}
uint8_t  Display::GetChrArea    (char* ChrPtr, POINT P, BLOCK* B){
	uint8_t Width = CSys.Style.W_Space -1;
	BLOCK   TB;
	BLOCK   Blk = CSys.SCRN;

	if(IsFullChr(ChrPtr)){
		if(CSys.CIDUsed<0){
			Width+=CSys.Style.Font->Width;
		}else{
			Width+=CSys.Style.DBFont->Width;
		}
	}else{
		if(CSys.CIDUsed<0){
			Width+=CSys.Style.Font->semiWidth;
		}else{
			Width+=CSys.Style.Font->Width;
		}
	}
	switch(CSys.Att.Dir){
		case 0:			// 0度
			B->Start = P;
			B->End.x = (B->Start.x + Width + CSys.Style.Att.Bold);
			if(CSys.Att.isSwap){							//超出時的處理
				if(B->End.x>Blk.End.x){
					B->Start.x=Blk.Start.x;
					B->Start.y=B->Start.y + CSys.Style.MaxH + CSys.Style.H_Space;
					B->End.x = B->Start.x + Width;
				}
			}
			B->End.y = (B->Start.y + CSys.Style.MaxH );
			CSys.CP=(POINT){B->End.x, B->Start.y};			//<====備份下一個字元輸出的位置
			break;
		case 1:			//  順時鐘90度
			B->End.x  =P.x;
			B->Start.y=P.y;
			B->End.y  =(B->Start.y + Width + CSys.Style.Att.Bold);
			if(CSys.Att.isSwap){
				if(B->End.y>Blk.End.y){
					B->Start.y=Blk.Start.y;
					B->End.x=B->End.x - CSys.Style.MaxH - CSys.Style.H_Space;
					B->End.y = B->Start.y + Width;
				}
			}
			B->Start.x = (B->End.x - CSys.Style.MaxH );
			CSys.CP=(POINT){B->End.x, B->End.y};				//<====備份下一個字元輸出的位置
			break;
		case 2:			//  
			B->End   =P;
			if(CSys.Att.U2D){
				B->Start.x =(B->End.x - CSys.Style.MaxH - CSys.Style.H_Space);
				if(CSys.Att.isSwap){
					if(B->Start.x<Blk.Start.x){
						B->End.x=Blk.End.x;
						B->End.y=B->End.y - Width - CSys.Style.Att.Bold - CSys.Style.W_Space;
						B->Start.x = B->End.x  - CSys.Style.MaxH - CSys.Style.H_Space;
					}
				}
				B->Start.y = (B->End.y - Width - CSys.Style.Att.Bold - CSys.Style.W_Space );
			}else{
				B->Start.x =(B->End.x - Width - CSys.Style.Att.Bold);
				if(CSys.Att.isSwap){
					if(B->Start.x<Blk.Start.x){
						B->End.x=Blk.End.x;
						B->End.y=B->End.y - CSys.Style.MaxH - CSys.Style.H_Space;
						B->Start.x = B->End.x - Width;
					}
				}
				B->Start.y = (B->End.y - CSys.Style.MaxH );
			}
			CSys.CP=(POINT){B->Start.x,B->End.y};			//<====備份下一個字元輸出的位置
			break;
		case 3:
			B->Start.x =P.x;
			B->End.y   =P.y;
			B->Start.y =(B->End.y - Width - CSys.Style.Att.Bold);
			if(CSys.Att.isSwap){
				if(B->Start.y<Blk.Start.y){
					B->End.y=Blk.End.y;
					B->Start.x=B->Start.x + CSys.Style.MaxH + CSys.Style.H_Space;
					B->Start.y = B->End.y - Width;
				}
			}
			B->End.x = (B->Start.x + CSys.Style.MaxH );
			CSys.CP=B->Start;								//<====備份下一個字元輸出的位置
			break;
	}
	
	return AreaUnion(*B, Blk, &TB);
}
char*    Display::GetFontName   (uint8_t ID){
  if(ID){
    return CSys.Style.DBFont->Name;
  }else{
    return CSys.Style.Font->Name;
  }
}

void     Display::DrawUnderLine (BLOCK B){
	drawLine((int32_t)B.Start.x, (int32_t)B.End.y, (int32_t)B.End.x, (int32_t)B.End.y, (uint32_t)CSys.Style.fgColor);
}
char*    Display::ValueStrN     (int32_t Value){
	uint8_t t=15;
	uint8_t Neg=0;

	STR[t]=0;
	if(Value==0) {
		STR[--t]=0x30;
	}else{
		if(Value<0){
			Neg=1;
			Value=-Value;
		}
	}
	while(Value){
		STR[--t]=(Value%10)+48;
		Value/=10;
	}
	if(Neg){ STR[--t]=0x2D; }
	return &STR[t];
}
//=========Ctrl Function============================================
/**********************************************
 * 功用：設定目前使用的雙位元字元對照表       *
 **********************************************/
/*
void   SetCTable       (const char* TBL){
	CSys.LTable=(u8*)TBL;
}
*/
/**********************************************
 * 功用：設定目前的雙位元編碼       *
 **********************************************/
/*
void   SetCode         (uint8_t LC){
    if(CSys.LTable!=NULL){ free(CSys.LTable); }
    CSys.LCode=LC;
    switch(LC){
        case BIG5:
            CSys.LTable=FILE_Open("Lang/cp950.tbl");
			break;
        case GBK:
            CSys.LTable=FILE_Open("Lang/cp936.tbl");
			break;
        case JIS:
			CSys.LTable=FILE_Open("Lang/cp932.tbl");
			break;
	}
}
*/
/*
void   HookCode        (uint8_t LC, uint8_t* C){
    if(CSys.LTable!=NULL){ CSys.LTable=NULL; }
    CSys.LCode=LC;
    CSys.LTable=C;
}
*/
/**********************************************
 * 功用：取得目前的雙位元編碼       *
 **********************************************/
/*
uint8_t     GetCode         (){
    return CSys.LCode;
}
*/
/**********************************************
 * 功用：設定輸出的字體顏色(直接)       *
 **********************************************/
uint8_t Display::SetColor      (uint8_t Color){
	uint8_t Temp = CSys.Style.fgColor;
	CSys.Style.fgColor = Color;
	return Temp;
}
/**********************************************
 * 功用：取得輸出的字體顏色       *
 **********************************************/
uint8_t Display::GetColor      (){
	return CSys.Style.fgColor;
}
/**********************************************
 * 功用：設定文字背景顏色       *
 **********************************************/
uint8_t Display::SetBgColor    (uint8_t Color){
	uint8_t Temp = CSys.Style.bgColor;
	CSys.Style.bgColor = Color;
	return Temp;
}
/**********************************************
 * 功用：取得文字背景顏色       *
 **********************************************/
uint8_t Display::GetBgColor    (){
	return CSys.Style.bgColor;
}
/**********************************************
 * 功用：設定文字特效       *
 **********************************************/
uint8_t  Display::SetFx         (CharFx Fx){
	uint8_t Temp = CSys.Style.Att.Fx;
	CSys.Style.Att.Fx=Fx;
	return Temp;
}
/**********************************************
 * 功用：設定文字橫方向的間距       *
 **********************************************/
uint8_t  Display::SetWSpace     (uint8_t W){
	uint8_t Temp=CSys.Style.W_Space;
	CSys.Style.W_Space=W;
	return Temp;
}
/**********************************************
 * 功用：取得文字橫方向的間距       *
 **********************************************/
uint8_t  Display::GetWSpace     (){
	return CSys.Style.W_Space;
}
/**********************************************
 * 功用：設定行距       *
 **********************************************/
uint8_t  Display::SetHSpace     (uint8_t H){
	uint8_t Temp = CSys.Style.H_Space;
	CSys.Style.H_Space = H;
	return Temp;
}
/**********************************************
 * 功用：取得行距       *
 **********************************************/
uint8_t  Display::GetHSpace     (){
	return CSys.Style.H_Space;
}
/**********************************************
 * 功用：設定字元方向       *
 **********************************************/
uint8_t  Display::SetRotate     (uint8_t R){
	uint8_t Temp = CSys.Att.Dir;
	CSys.Att.Dir = R;
	if(CSys.Att.U2D){
		CSys.Att.NType = R<3?R+1:0;
	}else{
		CSys.Att.NType = R;
	}
	return Temp;
}
/**********************************************
 * 功用：設定文字方向       *
 **********************************************/
uint8_t  Display::SetDN         (uint8_t DN){
	uint8_t Temp = CSys.Att.U2D;
	CSys.Att.U2D = DN;
	if(DN){
		CSys.Att.NType=CSys.Att.Dir<3?CSys.Att.Dir+1:0;
	}else{
		CSys.Att.NType=CSys.Att.Dir;
	}
	return Temp;
}
/**********************************************
 * 功用：設定文字的屬性(粗體、斜體、下底線)       *
 **********************************************/
void     Display::SetBIU        (uint8_t B, uint8_t I, uint8_t U){
	CSys.Style.Att.Bold  =B;
	CSys.Style.Att.Italic=I;
	CSys.Style.Att.U_Line=U;
}
/**********************************************
 * 功用：取得目前最大的字高      *
 **********************************************/
uint8_t  Display::GetMaxH       (){
	return CSys.Style.MaxH;
}
/**********************************************
* 功用：設定文字編碼系統
**********************************************/
void     Display::SetCID        (Lid C){
	CSys.CID=C;
}


uint32_t Display::PTR_ReadLong  (uint8_t* Ptr){
  uint32_t RV=0;
  uint8_t  t;
  for(t=0;t<4;t++){
    RV<<=8;
    RV += Ptr[3-t];
  }
  return RV;
}
uint32_t Display::PTR_ReadLongBE(uint8_t* Ptr){
  uint32_t RV=0;
  uint8_t  t;
  for(t=0;t<4;t++){
    RV<<=8;
    RV += Ptr[t];
  }
  return RV;
}
uint16_t Display::PTR_ReadInt   (uint8_t* Ptr){    // 高位在後
  uint16_t RV=0;
  uint8_t  t;
  for(t=0;t<2;t++){
    RV<<=8;
    RV += Ptr[1-t];
  }
  return RV;
}
uint16_t Display::PTR_ReadIntBE (uint8_t* Ptr){    // 高位在前
  uint16_t RV=0;
  uint8_t  t;
  for(t=0;t<2;t++){
    RV<<=8;
    RV += Ptr[t];
  }
  return RV;
}
uint8_t  Display::AreaUnion     (BLOCK Blk1, BLOCK Blk2, BLOCK* R){
	if((Blk1.Start.x>Blk2.End.x)||(Blk2.Start.x>Blk1.End.x)){ return 0; }
	if((Blk1.Start.y>Blk2.End.y)||(Blk2.Start.y>Blk1.End.y)){ return 0; }
	R->Start.x=(Blk1.Start.x>Blk2.Start.x)?Blk1.Start.x:Blk2.Start.x;
	R->Start.y=(Blk1.Start.y>Blk2.Start.y)?Blk1.Start.y:Blk2.Start.y;
	R->End.x  =(Blk1.End.x  <Blk2.End.x)  ?Blk1.End.x  :Blk2.End.x;
	R->End.y  =(Blk1.End.y  <Blk2.End.y)  ?Blk1.End.y  :Blk2.End.y;
	return 1;
}
uint8_t  Display::IsReturn      (char* Ptr){
	uint16_t Next = GetNextChar(Ptr);
	switch(Next){
		case 0xA:
		case 0x9:
			if(CSys.CID>0){ Ptr++; }
			else     { Ptr+=2;}
			return 1;
		default:
			return 0;
	}
}
uint8_t  Display::IsFullChr     (char* Ptr){
	//uint8_t  DB;
	//uint32_t Offset = GetOffset(Ptr, &DB);

	// if(CSys.CIDUsed>=0){
		// return DB;
	// }else{
		// return CSys.Style.Font->Gfx[Offset  ]&0x80;
	// }
	
	if(*Ptr <= 0xA0)
		return 0;
	else
		return 1;
}

void Display::update(){
	this->display();
	//while (this->nextPage());
}

void Display::qrcode(const char *string, uint16_t x, uint16_t y, uint8_t width, uint8_t version) {
  
  //Create the QR code
  QRCode qrcode;
  uint8_t qrcodeData[qrcode_getBufferSize(version)];
  qrcode_initText(&qrcode, qrcodeData, version, 0, string);
  
  //Top quiet zone
  uint8_t thickness = width / qrcode.size;
  uint16_t lineLength = qrcode.size * thickness;
  uint8_t xOffset = x + (width-lineLength)/2;
  uint8_t yOffset = y + (width-lineLength)/2;
  fillRect(x, y, width, width, GxEPD_WHITE);

  for (uint8_t y = 0; y < qrcode.size; y++) {
    for (uint8_t x = 0; x < qrcode.size; x++) {
      uint8_t q = qrcode_getModule(&qrcode, x, y);
      if (q) fillRect(x * thickness + xOffset, y * thickness + yOffset, thickness, thickness, GxEPD_BLACK);
    }
  }
}
void Display::qrcode(const String &string, uint16_t x, uint16_t y, uint8_t width, uint8_t version) {
  int16_t len = string.length() + 1;
  char buffer[len];
  string.toCharArray(buffer, len);
  qrcode(buffer, x, y, width, version);
}
