/**
**  scsi.cpp
**  Saturday. September 21, 2002
**
**  Talk thru a Scsi connection to a device.
**
**  Provide a single common interface for all the types of Scsi
**  connection known to be available locally.  Choose by call:
**
**      1: pass thru to all
**      2: pass thru til one succeeds
**      3: pass thru only to the open connection
**
**  Grep views include:
**
**      grep "^[A-Za-z_]"   // routines
**/

/**
**  Link with standard C libraries.
**/

#include <limits.h> /* INT_MAX ... */
#include <stdio.h> /* FILE ... */
#include <stdlib.h> /* calloc ... */
#include <string.h> /* memset ... */

/**
**  Link with the *.c* of ../plscsi/.
**/

#include "plscsi.h"

/**
**  Describe a Scsi connection to a device.
**/

struct Scsi /* connection */
    {
    void *          theConnection;
#ifdef SGIO
    Sgio *          theSgio;            /* Linux since kernel 2.4 */
#endif
#ifdef XXXASPI
    Aspi *          theAspi;            /* Dos/Win 95/98/ME */
#endif
#ifdef SPTX
    Sptx *          theSptx;            /* Win NT/2K/XP */
#endif
    };

/**
**  Construct a new Scsi connection.
**
**  Return null to fail, else don't.
**/

Scsi * newScsi(void)
    {

    /* Alloc and zero a new Scsi. */

    Scsi * scsi = (Scsi *) calloc(1, sizeof (Scsi));
    if (scsi == NULL) return NULL;

    /* Say closed but allow open. */

    scsi->theConnection = NULL;
#ifdef SGIO
    scsi->theSgio = newSgio();
#endif
#ifdef XXXASPI
    scsi->theAspi = newAspi();
#endif
#ifdef SPTX
    int maxSptLength = -1; /* -1 = don't care */
    scsi->theSptx = newSptx(maxSptLength);
#endif

    /* Succeed. */

    return scsi;
    }

/**
**  Redirect the stderr of this Scsi connection.
**/

void scsiSetErr(Scsi * scsi, FILE * fi)
    {
    if (scsi == NULL) return;
#ifdef SGIO
    sgioSetErr(scsi->theSgio, fi);
#endif
#ifdef XXXASPI
    aspiSetErr(scsi->theAspi, fi);
#endif
#ifdef SPTX
    sptxSetErr(scsi->theSptx, fi);
#endif
    }

/**
**  Close this Scsi connection.
**/

void scsiClose(Scsi * scsi)
    {
    if (scsi == NULL) return;
#ifdef SGIO
    sgioClose(scsi->theSgio);
#endif
#ifdef XXXASPI
    ;
#endif
#ifdef SPTX
    sptxClose(scsi->theSptx);
#endif
    scsi->theConnection = NULL;
    }

/**
**  Open this Scsi connection.
**
**  Return zero to succeed, else don't.
**/

int scsiOpen(Scsi * scsi, char const * name)
    {
    if (scsi == NULL) return -1;
    scsi->theConnection = NULL;
#ifdef SGIO
    if (sgioOpen(scsi->theSgio, name) == 0)
        {
        scsi->theConnection = scsi->theSgio;
        return 0;
        }
#endif
#ifdef XXXASPI
    if (aspiOpen(scsi->theAspi, name) == 0)
        {
        scsi->theConnection = scsi->theAspi;
        return 0;
        }
#endif
#ifdef SPTX
    if (sptxOpen(scsi->theSptx, name) == 0)
        {
        scsi->theConnection = scsi->theSptx;
        return 0;
        }
#endif
    return -1;
    }

/**
**  Limit the count of sense bytes copied In by scsiSay.
**
**  Return zero if ok, else negative.
**/

int scsiLimitSense(Scsi * scsi, int maxSenseLength)
    {
    if (scsi == NULL) return -1;
#ifdef SGIO
    if (scsi->theConnection == scsi->theSgio)
        {
        int exitInt = sgioLimitSense(scsi->theSgio, maxSenseLength);
        return exitInt;
        }
#endif
#ifdef XXXASPI
    if (scsi->theConnection == scsi->theAspi)
        {
        int exitInt = aspiLimitSense(scsi->theAspi, maxSenseLength);
        return exitInt;
        }
#endif
#ifdef SPTX
    if (scsi->theConnection == scsi->theSptx)
        {
        int exitInt = sptxLimitSense(scsi->theSptx, maxSenseLength);
        return exitInt;
        }
#endif
    return -1;
    }

/**
**  Specify the min time to wait before cancelling scsiSay.
**
**  Let negative time mean allow the max possible time.
**
**  Return zero if ok, else negative.
**/

int scsiLimitSeconds(Scsi * scsi, INT s, INT ns)
    {
    if (scsi == NULL) return -1;
#ifdef SGIO
    if (scsi->theConnection == scsi->theSgio)
        {
        int exitInt = sgioLimitSeconds(scsi->theSgio, s, ns);
        return exitInt;
        }
#endif
#ifdef DOSASPI
    s = s; ns = ns;
    return -1;
#endif
#ifdef WINASPI
    if (scsi->theConnection == scsi->theAspi)
        {
        int exitInt = aspiLimitSeconds(scsi->theAspi, s, ns);
        return exitInt;
        }
#endif
#ifdef SPTX
    if (scsi->theConnection == scsi->theSptx)
        {
        int exitInt = sptxLimitSeconds(scsi->theSptx, s, ns);
        return exitInt;
        }
#endif
    return -1;
    }

/**
**  Speak a sentence of Scsi through a Scsi connection.
**
**  That is, copy the cdbLength of the cdbChars Out, copy between zero
**  and the maxLength of the dataChars In or Out, copy status In, and
**  copy sense bytes In if available.
**
**  Return zero if all was ok, positive to say specifically how many
**  less than all data bytes were copied, else negative.  Also return
**  zero if all was ok except that the number of data bytes copied is
**  unknown.
**/

INT scsiSay(Scsi * scsi,
        char const * cdbChars, int cdbLength, char * dataChars, INT maxLength, int direction)
    {
    if (scsi == NULL) return -1;
#ifdef SGIO
    if (scsi->theConnection == scsi->theSgio)
        {
        int exitInt = sgioSay(scsi->theSgio, cdbChars, cdbLength, dataChars, maxLength, direction);
        return exitInt;
        }
#endif
#ifdef XXXASPI
    if (scsi->theConnection == scsi->theAspi)
        {
        INT exitInt = aspiSay(scsi->theAspi, cdbChars, cdbLength, dataChars, maxLength, direction);
        return exitInt;
        }
#endif
#ifdef SPTX
    if (scsi->theConnection == scsi->theSptx)
        {
        int exitInt = sptxSay(scsi->theSptx, cdbChars, cdbLength, dataChars, maxLength, direction);
        return exitInt;
        }
#endif
    return -1;
    }

/**
**  Count the data bytes copied In or Out by scsiSay.
**
**  Guess the elseLength if the actual length of data bytes copied is
**  unknown.  Also if the scsi is null.
**/

INT scsiGetLength(Scsi * scsi, INT elseLength)
    {
    if (scsi == NULL) return elseLength;
#ifdef SGIO
    if (scsi->theConnection == scsi->theSgio)
        {
        int exitInt = sgioGetLength(scsi->theSgio, elseLength);
        return exitInt;
        }
#endif
#ifdef XXXASPI
    if (scsi->theConnection == scsi->theAspi)
        {
        INT exitInt = aspiGetLength(scsi->theAspi, elseLength);
        return exitInt;
        }
#endif
#ifdef SPTX
    if (scsi->theConnection == scsi->theSptx)
        {
        int exitInt = sptxGetLength(scsi->theSptx, elseLength);
        return exitInt;
        }
#endif
    return -1;
    }

/**
**  Copy some or all of the sense bytes copied by scsiSay.
**
**  Guess the elseLength were copied if the length copied is unknown,
**  except guess 0 if scsi is null.
**
**  Copy the min of the charsLength with the actual or guessed copy
**  length.
**
**  Return the count of bytes copied.
**/

int scsiGetSense(Scsi * scsi, char * chars, int charsLength, int elseLength)
    {
    if (scsi == NULL) return 0;
#ifdef SGIO
    if (scsi->theConnection == scsi->theSgio)
        {
        int exitInt = sgioGetSense(scsi->theSgio, chars, charsLength, elseLength);
        return exitInt;
        }
#endif
#ifdef XXXASPI
    if (scsi->theConnection == scsi->theAspi)
        {
        int exitInt = aspiGetSense(scsi->theAspi, chars, charsLength, elseLength);
        return exitInt;
        }
#endif
#ifdef SPTX
    if (scsi->theConnection == scsi->theSptx)
        {
        int exitInt = sptxGetSense(scsi->theSptx, chars, charsLength, elseLength);
        return exitInt;
        }
#endif
    return -1;
    }

/**
**  Read the first else the next well-known device path name.
**
**  Copy a name of up to charsLength to the chars.
**
**  Return the positive length of chars assigned, else zero to say the
**  name was too long (or if chars is null), else negative if no more
**  names exist (or if scsi is null).
**/

int scsiReadName(Scsi * scsi, char * chars, int charsLength)
    {
    if (scsi == NULL) return -1;

    int exitInt = 0;
#ifdef SGIO
    exitInt = sgioReadName(scsi->theSgio, chars, charsLength);
    if (0 <= exitInt) return exitInt;
#endif
#ifdef XXXASPI
    exitInt = aspiReadName(scsi->theAspi, chars, charsLength);
    if (0 <= exitInt) return exitInt;
#endif
#ifdef SPTX
    exitInt = sptxReadName(scsi->theSptx, chars, charsLength);
    if (0 <= exitInt) return exitInt;
#endif

    return -1;
    }

/**
**  Accept a -X option of a Scsi command line.
**
**  Return zero if ok, else negative.
**/

int scsiSwallowArg(Scsi * scsi, char const * arg)
    {
    if (scsi == NULL) return -1;
    if (arg == NULL) return -1;

    int exitInt = -1;
#ifdef SGIO
    int sgioInt = sgioSwallowArg(scsi->theSgio, arg);
    if (0 <= sgioInt) exitInt = 0;
#endif
#ifdef XXXASPI
    int aspiInt = aspiSwallowArg(scsi->theAspi, arg);
    if (0 <= aspiInt)
        {
        #ifdef SPTX
        scsi->theSptx = NULL;
        #endif
        exitInt = 0;
        }
#endif
#ifdef SPTX
    int sptxInt = sptxSwallowArg(scsi->theSptx, arg);
    if (0 <= sptxInt)
        {
        #ifdef XXXASPI
        scsi->theAspi = NULL;
        #endif
        exitInt = 0;
        }
#endif

    return exitInt;
    }

/* end of file */
