/*************************************************************************
 *
*    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2008
 *
 *    File name   : drv_glcd.h
 *    Description : Graphical LCD driver include file
 *
 *    History :
 *    1. Date        : 6, March 2008
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *
 *    $Revision: 18137 $
 **************************************************************************/
#include "arm_comm.h"
#include "bmp.h"

#ifndef __GLCD_DRV_H
#define __GLCD_DRV_H


#define FrameSizeX  C_GLCD_H_SIZE
#define FrameSizeY  C_GLCD_V_SIZE

typedef struct 
{
  uint8_t   red;
  uint8_t   green;
  uint8_t   blue;
  uint8_t   :8;
} lcd_24bitpixel_t;

typedef struct _Pic_t {
  Int32U  H_Size;
  Int32U  V_Size;
  Int32U  BitsPP;
  Int32U  BytesPP;
  pInt32U pPalette;
  lcd_24bitpixel_t * pPicStream;
  pInt8U  pPicDesc;
} Pic_t, *pPic_t;


typedef struct _FontType_t {
  Int32U H_Size;
  Int32U V_Size;
  Int32U CharacterOffset;
  Int32U CharactersNuber;
  pInt8U pFontStream;
  pInt8U pFontDesc;
} FontType_t, *pFontType_t;

typedef Int32U LdcPixel_t, *pLdcPixel_t;

// pixel clock frequency < 6.4MHz
// ~5.4Mhz = 408 clk per line * (240 line + 3 Vsynch pulse
//           + 5 front porch + 15 back porch)

#define C_GLCD_REFRESH_FREQ     (50HZ)
#define C_GLCD_H_SIZE           480       //
#define C_GLCD_H_PULSE          45        //
#define C_GLCD_H_FRONT_PORCH    45        //45
#define C_GLCD_H_BACK_PORCH     45       //45
#define C_GLCD_V_SIZE           272       //
#define C_GLCD_V_PULSE          16        //
#define C_GLCD_V_FRONT_PORCH    16        //16
#define C_GLCD_V_BACK_PORCH     16        //16

#define C_GLCD_PWR_ENA_DIS_DLY  50000
#define C_GLCD_ENA_DIS_DLY      50000

#define CRSR_PIX_32     0
#define CRSR_PIX_64     1
#define CRSR_ASYNC      0
#define CRSR_FRAME_SYNC 2

#define TEXT_DEF_TAB_SIZE 5

#define TEXT_BEL1_FUNC()

#define MAX_GLCD_STR_SIZE       256

extern Int32U LCD_Buffer[2][C_GLCD_H_SIZE * C_GLCD_V_SIZE];
extern int BuffIndex;

#define ACTIVE_BUFFER   (BuffIndex)
#define INACTIVE_BUFFER (!BuffIndex)

void GLCD_Init(pPic_t pPic, const Int32U * pPallete);
void GLCD_SetPallet (const Int32U * pPallete);
void GLCD_Ctrl (Boolean bEna);
void GLCD_Cursor_Cfg(int Cfg);
void GLCD_Cursor_En(int cursor);
void GLCD_Cursor_Dis(int cursor);
void GLCD_Move_Cursor(int x, int y);
void GLCD_Copy_Cursor (const Int32U *pCursor, int cursor, int size);
void GLCD_SetFont(pFontType_t pFont, LdcPixel_t Color, LdcPixel_t BackgndColor);
void GLCD_SetWindow(Int32U X_Left, Int32U Y_Up,
                    Int32U X_Right, Int32U Y_Down);
void GLCD_TextSetPos(Int32U X, Int32U Y);
void GLCD_TextSetTabSize(Int32U Size);
static void LCD_SET_WINDOW (Int32U X_Left, Int32U X_Right,
                            Int32U Y_Up, Int32U Y_Down);
static void LCD_WRITE_PIXEL (Int32U Pixel);
static Boolean GLCD_TextCalcWindow (pInt32U pXL, pInt32U pXR,
                                    pInt32U pYU, pInt32U pYD,
                                    pInt32U pH_Size, pInt32U pV_Size);
pPic_t GLCD_LoadPic (bmp_t * pBmp);
void GLCD_ShowPic (Int32S X, Int32S Y, pPic_t pPic, Int32U Mask);
void GLCD_FreePic (pPic_t pPic);
void GLCD_print (const char *fmt, ...);

void GLCD_ShowPic_in_Box (pPic_t pPic, Point UL, Point DR, Int32U Mask, int ClearFlag);
void Switch_Buffers (void);
void Switch_to_Active_Buffer (void);
void Switch_to_Inactive_Buffer (void);
void Synchronize_Buffers (void);

#endif // __GLCD_DRV_H
