#ifndef _GPS_H_
#define _GPS_H_

// PUBLIC:
void InitGPS(Uart& streamGPS);
void ReadGPS(Uart& streamGPS);

// GLOBALS:
typedef struct gpsdata_t {
    bool    fFix;           // do we have GPS data? Don't try to read GPS data until this is true
                            // and gpsQuality is >= 1

    uint8_t hours;          // time GMT
    uint8_t minutes;        // time GMT
    uint8_t seconds;        // time GMT
    uint8_t day;            // day of month
    uint8_t month;          // month
    uint16_t year;          // year

    signed int degreesLat;  // degrees latitude
    char hemLat;            // N or S
    uint8_t minutesLat;     // minutes latitude
    double secondsLat;      // seconds latitude
    double decLat;          // Latitude in decimal degrees, + for north

    signed int degreesLong; // degrees longitude
    char hemLong;           // E or W
    uint8_t minutesLong;    // minutes longitude
    double secondsLong;     // seconds longitude
    double decLong;         // Longitude in decimal degrees, + for east

    uint8_t gpsQuality;     // 0 = no fix, 1 = GPS, 2 = DGPS
    uint8_t cSatellites;    // number of satellites
    float hdop;             // HDOP (Horizontal Dilution of Precision)
    float altitude;         // altitude in meters
    float geosep;           // geoidal separation in meters
    float speed;            // speed over the ground in knots
    float cmg;              // "course made good" aka direction of travel in degrees
} GPSDATA; 

extern GPSDATA gpsdata;     // this global is how everyone finds out GPS data. 


// PRIVATE:
void ProcessNMEASentence(char* psz, size_t cch);
bool IsValidNMEASentence(char* psz, size_t cch);
void ParseGGA(char* psz);
void ParseRMC(char* psz);

#endif