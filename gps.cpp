#include "LightVest2.h"
#include "utils.h"
#include "gps.h"

#define cchBuf 84   /* max length of NMEA string is 82 */

char rgchBuf[cchBuf];
size_t ixBuf = 0;

void InitGPS(Uart& streamGPS)
{
    streamGPS.begin(9600);
    ixBuf = 0;
}

void ReadGPS(Uart& streamGPS)
{   
    while (streamGPS.available())
    {
        if (ixBuf >= cchBuf - 1)
        {
            /* somehow the line is too long. maybe we missed
               the CRLF? Throw away and start over */
            ixBuf = 0;
            DebugPrintf("CRIKEY!!!!\n");
        }

        char c = streamGPS.read();
        rgchBuf[ixBuf++] = c;

        if (c == '\n')
        {
            rgchBuf[ixBuf] = '\0';
            ProcessNMEASentence(rgchBuf, ixBuf);
            ixBuf = 0;
        }
    }
}


void ProcessNMEASentence(char* psz, size_t cch)
{
    if (!IsValidNMEASentence(psz, cch))
    {   
        DebugPrintf("*** Invalid NMEA Sentence Discarded ***\n");
        return;
    }

    DebugPrintf("GPS: %s", psz);
}

bool IsValidNMEASentence(char* psz, size_t cch)
{
    // need at least $GPxxx, *xx, and crlf or it can't be valid 
    // this is mostly just protection to insure I can read checksum, sentence type, etc
    if (cch < 11) 
        return false;

    // check basic format of NMEA
    if (psz[0] != '$' || 
        psz[1] != 'G' ||
        psz[2] != 'P' ||
        psz[cch-1] != '\n' ||
        psz[cch-2] != '\r' ||
        psz[cch-5] != '*' )
    {
        return false;
    }

    // UNDONE CHECKSUM

    return true;
}