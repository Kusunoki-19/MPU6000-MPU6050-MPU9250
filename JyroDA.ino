#include <Wire.h>
#include <math.h>

void setup() {    
    Serial.begin(115200);
    Serial.println("Start");
    
    Wire.beginTransmission(0x68);
    Wire.write(0x1B);//速度のフルスケール設定があるレジスタ
    Wire.write(0b00000000);//4,3bitを01に指定することで500degree/secまで測れる
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
    int static timeA = 0;
    int static timeB = 0;
    double static timeDelta = 0;
    Wire.beginTransmission(0x68);
    Wire.write(0x3B);
    Wire.endTransmission();
    Wire.requestFrom(0x68, 14);
    uint8_t index = 0;
    
    while(Wire.available()){
       Data[index++] = Wire.read();
    }
    //デジタル値の取得
    DData[0] = (int16_t)(Data[0]<<8 | Data[1]);//x加速度
    DData[1] = (int16_t)(Data[2]<<8 | Data[3]);//y加速度
    DData[2] = (int16_t)(Data[4]<<8 | Data[5]);//z加速度
    DData[3] = (int16_t)(Data[6]<<8 | Data[7]);
    DData[4] = (int16_t)(Data[8]<<8 | Data[9]);//x角速度
    DData[5] = (int16_t)(Data[10]<<8 | Data[11]);//y角速度
    DData[6] = (int16_t)(Data[12]<<8 | Data[13]);//z角速度
　　//単位はメートル毎秒毎秒[m/s^2]
    AData[0] = ((float)DData[0]/16383)*9.81;//x加速度
    AData[1] = ((float)DData[1]/16383)*9.81;//y加速度
    AData[2] = ((float)DData[2]/16383)*9.81;//z加速度
　　//単位はdegree毎秒[degree/s]
    AData[4] = (float)DData[4]/131.072;//x角速度
    AData[5] = (float)DData[5]/131.072;//y角速度
    AData[6] = (float)DData[6]/131.072;//z角速度
    
    Serial.print(" ACC[g]:  ");
    Serial.print(AData[0]);    Serial.print("\t");
    Serial.print(AData[1]);    Serial.print("\t");
    Serial.print(AData[2]);    Serial.print("\t");
    
    Serial.print("GYRO[deg/s]:  ");
    Serial.print(AData[4]);    Serial.print("\t");
    Serial.print(AData[5]);    Serial.print("\t");
    Serial.print(AData[6]);    Serial.print("\t");
    
    Serial.print("\n");
    delay(10);
}
