//See winsen-ZE03 datasheet
//URL : https://www.winsen-sensor.com/d/files/PDF/Gas%20Sensor%20Module/Industrial%20Application%20Gas%20Sensor%20Module/ZE03%20Electrochemical%20Module%20V2.4.pdf
byte command[] = {0xFF, 0x01, 0x78, 0x04, 0x00, 0x00, 0x00, 0x00, 0x83};//Q&A mode command
byte QnA[] = {0xFF, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};//To read the concentration value
byte readBuffer[9] = {};
float O3;

unsigned char FucCheckSum(unsigned char *i, unsigned char ln)//CheckSum
{
  unsigned char j, tempq = 0;
  i += 1;
  for (j = 0; j < (ln - 2); j++)
  {
    tempq += *i;
    i++;
  }
  tempq = (~tempq) + 1;
  return (tempq);
}

void setup() {
  Serial.begin(115200);//Start Serial
  Serial2.begin(9600, SERIAL_8N1, 15, 13);//Start Serial2, mode, RX, TX
  Serial2.write(command, sizeof(command));//QnA mode command send
  Serial2.readBytes(readBuffer, sizeof(readBuffer));//read data
  for (int i = 0; i < 8; i++) {//Receive mode
    Serial.print(readBuffer[i], HEX); Serial.print(" ");//print receive data
  }
  Serial.println(readBuffer[8], HEX);
  for(int i=0; i<300; i++){
    Serial.print("wait until 300"); Serial.print(" "); Serial.println(i);
    delay(1000);
  }
}

void loop() {
  if (Serial.available() > 0) {//If there is a value received in the serial
    String incomingText = Serial.readStringUntil('\n');//Receive before newline character
    if (incomingText == "QnA") {//String QnA set
      Serial2.write(QnA, sizeof(QnA));//Q&A send
      Serial2.readBytes(readBuffer, sizeof(readBuffer));//Q&A read
      byte Check = FucCheckSum(readBuffer, sizeof(readBuffer));//Checksum
      if (Check == readBuffer[8]) {//Checksum compare
        O3 = readBuffer[2] * 256 + readBuffer[3]*0.1;//gas concentration
        Serial.println(O3);
      }
    }
  }
}
