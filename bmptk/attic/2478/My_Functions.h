#ifndef _MY_FUNCTIONS_H
#define _MY_FUNCTIONS_H

#define MAX_FILE_NAME_LENGTH  20
#define MAX_FILE_EXT_LENGTH   5

#ifndef TRUE
  #define TRUE  1
#endif
#ifndef FALSE
  #define FALSE 0
#endif

#define CASE_SENS     1
#define NO_CASE_SENS  0

#define NEXT      1
#define CURRENT   0
#define PREVIOUS  -1

#define POINT     99999.0
#define VERTICAL  99998.0
enum e_ResultValue
{OK=0, MATCH, PARALLEL, NO_LINE, OUTSIDE_POINT};
typedef enum e_ResultValue ResultValue;

// structure describes a point with X and Y coordinates
struct s_Point
{
  int X, Y;
};
typedef struct s_Point Point;

struct s_FPoint
{
  float X, Y;
};
typedef struct s_FPoint FPoint;

struct s_LineEquation
{
  float A, B;
};
typedef struct s_LineEquation LineEquation;

// structure that contains filename, extension and both combined
struct s_MyFile
{
  char Name[MAX_FILE_NAME_LENGTH];
  char Extension[MAX_FILE_EXT_LENGTH];
  char FullName[MAX_FILE_NAME_LENGTH + MAX_FILE_EXT_LENGTH];
};
typedef struct s_MyFile MyFile;

// common functions
int Max (int A, int B);
int Min (int A, int B);
int Round (float x);
int My_StrCmp (char str1[], char str2[], int Case_Sens);
void String_Copy_From_To (char Destination[], char Source[], int First, int Last);
long int Convert_String_to_Int (char Str[]);

// Simple blinking LED function for error
void Error (void);

// convert the input string into name, extension and both combined
void Form_File_Name (MyFile *FileInfo, unsigned char Input[]);


ResultValue Is_Inside_Point (int X0, int Y0, int X1, int Y1, int X2, int Y2, int X3, int Y3);

ResultValue Line_Through_Two_Points (int X1, int Y1, int X2, int Y2, LineEquation *Result);

ResultValue Intersection (LineEquation L1, LineEquation L2, FPoint *Result);

#endif