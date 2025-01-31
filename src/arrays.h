#ifndef ARRAYS_H
#define ARRAYS_H

#include <stdint.h>

// pluto specific array 
struct periodicTerms {
	char j, s, p;
	long longA, longB;
	long latA, latB;
	long radA, radB;
};
extern  struct periodicTerms pt[43];

extern const char font5_7[96][5];

// ********  constellations and icons *************

extern const char moonbright[7];
extern const char moondim[7];
extern const char moondark[7];
extern const char moonphtext[8][7];

// containing information to draw 7 phases of moon  with 4 colors as 8x8 icons
//1st index is moon phase
//2nd index is color
//3rd index is the 8 lines containing 8 pixels each
extern const char moon_drawing[8][4][8];

// containing information to draw planet with 4 colors as 8x8 icons
// 1st index is planet with order :
//0 - Sun
//1 - Mercury
//2 - Venus
//3 - Earth
//4 - Mars
//5 - Jupiter
//6 - Saturn
//7 - Uranus
//8 - Neptune
//9 - Pluto
//2nd index is color
//3rd index is the 8 lines containing 8 pixels each
extern const char planet_drawing[10][4][8];

//extern const unsigned char planet_icon[10][32]; //replaced by planet_drawing
extern const unsigned char star_icon[9][8]; // not used


// Constellation names (Constellation and English name)
//	Constellation names and location to draw them			
//	RA * 1000	 Dec * 100	 Constellation_name English_name
// format: (NOT aligned!!!!)
// bytes  0-3: RA
// bytes  4-7: Dec
// bytes 8-27: Constellation name
// bytes 28-47:English name

extern const unsigned char constellation_array[];

// Constellation Lines
//				
//	Format:  <constellation name>	<RA-from>	<Dec-from>	<RA-to>	<Dec-to>
//				
//			 Right ascension is expressed as decimal hours * 1000 (12.234h => 12234)				
//			 Declination is expressed as decimal degrees * 100 (-85� => -8500)			

// format: bytes  0-2: 3-byte constellation abbreviation
//         byte   3  : 0 (null)
//         bytes  4-5: RA-from
//         bytes  6-7: Dec-from
//         bytes  8-9: RA-to
//         bytes 10-11:Dec-to

extern const uint16_t constellation_lines_array[];

// Constellation boundaries
//   Constellation boundary lines		
//		
//  Format:		
//          Move=0/Draw=1/End=0xffff	Right_Ascension	Declination 
//		
//  Right ascension is decimal hours * 1000		
//  Declination is decimal degrees * 100		
// format:
// bytes 0-1: move/draw/end code
// bytes 2-3: RA
// bytes 4-5: Dec

extern const uint16_t constellation_bound_array[];

// yale array (this is in file yale.c)
//#ifdef WITH_YALE
//extern const unsigned char yale_array[];
//#endif

#endif //ARRAYS_H

