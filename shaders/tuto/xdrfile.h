/*
 * $Id: xdrfile.h,v 1.1.1.1 2006/08/14 10:08:32 marc Exp $
 *
 * This file is part of Gromacs 4.0, but the XDR routines in 
 * xdrfile.h / xdrfile.c form a completely separate module and can be 
 * used in any program.
 *
 * Copyright (c) Erik Lindahl, David van der Spoel 2003.
 *
 * The Gromacs package is licensed under the GPL, but this file can be 
 * redistributed freely under the BSD license, and even used in 
 * closed source proprietary applications, if appropriate credit is given.
 *
 * Defines and macros you might want to use:
 * 
 * HAVE_XDR         Use system RPC/XDR libraries instead of our
 *                  own implementation. This can be useful if you have
 *                  very strange hardware with non-IEEE floating-point
 *                  (VAX) or other incompatibilities. 
 *                  Obviously it wont hurt to enable in other cases.
 *                  On some systems you might have to link with -lnsl
 *                  
 *                  If you dont define this (MS Windows doesnt have XDR,
 *                  for instance), we use a standalone XDR implementation
 *                  included in xdrfile.c
 *
 * F77_FUNC         Macro to define name mangling for the Fortran77 interface.
 *                  By default we keep the names in lower case and append an
 *                  underscore. Alternative name mangling can be defined like:
 *
 *                  #define F77_FUNC(name,NAME) _ ## NAME
 *                  (Use upper case, prepend an underscore)
 *
 * HAVE_FSEEKO      Define this if you have support for large files (>2GB).
 *                  You might need to add a define like __LARGEFILE=1 to enable
 *                  the 64-bit fseeko/ftello routines. Even if you dont define
 *                  this, large files might be supported, but you should not
 *                  try to use seek operations to set/get positions above 2GB. 
 */

#ifndef _xdrfile_h_
#define _xdrfile_h_

#include <stdio.h>

#ifndef F77_FUNC
#define F77_FUNC(name,NAME) name ## _
#endif

typedef struct {
  FILE   *fp;
  void   *xdr; 
  char    mode;
  int    *buf1;
  int     buf1size;
  int    *buf2;
  int     buf2size;
} XDRFILE;


#ifdef __cplusplus
extern "C" {
#endif

XDRFILE *
xdrfile_open(const char *path, const char *mode);

int
xdrfile_close(XDRFILE *fp);


/* Warning: The get/set file position routines in XDR might not be 
 * 64-bit compatible, so you are adviced _not_ to use these.
 * (64-bit read/writes should still work, though)
 */
#ifdef HAVE_FSEEKO /* 64-bit file IO */
off_t
long xdrfile_getpos(XDRFILE *xfp);
int
xdrfile_setpos(XDRFILE *xfp, off_t pos);
#else /* 32-bit file IO */
//unsigned int
long xdrfile_getpos(XDRFILE *xfp);
//int
void xdrfile_setpos(XDRFILE *xfp, long pos);//unsigned int pos);
#endif /* HAVE_FSEEKO */

int
xdrfile_read_char(char *ptr, int ndata, XDRFILE* xfp);

int
xdrfile_write_char(char *ptr, int ndata, XDRFILE* xfp);

int
xdrfile_read_u_char(unsigned char *ptr, int ndata, XDRFILE* xfp);

int
xdrfile_write_u_char(unsigned char *ptr, int ndata, XDRFILE* xfp);

int
xdrfile_read_short(short *ptr, int ndata, XDRFILE* xfp);

int
xdrfile_write_short(short *ptr, int ndata, XDRFILE* xfp);

int
xdrfile_read_u_short(unsigned short *ptr, int ndata, XDRFILE* xfp);

int
xdrfile_write_u_short(unsigned short *ptr, int ndata, XDRFILE* xfp);

int
xdrfile_read_int(int *ptr, int ndata, XDRFILE* xfp);

int
xdrfile_write_int(int *ptr, int ndata, XDRFILE* xfp);

int
xdrfile_read_u_int(unsigned int *ptr, int ndata, XDRFILE* xfp);

int
xdrfile_write_u_int(unsigned int *ptr, int ndata, XDRFILE* xfp);

/* Unfortunately no 64 bit integer calls, since not all XDR implementations
 * support them. Separate the data into two 32 bit integers and write
 * them separately instead (that way it is more portable anyway).
 */

int
xdrfile_read_float(float *ptr, int ndata, XDRFILE* xfp);

int
xdrfile_write_float(float *ptr, int ndata, XDRFILE* xfp);

int
xdrfile_read_double(double *ptr, int ndata, XDRFILE* xfp);

int
xdrfile_write_double(double *ptr, int ndata, XDRFILE* xfp);

int
xdrfile_read_string(char *ptr, int maxlen, XDRFILE* xfp);

int
xdrfile_write_string(char *ptr, XDRFILE* xfp);

int
xdrfile_read_opaque(char *ptr, int nbytes, XDRFILE* xfp);

int
xdrfile_write_opaque(char *ptr, int nbytes, XDRFILE* xfp);


  /* Routines for reading/writing three-dimensional coordinates
   * using lossy compression. The coordinates are multiplied with 
   * precision, rounded to integers and then packed.
   *
   * The resulting files can be a factor 10 smaller than raw data.
   * 
   * We suggest using precision 1000.0 by default. This means that
   * the coordinate 54.194312 would be stored as 54.194
   * There are also some other tricks like packing of water molecules
   * to get the size down. 
   *
   * IMPORTANT-IMPORTANT-IMPORTANT:
   *
   * Do check the returned value when you are calling these routines
   * to write data. If it is smaller than ncoord an error occured.
   * This should never happen with normal data, but if your coordinates
   * are NaN or very large (>1e6) it is not possible to use the
   * compression routines.
   *
   * The return value is the only indication you get, since we dont 
   * want to write to standard error if you are using this in a program.
   */

int
xdrfile_read_compr_coord_float(float    *ptr,
			       int      *ncoord,
			       float    *precision,
			       XDRFILE*  xfp);

int
xdrfile_write_compr_coord_float(float   *ptr, /* length 3*ncoord */
				int      ncoord,
				float    precision,
				XDRFILE* xfp);
  
/* The double version of the compressed coordinate routine will NOT
 * give you any increased accuracy - the data is translated to float
 * internally. It is just provided as a convenient interface if your
 * program is double precision.
 * If you need double precision accuracy you should avoid using
 * the lossy compression routine.
 */ 
int
xdrfile_read_compr_coord_double(double   *ptr,
				int      *ncoord,
				double   *precision,
				XDRFILE*  xfp);

int
xdrfile_write_compr_coord_double(double   *ptr, /* length 3*ncoord */
				 int      ncoord,
				 double   precision,
				 XDRFILE* xfp);

#ifdef __cplusplus
}
#endif

#endif /* _xdrfile_h_ */
