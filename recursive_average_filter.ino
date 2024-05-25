#include <NewPing.h>

#define SONAR_NUM 3      // 센서의 개수.
#define MAX_DISTANCE 180 // 핑을 보낼 최대 거리 (cm 단위).

#define Front 0
#define Left  1 
#define Right 2

#define TRIG1 12 // 초음파 센서 1번 Trig 핀 번호
#define ECHO1 13 // 초음파 센서 1번 Echo 핀 번호

#define TRIG2 16 // 초음파 센서 2번 Trig 핀 번호
#define ECHO2 17 // 초음파 센서 2번 Echo 핀 번호

#define TRIG3 14 // 초음파 센서 3번 Trig 핀 번호
#define ECHO3 15 // 초음파 센서 3번 Echo 핀 번호

NewPing sonar[SONAR_NUM] = {   // 센서 객체 배열.
NewPing(TRIG1, ECHO1, MAX_DISTANCE), // 각 센서의 트리거 핀, 에코 핀, 및 핑을 보낼 최대 거리.
NewPing(TRIG2, ECHO2, MAX_DISTANCE),
NewPing(TRIG3, ECHO3, MAX_DISTANCE)
};

float front_sonar = 0.0;
float left_sonar  = 0.0;
float right_sonar = 0.0;

// 재귀 평균 필터 변수
float alpha = 0.1; // 필터 계수, 0 < alpha < 1
float front_filtered = 0.0;
float left_filtered = 0.0;
float right_filtered = 0.0;

void setup() 
{
  pinMode(TRIG1, OUTPUT);
  pinMode(ECHO1, INPUT);

  pinMode(TRIG2, OUTPUT);
  pinMode(ECHO2, INPUT);

  pinMode(TRIG3, OUTPUT);
  pinMode(ECHO3, INPUT);
  
  Serial.begin(115200); // 통신 속도를 115200으로 정의
}

void loop() 
{
  // 초음파 센서 측정값 업데이트
  float front_raw = sonar[Front].ping_cm(); // 전방 센서 측정
  float left_raw  = sonar[Left].ping_cm();  // 좌측 센서 측정
  float right_raw = sonar[Right].ping_cm(); // 우측 센서 측정

  // 0.0 출력이 최대값이므로 보정 (측정값이 0일 때 기존 필터 값을 유지)
  if(front_raw != 0) front_sonar = front_raw; 
  if(left_raw  != 0) left_sonar = left_raw;
  if(right_raw != 0) right_sonar = right_raw;

  // 재귀 평균 필터 적용
  front_filtered = alpha * front_sonar + (1 - alpha) * front_filtered;
  left_filtered  = alpha * left_sonar  + (1 - alpha) * left_filtered;
  right_filtered = alpha * right_sonar + (1 - alpha) * right_filtered;

  Serial.print("F(recursive average filter): "); Serial.print(front_filtered); Serial.print(" cm, ");
  Serial.print("F(no filter): "); Serial.print(front_sonar); Serial.print(" cm | ");

  Serial.print("L(recursive average filter): "); Serial.print(left_filtered); Serial.print(" cm, ");
  Serial.print("L(no filter): "); Serial.print(left_sonar); Serial.print(" cm | ");

  Serial.print("R(recursive average filter): "); Serial.print(right_filtered); Serial.print(" cm, ");
  Serial.print("R(no filter): "); Serial.print(right_sonar); Serial.println(" cm");

  delay(100); // 데이터 갱신 주기 조절
}
