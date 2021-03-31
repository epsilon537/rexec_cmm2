/*******************************************************************************************
 *
 *  Definitions used when calling MMBasic Interpreter API Functions from CFunctions
 *  For CMM2 MMBasic V5.06.00
 *
 *  This file is public domain and may be used without license.
 *
 *	V1.5
 *
 ******************************************************************************************/
#define MAXVARLEN           32                      // maximum length of a variable name
#define MAXDIM              5                       // maximum nbr of dimensions to an array
#define MMFLOAT double

//Addresses in the API Table for the pointers to each function

#define Vector_uSec               (*(int*)0x80002a0)       // void uSec(unsigned int us)
#define Vector_putConsole         (*(int*)0x80002a4)       // void putConsole(int c)
#define Vector_getConsole         (*(int*)0x80002a8)       // int getConsole(void)
#define Vector_ExtCfg             (*(int*)0x80002ac)       // void ExtCfg(int pin, int cfg, int option)
#define Vector_ExtSet             (*(int*)0x80002b0)       // void ExtSet(int pin, int val)
#define Vector_ExtInp             (*(int*)0x80002b4)       // int ExtInp(int pin)
#define Vector_PinSetBit          (*(int*)0x80002b8)       // void PinSetBit(int pin, unsigned int offset)
#define Vector_PinRead            (*(int*)0x80002bc)       // int PinRead(int pin)
#define Vector_MMPrintString      (*(int*)0x80002c0)       // void MMPrintString(char* s)
#define Vector_IntToStr           (*(int*)0x80002c4)       // void IntToStr(char *strr, long long int nbr, unsigned int base)
#define Vector_CheckAbort         (*(int*)0x80002c8)       // void CheckAbort(void)
#define Vector_GetMemory          (*(int*)0x80002cc)       // void *GetMemory(size_t msize);
#define Vector_GetTempMemory      (*(int*)0x80002d0)       // void *GetTempMemory(int NbrBytes)
#define Vector_FreeMemory         (*(int*)0x80002d4)       // void FreeMemory(void *addr)
#define Vector_DrawRectangle      *(unsigned int *)(int*)0x80002d8          // void DrawRectangle(int x1, int y1, int x2, int y2, int c)
#define Vector_DrawBitmap         *(unsigned int *)(int*)0x80002dc          // void DrawBitmap(int x1, int y1, int width, int height, int scale, int fg, int bg, unsigned char *bitmap )
#define Vector_DrawLine           (*(int*)0x80002e0)       // void DrawLine(int x1, int y1, int x2, int y2, int w, int c)
#define Vector_FontTable          (*(int*)0x80002e4)       // const unsigned char *FontTable[FONT_NBR]
#define Vector_ExtCurrentConfig   (*(int*)0x80002e8)       // int ExtCurrentConfig[NBRPINS + 1];
#define Vector_HRes               (*(int*)0x80002ec)       // HRes
#define Vector_VRes               (*(int*)0x80002f0)       // VRes
#define Vector_SoftReset          (*(int*)0x80002f4)       // void SoftReset(void)
#define Vector_error              (*(int*)0x80002f8)       // void error(char *msg)
#define Vector_ProgFlash          (*(int*)0x80002fc)       // ProgFlash
#define Vector_vartbl             (*(int*)0x8000300)       // vartbl
#define Vector_varcnt             (*(int*)0x8000304)       // varcnt
#define Vector_DrawBuffer         *(unsigned int *)(int*)0x8000308          // void DrawRectangle(int x1, int y1, int x2, int y2, int c)
#define Vector_ReadBuffer         *(unsigned int *)(int*)0x800030c          // void DrawRectangle(int x1, int y1, int x2, int y2, int c)
#define Vector_FloatToStr         (*(int*)0x8000310)     // convert a float to a string including scientific notation if necessary
#define Vector_ExecuteProgram     (*(int*)0x8000314)       // void ExecuteProgram(char *fname)
#define Vector_CFuncmSec          (*(int*)0x8000318)       // CFuncmSec
#define Vector_CFuncRam           (*(int*)0x800031C)       // StartOfCFuncRam
#define Vector_ScrollLCD          *(unsigned int *)(int*)0x8000320          // void scrollLCD(int lines, int blank)
#define Vector_ScrollBufferV      *(unsigned int *)(int*)0x8000324          // void ScrollBuffV(int lines, int blank)
#define Vector_ScrollBufferH	  *(unsigned int *)(int*)0x8000328          // void ScrollBuffH(int pixels)
#define Vector_DrawBufferFast	  *(unsigned int *)(int*)0x800032C          // void DrawBufferFast(int x1, int y1, int x2, int y2, char* p)
#define Vector_ReadBufferFast	  *(unsigned int *)(int*)0x8000330          // void ReadBufferFast(int x1, int y1, int x2, int y2, char* p)
#define Vector_MoveBuffer		  *(unsigned int *)(int*)0x8000334         // void MoveBuffer(int x1, int y1, int x2, int y2, int w, int h, int flip)
#define Vector_DrawPixel          (*(int*)0x8000338)       	// void DrawPixel(int x, int y, int c)
#define Vector_RoutineChecks	  (*(int*)0x800033C)		// void routinechecks(void)
#define Vector_GetPageAddress     (*(int*)0x8000340)       	// uint32_t GetPageAddress(int page)
#define Vector_mycopysafe         (*(int*)0x8000344)       	// void mycopysafe(void *out, const void *in, int n)
#define Vector_IntToFloat         (*(int*)0x8000348)       	// MMFLOAT IntToFloat(long long int a)
#define Vector_FloatToInt         (*(int*)0x800034C)       	// long long int FloatToInt64(MMFLOAT x)
#define Vector_Option             (*(int*)0x8000350)       	// Option
#define Vector_ReadPageAddress    (*(int*)0x8000354)       	// ReadPageAddress
#define Vector_WritePageAddress   (*(int*)0x8000358)       	// WritePageAddress
#define Vector_Timer   			  (*(int*)0x800035C)       	// uint64_t timer(void)
#define Vector_CFuncInt1          (*(int*)0x8000360)        // CFuncInt1
#define Vector_CFuncInt2          (*(int*)0x8000364)        // CFuncInt2
#define Vector_FastTimer   	      (*(int*)0x8000368)       	// uint64_t fasttimer(void)
#define Vector_TicksPerUsec       (*(int*)0x800036C)       	// ticks_per_microsecond
#define Vector_Map                (*(int*)0x8000370)       	// int map(int cl)
#define Vector_Sine               (*(int*)0x8000374)       	// MMFLOAT sin(MMFLOAT)
#define Vector_VideoColour        (*(int*)0x8000378)       	// VideoColour
#define Vector_DrawCircle         (*(int*)0x800037C)       	// DrawCircle(int x, int y, int radius, int w, int c, int fill, MMFLOAT aspect)
#define Vector_DrawTriangle       (*(int*)0x8000380)       	// DrawTriangle(int x0, int y0, int x1, int y1, int x2, int y2, int c, int fill)
//Macros to call each function.

#define uSec(a)                         ((void (*)(unsigned int )) Vector_uSec) (a)
#define putConsole(a)                   ((void (*)(char)) Vector_putConsole) (a)
#define getConsole()                    ((int (*)(void)) Vector_getConsole) ()
#define ExtCfg(a,b,c)                   ((void (*)(int, int, int)) Vector_ExtCfg) (a,b,c)
#define ExtSet(a,b)                     ((void(*)(int, int)) Vector_ExtSet) (a,b)
#define ExtInp(a)                       ((int(*)(int)) Vector_ExtInp) (a)
#define PinSetBit(a,b)                  ((void(*)(int, int)) Vector_PinSetBit) (a,b)
#define PinRead(a)                      ((int(*)(int)) Vector_PinRead) (a)
#define MMPrintString(a)                ((void (*)(char*)) Vector_MMPrintString) (a)
#define IntToStr(a,b,c)                 ((void (*)(char *, long long int, unsigned int)) Vector_IntToStr) (a,b,c)
#define CheckAbort()                    ((void (*)(void)) Vector_CheckAbort) ()
#define GetMemory(a)                    ((void* (*)(int)) Vector_GetMemory) (a)
#define GetTempMemory(a)                ((void* (*)(int)) Vector_GetTempMemory) (a)
#define FreeMemory(a)                   ((void (*)(void *)) Vector_FreeMemory) (a)
#define DrawRectangle(a,b,c,d,e)        ((void (*)(int,int,int,int,int)) (*(unsigned int *)Vector_DrawRectangle)) (a,b,c,d,e)
#define DrawRectangleVector             (*(unsigned int *)Vector_DrawRectangle)
#define DrawBitmap(a,b,c,d,e,f,g,h)     ((void (*)(int,int,int,int,int,int,int, char*)) (*(unsigned int *)Vector_DrawBitmap)) (a,b,c,d,e,f,g,h)
#define DrawBitmapVector                (*(unsigned int *)Vector_DrawBitmap)
#define DrawLine(a,b,c,d,e,f)           ((void (*)(int,int,int,int,int,int)) Vector_DrawLine) (a,b,c,d,e,f)
#define FontTable                       (void*)((int*)(Vector_FontTable))
#define ExtCurrentConfig                ((int *) Vector_ExtCurrentConfig)
#define HRes                            (*(unsigned int *) Vector_HRes)
#define VRes                            (*(unsigned int *) Vector_VRes)
#define SoftReset()                     ((void (*)(void)) Vector_SoftReset) ()
#define error(a)                        ((void (*)(char *)) Vector_error) (a)
#define ProgFlash                       ((int *) Vector_ProgFlash)
#define vartbl                          (*(struct s_vartbl *) Vector_vartbl)
#define varcnt                          (*(unsigned int *) Vector_varcnt)
#define DrawBuffer(a,b,c,d,e)           ((void (*)(int,int,int,int,char *)) (*(unsigned int *)Vector_DrawBuffer)) (a,b,c,d,e)
#define DrawBufferVector                (*(unsigned int *)Vector_DrawBuffer)
#define ReadBuffer(a,b,c,d,e)           ((void (*)(int,int,int,int,char *)) (*(unsigned int *)Vector_ReadBuffer)) (a,b,c,d,e)
#define ReadBufferVector                (*(unsigned int *)Vector_ReadBuffer)
#define FloatToStr(a,b,c,d,e)           ((void (*)(char *, MMFLOAT, int, int, char)) Vector_FloatToStr) (a,b,c,d,e)
// NOTE:  The argument to RunBasicSub is a string specifying the name of the BASIC subroutine to be executed.
//        It MUST be terminated with TWO null chars.
#define RunBasicSub(a)                  ((void (*)(char *)) Vector_ExecuteProgram) (a)
#define CFuncmSec                       (*(unsigned int *) Vector_CFuncmSec)
#define CFuncRam                        ((int *) Vector_CFuncRam)
#define ScrollLCD(a,b)                  ((void (*)(int, int)) (*(unsigned int *)Vector_ScrollLCD)) (a, b)
#define ScrollLCDVector                 (*(unsigned int *)Vector_ScrollLCD)
#define ScrollBufferV(a,b)              ((void (*)(int, int)) (*(unsigned int *)Vector_ScrollBufferV)) (a, b)
#define ScrollBufferVVector             (*(unsigned int *)Vector_ScrollBufferV)
#define ScrollBufferH(a)                ((void (*)(int)) (*(unsigned int *)Vector_ScrollBufferH)) (a)
#define ScrollBufferHVector             (*(unsigned int *)Vector_ScrollBufferH)
#define DrawBufferFast(a,b,c,d,e)       ((void (*)(int,int,int,int, char*)) (*(unsigned int *)Vector_DrawBufferFast)) (a,b,c,d,e)
#define DrawBufferFastVector            (*(unsigned int *)Vector_DrawBufferFast)
#define ReadBufferFast(a,b,c,d,e)       ((void (*)(int,int,int,int, char*)) (*(unsigned int *)Vector_ReadBufferFast)) (a,b,c,d,e)
#define ReadBufferFastVector            (*(unsigned int *)Vector_ReadBufferFast)
#define MoveBufferFast(a,b,c,d,e,f,g)   ((void (*)(int,int,int,int, int,int,int)) (*(unsigned int *)Vector_MoveBuffer)) (a,b,c,d,e,f,g)
#define MoveBufferFastVector            (*(unsigned int *)Vector_MoveBuffer)
#define DrawPixel(a,b,c)                ((void(*)(int, int, int)) Vector_DrawPixel) (a,b,c)
#define RoutineChecks()                 ((void (*)(void)) Vector_RoutineChecks) ()
#define GetPageAddress(a)               ((int(*)(int)) Vector_GetPageAddress) (a)
#define memcpy(a,b,c)                   ((void (*)(void *, void *, int)) Vector_mycopysafe) (a,b,c)
#define IntToFloat(a)                   ((MMFLOAT (*)(long long)) Vector_IntToFloat) (a)
#define FloatToInt(a)                   ((long long (*)(MMFLOAT)) Vector_FloatToInt) (a)
#define Option                          ({struct option_s *optionstructurepointer; optionstructurepointer=(void *)(unsigned int)Vector_Option;})
#define ReadPageAddress                 (*(unsigned int *) Vector_ReadPageAddress)
#define WritePageAddress                (*(unsigned int *) Vector_WritePageAddress)
#define uSecTimer                       ((unsigned long long (*)(void)) Vector_Timer)
#define CFuncInt1                       (*(unsigned int *) Vector_CFuncInt1)
#define CFuncInt2                       (*(unsigned int *) Vector_CFuncInt2)
#define FastTimer                       ((unsigned long long  (*)(void)) Vector_FastTimer)
#define TicksPerUsec                    (*(unsigned int *) Vector_TicksPerUsec)
#define map(a)							((int(*)(int)) Vector_Map) (a)
#define Sine(a)                         ((MMFLOAT (*)(MMFLOAT)) Vector_Sine) (a)
#define VideoColour                     (*(int *) Vector_VideoColour)
#define DrawCircle(a,b,c,d,e,f,g)       ((void (*)(int,int,int,int,int,int,MMFLOAT)) Vector_DrawCircle) (a,b,c,d,e,f,g)
#define DrawTriangle(a,b,c,d,e,f,g,h)   ((void (*)(int,int,int,int,int,int,int,int)) Vector_DrawTriangle) (a,b,c,d,e,f,g,h)
// the structure of the variable table, passed to the CFunction as a pointer Vector_vartbl which is #defined as vartbl
struct s_vartbl {                               // structure of the variable table
	char name[MAXVARLEN];                       // variable's name
    char type;                                  // its type (T_NUM, T_INT or T_STR)
    char level;                                 // its subroutine or function level (used to track local variables)
    unsigned char size;                         // the number of chars to allocate for each element in a string array
    char dummy;
    int __attribute__ ((aligned (4))) dims[MAXDIM];                     // the dimensions. it is an array if the first dimension is NOT zero
    union u_val{
        MMFLOAT f;                              // the value if it is a float
        long long int i;                        // the value if it is an integer
        MMFLOAT *fa;                            // pointer to the allocated memory if it is an array of floats
        long long int *ia;                      // pointer to the allocated memory if it is an array of integers
        char *s;                                // pointer to the allocated memory if it is a string
    }  __attribute__ ((aligned (8))) val;
} __attribute__ ((aligned (8))) val;

//  Useful macros


// Types used to define a variable in the variable table (vartbl).   Often they are ORed together.
// Also used in tokens and arguments to functions
#define T_NOTYPE       0                            // type not set or discovered
#define T_NBR       0x01                            // number (or float) type
#define T_STR       0x02                            // string type
#define T_INT       0x04                            // 64 bit integer type
#define T_PTR       0x08                            // the variable points to another variable's data
#define T_IMPLIED   0x10                            // the variables type does not have to be specified with a suffix
#define T_CONST     0x20                            // the contents of this variable cannot be changed


//***************************************************************************************************
// Constants and definitions copied from the Micromite MkII and Micromite Plus source
//***************************************************************************************************

//The Option structure
struct option_s {
    char Autorun;
    char Tab;
    char Invert;
    char Listcase;
// 4
    unsigned int RunInRam;
    unsigned int  PIN;
    unsigned int  Baudrate;
    int DefaultFC, DefaultBC;      // the default colours
    short DISPLAY_WIDTH;
    short DISPLAY_HEIGHT;
// 28
    short RTC_Calibrate;
    volatile unsigned char USBPolling;
    unsigned char mode;
// 32
    char Height;
    char Width;
    char Console;
    char DefaultFont;
// 36
    char SerialPullup;
    char fulltime;
    char USBKeyboard;
    char colourmode;
// 40
    unsigned char showstatus;
    char colourmap;
    char SDspeed;
    unsigned char editfont;
// 44
    unsigned char F11Key[24];
    unsigned char F12Key[24];
// 92
    unsigned char rtcdrive;
    unsigned char noLED;
    char dummy[6];
// 100
	short RepeatStart;
	short RepeatRate;
	unsigned char path[24];
// 128
};



// Define the offsets from the PORT address
// these are used by GetPortAddr(a,b)
#define ANSEL               -8
#define ANSELCLR            -7
#define ANSELSET            -6
#define ANSELINV            -5
#define TRIS                -4
#define TRISCLR             -3
#define TRISSET             -2
#define TRISINV             -1
#define PORT                0
#define PORTCLR             1
#define PORTSET             2
#define PORTINV             3
#define LAT                 4
#define LATCLR              5
#define LATSET              6
#define LATINV              7
#define ODC                 8
#define ODCCLR              9
#define ODCSET              10
#define ODCINV              11
#define CNPU                12
#define CNPUCLR             13
#define CNPUSET             14
#define CNPUINV             15
#define CNPD                16
#define CNPDCLR             17
#define CNPDSET             18
#define CNPDINV             19
#define CNCON               20
#define CNCONCLR            21
#define CNCONSET            22
#define CNCONINV            23
#define CNEN                24
#define CNENCLR             25
#define CNENSET             26
#define CNENINV             27
#define CNSTAT              28
#define CNSTATCLR           29
#define CNSTATSET           30
#define CNSTATINV           31

// configurations for an I/O pin
// these are used by ExtCfg(a,b,c)
#define EXT_NOT_CONFIG          0
#define EXT_ANA_IN				1
#define EXT_DIG_IN				2
#define EXT_FREQ_IN				3
#define EXT_PER_IN				4
#define EXT_CNT_IN				5
#define EXT_INT_HI				6
#define EXT_INT_LO				7
#define EXT_DIG_OUT				8
#define EXT_OC_OUT				9
#define EXT_INT_BOTH			10
#define EXT_COM_RESERVED        100                 // this pin is reserved and SETPIN and PIN cannot be used
#define EXT_BOOT_RESERVED       101                 // this pin is reserved at bootup and cannot be used
#define EXT_DS18B20_RESERVED    102                 // this pin is reserved for DS18B20 and cannot be used
