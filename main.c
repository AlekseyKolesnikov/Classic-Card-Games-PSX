#include "pad.h"
#include <sys/types.h>
#include <sys/file.h>
#include <asm.h>
#include <kernel.h>
#include <libetc.h>
#include <libgte.h>
#include <libgpu.h>
#include <libgs.h>
#include <libcd.h>
#include <libsnd.h>
#include <libspu.h>
#include <libmcrd.h>
#include <libmath.h>
#include <rand.h>
#include "hand1.h"
#include "hand2.h"
#include "hand3.h"

#define LOAD_ADDR     0x80010000

// Menu - additional stuff
#define ALPHABET_ADDR 0x80010000
#define PACKMENU_ADDR 0x80013800
#define POLZ_ADDR     0x80016800
//      ->            0x8001A800

// Menu - main stuff
#define MENU_ADDR     0x80010000
#define MENU_G_ADDR   0x80036000
#define MENU_S_ADDR   0x8005C000
#define MM_LOAD_ADDR  0x80082000
#define MM_SAVE_ADDR  0x8008A800
#define AUTHORS_ADDR  0x80093000
#define RECORDS_ADDR  0x800AA000
#define NAME_ADDR     0x800D0000
// For all games and menu
#define HELP_ADDR     0x800F6000
#define HELP_1_ADDR   0x8011C000
#define HELP_2_ADDR   0x80125800
#define HELP_3_ADDR   0x8012F000
//      ->            0x80138800

// For all games
#define ALPHABET_ADDR 0x80010000
#define MC_LOAD_ADDR  0x80013800
#define MC_SAVE_ADDR  0x8001C000
#define BACK_ADDR     0x80024800
#define BLACK_C_ADDR  0x8004A800
#define PACK_ADDR     0x8004F800
#define RED_C_ADDR    0x80053000
#define GM_DRAW_ADDR  0x80058000
#define GM_KING_ADDR  0x80060800
#define GM_LOSE_ADDR  0x80065000
#define GM_SCORE_ADDR 0x8006D800
#define GM_WIN_ADDR   0x80076000
#define GM_WSCR_ADDR  0x8007E800
#define PM_EXIT_ADDR  0x80087000
#define PM_PAUSE_ADDR 0x8008F800
#define PM_RETRY_ADDR 0x80098000
// For all games except Nine
#define BLACK_ADDR    0x800A0800
#define RED_ADDR      0x800A7000
//      ->            0x800AD800

// For Nine
#define BLACK9_ADDR   0x800A0800
#define BLACK9CS_ADDR 0x800A6800
#define RED9_ADDR     0x800A9800
#define RED9CS_ADDR   0x800AF800
//      ->            0x800B2800

#define S1_ADDR       0x80010000
#define S2_ADDR       0x80010800
#define S3_ADDR       0x80011000
#define S4_ADDR       0x80011800
#define S5_ADDR       0x80012800
#define S6_ADDR       0x80019800
#define S7_ADDR       0x8001B800
#define S8_ADDR       0x8001E800
#define S9_ADDR       0x8001F000
//      ->            0x8001F800

#define S1_SIZE   128
#define S2_SIZE  1152
#define S3_SIZE  1040
#define S4_SIZE  4032
#define S5_SIZE 28208
#define S6_SIZE  6896
#define S7_SIZE 10944
#define S8_SIZE  1424
#define S9_SIZE  1632


#define AllBright  0x80

#define cHearts2     0
#define cHearts3     1
#define cHearts4     2
#define cHearts5     3
#define cHearts6     4
#define cHearts7     5
#define cHearts8     6
#define cHearts9     7
#define cHearts10    8
#define cHeartsJ     9
#define cHeartsQ    10
#define cHeartsK    11
#define cHeartsA    12

#define cCross2     13
#define cCross3     14
#define cCross4     15
#define cCross5     16
#define cCross6     17
#define cCross7     18
#define cCross8     19
#define cCross9     20
#define cCross10    21
#define cCrossJ     22
#define cCrossQ     23
#define cCrossK     24
#define cCrossA     25

#define cDiamonds2  26
#define cDiamonds3  27
#define cDiamonds4  28
#define cDiamonds5  29
#define cDiamonds6  30
#define cDiamonds7  31
#define cDiamonds8  32
#define cDiamonds9  33
#define cDiamonds10 34
#define cDiamondsJ  35
#define cDiamondsQ  36
#define cDiamondsK  37
#define cDiamondsA  38

#define cSpades2    39
#define cSpades3    40
#define cSpades4    41
#define cSpades5    42
#define cSpades6    43
#define cSpades7    44
#define cSpades8    45
#define cSpades9    46
#define cSpades10   47
#define cSpadesJ    48
#define cSpadesQ    49
#define cSpadesK    50
#define cSpadesA    51

#define bgLUp      55
#define bgMUp      56
#define bgRUp      57
#define bgHelp1    58
#define bgHelp2    59
//#define bgLDn     58
//#define bgMDn     59
//#define bgRDn     60

#define Ball      61
#define Polz      62

#define cPackV    64
#define cPackH    65
#define cBackV    66
#define Alphabet  67
#define pGame     68
#define pDown     69
#define pSelect   70
#define Auth1     71
#define Auth2     72
#define cPackV1   73
#define cTop      74
#define cTop1     75
#define cTop2     76
#define cTop3     77
#define cTop4     78
#define cTop5     79
#define cTop6     80
#define oCard     81
#define oPAL      84
#define oNTSC     85
#define TheMsg    86

#define mPolz     87
#define mPolz1    87
#define mPolz2    88
#define mPolz3    89
#define mPolz4    90
#define mPolz5    91

#define kTake     90
#define kDontTake 91
#define kKing     92
#define kKing1    93
#define kKing2    94
#define kKing3    95
#define kKing4    96
#define kKing5    97
#define kKing6    98
#define kKing7    99

#define pMenu     181
#define pExit     182
#define pRetry    183
#define wWin      184
#define wLose     185
#define wScore    186
#define wNoOne    187
#define wTheScore 188


#define Loading   100

// Russian letters: 101 -> 133
// Numbers:         140 -> 149
// Latin letters:   151 -> 176
// Small cards:     200 -> 251

#define mHearts   280
#define mCross    281
#define mDiamonds 282
#define mSpades   283


#define cJack     11
#define cQueen    12
#define cKing     13
#define cAce      14

#define cBlack    0x01
#define cRed      0x02
#define cHearts   0x12
#define cCross    0x21
#define cDiamonds 0x42
#define cSpades   0x81

// #define           0x0080
// #define           0x0100
// #define           0x0200
// #define           0x0400
// #define           0x0800
// #define           0x1000
// #define           0x2000
// #define           0x4000
// #define           0x8000

#define gmNine      0
#define gmKing      1
#define gmFool      2
#define gmHearts    3
#define gmBlackJack 4

#define gmSoliter   5
#define gmFreeCell  6
#define gmGolf      7
#define gmFourteen  8


#define OT_LENGTH  10
GsOT WorldOT[2];
GsOT_TAG OTTags[2][1<<OT_LENGTH];

#define PACKETMAX 300
PACKET GpuPacketArea[2][PACKETMAX*24];

/* This should be more than enough */
#define SPRITEMAX 300
GsSPRITE Sprites[SPRITEMAX];

GsIMAGE TexInfo;
RECT rect1;

signed char VMode=0, Dith=1, TheGame;
short NormalBright;
short Name[8];
short Score;
short Priority[SPRITEMAX];
short i, j, k, l, ii;
signed char vBack[7];      // The number of inverted cards (covers)
signed char ReadyNum[4], ListNum[12]; // The number of opened cards
signed char Ready[4][13];  // Decomposed cards (at top)
signed char List[12][36];  // The list of opened cards: List[0] - at the left
                           // List[x][0] - lays on the inverted or at the table
signed char TempList[14];
short TempNum;
signed char PackList[24];  // cards in the deck (top-left)
signed char CurPack;       // the current item in the Pack (top-left)
signed char PackNum;       // the number of cards in the Pack (top-left)
signed char PackLeft;      // how much inverted cards at the top-left
signed char CurDeck;       // the current item in the Deck
signed char SrcPos;        // cursor position (selected card)
short SelectedCard;        // selected card (highlighted)
short DstCard;             // where to place the selected card
short Cursor, CurPos;
signed char CurInc;
struct Cur_ {
       short x, y;
       };
struct Cur_ Cur[36];       // possible cursor position
struct Cur_ mBright[11];   // possible location of the brightness slider
struct Cur_ mMusic[11];    // possible location of the music slider
struct Cur_ mSFX[11];      // possible location of the sfx slider
struct Cur_ Alpha;
signed char pBright=4, pMusic=7, pSFX=7;
struct Deck_ {
       short Value, Suit;
       } SrcDeck[52];
signed char Deck[52], TempDeck[52];
u_long PadData, TheData, LastData;
signed char Player, StartPlayer, StartSuit, SelectedSuit, BUplayer;
signed char PlayerCards[4][4][13];  //[Player][Suit][Value]
signed char PlayerSuit[4];          //[Suit] 0 - yes, 1 - no
signed char PlayerNotTake[4];       //[Suit] 0 - yes, 1 - no
signed char DONTtake[14];           //cards with points
signed char DTnumber;               //and the number of them
short SuitPriority[4];              //[Suit] suit priority
short En;
signed char Danger1, Danger2;       //danger suits
signed char GameNumber;             //game number
signed char DisableSuit;            //a suit from which you can not start until it tossed
signed char DisableCard;            //a card from which you can not start at the moment
signed char Drain;                  //dropping cards
signed char CardOnHand;             //cards in the hand (reamains)
short Scores[4];                    //scores
signed char Cheater=0, Bonus=0, QuitFlag, Winner, EndPos, FirstRun=0;

signed char Suit, Pl, MaxE, MinP, BeCare[4], TrumpSuit, CurCard;
short MinE, MaxP;
short StartNum;
short ai, bi;
short NextCard, FreeCells;
short Start_x, Start_y, End_x, End_y;
signed char Animate=0;
short MyMsg=-1;


// Sound
int pitch=0x400;
int cdVol=127;
int tracks[11];
u_long sample[10];
#define NSPUMALLOCS 64
char spuheap[SPU_MALLOC_RECSIZ * (NSPUMALLOCS + 1)];


// Memory card
typedef struct {
    long        id;
    long        flag;
    long        cbnum;
    short       cx;
    short       cy;
    short       cw;
    short       ch;
    char        clut[32];
    long        pbnum;
    short       px;
    short       py;
    short       pw;
    short       ph;
    char        image[2];
} _TIM4;

typedef struct {
        char    Magic[2];
        char    Type;
        char    BlockEntry;
        char    Title[64];
        char    reserve[28];
        char    Clut[32];
        char    Icon[3][128];

        signed  char VMode, Inter, CardBright, MusicVol, SFXVol;
        short   Name[8];
        short   NameNine[8],    ScoreNine;
        short   NameKing[8],    ScoreKing;
        short   NameHearts[8],  ScoreHearts;
        short   NameSoliter[8], ScoreSoliter;
        char    Dummy[43];
} _CARD;
static _CARD dummy1HEAD;
static _CARD HEAD;
static _CARD dummy2HEAD;



main()
{
        // Common init process
        ResetCallback();
        CdInit();
        //_96_init();             //[Data file reading]

        // Sound init
        cd_read_sub("\\DATA\\SOUND\\S1.VB;1", (void *)S1_ADDR);
        cd_read_sub("\\DATA\\SOUND\\S2.VB;1", (void *)S2_ADDR);
        cd_read_sub("\\DATA\\SOUND\\S3.VB;1", (void *)S3_ADDR);
        cd_read_sub("\\DATA\\SOUND\\S4.VB;1", (void *)S4_ADDR);
        cd_read_sub("\\DATA\\SOUND\\S5.VB;1", (void *)S5_ADDR);
        cd_read_sub("\\DATA\\SOUND\\S6.VB;1", (void *)S6_ADDR);
        cd_read_sub("\\DATA\\SOUND\\S7.VB;1", (void *)S7_ADDR);
        cd_read_sub("\\DATA\\SOUND\\S8.VB;1", (void *)S8_ADDR);
        cd_read_sub("\\DATA\\SOUND\\S9.VB;1", (void *)S9_ADDR);
        InitMySound();
        cd_read_sub("\\DATA\\MENU\\GINTRO.TIM;1", (void *)LOAD_ADDR);

        // Init video mode
        //SetVideoMode(VMode);
        ResetGraph(0);
        GsInitGraph(640, 240, GsOFSGPU, 0, 0);
        GsDefDispBuff(0,0,0,240);
        GsInit3D();

        WorldOT[0].length=OT_LENGTH;
        WorldOT[0].org=OTTags[0];
        WorldOT[1].length=OT_LENGTH;
        WorldOT[1].org=OTTags[1];

        // Shows the first intro
        PlayIt(sample[6]);
        InitBack(LOAD_ADDR);
        Fade_In();
        SayIntro();

        // Init additional graphics
        cd_read_sub("\\DATA\\MENU\\LOAD.TIM;1",    (void *)LOAD_ADDR);
        InitLoad(LOAD_ADDR);
        // Help background
        cd_read_sub("\\DATA\\MENU\\HELP.TIM;1",    (void *)HELP_ADDR);

        // Init CD-music parameters
        for (i=0;i<11;i++) tracks[i]=i+2;
        // sets main vol (left and right channels) vol can be 0 to 127
        SsSetMVol( 127, 127 );
        SsSetMute(SS_MUTE_OFF);                  // make sure mute is not on
        // sets serial sound attributes (serial_a is music direct from cd
        SsSetSerialAttr(SS_SERIAL_A, SS_MIX, SS_SON);         //  player)
        cdVol=pMusic*12;
        SsSetSerialVol( SS_SERIAL_A, cdVol, cdVol ); // sets serial volume

        // Initialization of priorities
        for (i=0; i<SPRITEMAX; i++) {
            Priority[i]=100;
        }
        for (i=bgLUp; i<=bgRUp; i++) {
            Priority[i]=30;         // background
        }
        Priority[Ball]=10;          // menu cursor
        for (i=cHearts2; i<=cSpadesA; i++) {
            Priority[i]=15;         // cards
        }
        Priority[cPackV]=23;        // pack
        Priority[cPackH]=23;        // pack
        Priority[cBackV]=23;        // card cover
        Priority[pDown]=5;          // cursor
        Priority[pGame]=5;          // cursor
        Priority[pSelect]=5;        // cursor
        Priority[Loading]=5;        // loading
        for (i=101; i<181; i++) {   // Russian letters: 101 -> 133
            Priority[i]=20;         // Numbers:         140 -> 149
        }                           // Latin letters:   151 -> 176

        // Init system variables
        InitDeck();     // deck
        Name[0]=110;    Name[1]=104;    Name[2]=118;    Name[3]=116;
        Name[4]=112;    Name[5]=180;    Name[6]=180;    Name[7]=180;
        CurPos=0;

        // Init MC save parameters
        HEAD.NameNine[0]=0;
        HEAD.NameKing[0]=0;
        HEAD.NameHearts[0]=0;
        HEAD.NameSoliter[0]=0;
        HEAD.ScoreNine=0;
        HEAD.ScoreKing=0;
        HEAD.ScoreHearts=1000;
        HEAD.ScoreSoliter=0;
        for (i=0;i<8;i++) HEAD.Name[i]=Name[i];

        // Init menu graphics
        InitMenuStuff();

        // Init MC and read data
        MemCardInit(1);
        MemCardStart();
        InitMsg(MM_LOAD_ADDR); SayMsg();
        ReadCard();
        CheckBonus();
        NormalBright=0x3C+4*pBright;

        InitMenu();
        Fade_Out();

        if (VMode!=0) {
           ResetGraph(0);
           SetVideoMode(VMode);
           ResetGraph(0);
           GsInitGraph(640, 240, GsOFSGPU, Dith, 0);
           GsDefDispBuff(0,0,0,240);
           GsInit3D();
        }

        InitMainMenu();
        Fade_In();

        CdPlay( 2, tracks, 0);  //plays tracks[i] of cd

        PadInit(0);
        PadData=0;
        TheData=0;
        LastData=0;
        Cursor=mPolz;
        CurInc=1;

        /* Main loop */
        while ( 1 ) {
              MenuSelect();
              Draw_Menu();

              if ((PadData & PADRdown)>0) {
                 PlayIt(sample[8]);
                 Fade_Out();
                 switch (CurPos) {
                   case 0: // Games
                           Run_G_Menu();
                           CurPos=0;
                           break;
                   case 1: // Solitaires
                           Run_S_Menu();
                           CurPos=1;
                           break;
                   case 2: // Options
                           SayLoad();
                           Run_Options();
                           SayLoad();
                           InitMenu();
                           CdPlay( 2, tracks, 0);  //plays tracks[0] of cd
                           CurPos=2;
                           break;
                   case 3: // Records
                           Run_Records();
                           PadData=0;
                           CurPos=3;
                           break;
                 }
                 PlayIt(sample[8]);
                 InitMainMenu();
                 Fade_In();
              }

              if ((PadData & PADL1)>0) {
                 Run_Help();
                 InitMainMenu();
              }
        }
}

Run_G_Menu()
{
        InitMenu_G();
        Fade_In();
        CurPos=0;
        /* Main loop */
        while ( 1 ) {
              MenuSelect();
              Draw_Menu();

              if ((PadData & PADRup)>0) {
                 Fade_Out();
                 return 0;
              }
              if ((PadData & PADRdown)>0) {
                 PadData=0;
                 if (FirstRun==1) goto SkipAskName;
                 PlayIt(sample[8]);
                 Fade_Out();
                 if (Ask_Name()==1) {
                    Fade_Out();
                    for (i=0;i<8;i++) Name[i]=HEAD.Name[i];
                    PadData=0;
                    goto SkipRun;
                 }
                 PadData=0;
                 FirstRun=1;
                 j=0;
                 for (i=0;i<8;i++) if (Name[i]!=HEAD.Name[i]) j=1;
                 if ((Name[0]==152) && (Name[1]==155) && (Name[2]==170) &&
                     (Name[3]==151) && (Name[4]==180) && (Name[5]==180) &&
                     (Name[6]==180) && (Name[7]==180)) {
                    Cheater=1;
                    j=0;
                 }
                 if (j!=0) {
                    for (i=0;i<8;i++) HEAD.Name[i]=Name[i];
                    InitMsg(MM_SAVE_ADDR); SayMsg();
                    WriteCard();
                 }

              SkipAskName:
                 PlayIt(sample[6]);
                 Fade_Out();
                 SayLoad();
                 TheGame=CurPos;
                 switch (CurPos) {
                   case 0: Run_Nine();
                           CurPos=0;
                           break;
                   case 1: Run_King();
                           CurPos=1;
                           break;
                   case 2: Run_Fool();
                           CurPos=2;
                           break;
                   case 3: Run_Hearts();
                           CurPos=3;
                           break;
                   case 4: Run_BJ();
                           CurPos=4;
                           break;
                 }
                 SayLoad();
                 InitMenuStuff();
                 InitMenu();
                 CdPlay( 2, tracks, 0);  //plays tracks[0] of cd

              SkipRun:
                 PlayIt(sample[8]);
                 InitMenu_G();
                 Fade_In();
                 Cursor=mPolz;
                 CurInc=1;
              }

              if ((PadData & PADL1)>0) {
                 Run_Help();
                 InitMenu_G();
              }
        }
}

Run_S_Menu()
{
        InitMenu_S();
        Fade_In();
        CurPos=0;
        /* Main loop */
        while ( 1 ) {
              MenuSelect();
              Draw_Menu();

              if ((PadData & PADRup)>0) {
                 Fade_Out();
                 return 0;
              }
              if ((PadData & PADRdown)>0) {
                 PadData=0;
                 if (FirstRun==1) goto SkipAskName;
                 PlayIt(sample[8]);
                 Fade_Out();
                 if (Ask_Name()==1) {
                    Fade_Out();
                    for (i=0;i<8;i++) Name[i]=HEAD.Name[i];
                    PadData=0;
                    goto SkipRun;
                 }
                 PadData=0;
                 FirstRun=1;
                 j=0;
                 for (i=0;i<8;i++) if (Name[i]!=HEAD.Name[i]) j=1;
                 if ((Name[0]==152) && (Name[1]==155) && (Name[2]==170) &&
                     (Name[3]==151) && (Name[4]==180) && (Name[5]==180) &&
                     (Name[6]==180) && (Name[7]==180)) {
                    Cheater=1;
                    j=0;
                 }
                 if (j!=0) {
                    for (i=0;i<8;i++) HEAD.Name[i]=Name[i];
                    InitMsg(MM_SAVE_ADDR); SayMsg();
                    WriteCard();
                 }

              SkipAskName:
                 PlayIt(sample[6]);
                 Fade_Out();
                 SayLoad();
                 TheGame=CurPos+5;
                 switch (CurPos) {
                   case 0: Run_Soliter();
                           CurPos=0;
                           break;
                   case 1: Run_FreeCell();
                           CurPos=1;
                           break;
                   case 2: Run_Golf();
                           CurPos=2;
                           break;
                   case 3: Run_Fourteen();
                           CurPos=3;
                           break;
                 }
                 SayLoad();
                 InitMenuStuff();
                 InitMenu();
                 CdPlay( 2, tracks, 0);  //plays tracks[0] of cd

              SkipRun:
                 PlayIt(sample[8]);
                 InitMenu_S();
                 Fade_In();
                 Cursor=mPolz;
                 CurInc=1;
              }

              if ((PadData & PADL1)>0) {
                 Run_Help();
                 InitMenu_S();
              }
        }
}


WriteCard()
{
        long chan;
        unsigned long rslt;

        HEAD.VMode=VMode;
        HEAD.CardBright=pBright;
        HEAD.MusicVol=pMusic;
        HEAD.SFXVol=pSFX;
        //for (i=0;i<8;i++) HEAD.Name[i]=Name[i];

        /* port number */
        chan = 0x00;

        /* test for card connection */
        MemCardAccept(chan);

        /* wait for MemCardAccept to finish */
        MemCardSync(0,0,&rslt);

        /* error handling */
        if(rslt!=McErrNone && rslt!=McErrNewCard) {
                return 0;
        }

        /* create file */
        rslt =MemCardCreateFile( chan, "BESLES-00000", 1 );
        if( rslt!=McErrNone ) {
                if (rslt==McErrAlreadyExist) {
                   MemCardDeleteFile(chan, "BESLES-00000" );
                   rslt =MemCardCreateFile( chan, "BESLES-00000", 1 );
                   if( rslt!=McErrNone ) return 0;
                } else {
                   return 0;
                }
        }

        /* prepare file header */
        HEAD.Magic[0] = 'S';
        HEAD.Magic[1] = 'C';
        HEAD.Type = 0x11;
        HEAD.BlockEntry = 1;

        strcpy( HEAD.Title, "ClassicCard2" );

        memcpy( HEAD.Clut, ((_TIM4*)hand1)->clut, 32 );
        memcpy( HEAD.Icon[0], ((_TIM4*)hand1)->image, 128);
        memcpy( HEAD.Icon[1], ((_TIM4*)hand2)->image, 128);
        memcpy( HEAD.Icon[2], ((_TIM4*)hand3)->image, 128);

        /* write file data */
        rslt=sizeof(HEAD);
        if ((rslt%128)!=0) rslt=(rslt/128+1)*128;
        MemCardWriteFile( chan, "BESLES-00000", (long*)&HEAD, 0, rslt);

        /* wait for finish */
        MemCardSync(0,0,&rslt);

        return 0;
}

ReadCard()
{
        SpuVoiceAttr s_attr;

        long chan;
        unsigned long rslt;

        /* port number */
        chan = 0x00;

        /* test for card connection */
        MemCardAccept(chan);

        /* wait for MemCardAccept to finish */
        MemCardSync(0,0,&rslt);

        /* error handling */
        if(rslt!=McErrNone && rslt!=McErrNewCard) {
                return 0;
        }

        /* create file
        rslt =MemCardOpen( chan, "HAND", 0 );
        if( rslt!=McErrNone ) return 0;
        */

        /* write file data */
        rslt=sizeof(HEAD);
        if ((rslt%128)!=0) rslt=(rslt/128+1)*128;
        MemCardReadFile( chan, "BESLES-00000", (long*)&HEAD, 0, rslt);

        /* wait for finish */
        MemCardSync(0,0,&rslt);
        if(rslt!=McErrNone) return 0;

        VMode=HEAD.VMode;
        pBright=HEAD.CardBright;
        pMusic=HEAD.MusicVol;
        pSFX=HEAD.SFXVol;
        for (i=0;i<8;i++) Name[i]=HEAD.Name[i];
        cdVol=pMusic*12;
        SsSetSerialVol( SS_SERIAL_A, cdVol, cdVol ); // sets serial volume

        // items of voice attribute
        s_attr.mask = (SPU_VOICE_VOLL |
                       SPU_VOICE_VOLR |
                       SPU_VOICE_PITCH |
                       SPU_VOICE_WDSA |
                       SPU_VOICE_ADSR_AMODE |
                       SPU_VOICE_ADSR_SMODE |
                       SPU_VOICE_ADSR_RMODE |
                       SPU_VOICE_ADSR_AR |
                       SPU_VOICE_ADSR_DR |
                       SPU_VOICE_ADSR_SR |
                       SPU_VOICE_ADSR_RR |
                       SPU_VOICE_ADSR_SL
                       );

        // set the attributes with all voice
        s_attr.voice = SPU_ALLCH;

        // value of each voice attribute
        s_attr.volume.left = 1638*pSFX;                // Left volume
        s_attr.volume.right = 1638*pSFX;               // Right volume
        s_attr.pitch = pitch;                       // Pitch
        s_attr.addr = NULL;                 // Waveform data start address
        s_attr.a_mode       = SPU_VOICE_LINEARIncN; // Attack curve
        s_attr.s_mode       = SPU_VOICE_LINEARIncN; // Sustain curve
        s_attr.r_mode       = SPU_VOICE_LINEARDecN; // Release curve
        s_attr.ar = 0x0;                            // Attack rate value
        s_attr.dr = 0x0;                            // Decay rate value
        s_attr.sr = 0x0;                            // Sustain rate value
        s_attr.rr = 0x0;                            // Release rate value
        s_attr.sl = 0xf;                            // Sustain level value

        SpuSetVoiceAttr(&s_attr);

        return 0;
}


InitMySound()
{
    u_long addr;
    u_long size;
    VabHdr *vag;
    SpuCommonAttr c_attr;
    SpuVoiceAttr s_attr;


    SpuInit();                           // Initialize SPU
    SpuInitMalloc(NSPUMALLOCS, spuheap); // Init SPU heap

    c_attr.mask = (SPU_COMMON_MVOLL | SPU_COMMON_MVOLR);
    c_attr.mvol.left  = 0x3fff; // Master volume (left)
    c_attr.mvol.right = 0x3fff; // Master volume (right)
    SpuSetCommonAttr(&c_attr);

    SpuSetTransferMode(SpuTransByDMA);// select transfer method -> DMA!

    // Calculate the size of all samples ...
    size=S1_SIZE+S2_SIZE+S3_SIZE+S4_SIZE+S5_SIZE+S6_SIZE+S7_SIZE+S8_SIZE+S9_SIZE;

    addr=SpuMalloc(size);  // Allocate SPU memory

    SpuSetTransferStartAddr(addr);

    // Transferring samples
    SpuWritePartly((char *)S1_ADDR, S1_SIZE);
    sample[0]=addr;
    SpuIsTransferCompleted(SPU_TRANSFER_WAIT); // Wait for transfer
    addr+=S1_SIZE;

    SpuWritePartly((char *)S2_ADDR, S2_SIZE);
    sample[1]=addr;
    SpuIsTransferCompleted(SPU_TRANSFER_WAIT); // Wait for transfer
    addr+=S2_SIZE;

    SpuWritePartly((char *)S3_ADDR, S3_SIZE);
    sample[2]=addr;
    SpuIsTransferCompleted(SPU_TRANSFER_WAIT); // Wait for transfer
    addr+=S3_SIZE;

    SpuWritePartly((char *)S4_ADDR, S4_SIZE);
    sample[3]=addr;
    SpuIsTransferCompleted(SPU_TRANSFER_WAIT); // Wait for transfer
    addr+=S4_SIZE;

    SpuWritePartly((char *)S5_ADDR, S5_SIZE);
    sample[4]=addr;
    SpuIsTransferCompleted(SPU_TRANSFER_WAIT); // Wait for transfer
    addr+=S5_SIZE;

    SpuWritePartly((char *)S6_ADDR, S6_SIZE);
    sample[5]=addr;
    SpuIsTransferCompleted(SPU_TRANSFER_WAIT); // Wait for transfer
    addr+=S6_SIZE;

    SpuWritePartly((char *)S7_ADDR, S7_SIZE);
    sample[6]=addr;
    SpuIsTransferCompleted(SPU_TRANSFER_WAIT); // Wait for transfer
    addr+=S7_SIZE;

    SpuWritePartly((char *)S8_ADDR, S8_SIZE);
    sample[7]=addr;
    SpuIsTransferCompleted(SPU_TRANSFER_WAIT); // Wait for transfer
    addr+=S8_SIZE;

    SpuWritePartly((char *)S9_ADDR, S9_SIZE);
    sample[8]=addr;
    SpuIsTransferCompleted(SPU_TRANSFER_WAIT); // Wait for transfer


    // items of voice attribute
    s_attr.mask = (SPU_VOICE_VOLL |
                   SPU_VOICE_VOLR |
                   SPU_VOICE_PITCH |
                   SPU_VOICE_WDSA |
                   SPU_VOICE_ADSR_AMODE |
                   SPU_VOICE_ADSR_SMODE |
                   SPU_VOICE_ADSR_RMODE |
                   SPU_VOICE_ADSR_AR |
                   SPU_VOICE_ADSR_DR |
                   SPU_VOICE_ADSR_SR |
                   SPU_VOICE_ADSR_RR |
                   SPU_VOICE_ADSR_SL
                   );

    // set the attributes with all voice
    s_attr.voice = SPU_ALLCH;

    // value of each voice attribute
    s_attr.volume.left = 0x3fff;                // Left volume
    s_attr.volume.right = 0x3fff;               // Right volume
    s_attr.pitch = pitch;                       // Pitch
    s_attr.addr = NULL;                 // Waveform data start address
    s_attr.a_mode       = SPU_VOICE_LINEARIncN; // Attack curve
    s_attr.s_mode       = SPU_VOICE_LINEARIncN; // Sustain curve
    s_attr.r_mode       = SPU_VOICE_LINEARDecN; // Release curve
    s_attr.ar = 0x0;                            // Attack rate value
    s_attr.dr = 0x0;                            // Decay rate value
    s_attr.sr = 0x0;                            // Sustain rate value
    s_attr.rr = 0x0;                            // Release rate value
    s_attr.sl = 0xf;                            // Sustain level value

    SpuSetVoiceAttr(&s_attr);
}

PlayIt(u_long addr)
{
    SpuVoiceAttr s_attr;

    s_attr.mask = SPU_VOICE_PITCH | SPU_VOICE_WDSA;
    s_attr.addr = addr;
    s_attr.voice = SPU_VOICECH(0);
    s_attr.pitch = pitch;
    SpuSetKeyOnWithAttr(&s_attr);
}


short CurName;

Ask_Name()
{
        InitBack(NAME_ADDR);
        Fade_In();

        Alpha.x=0;    Alpha.y=0;

        CurName=-1;
        for (i=0;i<8;i++) if (Name[i]<180) CurName=i;
        CurName++;      // the current input position
        Draw_AskName();

        while ( 1 ) {
              if ((PadData & PADLdown)>0) {
                 PlayIt(sample[7]);
                 Alpha.y++;
                 if (Alpha.y>7) Alpha.y=7;
                 Draw_AskName();
              }
              if ((PadData & PADLup)>0) {
                 PlayIt(sample[7]);
                 Alpha.y--;
                 if (Alpha.y<0) Alpha.y=0;
                 Draw_AskName();
              }
              if ((PadData & PADLright)>0) {
                 PlayIt(sample[7]);
                 Alpha.x++;
                 if (Alpha.x>9) Alpha.x=9;
                 Draw_AskName();
              }
              if ((PadData & PADLleft)>0) {
                 PlayIt(sample[7]);
                 Alpha.x--;
                 if (Alpha.x<0) Alpha.x=0;
                 Draw_AskName();
              }

              if ((PadData & PADRright)>0) {
                 if (CurName<8) {
                    PlayIt(sample[7]);
                    Name[CurName]=Alpha.y*10+Alpha.x+101;
                    CurName++;
                    Draw_AskName();
                 }
              }
              if ((PadData & PADRleft)>0) {
                 if (CurName>0) {
                    PlayIt(sample[7]);
                    CurName--;
                    Name[CurName]=180;
                    Draw_AskName();
                 }
              }

              ReadKey();

              if ((PadData & PADstart)>0) return 0;
              if ((PadData & PADRup)>0) return 1;
        }
}


Run_Records()
{
        InitBack(RECORDS_ADDR);
        Fade_In();
        Draw_Records();
        while ( 1 ) {
              ReadKey();
              if (((PadData & PADRdown)>0)||((PadData & PADRup)>0)) {
                 PlayIt(sample[8]);
                 return 0;
              }
        }
}


Run_Options()
{
        cd_read_sub("\\DATA\\MENU\\OPTIONS.TIM;1", (void *)MENU_ADDR);
        InitBack(MENU_ADDR);
        cd_read_sub("\\DATA\\MENU\\ABOUT.TIM;1", (void *)MENU_G_ADDR);
        CdPlay( 2, tracks, 1);  //plays tracks[i] of cd
        Fade_In();
        CurPos=0;

        Cur[0].x=100;
        Cur[0].y=68;
        Cur[1].x=100;
        Cur[1].y=88;
        Cur[2].x=100;
        Cur[2].y=140;
        Cur[3].x=100;
        Cur[3].y=160;
        Cur[4].x=232;
        Cur[4].y=180;

        for (i=0;i<11;i++) {
            mBright[i].x=215+i*19;
            mBright[i].y=104;
            mMusic[i].x=305+i*19;
            mMusic[i].y=142;
            mSFX[i].x=305+i*19;
            mSFX[i].y=162;
        }
        MyMsg=-1;

        Draw_Options();

        while ( 1 ) {
              Options_Select();
              if ((PadData & PADRup)>0) {
                 InitMsg(MM_SAVE_ADDR); SayMsg();
                 WriteCard();
                 Fade_Out();
                 return 0;
              }
        }

}


SpuVoiceAttr s_attr;

Options_Select()
{
        /*if ((PadData & PADR2)>0) {         // dithering on/off
           if (Dith==0) Dith=1;
           else         Dith=0;
           ResetGraph(0);
           SetVideoMode(VMode);
           ResetGraph(0);
           GsInitGraph(640, 240, GsOFSGPU, Dith, 0);
           GsDefDispBuff(0,0,0,240);
           GsInit3D();
        }*/
        if ((PadData & PADLdown)>0) {
           CurPos++;
           if (CurPos>4) CurPos=4;
           else PlayIt(sample[2]);
           Draw_Options();
        }
        if ((PadData & PADLup)>0) {
           CurPos--;
           if (CurPos<0) CurPos=0;
           else PlayIt(sample[2]);
           Draw_Options();
        }
        if ( ((PadData & PADRdown)>0) && (CurPos==4) ) {
           PadData=0;
           PlayIt(sample[8]);
           Fade_Out();
           Run_About();
           PlayIt(sample[8]);
           Fade_In();
           Draw_Options();
           PadData=0;
        }
        if ((PadData & PADLright)>0) {
           switch (CurPos) {
             case 0: if (VMode==1) break;
                     PlayIt(sample[7]);
                     VMode=1;
                     ResetGraph(0);
                     SetVideoMode(VMode);
                     ResetGraph(0);
                     GsInitGraph(640, 240, GsOFSGPU, Dith, 0);
                     GsDefDispBuff(0,0,0,240);
                     GsInit3D();
                     break;
             case 1: pBright++;
                     PlayIt(sample[7]);
                     if (pBright>10) pBright=10;
                     NormalBright=0x3C+4*pBright;
                     break;
             case 2: pMusic++;
                     PlayIt(sample[7]);
                     if (pMusic>10) pMusic=10;
                     cdVol=pMusic*12+7;
                     SsSetSerialVol( SS_SERIAL_A, cdVol, cdVol ); // sets serial volume
                     break;
             case 3: pSFX++;
                     if (pSFX>10) pSFX=10;

                     // items of voice attribute
                     s_attr.mask = (SPU_VOICE_VOLL |
                                    SPU_VOICE_VOLR |
                                    SPU_VOICE_PITCH |
                                    SPU_VOICE_WDSA |
                                    SPU_VOICE_ADSR_AMODE |
                                    SPU_VOICE_ADSR_SMODE |
                                    SPU_VOICE_ADSR_RMODE |
                                    SPU_VOICE_ADSR_AR |
                                    SPU_VOICE_ADSR_DR |
                                    SPU_VOICE_ADSR_SR |
                                    SPU_VOICE_ADSR_RR |
                                    SPU_VOICE_ADSR_SL
                                    );

                     // set the attributes with all voice
                     s_attr.voice = SPU_ALLCH;

                     // value of each voice attribute
                     s_attr.volume.left = 1638*pSFX;                // Left volume
                     s_attr.volume.right = 1638*pSFX;               // Right volume
                     s_attr.pitch = pitch;                       // Pitch
                     s_attr.addr = NULL;                 // Waveform data start address
                     s_attr.a_mode       = SPU_VOICE_LINEARIncN; // Attack curve
                     s_attr.s_mode       = SPU_VOICE_LINEARIncN; // Sustain curve
                     s_attr.r_mode       = SPU_VOICE_LINEARDecN; // Release curve
                     s_attr.ar = 0x0;                            // Attack rate value
                     s_attr.dr = 0x0;                            // Decay rate value
                     s_attr.sr = 0x0;                            // Sustain rate value
                     s_attr.rr = 0x0;                            // Release rate value
                     s_attr.sl = 0xf;                            // Sustain level value

                     SpuSetVoiceAttr(&s_attr);

                     PlayIt(sample[7]);

                     break;
           }
           Draw_Options();
        }
        if ((PadData & PADLleft)>0) {
           switch (CurPos) {
             case 0: if (VMode==0) break;
                     PlayIt(sample[7]);
                     VMode=0;
                     ResetGraph(0);
                     SetVideoMode(VMode);
                     ResetGraph(0);
                     GsInitGraph(640, 240, GsOFSGPU, Dith, 0);
                     GsDefDispBuff(0,0,0,240);
                     GsInit3D();
                     break;
             case 1: pBright--;
                     PlayIt(sample[7]);
                     if (pBright<0) pBright=0;
                     NormalBright=0x3C+4*pBright;
                     break;
             case 2: pMusic--;
                     PlayIt(sample[7]);
                     if (pMusic<0) pMusic=0;
                     cdVol=pMusic*12+7;
                     SsSetSerialVol( SS_SERIAL_A, cdVol, cdVol ); // sets serial volume
                     break;
             case 3: pSFX--;
                     if (pSFX<0) pSFX=0;

                     // items of voice attribute
                     s_attr.mask = (SPU_VOICE_VOLL |
                                    SPU_VOICE_VOLR |
                                    SPU_VOICE_PITCH |
                                    SPU_VOICE_WDSA |
                                    SPU_VOICE_ADSR_AMODE |
                                    SPU_VOICE_ADSR_SMODE |
                                    SPU_VOICE_ADSR_RMODE |
                                    SPU_VOICE_ADSR_AR |
                                    SPU_VOICE_ADSR_DR |
                                    SPU_VOICE_ADSR_SR |
                                    SPU_VOICE_ADSR_RR |
                                    SPU_VOICE_ADSR_SL
                                    );

                     // set the attributes with all voice
                     s_attr.voice = SPU_ALLCH;

                     // value of each voice attribute
                     s_attr.volume.left = 1638*pSFX;                // Left volume
                     s_attr.volume.right = 1638*pSFX;               // Right volume
                     s_attr.pitch = pitch;                       // Pitch
                     s_attr.addr = NULL;                 // Waveform data start address
                     s_attr.a_mode       = SPU_VOICE_LINEARIncN; // Attack curve
                     s_attr.s_mode       = SPU_VOICE_LINEARIncN; // Sustain curve
                     s_attr.r_mode       = SPU_VOICE_LINEARDecN; // Release curve
                     s_attr.ar = 0x0;                            // Attack rate value
                     s_attr.dr = 0x0;                            // Decay rate value
                     s_attr.sr = 0x0;                            // Sustain rate value
                     s_attr.rr = 0x0;                            // Release rate value
                     s_attr.sl = 0xf;                            // Sustain level value

                     SpuSetVoiceAttr(&s_attr);

                     PlayIt(sample[7]);
                     break;
           }
           Draw_Options();
        }
        ReadKey();
}

Run_About()
{
        short Pos=140,  // screen position (+ top margin)
              Start=0,  // the start at the picture
              Height=0, // picture height
              Level=0;

        InitBack(MENU_G_ADDR);
        Fade_In();

        while ( 1 ) {
           switch (Level) {
             case 0: Pos--;
                     Height++;
                     if (Pos<1) Level=1;
                     break;
             case 1: Start++;
                     if (Start>99) Level=2;
                     break;
             case 2: Start++;
                     Height--;
                     if (Start>239) {
                        Level=0;
                        Pos=140;
                        Start=0;
                        Height=0;
                     }
                     break;
           }
           Sprites[Auth1].y=60+Pos;     Sprites[Auth2].y=60+Pos;
           Sprites[Auth1].v=Start;      Sprites[Auth2].v=Start;
           Sprites[Auth1].h=Height;     Sprites[Auth2].h=Height;
           SayAbout();
           SlowDelay();

           ReadKey();
           if ((PadData & PADRdown)>0) {
              Fade_Out();
              InitBack(MENU_ADDR);
              return 0;
           }
        }
}

Run_Help()
{
        char NextHelp=0;

        InitBack(HELP_ADDR);
        InitHelp(HELP_1_ADDR);
        SayHelp();

        while ( 1 ) {
           ReadKey();
           if ((PadData & PADRup)>0) {
              PadData=0;
              return 0;
           }
           if ( ((PadData & PADRdown)>0) && (NextHelp==0) ) {
              InitHelp(HELP_2_ADDR);
              NextHelp++;
              SayHelp();
           }
        }
}


// ========================== R U N   G A M E S ========================= //

Draw_Game()
{
       switch (TheGame) {
         case gmSoliter  : Draw_Soliter();
                           break;
         case gmGolf     : Draw_Golf();
                           break;
         case gmFreeCell : Draw_FreeCell();
                           break;
         case gmFourteen : Draw_Fourteen();
                           break;
         case gmHearts   : Draw_Hearts();
                           break;
         case gmKing     : Draw_King();
                           break;
         case gmNine     : Draw_Nine();
                           break;
         case gmFool     : Draw_Fool();
                           break;
         case gmBlackJack: Draw_BJ();
                           break;
       }
}

Start_Menu()
{
      InitMsg(PM_PAUSE_ADDR);
      MyMsg=pMenu;
      Draw_Game();
      while ((PadData & PADRup)==0) {
         ReadKey();
         if ((MyMsg==pMenu) && ((PadData & PADRright)>0)) {
            InitMsg(PM_EXIT_ADDR);
            MyMsg=pExit;
            Draw_Game();
         }
         if ((MyMsg==pMenu) && ((PadData & PADRleft)>0)) {
            InitMsg(PM_RETRY_ADDR);
            MyMsg=pRetry;
            Draw_Game();
         }
         if ((MyMsg==pExit)  && ((PadData & PADRdown)>0)) return 2; //Exit
         if ((MyMsg==pRetry) && ((PadData & PADRdown)>0)) {
            PadData=0;
            return 1; //Restart
         }
      }
      MyMsg=-1;
      if (TheGame<gmSoliter) ShowCard();
      Draw_Game();
      return 0;
}


Run_Golf()
{
        cd_read_sub("\\DATA\\HELP\\GOLF0.TIM;1",  (void *)HELP_1_ADDR);
        cd_read_sub("\\DATA\\HELP\\GOLF1.TIM;1",  (void *)HELP_2_ADDR);
        cd_read_sub("\\DATA\\GAME\\BACKGOLF.TIM;1", (void *)BACK_ADDR);
        InitGameFiles();
        CdPlay( 2, tracks, 9);  //plays tracks[i] of cd

        for (i=0; i<7; i++) {
            Cur[i].x=59+i*80;
            Cur[i].y=90;
        }
        SelectedCard=0;

Restart:
        PlayIt(sample[1]);

        PadData=0;
        Sprites[SelectedCard].r=NormalBright;
        Sprites[SelectedCard].g=NormalBright;
        Sprites[SelectedCard].b=NormalBright;
        MyMsg=-1;
        Cursor=pSelect;

        CurPos=0;
        SortDeck();
        for (i=0; i<7; i++) {
            for (j=0; j<5; j++) List[i][j]=Deck[i*5+j];
            ListNum[i]=5;
        }
        for (i=35; i<52; i++) {
            PackList[i-35]=Deck[i];
        }
        CurPack=0;

        Draw_Golf();

        while ( (ListNum[0]>0) || (ListNum[1]>0) || (ListNum[2]>0) ||
                (ListNum[3]>0) || (ListNum[4]>0) || (ListNum[5]>0) ||
                (ListNum[6]>0) ) {
              Golf_Select();

              if ((PadData & PADstart)>0) {
                 switch (Start_Menu()) {
                   case 1: goto Restart;
                   case 2: Fade_Out();
                           return 0;
                 }
              }

              if ((PadData & PADL1)>0) {
                 Run_Help();
                 InitBack(BACK_ADDR);
                 Draw_Golf();
              }
        }
        PlayIt(sample[4]);
        InitMsg(GM_WIN_ADDR);
        MyMsg=wWin;
        Draw_Golf();
        RunDelay();
        while (1) {
           ReadKey();
           if ((PadData & PADRdown)>0)  goto Restart;
           if ((PadData & PADRright)>0) {
              Fade_Out();
              return 0;
           }
        }
}

Run_Soliter()
{
        cd_read_sub("\\DATA\\HELP\\SOLITER0.TIM;1", (void *)HELP_1_ADDR);
        cd_read_sub("\\DATA\\HELP\\SOLITER1.TIM;1", (void *)HELP_2_ADDR);
        cd_read_sub("\\DATA\\GAME\\BACKSOL.TIM;1",  (void *)BACK_ADDR);
        InitGameFiles();
        CdPlay( 2, tracks, 7);  //plays tracks[i] of cd

        Cur[0].x=44;
        Cur[0].y=30;
        Cur[1].x=122;
        Cur[1].y=30;
        for (i=2; i<9; i++) {
            Cur[i].x=51+(i-2)*80;
            Cur[i].y=90;
        }
        for (i=9; i<13; i++) {
            Cur[i].x=314+(i-9)*78;
            Cur[i].y=30;
        }
        SelectedCard=0;

Restart:
        PlayIt(sample[1]);

        PadData=0;
        Sprites[SelectedCard].r=NormalBright;
        Sprites[SelectedCard].g=NormalBright;
        Sprites[SelectedCard].b=NormalBright;
        MyMsg=-1;
        Score=0;
        Cursor=pSelect;
        for (i=0; i<7; i++) {
            vBack[i]=i;
        }
        for (i=0; i<4; i++) {
            ReadyNum[i]=0;
        }

        CurPos=0;
        SortDeck();
        for (i=0; i<24; i++) {
            PackList[i]=Deck[i];
        }
        for (i=0; i<7; i++) {
            List[i][0]=Deck[i+24];
            ListNum[i]=1;
        }
        CurDeck=31;
        CurPack=0;
        PackNum=24;
        PackLeft=23;

        Draw_Soliter();

        while ( (ReadyNum[0]<13) || (ReadyNum[1]<13) || (ReadyNum[2]<13) ||
                (ReadyNum[3]<13) ) {
              Soliter_Select();

              if ((PadData & PADstart)>0) {
                 switch (Start_Menu()) {
                   case 1: goto Restart;
                   case 2: Fade_Out();
                           return 0;
                 }
              }

              if ((PadData & PADL1)>0) {
                 Run_Help();
                 InitBack(BACK_ADDR);
                 Draw_Soliter();
              }
        }
        PlayIt(sample[4]);
        InitMsg(GM_WSCR_ADDR);
        MyMsg=wScore;
        Draw_Soliter();
        RunDelay();
        while (1) {
           ReadKey();
           if ( ((PadData & PADRdown)>0) || ((PadData & PADRright)>0) ) {
              if ((Score>HEAD.ScoreSoliter)&&(Cheater==0)) {
                 HEAD.ScoreSoliter=Score;
                 for (i=0;i<8;i++) HEAD.NameSoliter[i]=Name[i];
                 InitMsg(MC_SAVE_ADDR); SayMsg();
                 WriteCard();
              }
              if ((PadData & PADRdown)>0)  goto Restart;
              if ((PadData & PADRright)>0) {
                 Fade_Out();
                 return 0;
              }
           }
        }
}

Run_FreeCell()
{
        cd_read_sub("\\DATA\\HELP\\FREECEL0.TIM;1", (void *)HELP_1_ADDR);
        cd_read_sub("\\DATA\\HELP\\FREECEL1.TIM;1", (void *)HELP_2_ADDR);
        cd_read_sub("\\DATA\\GAME\\BACKFREE.TIM;1", (void *)BACK_ADDR);
        InitGameFiles();
        CdPlay( 2, tracks, 8);  //plays tracks[i] of cd

        for (i=0; i<4; i++) {
            Cur[i].x=50+i*68;
            Cur[i].y=30;
        }
        for (i=4; i<12; i++) {
            Cur[i].x=59+(i-4)*70;
            Cur[i].y=90;
        }
        for (i=12; i<16; i++) {
            Cur[i].x=346+(i-12)*68;
            Cur[i].y=30;
        }
        SelectedCard=0;

Restart:
        PlayIt(sample[1]);

        PadData=0;
        Sprites[SelectedCard].r=NormalBright;
        Sprites[SelectedCard].g=NormalBright;
        Sprites[SelectedCard].b=NormalBright;
        MyMsg=-1;
        Cursor=pSelect;
        for (i=0; i<4; i++) {
            ReadyNum[i]=0;
        }
        for (i=0; i<4; i++) {
            PackList[i]=-1;
        }

        CurPos=0;

        SortDeck();

        for (i=0; i<7; i++) {
            List[0][i]=Deck[i];     // 0 -> 6
            List[2][i]=Deck[i+7];   // 7 ->13
            List[4][i]=Deck[i+14];  // 14->20
            List[6][i]=Deck[i+21];  // 21->27
        }
        for (i=0; i<6; i++) {
            List[1][i]=Deck[i+28];  // 28->33
            List[3][i]=Deck[i+34];  // 34->39
            List[5][i]=Deck[i+40];  // 40->45
            List[7][i]=Deck[i+46];  // 46->51
        }
        ListNum[0]=7;
        ListNum[1]=6;
        ListNum[2]=7;
        ListNum[3]=6;
        ListNum[4]=7;
        ListNum[5]=6;
        ListNum[6]=7;
        ListNum[7]=6;

        Draw_FreeCell();

        while ( (ReadyNum[0]<13) || (ReadyNum[1]<13) || (ReadyNum[2]<13) ||
                (ReadyNum[3]<13) ) {
              FreeCell_Select();
              if ((PadData & PADstart)>0) {
                 switch (Start_Menu()) {
                   case 1: goto Restart;
                   case 2: Fade_Out();
                           return 0;
                 }
              }

              if ((PadData & PADL1)>0) {
                 Run_Help();
                 InitBack(BACK_ADDR);
                 Draw_FreeCell();
              }
        }
        PlayIt(sample[4]);
        InitMsg(GM_WIN_ADDR);
        MyMsg=wWin;
        Draw_FreeCell();
        RunDelay();
        while (1) {
           ReadKey();
           if ((PadData & PADRdown)>0)  goto Restart;
           if ((PadData & PADRright)>0) {
              Fade_Out();
              return 0;
           }
        }
}

Run_Fourteen()
{
        cd_read_sub("\\DATA\\HELP\\FOUR0.TIM;1", (void *)HELP_1_ADDR);
        cd_read_sub("\\DATA\\HELP\\FOUR1.TIM;1", (void *)HELP_2_ADDR);
        cd_read_sub("\\DATA\\GAME\\BACKFOUR.TIM;1", (void *)BACK_ADDR);
        InitGameFiles();
        CdPlay( 2, tracks, 10);  //plays tracks[i] of cd

        for (i=0; i<3; i++) {
            for (j=0; j<4; j++) {
                Cur[i*4+j].x=92+j*130;
                Cur[i*4+j].y=52+i*54;
            }
        }
        SelectedCard=0;

Restart:
        PlayIt(sample[1]);

        PadData=0;
        Sprites[SelectedCard].r=NormalBright;
        Sprites[SelectedCard].g=NormalBright;
        Sprites[SelectedCard].b=NormalBright;
        MyMsg=-1;
        Cursor=pSelect;

        CurPos=0;

        SortDeck();

        for (i=0; i<12; i++) {
            List[i][0]=Deck[i*4];
            List[i][1]=Deck[i*4+1];
            List[i][2]=Deck[i*4+2];
            List[i][3]=Deck[i*4+3];
            ListNum[i]=4;
        }
        List[0][4]=Deck[48];   ListNum[0]=5;
        List[1][4]=Deck[49];   ListNum[1]=5;
        List[2][4]=Deck[50];   ListNum[2]=5;
        List[3][4]=Deck[51];   ListNum[3]=5;

        Draw_Fourteen();

        while ( (ListNum[0]>0) || (ListNum[1]>0) || (ListNum[2]>0) ||
                (ListNum[3]>0) || (ListNum[4]>0) || (ListNum[5]>0) ||
                (ListNum[6]>0) || (ListNum[7]>0) || (ListNum[8]>0) ||
                (ListNum[9]>0) || (ListNum[10]>0) || (ListNum[11]>0) ) {
              Fourteen_Select();
              if ((PadData & PADstart)>0) {
                 switch (Start_Menu()) {
                   case 1: goto Restart;
                   case 2: Fade_Out();
                           return 0;
                 }
              }

              if ((PadData & PADL1)>0) {
                 Run_Help();
                 InitBack(BACK_ADDR);
                 Draw_Fourteen();
              }
        }
        PlayIt(sample[4]);
        InitMsg(GM_WIN_ADDR);
        MyMsg=wWin;
        Draw_Fourteen();
        RunDelay();
        while (1) {
           ReadKey();
           if ((PadData & PADRdown)>0)  goto Restart;
           if ((PadData & PADRright)>0) {
              Fade_Out();
              return 0;
           }
        }
}


Run_Hearts()
{
        cd_read_sub("\\DATA\\HELP\\HEARTS0.TIM;1",  (void *)HELP_1_ADDR);
        cd_read_sub("\\DATA\\HELP\\HEARTS1.TIM;1",  (void *)HELP_2_ADDR);
        cd_read_sub("\\DATA\\GAME\\BACKHRTS.TIM;1", (void *)BACK_ADDR);
        InitGameFiles();
        CdPlay( 2, tracks, 5);  //plays tracks[i] of cd
        for (i=0; i<13; i++) {
            Cur[i].x=267-13*13+i*26;
            Cur[i].y=175;
        }
        Cursor=pGame;
Restart:
        PadData=0;
        MyMsg=-1;
        for (i=0; i<4; i++) {
            Scores[i]=0;      // the scores of each player
        }
        for (i=0;i<13;i++) {
            DONTtake[i]=cHearts2+i;
        }
        DONTtake[13]=cSpadesQ;
        DTnumber=14;
        Danger1=0; Danger2=3;  // Danger suits
        DisableCard=-1;
        GameNumber=1;

NextGo:
        DisableSuit=0;
        CurPos=0;
        SortDeck();

        for (i=0; i<13; i++) {
            List[0][i]=Deck[i*4];    //the human
            List[1][i]=Deck[i*4+1];  //PC 1
            List[2][i]=Deck[i*4+2];  //PC 2
            List[3][i]=Deck[i*4+3];  //PC 3
        }

        for (l=0; l<4; l++) {
            for (j=0; j<13; j++) {
                k=0;
                for (i=0; i<13; i++) {
                    if (List[l][i]<List[l][k]) k=i;
                }
                PackList[j]=List[l][k];
                List[l][k]=123;
            }
            for (j=0; j<13; j++) {
                List[l][j]=PackList[j];
                if (List[l][j]==cCross2) Player=l;
            }
        }

        for (i=0; i<4; i++) {
            Ready[0][i]=-1;   // laid out cards of each player
        }

        StartPlayer=Player;
        En=0;
        if (Player==0) ShowCard();

        Draw_Hearts();

        while ( 1 ) {
              if ( (En!=1) && (Player==StartPlayer) && (Ready[0][0]!=-1) ) {
                 HideCard();
                 l=-1;    // loser
                 k=-1;    // loser's card
                 j=0;     // score
                 for (i=0;i<4;i++) {
                     if ((Ready[0][i]>=StartSuit*13) &&
                         (Ready[0][i]<(StartSuit+1)*13)) {
                        if (Ready[0][i]>k) {
                           l=i; k=Ready[0][i];
                        }
                     }
                     if ((Ready[0][i]>=cHearts2) &&
                         (Ready[0][i]<=cHeartsA)) {
                        j++;
                        DisableSuit=-1;
                     }
                     if (Ready[0][i]==cSpadesQ) j+=13;
                 }
                 Scores[l]+=j;
                 StartPlayer=l;
                 Player=l;
                 En=1;

                 // calculating how many cards are left
                 k=0;
                 for (i=0;i<13;i++) if (List[0][i]>-1) k++;
                 if (k==0) {           // the layout is complete
                    if ( (Scores[0]>99) || (Scores[1]>99) ||
                         (Scores[2]>99) || (Scores[3]>99) ) goto Stop;
                    else {
                       Draw_Hearts();
                       //HideCard();

                       InitMsg(GM_SCORE_ADDR);
                       MyMsg=wTheScore;
                       Draw_Hearts();
                       RunDelay();
                       PadData=0;
                       while ((PadData & PADRdown)==0) ReadKey();
                       PadData=0;
                       MyMsg=-1;
                       goto NextGo;
                    }
                 }
                 Draw_Hearts();
              }
              if ((En==1) || (Player<1)) {
                 Hearts_Select();
              } else {
                 //HideCard();
                 GetHeart();
                 //if (Player<1) ShowCard();
                 Draw_Hearts();
              }
              if ((PadData & PADstart)>0) {
                 HideCard();
                 switch (Start_Menu()) {
                   case 1: goto Restart;
                   case 2: Fade_Out();
                           return 0;
                 }
              }

              if ((PadData & PADL1)>0) {
                 Run_Help();
                 InitBack(BACK_ADDR);
                 Draw_Hearts();
              }
        }
Stop:
        HideCard();
        j=Scores[0];
        k=0;
        for (i=1;i<4;i++) {
            if (Scores[i]<j) {
               j=Scores[i];
               k=i;
            }
        }
        if (k==0) {
           PlayIt(sample[4]);
           InitMsg(GM_WSCR_ADDR);
           MyMsg=wScore;
           Score=j;
        } else {
           PlayIt(sample[5]);
           InitMsg(GM_LOSE_ADDR);
           MyMsg=wLose;
        }
        Draw_Hearts();
        RunDelay();
        while (1) {
           ReadKey();
           if ( ((PadData & PADRdown)>0) || ((PadData & PADRright)>0) ) {
              if (MyMsg==wScore) {
                 if ((Score<HEAD.ScoreHearts)&&(Cheater==0)) {
                    HEAD.ScoreHearts=Score;
                    for (i=0;i<8;i++) HEAD.NameHearts[i]=Name[i];
                    InitMsg(MC_SAVE_ADDR); SayMsg();
                    WriteCard();
                 }
              }
              if ((PadData & PADRdown)>0)  goto Restart;
              if ((PadData & PADRright)>0) {
                 Fade_Out();
                 return 0;
              }
           }
        }
}

Run_King()
{
        cd_read_sub("\\DATA\\HELP\\KING0.TIM;1",    (void *)HELP_1_ADDR);
        cd_read_sub("\\DATA\\HELP\\KING1.TIM;1",    (void *)HELP_2_ADDR);
        cd_read_sub("\\DATA\\GAME\\BACKKING.TIM;1", (void *)BACK_ADDR);
        cd_read_sub("\\DATA\\GAME\\GM_KING.TIM;1",  (void *)GM_KING_ADDR);
        InitKing(GM_KING_ADDR);
        InitGameFiles();
        CdPlay( 2, tracks, 3);  //plays tracks[i] of cd
        Cursor=pGame;
        for (i=0; i<8; i++) {
            Cur[i].x=181+i*30;
            Cur[i].y=175;
        }
Restart:
        PadData=0;
        MyMsg=-1;
        CurPos=0;

        for (i=0; i<4; i++) {
            Ready[0][i]=-1;   // laid out cards of each player
            Scores[i]=0;      // scores of each player
        }

        GameNumber=1;           // game number
        ResortKing();

        StartPlayer=Player;
        En=0;

        CheckRules();           // after changing the game number, we introduce the rules
        if (Player==0) ShowCard();

        Draw_King();

        while ( GameNumber<15 ) {
              if ( (En!=1) && (Player==StartPlayer) && (Ready[0][0]!=-1) ) {
                 HideCard();
                 CalcKing();
                 //if (Player==0) ShowCard();
                 Draw_King();
              }
              if ((En==1) || (Player<1)) {
                 King_Select();
              } else {
                 if (GameNumber<8) {
                    GetHeart();
                 } else {
                    GetReverseKing();
                 }
                 Draw_King();
              }
              if ((PadData & PADstart)>0) {
                 HideCard();
                 switch (Start_Menu()) {
                   case 1: goto Restart;
                   case 2: Fade_Out();
                           return 0;
                 }
              }

              if ((PadData & PADL1)>0) {
                 Run_Help();
                 InitBack(BACK_ADDR);
                 InitGameStuff(GM_KING_ADDR);
                 Draw_King();
              }
        }
        HideCard();
        j=Scores[0];
        k=0;
        for (i=1;i<4;i++) {
            if (Scores[i]>j) {
               j=Scores[i];
               k=i;
            }
        }
        if (k==0) {
           PlayIt(sample[4]);
           InitMsg(GM_WSCR_ADDR);
           MyMsg=wScore;
           Score=j;
        } else {
           PlayIt(sample[5]);
           InitMsg(GM_LOSE_ADDR);
           MyMsg=wLose;
        }
        Draw_King();
        RunDelay();
        while (1) {
           ReadKey();
           if (((PadData & PADRdown)>0) || ((PadData & PADRright)>0)) {
              if (MyMsg==wScore) {
                 if ((Score>HEAD.ScoreKing)&&(Cheater==0)) {
                    HEAD.ScoreKing=Score;
                    for (i=0;i<8;i++) HEAD.NameKing[i]=Name[i];
                    InitMsg(MC_SAVE_ADDR); SayMsg();
                    WriteCard();
                 }
              }
              if ((PadData & PADRdown)>0)  goto Restart;
              if ((PadData & PADRright)>0) {
                 Fade_Out();
                 return 0;
              }
           }
        }
}

Run_Nine()
{
        cd_read_sub("\\DATA\\HELP\\NINE0.TIM;1",    (void *)HELP_1_ADDR);
        cd_read_sub("\\DATA\\HELP\\NINE1.TIM;1",    (void *)HELP_2_ADDR);
        cd_read_sub("\\DATA\\GAME\\ALPHABET.TIM;1", (void *)ALPHABET_ADDR);
        cd_read_sub("\\DATA\\GAME\\MC_LOAD.TIM;1",  (void *)MC_LOAD_ADDR);
        cd_read_sub("\\DATA\\GAME\\MC_SAVE.TIM;1",  (void *)MC_SAVE_ADDR);
        cd_read_sub("\\DATA\\GAME\\BLACK_C.TIM;1",  (void *)BLACK_C_ADDR);
        cd_read_sub("\\DATA\\GAME\\PACK.TIM;1",     (void *)PACK_ADDR);
        cd_read_sub("\\DATA\\GAME\\RED_C.TIM;1",    (void *)RED_C_ADDR);
        cd_read_sub("\\DATA\\GAME\\GM_DRAW.TIM;1",  (void *)GM_DRAW_ADDR);
        cd_read_sub("\\DATA\\GAME\\GM_LOSE.TIM;1",  (void *)GM_LOSE_ADDR);
        //cd_read_sub("\\DATA\\GAME\\GM_SCORE.TIM;1", (void *)GM_SCORE_ADDR);
        //cd_read_sub("\\DATA\\GAME\\GM_WIN.TIM;1",   (void *)GM_WIN_ADDR);
        cd_read_sub("\\DATA\\GAME\\GM_WSCR.TIM;1",  (void *)GM_WSCR_ADDR);
        cd_read_sub("\\DATA\\GAME\\PM_EXIT.TIM;1",  (void *)PM_EXIT_ADDR);
        cd_read_sub("\\DATA\\GAME\\PM_PAUSE.TIM;1", (void *)PM_PAUSE_ADDR);
        cd_read_sub("\\DATA\\GAME\\PM_RETRY.TIM;1", (void *)PM_RETRY_ADDR);
        cd_read_sub("\\DATA\\GAME\\BLACK9.TIM;1",   (void *)BLACK9_ADDR);
        cd_read_sub("\\DATA\\GAME\\BLACK9CS.TIM;1", (void *)BLACK9CS_ADDR);
        cd_read_sub("\\DATA\\GAME\\RED9.TIM;1",     (void *)RED9_ADDR);
        cd_read_sub("\\DATA\\GAME\\RED9CS.TIM;1",   (void *)RED9CS_ADDR);

        cd_read_sub("\\DATA\\GAME\\BACKNINE.TIM;1", (void *)BACK_ADDR);

        InitBack(BACK_ADDR);
        Fade_In();
        InitPack(PACK_ADDR);
        Init9Cards(BLACK9_ADDR, BLACK_C_ADDR, BLACK9CS_ADDR,
                   RED9_ADDR,   RED_C_ADDR,   RED9CS_ADDR);
        InitAlphabet(ALPHABET_ADDR);
        CdPlay( 2, tracks, 2);  //plays tracks[i] of cd

        Cursor=pGame;
        for (i=0; i<12; i++) {
            Cur[i].x=299-12*15+i*30;
            Cur[i].y=175;
        }
Restart:
        PadData=0;
        MyMsg=-1;
        Score=0;
        CurPos=0;

        for (i=0; i<4; i++) {
            for (j=0; j<13; j++) Ready[i][j]=-1;   // laid out cards
        }

        for (j=0; j<4; j++) {
            for (i=0; i<13; i++) {
                List[j][i]=-1;
                Ready[j][i]=-1;
            }
        }
        SortDeck36();
        Player=0;

        for (i=0; i<12; i++) {
            List[0][i]=Deck[i*3];    //the human
            List[1][i]=Deck[i*3+1];  //PC 1
            List[2][i]=Deck[i*3+2];  //PC 2
        }
        for (l=0; l<3; l++) {
            for (j=0; j<12; j++) {
                k=0;
                for (i=0; i<12; i++) {
                    if (List[l][i]<List[l][k]) k=i;
                }
                PackList[j]=List[l][k];
                List[l][k]=123;
            }
            for (j=0; j<12; j++) {
                List[l][j]=PackList[j];
                if (List[l][j]==cDiamonds9) Player=l;
            }
        }

        StartPlayer=Player;
        Winner=-1;
        QuitFlag=3;     // the number of players: when player quits dec by 1

        if (Player==0) ShowCard();
        Draw_Nine();

        while ( QuitFlag!=0 ) {
              if (Player<1) {
                 j=0;
                 for (i=0;i<12;i++) if (List[0][i]>-1) j=1;
                 if (j>0) {
                    l=0;
                    for (k=0;k<12;k++) {
                        if (List[0][k]>-1) {
                           j=List[0][k]/13;          // suit
                           i=List[0][k]%13-cHearts6; // card
                           if (i==3) l=1;
                           if ((i<3) && (Ready[j][i+1]!=-1)) l=1;
                           if ((i>3) && (Ready[j][i-1]!=-1)) l=1;
                        }
                    }
                    if (l==0) {
                       Score+=10;
                       HideCard();
                       Player++;    // skip a move
                       Draw_Nine();
                    } else {
                      Nine_Select();
                    }
                    j=0;
                    for (i=0;i<12;i++) if (List[0][i]>-1) j=1;
                    if (j==0) {
                       QuitFlag--;
                       if (Winner<0) Winner=0;
                    }
                 } else {
                    Player++;
                 }
              } else {
                 j=0;
                 for (i=0;i<12;i++) if (List[Player][i]>-1) j=1;
                 if (j>0) {
                    BUplayer=Player;
                    GetNine();
                    j=0;
                    for (i=0;i<12;i++) if (List[BUplayer][i]>-1) j=1;
                    if (j==0) {
                       QuitFlag--;
                       if (Winner<0) Winner=BUplayer;
                    }
                 } else {
                    Player++;
                    if (Player>2) Player=0;
                    if (Player==0) ShowCard();
                 }
                 Draw_Nine();
              }
              if ((PadData & PADstart)>0) {
                 HideCard();
                 switch (Start_Menu()) {
                   case 1: goto Restart;
                   case 2: Fade_Out();
                           return 0;
                 }
              }

              if ((PadData & PADL1)>0) {
                 Run_Help();
                 InitBack(BACK_ADDR);
                 InitGameStuff(RED9CS_ADDR);
                 Draw_Nine();
              }
        }
        HideCard();
        if (Winner==0) {
           PlayIt(sample[4]);
           InitMsg(GM_WSCR_ADDR);
           MyMsg=wScore;
        } else {
           PlayIt(sample[5]);
           InitMsg(GM_LOSE_ADDR);
           MyMsg=wLose;
        }
        Draw_Nine();
        RunDelay();
        while (1) {
           ReadKey();
           if ( ((PadData & PADRdown)>0) || ((PadData & PADRright)>0)) {
              if (MyMsg==wScore) {
                 if ((Score>HEAD.ScoreNine)&&(Cheater==0)) {
                    HEAD.ScoreNine=Score;
                    for (i=0;i<8;i++) HEAD.NameNine[i]=Name[i];
                    InitMsg(MC_SAVE_ADDR); SayMsg();
                    WriteCard();
                 }
              }
              if ((PadData & PADRdown)>0)  goto Restart;
              if ((PadData & PADRright)>0) {
                 Fade_Out();
                 return 0;
              }
           }
        }
}

Run_Fool()
{
        cd_read_sub("\\DATA\\HELP\\FOOL0.TIM;1",    (void *)HELP_1_ADDR);
        cd_read_sub("\\DATA\\HELP\\FOOL1.TIM;1",    (void *)HELP_2_ADDR);
        cd_read_sub("\\DATA\\GAME\\BACKFOOL.TIM;1", (void *)BACK_ADDR);
        InitGameFiles();
        CdPlay( 2, tracks, 4);  //plays tracks[i] of cd
        Cursor=pGame;
Restart:
        PadData=0;
        MyMsg=-1;
        CurPos=0;
        // at first we calculate for 6 cards
        ListNum[0]=6;
        ListNum[1]=6;
        for (i=0; i<6; i++) {
            Cur[i].x=299-ListNum[0]*15+i*30;
            Cur[i].y=175;
        }

        for (i=0; i<6; i++) { // up to 6 cards
            Ready[0][i]=-1;   // laid out cards of the human
            Ready[1][i]=-1;   // laid out cards of the PC
        }

        SortDeck36();

        for (i=0; i<6; i++) {
            List[0][i]=Deck[i*2];    //the human
            List[1][i]=Deck[i*2+1];  //PC 1
        }
        CurDeck=12;             // the current item in the deck - max 35
        TrumpSuit=rand()%4;     // trump suit
        MinP=100;               // minimal trump card
        Player=0;               // and who have it

        for (l=0; l<2; l++) {
            for (j=0; j<6; j++) {
                k=0;
                for (i=0; i<6; i++) {
                    if (List[l][i]<List[l][k]) k=i;
                }
                PackList[j]=List[l][k];
                List[l][k]=123;
            }
            for (j=0; j<6; j++) {
                List[l][j]=PackList[j];
                if ( ((List[l][j]/13)==TrumpSuit) &&
                     (List[l][j]<MinP) ) {
                   Player=l;
                   MinP=List[l][j];
                }
            }
        }

        StartPlayer=Player;
        CurCard=0;

        Winner=-1;

        if (Player==0) ShowCard();
        if (Player<1) Draw_Fool();

        while ( Winner<0 ) {
              //Name[1]=ListNum[0]+140;
              //Name[2]=ListNum[1]+140;
              //Name[4]=CurDeck/10+140;
              //Name[5]=CurDeck+140;
              if (Player<1) {
                 l=0;
                 for (k=0;k<ListNum[0];k++) {
                     if (List[0][k]>-1) l=1;
                 }
                 if ((l==0) && (CurDeck>=36)) {
                    HideCard();
                    NextStepFool();
                    StartPlayer=1;
                    Player=1;
                    //ShowCard();
                    Draw_Fool();
                 } else {
                   Fool_Select();
                 }
              } else {
                 if (StartPlayer==1) {
                    if (CurCard==0) StartFool();
                    else              AddFool();
                 } else {
                    EndFool();
                 }
                 Draw_Fool();
              }
              if ((PadData & PADstart)>0) {
                 HideCard();
                 switch (Start_Menu()) {
                   case 1: goto Restart;
                   case 2: Fade_Out();
                           return 0;
                 }
              }

              if ((PadData & PADL1)>0) {
                 Run_Help();
                 InitBack(BACK_ADDR);
                 Draw_Fool();
              }
        }
        HideCard();
        switch (Winner) {
          case 0: MyMsg=wWin;
                  InitMsg(GM_WIN_ADDR);
                  PlayIt(sample[4]);
                  break;
          case 1: MyMsg=wLose;
                  InitMsg(GM_LOSE_ADDR);
                  PlayIt(sample[5]);
                  break;
          case 2: MyMsg=wNoOne;
                  InitMsg(GM_DRAW_ADDR);
                  PlayIt(sample[5]);
                  break;
        }
        Draw_Fool();
        RunDelay();
        while (1) {
           ReadKey();
           if ((PadData & PADRdown)>0)  goto Restart;
           if ((PadData & PADRright)>0) {
              Fade_Out();
              return 0;
           }
        }
}

Run_BJ()
{
        cd_read_sub("\\DATA\\HELP\\BJACK0.TIM;1", (void *)HELP_1_ADDR);
        cd_read_sub("\\DATA\\HELP\\BJACK1.TIM;1", (void *)HELP_2_ADDR);
        cd_read_sub("\\DATA\\GAME\\BACKBJ.TIM;1", (void *)BACK_ADDR);
        InitGameFiles();
        CdPlay( 2, tracks, 6);  //plays tracks[i] of cd

Restart:
        PlayIt(sample[1]);

        PadData=0;
        MyMsg=-1;
        Cursor=pDown;

        CurPos=0;
        SortDeck();

        ListNum[0]=2;
        ListNum[1]=2;
        List[0][0]=Deck[0];  //the human
        List[1][0]=Deck[1];  //PC
        List[0][1]=Deck[2];  //the human
        List[1][1]=Deck[3];  //PC
        // score of each player
        i=SrcDeck[List[0][0]].Value;
        if (i==cAce)   i=11;
        else if (i>10) i=10;
        Scores[0]=i;
        i=SrcDeck[List[0][1]].Value;
        if (i==cAce)   i=11;
        else if (i>10) i=10;
        Scores[0]+=i;
        i=SrcDeck[List[1][0]].Value;
        if (i==cAce)   i=11;
        else if (i>10) i=10;
        Scores[1]=i;
        i=SrcDeck[List[1][1]].Value;
        if (i==cAce)   i=11;
        else if (i>10) i=10;
        Scores[1]+=i;
        CurDeck=4;

        Player=0;
        ReadyNum[0]=0;
        ReadyNum[1]=0;

        Draw_BJ();
        if ((List[1][0]==cAce) && (List[1][1]==cAce)) {
           Scores[1]=21;
           Player=2;
        }
        if ((List[0][0]==cAce) && (List[0][1]==cAce)) {
           Scores[0]=21;
           Player=2;
        }

        while ( Player<2 ) {
              if (Player==0) {
                 BJ_Select();   // player move
              } else {
                 BJ_GetCard();  // PC move
              }
              if (Scores[0]>20) ReadyNum[0]=1;
              if (Scores[1]>20) ReadyNum[1]=1;
              if ((ReadyNum[0]==1) && (ReadyNum[1]==1)) Player=2;
              if ((PadData & PADstart)>0) {
                 switch (Start_Menu()) {
                   case 1: goto Restart;
                   case 2: Fade_Out();
                           return 0;
                 }
              }

              if ((PadData & PADL1)>0) {
                 Run_Help();
                 InitBack(BACK_ADDR);
                 Draw_BJ();
              }
        }
Stop:
        Player=2;
        if (Scores[1]==Scores[0]) {
           PlayIt(sample[5]);
           InitMsg(GM_DRAW_ADDR);
           MyMsg=wNoOne;
           goto SayIt;
        }
        if (Scores[1]==21) {
           PlayIt(sample[5]);
           InitMsg(GM_LOSE_ADDR);
           MyMsg=wLose;
           goto SayIt;
        }
        if (Scores[0]==21) {
           PlayIt(sample[4]);
           InitMsg(GM_WIN_ADDR);
           MyMsg=wWin;
           goto SayIt;
        }
        if ( (Scores[0]<21) && ((Scores[1]>21) || (Scores[1]<Scores[0])) ) {
           PlayIt(sample[4]);
           InitMsg(GM_WIN_ADDR);
           MyMsg=wWin;
           goto SayIt;
        }
        if ( (Scores[1]<21) && ((Scores[0]>21) || (Scores[0]<Scores[1])) ) {
           PlayIt(sample[5]);
           InitMsg(GM_LOSE_ADDR);
           MyMsg=wLose;
           goto SayIt;
        }
        if (Scores[0]<Scores[1]) {
           PlayIt(sample[4]);
           InitMsg(GM_WIN_ADDR);
           MyMsg=wWin;
        } else {
           PlayIt(sample[5]);
           InitMsg(GM_LOSE_ADDR);
           MyMsg=wLose;
        }
SayIt:
        Draw_BJ();
        RunDelay();
        while (1) {
           ReadKey();
           if ((PadData & PADRdown)>0)  goto Restart;
           if ((PadData & PADRright)>0) {
              Fade_Out();
              return 0;
           }
        }
}



// ========================= G A M E   S T U F F ======================== //


InitGameFiles()
{
        cd_read_sub("\\DATA\\GAME\\ALPHABET.TIM;1", (void *)ALPHABET_ADDR);
        cd_read_sub("\\DATA\\GAME\\MC_LOAD.TIM;1",  (void *)MC_LOAD_ADDR);
        cd_read_sub("\\DATA\\GAME\\MC_SAVE.TIM;1",  (void *)MC_SAVE_ADDR);
        cd_read_sub("\\DATA\\GAME\\BLACK_C.TIM;1",  (void *)BLACK_C_ADDR);
        cd_read_sub("\\DATA\\GAME\\PACK.TIM;1",     (void *)PACK_ADDR);
        cd_read_sub("\\DATA\\GAME\\RED_C.TIM;1",    (void *)RED_C_ADDR);
        cd_read_sub("\\DATA\\GAME\\GM_DRAW.TIM;1",  (void *)GM_DRAW_ADDR);
        cd_read_sub("\\DATA\\GAME\\GM_LOSE.TIM;1",  (void *)GM_LOSE_ADDR);
        cd_read_sub("\\DATA\\GAME\\GM_SCORE.TIM;1", (void *)GM_SCORE_ADDR);
        cd_read_sub("\\DATA\\GAME\\GM_WIN.TIM;1",   (void *)GM_WIN_ADDR);
        cd_read_sub("\\DATA\\GAME\\GM_WSCR.TIM;1",  (void *)GM_WSCR_ADDR);
        cd_read_sub("\\DATA\\GAME\\PM_EXIT.TIM;1",  (void *)PM_EXIT_ADDR);
        cd_read_sub("\\DATA\\GAME\\PM_PAUSE.TIM;1", (void *)PM_PAUSE_ADDR);
        cd_read_sub("\\DATA\\GAME\\PM_RETRY.TIM;1", (void *)PM_RETRY_ADDR);
        cd_read_sub("\\DATA\\GAME\\BLACK.TIM;1",    (void *)BLACK_ADDR);
        cd_read_sub("\\DATA\\GAME\\RED.TIM;1",      (void *)RED_ADDR);

        InitBack(BACK_ADDR);
        Fade_In();
        InitCards(BLACK_ADDR, BLACK_C_ADDR, RED_ADDR, RED_C_ADDR);
        InitPack(PACK_ADDR);
        InitAlphabet(ALPHABET_ADDR);
}

DeselectCard()
{
    Sprites[SelectedCard].r=NormalBright;
    Sprites[SelectedCard].g=NormalBright;
    Sprites[SelectedCard].b=NormalBright;
    Cursor=pSelect;
    Sprites[pSelect].x=Sprites[pDown].x;
    Sprites[pSelect].y=Sprites[pDown].y;
}

SelectCard()
{
    Cursor=pDown;
    Sprites[pDown].x=Sprites[pSelect].x;
    Sprites[pDown].y=Sprites[pSelect].y;
    Sprites[SelectedCard].r=NormalBright+0x30;
    Sprites[SelectedCard].g=NormalBright+0x30;
    Sprites[SelectedCard].b=NormalBright;
}

int ShowCard()
{
    short ii;

    ii=List[0][CurPos];
    if (ii<0) return 0;
    Sprites[ii].r=NormalBright+0x30;
    Sprites[ii].g=NormalBright+0x30;
    Sprites[ii].b=NormalBright;
}

int HideCard()
{
    short ii;

    ii=List[0][CurPos];
    if (ii<0) return 0;
    Sprites[ii].r=NormalBright;
    Sprites[ii].g=NormalBright;
    Sprites[ii].b=NormalBright;
}

int MayBeLaid()
{
   if (SrcDeck[NextCard].Value==cAce) return 1;
   if ( ((SrcDeck[SelectedCard].Suit & SrcDeck[NextCard].Suit)==0) &&
        ( ((SrcDeck[NextCard].Value-SrcDeck[SelectedCard].Value)==1) |
          ((SrcDeck[NextCard].Value==2) && (SrcDeck[SelectedCard].Value==cAce)) ) ) {
      return 0;
   }
   return 1;
}

int CanLayGolf()
{
   if ( abs(SrcDeck[DstCard].Value-SrcDeck[SelectedCard].Value)==1 ) return 0;
   if ( ((SrcDeck[DstCard].Value==2) && (SrcDeck[SelectedCard].Value==cAce)) ||
        ((SrcDeck[DstCard].Value==cAce) && (SrcDeck[SelectedCard].Value==2)) )
      return 0;
   return 1;
}

int CanLay14()
{
   short si, di;

   si=SrcDeck[SelectedCard].Value;   if (si==cAce) si=1;
   di=SrcDeck[DstCard].Value;        if (di==cAce) di=1;

   if ( (di+si)==14 ) return 0;
   return 1;
}

int CanLay()
{
   if (SrcDeck[DstCard].Value==cAce) return 1;
   if ( ((SrcDeck[SelectedCard].Suit & SrcDeck[DstCard].Suit)==0) &&
        ( ((SrcDeck[DstCard].Value-SrcDeck[SelectedCard].Value)==1) |
          ((SrcDeck[DstCard].Value==2) && (SrcDeck[SelectedCard].Value==cAce)) ) ) {
      return 0;
   }
   return 1;
}

int CanLayReady()
{
   if ( (SrcDeck[SelectedCard].Suit==SrcDeck[DstCard].Suit) &&
        ( ((SrcDeck[DstCard].Value-SrcDeck[SelectedCard].Value)==-1) |
          ((SrcDeck[SelectedCard].Value==2) && (SrcDeck[DstCard].Value==cAce)) ) ) {
      return 0;
   }
   return 1;
}


// ========================= G A M E   L O G I C ======================== //


ResortFool()
{
        for (l=0; l<2; l++) {
            for (j=0; j<ListNum[l]; j++) {
                k=0;
                for (i=0; i<ListNum[l]; i++) {
                    if (List[l][i]<List[l][k]) k=i;
                }
                PackList[j]=List[l][k];
                List[l][k]=123;
            }
            for (j=0; j<ListNum[l]; j++) List[l][j]=PackList[j];
        }
}

NextStepFool()
{
   short l, j;

   for (i=0;i<6;i++) {
       Ready[0][i]=-1;
       Ready[1][i]=-1;
   }
   CurCard=0;

   if (StartPlayer==0) l=1;
   else                l=0;
   while ( (CurDeck<36) && ((ListNum[StartPlayer]<6) || (ListNum[l]<6)) ) {
      j=ListNum[StartPlayer];
      if (j<6) {
         /// ??? ///
         TempList[0]=cBackV;
         TempNum=1;
         Start_x=560;
         Start_y=90;
         End_x=300;
         if (StartPlayer==0) End_y=162;
         else                End_y=20;
         Animate_Game();
         /// ??? ///
         List[StartPlayer][j]=Deck[CurDeck];    // adding card
         ListNum[StartPlayer]++;
         CurDeck++;
      }
      j=ListNum[l];
      if ((CurDeck<36) && (j<6)) {
         /// ??? ///
         TempList[0]=cBackV;
         TempNum=1;
         Start_x=560;
         Start_y=90;
         End_x=300;
         if (l==0) End_y=162;
         else      End_y=20;
         Animate_Game();
         /// ??? ///
         List[l][j]=Deck[CurDeck];              // adding card
         ListNum[l]++;
         CurDeck++;
      }
   }

   if (CurDeck>=36) {
      if (ListNum[0]<1) Winner+=1;    // -1 => 0
      if (ListNum[1]<1) Winner+=2;    // -1 => 1,  0 => 2 - the draw
   }

   ResortFool();

   k=30;
   if (ListNum[0]>17) k=20;
   if (ListNum[0]>26) k=14;
   for (i=0; i<ListNum[0]; i++) {
       Cur[i].x=299-k*ListNum[0]/2+i*k;
       Cur[i].y=170;
   }
}

ResortKing()
{
        for (j=0; j<4; j++) {
            for (i=0; i<13; i++) {
                List[j][i]=-1;
            }
        }
        SortDeck32();

        for (i=0; i<8; i++) {
            List[0][i]=Deck[i*4];    //the human
            List[1][i]=Deck[i*4+1];  //PC 1
            List[2][i]=Deck[i*4+2];  //PC 2
            List[3][i]=Deck[i*4+3];  //PC 3
        }
        for (l=0; l<4; l++) {
            for (j=0; j<8; j++) {
                k=0;
                for (i=0; i<8; i++) {
                    if (List[l][i]<List[l][k]) k=i;
                }
                PackList[j]=List[l][k];
                List[l][k]=123;
            }
            for (j=0; j<8; j++) {
                List[l][j]=PackList[j];
                if ((GameNumber==1) && (List[l][j]==cCross7)) Player=l;
            }
        }
}

CheckRules()
{
    switch (GameNumber) {
       case 8:
       case 1: Drain=1;                    // do not take bribes
               DTnumber=0;
               Danger1=-1; Danger2=-1;
               DisableSuit=-1;
               DisableCard=-1;
               break;
       case 9:
       case 2: Drain=0;                    // do not take hearts
               for (i=0;i<8;i++) {
                   DONTtake[i]=cHearts7+i;
               }
               DTnumber=8;
               Danger1=0; Danger2=-1;
               DisableSuit=0;
               DisableCard=-1;
               break;
       case 10:
       case 3: Drain=0;                    // do not take males
               DONTtake[0]=cHeartsJ;
               DONTtake[1]=cCrossJ;
               DONTtake[2]=cDiamondsJ;
               DONTtake[3]=cSpadesJ;
               DONTtake[4]=cHeartsK;
               DONTtake[5]=cCrossK;
               DONTtake[6]=cDiamondsK;
               DONTtake[7]=cSpadesK;
               DTnumber=8;
               Danger1=-1; Danger2=-1;
               DisableSuit=-1;
               DisableCard=-1;
               break;
       case 11:
       case 4: Drain=0;                    // do not take females
               DONTtake[0]=cHeartsQ;
               DONTtake[1]=cCrossQ;
               DONTtake[2]=cDiamondsQ;
               DONTtake[3]=cSpadesQ;
               DTnumber=4;
               Danger1=-1; Danger2=-1;
               DisableSuit=-1;
               DisableCard=-1;
               break;
       case 12:
       case 5: Drain=0;                    // do not take tha last two
               DTnumber=0;
               Danger1=-1; Danger2=-1;
               DisableSuit=-1;
               DisableCard=-1;
               break;
       case 13:
       case 6: Drain=0;                    // do not take king
               DONTtake[0]=cHeartsK;
               DTnumber=1;
               Danger1=-1; Danger2=-1;
               DisableSuit=-1;
               DisableCard=cHeartsK;
               break;
       case 14:
       case 7: Drain=1;                    // do not take all
               DONTtake[0]=cHearts7;
               DONTtake[1]=cHearts8;
               DONTtake[2]=cHearts9;
               DONTtake[3]=cHearts10;

               DONTtake[4]=cHeartsJ;
               DONTtake[5]=cCrossJ;
               DONTtake[6]=cDiamondsJ;
               DONTtake[7]=cSpadesJ;
               DONTtake[8]=cCrossK;
               DONTtake[9]=cDiamondsK;
               DONTtake[10]=cSpadesK;

               DONTtake[11]=cHeartsQ;
               DONTtake[12]=cCrossQ;
               DONTtake[13]=cDiamondsQ;
               DONTtake[14]=cSpadesQ;

               DONTtake[15]=cHeartsK;

               DTnumber=16;
               Danger1=0; Danger2=-1;
               DisableSuit=0;
               DisableCard=cHeartsK;
               break;
    }
}

CalcKing()   // score calculation
{
     l=-1;              // loser
     k=-1;              // loser's card
     j=0;               // scores
     CardOnHand=0;      // the number of cards in the hands
     for (i=0;i<13;i++) {
         if (List[0][i]>-1) CardOnHand++;
     }
     for (i=0;i<4;i++) {
         if ((Ready[0][i]>=StartSuit*13) &&      // searching the loser
             (Ready[0][i]<(StartSuit+1)*13)) {
            if (Ready[0][i]>k) {
               l=i; k=Ready[0][i];
            }
         }
         switch (GameNumber) {                   // calculate the scores
            case 8:
            case 1: j=2;
                    break;
            case 9:
            case 2: if ((Ready[0][i]>=cHearts2) &&
                        (Ready[0][i]<=cHeartsA)) {
                       j+=2;
                       DisableSuit=-1;
                    }
                    break;
            case 10:
            case 3: if ( ((Ready[0][i]+2)%13==0) ||
                         ((Ready[0][i]+4)%13==0) ) j+=2;
                    break;
            case 11:
            case 4: if ((Ready[0][i]+3)%13==0) j+=4;
                    break;
            case 12:
            case 5: if (CardOnHand<2) j=8;
                    break;
            case 13:
            case 6: if (Ready[0][i]==cHeartsK) j=16;
                    break;
            case 14:
            case 7:
                    if (i==0) j+=2;
                    if ((Ready[0][i]>=cHearts2) &&
                        (Ready[0][i]<=cHeartsA)) {
                       j+=2;
                       DisableSuit=-1;
                    }
                    if ( ((Ready[0][i]+2)%13==0) ||
                         ((Ready[0][i]+4)%13==0) ) j+=2;
                    if ((Ready[0][i]+3)%13==0) j+=4;
                    if ((CardOnHand<2) && (i==0)) j+=8;
                    if (Ready[0][i]==cHeartsK) j+=16;
                    break;
         }
     }
     Scores[l]+=j;
     StartPlayer=l;
     Player=l;
     En=1;
}


int GetHeart()
// Select the suit, the card of the current player and "laid out" the card at the table Ready[0]
{
   // checking for 2-cross
   for (i=0;i<13;i++) {
       if ((GameNumber==1) && (List[Player][i]==cCross2)) {
          StartSuit=1;
          SelectedSuit=1;
          SelectedCard=i;
          goto CardDone;
       }
   }

   // Builds a table of cards available in the hands of the Player and all the rest (Enemies_Cards) in PlayerCards[0]
   //   2 3 4 ... J Q K A     - for each player
   // C 0 0 1 ... 0 0 1 0
   // K 1 0 0 ... 0 0 0 1
   // B 1 0 0 ... 1 0 0 0
   // P 0 1 0 ... 1 0 1 1
   for (Pl=0;Pl<4;Pl++) {
       for (i=0;i<13;i++) {
           for (Suit=0;Suit<4;Suit++) PlayerCards[Pl][Suit][i]=0;
       }
   }
   for (Pl=0;Pl<4;Pl++) {
       for (i=0;i<13;i++) {
           k=List[Pl][i];
           if ( (k>=cHearts2) && (k<=cHeartsA) ) {
              PlayerCards[Pl][0][k]=1;
           } else {
              if ( (k>=cCross2) && (k<=cCrossA) ) {
                 PlayerCards[Pl][1][k-13]=1;
              } else {
                 if ( (k>=cDiamonds2) && (k<=cDiamondsA) ) {
                    PlayerCards[Pl][2][k-26]=1;
                 } else {
                    if ( (k>=cSpades2) && (k<=cSpadesA) ) {
                       PlayerCards[Pl][3][k-39]=1;
                    }
                 }
              }
           }
       } // for (i=0;i<13;i++)
   } // for (Pl=0;Pl<4;i++)
   // Combines all PlayerCards except Player's into the PlayerCards[0]
   for (Pl=1;Pl<4;Pl++) {
       if (Pl!=Player) {
          for (Suit=0;Suit<4;Suit++) {
              for (i=0;i<13;i++) PlayerCards[0][Suit][i]+=PlayerCards[Pl][Suit][i];
          }
       }
   }

   BeCare[0]=0;
   BeCare[1]=0;
   BeCare[2]=0;
   BeCare[3]=0;
   k=-1;
   for (i=0;i<13;i++) {   // MAX cards player have
       if (PlayerCards[Player][Suit][i]>0) k=i;
   }
   switch (GameNumber) {
      case 3:
           for (Suit=0;Suit<4;Suit++) {
               j=0; l=0;
               // opponents have D(j) and K(l)
               if (PlayerCards[0][Suit][9]>0) j=1;
               if (PlayerCards[0][Suit][11]>0) l=1;
               switch (k) {
                      case 12:
                           if ((j>0) || (l>0)) BeCare[Suit]=1;
                           break;
                      case 11:
                      case  9:
                           BeCare[Suit]=1;
                           break;
                      case 10:
                           if (j>0) BeCare[Suit]=1;
                           break;
               }
           }
           break;
      case 4:
           for (Suit=0;Suit<4;Suit++) {
               j=0;
               // opponents have D(j)
               if (PlayerCards[0][Suit][10]>0) j=1;
               switch (k) {
                      case 12:
                      case 11:
                           if (j>0) BeCare[Suit]=1;
                           break;
                      case 10:
                           BeCare[Suit]=1;
                           break;
               }
           }
           break;
      case 6:
           if ((PlayerCards[0][0][12]>0) &&
               (PlayerCards[Player][0][11]>0)) BeCare[0]=1;
   }

   // the number of cards of each suit in the Player's hand
   k=0;
   for (Suit=0; Suit<4; Suit++) {
       PlayerSuit[Suit]=0;
       for (i=0; i<13; i++) PlayerSuit[Suit]+=PlayerCards[Player][Suit][i];
       if (PlayerSuit[Suit]>0) {
          k++;
          j=Suit;
       }
   }
   if (k==1) {  // if the suit is only one then return it
      SelectedSuit=j;
      if (Player!=StartPlayer) {
         if (SelectedSuit==StartSuit) goto AddCard;
         goto SelAddCard; // !!! folding the suit
      }
      goto SelectCard;
   }

   // divides the suits into those we take (0) and those we do not take (1)
   for (Suit=0; Suit<4; Suit++) {
       if (PlayerSuit[Suit]>0) {     // if Player has the suit
          // chooses the maximum card in the given suit that opponents have and
          // the minimum card in the given suit that Player has
          MaxE=-1; // opponents do not have such suit
          MinP=0;
          for (i=0;i<13;i++)  if (PlayerCards[0][Suit][i]>0) MaxE=i;
          for (i=12;i>-1;i--) if (PlayerCards[Player][Suit][i]>0) MinP=i;
          // MaxE > MinP => DONT take
          // MaxE < MinP => take
          // MaxE = -1   => take
          if (MaxE > MinP) {
             PlayerNotTake[Suit]=1;  // don't take
          } else {
             PlayerNotTake[Suit]=0;  // take
          }
       } else {
         PlayerNotTake[Suit]=-1;     // does not exists
       }
   }

   if (Player!=StartPlayer) goto AddCard; // toss

   k=0;
   for (Suit=0; Suit<4; Suit++) {
       if (PlayerSuit[Suit]>0) {
          k+=PlayerNotTake[Suit];
          j=Suit;                            // the last suit
          if (PlayerNotTake[Suit]>0) i=Suit; // the last suit that do not take
       }
   }
   if (k==0) {  // if there are no suits we want to take, then return any
      SelectedSuit=j;    // not Hearts, because it is rhe first
      goto SelectCard;
   }
   if (k==1) {  // if there is only one such suit
      if (i==DisableSuit) {
         SelectedSuit=j;  // if we can't play this suit, then return any
      } else {
         SelectedSuit=i;  // otherwise return this suit
      }
      goto SelectCard;
   }

   // calculates the priority of every suit - SuitPriority for all suits that we do not take on
   for (Suit=0;Suit<4;Suit++) {
       if (PlayerNotTake[Suit]>0) { // if Player has and we do not take on it
          k=0;
          j=0;
          for (i=0;i<13;i++) {
              if ((PlayerCards[Player][Suit][i])>0) k+=(i+1)*10;
          }
          j=PlayerSuit[Suit];
          SuitPriority[Suit]=k/j;
       } else {
          SuitPriority[Suit]=32000;  // for suits on which we take
       }
   }
   // choose the two lowest priorities
   k=30000; l=0;
   i=30000; j=0;
   for (Suit=0; Suit<4; Suit++) {
       if (SuitPriority[Suit]<k) {
          k=SuitPriority[Suit];
          l=Suit;
       }
   }
   for (Suit=0; Suit<4; Suit++) {
       if ( (SuitPriority[Suit]<i) && (Suit!=l) ) {
          i=SuitPriority[Suit];
          j=Suit;
       }
   }
   if (j==DisableSuit) {       // if we cann't start from this suit
      SelectedSuit=l;
      goto SelectCard;
   }
   if (l==DisableSuit) {       // if we cann't start from this suit
      SelectedSuit=j;
      goto SelectCard;
   }
   if (i<k) {
      SelectedSuit=j;
   } else {
      if (k<i) {
         SelectedSuit=l;
      } else {
         i=rand();
         if ((i%2)==1) {
            SelectedSuit=j;
         } else {
            SelectedSuit=l;
         }
      }
   }

SelectCard:
   StartSuit=SelectedSuit;
   // Suit selected -> now select card

   // if this suit is not dangerous, enemies have more than 4 cards of this suit,
   // and we do not drain then toss out the maximum card
   k=0;        // calculate the number of cards of this suit for enemies
   for (i=0;i<13;i++) k+=PlayerCards[0][StartSuit][i];
   if ( (StartSuit!=Danger1) && (StartSuit!=Danger2) && (k>4) &&
        (Drain==0) && (BeCare[SelectedSuit]==0) ) {
      for (i=12;i>-1;i--) {
          if ((List[Player][i]>-1) && (List[Player][i]<((StartSuit+1)*13))) {
             SelectedCard=i;
             goto CardDone;
          }
      }
   } else {
      for (i=0;i<13;i++) {
          if (List[Player][i]>=(StartSuit*13)) {
             SelectedCard=i;
             goto CardDone;
          }
      }
   }
   goto CardDone;


AddCard:
   // if player has the suit
   if (PlayerSuit[StartSuit]>0) {
      SelectedSuit=StartSuit;
      // choose the maximum enemy card on the screen (this suit)
      k=0;
      MaxE=-1;
      for (i=0;i<4;i++) {
          if ( (Ready[0][i]>=(StartSuit*13)) &&
               (Ready[0][i]<((StartSuit+1)*13)) ) {
             if (MaxE<Ready[0][i]) MaxE=Ready[0][i];
          }
          if (Ready[0][i]>0) k++; // player counter
      }
      // choose a number: our min card < enemy max card
      MinP=-1;
      l=-1;
      for (i=0;i<13;i++) {
          if ( (List[Player][i]>=(StartSuit*13)) &&
               (List[Player][i]<((StartSuit+1)*13)) ) {
             j=i;                               // max our card of this suit
             if (l==-1) l=i;                    // min our card of this suit
             if (List[Player][i]<MaxE) MinP=i;  // suitable card
          }
      }
      if (MinP==-1) {  // no suitable card
         if (k==3) {
            MinP=j;  // take a bribe
         } else {
            MinP=l;  // there is a chance not to take
         }
      }
      // if this suit is not dangerous, enemies have more than 3 cards of this suit,
      // and there are no cards in the list that they can beat us and we do not drain,
      // then we toss out the maximum card
      k=0;        // calculate the number of cards of this suit for enemies
      for (i=0;i<13;i++) k+=PlayerCards[0][StartSuit][i];
      l=0;        // check for problems
      for (i=0;i<4;i++) {
          for (ii=0;ii<DTnumber;ii++) {
              if (Ready[0][i]==DONTtake[ii]) l=1;
          }
      }
      if ( (StartSuit!=Danger1) && (StartSuit!=Danger2) && (k>3) && (l==0) &&
           (Drain==0) && (BeCare[SelectedSuit]==0) ) MinP=j;
      SelectedCard=MinP;
      goto CardDone;
   }

   // fortunately, there is no such suit => choose from the list
   for (i=DTnumber-1;i>-1;i--) {
       for (j=0;j<13;j++) {
           if (List[Player][j]==DONTtake[i]) {
              SelectedCard=j;
              goto CardDone;
           }
       }
   }
   // if nothing found => drain the suit that we don't like
   // the suit is not the one (otherwise we have jumped before), there is no Hearts and no reuired suit => Player has 2 suits
   // searchibg for this suits
   k=-1;  l=-1;
   for (Suit=0; Suit<4; Suit++) {
       if (PlayerSuit[Suit]>0) {     // suit found
          if (k==-1) k=Suit;         // first suit
          l=Suit;                    // second suit
       }
   }
   // calculates first suit priorities
   Suit=k;
   k=0;
   for (i=0;i<13;i++) {
       if ((PlayerCards[Player][Suit][i])>0) {
          k+=(i+1)*10;
          j++;
       }
   }
   SuitPriority[Suit]=k/j;
   k=Suit;
   // calculate the second suit priority
   Suit=l;
   l=0;
   for (i=0;i<13;i++) {
       if ((PlayerCards[Player][Suit][i])>0) {
          l+=(i+1)*10;
          j++;
       }
   }
   SuitPriority[Suit]=l/j;
   l=Suit;

   if (SuitPriority[k]>SuitPriority[l]) {
      SelectedSuit=k;
      goto SelAddCard;
   }
   if (SuitPriority[l]>SuitPriority[k]) {
      SelectedSuit=l;
      goto SelAddCard;
   }
   i=rand();
   if ((i%2)==1) {
      SelectedSuit=l;
   } else {
      SelectedSuit=k;
   }

SelAddCard:
   // drain the suit => select max card
   for (i=12;i>-1;i--) {
       if ( (List[Player][i]>=SelectedSuit*13) &&
            (List[Player][i]<(SelectedSuit+1)*13) ) {
          SelectedCard=i;
          goto CardDone;
       }
   }

CardDone:
   TempList[0]=List[Player][SelectedCard];
   TempNum=1;
   List[Player][SelectedCard]=-1;

   /// ??? ///
   switch (Player) {
     case 1: Start_x=40;
             Start_y=20+SelectedCard*8;
             End_x=253;
             End_y=91;
             break;
     case 2: Start_x=170+SelectedCard*20;
             Start_y=20;
             End_x=284;
             End_y=81;
             break;
     case 3: Start_x=540;
             Start_y=20+SelectedCard*8;
             End_x=333;
             End_y=90;
             break;
   }
   Animate_Game();
   /// ??? ///

   Ready[0][Player]=TempList[0];

   Player++;
   if (Player>3) Player=0;
   if (Player==0) ShowCard();
   PlayIt(sample[0]);
}

int GetReverseKing()
// Select the suit and the card of the current player and "lay" the card on the table Ready[0]
{
   // build a table of cards in the player's hands and all the others (Enemies_Cards) in PlayerCards[0]
   //   2 3 4 ... J Q K A     - for each player
   // H 0 0 1 ... 0 0 1 0
   // C 1 0 0 ... 0 0 0 1
   // D 1 0 0 ... 1 0 0 0
   // S 0 1 0 ... 1 0 1 1
   for (Pl=0;Pl<4;Pl++) {
       for (i=0;i<13;i++) {
           for (Suit=0;Suit<4;Suit++) PlayerCards[Pl][Suit][i]=0;
       }
   }
   for (Pl=0;Pl<4;Pl++) {
       for (i=0;i<13;i++) {
           k=List[Pl][i];
           if ( (k>=cHearts2) && (k<=cHeartsA) ) {
              PlayerCards[Pl][0][k]=1;
           } else {
              if ( (k>=cCross2) && (k<=cCrossA) ) {
                 PlayerCards[Pl][1][k-13]=1;
              } else {
                 if ( (k>=cDiamonds2) && (k<=cDiamondsA) ) {
                    PlayerCards[Pl][2][k-26]=1;
                 } else {
                    if ( (k>=cSpades2) && (k<=cSpadesA) ) {
                       PlayerCards[Pl][3][k-39]=1;
                    }
                 }
              }
           }
       } // for (i=0;i<13;i++)
   } // for (Pl=0;Pl<4;i++)
   // Combine all PlayerCards except Player's into the one PlayerCards[0]
   for (Pl=1;Pl<4;Pl++) {
       if (Pl!=Player) {
          for (Suit=0;Suit<4;Suit++) {
              for (i=0;i<13;i++) PlayerCards[0][Suit][i]+=PlayerCards[Pl][Suit][i];
          }
       }
   }

   // the number of cards of each suit available in the Player's hands
   k=0;
   for (Suit=0; Suit<4; Suit++) {
       PlayerSuit[Suit]=0;
       for (i=0; i<13; i++) PlayerSuit[Suit]+=PlayerCards[Player][Suit][i];
       if (PlayerSuit[Suit]>0) {
          k++;
          j=Suit;
       }
   }
   if (k==1) {  // if the suit is only one then return it
      SelectedSuit=j;
      if (Player!=StartPlayer) goto AddCard;
      goto SelectCard;
   }

   // we divide the suit into those on which we take (1) and those on which we do not take (0)
   for (Suit=0; Suit<4; Suit++) {
       if (PlayerSuit[Suit]>0) {     // if Player has this suit
          // choose the minimum card of the given suit for enemies and the maximum card of the suit for Player
          MinE=-1; // enemies do not have it
          MaxP=0;
          for (i=12;i>-1;i--) if (PlayerCards[0][Suit][i]>0) MinE=i;
          for (i=0;i<13;i++)  if (PlayerCards[Player][Suit][i]>0) MaxP=i;
          // MaxP > MinE => take
          // MaxP < MinE => do not take
          // MinE = -1   => take
          if (MaxP < MinE) {
             PlayerNotTake[Suit]=0;  // do not take
          } else {
             PlayerNotTake[Suit]=1;  // take
          }
       } else {
          PlayerNotTake[Suit]=-1;    // nothing
       }
   }

   if (Player!=StartPlayer) goto AddCard;

   k=0;
   for (Suit=0; Suit<4; Suit++) {
       if (PlayerSuit[Suit]>0) {
          k+=PlayerNotTake[Suit];
          j=Suit;                             // the last suit
          if (PlayerNotTake[Suit]>0) i=Suit;  // the last suit on which we take
       }
   }
   if (k==0) {  // i there no suits on which we take -> return any
      SelectedSuit=j;
      goto SelectCard;
   }
   if (k==1) {  // i there is only one such suit
      if (i==DisableSuit) {
         SelectedSuit=j;  // if we can't go this suit -> return any
      } else {
         SelectedSuit=i;  // return this suit
      }
      goto SelectCard;
   }

   // calculate suit priorities for all suits on which we take -> SuitPriority
   for (Suit=0;Suit<4;Suit++) {
       if (PlayerNotTake[Suit]>0) { // if Player has this suit and we can take
          k=0;
          j=0;
          for (i=0;i<13;i++) {
              if ((PlayerCards[Player][Suit][i])>0) k+=(i+1)*10;
          }
          j=PlayerSuit[Suit];
          SuitPriority[Suit]=k/j;
       } else {
          SuitPriority[Suit]=0;  // for suits on which we can't take
       }
   }
   // selec two with max priority
   k=0; l=0;
   i=0; j=0;
   for (Suit=0; Suit<4; Suit++) {
       if (SuitPriority[Suit]>k) {
          k=SuitPriority[Suit];
          l=Suit;
       }
   }
   for (Suit=0; Suit<4; Suit++) {
       if ( (SuitPriority[Suit]>i) && (Suit!=l) ) {
          i=SuitPriority[Suit];
          j=Suit;
       }
   }
   if (j==DisableSuit) {       // if we can't go with this suit
      SelectedSuit=l;
      goto SelectCard;
   }
   if (l==DisableSuit) {       // if we can't go with this suit
      SelectedSuit=j;
      goto SelectCard;
   }
   if (i>k) {
      SelectedSuit=j;
   } else {
      if (k>i) {
         SelectedSuit=l;
      } else {
         i=rand();
         if ((i%2)==1) {
            SelectedSuit=j;
         } else {
            SelectedSuit=l;
         }
      }
   }

SelectCard:
   StartSuit=SelectedSuit;
   // Suit selected -> select card

   // toss out the maximum card
   for (i=12;i>-1;i--) {
       if ((List[Player][i]>-1) && (List[Player][i]<((StartSuit+1)*13))) {
          SelectedCard=i;
          goto CardDone;
       }
   }
   goto CardDone;


AddCard:
   // Toss card

   // if player has this suit
   if (PlayerSuit[StartSuit]>0) {
      SelectedSuit=StartSuit;
      // select maximum enemy card for this suit on the screen
      MaxE=-1;
      for (i=0;i<4;i++) {
          if ( (Ready[0][i]>=(StartSuit*13)) &&
               (Ready[0][i]<((StartSuit+1)*13)) ) {
             if (MaxE<Ready[0][i]) MaxE=Ready[0][i];
          }
      }
      // select number of our max card that greater of max enemy card of the same suit
      MaxP=-1;
      l=-1;
      for (i=0;i<13;i++) {
          if ( (List[Player][i]>=(StartSuit*13)) &&
               (List[Player][i]<((StartSuit+1)*13)) ) {
             if (l==-1) l=i;                    // min our card of this suit
             if ((MaxP==-1) && (List[Player][i]>MaxE)) MaxP=i;
                                                // good card
          }
      }
      if (MaxP==-1) MaxP=l;   // there is no suitable card - do not take a bribe
      SelectedCard=MaxP;
      goto CardDone;
   }

   // no such suit => toss the smallest not in the list
   MinP=-1;
   if (DTnumber>0) {
      for (j=0;j<13;j++) {
          l=0;
          for (i=0;i<DTnumber;i++) {
              if (List[Player][j]==DONTtake[i]) l=1;  // exists in list
          }
          if ( (l==0) && (List[Player][j]>-1) &&
               ((List[Player][j]%13)<(MinP%13)) ) MinP=List[Player][j];
      }
      if (MinP>-1) {
         SelectedCard=MinP;
         goto CardDone;
      }
   }

   // bad deal => toss the minimum card
   MinP=-1;
   for (j=0;j<13;j++) {
       if (List[Player][j]>-1) {
          if ((MinP==-1) ||
              ((List[Player][j]%13)<(List[Player][MinP]%13)) ) MinP=j;
       }
   }
   SelectedCard=MinP;

CardDone:
   TempList[0]=List[Player][SelectedCard];
   TempNum=1;
   List[Player][SelectedCard]=-1;

   /// ??? ///
   switch (Player) {
     case 1: Start_x=40;
             Start_y=20+SelectedCard*8;
             End_x=253;
             End_y=91;
             break;
     case 2: Start_x=170+SelectedCard*20;
             Start_y=20;
             End_x=284;
             End_y=81;
             break;
     case 3: Start_x=540;
             Start_y=20+SelectedCard*8;
             End_x=333;
             End_y=90;
             break;
   }
   Animate_Game();
   /// ??? ///

   Ready[0][Player]=TempList[0];

   Player++;
   if (Player>3) Player=0;
   if (Player==0) ShowCard();
   PlayIt(sample[0]);
}

int GetNine()
// Select suit and card for the current Player and place card on the table Ready[0]
{
   int i, j;

   // Build a table of cards available for Player
   //    2  3  4 ...  J  Q  K  A
   // C -1 -1  1 ... -1 -1  1 -1
   // K  1 -1 -1 ... -1 -1 -1  1
   // B  1 -1 -1 ...  1 -1 -1 -1
   // P -1  1 -1 ...  1 -1  1  1

   for (i=0;i<13;i++) {
       for (Suit=0;Suit<4;Suit++) PlayerCards[0][Suit][i]=-1;
   }
   for (i=0;i<12;i++) {
       k=List[Player][i];
       if ( (k>=cHearts6) && (k<=cHeartsA) ) {
          PlayerCards[0][0][k-cHearts6]=1;
          PlayerCards[1][0][k-cHearts6]=i;
       } else {
          if ( (k>=cCross6) && (k<=cCrossA) ) {
             PlayerCards[0][1][k-cCross6]=1;
             PlayerCards[1][1][k-cCross6]=i;
          } else {
             if ( (k>=cDiamonds6) && (k<=cDiamondsA) ) {
                PlayerCards[0][2][k-cDiamonds6]=1;
                PlayerCards[1][2][k-cDiamonds6]=i;
             } else {
                if ( (k>=cSpades6) && (k<=cSpadesA) ) {
                   PlayerCards[0][3][k-cSpades6]=1;
                   PlayerCards[1][3][k-cSpades6]=i;
                }
             }
          }
       }
   } // for (i=0;i<13;i++)

   MaxP=-1;
   // prioritize cards
   // k - the current priority (initially -1)
   for (Suit=0;Suit<4;Suit++) {
       k=-1;
       for (i=0;i<4;i++) {
           if (PlayerCards[0][Suit][i]>-1) {
              if (k<0) {
                 k=3-i;
                 PlayerCards[0][Suit][i]=k;
              } else {
                 PlayerCards[0][Suit][i]=k+1;
              }
              // max priority
              if (MaxP<PlayerCards[0][Suit][i]) MaxP=PlayerCards[0][Suit][i];
           }
       }
       j=PlayerCards[0][Suit][3]; // priority of nine
       k=-1;
       for (i=8;i>2;i--) {
           if (PlayerCards[0][Suit][i]>-1) {
              if (k<0) {
                 k=i-3;
                 PlayerCards[0][Suit][i]=k;
              } else {
                 PlayerCards[0][Suit][i]=k+1;
              }
              // max priority
              if (MaxP<PlayerCards[0][Suit][i]) MaxP=PlayerCards[0][Suit][i];
           }
       }
       // priority of nine
       if (j>PlayerCards[0][Suit][3]) PlayerCards[0][Suit][3]=j;
   }

   // looking for the card in the array with max priority
   // check if we can place it (loop on priority)
   // decrease priority MaxP by 1 after each pass
   while (MaxP>-1) {
      for (Suit=0;Suit<4;Suit++) {
          for (i=0;i<9;i++) {
              if (PlayerCards[0][Suit][i]==MaxP) {
                 if (i==3) {
                    SelectedCard=Suit*13+i+cHearts6;
                    goto CardDone;
                 } else {
                    if (i<3) {
                       if (Ready[Suit][i+1]>-1) {
                          SelectedCard=Suit*13+i+cHearts6;
                          goto CardDone;
                       }
                    } else {
                       if (Ready[Suit][i-1]>-1) {
                          SelectedCard=Suit*13+i+cHearts6;
                          goto CardDone;
                       }
                    }
                 }
              } // if (PlayerCards[0][Suit][i]==MaxP)
          }
      } // for (Suit=0;Suit<4;Suit++)
      MaxP--;
   }

   Score+=10;
   Player++;
   if (Player>2) Player=0;
   if (Player==0) ShowCard();
   return 0;

CardDone:
   j=PlayerCards[1][Suit][i];
   List[Player][j]=-1;

   /// ??? ///
   if (Player==1) Start_x=40;
   else           Start_x=540;
   Start_y=20+j*8;
   End_x=115+46*i;
   End_y=25+32*Suit;
   TempList[0]=SelectedCard;
   TempNum=1;
   Animate_Game();
   /// ??? ///

   Ready[Suit][i]=SelectedCard;

   Player++;
   if (Player>2) Player=0;
   if (Player==0) ShowCard();
   PlayIt(sample[0]);
}

// ======= PC goes first =======
StartFool()
{
   RunDelay();
   MinP=100;  // min card
   k=-1;      // min card index in the list
   for (i=0;i<ListNum[1];i++) {
       if ((List[1][i]%13<MinP) && (List[1][i]/13!=TrumpSuit)) {
          MinP=List[1][i]%13;
          k=i;
       }
   }
   if (k==-1) {
      for (i=0;i<ListNum[1];i++) {
          if (List[1][i]%13<MinP) {
             MinP=List[1][i]%13;
             k=i;
          }
      }
   }

   TempList[0]=List[1][k];
   TempNum=1;

   for (i=k;i<ListNum[1]-1;i++) {
       List[1][i]=List[1][i+1];
   }
   ListNum[1]--;

   /// ??? ///
   Start_x=300;
   Start_y=20;
   End_x=121;
   End_y=75;
   Animate_Game();
   /// ??? ///

   Ready[1][0]=TempList[0];

   Player=0;
   ShowCard();
   PlayIt(sample[0]);
}

// ======= PC defence (repel) =======
EndFool()
{
   RunDelay();
   MaxE=Ready[0][CurCard]%13;   // the value of the enemy card
   l=Ready[0][CurCard]/13;      // the suit of the enemy card
   MinP=100;                    // minimum suitable card
   k=-1;                        // the index of the minimum suitable card
   for (i=0;i<ListNum[1];i++) {
       if ((List[1][i]/13==l) && (List[1][i]%13>MaxE) &&
           (List[1][i]%13<MinP)) {
          MinP=List[1][i]%13;
          k=i;
       }
   }
   if ((k==-1) && (l!=TrumpSuit)) {
      for (i=0;i<ListNum[1];i++) {
          if ((List[1][i]/13==TrumpSuit) && (List[1][i]%13<MinP)) {
             MinP=List[1][i]%13;
             k=i;
          }
      }
   }

   if (k>-1) goto CardDone;    // fought off (defended)

   // bad defence - should take
   TempNum=0;
   for (i=0;i<6;i++) {
       if (Ready[1][i]>-1) {
          TempList[TempNum]=Ready[1][i];
          TempNum++;
          Ready[1][i]=-1;
       }
       if (Ready[0][i]>-1) {
          TempList[TempNum]=Ready[0][i];
          TempNum++;
          Ready[0][i]=-1;
       }
   }

   /// ??? ///
   Start_x=121;
   Start_y=75;
   End_x=300;
   End_y=20;
   Animate_Game();
   /// ??? ///

   for (i=0;i<TempNum;i++) {
       List[1][ListNum[1]]=TempList[i];
       ListNum[1]++;
   }

   // go to the next move
   HideCard();
   NextStepFool();
   ShowCard();
   Player=StartPlayer;
   RunDelay();
   return 0;

CardDone:
   TempList[0]=List[1][k];
   TempNum=1;
   for (i=k;i<ListNum[1]-1;i++) {
       List[1][i]=List[1][i+1];
   }
   ListNum[1]--;

   /// ??? ///
   Start_x=300;
   Start_y=20;
   End_x=121+CurCard*64;
   End_y=75;
   Animate_Game();
   /// ??? ///

   Ready[1][CurCard]=TempList[0];

   Player=0;
   ShowCard();
   CurCard++;
   PlayIt(sample[0]);
}

// ======= PC tosses =======
AddFool()
{
   RunDelay();
   //Name[0]=140;
   if ((ListNum[0]<1) || (CurCard>5)) goto StopAdd;
   //Name[0]=141;

   for (l=0;l<CurCard;l++) {
       MaxE=SrcDeck[Ready[0][l]].Value;     // max enemy card value
       MaxP=SrcDeck[Ready[1][l]].Value;     // max player card value
       k=-1;                    // the index of the suitable card in the list
       for (i=0;i<ListNum[1];i++) {
           j=List[1][i];
           if ( ((SrcDeck[j].Value==MaxE) ||
                 (SrcDeck[j].Value==MaxP)) &&
                (SrcDeck[j].Suit!=TrumpSuit) ) {
              k=i;
              goto CardDone;
           }
       }
   }
   //Name[0]=142;

   i=rand()%2;
   j=rand()%2;

   if ((i!=0) || (j!=0)) goto StopAdd;
   //Name[0]=143;

   for (l=0;l<CurCard;l++) {
       MaxE=SrcDeck[Ready[0][l]].Value;     // max enemy card value
       MaxP=SrcDeck[Ready[1][l]].Value;     // max player card value
       k=-1;                    // the index of the suitable card in the list
       for (i=0;i<ListNum[1];i++) {
           j=List[1][i];
           if ( (SrcDeck[j].Value==MaxE) ||
                (SrcDeck[j].Value==MaxP) ) {
              k=i;
              goto CardDone;
           }
       }
   }
   //Name[0]=144;

StopAdd:
   //Name[0]=145;
   // nothing to toss -> go to the next move
   RunDelay();
   HideCard();
   NextStepFool();
   ShowCard();
   StartPlayer=0;
   Player=0;
   return 0;

CardDone:
   //Name[0]=146;
   TempList[0]=List[1][k];
   TempNum=1;
   l=ListNum[1]-1;
   for (i=k;i<l;i++) {
       List[1][i]=List[1][i+1];
   }
   ListNum[1]--;

   /// ??? ///
   Start_x=300;
   Start_y=20;
   End_x=121+CurCard*64;
   End_y=75;
   Animate_Game();
   /// ??? ///
   //Name[0]=147;

   Ready[1][CurCard]=TempList[0];

   ShowCard();
   Player=0;
   PlayIt(sample[0]);
}

BJ_GetCard()
{
     RunDelay();
     if (Scores[1]<15) goto Next;
     switch (Scores[1]) {
       case 16: if ((rand()/328)<80) goto Next;
                break;
       case 17: if ((rand()/328)<50) goto Next;
                break;
       case 18: if ((rand()/328)<20) goto Next;
                break;
     }
     ReadyNum[1]=1;
     if (ReadyNum[0]==0) Player--;
     else                Player=2;
     Draw_BJ();
     return 0;

Next:
     TempNum=1;
     TempList[1]=Deck[CurDeck];
     TempList[0]=cBackV;
     CurDeck++;
     /// ??? ///
     Start_x=500;
     Start_y=90;
     End_x=300-5*13+ListNum[1]*30;
     End_y=30;
     Animate_Game();
     /// ??? ///
     PlayIt(sample[0]);
     List[1][ListNum[1]]=TempList[1];
     ListNum[1]++;
     CurDeck++;
     i=SrcDeck[TempList[1]].Value;
     if (i==cAce) i=11;
     else if (i>10) i=10;
     Scores[1]+=i;

     if (ReadyNum[0]==0) Player--;
     Draw_BJ();
}


// ======================== G A M E   A N I M A T E ====================== //


Animate_Game()
{
   short Scale_x, Scale_y, ii, jj;
   double D;

   Animate=1;

   Scale_x=End_x-Start_x;
   Scale_y=End_y-Start_y;
   D=abs(Scale_x*Scale_x)+abs(Scale_y*Scale_y);
   ii=sqrt(D)/25;
   Scale_x=Scale_x/ii;
   Scale_y=Scale_y/ii;

   for (jj=0;jj<ii;jj++) {
       Start_x+=Scale_x;
       Start_y+=Scale_y;
       Draw_Game();
   }

   Animate=0;
}


// ========================= G A M E   S E L E C T ======================= //


short CurPack_Temp;

Golf_Select()
{
        if ((PadData & PADLleft)>0) {
           CurPos--;
           if (CurPos<0) CurPos=0;
           Draw_Golf();
        }
        if ((PadData & PADLright)>0) {
           CurPos++;
           if (CurPos>6) CurPos=6;
           Draw_Golf();
        }
        if ((PadData & PADRright)>0) {
             // laying out cards
             if (ListNum[CurPos]<1) goto SkipO;
             SelectedCard=List[CurPos][ListNum[CurPos]-1];
             DstCard=PackList[CurPack];
             // looking for a place to put
             if (CanLayGolf()!=0) goto SkipO;

             ListNum[CurPos]--;
             /// ??? ///
             Start_x=Sprites[SelectedCard].x;
             Start_y=Sprites[SelectedCard].y;
             End_x=336;
             End_y=25;
             TempNum=1;
             TempList[0]=SelectedCard;
             Animate_Game();
             /// ??? ///
             PackList[CurPack]=SelectedCard;

             PlayIt(sample[0]);
        SkipO:
             Draw_Golf();
        }
        if ((PadData & PADRleft)>0) {
           if (CurPack<16) {
              /// ??? ///
              Start_x=238;
              Start_y=25;
              End_x=336;
              End_y=25;
              TempNum=1;
              TempList[0]=PackList[CurPack+1];
              Animate_Game();
              /// ??? ///

              PlayIt(sample[0]);
              CurPack++;
           }
           Draw_Golf();
        }
        ReadKey();
}

Soliter_Select()
{
        if ((PadData & PADLleft)>0) {
           CurPos--;
           if (CurPos<0) CurPos=0;
           if (CurPos==1) CurPos=2;
           if (CurPos==8) CurPos=1;
           Draw_Soliter();
        }
        if ((PadData & PADLright)>0) {
           CurPos++;
           if (CurPos>12) CurPos=12;
           if (CurPos==9) CurPos=8;
           if (CurPos==2) CurPos=9;
           Draw_Soliter();
        }
        if ((PadData & PADLdown)>0) {
           if ((CurPos==0) || (CurPos==1)) CurPos+=2;
           if ((CurPos>8) && (CurPos<13)) CurPos-=4;
           Draw_Soliter();
        }
        if ((PadData & PADLup)>0) {
           if ((CurPos==2) || (CurPos==3)) CurPos-=2;
           if ((CurPos>4) && (CurPos<9)) CurPos+=4;
           Draw_Soliter();
        }
        if ( ((PadData & PADRright)>0) &&
             // laying out
             ((CurPos>0) && (CurPos<9)) ) {
             if (Cursor==pDown) DeselectCard();
             if (CurPos==1) {
                if ((PackNum==0) | (CurPack<0)) goto SkipO;
                SelectedCard=PackList[CurPack];
             } else {
                if (ListNum[CurPos-2]<1) goto SkipO;
                SelectedCard=List[CurPos-2][ListNum[CurPos-2]-1];
             }
             // looking for a place to put
             bi=-1;
             for (ai=0;ai<4;ai++) {
                 if (ReadyNum[ai]>0) {
                    DstCard=Ready[ai][ReadyNum[ai]-1];
                    if (CanLayReady()==0) bi=ai;
                 } else {
                    if (SrcDeck[SelectedCard].Value==cAce) bi=ai;
                 }
             }
             if (bi<0) goto SkipO;      // not found -> skip
             if (CurPos==1) {           // put in place directly from the deck
                PackNum--;
                PackList[CurPack]=-1;
                if (PackNum>=1) {
                   while ( (CurPack>-1) && (PackList[CurPack]<0) ) CurPack--;
                   if (CurPack<0) {
                      PackLeft--;
                      CurPack=0;
                      while ( (CurPack<24) && (PackList[CurPack]<0) ) {
                         CurPack++;
                      }
                   }
                }
                /// ??? ///
                Start_x=Sprites[SelectedCard].x;
                Start_y=Sprites[SelectedCard].y;
                End_x=304+bi*78;
                End_y=22;
                TempNum=1;
                TempList[0]=SelectedCard;
                Animate_Game();
                /// ??? ///
                Ready[bi][ReadyNum[bi]]=SelectedCard;
                ReadyNum[bi]++;
                Score+=10;
             } else {
                ListNum[CurPos-2]--;
                if ( (ListNum[CurPos-2]<1) &&
                     (vBack[CurPos-2]>0) ) {
                   List[CurPos-2][ListNum[CurPos-2]]=Deck[CurDeck];
                   CurDeck++;
                   ListNum[CurPos-2]++;
                   vBack[CurPos-2]--;
                   Score+=5;
                }
                /// ??? ///
                Start_x=Sprites[SelectedCard].x;
                Start_y=Sprites[SelectedCard].y;
                End_x=304+bi*78;
                End_y=22;
                TempNum=1;
                TempList[0]=SelectedCard;
                Animate_Game();
                /// ??? ///
                Ready[bi][ReadyNum[bi]]=SelectedCard;
                ReadyNum[bi]++;
                Score+=10;
             }
             PlayIt(sample[0]);
        SkipO:
             Draw_Soliter();
        }
        if ((PadData & PADRup)>0) {
           if (Cursor==pDown) DeselectCard();
           Draw_Soliter();
        }
        if ((PadData & PADRdown)>0) {
           switch (CurPos) {
              case 0:
                   if (PackNum==0) break;
                   if (Cursor==pDown) break;
                   if (PackLeft>0) {
                      CurPack_Temp=CurPack;
                      CurPack_Temp++;
                      while ( (CurPack_Temp<24) && (PackList[CurPack_Temp]<0) ) {
                         CurPack_Temp++;
                      }

                      /// ??? ///
                      Start_x=38;
                      Start_y=22;
                      End_x=112;
                      End_y=22;
                      TempNum=1;
                      TempList[0]=PackList[CurPack_Temp];
                      Animate_Game();
                      /// ??? ///

                      CurPack=CurPack_Temp;
                      PackLeft--;
                   } else {
                      // substract from Score 100 (if <0 => =0)
                      if (PackNum>1) {
                         Score-=100;
                         if (Score<0) Score=0;
                      }
                      PackLeft=PackNum-1;
                      CurPack=0;
                      while ( (CurPack<24) && (PackList[CurPack]<0) ) {
                         CurPack++;
                      }
                   }
                   PlayIt(sample[0]);
                   break;
              case 1:
                   if ((PackNum==0) | (CurPack<0)) break;
                   if (Cursor==pSelect) {
                      SrcPos=CurPos;
                      SelectedCard=PackList[CurPack];
                      SelectCard();
                   } else {
                      DeselectCard();
                   }
                   break;
              case 2:
              case 3:
              case 4:
              case 5:
              case 6:
              case 7:
              case 8:
                   if (Cursor==pSelect) {
                      if (ListNum[CurPos-2]<1) break;
                      SrcPos=CurPos;
                      SelectedCard=List[CurPos-2][ListNum[CurPos-2]-1];
                      SelectCard();
                      break;
                   }
                   if (SrcPos==CurPos) {
                      DeselectCard();
                      break;
                   }
                   switch (SrcPos) {
                      case 1:
                           if (ListNum[CurPos-2]>0) {
                              // put not on an empty place
                              DstCard=List[CurPos-2][ListNum[CurPos-2]-1];
                              if (CanLay()>0) break;
                           } else {
                              // put only the king on the empty space
                              if (SrcDeck[SelectedCard].Value!=cKing) break;
                           }

                           PackNum--;
                           PackList[CurPack]=-1;
                           if (PackNum>0) {
                              while ( (CurPack>-1) && (PackList[CurPack]<0) ) {
                                 CurPack--;
                              }
                              if (CurPack<0) {
                                 PackLeft--;
                                 CurPack=0;
                                 while ( (CurPack<24) && (PackList[CurPack]<0) ) {
                                    CurPack++;
                                 }
                              }
                           }
                           DeselectCard();

                           /// ??? ///
                           Start_x=Sprites[SelectedCard].x;
                           Start_y=Sprites[SelectedCard].y;
                           End_x=49+(CurPos-2)*80;
                           End_y=80+ListNum[CurPos-2]*10;
                           TempNum=1;
                           TempList[0]=SelectedCard;
                           Animate_Game();
                           /// ??? ///

                           Score+=5;
                           PlayIt(sample[0]);

                           List[CurPos-2][ListNum[CurPos-2]]=SelectedCard;
                           ListNum[CurPos-2]++;

                           break;
                      case 2:
                      case 3:
                      case 4:
                      case 5:
                      case 6:
                      case 7:
                      case 8:
                           if (ListNum[CurPos-2]>0) {
                              // put not on an empty place
                              DstCard=List[CurPos-2][ListNum[CurPos-2]-1];
                              for (StartNum=0; StartNum<ListNum[SrcPos-2]; StartNum++) {
                                  SelectedCard=List[SrcPos-2][StartNum];
                                  if (CanLay()==0) goto CardIs;
                              }
                              break;
                           } else {
                              // put only the king on the empty space
                              if (SrcDeck[List[SrcPos-2][0]].Value!=cKing) break;
                              StartNum=0;
                           }
                      CardIs:
                           SelectedCard=List[SrcPos-2][ListNum[SrcPos-2]-1];

                           Start_x=49+(SrcPos-2)*80;
                           Start_y=80+StartNum*10;

                           for (i=StartNum; i<ListNum[SrcPos-2]; i++) {
                               TempList[i-StartNum]=List[SrcPos-2][i];
                           }
                           TempNum=ListNum[SrcPos-2]-StartNum;
                           ListNum[SrcPos-2]=StartNum;
                           if ( (ListNum[SrcPos-2]<1) &&
                                (vBack[SrcPos-2]>0) ) {
                              List[SrcPos-2][ListNum[SrcPos-2]]=Deck[CurDeck];
                              CurDeck++;
                              ListNum[SrcPos-2]++;
                              vBack[SrcPos-2]--;
                              Score+=5;
                           }
                           DeselectCard();

                           /// ??? ///
                           End_x=49+(CurPos-2)*80;
                           End_y=80+ListNum[CurPos-2]*10;
                           Animate_Game();
                           /// ??? ///

                           for (i=0; i<TempNum; i++) {
                               List[CurPos-2][ListNum[CurPos-2]]=TempList[i];
                               ListNum[CurPos-2]++;
                           }

                           PlayIt(sample[0]);
                   }
                   break;
              case 9:
              case 10:
              case 11:
              case 12:
                   if (Cursor==pSelect) break;
                   switch (SrcPos) {
                      case 1:
                           if (ReadyNum[CurPos-9]>0) {
                              // put not on an empty place
                              DstCard=Ready[CurPos-9][ReadyNum[CurPos-9]-1];
                              if (CanLayReady()>0) break;
                           } else {
                              // put only the king on the empty space
                              if (SrcDeck[SelectedCard].Value!=cAce) break;
                           }
                           DeselectCard();

                           PackNum--;
                           PackList[CurPack]=-1;
                           if (PackNum>0) {
                              while ( (CurPack>-1) && (PackList[CurPack]<0) ) {
                                 CurPack--;
                              }
                              if (CurPack<0) {
                                 PackLeft--;
                                 CurPack=0;
                                 while ( (CurPack<24) && (PackList[CurPack]<0) ) {
                                    CurPack++;
                                 }
                              }
                           }

                           /// ??? ///
                           Start_x=Sprites[SelectedCard].x;
                           Start_y=Sprites[SelectedCard].y;
                           End_x=304+(CurPos-9)*80;
                           End_y=22;
                           TempNum=1;
                           TempList[0]=SelectedCard;
                           Animate_Game();
                           /// ??? ///

                           Ready[CurPos-9][ReadyNum[CurPos-9]]=SelectedCard;
                           ReadyNum[CurPos-9]++;

                           Score+=10;
                           PlayIt(sample[0]);
                           break;
                      case 2:
                      case 3:
                      case 4:
                      case 5:
                      case 6:
                      case 7:
                      case 8:
                           if (ReadyNum[CurPos-9]>0) {
                              // put not on an empty place
                              DstCard=Ready[CurPos-9][ReadyNum[CurPos-9]-1];
                              if (CanLayReady()>0) break;
                           } else {
                              // put only the king on the empty space
                              if (SrcDeck[SelectedCard].Value!=cAce) break;
                           }
                           DeselectCard();

                           ListNum[SrcPos-2]--;
                           if ( (ListNum[SrcPos-2]<1) &&
                                (vBack[SrcPos-2]>0) ) {
                              List[SrcPos-2][ListNum[SrcPos-2]]=Deck[CurDeck];
                              CurDeck++;
                              ListNum[SrcPos-2]++;
                              vBack[SrcPos-2]--;
                              Score+=5;
                           }

                           /// ??? ///
                           Start_x=Sprites[SelectedCard].x;
                           Start_y=Sprites[SelectedCard].y;
                           End_x=304+(CurPos-9)*80;
                           End_y=22;
                           TempNum=1;
                           TempList[0]=SelectedCard;
                           Animate_Game();
                           /// ??? ///

                           Ready[CurPos-9][ReadyNum[CurPos-9]]=SelectedCard;
                           ReadyNum[CurPos-9]++;

                           Score+=10;
                           PlayIt(sample[0]);
                   }
                   break;
           }
           Draw_Soliter();
        }
        ReadKey();
}

FreeCell_Select()
{
        if ((PadData & PADLleft)>0) {
           CurPos--;
           if (CurPos<0) CurPos=0;
           if (CurPos==3) CurPos=4;
           if (CurPos==11) CurPos=3;
           Draw_FreeCell();
        }
        if ((PadData & PADLright)>0) {
           CurPos++;
           if (CurPos>15) CurPos=15;
           if (CurPos==12) CurPos=11;
           if (CurPos==4) CurPos=12;
           Draw_FreeCell();
        }
        if ((PadData & PADLdown)>0) {
           if ((CurPos>=0) && (CurPos<4)) CurPos+=4;
           if ((CurPos>11) && (CurPos<16)) CurPos-=4;
           Draw_FreeCell();
        }
        if ((PadData & PADLup)>0) {
           if ((CurPos>3) && (CurPos<8)) CurPos-=4;
           if ((CurPos>7) && (CurPos<12)) CurPos+=4;
           Draw_FreeCell();
        }
        if ( ((PadData & PADRright)>0) && (CurPos<12) ) {
             if (Cursor==pDown) DeselectCard();
             if (CurPos<4) {
                if (PackList[CurPos]<0) goto SkipO;
                SelectedCard=PackList[CurPos];
             } else {
                if (ListNum[CurPos-4]<1) goto SkipO;
                SelectedCard=List[CurPos-4][ListNum[CurPos-4]-1];
             }
             bi=-1;
             for (ai=0;ai<4;ai++) {
                 if (ReadyNum[ai]>0) {
                    DstCard=Ready[ai][ReadyNum[ai]-1];
                    if (CanLayReady()==0) bi=ai;
                 } else {
                    if (SrcDeck[SelectedCard].Value==cAce) bi=ai;
                 }
             }
             if (bi<0) goto SkipO;
             if (CurPos<4) {
                PackList[CurPos]=-1;
             } else {
                ListNum[CurPos-4]--;
             }

             /// ??? ///
             Start_x=Sprites[SelectedCard].x;
             Start_y=Sprites[SelectedCard].y;
             End_x=336+bi*68;
             End_y=20;
             TempNum=1;
             TempList[0]=SelectedCard;
             Animate_Game();
             /// ??? ///

             Ready[bi][ReadyNum[bi]]=SelectedCard;
             ReadyNum[bi]++;

             PlayIt(sample[0]);
        SkipO:
             Draw_FreeCell();
        }
        if ( ((PadData & PADRleft)>0) && (CurPos>3) && (CurPos<12) ) {
             if (Cursor==pDown) DeselectCard();
             if (ListNum[CurPos-4]<1) goto SkipS;
             SelectedCard=List[CurPos-4][ListNum[CurPos-4]-1];
             bi=-1;
             for (ai=0;ai<4;ai++) if (PackList[ai]<0) bi=ai;
             if (bi<0) goto SkipS;

             ListNum[CurPos-4]--;

             /// ??? ///
             Start_x=Sprites[SelectedCard].x;
             Start_y=Sprites[SelectedCard].y;
             End_x=40+bi*68;
             End_y=20;
             TempNum=1;
             TempList[0]=SelectedCard;
             Animate_Game();
             /// ??? ///

             PackList[bi]=SelectedCard;

             PlayIt(sample[0]);
        SkipS:
             Draw_FreeCell();
        }
        if ((PadData & PADRup)>0) {
           if (Cursor==pDown) DeselectCard();
           Draw_FreeCell();
        }
        if ((PadData & PADRdown)>0) {
           switch (CurPos) {
              case 0: // top left
              case 1:
              case 2:
              case 3:
                   if (Cursor==pSelect) {
                      if (PackList[CurPos]<0) break;
                      SrcPos=CurPos;
                      SelectedCard=PackList[CurPos];
                      SelectCard();
                      break;
                   }
                   if (SrcPos==CurPos) {
                      DeselectCard();
                      break;
                   }
                   if (PackList[CurPos]>-1) break;
                   switch (SrcPos) {
                      case 0:
                      case 1:
                      case 2:
                      case 3:
                           PackList[CurPos]=SelectedCard;
                           PackList[SrcPos]=-1;
                           DeselectCard();
                           PlayIt(sample[0]);
                           break;
                      case 4:
                      case 5:
                      case 6:
                      case 7:
                      case 8:
                      case 9:
                      case 10:
                      case 11:
                           DeselectCard();

                           ListNum[SrcPos-4]--;

                           /// ??? ///
                           Start_x=Sprites[SelectedCard].x;
                           Start_y=Sprites[SelectedCard].y;
                           End_x=40+CurPos*68;
                           End_y=20;
                           TempNum=1;
                           TempList[0]=SelectedCard;
                           Animate_Game();
                           /// ??? ///

                           PackList[CurPos]=SelectedCard;

                           PlayIt(sample[0]);
                           break;
                   }
                   break;
              case 4: // a stack at the bottom
              case 5:
              case 6:
              case 7:
              case 8:
              case 9:
              case 10:
              case 11:
                   if (Cursor==pSelect) {
                      if (ListNum[CurPos-4]<1) break;
                      SrcPos=CurPos;
                      SelectedCard=List[CurPos-4][ListNum[CurPos-4]-1];
                      SelectCard();
                      break;
                   }
                   if (SrcPos==CurPos) {
                      DeselectCard();
                      break;
                   }
                   switch (SrcPos) {
                      case 0:
                      case 1:
                      case 2:
                      case 3:
                           if (ListNum[CurPos-4]>0) {
                              // put not on an empty place
                              DstCard=List[CurPos-4][ListNum[CurPos-4]-1];
                              if (CanLay()>0) break;
                           }
                           DeselectCard();

                           PackList[SrcPos]=-1;

                           /// ??? ///
                           Start_x=Sprites[SelectedCard].x;
                           Start_y=Sprites[SelectedCard].y;
                           End_x=40+(CurPos-4)*70;
                           End_y=70+ListNum[CurPos-4]*10;
                           TempNum=1;
                           TempList[0]=SelectedCard;
                           Animate_Game();
                           /// ??? ///

                           List[CurPos-4][ListNum[CurPos-4]]=SelectedCard;
                           ListNum[CurPos-4]++;

                           PlayIt(sample[0]);
                           break;
                      case 4:
                      case 5:
                      case 6:
                      case 7:
                      case 8:
                      case 9:
                      case 10:
                      case 11:
                           if (ListNum[CurPos-4]<1) {
                              // put on the empty space !!!!!!!!!!!!!
                              FreeCells=0;
                              for (i=0; i<4; i++) {
                                  if (PackList[i]==-1) FreeCells++;
                              } // FreeCells - the number of free cells
                              for (StartNum=ListNum[SrcPos-4]-1; StartNum>-1; StartNum--) {
                                  if (FreeCells<1) break;
                                  SelectedCard=List[SrcPos-4][StartNum];
                                  if (StartNum>0) {
                                     NextCard=List[SrcPos-4][StartNum-1];
                                     if (MayBeLaid()!=0) break;
                                  } else {
                                    goto CardIs;
                                  }
                                  FreeCells--;
                              }
                              goto CardIs;
                           }
                           // put not on an empty place
                           DstCard=List[CurPos-4][ListNum[CurPos-4]-1];
                           for (StartNum=ListNum[SrcPos-4]-1; StartNum>-1; StartNum--) {
                               SelectedCard=List[SrcPos-4][StartNum];
                               if (CanLay()==0) goto CardIs;
                               if (StartNum>0) {
                                  NextCard=List[SrcPos-4][StartNum-1];
                                  if (MayBeLaid()!=0) break;
                               } else {
                                 goto CardIs2;
                               }
                           }
                      CardIs2:
                           SelectedCard=List[SrcPos-4][ListNum[SrcPos-4]-1];
                           break;
                      CardIs:
                           SelectedCard=List[SrcPos-4][ListNum[SrcPos-4]-1];
                           DeselectCard();

                           FreeCells=0;
                           for (i=0; i<4; i++) {
                               if (PackList[i]==-1) FreeCells++;
                           }
                           if ((ListNum[SrcPos-4]-1-StartNum)>FreeCells) break;


                           for (i=StartNum; i<ListNum[SrcPos-4]; i++) {
                               TempList[i-StartNum]=List[SrcPos-4][i];
                           }
                           TempNum=ListNum[SrcPos-4]-StartNum;

                           ListNum[SrcPos-4]=StartNum;

                           /// ??? ///
                           Start_x=Sprites[SelectedCard].x;
                           Start_y=Sprites[SelectedCard].y;
                           End_x=40+(CurPos-4)*70;
                           End_y=70+ListNum[CurPos-4]*10;
                           Animate_Game();
                           /// ??? ///

                           for (i=0; i<TempNum; i++) {
                               List[CurPos-4][ListNum[CurPos-4]]=
                                                    TempList[i];
                               ListNum[CurPos-4]++;
                           }

                           PlayIt(sample[0]);
                           break;
                   }
                   break;
              case 12: // upper right (ordered)
              case 13:
              case 14:
              case 15:
                   if (Cursor==pSelect) break;
                   switch (SrcPos) {
                      case 0:
                      case 1:
                      case 2:
                      case 3:
                           if (ReadyNum[CurPos-12]>0) {
                              // put not on an empty place
                              DstCard=Ready[CurPos-12][ReadyNum[CurPos-12]-1];
                              if (CanLayReady()>0) break;
                           } else {
                              // put only an Ace on an empty space
                              if (SrcDeck[SelectedCard].Value!=cAce) break;
                           }
                           DeselectCard();

                           PackList[SrcPos]=-1;

                           /// ??? ///
                           Start_x=Sprites[SelectedCard].x;
                           Start_y=Sprites[SelectedCard].y;
                           End_x=336+(CurPos-12)*68;
                           End_y=20;
                           TempNum=1;
                           TempList[0]=SelectedCard;
                           Animate_Game();
                           /// ??? ///

                           Ready[CurPos-12][ReadyNum[CurPos-12]]=SelectedCard;
                           ReadyNum[CurPos-12]++;

                           PlayIt(sample[0]);
                           break;
                      case 4:
                      case 5:
                      case 6:
                      case 7:
                      case 8:
                      case 9:
                      case 10:
                      case 11:
                           if (ReadyNum[CurPos-12]>0) {
                              // put not on an empty place
                              DstCard=Ready[CurPos-12][ReadyNum[CurPos-12]-1];
                              if (CanLayReady()>0) break;
                           } else {
                              // put only an Ace on an empty space
                              if (SrcDeck[SelectedCard].Value!=cAce) break;
                           }
                           DeselectCard();

                           ListNum[SrcPos-4]--;

                           /// ??? ///
                           Start_x=Sprites[SelectedCard].x;
                           Start_y=Sprites[SelectedCard].y;
                           End_x=336+(CurPos-12)*68;
                           End_y=20;
                           TempNum=1;
                           TempList[0]=SelectedCard;
                           Animate_Game();
                           /// ??? ///

                           Ready[CurPos-12][ReadyNum[CurPos-12]]=SelectedCard;
                           ReadyNum[CurPos-12]++;

                           PlayIt(sample[0]);
                           break;
                   }
                   break;
           }
           Draw_FreeCell();
        }

        ReadKey();
}

Fourteen_Select()
{
        if ((PadData & PADLleft)>0) {
           CurPos--;
           if (CurPos<0) CurPos=0;
           if (CurPos==3) CurPos=4;
           if (CurPos==7) CurPos=8;
           Draw_Fourteen();
        }
        if ((PadData & PADLright)>0) {
           CurPos++;
           if (CurPos>11) CurPos=11;
           if (CurPos==4) CurPos=3;
           if (CurPos==8) CurPos=7;
           Draw_Fourteen();
        }
        if ((PadData & PADLdown)>0) {
           if (CurPos<8) CurPos+=4;
           Draw_Fourteen();
        }
        if ((PadData & PADLup)>0) {
           if (CurPos>3) CurPos-=4;
           Draw_Fourteen();
        }
        if ((PadData & PADRup)>0) {
           if (Cursor==pDown) DeselectCard();
           Draw_Fourteen();
        }
        if ((PadData & PADRdown)>0) {
           if (Cursor==pSelect) {
              if (ListNum[CurPos]<1) goto SkipX;
              SrcPos=CurPos;
              SelectedCard=List[CurPos][ListNum[CurPos]-1];
              SelectCard();
              goto SkipX;
           }
           if (SrcPos==CurPos) {
              DeselectCard();
              goto SkipX;
           }

           if (ListNum[CurPos]<1) goto SkipX;
           DstCard=List[CurPos][ListNum[CurPos]-1];
           if (CanLay14()!=0) goto SkipX;
           ListNum[SrcPos]--;
           DeselectCard();
           /// ??? ///
           Start_x=Sprites[SelectedCard].x;
           Start_y=Sprites[SelectedCard].y;
           End_x=Sprites[DstCard].x;
           End_y=Sprites[DstCard].y;
           TempNum=1;
           TempList[0]=SelectedCard;
           Animate_Game();
           /// ??? ///
           ListNum[CurPos]--;

           PlayIt(sample[0]);

        SkipX:
           Draw_Fourteen();
        }

        ReadKey();
}

Hearts_Select()
{
        if ((En==0) && ((PadData & PADLleft)>0)) {
           HideCard();
           k=-1;
           for (i=0;i<CurPos;i++) if (List[0][i]>-1) k=i;
           if (k>-1) CurPos=k;
           ShowCard();
           Draw_Hearts();
        }
        if ((En==0) && ((PadData & PADLright)>0)) {
           HideCard();
           k=-1;
           for (i=12;i>CurPos;i--) if (List[0][i]>-1) k=i;
           if (k>-1) CurPos=k;
           ShowCard();
           Draw_Hearts();
        }
        if ((PadData & PADRdown)>0) {
           if (En==1) {
              En=0;
              for (i=0;i<4;i++) Ready[0][i]=-1;
              if (Player==0) ShowCard();
              goto SkipX;
           }
           if ((Player==0) && (En==0)) {
              k=-1;                 // index of 2 Cross
              l=0;                  // cards of NOT forbidden suit
              j=0;                  // cards of the right suit
              for (i=0;i<13;i++) {
                  if (List[0][i]==cCross2) k=i;
                  if ( (List[0][i]>-1) && ((List[0][i]<DisableSuit*13) ||
                       (List[0][i]>=(DisableSuit+1)*13)) ) l=1;
                  if (StartPlayer!=0) {
                     if ((List[0][i]>=StartSuit*13) &&
                         (List[0][i]<(StartSuit+1)*13) ) j=1;
                  }
              }
              if ((k>-1) && (CurPos!=k)) goto SkipX;
              if (List[0][CurPos]<0) goto SkipX;
              if (j==1) {
                 if ( (List[0][CurPos]<StartSuit*13) ||
                      (List[0][CurPos]>=(StartSuit+1)*13) ) goto SkipX;
              }
              if ((l==1) && ((List[0][CurPos]>=DisableSuit*13) &&
                  (List[0][CurPos]<(DisableSuit+1)*13)) &&
                  (StartPlayer==0)) goto SkipX;

              TempNum=1;
              TempList[0]=List[0][CurPos];
              if (StartPlayer==0) StartSuit=List[0][CurPos]/13;
              HideCard();
              List[0][CurPos]=-1;

              /// ??? ///
              Start_x=270-13*13+CurPos*26;
              Start_y=162;
              End_x=301;
              End_y=101;
              Animate_Game();
              /// ??? ///

              Ready[0][0]=TempList[0];

              Player++;
              PlayIt(sample[0]);

              k=-1;
              for (i=0;i<CurPos;i++) if (List[0][i]>-1) k=i;
              if (k>-1) {
                 CurPos=k;
              } else {
                 for (i=12;i>CurPos;i--) if (List[0][i]>-1) k=i;
                 if (k>-1) CurPos=k;
              }
           }
        SkipX:
           //ShowCard();
           Draw_Hearts();
        }

        ReadKey();
}

King_Select()
{
        if ((En==0) && ((PadData & PADLleft)>0)) {
           HideCard();
           k=-1;
           for (i=0;i<CurPos;i++) if (List[0][i]>-1) k=i;
           if (k>-1) CurPos=k;
           ShowCard();
           Draw_King();
        }
        if ((En==0) && ((PadData & PADLright)>0)) {
           HideCard();
           k=-1;
           for (i=7;i>CurPos;i--) if (List[0][i]>-1) k=i;
           if (k>-1) CurPos=k;
           ShowCard();
           Draw_King();
        }
        if ((PadData & PADRdown)>0) {
           if (En==1) {
              En=0;
              for (i=0;i<4;i++) Ready[0][i]=-1;
              if (CardOnHand==0) {
                 InitMsg(GM_SCORE_ADDR);
                 MyMsg=wTheScore;
                 Draw_King();
                 RunDelay();
                 PadData=0;
                 while ((PadData & PADRdown)==0) ReadKey();
                 PadData=0;
                 MyMsg=-1;

                 GameNumber++;
                 CheckRules();
                 ResortKing();
                 if (GameNumber==8) {
                    for (i=0;i<4;i++) Scores[i]=-Scores[i];
                 }
              }
              if (Player==0) ShowCard();

              goto SkipX;
           }
           if ((Player==0) && (En==0)) {
              l=0;                  // cards of NOT forbidden suit
              j=0;                  // cards of the right suit
              for (i=0;i<13;i++) {
                  if ( ((List[0][i]>-1)&&(List[0][i]<DisableSuit*13)) ||
                       (List[0][i]>=(DisableSuit+1)*13)) l=1;
                  if (StartPlayer!=0) {
                     if ((List[0][i]>=StartSuit*13) &&
                         (List[0][i]<(StartSuit+1)*13) ) j=1;
                  }
              }
              if (List[0][CurPos]<0) goto SkipX;
              if (j==1) {
                 if ( (List[0][CurPos]<StartSuit*13) ||
                      (List[0][CurPos]>=(StartSuit+1)*13) ) goto SkipX;
              }
              if ((l==1) && ((List[0][CurPos]>=DisableSuit*13) &&
                  (List[0][CurPos]<(DisableSuit+1)*13)) &&
                  (StartPlayer==0)) goto SkipX;

              TempNum=1;
              TempList[0]=List[0][CurPos];
              if (StartPlayer==0) StartSuit=List[0][CurPos]/13;
              HideCard();
              List[0][CurPos]=-1;

              /// ??? ///
              Start_x=300-15*8+CurPos*30;
              Start_y=162;
              End_x=301;
              End_y=101;
              Animate_Game();
              /// ??? ///

              Ready[0][0]=TempList[0];

              Player++;
              PlayIt(sample[0]);

              k=-1;
              for (i=0;i<CurPos;i++) if (List[0][i]>-1) k=i;
              if (k>-1) {
                 CurPos=k;
              } else {
                 for (i=7;i>CurPos;i--) if (List[0][i]>-1) k=i;
                 if (k>-1) CurPos=k;
              }
           }
        SkipX:
           Draw_King();
        }

        ReadKey();
}

Nine_Select()
{
        short j, i;

        if ((PadData & PADLleft)>0) {
           HideCard();
           k=-1;
           for (i=0;i<CurPos;i++) if (List[0][i]>-1) k=i;
           if (k>-1) CurPos=k;
           ShowCard();
           Draw_Nine();
        }
        if ((PadData & PADLright)>0) {
           HideCard();
           k=-1;
           for (i=11;i>CurPos;i--) if (List[0][i]>-1) k=i;
           if (k>-1) CurPos=k;
           ShowCard();
           Draw_Nine();
        }
        if ((PadData & PADRdown)>0) {
           if (List[0][CurPos]==-1) goto SkipX;

           j=List[0][CurPos]/13;          // suit
           i=List[0][CurPos]%13-cHearts6; // card

           if ((i<3) && (Ready[j][i+1]==-1)) goto SkipX;
           if ((i>3) && (Ready[j][i-1]==-1)) goto SkipX;

           TempNum=1;
           TempList[0]=List[0][CurPos];
           HideCard();
           List[0][CurPos]=-1;

           /// ??? ///
           Start_x=300-12*15+CurPos*30;
           Start_y=167;
           End_x=115+46*i;
           End_y=25+32*j;
           Animate_Game();
           /// ??? ///

           Ready[j][i]=TempList[0];

           Player++;
           PlayIt(sample[0]);

           k=-1;
           for (i=0;i<CurPos;i++) if (List[0][i]>-1) k=i;
           if (k>-1) {
              CurPos=k;
           } else {
              for (i=11;i>CurPos;i--) if (List[0][i]>-1) k=i;
              if (k>-1) CurPos=k;
           }

        SkipX:
           //ShowCard();
           Draw_Nine();
        }

        ReadKey();
}

Fool_Select()
{
        if ((PadData & PADLleft)>0) {
           HideCard();
           k=-1;
           for (i=0;i<CurPos;i++) if (List[0][i]>-1) k=i;
           if (k>-1) CurPos=k;
           ShowCard();
           Draw_Fool();
        }
        if ((PadData & PADLright)>0) {
           HideCard();
           k=-1;
           for (i=ListNum[0]-1;i>CurPos;i--) if (List[0][i]>-1) k=i;
           if (k>-1) CurPos=k;
           ShowCard();
           Draw_Fool();
        }
        if ( ((PadData & PADRright)>0) && (StartPlayer==1) ) {
           // take
           TempNum=0;
           for (i=0;i<6;i++) {
               if (Ready[1][i]>-1) {
                  TempList[TempNum]=Ready[1][i];
                  TempNum++;
                  Ready[1][i]=-1;
               }
               if (Ready[0][i]>-1) {
                  TempList[TempNum]=Ready[0][i];
                  TempNum++;
                  Ready[0][i]=-1;
               }
           }
           HideCard();

           /// ??? ///
           Start_x=121;
           Start_y=75;
           End_x=300;
           End_y=162;
           Animate_Game();
           /// ??? ///

           for (i=0;i<TempNum;i++) {
               List[0][ListNum[0]]=TempList[i];
               ListNum[0]++;
           }

           // go to the next move
           NextStepFool();
           Player=StartPlayer;
           //ShowCard();
           Draw_Fool();
        }
        if ( ((PadData & PADRleft)>0) && (StartPlayer==0) && (CurCard>0) ) {
           // go to the next move
           HideCard();
           NextStepFool();
           StartPlayer=1;
           Player=1;
           //ShowCard();
           Draw_Fool();
        }
        if ((PadData & PADRdown)>0) {
           if (StartPlayer==0) {
              if ((ListNum[1]<1) || (CurCard>5)) goto SkipX;
              // Player toss
              if (CurCard>0) {
                 k=0;                         // card could not be tossed
                 for (i=0;i<CurCard;i++) {
                     MaxE=Ready[0][i]%13;     // enemy card value
                     MaxP=Ready[1][i]%13;     // player card value
                     if ((List[0][CurPos]%13==MaxE) ||
                        (List[0][CurPos]%13==MaxP)) {
                        k=1;                  // can be tossed
                     }
                 }
                 if (k==0) goto SkipX;
              }
           } else {
              // Player fights back
              // checking card value (%13 not necessary) and suit
              if ( !((List[0][CurPos]>Ready[1][CurCard]) &&
                     (List[0][CurPos]/13==Ready[1][CurCard]/13) ||
                     (List[0][CurPos]/13==TrumpSuit) &&  // trump
                     (Ready[1][CurCard]/13!=TrumpSuit)) ) goto SkipX;
           }

           TempList[0]=List[0][CurPos];
           TempNum=1;
           HideCard();

           for (i=CurPos;i<ListNum[0]-1;i++) {
               List[0][i]=List[0][i+1];
           }
           ListNum[0]--;

           /// ??? ///
           Start_x=300;
           Start_y=162;
           End_x=124+CurCard*64;
           End_y=105;
           Animate_Game();
           /// ??? ///

           Ready[0][CurCard]=TempList[0];

           Player=1;
           PlayIt(sample[0]);

           if ((CurPos<0) || (CurPos>=ListNum[0])) CurPos=0;
           k=30;
           if (ListNum[0]>17) k=20;
           if (ListNum[0]>26) k=14;
           for (i=0; i<ListNum[0]; i++) {
               Cur[i].x=299-k*ListNum[0]/2+i*k;
               Cur[i].y=170;
           }

           if (StartPlayer==1) CurCard++;

        SkipX:
           //ShowCard();
           Draw_Fool();
        }

        ReadKey();
}

BJ_Select()
{
        if ((PadData & PADRdown)>0) {   // one more card
              TempNum=1;
              TempList[1]=Deck[CurDeck];
              TempList[0]=cBackV;
              CurDeck++;
              /// ??? ///
              Start_x=500;
              Start_y=90;
              End_x=300-5*13+ListNum[0]*30;
              End_x=300;
              End_y=150;
              Animate_Game();
              PlayIt(sample[0]);
              /// ??? ///
              List[0][ListNum[0]]=TempList[1];
              ListNum[0]++;
              i=SrcDeck[TempList[1]].Value;
              if (i==cAce) i=11;
              else if (i>10) i=10;
              Scores[0]+=i;
              Draw_BJ();
              if (ReadyNum[1]==0) Player++;
        }

        if ((PadData & PADRleft)>0) {   // pass a move to PC
           ReadyNum[0]=1;
           if (ReadyNum[1]==0) Player++;
           else                Player=2;
        }

        ReadKey();
}


// ======================== D R A W   G A M E S ======================== //


Draw_Golf()
{
    int activeBuff;

    for (ai=0;ai<4;ai++) {
        activeBuff=GsGetActiveBuff();
        GsSetWorkBase((PACKET*)GpuPacketArea[activeBuff]);
        GsClearOt(0, 0, &WorldOT[activeBuff]);

        GsSortSprite(&Sprites[bgLUp], &WorldOT[activeBuff], Priority[bgLUp]);
        GsSortSprite(&Sprites[bgMUp], &WorldOT[activeBuff], Priority[bgLUp]);
        GsSortSprite(&Sprites[bgRUp], &WorldOT[activeBuff], Priority[bgLUp]);

        if (MyMsg>-1) {
           Sprites[TheMsg].x=(640-Sprites[TheMsg].w)/2;
           Sprites[TheMsg].y=(240-Sprites[TheMsg].h)/2;
           GsSortSprite(&Sprites[TheMsg], &WorldOT[activeBuff], 2);
        }

        if (Animate>0) {
           for (i=TempNum-1;i>-1;i--) {
               Sprites[TempList[i]].x=Start_x;
               Sprites[TempList[i]].y=Start_y+i*10;
               GsSortSprite(&Sprites[TempList[i]], &WorldOT[activeBuff],
                                                        Priority[cHearts2]);
           }
        }

        for (i=0; i<8; i++) {
            j=Name[i];
            Sprites[j].x = 320-96+i*24;      Sprites[j].y = 208;
            GsSortSprite(&Sprites[j], &WorldOT[activeBuff], Priority[101]);
        }

        if (Animate==0) {
           Sprites[Cursor].x=Cur[CurPos].x;
           Sprites[Cursor].y=Cur[CurPos].y;
           GsSortSprite(&Sprites[Cursor], &WorldOT[activeBuff], Priority[Cursor]);
        }

        if (CurPack<10) {
           Sprites[cPackV].x = 234;      Sprites[cPackV].y = 21;
           GsSortSprite(&Sprites[cPackV], &WorldOT[activeBuff], Priority[cPackV]);
        } else {
           if (CurPack<14) {
              Sprites[cPackV1].x = 234;      Sprites[cPackV1].y = 21;
              GsSortSprite(&Sprites[cPackV1], &WorldOT[activeBuff], Priority[cPackV]);
           } else {
              if (CurPack<16) {
                 Sprites[cBackV].x = 234;      Sprites[cBackV].y = 21;
                 GsSortSprite(&Sprites[cBackV], &WorldOT[activeBuff], Priority[cPackV]);
              }
           }
        }

        Sprites[PackList[CurPack]].x = 336;
        Sprites[PackList[CurPack]].y = 25;
        GsSortSprite(&Sprites[PackList[CurPack]], &WorldOT[activeBuff],
                                                       Priority[cHearts2]);

        for (i=0; i<7; i++) {
            for (j=ListNum[i]-1; j>-1; j--) {
                Sprites[List[i][j]].y = 80+j*12;
                Sprites[List[i][j]].x = 49+i*80;
                GsSortSprite(&Sprites[List[i][j]], &WorldOT[activeBuff],
                                                       Priority[cHearts2]);
            }
        }

        VSync(0);
        GsSwapDispBuff();
        //GsSortClear(0, 0, 0, &WorldOT[activeBuff]);

        GsDrawOt(&WorldOT[activeBuff]);
    }
}

Draw_Soliter()
{
    int activeBuff;

    for (ai=0;ai<4;ai++) {
        activeBuff=GsGetActiveBuff();
        GsSetWorkBase((PACKET*)GpuPacketArea[activeBuff]);
        GsClearOt(0, 0, &WorldOT[activeBuff]);

        GsSortSprite(&Sprites[bgLUp], &WorldOT[activeBuff], Priority[bgLUp]);
        GsSortSprite(&Sprites[bgMUp], &WorldOT[activeBuff], Priority[bgLUp]);
        GsSortSprite(&Sprites[bgRUp], &WorldOT[activeBuff], Priority[bgLUp]);

        if (MyMsg>-1) {
           if (MyMsg==wScore) {
              i=Score;
              k=1;
              while (i>0) {
                    j=(i%10)+140;
                    Sprites[j].x = 350-k*24;      Sprites[j].y = 109;
                    GsSortSprite(&Sprites[j], &WorldOT[activeBuff], 1);
                    i/=10;
                    k++;
              }
           }
           Sprites[TheMsg].x=(640-Sprites[TheMsg].w)/2;
           Sprites[TheMsg].y=(240-Sprites[TheMsg].h)/2;
           GsSortSprite(&Sprites[TheMsg], &WorldOT[activeBuff], 2);
        }

        if (Animate>0) {
           for (i=TempNum-1;i>-1;i--) {
               Sprites[TempList[i]].x=Start_x;
               Sprites[TempList[i]].y=Start_y+i*10;
               GsSortSprite(&Sprites[TempList[i]], &WorldOT[activeBuff],
                                                        Priority[cHearts2]);
           }
        }

        for (i=0; i<8; i++) {
            j=Name[i];
            Sprites[j].x = 320-96+i*24;      Sprites[j].y = 208;
            GsSortSprite(&Sprites[j], &WorldOT[activeBuff], Priority[101]);
        }

        i=Score;
        k=1;
        while (i>0) {
              j=(i%10)+140;
              Sprites[j].x = 600-k*24;      Sprites[j].y = 208;
              GsSortSprite(&Sprites[j], &WorldOT[activeBuff], Priority[101]);
              i/=10;
              k++;
        }
        if (Score==0) {
           Sprites[140].x = 600-24;      Sprites[140].y = 208;
           GsSortSprite(&Sprites[140], &WorldOT[activeBuff], Priority[101]);
        }

        if (Animate==0) {
           Sprites[Cursor].x=Cur[CurPos].x;
           Sprites[Cursor].y=Cur[CurPos].y;
           GsSortSprite(&Sprites[Cursor], &WorldOT[activeBuff], Priority[Cursor]);
        }

        if (PackLeft>6) {
           Sprites[cPackV].x = 34;      Sprites[cPackV].y = 18;
           GsSortSprite(&Sprites[cPackV], &WorldOT[activeBuff], Priority[cPackV]);
        } else {
           if (PackLeft>2) {
              Sprites[cPackV1].x = 34;      Sprites[cPackV1].y = 18;
              GsSortSprite(&Sprites[cPackV1], &WorldOT[activeBuff], Priority[cPackV]);
           } else {
              if (PackLeft>0) {
                 Sprites[cBackV].x = 34;      Sprites[cBackV].y = 18;
                 GsSortSprite(&Sprites[cBackV], &WorldOT[activeBuff], Priority[cPackV]);
              }
           }
        }

        if ( (CurPack<24) && (CurPack>-1) && (PackList[CurPack]!=-1) ) {
           Sprites[PackList[CurPack]].x = 112;
           Sprites[PackList[CurPack]].y = 22;
           GsSortSprite(&Sprites[PackList[CurPack]], &WorldOT[activeBuff],
                                                       Priority[cHearts2]);
        }

        for (i=0; i<4; i++) {
            j=ReadyNum[i]-1;
            if (j>-1) {
               Sprites[Ready[i][j]].y = 22;
               Sprites[Ready[i][j]].x = 304+i*78;
               GsSortSprite(&Sprites[Ready[i][j]], &WorldOT[activeBuff],
                                                       Priority[cHearts2]);
            }
        }

        for (i=0; i<7; i++) {
            if (vBack[i]>0) {
               Sprites[cTop+vBack[i]].y = 80-vBack[i]*2;
               Sprites[cTop+vBack[i]].x = 41+i*80;
               GsSortSprite(&Sprites[cTop+vBack[i]], &WorldOT[activeBuff], Priority[cBackV]);
            }
        }

        for (i=0; i<7; i++) {
            for (j=ListNum[i]-1; j>-1; j--) {
                Sprites[List[i][j]].y = 80+j*10;
                Sprites[List[i][j]].x = 41+i*80;
                GsSortSprite(&Sprites[List[i][j]], &WorldOT[activeBuff],
                                                       Priority[cHearts2]);
            }
        }

        VSync(0);
        GsSwapDispBuff();
        //GsSortClear(0, 0, 0, &WorldOT[activeBuff]);

        GsDrawOt(&WorldOT[activeBuff]);
    }
}

Draw_FreeCell()
{
    int activeBuff;

    for (ai=0;ai<4;ai++) {
        activeBuff=GsGetActiveBuff();
        GsSetWorkBase((PACKET*)GpuPacketArea[activeBuff]);
        GsClearOt(0, 0, &WorldOT[activeBuff]);

        if (MyMsg>-1) {
           Sprites[TheMsg].x=(640-Sprites[TheMsg].w)/2;
           Sprites[TheMsg].y=(240-Sprites[TheMsg].h)/2;
           GsSortSprite(&Sprites[TheMsg], &WorldOT[activeBuff], 2);
        }

        GsSortSprite(&Sprites[bgLUp], &WorldOT[activeBuff], Priority[bgLUp]);
        GsSortSprite(&Sprites[bgMUp], &WorldOT[activeBuff], Priority[bgLUp]);
        GsSortSprite(&Sprites[bgRUp], &WorldOT[activeBuff], Priority[bgLUp]);

        if (Animate>0) {
           for (i=TempNum-1;i>-1;i--) {
               Sprites[TempList[i]].x=Start_x;
               Sprites[TempList[i]].y=Start_y+i*10;
               GsSortSprite(&Sprites[TempList[i]], &WorldOT[activeBuff],
                                                        Priority[cHearts2]);
           }
        }

        for (i=0; i<8; i++) {
            j=Name[i];
            Sprites[j].x = 320-96+i*24;      Sprites[j].y = 208;
            GsSortSprite(&Sprites[j], &WorldOT[activeBuff], Priority[101]);
        }

        if (Animate==0) {
           Sprites[Cursor].x=Cur[CurPos].x;
           Sprites[Cursor].y=Cur[CurPos].y;
           GsSortSprite(&Sprites[Cursor], &WorldOT[activeBuff], Priority[Cursor]);
        }

        for (i=0; i<4; i++) { // Temporary
            j=PackList[i];
            if (j>-1) {
               Sprites[j].y = 20;
               Sprites[j].x = 40+i*68;
               GsSortSprite(&Sprites[j], &WorldOT[activeBuff],
                                                       Priority[cHearts2]);
            }
        }

        for (i=0; i<4; i++) {
            j=ReadyNum[i]-1;
            if (j>-1) {
               Sprites[Ready[i][j]].y = 20;
               Sprites[Ready[i][j]].x = 336+i*68;
               GsSortSprite(&Sprites[Ready[i][j]], &WorldOT[activeBuff],
                                                       Priority[cHearts2]);
            }
        }

        for (i=0; i<8; i++) {
            for (j=ListNum[i]-1; j>-1; j--) {
                Sprites[List[i][j]].y = 70+j*10;
                Sprites[List[i][j]].x = 49+i*70;
                GsSortSprite(&Sprites[List[i][j]], &WorldOT[activeBuff],
                                                       Priority[cHearts2]);
            }
        }

        VSync(0);
        GsSwapDispBuff();
        //GsSortClear(0, 0, 0, &WorldOT[activeBuff]);

        GsDrawOt(&WorldOT[activeBuff]);
    }
}

Draw_Fourteen()
{
    int activeBuff;

    for (ai=0;ai<4;ai++) {
        activeBuff=GsGetActiveBuff();
        GsSetWorkBase((PACKET*)GpuPacketArea[activeBuff]);
        GsClearOt(0, 0, &WorldOT[activeBuff]);

        if (MyMsg>-1) {
           Sprites[TheMsg].x=(640-Sprites[TheMsg].w)/2;
           Sprites[TheMsg].y=(240-Sprites[TheMsg].h)/2;
           GsSortSprite(&Sprites[TheMsg], &WorldOT[activeBuff], 2);
        }

        GsSortSprite(&Sprites[bgLUp], &WorldOT[activeBuff], Priority[bgLUp]);
        GsSortSprite(&Sprites[bgMUp], &WorldOT[activeBuff], Priority[bgLUp]);
        GsSortSprite(&Sprites[bgRUp], &WorldOT[activeBuff], Priority[bgLUp]);

        if (Animate>0) {
           for (i=TempNum-1;i>-1;i--) {
               Sprites[TempList[i]].x=Start_x;
               Sprites[TempList[i]].y=Start_y+i*10;
               GsSortSprite(&Sprites[TempList[i]], &WorldOT[activeBuff],
                                                        Priority[cHearts2]);
           }
        }

        for (i=0; i<8; i++) {
            j=Name[i];
            Sprites[j].x = 320-96+i*24;      Sprites[j].y = 208;
            GsSortSprite(&Sprites[j], &WorldOT[activeBuff], Priority[101]);
        }

        if (Animate==0) {
           Sprites[Cursor].x=Cur[CurPos].x;
           Sprites[Cursor].y=Cur[CurPos].y;
           GsSortSprite(&Sprites[Cursor], &WorldOT[activeBuff], Priority[Cursor]);
        }

        for (i=0; i<3; i++) {
            for (j=0; j<4; j++) {
                for (k=ListNum[i*4+j]-1; k>-1; k--) {
                    Sprites[List[i*4+j][k]].y = 42+i*54;
                    Sprites[List[i*4+j][k]].x = 77+j*130+k*12;
                    GsSortSprite(&Sprites[List[i*4+j][k]], &WorldOT[activeBuff],
                                                           Priority[cHearts2]);
                }
            }
        }

        VSync(0);
        GsSwapDispBuff();
        //GsSortClear(0, 0, 0, &WorldOT[activeBuff]);

        GsDrawOt(&WorldOT[activeBuff]);
    }
}

Draw_Hearts()
{
    int activeBuff;

    for (ai=0;ai<4;ai++) {
        activeBuff=GsGetActiveBuff();
        GsSetWorkBase((PACKET*)GpuPacketArea[activeBuff]);
        GsClearOt(0, 0, &WorldOT[activeBuff]);

        if (MyMsg>-1) {
           if (MyMsg==wScore) {
              i=Score;
              k=1;
              while (i>0) {
                    j=(i%10)+140;
                    Sprites[j].x = 350-k*24;      Sprites[j].y = 109;
                    GsSortSprite(&Sprites[j], &WorldOT[activeBuff], 1);
                    i/=10;
                    k++;
              }
           }
           if (MyMsg==wTheScore) {
              i=Scores[0];
              k=1;
              while (i>0) {
                    j=(i%10)+140;
                    Sprites[j].x = 410-k*24;      Sprites[j].y = 56+96;
                    GsSortSprite(&Sprites[j], &WorldOT[activeBuff], 1);
                    i/=10;
                    k++;
              }
              if (Scores[0]==0) {
                 Sprites[140].x = 410-24;      Sprites[140].y = 56+96;
                 GsSortSprite(&Sprites[140], &WorldOT[activeBuff], 1);
              }
              i=Scores[1];
              k=1;
              while (i>0) {
                    j=(i%10)+140;
                    Sprites[j].x = 410-k*24;      Sprites[j].y = 56+36;
                    GsSortSprite(&Sprites[j], &WorldOT[activeBuff], 1);
                    i/=10;
                    k++;
              }
              if (Scores[1]==0) {
                 Sprites[140].x = 410-24;      Sprites[140].y = 56+36;
                 GsSortSprite(&Sprites[140], &WorldOT[activeBuff], 1);
              }
              i=Scores[2];
              k=1;
              while (i>0) {
                    j=(i%10)+140;
                    Sprites[j].x = 410-k*24;      Sprites[j].y = 56+56;
                    GsSortSprite(&Sprites[j], &WorldOT[activeBuff], 1);
                    i/=10;
                    k++;
              }
              if (Scores[2]==0) {
                 Sprites[140].x = 410-24;      Sprites[140].y = 56+56;
                 GsSortSprite(&Sprites[140], &WorldOT[activeBuff], 1);
              }
              i=Scores[3];
              k=1;
              while (i>0) {
                    j=(i%10)+140;
                    Sprites[j].x = 410-k*24;      Sprites[j].y = 56+76;
                    GsSortSprite(&Sprites[j], &WorldOT[activeBuff], 1);
                    i/=10;
                    k++;
              }
              if (Scores[3]==0) {
                 Sprites[140].x = 410-24;      Sprites[140].y = 56+76;
                 GsSortSprite(&Sprites[140], &WorldOT[activeBuff], 1);
              }
           }
           Sprites[TheMsg].x=(640-Sprites[TheMsg].w)/2;
           Sprites[TheMsg].y=(240-Sprites[TheMsg].h)/2;
           GsSortSprite(&Sprites[TheMsg], &WorldOT[activeBuff], 2);
        }

        GsSortSprite(&Sprites[bgLUp], &WorldOT[activeBuff], Priority[bgLUp]);
        GsSortSprite(&Sprites[bgMUp], &WorldOT[activeBuff], Priority[bgLUp]);
        GsSortSprite(&Sprites[bgRUp], &WorldOT[activeBuff], Priority[bgLUp]);

        if (Animate>0) {
           for (i=TempNum-1;i>-1;i--) {
               Sprites[TempList[i]].x=Start_x;
               Sprites[TempList[i]].y=Start_y+i*10;
               GsSortSprite(&Sprites[TempList[i]], &WorldOT[activeBuff],
                                                        Priority[cHearts2]);
           }
        }

        for (i=0; i<8; i++) {
            j=Name[i];
            Sprites[j].x = 320-96+i*24;      Sprites[j].y = 208;
            GsSortSprite(&Sprites[j], &WorldOT[activeBuff], Priority[101]);
        }

        /*
        */

        if ((Animate==0) && (En==0) && (Player<1) && (MyMsg<0)) {
           Sprites[Cursor].x=Cur[CurPos].x;
           Sprites[Cursor].y=Cur[CurPos].y;
           GsSortSprite(&Sprites[Cursor], &WorldOT[activeBuff], Priority[Cursor]);
        }

        if (Cheater==1) {
           for (i=12; i>-1; i--) {
               if (List[1][i]>-1) {
                  Sprites[List[1][i]].x = 40;
                  Sprites[List[1][i]].y = 20+i*8;
                  GsSortSprite(&Sprites[List[1][i]], &WorldOT[activeBuff],
                                                          Priority[cHearts2]);
               }
           }
           for (i=12; i>-1; i--) {
               if (List[2][i]>-1) {
                  Sprites[List[2][i]].y = 20;
                  Sprites[List[2][i]].x = 170+i*20;
                  GsSortSprite(&Sprites[List[2][i]], &WorldOT[activeBuff],
                                                          Priority[cHearts2]);
               }
           }
           for (i=12; i>-1; i--) {
               if (List[3][i]>-1) {
                  Sprites[List[3][i]].x = 540;
                  Sprites[List[3][i]].y = 20+i*8;
                  GsSortSprite(&Sprites[List[3][i]], &WorldOT[activeBuff],
                                                          Priority[cHearts2]);
               }
           }
        } else {
           for (i=12; i>-1; i--) {
               if (List[1][i]>-1) {
                  Sprites[cBackV].x = 36;
                  Sprites[cBackV].y = 16+i*8;
                  GsSortSprite(&Sprites[cBackV], &WorldOT[activeBuff],
                                                          Priority[cHearts2]);
               }
           }
           for (i=12; i>-1; i--) {
               if (List[2][i]>-1) {
                  Sprites[cBackV].y = 16;
                  Sprites[cBackV].x = 166+i*20;
                  GsSortSprite(&Sprites[cBackV], &WorldOT[activeBuff],
                                                          Priority[cHearts2]);
               }
           }
           for (i=12; i>-1; i--) {
               if (List[3][i]>-1) {
                  Sprites[cBackV].x = 536;
                  Sprites[cBackV].y = 16+i*8;
                  GsSortSprite(&Sprites[cBackV], &WorldOT[activeBuff],
                                                          Priority[cHearts2]);
               }
           }
        }


        for (i=12; i>-1; i--) {
            if (List[0][i]>-1) {
               Sprites[List[0][i]].y = 162;
               Sprites[List[0][i]].x = 270-13*13+i*26;
               GsSortSprite(&Sprites[List[0][i]], &WorldOT[activeBuff],
                                                       Priority[cHearts2]);
            }
        }

        if (Ready[0][0]>-1) {
           Sprites[Ready[0][0]].y = 101;
           Sprites[Ready[0][0]].x = 301;
           GsSortSprite(&Sprites[Ready[0][0]], &WorldOT[activeBuff],
                                                   Priority[cHearts2]);
        }
        if (Ready[0][3]>-1) {
           Sprites[Ready[0][3]].y = 90;
           Sprites[Ready[0][3]].x = 333;
           GsSortSprite(&Sprites[Ready[0][3]], &WorldOT[activeBuff],
                                                   Priority[cHearts2]);
        }
        if (Ready[0][2]>-1) {
           Sprites[Ready[0][2]].y = 81;
           Sprites[Ready[0][2]].x = 284;
           GsSortSprite(&Sprites[Ready[0][2]], &WorldOT[activeBuff],
                                                   Priority[cHearts2]);
        }
        if (Ready[0][1]>-1) {
           Sprites[Ready[0][1]].y = 91;
           Sprites[Ready[0][1]].x = 253;
           GsSortSprite(&Sprites[Ready[0][1]], &WorldOT[activeBuff],
                                                   Priority[cHearts2]);
        }

        VSync(0);
        GsSwapDispBuff();
        //GsSortClear(0, 0, 0, &WorldOT[activeBuff]);

        GsDrawOt(&WorldOT[activeBuff]);
    }
}

Draw_King()
{
    int activeBuff;

    for (ai=0;ai<4;ai++) {
        activeBuff=GsGetActiveBuff();
        GsSetWorkBase((PACKET*)GpuPacketArea[activeBuff]);
        GsClearOt(0, 0, &WorldOT[activeBuff]);

        if (GameNumber<15) {
           i=GameNumber+kKing;
           j=kDontTake;
           if (i>kKing7) {
              i=i-7;
              j=kTake;
           }
           Sprites[j].x = 35;      Sprites[j].y = 155;
           GsSortSprite(&Sprites[j], &WorldOT[activeBuff], Priority[101]);
           Sprites[i].x = 35;      Sprites[i].y = 185;
           GsSortSprite(&Sprites[i], &WorldOT[activeBuff], Priority[101]);
        }
        if (MyMsg>-1) {
           if (MyMsg==wScore) {
              i=Score;
              k=1;
              while (i>0) {
                    j=(i%10)+140;
                    Sprites[j].x = 350-k*24;      Sprites[j].y = 109;
                    GsSortSprite(&Sprites[j], &WorldOT[activeBuff], 1);
                    i/=10;
                    k++;
              }
           }
           if (MyMsg==wTheScore) {
              i=abs(Scores[0]);
              k=1;
              while (i>0) {
                    j=(i%10)+140;
                    Sprites[j].x = 410-k*24;      Sprites[j].y = 56+96;
                    GsSortSprite(&Sprites[j], &WorldOT[activeBuff], 1);
                    i/=10;
                    k++;
              }
              if (Scores[0]==0) {
                 Sprites[140].x = 410-24;      Sprites[140].y = 56+96;
                 GsSortSprite(&Sprites[140], &WorldOT[activeBuff], 1);
              }
              if ((Scores[0]<0) || (GameNumber<8) && (Scores[0]!=0)) {
                 Sprites[139].x = 410-k*24;      Sprites[139].y = 56+96;
                 GsSortSprite(&Sprites[139], &WorldOT[activeBuff], 1);
              }
              i=abs(Scores[1]);
              k=1;
              while (i>0) {
                    j=(i%10)+140;
                    Sprites[j].x = 410-k*24;      Sprites[j].y = 56+36;
                    GsSortSprite(&Sprites[j], &WorldOT[activeBuff], 1);
                    i/=10;
                    k++;
              }
              if (Scores[1]==0) {
                 Sprites[140].x = 410-24;      Sprites[140].y = 56+36;
                 GsSortSprite(&Sprites[140], &WorldOT[activeBuff], 1);
              }
              if ((Scores[1]<0) || (GameNumber<8) && (Scores[1]!=0)) {
                 Sprites[139].x = 410-k*24;      Sprites[139].y = 56+36;
                 GsSortSprite(&Sprites[139], &WorldOT[activeBuff], 1);
              }
              i=abs(Scores[2]);
              k=1;
              while (i>0) {
                    j=(i%10)+140;
                    Sprites[j].x = 410-k*24;      Sprites[j].y = 56+56;
                    GsSortSprite(&Sprites[j], &WorldOT[activeBuff], 1);
                    i/=10;
                    k++;
              }
              if (Scores[2]==0) {
                 Sprites[140].x = 410-24;      Sprites[140].y = 56+56;
                 GsSortSprite(&Sprites[140], &WorldOT[activeBuff], 1);
              }
              if ((Scores[2]<0) || (GameNumber<8) && (Scores[2]!=0)) {
                 Sprites[139].x = 410-k*24;      Sprites[139].y = 56+56;
                 GsSortSprite(&Sprites[139], &WorldOT[activeBuff], 1);
              }
              i=abs(Scores[3]);
              k=1;
              while (i>0) {
                    j=(i%10)+140;
                    Sprites[j].x = 410-k*24;      Sprites[j].y = 56+76;
                    GsSortSprite(&Sprites[j], &WorldOT[activeBuff], 1);
                    i/=10;
                    k++;
              }
              if (Scores[3]==0) {
                 Sprites[140].x = 410-24;      Sprites[140].y = 56+76;
                 GsSortSprite(&Sprites[140], &WorldOT[activeBuff], 1);
              }
              if ((Scores[3]<0) || (GameNumber<8) && (Scores[3]!=0)) {
                 Sprites[139].x = 410-k*24;      Sprites[139].y = 56+76;
                 GsSortSprite(&Sprites[139], &WorldOT[activeBuff], 1);
              }
           }
           Sprites[TheMsg].x=(640-Sprites[TheMsg].w)/2;
           Sprites[TheMsg].y=(240-Sprites[TheMsg].h)/2;
           GsSortSprite(&Sprites[TheMsg], &WorldOT[activeBuff], 2);
        }

        GsSortSprite(&Sprites[bgLUp], &WorldOT[activeBuff], Priority[bgLUp]);
        GsSortSprite(&Sprites[bgMUp], &WorldOT[activeBuff], Priority[bgLUp]);
        GsSortSprite(&Sprites[bgRUp], &WorldOT[activeBuff], Priority[bgLUp]);

        if (Animate>0) {
           for (i=TempNum-1;i>-1;i--) {
               Sprites[TempList[i]].x=Start_x;
               Sprites[TempList[i]].y=Start_y+i*10;
               GsSortSprite(&Sprites[TempList[i]], &WorldOT[activeBuff],
                                                        Priority[cHearts2]);
           }
        }

        for (i=0; i<8; i++) {
            j=Name[i];
            Sprites[j].x = 320-96+i*24;      Sprites[j].y = 208;
            GsSortSprite(&Sprites[j], &WorldOT[activeBuff], Priority[101]);
        }

        /*
        */

        if ((Animate==0) && (En==0) && (Player<1) && (MyMsg<0)) {
           Sprites[Cursor].x=Cur[CurPos].x;
           Sprites[Cursor].y=Cur[CurPos].y;
           GsSortSprite(&Sprites[Cursor], &WorldOT[activeBuff], Priority[Cursor]);
        }

        if (Cheater==1) {
           for (i=12; i>-1; i--) {
               if (List[1][i]>-1) {
                  Sprites[List[1][i]].x = 40;
                  Sprites[List[1][i]].y = 20+i*8;
                  GsSortSprite(&Sprites[List[1][i]], &WorldOT[activeBuff],
                                                          Priority[cHearts2]);
               }
           }
           for (i=12; i>-1; i--) {
               if (List[2][i]>-1) {
                  Sprites[List[2][i]].y = 20;
                  Sprites[List[2][i]].x = 170+i*20;
                  GsSortSprite(&Sprites[List[2][i]], &WorldOT[activeBuff],
                                                          Priority[cHearts2]);
               }
           }
           for (i=12; i>-1; i--) {
               if (List[3][i]>-1) {
                  Sprites[List[3][i]].x = 540;
                  Sprites[List[3][i]].y = 20+i*8;
                  GsSortSprite(&Sprites[List[3][i]], &WorldOT[activeBuff],
                                                          Priority[cHearts2]);
               }
           }
        } else {
           for (i=12; i>-1; i--) {
               if (List[1][i]>-1) {
                  Sprites[cBackV].x = 36;
                  Sprites[cBackV].y = 16+i*8;
                  GsSortSprite(&Sprites[cBackV], &WorldOT[activeBuff],
                                                          Priority[cHearts2]);
               }
           }
           for (i=12; i>-1; i--) {
               if (List[2][i]>-1) {
                  Sprites[cBackV].y = 16;
                  Sprites[cBackV].x = 296-8*8+i*15;
                  GsSortSprite(&Sprites[cBackV], &WorldOT[activeBuff],
                                                          Priority[cHearts2]);
               }
           }
           for (i=12; i>-1; i--) {
               if (List[3][i]>-1) {
                  Sprites[cBackV].x = 536;
                  Sprites[cBackV].y = 16+i*8;
                  GsSortSprite(&Sprites[cBackV], &WorldOT[activeBuff],
                                                          Priority[cHearts2]);
               }
           }
        }

        for (i=12; i>-1; i--) {
            if (List[0][i]>-1) {
               Sprites[List[0][i]].y = 162;
               Sprites[List[0][i]].x = 300-15*8+i*30;
               GsSortSprite(&Sprites[List[0][i]], &WorldOT[activeBuff],
                                                       Priority[cHearts2]);
            }
        }

        if (Ready[0][0]>-1) {
           Sprites[Ready[0][0]].y = 101;
           Sprites[Ready[0][0]].x = 301;
           GsSortSprite(&Sprites[Ready[0][0]], &WorldOT[activeBuff],
                                                   Priority[cHearts2]);
        }
        if (Ready[0][3]>-1) {
           Sprites[Ready[0][3]].y = 90;
           Sprites[Ready[0][3]].x = 333;
           GsSortSprite(&Sprites[Ready[0][3]], &WorldOT[activeBuff],
                                                   Priority[cHearts2]);
        }
        if (Ready[0][2]>-1) {
           Sprites[Ready[0][2]].y = 81;
           Sprites[Ready[0][2]].x = 284;
           GsSortSprite(&Sprites[Ready[0][2]], &WorldOT[activeBuff],
                                                   Priority[cHearts2]);
        }
        if (Ready[0][1]>-1) {
           Sprites[Ready[0][1]].y = 91;
           Sprites[Ready[0][1]].x = 253;
           GsSortSprite(&Sprites[Ready[0][1]], &WorldOT[activeBuff],
                                                   Priority[cHearts2]);
        }

        VSync(0);
        GsSwapDispBuff();
        //GsSortClear(0, 0, 0, &WorldOT[activeBuff]);

        GsDrawOt(&WorldOT[activeBuff]);
    }
}

Draw_Nine()
{
    int activeBuff;

    for (ai=0;ai<4;ai++) {
        activeBuff=GsGetActiveBuff();
        GsSetWorkBase((PACKET*)GpuPacketArea[activeBuff]);
        GsClearOt(0, 0, &WorldOT[activeBuff]);

        if (MyMsg>-1) {
           if (MyMsg==wScore) {
              i=Score;
              k=1;
              while (i>0) {
                    j=(i%10)+140;
                    Sprites[j].x = 350-k*24;      Sprites[j].y = 109;
                    GsSortSprite(&Sprites[j], &WorldOT[activeBuff], 1);
                    i/=10;
                    k++;
              }
           }
           Sprites[TheMsg].x=(640-Sprites[TheMsg].w)/2;
           Sprites[TheMsg].y=(240-Sprites[TheMsg].h)/2;
           GsSortSprite(&Sprites[TheMsg], &WorldOT[activeBuff], 2);
        }

        GsSortSprite(&Sprites[bgLUp], &WorldOT[activeBuff], Priority[bgLUp]);
        GsSortSprite(&Sprites[bgMUp], &WorldOT[activeBuff], Priority[bgLUp]);
        GsSortSprite(&Sprites[bgRUp], &WorldOT[activeBuff], Priority[bgLUp]);

        if (Animate>0) {
           for (i=TempNum-1;i>-1;i--) {
               Sprites[TempList[i]+200].x=Start_x;
               Sprites[TempList[i]+200].y=Start_y+i*10;
               GsSortSprite(&Sprites[TempList[i]+200], &WorldOT[activeBuff],
                                                        Priority[cHearts2]);
           }
        }

        for (i=0; i<8; i++) {
            j=Name[i];
            Sprites[j].x = 320-96+i*24;      Sprites[j].y = 208;
            GsSortSprite(&Sprites[j], &WorldOT[activeBuff], Priority[101]);
        }

        i=Score;
        k=1;
        while (i>0) {
              j=(i%10)+140;
              Sprites[j].x = 600-k*24;      Sprites[j].y = 208;
              GsSortSprite(&Sprites[j], &WorldOT[activeBuff], Priority[101]);
              i/=10;
              k++;
        }
        if (Score==0) {
           Sprites[140].x = 600-24;      Sprites[140].y = 208;
           GsSortSprite(&Sprites[140], &WorldOT[activeBuff], Priority[101]);
        }

        if ((Animate==0) && (Player==0)) {
           Sprites[Cursor].x=Cur[CurPos].x;
           Sprites[Cursor].y=Cur[CurPos].y;
           GsSortSprite(&Sprites[Cursor], &WorldOT[activeBuff], Priority[Cursor]);
        }

        for (j=0;j<4;j++) {
            for (i=0;i<9;i++) {
                if (Ready[j][i]>-1) {
                   Sprites[Ready[j][i]+200].x = 115+46*i;
                   Sprites[Ready[j][i]+200].y = 25+32*j;
                   GsSortSprite(&Sprites[Ready[j][i]+200],
                                  &WorldOT[activeBuff], Priority[cHearts2]);
                }
            }
        }


        if (Cheater==1) {
           for (i=11; i>-1; i--) {
               if (List[1][i]>-1) {
                  Sprites[List[1][i]].x = 40;
                  Sprites[List[1][i]].y = 20+i*8;
                  GsSortSprite(&Sprites[List[1][i]], &WorldOT[activeBuff],
                                                          Priority[cHearts2]);
               }
           }
           for (i=11; i>-1; i--) {
               if (List[2][i]>-1) {
                  Sprites[List[2][i]].x = 540;
                  Sprites[List[2][i]].y = 20+i*8;
                  GsSortSprite(&Sprites[List[2][i]], &WorldOT[activeBuff],
                                                          Priority[cHearts2]);
               }
           }
        } else {
           for (i=11; i>-1; i--) {
               if (List[1][i]>-1) {
                  Sprites[cBackV].x = 36;
                  Sprites[cBackV].y = 16+i*8;
                  GsSortSprite(&Sprites[cBackV], &WorldOT[activeBuff],
                                                          Priority[cHearts2]);
               }
           }
           for (i=11; i>-1; i--) {
               if (List[2][i]>-1) {
                  Sprites[cBackV].x = 536;
                  Sprites[cBackV].y = 16+i*8;
                  GsSortSprite(&Sprites[cBackV], &WorldOT[activeBuff],
                                                          Priority[cHearts2]);
               }
           }
        }


        for (i=11; i>-1; i--) {
            if (List[0][i]>-1) {
               Sprites[List[0][i]].y = 162;
               Sprites[List[0][i]].x = 300-12*15+i*30;
               GsSortSprite(&Sprites[List[0][i]], &WorldOT[activeBuff],
                                                       Priority[cHearts2]);
            }
        }


        VSync(0);
        GsSwapDispBuff();
        //GsSortClear(0, 0, 0, &WorldOT[activeBuff]);

        GsDrawOt(&WorldOT[activeBuff]);
    }
}

Draw_Fool()
{
    int activeBuff;

    for (ai=0;ai<4;ai++) {
        activeBuff=GsGetActiveBuff();
        GsSetWorkBase((PACKET*)GpuPacketArea[activeBuff]);
        GsClearOt(0, 0, &WorldOT[activeBuff]);

        if (MyMsg>-1) {
           Sprites[TheMsg].x=(640-Sprites[TheMsg].w)/2;
           Sprites[TheMsg].y=(240-Sprites[TheMsg].h)/2;
           GsSortSprite(&Sprites[TheMsg], &WorldOT[activeBuff], 2);
        }

        GsSortSprite(&Sprites[bgLUp], &WorldOT[activeBuff], Priority[bgLUp]);
        GsSortSprite(&Sprites[bgMUp], &WorldOT[activeBuff], Priority[bgLUp]);
        GsSortSprite(&Sprites[bgRUp], &WorldOT[activeBuff], Priority[bgLUp]);

        if (Animate>0) {
           for (i=TempNum-1;i>-1;i--) {
               Sprites[TempList[i]].x=Start_x+i*10;
               Sprites[TempList[i]].y=Start_y;
               GsSortSprite(&Sprites[TempList[i]], &WorldOT[activeBuff],
                                                        Priority[cHearts2]);
           }
        }

        for (i=0; i<8; i++) {
            j=Name[i];
            Sprites[j].x = 320-96+i*24;      Sprites[j].y = 208;
            GsSortSprite(&Sprites[j], &WorldOT[activeBuff], Priority[101]);
        }

        if ((Animate==0) && (Player==0)) {
           Sprites[Cursor].x=Cur[CurPos].x;
           Sprites[Cursor].y=Cur[CurPos].y;
           GsSortSprite(&Sprites[Cursor], &WorldOT[activeBuff], Priority[Cursor]);
        }

        if (CurDeck<28) {
           Sprites[cPackV].x = 536;      Sprites[cPackV].y = 86;
           GsSortSprite(&Sprites[cPackV], &WorldOT[activeBuff],
                                                          Priority[cHearts]-1);
        } else {
           if (CurDeck<33) {
              Sprites[cPackV1].x = 536;      Sprites[cPackV1].y = 86;
              GsSortSprite(&Sprites[cPackV1], &WorldOT[activeBuff],
                                                             Priority[cHearts]-1);
           } else {
              if (CurDeck<36) {
                 Sprites[cBackV].x = 536;      Sprites[cBackV].y = 86;
                 GsSortSprite(&Sprites[cBackV], &WorldOT[activeBuff],
                                                                Priority[cHearts]-1);
              }
           }
        }

        Sprites[TrumpSuit+280].x = 41;    Sprites[TrumpSuit+280].y = 99;
        GsSortSprite(&Sprites[TrumpSuit+280], &WorldOT[activeBuff],
                                                          Priority[cHearts]);

        k=30;
        if (ListNum[1]>17) k=20;
        if (ListNum[1]>26) k=14;
        if (Cheater==1) {
           for (i=ListNum[1]-1; i>-1; i--) {
               if (List[1][i]>-1) {
                  Sprites[List[1][i]].y = 20;
                  Sprites[List[1][i]].x = 300-k*ListNum[1]/2+i*k;
                  GsSortSprite(&Sprites[List[1][i]], &WorldOT[activeBuff],
                                                          Priority[cHearts2]);
               }
           }
        } else {
           for (i=ListNum[1]-1; i>-1; i--) {
               if (List[1][i]>-1) {
                  Sprites[cBackV].y = 16;
                  Sprites[cBackV].x = 296-k*ListNum[1]/2+i*k;
                  GsSortSprite(&Sprites[cBackV], &WorldOT[activeBuff],
                                                          Priority[cHearts2]);
               }
           }
        }

        k=30;
        if (ListNum[0]>17) k=20;
        if (ListNum[0]>26) k=14;
        for (i=ListNum[0]-1; i>-1; i--) {
            if (List[0][i]>-1) {
               Sprites[List[0][i]].y = 162;
               Sprites[List[0][i]].x = 300-k*ListNum[0]/2+i*k;
               GsSortSprite(&Sprites[List[0][i]], &WorldOT[activeBuff],
                                                       Priority[cHearts2]);
            }
        }


        for (i=0;i<6;i++) {
            if (Ready[1][i]>-1) {
               Sprites[Ready[1][i]].y = 75;
               Sprites[Ready[1][i]].x = 121+i*64;
               GsSortSprite(&Sprites[Ready[1][i]], &WorldOT[activeBuff],
                                             Priority[cHearts2]+StartPlayer);
            }
        }
        for (i=0;i<6;i++) {
            if (Ready[0][i]>-1) {
               Sprites[Ready[0][i]].y = 105;
               Sprites[Ready[0][i]].x = 124+i*64;
               GsSortSprite(&Sprites[Ready[0][i]], &WorldOT[activeBuff],
                                                         Priority[cHearts2]);
            }
        }

        VSync(0);
        GsSwapDispBuff();
        //GsSortClear(0, 0, 0, &WorldOT[activeBuff]);

        GsDrawOt(&WorldOT[activeBuff]);
    }
}

Draw_BJ()
{
    int activeBuff;

    for (ai=0;ai<4;ai++) {
        activeBuff=GsGetActiveBuff();
        GsSetWorkBase((PACKET*)GpuPacketArea[activeBuff]);
        GsClearOt(0, 0, &WorldOT[activeBuff]);

        if (MyMsg>-1) {
           if ((MyMsg!=wWin)&&(MyMsg!=wLose)&&(MyMsg!=wNoOne)) {
              Sprites[TheMsg].x=(640-Sprites[TheMsg].w)/2;
              Sprites[TheMsg].y=(240-Sprites[TheMsg].h)/2;
           } else {
              Sprites[TheMsg].x=600-Sprites[TheMsg].w;
              Sprites[TheMsg].y=(240-Sprites[TheMsg].h)/2-5;
           }
           GsSortSprite(&Sprites[TheMsg], &WorldOT[activeBuff], 2);
        }

        GsSortSprite(&Sprites[bgLUp], &WorldOT[activeBuff], Priority[bgLUp]);
        GsSortSprite(&Sprites[bgMUp], &WorldOT[activeBuff], Priority[bgLUp]);
        GsSortSprite(&Sprites[bgRUp], &WorldOT[activeBuff], Priority[bgLUp]);

        if (Animate>0) {
           for (i=TempNum-1;i>-1;i--) {
               Sprites[TempList[i]].x=Start_x+i*10;
               Sprites[TempList[i]].y=Start_y;
               GsSortSprite(&Sprites[TempList[i]], &WorldOT[activeBuff],
                                                        Priority[cHearts2]);
           }
        }

        if ((Animate==0) && (Player==0)) {
           Sprites[pGame].x=172;
           Sprites[pGame].y=172;
           GsSortSprite(&Sprites[pGame], &WorldOT[activeBuff], Priority[pGame]);
        }

        for (i=0; i<8; i++) {
            j=Name[i];
            Sprites[j].x = 320-96+i*24;      Sprites[j].y = 208;
            GsSortSprite(&Sprites[j], &WorldOT[activeBuff], Priority[101]);
        }

        i=Scores[0];
        k=1;
        while (i>0) {
              j=(i%10)+140;
              Sprites[j].x = 190-k*24;      Sprites[j].y = 146;
              GsSortSprite(&Sprites[j], &WorldOT[activeBuff], Priority[101]);
              i/=10;
              k++;
        }
        if (Scores[0]==0) {
           Sprites[140].x = 190-24;      Sprites[140].y = 146;
           GsSortSprite(&Sprites[140], &WorldOT[activeBuff], Priority[101]);
        }

        Sprites[cPackV].x = 465;      Sprites[cPackV].y = 92;
        GsSortSprite(&Sprites[cPackV], &WorldOT[activeBuff],
                                                          Priority[cHearts]-1);

        if ((Cheater==1) || (Player==2)) {
           i=Scores[1];
           k=1;
           while (i>0) {
                 j=(i%10)+140;
                 Sprites[j].x = 190-k*24;      Sprites[j].y = 66;
                 GsSortSprite(&Sprites[j], &WorldOT[activeBuff], Priority[101]);
                 i/=10;
                 k++;
           }
           if (Scores[1]==0) {
              Sprites[140].x = 190-24;      Sprites[140].y = 66;
              GsSortSprite(&Sprites[140], &WorldOT[activeBuff], Priority[101]);
           }

           for (i=ListNum[1]-1; i>-1; i--) {
               if (List[1][i]>-1) {
                  Sprites[List[1][i]].y = 52;
                  Sprites[List[1][i]].x = 218+i*30;
                  GsSortSprite(&Sprites[List[1][i]], &WorldOT[activeBuff],
                                                          Priority[cHearts2]);
               }
           }
        } else {
           for (i=ListNum[1]-1; i>0; i--) {
               if (List[1][i]>-1) {
                  Sprites[List[1][i]].y = 52;
                  Sprites[List[1][i]].x = 218+i*30;
                  GsSortSprite(&Sprites[List[1][i]], &WorldOT[activeBuff],
                                                          Priority[cHearts2]);
               }
           }
           Sprites[cBackV].y = 48;
           Sprites[cBackV].x = 214;
           GsSortSprite(&Sprites[cBackV], &WorldOT[activeBuff],
                                                          Priority[cHearts2]);
        }

        for (i=ListNum[0]-1; i>-1; i--) {
            if (List[0][i]>-1) {
               Sprites[List[0][i]].y = 139;
               Sprites[List[0][i]].x = 218+i*30;
               GsSortSprite(&Sprites[List[0][i]], &WorldOT[activeBuff],
                                                       Priority[cHearts2]);
            }
        }

        VSync(0);
        GsSwapDispBuff();
        //GsSortClear(0, 0, 0, &WorldOT[activeBuff]);

        GsDrawOt(&WorldOT[activeBuff]);
    }
}


// ====================== I N I T   G R A P H I C S ==================== //


InitGameStuff(addr1)
u_long *addr1;
{
        addr1++;

        GsGetTimInfo(addr1, &TexInfo);
        /* image */
        rect1.x=TexInfo.px;
        rect1.y=TexInfo.py;
        rect1.w=TexInfo.pw;
        rect1.h=TexInfo.ph;
        LoadImage(&rect1, TexInfo.pixel);
}


InitKing(Kaddr)
u_long *Kaddr;
{
        Kaddr++;

        GsGetTimInfo(Kaddr, &TexInfo);
        /* image */
        rect1.x=TexInfo.px;
        rect1.y=TexInfo.py;
        rect1.w=TexInfo.pw;
        rect1.h=TexInfo.ph;
        LoadImage(&rect1, TexInfo.pixel);
        /* palette */
        rect1.x = TexInfo.cx;
        rect1.y = TexInfo.cy;
        rect1.w = TexInfo.cw;
        rect1.h = TexInfo.ch;
        LoadImage(&rect1, TexInfo.clut);

        /* sprite init */
        Sprites[kTake].attribute = ((TexInfo.pmode&0x03)<<24);
        Sprites[kTake].w = 128;
        Sprites[kTake].h = 15;
        Sprites[kTake].x = 0;
        Sprites[kTake].y = 0;
        Sprites[kTake].mx = 320;
        Sprites[kTake].my = 120;
        Sprites[kTake].tpage = GetTPage(1, 0, TexInfo.px, TexInfo.py);
        Sprites[kTake].u = 0;
        Sprites[kTake].v = 128+13;
        Sprites[kTake].cx = TexInfo.cx;
        Sprites[kTake].cy = TexInfo.cy;
        Sprites[kTake].r = AllBright;
        Sprites[kTake].g = AllBright;
        Sprites[kTake].b = AllBright;
        Sprites[kTake].rotate = ONE*0;
        Sprites[kTake].scalex = ONE;
        Sprites[kTake].scaley = ONE;

        Sprites[kDontTake]=Sprites[kTake];
        Sprites[kDontTake].h = 27;
        Sprites[kDontTake].v = 128;

        Sprites[kKing1]=Sprites[kTake];
        Sprites[kKing1].h = 13;
        Sprites[kKing1].v = 128+27;

        Sprites[kKing2]=Sprites[kTake];
        Sprites[kKing2].h = 14;
        Sprites[kKing2].v = 128+39;

        Sprites[kKing3]=Sprites[kTake];
        Sprites[kKing3].h = 14;
        Sprites[kKing3].v = 128+52;

        Sprites[kKing4]=Sprites[kTake];
        Sprites[kKing4].h = 15;
        Sprites[kKing4].v = 128+65;

        Sprites[kKing5]=Sprites[kTake];
        Sprites[kKing5].h = 23;
        Sprites[kKing5].v = 128+105;

        Sprites[kKing6]=Sprites[kTake];
        Sprites[kKing6].h = 14;
        Sprites[kKing6].v = 128+80;

        Sprites[kKing7]=Sprites[kTake];
        Sprites[kKing7].h = 12;
        Sprites[kKing7].v = 128+94;
}


InitCards(Baddr, BCaddr, Raddr, RCaddr)
u_long *Baddr;
u_long *BCaddr;
u_long *Raddr;
u_long *RCaddr;
{
        Baddr++;
        BCaddr++;
        Raddr++;
        RCaddr++;

        // ================ BLACK ================

        GsGetTimInfo(Baddr, &TexInfo);
        /* image */
        rect1.x=TexInfo.px;
        rect1.y=TexInfo.py;
        rect1.w=TexInfo.pw;
        rect1.h=TexInfo.ph;
        LoadImage(&rect1, TexInfo.pixel);
        /* palette */
        rect1.x = TexInfo.cx;
        rect1.y = TexInfo.cy;
        rect1.w = TexInfo.cw;
        rect1.h = TexInfo.ch;
        LoadImage(&rect1, TexInfo.clut);

        /* sprite init */
        Sprites[cCross2].attribute = ((TexInfo.pmode&0x03)<<24);
        Sprites[cCross2].w = 60;
        Sprites[cCross2].h = 40;
        Sprites[cCross2].x = 0;
        Sprites[cCross2].y = 0;
        Sprites[cCross2].mx = 320;
        Sprites[cCross2].my = 120;
        Sprites[cCross2].tpage = GetTPage(0, 0, TexInfo.px, TexInfo.py);
        Sprites[cCross2].u = 0;
        Sprites[cCross2].v = 120;
        Sprites[cCross2].cx = TexInfo.cx;
        Sprites[cCross2].cy = TexInfo.cy;
        Sprites[cCross2].r = NormalBright;
        Sprites[cCross2].g = NormalBright;
        Sprites[cCross2].b = NormalBright;
        Sprites[cCross2].rotate = ONE*0;
        Sprites[cCross2].scalex = ONE;
        Sprites[cCross2].scaley = ONE;

        Sprites[cCross3]=Sprites[cCross2];
        Sprites[cCross3].u = 60;

        Sprites[cCross4]=Sprites[cCross2];
        Sprites[cCross4].u = 120;

        Sprites[cCross5]=Sprites[cCross2];
        Sprites[cCross5].u = 180;

        Sprites[cCross6]=Sprites[cCross2];
        Sprites[cCross6].tpage = GetTPage(0, 0, TexInfo.px+64, TexInfo.py);

        Sprites[cCross7]=Sprites[cCross6];
        Sprites[cCross7].u = 60;

        Sprites[cCross8]=Sprites[cCross6];
        Sprites[cCross8].u = 120;


        Sprites[cSpades2]=Sprites[cCross2];
        Sprites[cSpades2].v = 120+40;

        Sprites[cSpades3]=Sprites[cSpades2];
        Sprites[cSpades3].u = 60;

        Sprites[cSpades4]=Sprites[cSpades2];
        Sprites[cSpades4].u = 120;

        Sprites[cSpades5]=Sprites[cSpades2];
        Sprites[cSpades5].u = 180;

        Sprites[cSpades6]=Sprites[cSpades2];
        Sprites[cSpades6].tpage = GetTPage(0, 0, TexInfo.px+64, TexInfo.py);

        Sprites[cSpades7]=Sprites[cSpades6];
        Sprites[cSpades7].u = 60;

        Sprites[cSpades8]=Sprites[cSpades6];
        Sprites[cSpades8].u = 120;


        Sprites[cCross9]=Sprites[cCross2];
        Sprites[cCross9].v = 120+80;

        Sprites[cCross10]=Sprites[cCross9];
        Sprites[cCross10].u = 60;

        Sprites[mCross]=Sprites[cCross9];
        Sprites[mCross].u = 120;

        Sprites[cSpades9]=Sprites[cCross9];
        Sprites[cSpades9].u = 180;

        Sprites[cSpades10]=Sprites[cCross9];
        Sprites[cSpades10].tpage = GetTPage(0, 0, TexInfo.px+64, TexInfo.py);

        Sprites[mSpades]=Sprites[cSpades10];
        Sprites[mSpades].u = 60;


        // ================ RED ================

        GsGetTimInfo(Raddr, &TexInfo);
        /* image */
        rect1.x=TexInfo.px;
        rect1.y=TexInfo.py;
        rect1.w=TexInfo.pw;
        rect1.h=TexInfo.ph;
        LoadImage(&rect1, TexInfo.pixel);
        /* palette */
        rect1.x = TexInfo.cx;
        rect1.y = TexInfo.cy;
        rect1.w = TexInfo.cw;
        rect1.h = TexInfo.ch;
        LoadImage(&rect1, TexInfo.clut);

        /* sprite init */
        Sprites[cHearts2].attribute = ((TexInfo.pmode&0x03)<<24);
        Sprites[cHearts2].w = 60;
        Sprites[cHearts2].h = 40;
        Sprites[cHearts2].x = 0;
        Sprites[cHearts2].y = 0;
        Sprites[cHearts2].mx = 320;
        Sprites[cHearts2].my = 120;
        Sprites[cHearts2].tpage = GetTPage(0, 0, TexInfo.px, TexInfo.py);
        Sprites[cHearts2].u = 0;
        Sprites[cHearts2].v = 0;
        Sprites[cHearts2].cx = TexInfo.cx;
        Sprites[cHearts2].cy = TexInfo.cy;
        Sprites[cHearts2].r = NormalBright;
        Sprites[cHearts2].g = NormalBright;
        Sprites[cHearts2].b = NormalBright;
        Sprites[cHearts2].rotate = ONE*0;
        Sprites[cHearts2].scalex = ONE;
        Sprites[cHearts2].scaley = ONE;

        Sprites[cHearts3]=Sprites[cHearts2];
        Sprites[cHearts3].u = 60;

        Sprites[cHearts4]=Sprites[cHearts2];
        Sprites[cHearts4].u = 120;

        Sprites[cHearts5]=Sprites[cHearts2];
        Sprites[cHearts5].u = 180;

        Sprites[cHearts6]=Sprites[cHearts2];
        Sprites[cHearts6].tpage = GetTPage(0, 0, TexInfo.px+64, TexInfo.py);

        Sprites[cHearts7]=Sprites[cHearts6];
        Sprites[cHearts7].u = 60;

        Sprites[cHearts8]=Sprites[cHearts6];
        Sprites[cHearts8].u = 120;


        Sprites[cDiamonds2]=Sprites[cHearts2];
        Sprites[cDiamonds2].v = 40;

        Sprites[cDiamonds3]=Sprites[cDiamonds2];
        Sprites[cDiamonds3].u = 60;

        Sprites[cDiamonds4]=Sprites[cDiamonds2];
        Sprites[cDiamonds4].u = 120;

        Sprites[cDiamonds5]=Sprites[cDiamonds2];
        Sprites[cDiamonds5].u = 180;

        Sprites[cDiamonds6]=Sprites[cDiamonds2];
        Sprites[cDiamonds6].tpage = GetTPage(0, 0, TexInfo.px+64, TexInfo.py);

        Sprites[cDiamonds7]=Sprites[cDiamonds6];
        Sprites[cDiamonds7].u = 60;

        Sprites[cDiamonds8]=Sprites[cDiamonds6];
        Sprites[cDiamonds8].u = 120;


        Sprites[cHearts9]=Sprites[cHearts2];
        Sprites[cHearts9].v = 80;

        Sprites[cHearts10]=Sprites[cHearts9];
        Sprites[cHearts10].u = 60;

        Sprites[mHearts]=Sprites[cHearts9];
        Sprites[mHearts].u = 120;

        Sprites[cDiamonds9]=Sprites[cHearts9];
        Sprites[cDiamonds9].u = 180;

        Sprites[cDiamonds10]=Sprites[cHearts9];
        Sprites[cDiamonds10].tpage = GetTPage(0, 0, TexInfo.px+64, TexInfo.py);

        Sprites[mDiamonds]=Sprites[cDiamonds10];
        Sprites[mDiamonds].u = 60;


        // ================ BLACK COLOR ================

        GsGetTimInfo(BCaddr, &TexInfo);
        /* image */
        rect1.x=TexInfo.px;
        rect1.y=TexInfo.py;
        rect1.w=TexInfo.pw;
        rect1.h=TexInfo.ph;
        LoadImage(&rect1, TexInfo.pixel);
        /* palette */
        rect1.x = TexInfo.cx;
        rect1.y = TexInfo.cy;
        rect1.w = TexInfo.cw;
        rect1.h = TexInfo.ch;
        LoadImage(&rect1, TexInfo.clut);

        /* sprite init */
        Sprites[cCrossJ].attribute = ((TexInfo.pmode&0x03)<<24);
        Sprites[cCrossJ].w = 60;
        Sprites[cCrossJ].h = 40;
        Sprites[cCrossJ].x = 0;
        Sprites[cCrossJ].y = 0;
        Sprites[cCrossJ].mx = 320;
        Sprites[cCrossJ].my = 120;
        Sprites[cCrossJ].tpage = GetTPage(1, 0, TexInfo.px, TexInfo.py);
        Sprites[cCrossJ].u = 0;
        Sprites[cCrossJ].v = 0;
        Sprites[cCrossJ].cx = TexInfo.cx;
        Sprites[cCrossJ].cy = TexInfo.cy;
        Sprites[cCrossJ].r = NormalBright;
        Sprites[cCrossJ].g = NormalBright;
        Sprites[cCrossJ].b = NormalBright;
        Sprites[cCrossJ].rotate = ONE*0;
        Sprites[cCrossJ].scalex = ONE;
        Sprites[cCrossJ].scaley = ONE;

        Sprites[cCrossQ]=Sprites[cCrossJ];
        Sprites[cCrossQ].u = 60;

        Sprites[cSpadesJ]=Sprites[cCrossJ];
        Sprites[cSpadesJ].v = 40;

        Sprites[cSpadesQ]=Sprites[cSpadesJ];
        Sprites[cSpadesQ].u = 60;

        Sprites[cCrossK]=Sprites[cCrossJ];
        Sprites[cCrossK].v = 80;

        Sprites[cSpadesK]=Sprites[cCrossK];
        Sprites[cSpadesK].u = 60;

        Sprites[cCrossA]=Sprites[cCrossK];
        Sprites[cCrossA].v = 120;

        Sprites[cSpadesA]=Sprites[cCrossA];
        Sprites[cSpadesA].u = 60;


        // ================ RED COLOR ================

        GsGetTimInfo(RCaddr, &TexInfo);
        /* image */
        rect1.x=TexInfo.px;
        rect1.y=TexInfo.py;
        rect1.w=TexInfo.pw;
        rect1.h=TexInfo.ph;
        LoadImage(&rect1, TexInfo.pixel);
        /* palette */
        rect1.x = TexInfo.cx;
        rect1.y = TexInfo.cy;
        rect1.w = TexInfo.cw;
        rect1.h = TexInfo.ch;
        LoadImage(&rect1, TexInfo.clut);

        /* sprite init */
        Sprites[cHeartsJ].attribute = ((TexInfo.pmode&0x03)<<24);
        Sprites[cHeartsJ].w = 60;
        Sprites[cHeartsJ].h = 40;
        Sprites[cHeartsJ].x = 0;
        Sprites[cHeartsJ].y = 0;
        Sprites[cHeartsJ].mx = 320;
        Sprites[cHeartsJ].my = 120;
        Sprites[cHeartsJ].tpage = GetTPage(1, 0, TexInfo.px, TexInfo.py);
        Sprites[cHeartsJ].u = 0;
        Sprites[cHeartsJ].v = 0;
        Sprites[cHeartsJ].cx = TexInfo.cx;
        Sprites[cHeartsJ].cy = TexInfo.cy;
        Sprites[cHeartsJ].r = NormalBright;
        Sprites[cHeartsJ].g = NormalBright;
        Sprites[cHeartsJ].b = NormalBright;
        Sprites[cHeartsJ].rotate = ONE*0;
        Sprites[cHeartsJ].scalex = ONE;
        Sprites[cHeartsJ].scaley = ONE;

        Sprites[cHeartsQ]=Sprites[cHeartsJ];
        Sprites[cHeartsQ].u = 60;

        Sprites[cDiamondsJ]=Sprites[cHeartsJ];
        Sprites[cDiamondsJ].v = 40;

        Sprites[cDiamondsQ]=Sprites[cDiamondsJ];
        Sprites[cDiamondsQ].u = 60;

        Sprites[cHeartsK]=Sprites[cHeartsJ];
        Sprites[cHeartsK].v = 80;

        Sprites[cDiamondsK]=Sprites[cHeartsK];
        Sprites[cDiamondsK].u = 60;

        Sprites[cHeartsA]=Sprites[cHeartsK];
        Sprites[cHeartsA].v = 120;

        Sprites[cDiamondsA]=Sprites[cHeartsA];
        Sprites[cDiamondsA].u = 60;
}

Init9Cards(Baddr, BCaddr, BCSaddr, Raddr, RCaddr, RCSaddr)
u_long *Baddr;
u_long *BCaddr;
u_long *BCSaddr;
u_long *Raddr;
u_long *RCaddr;
u_long *RCSaddr;
{
        Baddr++;
        BCaddr++;
        BCSaddr++;
        Raddr++;
        RCaddr++;
        RCSaddr++;

        // ================ BLACK ================

        GsGetTimInfo(Baddr, &TexInfo);
        /* image */
        rect1.x=TexInfo.px;
        rect1.y=TexInfo.py;
        rect1.w=TexInfo.pw;
        rect1.h=TexInfo.ph;
        LoadImage(&rect1, TexInfo.pixel);
        /* palette */
        rect1.x = TexInfo.cx;
        rect1.y = TexInfo.cy;
        rect1.w = TexInfo.cw;
        rect1.h = TexInfo.ch;
        LoadImage(&rect1, TexInfo.clut);

        /* sprite init */
        Sprites[cCross6].attribute = ((TexInfo.pmode&0x03)<<24);
        Sprites[cCross6].w = 60;
        Sprites[cCross6].h = 40;
        Sprites[cCross6].x = 0;
        Sprites[cCross6].y = 0;
        Sprites[cCross6].mx = 320;
        Sprites[cCross6].my = 120;
        Sprites[cCross6].tpage = GetTPage(0, 0, TexInfo.px, TexInfo.py);
        Sprites[cCross6].u = 0;
        Sprites[cCross6].v = 120;
        Sprites[cCross6].cx = TexInfo.cx;
        Sprites[cCross6].cy = TexInfo.cy;
        Sprites[cCross6].r = NormalBright;
        Sprites[cCross6].g = NormalBright;
        Sprites[cCross6].b = NormalBright;
        Sprites[cCross6].rotate = ONE*0;
        Sprites[cCross6].scalex = ONE;
        Sprites[cCross6].scaley = ONE;

        Sprites[cCross7]=Sprites[cCross6];
        Sprites[cCross7].u = 60;

        Sprites[cCross8]=Sprites[cCross6];
        Sprites[cCross8].u = 120;

        Sprites[cCross9]=Sprites[cCross6];
        Sprites[cCross9].u = 180;


        Sprites[cSpades6]=Sprites[cCross6];
        Sprites[cSpades6].v = 120+40;

        Sprites[cSpades7]=Sprites[cSpades6];
        Sprites[cSpades7].u = 60;

        Sprites[cSpades8]=Sprites[cSpades6];
        Sprites[cSpades8].u = 120;

        Sprites[cSpades9]=Sprites[cSpades6];
        Sprites[cSpades9].u = 180;

        Sprites[cCross10]=Sprites[cCross6];
        Sprites[cCross10].v = 120+80;

        Sprites[cSpades10]=Sprites[cCross10];
        Sprites[cSpades10].u = 120;


        // small cards

        Sprites[cCross6+200]=Sprites[cCross6];
        Sprites[cCross6+200].w = 44;
        Sprites[cCross6+200].h = 30;
        Sprites[cCross6+200].tpage = GetTPage(0, 0, TexInfo.px+64, TexInfo.py);

        Sprites[cCross7+200]=Sprites[cCross6+200];
        Sprites[cCross7+200].u = 44;

        Sprites[cCross8+200]=Sprites[cCross6+200];
        Sprites[cCross8+200].u = 88;

        Sprites[cCross9+200]=Sprites[cCross6+200];
        Sprites[cCross9+200].v = 120+30;

        Sprites[cCross10+200]=Sprites[cCross9+200];
        Sprites[cCross10+200].u = 44;

        Sprites[cSpades6+200]=Sprites[cCross6+200];
        Sprites[cSpades6+200].v = 120+60;

        Sprites[cSpades7+200]=Sprites[cSpades6+200];
        Sprites[cSpades7+200].u = 44;

        Sprites[cSpades8+200]=Sprites[cSpades6+200];
        Sprites[cSpades8+200].u = 88;

        Sprites[cSpades9+200]=Sprites[cCross6+200];
        Sprites[cSpades9+200].v = 120+90;

        Sprites[cSpades10+200]=Sprites[cSpades9+200];
        Sprites[cSpades10+200].u = 44;


        // ================ RED ================

        GsGetTimInfo(Raddr, &TexInfo);
        /* image */
        rect1.x=TexInfo.px;
        rect1.y=TexInfo.py;
        rect1.w=TexInfo.pw;
        rect1.h=TexInfo.ph;
        LoadImage(&rect1, TexInfo.pixel);
        /* palette */
        rect1.x = TexInfo.cx;
        rect1.y = TexInfo.cy;
        rect1.w = TexInfo.cw;
        rect1.h = TexInfo.ch;
        LoadImage(&rect1, TexInfo.clut);

        /* sprite init */
        Sprites[cHearts6].attribute = ((TexInfo.pmode&0x03)<<24);
        Sprites[cHearts6].w = 60;
        Sprites[cHearts6].h = 40;
        Sprites[cHearts6].x = 0;
        Sprites[cHearts6].y = 0;
        Sprites[cHearts6].mx = 320;
        Sprites[cHearts6].my = 120;
        Sprites[cHearts6].tpage = GetTPage(0, 0, TexInfo.px, TexInfo.py);
        Sprites[cHearts6].u = 0;
        Sprites[cHearts6].v = 0;
        Sprites[cHearts6].cx = TexInfo.cx;
        Sprites[cHearts6].cy = TexInfo.cy;
        Sprites[cHearts6].r = NormalBright;
        Sprites[cHearts6].g = NormalBright;
        Sprites[cHearts6].b = NormalBright;
        Sprites[cHearts6].rotate = ONE*0;
        Sprites[cHearts6].scalex = ONE;
        Sprites[cHearts6].scaley = ONE;

        Sprites[cHearts7]=Sprites[cHearts6];
        Sprites[cHearts7].u = 60;

        Sprites[cHearts8]=Sprites[cHearts6];
        Sprites[cHearts8].u = 120;

        Sprites[cHearts9]=Sprites[cHearts6];
        Sprites[cHearts9].u = 180;


        Sprites[cDiamonds6]=Sprites[cHearts6];
        Sprites[cDiamonds6].v = 40;

        Sprites[cDiamonds7]=Sprites[cDiamonds6];
        Sprites[cDiamonds7].u = 60;

        Sprites[cDiamonds8]=Sprites[cDiamonds6];
        Sprites[cDiamonds8].u = 120;

        Sprites[cDiamonds9]=Sprites[cDiamonds6];
        Sprites[cDiamonds9].u = 180;

        Sprites[cHearts10]=Sprites[cHearts6];
        Sprites[cHearts10].v = 80;

        Sprites[cDiamonds10]=Sprites[cHearts10];
        Sprites[cDiamonds10].u = 120;


        // small cards

        Sprites[cHearts6+200]=Sprites[cHearts6];
        Sprites[cHearts6+200].w = 44;
        Sprites[cHearts6+200].h = 30;
        Sprites[cHearts6+200].tpage = GetTPage(0, 0, TexInfo.px+64, TexInfo.py);

        Sprites[cHearts7+200]=Sprites[cHearts6+200];
        Sprites[cHearts7+200].u = 44;

        Sprites[cHearts8+200]=Sprites[cHearts6+200];
        Sprites[cHearts8+200].u = 88;

        Sprites[cHearts9+200]=Sprites[cHearts6+200];
        Sprites[cHearts9+200].v = 30;

        Sprites[cHearts10+200]=Sprites[cHearts9+200];
        Sprites[cHearts10+200].u = 44;

        Sprites[cDiamonds6+200]=Sprites[cHearts6+200];
        Sprites[cDiamonds6+200].v = 60;

        Sprites[cDiamonds7+200]=Sprites[cDiamonds6+200];
        Sprites[cDiamonds7+200].u = 44;

        Sprites[cDiamonds8+200]=Sprites[cDiamonds6+200];
        Sprites[cDiamonds8+200].u = 88;

        Sprites[cDiamonds9+200]=Sprites[cHearts6+200];
        Sprites[cDiamonds9+200].v = 90;

        Sprites[cDiamonds10+200]=Sprites[cDiamonds9+200];
        Sprites[cDiamonds10+200].u = 44;


        // ================ BLACK COLOR ================

        GsGetTimInfo(BCaddr, &TexInfo);
        /* image */
        rect1.x=TexInfo.px;
        rect1.y=TexInfo.py;
        rect1.w=TexInfo.pw;
        rect1.h=TexInfo.ph;
        LoadImage(&rect1, TexInfo.pixel);
        /* palette */
        rect1.x = TexInfo.cx;
        rect1.y = TexInfo.cy;
        rect1.w = TexInfo.cw;
        rect1.h = TexInfo.ch;
        LoadImage(&rect1, TexInfo.clut);

        /* sprite init */
        Sprites[cCrossJ].attribute = ((TexInfo.pmode&0x03)<<24);
        Sprites[cCrossJ].w = 60;
        Sprites[cCrossJ].h = 40;
        Sprites[cCrossJ].x = 0;
        Sprites[cCrossJ].y = 0;
        Sprites[cCrossJ].mx = 320;
        Sprites[cCrossJ].my = 120;
        Sprites[cCrossJ].tpage = GetTPage(1, 0, TexInfo.px, TexInfo.py);
        Sprites[cCrossJ].u = 0;
        Sprites[cCrossJ].v = 0;
        Sprites[cCrossJ].cx = TexInfo.cx;
        Sprites[cCrossJ].cy = TexInfo.cy;
        Sprites[cCrossJ].r = NormalBright;
        Sprites[cCrossJ].g = NormalBright;
        Sprites[cCrossJ].b = NormalBright;
        Sprites[cCrossJ].rotate = ONE*0;
        Sprites[cCrossJ].scalex = ONE;
        Sprites[cCrossJ].scaley = ONE;

        Sprites[cCrossQ]=Sprites[cCrossJ];
        Sprites[cCrossQ].u = 60;

        Sprites[cSpadesJ]=Sprites[cCrossJ];
        Sprites[cSpadesJ].v = 40;

        Sprites[cSpadesQ]=Sprites[cSpadesJ];
        Sprites[cSpadesQ].u = 60;

        Sprites[cCrossK]=Sprites[cCrossJ];
        Sprites[cCrossK].v = 80;

        Sprites[cSpadesK]=Sprites[cCrossK];
        Sprites[cSpadesK].u = 60;

        Sprites[cCrossA]=Sprites[cCrossK];
        Sprites[cCrossA].v = 120;

        Sprites[cSpadesA]=Sprites[cCrossA];
        Sprites[cSpadesA].u = 60;


        // ================ RED COLOR ================

        GsGetTimInfo(RCaddr, &TexInfo);
        /* image */
        rect1.x=TexInfo.px;
        rect1.y=TexInfo.py;
        rect1.w=TexInfo.pw;
        rect1.h=TexInfo.ph;
        LoadImage(&rect1, TexInfo.pixel);
        /* palette */
        rect1.x = TexInfo.cx;
        rect1.y = TexInfo.cy;
        rect1.w = TexInfo.cw;
        rect1.h = TexInfo.ch;
        LoadImage(&rect1, TexInfo.clut);

        /* sprite init */
        Sprites[cHeartsJ].attribute = ((TexInfo.pmode&0x03)<<24);
        Sprites[cHeartsJ].w = 60;
        Sprites[cHeartsJ].h = 40;
        Sprites[cHeartsJ].x = 0;
        Sprites[cHeartsJ].y = 0;
        Sprites[cHeartsJ].mx = 320;
        Sprites[cHeartsJ].my = 120;
        Sprites[cHeartsJ].tpage = GetTPage(1, 0, TexInfo.px, TexInfo.py);
        Sprites[cHeartsJ].u = 0;
        Sprites[cHeartsJ].v = 0;
        Sprites[cHeartsJ].cx = TexInfo.cx;
        Sprites[cHeartsJ].cy = TexInfo.cy;
        Sprites[cHeartsJ].r = NormalBright;
        Sprites[cHeartsJ].g = NormalBright;
        Sprites[cHeartsJ].b = NormalBright;
        Sprites[cHeartsJ].rotate = ONE*0;
        Sprites[cHeartsJ].scalex = ONE;
        Sprites[cHeartsJ].scaley = ONE;

        Sprites[cHeartsQ]=Sprites[cHeartsJ];
        Sprites[cHeartsQ].u = 60;

        Sprites[cDiamondsJ]=Sprites[cHeartsJ];
        Sprites[cDiamondsJ].v = 40;

        Sprites[cDiamondsQ]=Sprites[cDiamondsJ];
        Sprites[cDiamondsQ].u = 60;

        Sprites[cHeartsK]=Sprites[cHeartsJ];
        Sprites[cHeartsK].v = 80;

        Sprites[cDiamondsK]=Sprites[cHeartsK];
        Sprites[cDiamondsK].u = 60;

        Sprites[cHeartsA]=Sprites[cHeartsK];
        Sprites[cHeartsA].v = 120;

        Sprites[cDiamondsA]=Sprites[cHeartsA];
        Sprites[cDiamondsA].u = 60;


        // ================ small BLACK COLOR ================

        GsGetTimInfo(BCSaddr, &TexInfo);
        /* image */
        rect1.x=TexInfo.px;
        rect1.y=TexInfo.py;
        rect1.w=TexInfo.pw;
        rect1.h=TexInfo.ph;
        LoadImage(&rect1, TexInfo.pixel);
        /* palette */
        rect1.x = TexInfo.cx;
        rect1.y = TexInfo.cy;
        rect1.w = TexInfo.cw;
        rect1.h = TexInfo.ch;
        LoadImage(&rect1, TexInfo.clut);

        /* sprite init */
        Sprites[cCrossJ+200].attribute = ((TexInfo.pmode&0x03)<<24);
        Sprites[cCrossJ+200].w = 44;
        Sprites[cCrossJ+200].h = 60;
        Sprites[cCrossJ+200].x = 0;
        Sprites[cCrossJ+200].y = 0;
        Sprites[cCrossJ+200].mx = 320;
        Sprites[cCrossJ+200].my = 240;
        Sprites[cCrossJ+200].tpage = GetTPage(0, 0, TexInfo.px, TexInfo.py);
        Sprites[cCrossJ+200].u = 132;
        Sprites[cCrossJ+200].v = 0;
        Sprites[cCrossJ+200].cx = TexInfo.cx;
        Sprites[cCrossJ+200].cy = TexInfo.cy;
        Sprites[cCrossJ+200].r = NormalBright;
        Sprites[cCrossJ+200].g = NormalBright;
        Sprites[cCrossJ+200].b = NormalBright;
        Sprites[cCrossJ+200].rotate = ONE*0;
        Sprites[cCrossJ+200].scalex = ONE;
        Sprites[cCrossJ+200].scaley = ONE/2;

        Sprites[cCrossQ+200]=Sprites[cCrossJ+200];
        Sprites[cCrossQ+200].v = 60;

        Sprites[cCrossK+200]=Sprites[cCrossJ+200];
        Sprites[cCrossK+200].v = 120;

        Sprites[cCrossA+200]=Sprites[cCrossJ+200];
        Sprites[cCrossA+200].v = 180;

        Sprites[cSpadesJ+200]=Sprites[cCrossJ+200];
        Sprites[cSpadesJ+200].u = 132+44;

        Sprites[cSpadesQ+200]=Sprites[cSpadesJ+200];
        Sprites[cSpadesQ+200].v = 60;

        Sprites[cSpadesK+200]=Sprites[cSpadesJ+200];
        Sprites[cSpadesK+200].v = 120;

        Sprites[cSpadesA+200]=Sprites[cSpadesJ+200];
        Sprites[cSpadesA+200].v = 180;


        // ================ small RED COLOR ================

        GsGetTimInfo(RCSaddr, &TexInfo);
        /* image */
        rect1.x=TexInfo.px;
        rect1.y=TexInfo.py;
        rect1.w=TexInfo.pw;
        rect1.h=TexInfo.ph;
        LoadImage(&rect1, TexInfo.pixel);
        /* palette */
        rect1.x = TexInfo.cx;
        rect1.y = TexInfo.cy;
        rect1.w = TexInfo.cw;
        rect1.h = TexInfo.ch;
        LoadImage(&rect1, TexInfo.clut);

        /* sprite init */
        Sprites[cHeartsJ+200].attribute = ((TexInfo.pmode&0x03)<<24);
        Sprites[cHeartsJ+200].w = 44;
        Sprites[cHeartsJ+200].h = 60;
        Sprites[cHeartsJ+200].x = 0;
        Sprites[cHeartsJ+200].y = 0;
        Sprites[cHeartsJ+200].mx = 320;
        Sprites[cHeartsJ+200].my = 240;
        Sprites[cHeartsJ+200].tpage = GetTPage(0, 0, TexInfo.px, TexInfo.py);
        Sprites[cHeartsJ+200].u = 0;
        Sprites[cHeartsJ+200].v = 136;
        Sprites[cHeartsJ+200].cx = TexInfo.cx;
        Sprites[cHeartsJ+200].cy = TexInfo.cy;
        Sprites[cHeartsJ+200].r = NormalBright;
        Sprites[cHeartsJ+200].g = NormalBright;
        Sprites[cHeartsJ+200].b = NormalBright;
        Sprites[cHeartsJ+200].rotate = ONE*0;
        Sprites[cHeartsJ+200].scalex = ONE;
        Sprites[cHeartsJ+200].scaley = ONE/2;

        Sprites[cHeartsQ+200]=Sprites[cHeartsJ+200];
        Sprites[cHeartsQ+200].u = 44;

        Sprites[cHeartsK+200]=Sprites[cHeartsJ+200];
        Sprites[cHeartsK+200].u = 88;

        Sprites[cDiamondsA+200]=Sprites[cHeartsJ+200];
        Sprites[cDiamondsA+200].u = 132;

        Sprites[cDiamondsJ+200]=Sprites[cHeartsJ+200];
        Sprites[cDiamondsJ+200].v = 136+60;

        Sprites[cDiamondsQ+200]=Sprites[cDiamondsJ+200];
        Sprites[cDiamondsQ+200].u = 44;

        Sprites[cDiamondsK+200]=Sprites[cDiamondsJ+200];
        Sprites[cDiamondsK+200].u = 88;

        Sprites[cHeartsA+200]=Sprites[cDiamondsJ+200];
        Sprites[cHeartsA+200].u = 132;
}

InitPack(addr1)
u_long *addr1;
{
        addr1++;

        GsGetTimInfo(addr1, &TexInfo);
        /* image */
        rect1.x=TexInfo.px;
        rect1.y=TexInfo.py;
        rect1.w=TexInfo.pw;
        rect1.h=TexInfo.ph;
        LoadImage(&rect1,TexInfo.pixel);
        /* palette */
        rect1.x = TexInfo.cx;
        rect1.y = TexInfo.cy;
        rect1.w = TexInfo.cw;
        rect1.h = TexInfo.ch;
        LoadImage(&rect1, TexInfo.clut);

        /* sprite init */
        Sprites[cPackV].attribute = ((TexInfo.pmode&0x03)<<24);
        Sprites[cPackV].w = 64;
        Sprites[cPackV].h = 44;
        Sprites[cPackV].x = 60;
        Sprites[cPackV].y = 60;
        Sprites[cPackV].mx = 320;
        Sprites[cPackV].my = 120;
        Sprites[cPackV].tpage = GetTPage(1, 0, TexInfo.px, TexInfo.py);
        Sprites[cPackV].u = 4;
        Sprites[cPackV].v = 4;
        Sprites[cPackV].cx = TexInfo.cx;
        Sprites[cPackV].cy = TexInfo.cy;
        Sprites[cPackV].r = NormalBright;
        Sprites[cPackV].g = NormalBright;
        Sprites[cPackV].b = NormalBright;
        Sprites[cPackV].rotate = ONE*0;
        Sprites[cPackV].scalex = ONE;
        Sprites[cPackV].scaley = ONE;

        Sprites[cPackV1] = Sprites[cPackV];
        Sprites[cPackV1].u = 2;
        Sprites[cPackV1].v = 2;

        Sprites[cBackV] = Sprites[cPackV];
        Sprites[cBackV].u = 0;
        Sprites[cBackV].v = 0;

        Sprites[pDown] = Sprites[cPackV];
        Sprites[pDown].r = AllBright;
        Sprites[pDown].g = AllBright;
        Sprites[pDown].b = AllBright;
        Sprites[pDown].w = 42;
        Sprites[pDown].h = 24;
        Sprites[pDown].u = 0;
        Sprites[pDown].v = 74;

        Sprites[pSelect] = Sprites[pDown];
        Sprites[pSelect].w = 42;
        Sprites[pSelect].h = 24;
        Sprites[pSelect].u = 0;
        Sprites[pSelect].v = 50;

        Sprites[cTop1] = Sprites[cPackV];
        Sprites[cTop1].w = 59;
        Sprites[cTop1].h = 2;
        Sprites[cTop1].u = 68;
        Sprites[cTop1].v = 0;

        Sprites[cTop2] = Sprites[cTop1];
        Sprites[cTop2].h = 4;

        Sprites[cTop3] = Sprites[cTop1];
        Sprites[cTop3].h = 6;

        Sprites[cTop4] = Sprites[cTop1];
        Sprites[cTop4].h = 8;

        Sprites[cTop5] = Sprites[cTop1];
        Sprites[cTop5].h = 10;

        Sprites[cTop6] = Sprites[cTop1];
        Sprites[cTop6].h = 12;

        Sprites[pGame] = Sprites[cPackV];
        Sprites[pGame].u = 68;
        Sprites[pGame].v = 12;
        Sprites[pGame].w = 60;
        Sprites[pGame].h = 30;
}

InitAlphabet(Aaddr)
u_long *Aaddr;
{
        Aaddr++;

        GsGetTimInfo(Aaddr, &TexInfo);
        /* image */
        rect1.x=TexInfo.px;
        rect1.y=TexInfo.py;
        rect1.w=TexInfo.pw;
        rect1.h=TexInfo.ph;
        LoadImage(&rect1,TexInfo.pixel);
        /* palette */
        rect1.x = TexInfo.cx;
        rect1.y = TexInfo.cy;
        rect1.w = TexInfo.cw;
        rect1.h = TexInfo.ch;
        LoadImage(&rect1, TexInfo.clut);

        // russian letters: 101 -> 133
        // numbers:         141 -> 150
        // latin letters:   151 -> 176

        /* sprite init */
        Sprites[101].attribute = ((TexInfo.pmode&0x03)<<24);
        Sprites[101].w = 24;
        Sprites[101].h = 12;
        Sprites[101].x = 0;
        Sprites[101].y = 0;
        Sprites[101].mx = 320;
        Sprites[101].my = 120;
        Sprites[101].tpage = GetTPage(0, 0, TexInfo.px, TexInfo.py);
        Sprites[101].u = 0;
        Sprites[101].v = 128;
        Sprites[101].cx = TexInfo.cx;
        Sprites[101].cy = TexInfo.cy;
        Sprites[101].r = AllBright;
        Sprites[101].g = AllBright;
        Sprites[101].b = AllBright;
        Sprites[101].rotate = ONE*0;
        Sprites[101].scalex = ONE;
        Sprites[101].scaley = ONE;

        for (j=0; j<8; j++) {
            for (i=0; i<10; i++) {
                Sprites[j*10+i+101] = Sprites[101];
                Sprites[j*10+i+101].u = i*24;
                Sprites[j*10+i+101].v = j*12+128;
            }
        }

        Sprites[Alphabet] = Sprites[101];
        Sprites[Alphabet].w = 240;
        Sprites[Alphabet].h = 96;
}


InitHelp(addr1)
u_long *addr1;
{
        addr1++;

        /* load background into the videomemory */
        GsGetTimInfo(addr1, &TexInfo);
        /* image */
        rect1.x=TexInfo.px;
        rect1.y=TexInfo.py;
        rect1.w=TexInfo.pw;
        rect1.h=TexInfo.ph;
        LoadImage(&rect1,TexInfo.pixel); /* dma xfer from memory to videoram */
        /* palette */
        rect1.x = TexInfo.cx;             // X pos in frame buffer
        rect1.y = TexInfo.cy;             // Y pos in frame buffer
        rect1.w = TexInfo.cw;             // Width of CLUT
        rect1.h = TexInfo.ch;             // Height of CLUT
        LoadImage(&rect1, TexInfo.clut);  // Load the data into the frame buffer

        /* sprite init */

        /* left upper part */
        Sprites[bgHelp1].attribute = ((TexInfo.pmode&0x03)<<24);
        Sprites[bgHelp1].w = 256;
        Sprites[bgHelp1].h = 150;
        Sprites[bgHelp1].x = 65;
        Sprites[bgHelp1].y = 50;
        Sprites[bgHelp1].mx = 320;       /* center f. rotation/scaling */
        Sprites[bgHelp1].my = 120;
        Sprites[bgHelp1].tpage = GetTPage(0, 0, TexInfo.px, TexInfo.py);
        Sprites[bgHelp1].u = 0;        /* offset of the sprite INSIDE */
        Sprites[bgHelp1].v = 0;        /* the 255X255 pixel TIM picture */
        Sprites[bgHelp1].cx = TexInfo.cx;
        Sprites[bgHelp1].cy = TexInfo.cy;
        Sprites[bgHelp1].r = NormalBright+0x10;
        Sprites[bgHelp1].g = NormalBright+0x10;
        Sprites[bgHelp1].b = NormalBright+0x10;
        Sprites[bgHelp1].rotate = ONE*0;
        Sprites[bgHelp1].scalex = ONE;
        Sprites[bgHelp1].scaley = ONE;

        /* center upper part */
        Sprites[bgHelp2] = Sprites[bgHelp1];
        Sprites[bgHelp2].x = 65+256;
        Sprites[bgHelp2].tpage = GetTPage(0, 0, TexInfo.px+64, TexInfo.py);
}

InitBack(addr1)
u_long *addr1;
{
        addr1++;

        /* load background into the videomemory */
        GsGetTimInfo(addr1, &TexInfo);
        /* image */
        rect1.x=TexInfo.px;
        rect1.y=TexInfo.py;
        rect1.w=TexInfo.pw;
        rect1.h=TexInfo.ph;
        LoadImage(&rect1,TexInfo.pixel); /* dma xfer from memory to videoram */
        /* palette */
        rect1.x = TexInfo.cx;             // X pos in frame buffer
        rect1.y = TexInfo.cy;             // Y pos in frame buffer
        rect1.w = TexInfo.cw;             // Width of CLUT
        rect1.h = TexInfo.ch;             // Height of CLUT
        LoadImage(&rect1, TexInfo.clut);  // Load the data into the frame buffer

        /* sprite init */

        /* left upper part */
        Sprites[bgLUp].attribute = ((TexInfo.pmode&0x03)<<24);
        Sprites[bgLUp].w = 256;
        Sprites[bgLUp].h = 240;
        Sprites[bgLUp].x = 0;
        Sprites[bgLUp].y = 0;
        Sprites[bgLUp].mx = 320;       /* center f. rotation/scaling */
        Sprites[bgLUp].my = 120;
        Sprites[bgLUp].tpage = GetTPage(1, 0, TexInfo.px, TexInfo.py);
        Sprites[bgLUp].u = 0;        /* offset of the sprite INSIDE */
        Sprites[bgLUp].v = 0;        /* the 255X255 pixel TIM picture */
        Sprites[bgLUp].cx = TexInfo.cx;
        Sprites[bgLUp].cy = TexInfo.cy;
        Sprites[bgLUp].r = AllBright;
        Sprites[bgLUp].g = AllBright;
        Sprites[bgLUp].b = AllBright;
        Sprites[bgLUp].rotate = ONE*0;
        Sprites[bgLUp].scalex = ONE;
        Sprites[bgLUp].scaley = ONE;

        /* center upper part */
        Sprites[bgMUp] = Sprites[bgLUp];
        Sprites[bgMUp].x = 256;
        Sprites[bgMUp].tpage = GetTPage(1, 0, TexInfo.px+128, TexInfo.py);

        /* right upper part */
        Sprites[bgRUp] = Sprites[bgLUp];
        Sprites[bgRUp].x = 512;
        Sprites[bgRUp].w = 128;
        Sprites[bgRUp].tpage = GetTPage(1, 0, TexInfo.px+256, TexInfo.py);
}

InitMenuPointers(addr1, addr2, addr3)
u_long *addr1;
u_long *addr2;
u_long *addr3;
{
        addr1++;
        addr2++;
        addr3++;

        /* load into the videomemory */
        GsGetTimInfo(addr1, &TexInfo);
        /* image */
        rect1.x=TexInfo.px;
        rect1.y=TexInfo.py;
        rect1.w=TexInfo.pw;
        rect1.h=TexInfo.ph;
        LoadImage(&rect1,TexInfo.pixel); /* dma xfer from memory to videoram */
        /* palette */
        rect1.x = TexInfo.cx;             // X pos in frame buffer
        rect1.y = TexInfo.cy;             // Y pos in frame buffer
        rect1.w = TexInfo.cw;             // Width of CLUT
        rect1.h = TexInfo.ch;             // Height of CLUT
        LoadImage(&rect1, TexInfo.clut);  // Load the data into the frame buffer

        /* sprite init */
        Sprites[Ball].attribute = ((TexInfo.pmode&0x03)<<24);
        Sprites[Ball].w = 24;
        Sprites[Ball].h = 12;
        Sprites[Ball].x = 0;
        Sprites[Ball].y = 0;
        Sprites[Ball].mx = 320;       /* center f. rotation/scaling */
        Sprites[Ball].my = 120;
        Sprites[Ball].tpage = GetTPage(1, 0, TexInfo.px, TexInfo.py);
        Sprites[Ball].u = 0;        /* offset of the sprite INSIDE */
        Sprites[Ball].v = 0;        /* the 255X255 pixel TIM picture */
        Sprites[Ball].cx = TexInfo.cx;
        Sprites[Ball].cy = TexInfo.cy;
        Sprites[Ball].r = AllBright;
        Sprites[Ball].g = AllBright;
        Sprites[Ball].b = AllBright;
        Sprites[Ball].rotate = ONE*0;
        Sprites[Ball].scalex = ONE;
        Sprites[Ball].scaley = ONE;

        Sprites[pSelect]=Sprites[Ball];
        Sprites[pSelect].w = 24;
        Sprites[pSelect].h = 12;
        Sprites[pSelect].u = 26;

        Sprites[Polz]=Sprites[Ball];
        Sprites[Polz].w = 12;
        Sprites[Polz].h = 10;
        Sprites[Polz].u = 52;

        Sprites[oCard]=Sprites[Ball];
        Sprites[oCard].w = 59;
        Sprites[oCard].h = 40;
        Sprites[oCard].v = 12;

        Sprites[oPAL]=Sprites[Ball];
        Sprites[oPAL].w = 84;
        Sprites[oPAL].h = 16;
        Sprites[oPAL].v = 54;

        Sprites[oNTSC] = Sprites[oPAL];
        Sprites[oNTSC].h = 14;
        Sprites[oNTSC].v = 70;


        GsGetTimInfo(addr2, &TexInfo);
        /* image */
        rect1.x=TexInfo.px;
        rect1.y=TexInfo.py;
        rect1.w=TexInfo.pw;
        rect1.h=TexInfo.ph;
        LoadImage(&rect1,TexInfo.pixel); /* dma xfer from memory to videoram */
        /* palette */
        rect1.x = TexInfo.cx;             // X pos in frame buffer
        rect1.y = TexInfo.cy;             // Y pos in frame buffer
        rect1.w = TexInfo.cw;             // Width of CLUT
        rect1.h = TexInfo.ch;             // Height of CLUT
        LoadImage(&rect1, TexInfo.clut);  // Load the data into the frame buffer

        /* sprite init */
        Sprites[mPolz1].attribute = ((TexInfo.pmode&0x03)<<24);
        Sprites[mPolz1].w = 29;
        Sprites[mPolz1].h = 20;
        Sprites[mPolz1].x = 0;
        Sprites[mPolz1].y = 0;
        Sprites[mPolz1].mx = 320;       /* center f. rotation/scaling */
        Sprites[mPolz1].my = 120;
        Sprites[mPolz1].tpage = GetTPage(1, 0, TexInfo.px, TexInfo.py);
        Sprites[mPolz1].u = 0;        /* offset of the sprite INSIDE */
        Sprites[mPolz1].v = 236;      /* the 255X255 pixel TIM picture */
        Sprites[mPolz1].cx = TexInfo.cx;
        Sprites[mPolz1].cy = TexInfo.cy;
        Sprites[mPolz1].r = AllBright;
        Sprites[mPolz1].g = AllBright;
        Sprites[mPolz1].b = AllBright;
        Sprites[mPolz1].rotate = ONE*0;
        Sprites[mPolz1].scalex = ONE;
        Sprites[mPolz1].scaley = ONE;

        Sprites[mPolz2] = Sprites[mPolz1];
        Sprites[mPolz2].u = 30;

        Sprites[mPolz3] = Sprites[mPolz1];
        Sprites[mPolz3].u = 60;

        Sprites[mPolz4] = Sprites[mPolz1];
        Sprites[mPolz4].u = 90;

        Sprites[mPolz5] = Sprites[mPolz1];
        Sprites[mPolz5].u = 120;


        GsGetTimInfo(addr3, &TexInfo);
        /* image */
        rect1.x=TexInfo.px;
        rect1.y=TexInfo.py;
        rect1.w=TexInfo.pw;
        rect1.h=TexInfo.ph;
        LoadImage(&rect1,TexInfo.pixel); /* dma xfer from memory to videoram */
        /* palette */
        rect1.x = TexInfo.cx;             // X pos in frame buffer
        rect1.y = TexInfo.cy;             // Y pos in frame buffer
        rect1.w = TexInfo.cw;             // Width of CLUT
        rect1.h = TexInfo.ch;             // Height of CLUT
        LoadImage(&rect1, TexInfo.clut);  // Load the data into the frame buffer

        /* sprite init */
        Sprites[Auth1].attribute = ((TexInfo.pmode&0x03)<<24);
        Sprites[Auth1].w = 256;
        Sprites[Auth1].h = 140;
        Sprites[Auth1].x = 128;
        Sprites[Auth1].y = 0;
        Sprites[Auth1].mx = 320;       /* center f. rotation/scaling */
        Sprites[Auth1].my = 120;
        Sprites[Auth1].tpage = GetTPage(1, 0, TexInfo.px, TexInfo.py);
        Sprites[Auth1].u = 0;        /* offset of the sprite INSIDE */
        Sprites[Auth1].v = 0;        /* the 255X255 pixel TIM picture */
        Sprites[Auth1].cx = TexInfo.cx;
        Sprites[Auth1].cy = TexInfo.cy;
        Sprites[Auth1].r = AllBright;
        Sprites[Auth1].g = AllBright;
        Sprites[Auth1].b = AllBright;
        Sprites[Auth1].rotate = ONE*0;
        Sprites[Auth1].scalex = ONE;
        Sprites[Auth1].scaley = ONE;

        Sprites[Auth2] = Sprites[Auth1];
        Sprites[Auth2].tpage = GetTPage(1, 0, TexInfo.px+128, TexInfo.py);
        Sprites[Auth2].w = 128;
        Sprites[Auth2].x = 256+128;
}

InitLoad(addr)
u_long *addr;
{
        addr++;

        GsGetTimInfo(addr, &TexInfo);
        /* image */
        rect1.x=TexInfo.px;
        rect1.y=TexInfo.py;
        rect1.w=TexInfo.pw;
        rect1.h=TexInfo.ph;
        LoadImage(&rect1,TexInfo.pixel); /* dma xfer from memory to videoram */
        /* palette */
        rect1.x = TexInfo.cx;             // X pos in frame buffer
        rect1.y = TexInfo.cy;             // Y pos in frame buffer
        rect1.w = TexInfo.cw;             // Width of CLUT
        rect1.h = TexInfo.ch;             // Height of CLUT
        LoadImage(&rect1, TexInfo.clut);  // Load the data into the frame buffer

        /* sprite init */

        /* left upper part */
        Sprites[Loading].attribute = ((TexInfo.pmode&0x03)<<24);
        Sprites[Loading].w = TexInfo.pw*2;
        Sprites[Loading].h = TexInfo.ph;
        Sprites[Loading].x = 0;
        Sprites[Loading].y = 0;
        Sprites[Loading].mx = 320;       /* center f. rotation/scaling */
        Sprites[Loading].my = 120;
        Sprites[Loading].tpage = GetTPage(1, 0, TexInfo.px, TexInfo.py);
        Sprites[Loading].u = 0;        /* offset of the sprite INSIDE */
        Sprites[Loading].v = 224;        /* the 255X255 pixel TIM picture */
        Sprites[Loading].cx = TexInfo.cx;
        Sprites[Loading].cy = TexInfo.cy;
        Sprites[Loading].r = AllBright;
        Sprites[Loading].g = AllBright;
        Sprites[Loading].b = AllBright;
        Sprites[Loading].rotate = ONE*0;
        Sprites[Loading].scalex = ONE;
        Sprites[Loading].scaley = ONE;
}

InitMsg(addr)
u_long *addr;
{
        addr++;

        GsGetTimInfo(addr, &TexInfo);
        /* image */
        rect1.x=TexInfo.px;
        rect1.y=TexInfo.py;
        rect1.w=TexInfo.pw;
        rect1.h=TexInfo.ph;
        LoadImage(&rect1,TexInfo.pixel); /* dma xfer from memory to videoram */
        /* palette */
        rect1.x = TexInfo.cx;             // X pos in frame buffer
        rect1.y = TexInfo.cy;             // Y pos in frame buffer
        rect1.w = TexInfo.cw;             // Width of CLUT
        rect1.h = TexInfo.ch;             // Height of CLUT
        LoadImage(&rect1, TexInfo.clut);  // Load the data into the frame buffer

        /* sprite init */

        /* left upper part */
        Sprites[TheMsg].attribute = ((TexInfo.pmode&0x03)<<24);
        Sprites[TheMsg].w = TexInfo.pw*2;
        Sprites[TheMsg].h = TexInfo.ph;
        Sprites[TheMsg].x = 0;
        Sprites[TheMsg].y = 0;
        Sprites[TheMsg].mx = 320;       /* center f. rotation/scaling */
        Sprites[TheMsg].my = 120;
        Sprites[TheMsg].tpage = GetTPage(1, 0, TexInfo.px, TexInfo.py);
        Sprites[TheMsg].u = 0;        /* offset of the sprite INSIDE */
        Sprites[TheMsg].v = 0;        /* the 255X255 pixel TIM picture */
        Sprites[TheMsg].cx = TexInfo.cx;
        Sprites[TheMsg].cy = TexInfo.cy;
        Sprites[TheMsg].r = AllBright;
        Sprites[TheMsg].g = AllBright;
        Sprites[TheMsg].b = AllBright;
        Sprites[TheMsg].rotate = ONE*0;
        Sprites[TheMsg].scalex = ONE;
        Sprites[TheMsg].scaley = ONE;
}


// =============================== M E N U ============================== //


InitMenu()
{
        CheckBonus();
        cd_read_sub("\\DATA\\MENU\\MENU.TIM;1",    (void *)MENU_ADDR);
        if (Bonus==1) {
           cd_read_sub("\\DATA\\MENU\\MENU_GB.TIM;1", (void *)MENU_G_ADDR);
           cd_read_sub("\\DATA\\MENU\\MENU_SB.TIM;1", (void *)MENU_S_ADDR);
        } else {
           cd_read_sub("\\DATA\\MENU\\MENU_G.TIM;1",  (void *)MENU_G_ADDR);
           cd_read_sub("\\DATA\\MENU\\MENU_S.TIM;1",  (void *)MENU_S_ADDR);
        }
}

InitMainMenu()
{
        InitBack(MENU_ADDR);
        EndPos=3;

        Cur[0].x=227;
        Cur[0].y=76;
        Cur[1].x=167;
        Cur[1].y=106;
        Cur[2].x=149;
        Cur[2].y=136;
        Cur[3].x=183;
        Cur[3].y=166;
}

InitMenu_G()
{
        if (Bonus==1) EndPos=4;
        else          EndPos=3;
        InitBack(MENU_G_ADDR);

        Cur[0].x=176;
        Cur[0].y=67;
        Cur[1].x=228;
        Cur[1].y=92;
        Cur[2].x=212;
        Cur[2].y=117;
        Cur[3].x=212;
        Cur[3].y=142;
        Cur[4].x=224;
        Cur[4].y=167;
}

InitMenu_S()
{
        if (Bonus==1) EndPos=3;
        else          EndPos=1;
        InitBack(MENU_S_ADDR);

        Cur[0].x=181;
        Cur[0].y=77;
        Cur[1].x=175;
        Cur[1].y=107;
        Cur[2].x=205;
        Cur[2].y=137;
        Cur[3].x=259;
        Cur[3].y=167;
}

InitMenuStuff()
{
        cd_read_sub("\\DATA\\HELP\\MENU0.TIM;1",    (void *)HELP_1_ADDR);
        cd_read_sub("\\DATA\\HELP\\MENU1.TIM;1",    (void *)HELP_2_ADDR);
        cd_read_sub("\\DATA\\MENU\\ALPHABET.TIM;1", (void *)ALPHABET_ADDR);
        cd_read_sub("\\DATA\\MENU\\AUTHORS.TIM;1",  (void *)AUTHORS_ADDR);
        cd_read_sub("\\DATA\\MENU\\PACKMENU.TIM;1", (void *)PACKMENU_ADDR);
        cd_read_sub("\\DATA\\MENU\\MM_LOAD.TIM;1",  (void *)MM_LOAD_ADDR);
        cd_read_sub("\\DATA\\MENU\\MM_SAVE.TIM;1",  (void *)MM_SAVE_ADDR);
        cd_read_sub("\\DATA\\MENU\\KURSOR.TIM;1",   (void *)POLZ_ADDR);
        cd_read_sub("\\DATA\\MENU\\NAME.TIM;1",     (void *)NAME_ADDR);
        cd_read_sub("\\DATA\\MENU\\RECORDS.TIM;1",  (void *)RECORDS_ADDR);
        InitAlphabet(ALPHABET_ADDR);
        InitMenuPointers(PACKMENU_ADDR, POLZ_ADDR, AUTHORS_ADDR);
}


MenuSelect()
{
        if ((PadData & PADLup)>0) {
           if (CurPos>0) {
              CurPos--;
              PlayIt(sample[7]);
           }
        }
        if ((PadData & PADLdown)>0) {
           if (CurPos<EndPos) {
              CurPos++;
              PlayIt(sample[7]);
           }
        }

        ReadKey();
}

Fade_In()
{
    int activeBuff;
    int i, j;

    for (i=(AllBright/0x10)-1;i>0;i--) {
        Sprites[bgLUp].r = AllBright-i*0x10;
        Sprites[bgLUp].g = AllBright-i*0x10;
        Sprites[bgLUp].b = AllBright-i*0x10;

        Sprites[bgMUp].r = AllBright-i*0x10;
        Sprites[bgMUp].g = AllBright-i*0x10;
        Sprites[bgMUp].b = AllBright-i*0x10;

        Sprites[bgRUp].r = AllBright-i*0x10;
        Sprites[bgRUp].g = AllBright-i*0x10;
        Sprites[bgRUp].b = AllBright-i*0x10;

        for (j=0;j<2;j++) {
            activeBuff=GsGetActiveBuff();
            GsSetWorkBase((PACKET*)GpuPacketArea[activeBuff]);
            GsClearOt(0, 0, &WorldOT[activeBuff]);

            GsSortSprite(&Sprites[bgLUp], &WorldOT[activeBuff], 2);
            GsSortSprite(&Sprites[bgMUp], &WorldOT[activeBuff], 2);
            GsSortSprite(&Sprites[bgRUp], &WorldOT[activeBuff], 2);

            VSync(0);     /* waits for vertical retrace */
            GsSwapDispBuff();
            //GsSortClear(0, 0, 0, &WorldOT[activeBuff]);

            GsDrawOt(&WorldOT[activeBuff]);
        }
    }

    Sprites[bgLUp].r = AllBright;
    Sprites[bgLUp].g = AllBright;
    Sprites[bgLUp].b = AllBright;

    Sprites[bgMUp].r = AllBright;
    Sprites[bgMUp].g = AllBright;
    Sprites[bgMUp].b = AllBright;

    Sprites[bgRUp].r = AllBright;
    Sprites[bgRUp].g = AllBright;
    Sprites[bgRUp].b = AllBright;
}

Fade_Out()
{
    int activeBuff;
    int i, j;

    for (i=1;i<AllBright/0x10;i++) {
        Sprites[bgLUp].r = AllBright-i*0x10;
        Sprites[bgLUp].g = AllBright-i*0x10;
        Sprites[bgLUp].b = AllBright-i*0x10;

        Sprites[bgMUp].r = AllBright-i*0x10;
        Sprites[bgMUp].g = AllBright-i*0x10;
        Sprites[bgMUp].b = AllBright-i*0x10;

        Sprites[bgRUp].r = AllBright-i*0x10;
        Sprites[bgRUp].g = AllBright-i*0x10;
        Sprites[bgRUp].b = AllBright-i*0x10;

        for (j=0;j<2;j++) {
            activeBuff=GsGetActiveBuff();
            GsSetWorkBase((PACKET*)GpuPacketArea[activeBuff]);
            GsClearOt(0, 0, &WorldOT[activeBuff]);

            GsSortSprite(&Sprites[bgLUp], &WorldOT[activeBuff], 2);
            GsSortSprite(&Sprites[bgMUp], &WorldOT[activeBuff], 2);
            GsSortSprite(&Sprites[bgRUp], &WorldOT[activeBuff], 2);

            VSync(0);     /* waits for vertical retrace */
            GsSwapDispBuff();
            //GsSortClear(0, 0, 0, &WorldOT[activeBuff]);

            GsDrawOt(&WorldOT[activeBuff]);
        }
    }

    Sprites[bgLUp].r = AllBright;
    Sprites[bgLUp].g = AllBright;
    Sprites[bgLUp].b = AllBright;

    Sprites[bgMUp].r = AllBright;
    Sprites[bgMUp].g = AllBright;
    Sprites[bgMUp].b = AllBright;

    Sprites[bgRUp].r = AllBright;
    Sprites[bgRUp].g = AllBright;
    Sprites[bgRUp].b = AllBright;
}


Draw_Menu()
{
    int activeBuff;

    for (i=0;i<4;i++) {
        activeBuff=GsGetActiveBuff();
        GsSetWorkBase((PACKET*)GpuPacketArea[activeBuff]);
        GsClearOt(0, 0, &WorldOT[activeBuff]);

        Sprites[Cursor].x=Cur[CurPos].x;
        Sprites[Cursor].y=Cur[CurPos].y;
        GsSortSprite(&Sprites[Cursor], &WorldOT[activeBuff], 1);

        Sprites[Cursor].x=608-Cur[CurPos].x;
        Sprites[Cursor].y=Cur[CurPos].y;
        GsSortSprite(&Sprites[Cursor], &WorldOT[activeBuff], 1);

        GsSortSprite(&Sprites[bgLUp], &WorldOT[activeBuff], 2);
        GsSortSprite(&Sprites[bgMUp], &WorldOT[activeBuff], 2);
        GsSortSprite(&Sprites[bgRUp], &WorldOT[activeBuff], 2);

        VSync(0);     /* waits for vertical retrace */
        GsSwapDispBuff();
        //GsSortClear(0, 0, 0, &WorldOT[activeBuff]);

        GsDrawOt(&WorldOT[activeBuff]);
    }

    Cursor+=CurInc;
    if (Cursor>mPolz5) {
       Cursor=mPolz4;
       CurInc=-1;
    }
    if (Cursor<mPolz1) {
       Cursor=mPolz2;
       CurInc=1;
    }
}

Draw_Options()
{
    int activeBuff;

    for (i=0;i<4;i++) {
        activeBuff=GsGetActiveBuff();
        GsSetWorkBase((PACKET*)GpuPacketArea[activeBuff]);
        GsClearOt(0, 0, &WorldOT[activeBuff]);

        if (MyMsg>-1) {
           Sprites[MyMsg].x=(640-256)/2;
           Sprites[MyMsg].y=(240-28)/2;
           GsSortSprite(&Sprites[MyMsg], &WorldOT[activeBuff], 1);
        }

        Sprites[oCard].x=448;
        Sprites[oCard].y=94;
        Sprites[oCard].r = NormalBright;
        Sprites[oCard].g = NormalBright;
        Sprites[oCard].b = NormalBright;
        GsSortSprite(&Sprites[oCard], &WorldOT[activeBuff], 1);

        if (VMode==1) {
           Sprites[oPAL].x=438;
           Sprites[oPAL].y=69;
           GsSortSprite(&Sprites[oPAL], &WorldOT[activeBuff], 1);
        } else {
           Sprites[oNTSC].x=435;
           Sprites[oNTSC].y=69;
           GsSortSprite(&Sprites[oNTSC], &WorldOT[activeBuff], 1);
        }

        Sprites[Ball].x=Cur[CurPos].x;
        Sprites[Ball].y=Cur[CurPos].y;
        GsSortSprite(&Sprites[Ball], &WorldOT[activeBuff], 1);
        Sprites[Polz].x=mBright[pBright].x;
        Sprites[Polz].y=mBright[pBright].y;
        GsSortSprite(&Sprites[Polz], &WorldOT[activeBuff], 1);
        Sprites[Polz].x=mMusic[pMusic].x;
        Sprites[Polz].y=mMusic[pMusic].y;
        GsSortSprite(&Sprites[Polz], &WorldOT[activeBuff], 1);
        Sprites[Polz].x=mSFX[pSFX].x;
        Sprites[Polz].y=mSFX[pSFX].y;
        GsSortSprite(&Sprites[Polz], &WorldOT[activeBuff], 1);
        GsSortSprite(&Sprites[bgLUp], &WorldOT[activeBuff], 2);
        GsSortSprite(&Sprites[bgMUp], &WorldOT[activeBuff], 2);
        GsSortSprite(&Sprites[bgRUp], &WorldOT[activeBuff], 2);

        VSync(0);     /* waits for vertical retrace */
        GsSwapDispBuff();
        //GsSortClear(0, 0, 0, &WorldOT[activeBuff]);

        GsDrawOt(&WorldOT[activeBuff]);
    }
}

Draw_Records()
{
    int activeBuff;

    for (ai=0;ai<4;ai++) {

        activeBuff=GsGetActiveBuff();
        GsSetWorkBase((PACKET*)GpuPacketArea[activeBuff]);
        GsClearOt(0, 0, &WorldOT[activeBuff]);

        if (HEAD.NameNine[0]>0) {
           for (i=0;i<8;i++) {
               j=HEAD.NameNine[i];
               Sprites[j].x=110+i*24;      Sprites[j].y=78;
               GsSortSprite(&Sprites[j], &WorldOT[activeBuff], 2);
           }
           i=HEAD.ScoreNine;
           k=1;
           while (i>0) {
                 j=(i%10)+140;
                 Sprites[j].x = 530-k*24;      Sprites[j].y = 78;
                 GsSortSprite(&Sprites[j], &WorldOT[activeBuff], 1);
                 i/=10;
                 k++;
           }
        }
        if (HEAD.NameKing[0]>0) {
           for (i=0;i<8;i++) {
               j=HEAD.NameKing[i];
               Sprites[j].x=110+i*24;      Sprites[j].y=78+32;
               GsSortSprite(&Sprites[j], &WorldOT[activeBuff], 2);
           }
           i=HEAD.ScoreKing;
           k=1;
           while (i>0) {
                 j=(i%10)+140;
                 Sprites[j].x = 530-k*24;      Sprites[j].y = 78+32;
                 GsSortSprite(&Sprites[j], &WorldOT[activeBuff], 1);
                 i/=10;
                 k++;
           }
        }
        if (HEAD.NameHearts[0]>0) {
           for (i=0;i<8;i++) {
               j=HEAD.NameHearts[i];
               Sprites[j].x=110+i*24;      Sprites[j].y=78+32*2;
               GsSortSprite(&Sprites[j], &WorldOT[activeBuff], 2);
           }
           i=HEAD.ScoreHearts;
           k=1;
           while (i>0) {
                 j=(i%10)+140;
                 Sprites[j].x = 530-k*24;      Sprites[j].y = 78+32*2;
                 GsSortSprite(&Sprites[j], &WorldOT[activeBuff], 1);
                 i/=10;
                 k++;
           }
        }
        if (HEAD.NameSoliter[0]>0) {
           for (i=0;i<8;i++) {
               j=HEAD.NameSoliter[i];
               Sprites[j].x=110+i*24;      Sprites[j].y=78+32*3;
               GsSortSprite(&Sprites[j], &WorldOT[activeBuff], 2);
           }
           i=HEAD.ScoreSoliter;
           k=1;
           while (i>0) {
                 j=(i%10)+140;
                 Sprites[j].x = 530-k*24;      Sprites[j].y = 78+32*3;
                 GsSortSprite(&Sprites[j], &WorldOT[activeBuff], 1);
                 i/=10;
                 k++;
           }
        }
        GsSortSprite(&Sprites[bgLUp], &WorldOT[activeBuff], 20);
        GsSortSprite(&Sprites[bgMUp], &WorldOT[activeBuff], 20);
        GsSortSprite(&Sprites[bgRUp], &WorldOT[activeBuff], 20);

        VSync(0);     /* waits for vertical retrace */
        GsSwapDispBuff();
        //GsSortClear(0, 0, 0, &WorldOT[activeBuff]);

        GsDrawOt(&WorldOT[activeBuff]);
    }
}

Draw_AskName()
{
    int activeBuff;

    for (ai=0;ai<4;ai++) {
        activeBuff=GsGetActiveBuff();
        GsSetWorkBase((PACKET*)GpuPacketArea[activeBuff]);
        GsClearOt(0, 0, &WorldOT[activeBuff]);

        for (i=0;i<8;i++) {
            j=Name[i];
            Sprites[j].x=276+i*24;      Sprites[j].y=70;
            GsSortSprite(&Sprites[j], &WorldOT[activeBuff], 1);
        }

        if (CurName<8) {
            Sprites[pSelect].x=276+CurName*24;      Sprites[pSelect].y=70;
            GsSortSprite(&Sprites[pSelect], &WorldOT[activeBuff], 2);
        }

        Sprites[pSelect].x=Alpha.x*24+198;
        Sprites[pSelect].y=Alpha.y*12+95;
        GsSortSprite(&Sprites[pSelect], &WorldOT[activeBuff], 2);

        Sprites[Alphabet].x=198;
        Sprites[Alphabet].y=95;
        GsSortSprite(&Sprites[Alphabet], &WorldOT[activeBuff], 1);

        GsSortSprite(&Sprites[bgLUp], &WorldOT[activeBuff], 20);
        GsSortSprite(&Sprites[bgMUp], &WorldOT[activeBuff], 20);
        GsSortSprite(&Sprites[bgRUp], &WorldOT[activeBuff], 20);

        VSync(0);     /* waits for vertical retrace */
        GsSwapDispBuff();
        //GsSortClear(0, 0, 0, &WorldOT[activeBuff]);

        GsDrawOt(&WorldOT[activeBuff]);
    }
}

SayIntro()
{
    int activeBuff;

    for (i=0;i<4;i++) {
        activeBuff=GsGetActiveBuff();
        GsSetWorkBase((PACKET*)GpuPacketArea[activeBuff]);
        GsClearOt(0, 0, &WorldOT[activeBuff]);

        GsSortSprite(&Sprites[bgLUp], &WorldOT[activeBuff], 2);
        GsSortSprite(&Sprites[bgMUp], &WorldOT[activeBuff], 2);
        GsSortSprite(&Sprites[bgRUp], &WorldOT[activeBuff], 2);

        VSync(0);     /* waits for vertical retrace */
        GsSwapDispBuff();
        //GsSortClear(0, 0, 0, &WorldOT[activeBuff]);

        GsDrawOt(&WorldOT[activeBuff]);
    }
}

SayAbout()
{
    int activeBuff;

    activeBuff=GsGetActiveBuff();
    GsSetWorkBase((PACKET*)GpuPacketArea[activeBuff]);
    GsClearOt(0, 0, &WorldOT[activeBuff]);

    GsSortSprite(&Sprites[Auth1], &WorldOT[activeBuff], 1);
    GsSortSprite(&Sprites[Auth2], &WorldOT[activeBuff], 1);

    GsSortSprite(&Sprites[bgLUp], &WorldOT[activeBuff], 2);
    GsSortSprite(&Sprites[bgMUp], &WorldOT[activeBuff], 2);
    GsSortSprite(&Sprites[bgRUp], &WorldOT[activeBuff], 2);

    VSync(0);     /* waits for vertical retrace */
    GsSwapDispBuff();
    //GsSortClear(0, 0, 0, &WorldOT[activeBuff]);

    GsDrawOt(&WorldOT[activeBuff]);
}

SayHelp()
{
    int activeBuff;

    for (i=0;i<4;i++) {
        activeBuff=GsGetActiveBuff();
        GsSetWorkBase((PACKET*)GpuPacketArea[activeBuff]);
        GsClearOt(0, 0, &WorldOT[activeBuff]);

        GsSortSprite(&Sprites[bgLUp], &WorldOT[activeBuff], 2);
        GsSortSprite(&Sprites[bgMUp], &WorldOT[activeBuff], 2);
        GsSortSprite(&Sprites[bgRUp], &WorldOT[activeBuff], 2);

        GsSortSprite(&Sprites[bgHelp1], &WorldOT[activeBuff], 1);
        GsSortSprite(&Sprites[bgHelp2], &WorldOT[activeBuff], 1);

        VSync(0);     /* waits for vertical retrace */
        GsSwapDispBuff();
        //GsSortClear(0, 0, 0, &WorldOT[activeBuff]);

        GsDrawOt(&WorldOT[activeBuff]);
    }
}

SayMsg()
{
    int activeBuff;

    for (i=0;i<4;i++) {
        activeBuff=GsGetActiveBuff();
        GsSetWorkBase((PACKET*)GpuPacketArea[activeBuff]);
        GsClearOt(0, 0, &WorldOT[activeBuff]);

        GsSortSprite(&Sprites[bgLUp], &WorldOT[activeBuff], 2);
        GsSortSprite(&Sprites[bgMUp], &WorldOT[activeBuff], 2);
        GsSortSprite(&Sprites[bgRUp], &WorldOT[activeBuff], 2);

        Sprites[TheMsg].x=(640-Sprites[TheMsg].w)/2;
        Sprites[TheMsg].y=(240-Sprites[TheMsg].h)/2;
        GsSortSprite(&Sprites[TheMsg], &WorldOT[activeBuff], 0);

        VSync(0);     /* waits for vertical retrace */
        GsSwapDispBuff();
        //GsSortClear(0, 0, 0, &WorldOT[activeBuff]);

        GsDrawOt(&WorldOT[activeBuff]);
    }
}

SayLoad()
{
    int activeBuff;

    for (i=0;i<4;i++) {
        activeBuff=GsGetActiveBuff();
        GsSetWorkBase((PACKET*)GpuPacketArea[activeBuff]);
        GsClearOt(0, 0, &WorldOT[activeBuff]);

        Sprites[Loading].x=(640-Sprites[Loading].w)/2;
        Sprites[Loading].y=200;
        GsSortSprite(&Sprites[Loading], &WorldOT[activeBuff], 2);

        VSync(0);     /* waits for vertical retrace */
        GsSwapDispBuff();
        GsSortClear(0, 0, 0, &WorldOT[activeBuff]);

        GsDrawOt(&WorldOT[activeBuff]);
    }
}


//  ============================= System ===============================


ReadKey()
{
    //VSync(0);
    if (PadData!=0) PadData=0;
    TheData=PadRead(0);
    if (LastData!=TheData) {
       PadData=TheData;
       LastData=PadData;
    }
}


int cd_read_sub(char *name,void *addr)
{
        CdlFILE finfo;
        short i;
        short cnt;

        for (i=0;i<20;i++)
        {
          if (CdSearchFile(&finfo,name)!=0) break;
          else return -1;
        }

        for (i=0;i<20;i++)
        {
          CdReadFile(name,addr,finfo.size);
          while ((cnt=CdReadSync(1,0))>0) {
                VSync(0);
          }
          if (cnt==0) break;
        }
        return 0;
}

InitDeck()
{
        for (i=0; i<13; i++) {
            SrcDeck[i].Value=i+2;
            SrcDeck[i].Suit= cHearts;
        }
        for (i=13; i<26; i++) {
            SrcDeck[i].Value=i-11;
            SrcDeck[i].Suit= cCross;
        }
        for (i=26; i<39; i++) {
            SrcDeck[i].Value=i-24;
            SrcDeck[i].Suit= cDiamonds;
        }
        for (i=39; i<52; i++) {
            SrcDeck[i].Value=i-37;
            SrcDeck[i].Suit= cSpades;
        }
}

SortDeck()
{
        ClearSelection();
        srand(GetRCnt(0));
        i=rand();
        srand(i);
        rand();
        for (i=0; i<52; i++) {
            Deck[i]=-1;
            TempDeck[i]=-1;
        }
        i=rand(); srand(i);
        for (i=0; i<52; i++) {
            j=rand();
            j/=638;
            if (Deck[j]>-1) {
               for (k=0; k<52; k++) {
                   if (Deck[k]==-1) {
                      Deck[k]=i;
                      break;
                   }
               }
            } else {
               Deck[j]=i;
            }
        }
        for (i=0; i<52; i++) {
            j=rand();
            j/=638;
            if (TempDeck[j]>-1) {
               for (k=0; k<52; k++) {
                   if (TempDeck[k]==-1) {
                      TempDeck[k]=Deck[i];
                      break;
                   }
               }
            } else {
               TempDeck[j]=Deck[i];
            }
        }
        for (i=0; i<52; i++) {
            Deck[i]=-1;
        }
        for (i=0; i<52; i++) {
            j=rand();
            j/=638;
            if (Deck[j]>-1) {
               for (k=0; k<52; k++) {
                   if (Deck[k]==-1) {
                      Deck[k]=TempDeck[i];
                      break;
                   }
               }
            } else {
               Deck[j]=TempDeck[i];
            }
        }
}

SortDeck32()
{
        ClearSelection();
        for (i=0; i<52; i++) {
            Deck[i]=-1;
            TempDeck[i]=-1;
        }
        srand(GetRCnt(0));
        i=rand(); srand(i);
        for (i=0; i<32; i++) {
            j=rand();
            j/=1042;
            if (Deck[j]>-1) {
               for (k=0; k<32; k++) {
                   if (Deck[k]==-1) {
                      if (i<8) {
                         l=i+cHearts7;
                      } else {
                         if (i<16) {
                            l=i+cCross7-8;
                         } else {
                            if (i<24) {
                               l=i+cDiamonds7-16;
                            } else {
                               l=i+cSpades7-24;
                            }
                         }
                      }
                      Deck[k]=l;
                      break;
                   }
               }
            } else {
               if (i<8) {
                  l=i+cHearts7;
               } else {
                  if (i<16) {
                     l=i+cCross7-8;
                  } else {
                     if (i<24) {
                        l=i+cDiamonds7-16;
                     } else {
                        l=i+cSpades7-24;
                     }
                  }
               }
               Deck[j]=l;
            }
        }
        for (i=0; i<32; i++) {
            j=rand();
            j/=1042;
            if (TempDeck[j]>-1) {
               for (k=0; k<32; k++) {
                   if (TempDeck[k]==-1) {
                      TempDeck[k]=Deck[i];
                      break;
                   }
               }
            } else {
               TempDeck[j]=Deck[i];
            }
        }
        for (i=0; i<52; i++) {
            Deck[i]=-1;
        }
        for (i=0; i<32; i++) {
            j=rand();
            j/=1042;
            if (Deck[j]>-1) {
               for (k=0; k<32; k++) {
                   if (Deck[k]==-1) {
                      Deck[k]=TempDeck[i];
                      break;
                   }
               }
            } else {
               Deck[j]=TempDeck[i];
            }
        }
}

SortDeck36()
{
        ClearSelection();
        for (i=0; i<52; i++) {
            Deck[i]=-1;
            TempDeck[i]=-1;
        }
        srand(GetRCnt(0));
        i=rand(); srand(i);
        for (i=0; i<36; i++) {
            j=rand();
            j/=925;
            if (Deck[j]>-1) {
               for (k=0; k<36; k++) {
                   if (Deck[k]==-1) {
                      if (i<9) {
                         l=i+cHearts6;
                      } else {
                         if (i<18) {
                            l=i+cCross6-9;
                         } else {
                            if (i<27) {
                               l=i+cDiamonds6-18;
                            } else {
                               l=i+cSpades6-27;
                            }
                         }
                      }
                      Deck[k]=l;
                      break;
                   }
               }
            } else {
               if (i<9) {
                  l=i+cHearts6;
               } else {
                  if (i<18) {
                     l=i+cCross6-9;
                  } else {
                     if (i<27) {
                        l=i+cDiamonds6-18;
                     } else {
                        l=i+cSpades6-27;
                     }
                  }
               }
               Deck[j]=l;
            }
        }
        for (i=0; i<36; i++) {
            j=rand();
            j/=925;
            if (TempDeck[j]>-1) {
               for (k=0; k<36; k++) {
                   if (TempDeck[k]==-1) {
                      TempDeck[k]=Deck[i];
                      break;
                   }
               }
            } else {
               TempDeck[j]=Deck[i];
            }
        }
        for (i=0; i<52; i++) {
            Deck[i]=-1;
        }
        for (i=0; i<36; i++) {
            j=rand();
            j/=925;
            if (Deck[j]>-1) {
               for (k=0; k<36; k++) {
                   if (Deck[k]==-1) {
                      Deck[k]=TempDeck[i];
                      break;
                   }
               }
            } else {
               Deck[j]=TempDeck[i];
            }
        }
}

RunDelay()
{
        short kk;
        int ll;

        for (kk=0;kk<25;kk++) {
            ll=VSync(-1);
            while (ll==VSync(-1)) VSync(0);
        }
}

SlowDelay()
{
        short kk;
        int ll;

        for (kk=0;kk<3;kk++) {
            ll=VSync(-1);
            while (ll==VSync(-1)) VSync(0);
        }
}

CheckBonus()
{
    if ((HEAD.ScoreNine>=70)  && (HEAD.ScoreKing>=50) &&
        (HEAD.ScoreHearts<=40) && (HEAD.ScoreSoliter>=600)) Bonus=1;
    if (Cheater==1) Bonus=1;
}

ClearSelection()
{
    short i;
    for (i=0;i<52;i++) {
        Sprites[i].r=NormalBright;
        Sprites[i].g=NormalBright;
        Sprites[i].b=NormalBright;
    }
}
