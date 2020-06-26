 /*
  * UAE - The Un*x Amiga Emulator
  *
  * Try to include the right system headers and get other system-specific
  * stuff right & other collected kludges.
  *
  * If you think about modifying this, think twice. Some systems rely on
  * the exact order of the #include statements. That's also the reason
  * why everything gets included unconditionally regardless of whether
  * it's actually needed by the .c file.
  *
  * Copyright 1996, 1997 Bernd Schmidt
  */

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>
#include <limits.h>

#include "EmTypes.h"		// int8, int16, etc.

#ifndef __STDC__
#define __STDC__ 1
#endif
#ifndef __STDC__
#error "Your compiler is not ANSI. Get a real one."
#endif

#include <stdarg.h>

#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#ifdef HAVE_VALUES_H
#include <values.h>
#endif

#ifdef HAVE_STRINGS_H
#include <strings.h>
#endif
#ifdef HAVE_STRING_H
#include <string.h>
#endif

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#ifdef HAVE_FCNTL_H
#include <fcntl.h>
#endif

#ifdef HAVE_UTIME_H
#include <utime.h>
#endif

#ifdef HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif

#if TIME_WITH_SYS_TIME
# include <sys/time.h>
# include <time.h>
#else
# if HAVE_SYS_TIME_H
#  include <sys/time.h>
# else
#  include <time.h>
# endif
#endif

#if HAVE_DIRENT_H
# include <dirent.h>
#else
# define dirent direct
# if HAVE_SYS_NDIR_H
#  include <sys/ndir.h>
# endif
# if HAVE_SYS_DIR_H
#  include <sys/dir.h>
# endif
# if HAVE_NDIR_H
#  include <ndir.h>
# endif
#endif

#ifdef HAVE_SYS_UTIME_H
# include <sys/utime.h>
#endif

#include <errno.h>
#include <assert.h>

#if EEXIST == ENOTEMPTY
#define BROKEN_OS_PROBABLY_AIX
#endif

#ifdef __NeXT__
#define S_IRUSR S_IREAD
#define S_IWUSR S_IWRITE
#define S_IXUSR S_IEXEC
#define S_ISDIR(val) (S_IFDIR & val)
struct utimbuf
{
    time_t actime;
    time_t modtime;
};
#endif

#if defined(__GNUC__) && defined(AMIGA)
/* gcc on the amiga need that __attribute((regparm)) must */
/* be defined in function prototypes as well as in        */
/* function definitions !                                 */
#define REGPARAM2 REGPARAM
#else /* not(GCC & AMIGA) */
#define REGPARAM2
#endif

/* sam: some definitions so that SAS/C can compile UAE */
#if defined(__SASC) && defined(AMIGA)
#define REGPARAM2 
#define REGPARAM
#define S_IRUSR S_IREAD
#define S_IWUSR S_IWRITE
#define S_IXUSR S_IEXECUTE
#define S_ISDIR(val) (S_IFDIR & val)
#define mkdir(x,y) mkdir(x)
#define truncate(x,y) 0
#define creat(x,y) open("T:creat",O_CREAT|O_TEMP|O_RDWR) /* sam: for zfile.c */
#define strcasecmp stricmp
#define utime(file,time) 0
struct utimbuf
{
    time_t actime;
    time_t modtime;
};
#endif

#if defined(WARPUP)
#include "devices/timer.h"
#include "osdep/posixemu.h"
#define REGPARAM
#define REGPARAM2
#define RETSIGTYPE
#define USE_ZFILE
#define strcasecmp stricmp
#define memcpy q_memcpy
#define memset q_memset
#define strdup my_strdup
#define random rand
#define creat(x,y) open("T:creat",O_CREAT|O_RDWR,777)
extern void* q_memset(void*,int,size_t);
extern void* q_memcpy(void*,const void*,size_t);
#endif

#ifdef __DOS__
#include <pc.h>
#include <io.h>
#endif

/* Acorn specific stuff */
#ifdef ACORN

#define S_IRUSR S_IREAD
#define S_IWUSR S_IWRITE
#define S_IXUSR S_IEXEC

#define strcasecmp stricmp

#endif

#ifndef L_tmpnam
#define L_tmpnam 128 /* ought to be safe */
#endif

/* If char has more then 8 bits, good night. */

typedef int8	uae_s8;
typedef int16	uae_s16;
typedef int32	uae_s32;
typedef int64	uae_s64;

typedef uint8	uae_u8;
typedef uint16	uae_u16;
typedef uint32	uae_u32;
typedef uint64	uae_u64;

typedef emuptr	uaecptr;


#ifdef HAVE_STRDUP
#define my_strdup strdup
#else
extern char *my_strdup (const char*s);
#endif

extern void *xmalloc(size_t);

/* We can only rely on GNU C getting enums right. Mickeysoft VSC++ is known
 * to have problems, and it's likely that other compilers choke too. */
#ifdef __GNUC__
#define ENUMDECL typedef enum
#define ENUMNAME(name) name
#else
#define ENUMDECL enum
#define ENUMNAME(name) ; typedef int name
#endif

/*
 * Porters to weird systems, look! This is the preferred way to get
 * filesys.c (and other stuff) running on your system. Define the
 * appropriate macros and implement wrappers in a machine-specific file.
 *
 * I guess the Mac port could use this (Ernesto?)
 */

//#undef DONT_HAVE_POSIX
//#undef DONT_HAVE_REAL_POSIX /* define if open+delete doesn't do what it should */
//#undef DONT_HAVE_STDIO
//#undef DONT_HAVE_MALLOC

#if defined(WARPUP)
#define DONT_HAVE_POSIX
#endif

#if defined _WIN32

#if defined __WATCOMC__

#define O_NDELAY 0
#include <direct.h>
#define dirent direct
#define mkdir(a,b) mkdir(a)
#define strcasecmp stricmp

#elif defined __MINGW32__

#define O_NDELAY 0
#define mkdir(a,b) mkdir(a)

#endif

#endif /* _WIN32 */ 

#ifdef DONT_HAVE_POSIX

#define access posixemu_access
extern int posixemu_access (const char *, int);
#define open posixemu_open
extern int posixemu_open (const char *, int, int);
#define close posixemu_close
extern void posixemu_close (int);
#define read posixemu_read
extern int posixemu_read (int, char *, int);
#define write posixemu_write
extern int posixemu_write (int, const char *, int);
#undef lseek
#define lseek posixemu_seek
extern int posixemu_seek (int, int, int);
#define stat(a,b) posixemu_stat ((a), (b))
extern int posixemu_stat (const char *, STAT *);
#define mkdir posixemu_mkdir
extern int mkdir (const char *, int);
#define rmdir posixemu_rmdir
extern int posixemu_rmdir (const char *);
#define unlink posixemu_unlink
extern int posixemu_unlink (const char *);
#define truncate posixemu_truncate
extern int posixemu_truncate (const char *, long int);
#define rename posixemu_rename
extern int posixemu_rename (const char *, const char *);
#define chmod posixemu_chmod
extern int posixemu_chmod (const char *, int);
#define tmpnam posixemu_tmpnam
extern void posixemu_tmpnam (char *);
#define utime posixemu_utime
extern int posixemu_utime (const char *, struct utimbuf *);
#define opendir posixemu_opendir
extern DIR * posixemu_opendir (const char *);
#define readdir posixemu_readdir
extern struct dirent* readdir (DIR *);
#define closedir posixemu_closedir
extern void closedir (DIR *);

/* This isn't the best place for this, but it fits reasonably well. The logic
 * is that you probably don't have POSIX errnos if you don't have the above
 * functions. */
extern long dos_errno (void);

#endif

#ifdef DONT_HAVE_STDIO

extern FILE *stdioemu_fopen (const char *, const char *);
#define fopen(a,b) stdioemu_fopen(a, b)
extern int stdioemu_fseek (FILE *, int, int);
#define fseek(a,b,c) stdioemu_fseek(a, b, c)
extern int stdioemu_fread (char *, int, int, FILE *);
#define fread(a,b,c,d) stdioemu_fread(a, b, c, d)
extern int stdioemu_fwrite (const char *, int, int, FILE *);
#define fwrite(a,b,c,d) stdioemu_fwrite(a, b, c, d)
extern int stdioemu_ftell (FILE *);
#define ftell(a) stdioemu_ftell(a)
extern int stdioemu_fclose (FILE *);
#define fclose(a) stdioemu_fclose(a)

#endif

#ifdef DONT_HAVE_MALLOC

#define malloc(a) mallocemu_malloc(a)
extern void *mallocemu_malloc (int size);
#define free(a) mallocemu_free(a)
extern void mallocemu_free (void *ptr);

#endif

#ifdef X86_ASSEMBLY
#define ASM_SYM_FOR_FUNC(a) __asm__(a)
#else
#define ASM_SYM_FOR_FUNC(a)
#endif

#if defined USE_COMPILER
#undef NO_PREFETCH_BUFFER
#undef NO_EXCEPTION_3
#define NO_EXCEPTION_3
#define NO_PREFETCH_BUFFER
#endif

#include "target.h"

#ifndef O_BINARY
#define O_BINARY 0
#endif

#ifndef STATIC_INLINE
#define STATIC_INLINE static __inline__
#endif

#ifdef __cplusplus
}
#endif
