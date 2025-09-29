#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include <SPI.h>
#include <GxEPD2_BW.h>
#include "config.h"
#include "Ming9c.h"
#include "utility/qrcode.h"


typedef enum {UTF,UTF8,BIG5,GBK,JIS} Lid;
typedef enum {FX_NORMAL,FX_HOLLOW,FX_SHADOW,FX_BACKGROUND,FX_TRANSHADOW} CharFx;

typedef struct POINT	{
	int32_t x;
	int32_t y;
}POINT;
typedef struct BLOCK	{
   POINT Start;
   POINT End;
} BLOCK;
typedef struct CODE    {
	uint8_t HS[2];
	uint8_t HE[2];
	uint8_t LS[2];
	uint8_t LE[2];
} CODE;
typedef struct FONT    {
  char    Name[32];     //字型名稱
  uint8_t *file;        //字型指針
  uint8_t *Index;       //字元索引位置指針
  uint8_t *Gfx;         //字元圖像開始位置指針
  struct{
    uint8_t  Code    :3;     //本字型的編碼法 0:Unicode, 1:English, 2:BIG5, 3:GBK, 4:JIS
    uint8_t  Prop    :1;     //本字型為固定寬度或可變寬度 0:Fix 1:Prop
    uint8_t  UseIndex:1;     //是否使用索引 
    uint8_t  Dummy   :3;
  } Att;
  uint8_t semiWidth;     // 使用固定寬度字型時的半型字寬
  uint8_t semiHeight;    // 半型字高
  uint8_t Width;         // 全型字寬
  uint8_t Height;        // 全型字高
  uint8_t BPC;           // 固定寬度字型每字所佔的位元數BytePerChar
} FONT;
typedef struct CHARSTAT{
	uint8_t fgColor;
	uint8_t bgColor;
	union{
		uint8_t AttValue;
		struct {
			uint8_t Align :2;    //0:Left, 1:Center, 2:Right
			uint8_t Bold  :1;    //000:Normal, 001:Bold, 010:Italic, 100:UnderLine
			uint8_t Italic:1;	//斜體
			uint8_t U_Line:1;	//底線
			uint8_t Fx    :3;    //0:Normal, 1:Hollow, 2:Shadow, 3: Background, 4:ColorShadow
		} Att;
	};
	uint8_t    W_Space;
	uint8_t    H_Space;
	uint8_t    MaxH;
	FONT* Font;
	FONT* DBFont;
} CHARSTAT;
typedef struct CHRSYS{
  CHARSTAT Style;     		// 字元輸出狀態
  POINT    CP;        		// 最後的輸出位置
  FONT     Dbc;       		// 雙位元字體(Unicode字體兼用)
  FONT     Enu;       		// 英數字體
  uint8_t *LTable;    		// 轉換碼表
  Lid      CID;       		// 程式文字系統使用的編碼
  int8_t   CIDUsed;   		// 載入字型使用的編碼      -1:Unicode, 0:BIG5, 1:GBK, 2:JIS
  uint8_t  LCode;     		// 編碼法
  uint8_t  Skip;			// 下個字的字元數
  union{
	  uint8_t AttValue;
	  struct{
	    uint8_t isInit:1;		// 是否初始化
	    uint8_t isSwap:1;		// 是否自動換行
	    uint8_t U2D:1;		// 是否直書
	    uint8_t Dir:2;    	// 旋轉方向 0: 0度(正常); 1: 90度(順時針); 2:180度; 3:270度(順時針)
	    uint8_t NType:2;		// 
	    uint8_t Dummy:1;
      }Att;
  };
  BLOCK    SCRN;	
} CHRSYS;

//class Display : public GxGDEW0154M09 {
class Display : public GxEPD2_BW<GxEPD2_154_M09, GxEPD2_154_M09::HEIGHT>{
  public:
	//Display(GxIO& io, int8_t rst, int8_t busy);
	Display(GxEPD2_154_M09 epd2_instance);
	//SPIClass hspi;
	void begin();
	void clear();
	void     drawArc(int x, int y, int StartAngle, int EndAngle, int r, int w, uint8_t color);
	void     fillArc(int x, int y, int StartAngle, int EndAngle, int r, int w, uint8_t color);
	//void     fillArc(int x, int y, int start_angle, int seg_count, int rx, int ry, int w, uint8_t color);
	void     InitClib      (uint8_t fgColor = 1, uint8_t bgColor = 0);
	FONT*    LoadFont      (FONT* F);
    char*    GetFontName   (uint8_t ID);
    uint8_t  HookFontFile  (const uint8_t* F);
    void     SetFont       (uint8_t ID, FONT* F);
	uint32_t cPrint        (char* Str, POINT Init, uint8_t C, uint8_t bC, Lid Lang);
	uint32_t cSimPrint     (char* Str, int16_t x, int16_t y, Lid Lang = UTF8);
	uint32_t cSimPrint     (char* Str, int16_t x, int16_t y, uint8_t Color);
	uint32_t cSimPrint     (char* Str, int16_t x, int16_t y, uint8_t Color, Lid Lang);
	uint32_t cSimPrint     (int32_t Value, int16_t x, int16_t y, uint8_t Color);
	uint32_t cSimPrint     (int32_t Value, int16_t x, int16_t y);
	uint8_t  DrawChrEx     (char* Str, POINT P, uint8_t Color);
    uint32_t PTR_ReadLong  (uint8_t* Ptr);
    uint16_t PTR_ReadInt   (uint8_t* Ptr);
    uint32_t PTR_ReadLongBE(uint8_t* Ptr);
    uint16_t PTR_ReadIntBE (uint8_t* Ptr);
    uint32_t GetOffset     (char* StrPtr, uint8_t *DBC);
    uint8_t  IsInArea      (BLOCK B , POINT P);
	uint8_t  IsFullChr     (char* Ptr);
	uint8_t  IsReturn      (char* Ptr);
	uint16_t GetNextChar   (char* Str);
	uint8_t  UTF_fromLocal (char* Chr, uint16_t* UTF16, const uint8_t* Table, Lid LID);
	void     GetTextArea   (char* Ptr, uint16_t* Width, uint16_t* Height, BLOCK Limit);
	uint8_t  GetChrArea    (char* ChrPtr, POINT P, BLOCK* B);
	void     DrawUnderLine (BLOCK B);
	uint8_t  AreaUnion     (BLOCK Blk1, BLOCK Blk2, BLOCK* R);
	char*    ValueStrN     (int32_t Value);
//=========Ctrl Function============================================
	//void   SetCTable       (const char* TBL);
	//void   SetCode         (uint8_t LC);
	//void   HookCode        (uint8_t LC, uint8_t* C);
	uint8_t  GetCode         (void);
	uint8_t  SetColor        (uint8_t Color);
	uint8_t  GetColor        ();
	uint8_t  SetBgColor      (uint8_t Color);
	uint8_t  GetBgColor      ();
	uint8_t  SetFx           (CharFx Fx);
	uint8_t  SetWSpace       (uint8_t W);
	uint8_t  GetWSpace       ();
	uint8_t  SetHSpace       (uint8_t H);
	uint8_t  GetHSpace       ();
	uint8_t  SetRotate       (uint8_t R);
	uint8_t  SetDN           (uint8_t DN);
	void     SetBIU          (uint8_t B, uint8_t I, uint8_t U);
	uint8_t  GetMaxH         ();
	void     SetCID          (Lid C);
  
	void qrcode(const char *string, uint16_t x = 5, uint16_t y = 45, uint8_t width = 70, uint8_t version = 7);
	void qrcode(const String &string, uint16_t x = 5, uint16_t y = 45, uint8_t width = 70, uint8_t version = 7);

	void update();

    CHRSYS   CSys;

  private:
  
    CODE     DBCS[3];
	uint8_t TempA;
	uint8_t TempB;


};
#endif
