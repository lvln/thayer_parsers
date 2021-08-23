/* 
 * tv.c --- 
 * 
 * Author: Stephen Taylor & Brandon Guzman
 * Created: 12-02-2020
 * Version: 1.0
 * 
 * Description: 
 * 
 */

//generate good and bad inputs

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

/*
 * target: <tag> <lat> <long> <wid>, ,msgid>
aor 
ez 

1: +90  -180   0  0 
2: -90  +180   60 60
3 0     0      
4 -1    -1
5 +1     +1
         +128
				 -128


 */
typedef struct Coord {
	int8_t lat;
	int16_t lon;
	uint8_t min;
	uint8_t sec;
}cord_t;


void write_coord(FILE *fp, cord_t *p){
	fwrite(&p->lat, sizeof(int8_t),1 ,fp);                              
	fwrite(&p->min,sizeof(uint8_t), 1,fp);                              
	fwrite(&p->sec,sizeof(uint8_t), 1,fp);                              
	fwrite(&p->lon, sizeof(int16_t), 1, fp);
	fwrite(&p->min,sizeof(uint8_t), 1,fp);                             
	fwrite(&p->sec,sizeof(uint8_t), 1,fp);

}

cord_t *make_coord(int8_t lat, int16_t lon, uint8_t min, uint8_t sec){
	cord_t *p = malloc(sizeof(cord_t));
	p->lat = lat;                                                      
  p->lon = lon;                                                      
  p->min = min;                                                      
  p-> sec = sec;
	return p;

}
int main(){

	//first test with 0
	FILE *fp;
	
	cord_t *p1 = make_coord(-70, -170,0 ,60);
	cord_t *p2 = make_coord(70, 170, 30, 30);
	uint8_t ID = 1;
	uint8_t tp1 = 16;
	uint8_t tp2 = 32;
	uint8_t tp3 = 48;
	uint8_t WPN = 2;

	
	fp = fopen("./pass.1", "wb");
	if (fp == NULL) return 1;
	
	fwrite(&tp1, sizeof(uint8_t), 1, fp);
	write_coord(fp, p1);
	write_coord(fp,p2);
	fwrite(&ID, sizeof(uint8_t),1,fp);
	fclose(fp);


	fp = fopen("./pass.2","wb");
	if (fp == NULL) return 1;
	
	ID = 2;
	fwrite(&tp2, sizeof(uint8_t), 1, fp);
	p1->lat = 90;
	p1->lon = 0;
	p1->min = 1;
	p1->sec = 59;
	p2->lat = -90;
	p2->lon = 180;
	p2->min = 15;
	p2->sec = 45;
  write_coord(fp, p1);
	write_coord(fp, p2);
  fwrite(&ID, sizeof(uint8_t), 1, fp);                                        
  fclose(fp);

	fp = fopen("./pass.3", "wb");
	ID = 255;
	fwrite(&tp3, sizeof(uint8_t), 1, fp);
	p1->lat = -1;
	p1->lon = -1;
  write_coord(fp, p1);
  fwrite(&WPN, sizeof(uint8_t),1,fp);
  fwrite(&ID, sizeof(uint8_t), 1, fp);
  fclose(fp);


	fp = fopen("./pass.4", "wb");
  ID = 0;
	WPN = 1;
  fwrite(&tp3, sizeof(uint8_t), 1, fp);
  p1->lat = 1;
  p1->lon = -180;
  write_coord(fp, p1);
  fwrite(&WPN, sizeof(uint8_t),1,fp);
  fwrite(&ID, sizeof(uint8_t), 1, fp);
  fclose(fp);

	fp = fopen("./pass.5", "wb"); 
	ID = 127;
  fwrite(&tp1, sizeof(uint8_t), 1, fp);
  p1->lat = -45;
  p1->lon = 127;
	p2->lat = 89;
  p2->lon = -128;
	write_coord(fp, p1);
  write_coord(fp, p2);
  fwrite(&ID, sizeof(uint8_t), 1, fp);
  fclose(fp); 

	//only one coordinate for cmd 1
	fp = fopen("./fail.1", "wb");
  fwrite(&tp1, sizeof(uint8_t), 1, fp);
  write_coord(fp, p1);
  fwrite(&ID, sizeof(uint8_t), 1, fp);
  fclose(fp);

	//missing msg ID
	fp = fopen("./fail.2", "wb");
  fwrite(&tp1, sizeof(uint8_t), 1, fp);
  write_coord(fp, p1);
	write_coord(fp, p2); 
  fclose(fp);

	//weapon id that is not 1 or 2
	fp = fopen("./fail.3", "wb");
	WPN = 3;
  fwrite(&tp3, sizeof(uint8_t), 1, fp);
  write_coord(fp, p1);
  fwrite(&WPN, sizeof(uint8_t),1,fp);
	fwrite(&ID, sizeof(uint8_t), 1, fp); 
  fclose(fp);

	WPN = 2;
	//incorrect type
	fp = fopen("./fail.4", "wb");
	uint8_t tp4 = 4;
  fwrite(&tp4, sizeof(uint8_t), 1, fp);
  write_coord(fp, p1);
  fwrite(&WPN, sizeof(uint8_t),1,fp);
	fwrite(&ID, sizeof(uint8_t), 1, fp); 
  fclose(fp);
	

	//invalid lat
	fp = fopen("./fail.5", "wb");
  fwrite(&tp3, sizeof(uint8_t), 1, fp);
	p1->lat = 91;
  write_coord(fp, p1);
  fwrite(&WPN, sizeof(uint8_t),1,fp);
  fwrite(&ID, sizeof(uint8_t), 1, fp);
  fclose(fp);


	//invalid long
	fp = fopen("./fail.6","wb");
  fwrite(&tp2, sizeof(uint8_t), 1, fp);
	p1->lat = 56;
  p1->lon = -200;
  write_coord(fp, p1);
  write_coord(fp, p2);
  fwrite(&ID, sizeof(uint8_t), 1, fp);
  fclose(fp);

	//invalid minute
	fp = fopen("./fail.7","wb");
  fwrite(&tp2, sizeof(uint8_t), 1, fp);
  p1->lon = -75;
	p1->min = 90;
  write_coord(fp, p1);
  write_coord(fp, p2);
  fwrite(&ID, sizeof(uint8_t), 1, fp);
  fclose(fp);
	
	free(p1);
	free(p2);
	return 0;
	
	
}
