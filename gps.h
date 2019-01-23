#ifndef _GPS_H_
#define _GPS_H_

// PUBLIC:
void InitGPS(Uart& streamGPS);
void ReadGPS(Uart& streamGPS);

// PRIVATE:
void ProcessNMEASentence(char* psz, size_t cch);
bool IsValidNMEASentence(char* psz, size_t cch);

#endif