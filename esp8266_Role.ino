/////////////////////////////////////
///// ESP8266 | Arduino	| Röle	/////
/////	RX         TX	   x	/////
/////	TX         RX	   x	/////
/////	Vcc       +5V	   x	/////
/////	CH_EN     +5V	  Vcc	/////
/////	GND       GND	  GND	/////
/////    x         8      in	/////
/////////////////////////////////////
// ESP8266 için önerilen çalışma voltajı 3.3v olmasına rağmen verim alamadığım için 5v kullandım ve sorunsuz çalışıyor.

#define ag_ismi "Ağ adı"          //Kablosuz ağ adınızı tanımlayın
#define ag_sifresi "Ağ şifresi"   //Kablosuz ağ şifrenizi tanımlayın

bool durum = false;               //Durum kontrolu için değişken
const int relayPin = 8;           //Röle pini

void setup()
{
  Serial.begin(115200);           //Seri haberleşmeyi başlat              
  Serial.println("AT");           //Bağlantı kontrolu için AT komutu gönder
  pinMode(13,OUTPUT);             
  delay(3000);  
  if(Serial.find("OK")){         //Bağlantı sağlandı ise..
     Serial.println("AT+CWMODE=1"); //ESP8266 Çalışma modu
     delay(2000);
     Serial.println("AT+CIPSTA=\"192.168.1.28\""); //Yerel sabit ip tanımlaması
     delay(1000);
     String baglantiKur=String("AT+CWJAP=\"")+ag_ismi+"\",\""+ag_sifresi+"\""; //Bağlantı komutu
     Serial.println(baglantiKur);
     delay(5000);
 } 
   Serial.print("AT+CIPMUX=1\r\n"); //Birden çok bağlantıyı etkinleştirme
   delay(200);
   Serial.print("AT+CIPSERVER=1,80\r\n"); //80 portunda bir sunucu oluşturma
   delay(1000);
}
void loop(){
  if(Serial.available()>0){
    if(Serial.find("+IPD,")){
      String metin = "<head> Kontrol <br> </head>";
      if (durum == true) {
        metin += "<br><a href=\" ?pin=off\"><button type='button'>OFF</button></a>";
      }
      else {
        metin += "<br><a href=\" ?pin=on\"><button type='button'>ON</button></a>";
      }            
      String cipsend = "AT+CIPSEND=";
      cipsend +="0";
      cipsend +=",";
      cipsend += metin.length();
      cipsend += "\r\n";
      Serial.print(cipsend);
      delay(500);
      Serial.println(metin);
      isiklariYak();
      delay(2000);
      Serial.println("AT+CIPCLOSE=0");      
    }
  }
}

void isiklariYak(){
 String gelen ="";
 char serialdenokunan;
 while(Serial.available()>0){
 serialdenokunan = Serial.read();
 gelen +=serialdenokunan;
 
 }
 Serial.println(gelen);
 if((gelen.indexOf(":GET /?pin=on")>1)){  
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW);
  digitalWrite(13,HIGH);                  
  durum=true;
 }
  if((gelen.indexOf(":GET /?pin=off")>1)){ 
  pinMode(relayPin, INPUT);
  digitalWrite(13,LOW);                    
  durum=false;
 }
 }
