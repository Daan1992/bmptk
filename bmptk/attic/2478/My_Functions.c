#include "includes.h"
#include "My_Functions.h"

int Max (int A, int B)
{
  if (A>B)
    return A;
  else
    return B;
}

int Min (int A, int B)
{
  if (A<B)
    return A;
  else
    return B;
}

int Round (float x)
{
  int Result;
  x = x + 0.5;
  Result = (int) x;
  return Result;
}

int My_StrCmp (char str1[], char str2[], int Case_Sens)
{
  int i;
  if (Case_Sens)
  {
    i = strcmp (str1, str2);
    if (i==0)
      return 0;
    else
      if (i>0)
        return 1;
      else
        return -1;
  }
  else
  {
    char tmp1[20], tmp2[20];
    strcpy (tmp1, str1);
    strcpy (tmp2, str2);
    for (i=0; tmp1[i]; i++)
      if ((tmp1[i] >= 'a' && tmp1[i] <= 'z') || (tmp1[i] >= 'A' && tmp1[i] <= 'Z'))
      tmp1[i] = tmp1[i] | 0x20;
    for (i=0; tmp2[i]; i++)
      if ((tmp2[i] >= 'a' && tmp2[i] <= 'z') || (tmp2[i] >= 'A' && tmp2[i] <= 'Z'))
      tmp2[i] = tmp2[i] | 0x20;
    i = strcmp (tmp1, tmp2);
    if (i==0)
      return 0;
    else
      if (i>0)
        return 1;
      else
        return -1;
  }
}

void String_Copy_From_To (char Destination[], char Source[], int First, int Last)
{
  int i;
  for (i=First; i<=Last && Source[i]; i++)
    Destination[i-First] = Source[i];
  Destination[i-First] = 0;
}

long int Convert_String_to_Int (char Str[])
{
  long int Temp=0;
  int i, First=0, Negative_Flag=1, System=10, Digit;
  
  // check for negative value
  if (Str[0] == '-')
  {
    Negative_Flag = -1;
    First = 1;
  }
  
  // check for binary or hexadecimal format
  if (Str[First] == '0' && (Str[First+1] == 'b' || Str[First+1] == 'B' || Str[First+1] == 'x' || Str[First+1] == 'X'))
  {
    if (Str[First+1] == 'b' || Str[First+1] == 'B')
      System = 2;
    else
      System = 16;
    First = First + 2;
  }
  
  // convert string to binary/decimal/hexadecimal
  switch (System)
  {
  case 2:
    for (i=First; Str[i]; i++)
    {
      Digit = Str[i];
      if (Digit >= '0' && Digit<= '1')
        Temp = Temp*2 + (Digit & 0x0F);
    }
    break;
  case 10:
    for (i=First; Str[i]; i++)
    {
      Digit = Str[i];
      if (Digit >= '0' && Digit<= '9')
        Temp = Temp*10 + (Digit & 0x0F);
    }
    break;
  case 16:
    for (i=First; Str[i]; i++)
    {
      Digit = Str[i];
      if (Digit >= '0' && Digit <= '9')
        Digit = Digit - '0';
      if (Digit >= 'a' && Digit <= 'f')
        Digit = Digit - 'a' + 10;
      if (Digit >= 'A' && Digit <= 'F')
        Digit = Digit - 'A' + 10;
      Temp = Temp * 16 + Digit;
    }
    break;
  }
  
  return Temp*Negative_Flag;
}

void Error (void)
{
  int prev_secs;
  // toggle led every second

  while (BUT1_FIO & BUT1_MASK)
  {
    if (prev_secs != SEC_bit.SEC)
    {
      prev_secs = SEC_bit.SEC;
      if (prev_secs % 2)
        USB_D_LINK_LED_FCLR = USB_D_LINK_LED_MASK;
      else
        USB_D_LINK_LED_FSET = USB_D_LINK_LED_MASK;
    }
  }
}

void Form_File_Name (MyFile *FileInfo, unsigned char Input[])
{
  int i, end;
  for (end=7; end>=0; end--)
    if (Input[end] != ' ')
      break;
  end++;
  for (i=0; i<end; i++)
    FileInfo->Name[i] = Input[i];
  FileInfo->Name[i] = 0;
  
  for (i=8; Input[i]!=0x20; i++)
    FileInfo->Extension[i-8] = Input[i];
  FileInfo->Extension[i-8] = 0;
  
  if (i == 8)
    sprintf (FileInfo->FullName, "%s", FileInfo->Name);
  else
    sprintf (FileInfo->FullName, "%s.%s", FileInfo->Name, FileInfo->Extension);
  return;
}


ResultValue Is_Inside_Point (int X0, int Y0, int X1, int Y1, int X2, int Y2, int X3, int Y3)
{
  LineEquation L1, L2;
  FPoint PA, PB, PC;
  int PAX, PAY, PBX, PBY, PCX, PCY;
  if (Line_Through_Two_Points (X0, Y0, X1, Y1, &L1) || Line_Through_Two_Points (X2, Y2, X3, Y3, &L2))
    return OUTSIDE_POINT;
  
  if (Intersection (L1, L2, &PA))
    return OUTSIDE_POINT;
  
  if (Line_Through_Two_Points (X0, Y0, X2, Y2, &L1) || Line_Through_Two_Points (X1, Y1, X3, Y3, &L2))
    return OUTSIDE_POINT;
  
  if (Intersection (L1, L2, &PB))
    return OUTSIDE_POINT;
  
  if (Line_Through_Two_Points (X0, Y0, X3, Y3, &L1) || Line_Through_Two_Points (X1, Y1, X2, Y2, &L2))
    return OUTSIDE_POINT;
  
  if (Intersection (L1, L2, &PC))
    return OUTSIDE_POINT;
  
  PAX = Round(PA.X);
  PAY = Round(PA.Y);
  PBX = Round(PB.X);
  PBY = Round(PB.Y);
  PCX = Round(PC.X);
  PCY = Round(PC.Y);

  if (
      PAX < Min(X2, X3) || PAY < Min(Y2, Y3) || PAX > Max(X2, X3) || PAY > Max (Y2, Y3) ||
      PBX < Min(X1, X3) || PBY < Min(Y1, Y3) || PBX > Max(X1, X3) || PBY > Max (Y1, Y3) ||
      PCX < Min(X1, X2) || PCY < Min(Y1, Y2) || PCX > Max(X1, X2) || PCY > Max (Y1, Y2)
      )
    return OUTSIDE_POINT;
                    
  return OK;
}

ResultValue Line_Through_Two_Points (int X1, int Y1, int X2, int Y2, LineEquation *Result)
{
  if (X1 == X2 && Y1 == Y2) // the points match ==> no line at all
  {
    Result->A = POINT;
    return MATCH;
  }
    
  if (X1 == X2) // line is vertical
  {
    Result->A = VERTICAL;
    Result->B = X1;
    return OK;
  }
    
  Result->A = (float)(Y2-Y1)/(X2-X1);
  Result->B = (float)(Y2-Y1)/(X1-X2) * X1 + Y1;
  return OK;
}

ResultValue Intersection (LineEquation L1, LineEquation L2, FPoint *Result)
{
  if (L1.A == POINT || L2.A == POINT)
    return NO_LINE;
  if (L1.A == L2.A && L1.B == L2.B)   // the lines match
    return MATCH;

  if (L1.A == L2.A)   // parallel lines and eventually both vertical
    return PARALLEL;         // no intersection
  
  if (L1.A == VERTICAL)   // L1 is vertical
  {
    Result->X = L1.B;
    Result->Y = L2.A * Result->X + L2.B;
    return OK;           // intersection point coordinates are kept in the result
  }
  
  if (L2.A == VERTICAL)   // L2 is vertical
  {
    Result->X = L2.B;
    Result->Y = L1.A * Result->X + L1.B;
    return OK;           // intersection point coordinates are kept in the result
  }
  
  // no vertical or parallel lines
  Result->X = (L2.B - L1.B) / (L1.A - L2.A);
  Result->Y = L1.A * Result->X + L1.B;
  return OK;           // intersection point coordinates are kept in the result
}