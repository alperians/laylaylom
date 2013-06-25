#include <LiquidCrystal.h>
#define warnlightH 50;
float warntempH= 27.9;
#define warntempL 21.0
#define sicaklikdizimax 10
#define lightdizimax 3
int tempdizistart=0;
float tempmax=-10;
float tempmin=99;
float sicaklik;
int sicaklikdeger;
float sicakliktoplam[sicaklikdizimax];
float sicakliktoplamdeger=0;
int sicakliktoplamgezgin=0;
float lighttoplam[lightdizimax];//okunan ışık değerlerinin  tutulduğu dizi
float lighttoplamdeger=0;//light dizisi işlemlerinde kullanılan toplama sonucu değişkeni
int lighttoplamgezgin=0;//light dizisinin gezmesini sağlayan değişken
float gerilim;
float aydinlik;//ortalama aydınlik değeri
float olcumaydinlik;//o an ölçülen aydinlik değeri
int sicaklikPin = 0;
int lightpin=5;
float deltatemp=0.5;
float oldlight=0;
int buzzerpin=13;
int buttonpin=12;
//int ledpin=13;
int redpin=9;
int bluepin=10;
int greenpin=11;
boolean alarm=false;
boolean buton=false;
boolean susturucu=false;
int zamanlama=400;
int buttonpushtime=0;//butona basıldığı sürece artar bırakıldıüında menü komutu işlenir
int buttonkomutonaylama=5;
/*
  LCD Ekran Kullanımı - 1
 Arduino ile LCD ekran arasındaki bağlantılar
 * LCD RS     ==> digital-8
 * LCD Enable ==> digital-7
 * LCD D4     ==> digital-3
 * LCD D5     ==> digital-4
 * LCD D6     ==> digital-5
 * LCD D7     ==> digital-6
 * LCD R/W    ==> toprak (GND)
 * 10K potansiyometre 0-5V bağlantı:
 * orta ucu LCD'nin V0 pinine(pin 3)
*/
LiquidCrystal lcd(8, 7, 3, 4, 5, 6);

void setup(void) {
 analogReference(INTERNAL);
  pinMode(buzzerpin,OUTPUT);
 // pinMode(ledpin,OUTPUT);
  pinMode(buttonpin,INPUT);
  lcd.begin(16, 2);
  Serial.begin(115200);
  
  Serial.println("Ready!");
   lcd.print("Bismillah!");
   delay(1000);
   randomSeed(analogRead(lightpin));

}
void loop(void) {
  delay(zamanlama);
  sicaklikk();
  if(!alarm && (sicaklik> (warntempH+deltatemp) || sicaklik<(warntempL-deltatemp)))
  {
    buzz(15);
    alarm=true;
  }
  if(alarm && (sicaklik<(warntempH-deltatemp) && sicaklik>(warntempL+deltatemp)))
  {
    alarm =false;    
  }

   // digitalWrite(ledpin,alarm);
    buton=false;
    if(digitalRead(buttonpin) && buttonpushtime>=0)//butontest
    {
      buttonpushtime++;
      buzz(75);
    }
    if(!digitalRead(buttonpin) && buttonpushtime>0)
    {
      buttonkomutonaylama=4;
      butondegerlendir();
      //      menudegerlendirme işlemleri çagrılacak
      //1 period için sadece alarm kontrolü onaysız
      //2 perion için max min silme olacak
      //3 period için max temp alarm set etme
      buttonpushtime=0;
    }

    if(alarm && !susturucu)
    {
      
    analogWrite(redpin,random(255));
    analogWrite(bluepin,random(255));
    analogWrite(greenpin,random(255));
    }
    else
    {
      
     analogWrite(redpin,255);
    analogWrite(bluepin,255);
    if(!susturucu){    analogWrite(greenpin,254);}
    else analogWrite(greenpin,255);
    
    }
  
  
  ///////////////
  //String test="151";
  
  //char test_as_char[test.length()+1];
  //test.toCharArray(test_as_char, test.length()+1);
  //Serial.print("testaschar");  Serial.println(test_as_char);
  //int i=int(test_as_char);
  //int j=atoi(test_as_char);
  
  // Serial.print("j"); Serial.println(j);
  //if(i==2280)
  //{
   // Serial.println("15");
    
  //}
  
  ////////////////////
  
}
void butondegerlendir()
{
  if(buttonpushtime>3)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Uzun bastin!! "); lcd.print(buttonpushtime);
    delay(1000);
     buttonpushtime=0;
     return;
  }
  switch(buttonpushtime)
  {
   case 1:
      susturucu=!susturucu;
      buzz(25);
      delay(15);
      buzz(25);
      delay(15);
      //buton=true;
      lcd.clear();
      lcd.setCursor(0,0);
      if(susturucu) lcd.print("Ses YOK");
      else lcd.print("Ses VAR");
      delay(1000);
  break;
  case 2:
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("silelim mi?onay");
  delay(1000);
  while(buttonkomutonaylama>0)
  {
    lcd.print(buttonkomutonaylama);
   
    if(digitalRead(buttonpin))
    {
      tempmax=sicaklik;
      tempmin=sicaklik;
      lcd.setCursor(0,1);
      lcd.print("Silindi");
      buzz(150);
      delay(1000);
      return;
      
    }
     delay(1000);
     buttonkomutonaylama--;
     lcd.setCursor(15,0);

  }
  lcd.setCursor(0,1);
  lcd.print("Silinme iptal");
  buzz(50);
  delay(150);
  buzz(150);
  delay(1000);
  
  break;
  case 3:
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Isi max alarm");
  int buttonkomutonaylamatemp=buttonkomutonaylama*10;
  
  while(buttonkomutonaylamatemp>0)
  {
    lcd.setCursor(0,1);
    lcd.print(warntempH);
    
    lcd.print("C   ");
    lcd.setCursor(7,1);
    lcd.print(buttonkomutonaylamatemp);
    if(digitalRead(buttonpin))
    {
      buzz(20);
      warntempH=warntempH+0.50;
      if(warntempH>35) warntempH=23;
      buttonkomutonaylamatemp=buttonkomutonaylama*10;
      
    }
    else{
      buttonkomutonaylamatemp--;
    }
    delay(zamanlama/2);
  }
  buzz(150);
  delay(100);
  buzz(150);
  break;
   
    
  }
 
}
void buzz(int timeval)
{
  digitalWrite(buzzerpin,HIGH);
 delay(timeval);
 digitalWrite(buzzerpin,LOW);
}
void sicaklikk(void)
{
  
  delay(150); //1
 // gerilim = analogRead(sicaklikPin);  
  //delay(50);//2
  
  olcumaydinlik=analogRead(lightpin);
  
  //delay(150);//3
  gerilim = analogRead(sicaklikPin);
  gerilim = analogRead(sicaklikPin);
  sicaklikdeger= gerilim;
  delay(150);//4
  sicaklik=gerilim/9.31;
  //aref 1.1 olduğu için kapatıldı  gerilim = (gerilim / 1023) * 5000;//  readVcc()/*4900*/; 
//  sicaklik = gerilim / 10.0;
  
  sicakliktoplam[sicakliktoplamgezgin]=sicaklik;
  sicakliktoplamdeger=0;
  for(int i =0;i<sicaklikdizimax;i++)
  {
    sicakliktoplamdeger+=sicakliktoplam[i];
  }
  if(tempdizistart<sicaklikdizimax)
  {
    tempdizistart++;//ilk kalkış anında bu kısım real sıcaklık ile kalkış yapılmasını sağlayacak
  sicaklik=sicakliktoplamdeger/tempdizistart;
  }
  else
  {
    sicaklik=sicakliktoplamdeger/sicaklikdizimax;
    if(sicaklik>tempmax) 
    {
      if(!susturucu){
      buzz(2);      delay(20);      buzz(2);}
      tempmax=sicaklik;
      Serial.println("");
      Serial.println("+++ Sicaklik UST +++");
      Serial.print("Yeni:");Serial.println(sicaklik);
      Serial.println("");
    }
    if(sicaklik<tempmin) 
    {
      if(!susturucu){
      buzz(2);}
      tempmin=sicaklik;
      Serial.println("");
      Serial.println("--- Sicaklik ALT ---");
      Serial.print("Yeni:");Serial.println(sicaklik);
      Serial.println("");
      
    }
  }
  sicakliktoplamgezgin++;
  if(sicakliktoplamgezgin>=sicaklikdizimax) sicakliktoplamgezgin=0;
  
  ////// ışık dizisi doldurucu
  lighttoplam[lighttoplamgezgin]=olcumaydinlik;
  lighttoplamdeger=0;
  for(int i =0;i<lightdizimax;i++)
  {
    lighttoplamdeger+=lighttoplam[i];
  }
  aydinlik=lighttoplamdeger/lightdizimax;//aydinlik değeri son 3(lightdizimax) değerin ortalaması olmuş oluyor
  lighttoplamgezgin++;
  if(lighttoplamgezgin>=lightdizimax) lighttoplamgezgin=0;
  ///////////////////////////////////////////
  
  
  
  Serial.println("");
  Serial.print("Anlik sicaklik olcum:");
  Serial.println(gerilim);
  Serial.print("Ortalamasal Sicaklik degeri:");       
  Serial.print(sicaklik);                
  Serial.println(" C");
  
  Serial.print("Anlik Aydinlik:");
  Serial.println(olcumaydinlik);
  Serial.print("Ortalamasal Aydinlik (x/1023):");
  Serial.print(int(aydinlik));
  Serial.println("");
  Serial.print("Kernel:");
  Serial.println(readTemp());
  Serial.print("Vcc:");
  //Serial.println(readVcc());
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("S:");
  lcd.print(sicaklik);
  
  lcd.print("C");
  if(alarm)
  {
    lcd.print("!");
  }
  else{
    lcd.print(" ");
  }
  lcd.print("I:");
  lcd.print(int(aydinlik));
  lcd.setCursor(15,0);
  if(susturucu){ lcd.print("$");}
  lcd.setCursor(0,1);
  lcd.print(sicaklikdeger);
  lcd.print("Y");
  lcd.print(tempmax);
  lcd.print("D");
  lcd.print(tempmin);
  //lcd.print(readVcc());
  
  
  
  
}
long readVcc() {
  long result;
  // Read 1.1V reference against AVcc
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Convert
  while (bit_is_set(ADCSRA,ADSC));
  result = ADCL;
  result |= ADCH<<8;
  result = 1125300L / result; // Back-calculate AVcc in mV
  return result;
}
long readTemp() { 
  // Read temperature sensor against 1.1V reference
  #if defined(__AVR_ATmega32U4__)
    ADMUX = _BV(REFS1) | _BV(REFS0) | _BV(MUX2) | _BV(MUX1) | _BV(MUX0);
    ADCSRB = _BV(MUX5); // the MUX5 bit is in the ADCSRB register
  #elif defined (__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
    ADMUX = _BV(REFS1) | _BV(MUX5) | _BV(MUX1);
  #elif defined (__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
    ADMUX = _BV(REFS1) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1) | _BV(MUX0);
  #else
    ADMUX = _BV(REFS1) | _BV(REFS0) | _BV(MUX3);
  #endif

  delay(2); // Wait for ADMUX setting to settle
  ADCSRA |= _BV(ADSC); // Start conversion
  while (bit_is_set(ADCSRA,ADSC)); // measuring

  uint8_t low = ADCL; // must read ADCL first - it then locks ADCH
  uint8_t high = ADCH; // unlocks both
  long result = (high << 8) | low; // combine the two

  return result;
}
