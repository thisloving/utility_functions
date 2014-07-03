#ifndef _zType_h_
#define _zType_h_

#include <string.h>
#include <time.h>
#include <iostream>

#include <map>
#include <set>
#include <vector>

typedef unsigned char 	BYTE;

typedef unsigned short  WORD;

typedef signed short	SWORD;

typedef unsigned int    DWORD;

typedef signed   int	SDWORD;

typedef unsigned long	QWORD;

typedef signed long		SQWORD;

#define SAFE_SUB(x, y)	((x) > (y) ? (x) - (y) : 0)

#define SAFE_DELETE(x)	{if (x) {delete (x); (x) = NULL;}}

#define SAFE_DELETE_VEC(x) {if (x) {delete[](x); (x) = NULL;}}

#define MIN(x, y)		((x) < (y) ? (x) : (y))

#define MAX(x, y)		((x) > (y) ? (x) : (y))

#define MAX_NAMESIZE	32

#endif  //_zType_h_
