/**
**  plscsi.cpp
**  Saturday. September 21, 2002
**
**  Connect the command-line to a Scsi-over-whatever device.
**/

/**
**  Link with standard C libraries.
**/

#include <stdio.h> /* FILE ... */
#include <stdlib.h> /* calloc ... */
#include <string.h> /* memset ... */

/**
**  Link with the *.c* of ../plscsi/.
**/

#include "plscsi.h"

/**
**  Explain the command-line syntax in one line or two.
**/

static void fprintUsageLine(FILE * fi)
    {
    (void) fprintf(fi,
"Usage: plscsi\n"
        );
    }

/**
**  Explain the command-line syntax in one paragraph.
**/

static void fprintUsageParagraph(FILE * fi)
    {
    (void) fprintf(fi,
"Modified: Since Saturday. September 21, 2002 began.\n"
"Usage: plscsi --help\n"
"Usage: plscsi [$options] [$name...] // $comment...\n"
"Options: -hwl -vqpcyn -x $cdb -i $in -o $out\n"
"Options: -d $length -a $page $offset -f $from -t $to\n"
"Options: -X sense $n -X time $s $ns\n"
"Options: -X dos -X aspi -X sptx -X spt -X sptd -X sgio\n"
        );
    }

/**
**  Explain the command-line syntax in one page.
**/

static void fprintUsagePage(FILE * fi)
    {
    (void) fprintf(fi,
"Modified: Since Saturday. September 21, 2002 began.\n"
"\n"
"Usage: plscsi -$option $name... // $comment...\n"
"\n"
"Options:\n"
"\n"
"        -a --align $offset [ $length ]          // else 4 KiB\n"
"        -c --compare\n"
"        -d --data $length\n"
"        -f --from $fileName                     // -f \"\" is from stdin\n"
"        -h --help\n"
"        -i --in $length                         // else 0\n"
"        -l --loop\n"
"        -n --no\n"
"        -o --out $length                        // else 0\n"
"        -p --please                             // else forbid some things\n"
"        -q --quiet\n"
"        -t --to $fileName                       // -t \"\" is to stdout\n"
"        -v --verbose                            // else --quiet\n"
"        -w --whichever\n"
"        -x --x $cdb                             // else -x 12 0 0 0 24 0\n"
"        -X --X sense $length                    // else xE\n"
"        -X --X time $seconds [ $nano ]          // else max\n"
"        -y --yes\n"
"\n"
"Examples:\n"
"\n"
"        plscsi -w                               // list device names\n"
"        set PLSCSI=$name                        // choose a device name\n"
"        plscsi --                               // echo the chosen device name\n"
"\n"
"        plscsi -v -x \"12 0 0 0 24 0\" -i x24     // say Inquiry\n"
"        plscsi -v -x \"00 0 0 0 0 0\"             // say TestUnitReady\n"
"        plscsi -v -x \"1B 0 0 0 00 0\"            // say Stop\n"
"        plscsi -v -x \"1B 0 0 0 02 0\"            // say Stop and Eject\n"
"\n"
"        plscsi --loop -v -x \"00 FF FF FE\" -i x101 -t cycle.bin // write pattern\n"
"        plscsi --loop -v -x -o x210 -f cycle.bin // read pattern\n"
"        plscsi --loop -v -x \"00\" -i 1 -t x00.bin // write byte pattern\n"
        );
    }

/**
**  Print a positive or negative exitInt in decimal and hex.
**/

static void fprintExitInt(FILE * fi, char const * source, INT exitInt)
    {
    long exitLong = exitInt;
    if ((0 <= exitInt) && (exitInt <= 9))
        {
        (void) fprintf(fi, "// %ld = %s exit int\n",
            exitLong, source);
        }
    else if (exitInt < 0)
        {
        (void) fprintf(fi, "// -x%04lX = %ld = %s exit int\n",
            -exitLong, exitLong, source);
        }
    else
        {
        (void) fprintf(fi, "// %ld = x%lX = %s exit int\n",
            exitLong, exitLong, source);
        }
    }

/**
**  Run from the command line.
**/

int main(int argc, char const * const * argv)
    {

    /* Maybe explain usage in one paragraph. */

    --argc; ++argv;
    if (argc == 0)
        {
        fprintUsageParagraph(stderr);
        return -1;
        }

    /* Startup. */

    ScsiCommandLine * scl = newScsiCommandLine();
    if (scl == NULL) return -1;

    /* Accept args from the environment. */
    /* Accept args from the main command line. */

    char const * arg = getenv("PLSCSI");
    sclSetFromString(scl, arg);
    INT exitInt = sclParse(scl);
    if (exitInt == 0)
        {
        sclSet(scl, argv, argc);
        exitInt = sclParse(scl);
        }

    /* Maybe explain usage in one line. */

    if (exitInt != 0)
        {
        fprintUsageLine(stderr);
        return -1;
        }

    /* Maybe explain usage in one page. */

    int helpBit = sclGetHelpBit(scl);
    if (helpBit)
        {
        fprintUsagePage(stderr);
        return -1;
        }

    /* Speak to each device. */

    sclGuess(scl);
    exitInt = sclLimitSay(scl);
    if (exitInt == 0)
        {
        exitInt = sclAllotData(scl);
        }
    if (exitInt == 0)
        {
        int yesBit = sclGetYesBit(scl);
        int noBit = sclGetNoBit(scl);
        exitInt = -1;
        while (sclOpenNext(scl) == 0)
            {
            exitInt = sclSay(scl);

            /* Maybe speak repeatedly. */
            /* Reject positive residue only if it changes. */

            if (yesBit || noBit)
                {
                INT firstExitInt = exitInt;
                for (;;)
                    {
                    if (exitInt < 0)
                        {
                        if (noBit && !yesBit) break;
                        }
                    else if (exitInt == 0)
                        {
                        if (yesBit && !noBit) break;
                        }
                    else /* if (0 < exitInt) */
                        {
                        if (noBit && !yesBit)
                            {
                            if (exitInt != firstExitInt) break;
                            }
                        }
                    exitInt = sclSay(scl);
                    }
                }
            }
        sclClose(scl);
        }

    /* Complain of nonzero exit. */

    if ((exitInt != 0) || sclGetVerboseBit(scl))
        {
        fprintExitInt(stderr, "plscsi.main", exitInt);
        }

    /* Exit zero only if ok. */

    if (((EXIT_INT) exitInt) != exitInt)
        {
        exitInt = -1;
        }
    return ((int) exitInt);
    }

/**
**  Notes on Scsi limits across platforms:
**
**  Limits on the length of the -x $cdb include:
**
**      xC          // common max of Scsi over Ide
**      xC          // common max of Scsi over 1394 = (x20 - x14)
**      x10         // max Scsi over Ide
**      x10         // max Scsi over Usb
**      x10         // max declared Scsi via Dos Aspi or Win32 Ioctl
**      x10         // max Scsi-3 for standard op xXX other than x7F
**      xFF         // max expressible Scsi via Dos Aspi or Win32 Ioctl
**      xFF         // max expressible Scsi via Dos Aspi or Win32 Ioctl
**      x104        // max Scsi-3 permits for op x 7F XX XX = 8 + xFC
**      x107        // max Scsi-3 encodes for op x 7F XX XX = 8 + xFF
**      x3E8        // max Scsi over 1394 = ((xFF * 4) - x14)
**
**      July 2001 SPC-2 r20 requires some bridges to synthesise
**      SK ASC = x 5 24 IllegalRequest InvalidFieldInCdb
**      when an actual Cdb is too long.
**
**  Limits on --in $length and --out $length include:
**
**      x7FFF       // 32Ki > max positive int, if sizeof int is 16 bits
**      xFFF0       // 64Ki > max misaligned copy length of 16 bit Dos
**      xFFFF       // 64Ki > max aligned copy length of 16 bit Dos
**
**      x9FFFF      // 640Ki > max free memory in 16 bit Dos
**      xfFFFF      // 1Mi > max addressable memory in 16 bit Dos
**
**      x7fffFFFF   // 2Gi > max positive int, if sizeof int is 4 bytes
**
**  Limits on -X sense $length include:
**
**      x8      // just up to and including the additionalLength
**      xE      // Adaptec winaspi.h SENSE_LEN (xE = decimal 14)
**      xE      // just enough to include SK ASC ASCQ
**      x12     // just enough to include the SKSV info
**      x12     // min Scsi-3 [ tbd min for host or for device? ]
**      xFF     // max cdb4 allocationLength of Scsi op x03 RequestSense
**/

/**
**  Tbd:
**/

/* end of file */
