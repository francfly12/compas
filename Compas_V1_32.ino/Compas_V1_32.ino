/********************************************************************
* Compas magnetique pour avion ultraleger
* A base de capteur Adafruit Lsm303
* https://learn.adafruit.com/lsm303-accelerometer-slash-compass-breakout/overview
* Affichage Sur un display Oled pur chinois
*  Universal 8bit Graphics Library, https://github.com/olikraus/u8glib/
* Francois S. - Mar 2016  V  1.1
* Ce cablage est fait pour un arduino nano . peut s adapter a un autre
* Version 1.3   ;  affichage classique du compas simplifie
*********************************************************************/
// cablage le capteur et le display sont sur le meme bus I2C, il ne semble pas y avoir de conflit, alim 5V.
// SDA to I2C  data   sur Analog 4
// SCL to I2C  clock  sur Analog 5
 
#include "U8glib.h"
#include <Wire.h> //I2C Arduino Library
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#define address 0x1E //0011110b, I2C 7bit address of HMC5883 
/* Assign a unique ID to this sensor at the same time */
Adafruit_LSM303_Mag_Unified mag = Adafruit_LSM303_Mag_Unified(12345);

U8GLIB_SH1106_128X64 u8g(U8G_I2C_OPT_DEV_0|U8G_I2C_OPT_FAST);  // Dev 0, Fast I2C / TWI ,driver correspondant a mon modele d afficheur

int x,y,z; //triple axis data
int intcap;// Valeur en integer du cap 
char bufferCAP [20],bufferN [5]; 
int xPos = 0;   // position x de l affichage
int yPos = 0;   // position y de l affichage
int poscap = 0;  // position de l affichage du cap, selon 1,2 ou 3 digits
int posn,pose,posw,poss,pos3,pos6,pos12,pos15,pos21,pos24,pos30,pos33; // ce sont toutes les positions d affichage des symboles
float Pi = 3.14159;   
float cap,newcap,oldcap; 
int affdegres, affpix;  // utilise pour afficher les barres tous les dix degres
void setup(void) {
int bar =0;       // utilise pour compter l affichage des 7 barres tous les 10 degres

 // seulement pour des tests  // 
  Serial.begin(9600);
      Serial.println("Test du compas"); Serial.println("");
     // Initialise the sensor   
      if(!mag.begin())
      {
        // There was a problem detecting the LSM303 ... check your connections 
        Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
        while(1);
      }
  
 u8g.setFont(u8g_font_gdb30r);
// la fonte est documentee la https://github.com/olikraus/u8glib/wiki/fontgroupgentium
// partie affichage  
  x = 0;
  Wire.begin();
  
 }

void loop(void) {
 /* Get a new sensor event */ 
      sensors_event_t event; 
      mag.getEvent(&event);     
//      float Pi = 3.14159;      
      // Calculate the angle of the vector y,x
      float cap = (atan2(event.magnetic.y,-event.magnetic.x) * 180) / Pi;
      // ceci depend du position du capteur, si incorrect, essayer des signes - devant event    
      // Normalize to 0-360
      if (cap < 0)
      {
        cap = 360 + cap;
      }

oldcap=newcap;    //   lissage des nouvelles valeurs
newcap=cap;
if (((newcap-oldcap)>2)or ((oldcap-newcap)>2)) {newcap=cap;oldcap=cap; } //  pour verif // pas de lissage autour de 0 et 360 et aux grandes amplitudes
newcap= oldcap +((newcap-oldcap)/5);

 //      Serial.print("Compass cap: ");  //  pou verif
//      Serial.println(cap);
 //  casting du float en integer
  intcap=int(newcap);//  transformer le cap en integer 
  sprintf(bufferCAP, " %d", intcap);  // valeur a afficher
  
  u8g.firstPage();  //  debut du picture loop
  do {
    draw();
  } while( u8g.nextPage() ); // fin du picture loop
 delay(10);    // a modifier plus tard
    } //  fin du loop
  
void draw(void) {   //   fonction d affichage
    // pas de chiffre au centre    
    
// dessin partie du centre lettres et symboles   
u8g.drawBox(1,62,128,2); // affichage barre bas 1 pixel  
//   affichage partie basse du display ;
//  affichage N  ou symbole 0 suivant les gouts
if ((intcap < 75 ) or (intcap > 280)) {posn=2*(intcap)+52 ;if (posn >360) posn=posn-720; u8g.drawStr( posn-2,35,"N");};  // si visibilite de N, afficher
//  affichage 3 normal
if ((intcap > -45 ) and (intcap < 105)) {pos3=2*(intcap)-8 ;if (pos3 >360) pos3=pos3-720; u8g.drawStr( pos3+2,35,"3");};  //
//  affichage 3 a gauche du N
if ((intcap > 315 )) {pos3=2*(intcap)-8 ;if (pos3 >360) pos3=pos3-720; u8g.drawStr( pos3+2,35,"3");};  //
//  affichage 6
if ((intcap > -15 ) and (intcap < 135)) {pos6=2*(intcap)-68 ;if (pos6 >360) pos6=pos6-720; u8g.drawStr( pos6+2,35,"6");};  //
//  affichage W
if ((intcap > 15 ) and (intcap < 165)) {posw=2*(intcap)-128 ;if (posw >360) posw=posw-720; u8g.drawStr( posw-6,35,"W");};  // si visibilite de W, afficher
//  affichage 12
if ((intcap > 45 ) and (intcap < 195)) {pos12=2*(intcap)-188 ;if (pos12 >360) pos12=pos12-720; u8g.drawStr( pos12-10,35,"12");};  //
//  affichage 15
if ((intcap > 75 ) and (intcap < 225)) {pos15=2*(intcap)-248 ;if (pos15 >360) pos15=pos6-720; u8g.drawStr( pos15-10,35,"15");};  //
//  affichage S
if ((intcap > 105 ) and (intcap < 225)) {poss=2*(intcap)-308 ;if (poss >360) poss=poss-720; u8g.drawStr( poss,35,"S");};  // si visibilite de S, afficher
//  affichage 21
if ((intcap > 135 ) and (intcap < 285)) {pos21=2*(intcap)-368 ;if (pos21 >360) pos21=pos21-720; u8g.drawStr( pos21-10,35,"21");};  //
//  affichage 24
if ((intcap > 165 ) and (intcap < 315)) {pos24=2*(intcap)-428 ;if (pos24 >360) pos24=pos24-720; u8g.drawStr( pos24-12,35,"24");};  //
//  affichage E
if ((intcap > 195 ) and (intcap < 340)) {pose=2*(intcap)-488 ;if (pose >360) pose=pose-720; u8g.drawStr( pose,35,"E");};  // si visibilite de E, afficher
//  affichage 30
if ((intcap > 225 ) and (intcap < 375)) {pos30=2*(intcap)-548 ;if (pos30 >360) pos30=pos30-720; u8g.drawStr( pos30-12,35,"30");};  //
//  affichage 33 a droite du N
if (intcap < 15) {pos33=2*(intcap)+112 ; u8g.drawStr( pos33-12,35,"33");};  // cas du 33 affiche a droite du cadran
//  affichage 33 normal
if ((intcap > 255 ) and (intcap < 405)) {pos33=2*(intcap)-608 ;if (pos33 >360) pos33=pos33-720; u8g.drawStr( pos33-12,35,"33");};  //
//  affichage des dots

affdegres = (intcap %10); // cap modulo 10, devient une valeur de 0 a 9
 for (int bar = 0; bar < 7 ; bar++) {     //  8 barres maxi sur l affichage
  // u8g.drawBox(((bar)*20)+affdegres+1,1,((bar)*20)+affdegres+3,5); // affichage boite tous les 10 degres, soit 20 pixels
affpix = (affdegres*2)+(20*bar)  ;// valeur de decalage affichage
u8g.drawBox(affpix+4,42,3,20);  // gros trait
u8g.drawBox(affpix-5,50,1,14);  // petit trait  
 }
u8g.drawBox(65,1,1,64);  //  barre centrale 
}
/*      Serial.print(" intcap: ");  //  pour verif
      Serial.println(intcap);
      Serial.print(" posn: ");  //  pour verif
      Serial.println(posn);
      Serial.print("   ");  //  pour verif
   //   Serial.println(intcap);                       testage !
*/
     


