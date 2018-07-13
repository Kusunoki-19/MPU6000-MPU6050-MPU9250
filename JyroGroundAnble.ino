#include <Wire.h>
#include <math.h>

void setup() {    
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
int16_t DData[7] = {0};
float AData[7] = {0};
float grad[3] = {0};

void loop() {    
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
    
    DData[0] = (int16_t)(Data[0]<<8 | Data[1]);
    DData[1] = (int16_t)(Data[2]<<8 | Data[3]);
    DData[2] = (int16_t)(Data[4]<<8 | Data[5]);
    DData[3] = (int16_t)(Data[6]<<8 | Data[7]);
    DData[4] = (int16_t)(Data[8]<<8 | Data[9]);
    DData[5] = (int16_t)(Data[10]<<8 | Data[11]);
    DData[6] = (int16_t)(Data[12]<<8 | Data[13]);

    AData[0] = ((float)DData[0]/16383)*9.81;//単位は重力加速度[g]。例、1[g] = 9.81[m/s^2]
    AData[1] = ((float)DData[1]/16383)*9.81;
    AData[2] = ((float)DData[2]/16383)*9.81;
    
    grad[0] = atan2( (double) pow(pow(DData[1], 2)+pow(DData[2], 2) ,0.5) , DData[0] )*180/PI;
    grad[1] = atan2( (double) pow(pow(DData[2], 2)+pow(DData[0], 2) ,0.5) , DData[1] )*180/PI;
    grad[2] = atan2( (double) pow(pow(DData[0], 2)+pow(DData[1], 2) ,0.5) , DData[2] )*180/PI;
    /*
    Serial.print(" ACC[digital]:  ");
    Serial.print(DData[0]);    Serial.print("\t,");
    Serial.print(DData[1]);    Serial.print("\t,");
    Serial.print(DData[2]);    Serial.print("\t");
    */
    Serial.print(" ACC[g]:  ");
    Serial.print(AData[0]);    Serial.print("\t,");
    Serial.print(AData[1]);    Serial.print("\t,");
    Serial.print(AData[2]);    Serial.print("\t");
    /*
    Serial.print("\trot[deg]:  ");
    Serial.print(atan((float)DData[1]/DData[2])*180/PI);    Serial.print("\t,");
    Serial.print(atan((float)DData[2]/DData[3])*180/PI);    Serial.print("\t,");
    Serial.print(atan((float)DData[3]/DData[1])*180/PI);    Serial.print("\t");
    */
    Serial.print("\tgrad[deg]:  ");
    Serial.print(grad[0]);    Serial.print("\t,");
    Serial.print(grad[1]);    Serial.print("\t,");
    Serial.print(grad[2]);    Serial.print("\t");
    
    Serial.print("\tGYRO_X:  ");
    Serial.print(DData[4]);    Serial.print("\t,");
    Serial.print(DData[5]);    Serial.print("\t,");
    Serial.print(DData[6]);    Serial.print("\t");
    
    Serial.print("\n");
    delay(100);
    

}
