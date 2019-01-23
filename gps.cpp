#include "LightVest2.h"
#include "utils.h"
#include "gps.h"

// The GPS communicates with the vest over a serial (Uart) port at 9600 baud.
// It sends "sentences" in a standard format called NMEA. We ignore all but
// the GGA and RMC sentences which get us everything we need to know.
//
// All this GPS parsing code will make a lot more sense
// if you have read the NMEA spec:
//
// https://www.winsystems.com/wp-content/uploads/software/nmea.pdf
//
// There is a substantially more elaborate and buggy library from Adafruit
// for parsing which does more than we need and not all that well.
//

#define cchBuf 84   /* max length of NMEA string is 82 */

GPSDATA gpsdata;
char rgchBuf[cchBuf];
size_t ixBuf = 0;

void InitGPS(Uart& streamGPS)
{
    memset(&gpsdata, 0, sizeof(GPSDATA));
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

    psz[cch-5] = '\0';      // chop off checksum and crlf; we don't need them any more

    if (0 == strncmp(psz+3, "GGA", 3))
    {
        ParseGGA(psz+7);
    }
    else if (0 == strncmp(psz+3, "RMC", 3))
    {
        ParseRMC(psz+7);
    }
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

    // Test checksum
    unsigned int sum = 0;
    for (size_t ix = 1; ix < cch-5; ix++)
    {
        sum ^= psz[ix];
    }

    if (sum != strtol( psz + cch - 4, NULL, 16 ))
        return false;

    return true;
}

void ParseGGA(char* psz)
{
    int ix = 0;
    unsigned long ltmp = 0;
    char* tok;

    while (tok = strsep(&psz, ","))
    {
        if (*tok)
        {

            switch(ix)
            {
                case 0:     // Time
                    ltmp = atol(tok);
                    
                    gpsdata.hours = ltmp / 10000;
                    gpsdata.minutes = ltmp % 10000 / 100;
                    gpsdata.seconds = ltmp % 100;

                    DebugPrintf("time %d:%d:%d\n", gpsdata.hours, gpsdata.minutes, gpsdata.seconds);
                    break;

                case 1:     // Lat 
                    ParseAngle(tok, 4, gpsdata.lat);
                    break;

                case 2:     // Lat hem
                    gpsdata.lat.hem = tok[0];
                    if (tok[0] == 'S')
                        gpsdata.lat.dec = -gpsdata.lat.dec;

                    DebugPrintf("lat hem %c\n", gpsdata.lat.hem);
                    DebugPrintf("lat %ddeg %dmin %fsec or %F\n", gpsdata.lat.degrees, gpsdata.lat.minutes, gpsdata.lat.seconds, gpsdata.lat.dec);
                    break;

                case 3:     // Long
                    ParseAngle(tok, 5, gpsdata.lng);
                    break;

                case 4:     // Long hem
                    gpsdata.lng.hem = tok[0];
                    if (tok[0] == 'W')
                        gpsdata.lng.dec = -gpsdata.lng.dec;

                    DebugPrintf("long hem %c\n", gpsdata.lng.hem);
                    DebugPrintf("long %ddeg %dmin %fsec or %F\n", gpsdata.lng.degrees, gpsdata.lng.minutes, gpsdata.lng.seconds, gpsdata.lng.dec);
                    break;

                case 5:     // GPS quality
                    DebugPrintf("gps qual %s\n", tok);
                    break;

                case 6:     // Num satellites
                    DebugPrintf("num sat %s\n", tok);
                    break;

                case 7:     // hdop
                    DebugPrintf("hdop %s\n", tok);
                    break;

                case 8:     // altitude
                    DebugPrintf("alt %s\n", tok);
                    break;

                case 10:    // geoseparation
                    DebugPrintf("geosep %s\n", tok);
                    break;

            }
        }
        
        ix++;
    }

}

void ParseRMC(char* psz)
{
    int ix = 0;
    char* tok;

    while (tok = strsep(&psz, ","))
    {
        if (*tok)
        {
            switch(ix)
            {
                // most of these tokens were already received
                // in the GGA so I'm gonna ignore them.
                case 1:     // Status (A = valid, V = warning )
                    DebugPrintf("!status %s\n", tok);
                    break;

                case 6:     // speed in knots
                    DebugPrintf("!speed %s\n", tok);
                    break;

                case 7:     // angle of travel
                    DebugPrintf("!angle of travel %s\n", tok);
                    break;

                case 8:     // date
                    DebugPrintf("!date %s\n", tok);
                    break;
            }
        }

        ix++;
    }

}

void ParseAngle(char* psz, size_t ixDot, ANGLE& angle)
{
    unsigned long ltmp = atol(psz);

    angle.degrees = ltmp / 100;
    angle.minutes = ltmp % 100;
    angle.seconds = atof(psz+ixDot) * 60.0;
    angle.dec = angle.degrees + 
                angle.minutes / 60.0 +
                angle.seconds / 3600.0;

}
