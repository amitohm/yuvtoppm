#ifndef _YUVTOPPM_H_
#define _YUVTOPPM_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


typedef char			INT8;
typedef unsigned char	UINT8;

typedef short			INT16;
typedef unsigned short	UINT16;

typedef int				INT32;
typedef unsigned int	UINT32;

#define	 MIN(X,Y)	((X) < (Y) ? (X) : (Y))

#define	 MAX(X,Y)	((X) > (Y) ? (X) : (Y))


void print_usage();

#endif
