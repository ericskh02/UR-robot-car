#include <SoftwareSerial.h>

#define motorIn1 11 //L298N IN1 D8
#define motorIn2 10 //L298N IN2 D7
#define motorIn3 6 //L298N IN3 D6
#define motorIn4 5 //L298N IN4 D5

#define EchoPin A0 // 定義超音波信號接收腳位 A0
#define TrigPin A1 // 定義超音波信號發射腳位 A1
#define EchoPinL A2 // 定義超音波信號接收腳位 A2
#define TrigPinL A3 // 定義超音波信號發射腳位 A3
#define EchoPinR A4 // 定義超音波信號接收腳位 A4
#define TrigPinR A5 // 定義超音波信號發射腳位 A5

#define F_ang 90 //偵測前方時角度
#define R_ang 30 //偵測右方時角度
#define L_ang 150 //偵測左方時角度

int Fspeedd = 0; // 前速
int Rspeedd = 0; // 右速
int Lspeedd = 0; // 左速
int directionn = 0; // 前=8 後=2 左=4 右=6
int Fgo = 8; // 前進
int Rgo = 6; // 右轉
int Lgo = 4; // 左轉
int Bgo = 2; // 倒車

int power;
int power2;
int boom=0;
int angle;

const int DELAY = 300;

int old_ang = 90;

unsigned char Motion_val;
unsigned char Receive_val;
unsigned char FLAG;
unsigned char len;
unsigned char Buffer_temp[4];
unsigned long CAR_MODE;

void setup() {
 // put your setup code here, to run once:
 pinMode(motorIn1, OUTPUT);
 pinMode(motorIn2, OUTPUT);
 pinMode(motorIn3, OUTPUT);
 pinMode(motorIn4, OUTPUT);
 pinMode(EchoPin, INPUT); // 定義超音波輸入腳位
 pinMode(TrigPin, OUTPUT); // 定義超音波輸出腳位
 pinMode(EchoPinL, INPUT); // 定義超音波 L 輸入腳位
 pinMode(TrigPinL, OUTPUT); // 定義超音波 L 輸出腳位
 pinMode(EchoPinR, INPUT); // 定義超音波 R 輸入腳位
 pinMode(TrigPinR, OUTPUT); // 定義超音波 R 輸出腳位
 Serial.begin(9600);
 delay(250);
 myservo.detach();
 Motion_val = 0;
 FLAG = 0;
 len = 0;
 CAR_MODE = 0;
}

void loop() {
 Ultrasonic_CAR();
}
//////////////////////////////////////

void Ultrasonic_CAR()
{

 detection(); //測量角度 並且判斷要往哪一方向移動
 power = 100;
 power2 = power-15;
 if (directionn == Fgo) //假如 directionn(方向) = 8(前進)
 {
 forward(0); // 正常前進
 }
}

void detection() //測量 3 個角度
{
 boom++;
 ask_distance(F_ang); // 讀取前方距離
 angle = 0;
 if (Fspeedd < 8 || Rspeedd < 8 || Lspeedd < 8)
 {
 if(Lspeedd < 8)
 angle = 2;
 if(Rspeedd < 8)
 angle = 1;
 motorstop(0);
 if ((boom % 2)== 0){

 delay(250);
 turnL(15);
 if(angle == 2){
  forward(35);}
 else{
  forward(17);}
  turnR(15);
 }else{
  delay(250);
  turnR(13);
 if(angle == 1){
  forward(35);}
 else{
  forward(17);}
  turnL(15);
 }

 }
 else //加如前方不小於(大於)15 公分
 {
 directionn = Fgo; //向前走
 }
}

void ask_distance(int ang)
{
 digitalWrite(TrigPin, LOW); // 讓超聲波發射低電壓 2μs
 delayMicroseconds(2);
 digitalWrite(TrigPin, HIGH); // 讓超聲波發射高電壓 10μs，這裡至少是 10
μs
 delayMicroseconds(10);
 digitalWrite(TrigPin, LOW); // 維持超聲波發射低電壓
 float distance = pulseIn(EchoPin, HIGH); // 讀差相差時間
 digitalWrite(TrigPinL, LOW); // 讓超聲波發射低電壓 2μs
 delayMicroseconds(2);
 digitalWrite(TrigPinL, HIGH); // 讓超聲波發射高電壓 10μs，這裡至少是 10
μs
 delayMicroseconds(10);
 digitalWrite(TrigPinL, LOW); // 維持超聲波發射低電壓
 float distanceL = pulseIn(EchoPinL, HIGH); // 讀差相差時間
 digitalWrite(TrigPinR, LOW); // 讓超聲波發射低電壓 2μs
 delayMicroseconds(2);
 digitalWrite(TrigPinR, HIGH); // 讓超聲波發射高電壓 10μs，這裡至少是 10
μs
 delayMicroseconds(10);
 digitalWrite(TrigPinR, LOW); // 維持超聲波發射低電壓
 float distanceR = pulseIn(EchoPinR, HIGH); // 讀差相差時間
 distance = distance / 29 / 2;
 distanceL = distanceL / 29 / 2;
 distanceR = distanceR / 29 / 2;
 // distance= distance/5.8/10; // 將時間轉為距離距离（單位：公分）
 if (ang == F_ang)
 {
 Serial.print("L :"); //輸出距離（單位：公分）
 Serial.print(distanceL); //顯示距離
 Serial.print(" F :"); //輸出距離（單位：公分）
 Serial.print(distance); //顯示距離
 Serial.print(" R :"); //輸出距離（單位：公分）
 Serial.println(distanceR); //顯示距離
 Fspeedd = distance; // 將距離 讀入 Fspeedd(前速)
 Lspeedd = distanceL;
 Rspeedd = distanceR;
 }
}

void motorstop(int t)
{
 analogWrite(motorIn1, 0);
 analogWrite(motorIn2, 0);
 analogWrite(motorIn3, 0);
 analogWrite(motorIn4, 0);
 delay(t * 50);
}

void forward(int t)
{
 analogWrite(motorIn1, power2);
 analogWrite(motorIn2, 0);
 analogWrite(motorIn3, power);
 analogWrite(motorIn4, 0);
 delay(t * 50);
}

void backward(int t)
{
 analogWrite(motorIn1, 0);
 analogWrite(motorIn2, power2);
 analogWrite(motorIn3, 0);
 analogWrite(motorIn4, power);
 delay(t * 50);
}

void right(int t) //右轉(單輪)
{
 analogWrite(motorIn1, power2);
 analogWrite(motorIn2, 0);
 analogWrite(motorIn3, 0);
 analogWrite(motorIn4, 0);
 delay(t * 50);
}
void turnR(int t) //右轉(雙輪)
{
 analogWrite(motorIn1, power2);
 analogWrite(motorIn2, 0);
 analogWrite(motorIn3, 0);
 analogWrite(motorIn4, power);
 delay(t * 50);
}
void b_right(int t) //右輪後退
{
 analogWrite(motorIn1, 0);
 analogWrite(motorIn2, power2);
 analogWrite(motorIn3, 0);
 analogWrite(motorIn4, 0);
 delay(t * 50);
}

void left(int t) //左轉(單輪)
{
 analogWrite(motorIn1, 0);
 analogWrite(motorIn2, 0);
 analogWrite(motorIn3, power);
 analogWrite(motorIn4, 0);
 delay(t * 50);
}

void turnL(int t) //左轉(雙輪)
{
 analogWrite(motorIn1, 0);
 analogWrite(motorIn2, power2);
 analogWrite(motorIn3, power);
 analogWrite(motorIn4, 0);
 delay(t * 50);
}
void b_left(int t) //左輪後退
{
 analogWrite(motorIn1, 0);
 analogWrite(motorIn2, 0);
 analogWrite(motorIn3, 0);
 analogWrite(motorIn4, power);
 delay(t * 50);
}

void s0_right(int t)
{
 analogWrite(motorIn1, power2);
 analogWrite(motorIn2, 0);
 analogWrite(motorIn3, 0);
 analogWrite(motorIn4, 0);
 delay(t * 50);
 motorstop(3);
}
void s0_left(int t)
{
 analogWrite(motorIn1, 0);
 analogWrite(motorIn2, 0);
 analogWrite(motorIn3, power);
 analogWrite(motorIn4, 0);
 delay(t * 50);
 motorstop(3);
}
}












