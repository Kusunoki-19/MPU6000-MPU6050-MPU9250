#include <Wire.h>
void setup() {
    // put your setup code here, to run once:
    
    Serial.begin(115200);
    Serial.println("Start");
    
    Wire.beginTransmission(0x68);
    Wire.write(0x1B);//速度のフルスケール設定があるレジスタ
    Wire.write(0b00001000);//4,3bitを01に指定することで500degree/secまで測れる
    Wire.endTransmission();
    
    Wire.beginTransmission(0x68);
    Wire.write(0x1C);//加速度のスケールレンジ設定があるレジスタ
    Wire.write(0b00000000);//4,3bitを00に指定することで+-2gまで測れる。gが重力加速度
    Wire.endTransmission();
    
    Wire.beginTransmission(0x68);
    Wire.write(0x6B);//クロック設定があるレジスタ
    Wire.write(0b00000000);//2,1,0bitを000に指定することで、20MHz振動に設定
    Wire.endTransmission();
    delay(1000);
}

uint8_t Data[14] = {0};
int16_t RealData[7] = {0};

void loop() {
    // put your main code here, to run repeatedly:
    
    Wire.beginTransmission(0x68);
    Wire.write(0x3B);
    Wire.endTransmission();
    Wire.requestFrom(0x68, 14);
    uint8_t index = 0;
    while(Wire.available()){
       //Serial.print(Wire.read(),BIN);
       //Serial.print("\t");
       Data[index++] = Wire.read();
    }
    RealData[0] = (int16_t)(Data[0]<<8 | Data[1]);
    RealData[1] = (int16_t)(Data[2]<<8 | Data[3]);
    RealData[2] = (int16_t)(Data[4]<<8 | Data[5]);
    RealData[3] = (int16_t)(Data[6]<<8 | Data[7]);
    RealData[4] = (int16_t)(Data[8]<<8 | Data[9]);
    RealData[5] = (int16_t)(Data[10]<<8 | Data[11]);
    RealData[6] = (int16_t)(Data[12]<<8 | Data[13]);
    Serial.print(" ACC_X:"); //X加速度
    Serial.print(RealData[0]);
    
    Serial.print("\t ACC_Y:"); //Y加速度
    Serial.print(RealData[1]);
    
    Serial.print("\t ACC_Z:"); //Z加速度
    Serial.print(RealData[2]);
    
    Serial.print("\tGYRO_X:"); //X軸角速度
    Serial.print(RealData[4]);
    
    Serial.print("\tGYRO_Y:"); //Y軸角速度
    Serial.print(RealData[5]);
    
    Serial.print("\tGYRO_Z:"); //Z軸角速度
    Serial.println(RealData[6]);
    delay(100);
    

}
