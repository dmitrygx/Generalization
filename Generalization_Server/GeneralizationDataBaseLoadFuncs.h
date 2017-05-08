#pragma once

#include <windows.h>
#include <iostream>
#include "gdbms01.h"

//#if defined(_Windows) || defined(_WINDOWS) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
//# if !defined(DLL) && !defined(_DLL) && !defined(__DLL__)
//#  ifndef NODLL
//#   define NODLL
//#  endif
//# endif
//# if !defined(__DPMI16__) && !defined(__DPMI32__)
//#  if defined(__WIN32__) || defined(WIN32) || defined(_WIN32)
//#   define __FOR_WIN32__
//#  else
//#   define __FOR_WINDOWS__
//#  endif
//# else
//#  ifdef __DPMI32__
//#   define __FOR_DPMI32__
//#  else
//#   define __FOR_DPMI16__
//#  endif
//# endif
//#else
//# define __FOR_DOS__
//#endif
//
//#if defined(__FOR_DPMI32__) || defined(__FOR_WIN32__)
//# define A2NEAR
//# define A2HUGE
//# define A2FAR
//#else
//# define A2NEAR near
//# define A2HUGE huge
//# define A2FAR  far
//#endif
//
//#define USE_NEW_NETW
//
//#if defined(__FOR_WIN32__) || defined(__FOR_DPMI32__)
//# define USE_HASH_SEARCH
//#endif
//
//#ifndef USE_NEW_NETW
//#  define A2MAXDABUF 50
//#else
//#  if   defined(__FOR_DPMI32__) || defined(__FOR_WIN32__)
//#    define A2MAXDABUF 4096
//#  elif defined(__FOR_DPMI16__) || defined(__FOR_WINDOWS__)
//#    define A2MAXDABUF 512
//#  else /* __FOR_DOS__ */
//#    define A2MAXDABUF 128
//#  endif
//#endif
//
//#define  A2TRMAX    30
//
//#ifndef _DEF_TREF
//#define _DEF_TREF
//typedef long TREF;           /* far reference in the DA file        */
//#endif

//namespace GeneralizationDecl
//{
//	typedef struct a2trstln {      /*---------stack line structure--------*/
//		struct a2trstln  *sl;  /* pointer to previous line            */
//		unsigned short          d;  /* global level number                 */
//		char                    k;  /* vertex type: V_V - conv, V_O - net  */
//		char                   gs;  /* pseudostate: TS_N,_A,_P,_B,_E,_M    */
//		TREF                    r;       /*physical address of vertex      */
//	} STKLINE;
//
//	struct a2trcomm {             /*----------command structure----------*/
//		char                   m;  /* direction of movement               */
//		char                   d;  /* r/w operation type                  */
//		char                  kt;  /* key type                            */
//		char             *k;  /* pointer to key                      */
//		char                  vt;  /* value type                          */
//		char             *v;  /* pointer to value                    */
//		struct a2trcomm  *p;  /* pointer to next struct.             */
//	};
//	typedef struct a2trcomm TCOMMAND;
//
//	typedef struct a2dabuf  DABUF;
//	typedef struct a2dafcb DAFILE;
//
//	struct a2dabuf {           /*      the structure of the buffer     */
//		DAFILE    *da;       /* file control block                   */
//		long        blknmb;       /* block number                         */
//		DABUF   *next;       /* next in the used or free lists       */
//		DABUF   *prev;       /* previous in the list of used buffs   */
//		DABUF  *nextf;       /* next in the file list                */
//		DABUF  *prevf;       /* previous in the file list            */
//#ifdef USE_NEW_NETW
//		DABUF  *sNext;
//		DABUF  *sPrev;
//#endif
//#ifdef USE_HASH_SEARCH
//		DABUF  *hNext;
//		DABUF  *hPrev;
//#endif
//		char       flags;       /* DA buffer flags                      */
//		char        lock;       /* lock count                           */
//		char    block[1];       /* start of block buffer                */
//	};
//
//	struct a2datetm {
//		short year;
//		short month;
//		short day;
//		short hour;
//		short minute;
//		short second;
//	};
//
//	typedef struct {           /* structure of the file header block   */
//		struct a2datetm cordt;  /* date and time of file updating       */
//		long         fcounter;  /* file counter                         */
//		short           flags;  /* flags for DA file                    */
//		short          syslth;  /* sys. info. length in 0 block         */
//		short         blkflth;  /* full length of block                 */
//		short         blkulth;  /* user length of block                 */
//		long         freelist;  /* 1-st free block number               */
//		long          notused;  /* 1-st unused block number             */
//		long            space;  /* maximum block number                 */
//		long       reg_number;  /* registration number                  */
//		char       version[5];  /* number of version                    */
//		char            type1;  /* compatibility: protocol type         */
//		char            type2;  /* compatibility: journal life time     */
//		char    transLink[80];  /* transaction link file (old--journal) */
//		unsigned long  trTime;  /* transaction time                     */
//		char     cryptkey[32];  /* key of cryptography                  */
//	} DABLK0;
//
//	struct a2dafcb {           /*-------- file control table ----------*/
//		char    cbname[3 + 1];    /* DA control block signature -- "FCB"  */
//		char    filename[80];   /* file name                            */
//		int     fn;             /* file handler                         */
//		int     fnsj;           /* system journal file handler          */
//		int     opennmb;        /* number of open operations            */
//		DAFILE  *next;      /* pointer to next                      */
//		DABUF   *buf;       /* first buffer for the DA file         */
//#ifdef USE_HASH_SEARCH
//		int     daNum;
//#endif
//		char    server[16];     /* server name (for remote file)        */
//		int     channel;        /* channel number for server connection */
//		void   *ppd;       /* addr of last process descr. on server*/
//		DABLK0  blk0;           /* header of BLOCK0 in DA file          */
//		char    trType;         /* current transaction type (0,'r','w') */
//		char    trNumb;         /* number in transaction file table     */
//		char    trType1;        /* accuracy transaction type            */
//		char    key_wn[32];     /* uncrypted password (cryptkey)        */
//	};
//
//	struct a2trtree {
//		char             cbname[4]; /* block header                      */
//		DAFILE           *pdf; /* pointer to buffer control block   */
//		char              *nf; /* pointer to file name              */
//		struct a2trstln   *sl; /* pnt. to current stack line        */
//		struct a2trstln  *sl1; /* copy of stack (trans support)     */
//		struct a2trtree   *pt; /* next control block in list        */
//		struct a2trtree   *pf; /* nxt control block in the same file*/
//		char              trid[12]; /* trace identifier                  */
//		struct a2trtree  *trnxt; /* next in transaction list        */
//		void             *ppd; /* address of process on server      */
//		short                 blks; /* block size                        */
//		TCOMMAND                cm; /* for parameters transferring       */
//		char                   atr; /* index regime on/off               */
//		short                  lvl; /* level number from tree root       */
//		short                gllvl; /* level number in stack             */
//		TREF                   msk; /* vertex address mask               */
//		char                   nba; /* number of bits in vertex address  */
//		char                  emsg; /* message level                     */
//		char                   trc; /* trace level                       */
//		char                  pack; /* dense writing regime              */
//		char                     u; /* writing with replacing            */
//		char                 vtree; /* process is copy                   */
//		char             transType; /* type of transaction               */
//		char                    ro; /* read only process                 */
//	};
//
//	typedef DAFILE *PDAFILE;
//
//	typedef struct a2trtree TREE;
//
//	typedef struct
//	{
//		char Label[8];
//		char OpenReg[4];
//		TREE *base;
//		char PassWord[44];
//		int  FlagOpen;
//		int BaseFormat;
//		int CountItemsMet;
//		int SizeItemMet;
//		int TypeItemMet;
//		float UnitMet;
//		unsigned long Read;
//		unsigned long Write;
//		unsigned long Delete;
//		char FileName[MAX_PATH + 1];
//	} GBASE;
//
//	typedef unsigned __int64 BASE_INT64;
//	typedef short int BASE_INT;
//
//	typedef struct
//	{
//		unsigned long Number;
//		char Description[256];
//		char Type[8];
//		int Location;
//		char FileName[256];
//		int Action;
//		unsigned long Size;
//		unsigned long Adr;
//	} VIDEO_HEADER;
//
//	typedef  struct
//	{
//		unsigned long Number;
//		unsigned long NumberNew;
//		BASE_INT64 Scale;
//		unsigned int FlagScale;
//		long Key;
//		unsigned int FlagKey;
//		BASE_INT *pSem;
//		void *pSv;
//		void *pPr;
//		void *pSquare;
//		void *pMet;
//		char *pText;
//		VIDEO_HEADER *pVideo;
//		unsigned int FlagSem;
//		unsigned int FlagSv;
//		unsigned int FlagPr;
//		unsigned int FlagSquare;
//		unsigned int FlagMet;
//		unsigned int FlagText;
//		unsigned int FlagVideo;
//		long mSem;
//		long mSv;
//		long mPr;
//		long mSquare;
//		long mMet;
//		long mText;
//		long mVideo;
//		long qSem;
//		long qSv;
//		long qPr;
//		long qSquare;
//		long qMet;
//		long qText;
//		long qVideo;
//		long sSem;
//		long sSv;
//		long sPr;
//		long sSquare;
//		long sMet;
//		long sText;
//		long sVideo;
//		int Error[3];
//		BASE_INT Code[10];
//		unsigned int FlagReadWrite;
//	} GBASE_OBJECT;
//
//	typedef struct
//	{
//		long Key;
//		int Ident;
//		int Level;
//		int Current;
//		unsigned long Num;
//		BASE_INT Code[10];
//		int Type;
//		int FormatFlag;
//		int r1;
//		int r2;
//		int FlagKV;
//		int NumberKV;
//		GBASE *BaseKV;
//		char *NameKV;
//	} GBASE_QUERY;
//
//	const uint16_t OPEN_TREE = 0;
//	const uint16_t OPEN_CODE_SELECT = 1;
//	const uint16_t OPEN_CODE = 2;
//	const uint16_t READ_NEXT = 3;
//	const uint16_t READ_RETRY = 4;
//
//	/* Obj */
//	const uint32_t OBJ_CODE = 0;
//	const uint32_t OBJ_SEM = 1;
//	const uint32_t OBJ_SV = 2;
//	const uint32_t OBJ_PR = 4;
//	const uint32_t OBJ_SQUARE = 8;
//	const uint32_t OBJ_METRIC = 16;
//	const uint32_t OBJ_ALL = 31;
//	const uint32_t OBJ_TEXT = 32;
//	const uint32_t OBJ_VIDEO = 64;
//	const uint32_t OBJ_FULL = 127;
//	const uint32_t OBJ_SEM_HEX = 128;
//	const uint32_t OBJ_VIDEO_ALL = 2;
//	const uint32_t OBJ_SEM_PARTS = 256;
//	const uint32_t OBJ_SV_PARTS = 512;
//	const uint32_t OBJ_PR_PARTS = 1024;
//	const uint32_t OBJ_VIDEO_PARTS = 2048;
//	const uint32_t OBJ_PR_SORT = 4096;
//	const uint32_t OBJ_NUMBER = 8192;
//	const uint32_t OBJ_SCALE = 16384;
//	const uint32_t OBJ_CODE_SVPR = 32768;
//	const uint32_t OBJ_VIDEO_EXT = 65536;
//	const uint32_t OBJ_VIDEO_NAME = 131072;
//
//	/* Format */
//	const uint32_t BASE_NEW_FORMAT = 0;
//	const uint32_t BASE_OLD_FORMAT = 1;
//	const uint32_t BASE_OLD_FORMAT_DYN = 2;
//}

//using namespace GeneralizationDecl;

//typedef int(__cdecl *_BasePrintObjectToFile)(
//	char *FileName,
//	int RegOpenFile,
//	GBASE *cls,
//	GBASE_OBJECT *obj,
//	int Read,
//	char *tab,
//	int CountItemsMet,
//	int SizeItemMet,
//	int TypeItemMet
//);

class GeneralizationDataBaseLoadFuncs
{
private:
	HINSTANCE hinstLib;
public:
	GeneralizationDataBaseLoadFuncs();
	virtual ~GeneralizationDataBaseLoadFuncs();
	BaseOpen _BaseOpen;
	BaseOpenTextError _BaseOpenTextError;
	BaseCloseObject _BaseCloseObject;
	BaseClose _BaseClose;
	BaseInitObject _BaseInitObject;
	BaseInitQuery _BaseInitQuery;
	BaseTransMKO _BaseTransMKO;
	BaseReadObject _BaseReadObject;
	BaseObjectCount _BaseObjectCount;
	BaseCodeStr _BaseCodeStr;
	BaseWriteObject _BaseWriteObject;
	BaseDeleteObject _BaseDeleteObject;
};
