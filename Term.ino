// 서보 : 갈색 GND , 빨간색 5V , 주황색 9/10/11번핀
// 금속 : 파랑 GND, 갈색 5V, 검정색 2번핀
// LED : 짧은다리 GND, 긴다리 번호핀
// 조도 : 검정 GND, 빨강 5V, 파랑 A0

#include <Servo.h>

// 서보모터 객체 생성
Servo servoLight;   // 조도 센서 서보모터
Servo servoSound; // 소리 센서 서보모터
Servo servoMetal; // 금속 센서 서보모터

// 핀 설정
const int trigPin = 4;  // 초음파 센서 Trig 핀
const int echoPin = 5;  // 초음파 센서 Echo 핀
const int trigPin2 = 6;
const int echoPin2 = 7;
const int ldrPin = A0;  // 조도 센서 핀
const int metalPin = 2; // 금속 센서 핀
const int soundPin = A1; // 소리 센서 핀

const int ledPin3 = 3;
const int ledPin8 = 8;
const int ledPin12 = 12;
const int ledPin13 = 13;

// 서보모터 제어 핀
const int lightPin = 9;
const int servoSoundPin = 10;
const int servoMetalPin = 11;

// 임계값 설정
const int lightThreshold = 500;  // 조도 센서 임계값
const int soundThreshold = 100; // 소리 센서 임계값
const int distanceThreshold = 15; // 초음파 감지 거리 (cm)

// 물체가 분류되었을 때 LED를 켜기 위한 변수
const int distanceGlass = 20; // 초음파 감지 거리 (cm)
const int distancePlastic = 15; // 초음파 감지 거리 (cm)
const int distanceCan = 10; // 초음파 감지 거리 (cm)
const int distancePaper = 5; // 초음파 감지 거리 (cm)

long duration;
int distance;

long boxDuration;
int boxDistance;

int lightMax = 770;

void setup() {
  // 서보모터 초기화
  servoLight.attach(lightPin);
  servoSound.attach(servoSoundPin);
  servoMetal.attach(servoMetalPin);

  // 핀 모드 설정
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  pinMode(metalPin, INPUT);

  // 초기 서보모터 위치 설정 (중앙)
  servoLight.write(90);
  servoSound.write(90);
  servoMetal.write(90);

  Serial.begin(9600); // 디버깅용 시리얼 통신
}

void loop() {
  int lightValue = analogRead(lightPin);		// 조도센서값
  int metalDetected = digitalRead(metalPin);	// 금속센서값
  int soundLevel = analogRead(soundPin);	// 소리센서값

  // lightMax 값은 770으로 설정되어있음
  // 빛 투과됨 : 유리/플라스틱으로 분류 (조도센서 값이 480~770 사이)
  if (480 < lightValue && lightValue < lightMax) {
    
    Serial.println("빛 투과");
    Serial.println(lightValue);
    servoLight.write(180);  // 오른쪽으로 회전
    delay(1000);
    servoLight.write(90);   // 원위치
    delay(2000);

    Serial.println(soundLevel);

    // 소리값이 작으면 플라스틱(70~85), 크면 유리로 분류(85~220)
  if (70 < soundLevel && soundLevel < 85) {
    Serial.println("플라스틱");
    servoSound.write(180);  // 오른쪽으로 회전
    delay(1000);
    servoSound.write(90);   // 원위치
    delay(2000);
  } else if(85 <= soundLevel && soundLevel < 220) {
    Serial.println("유리");
    servoSound.write(0);  // 왼쪽으로 회전
    delay(1000);
    servoSound.write(90);   // 원위치
    delay(2000);
    }
    // 빛 투과 안됨 : 캔, 종이로 분류 (조도센서 값이 770~1023 사이)
  } else if(lightMax < lightValue && lightValue < 1023) {
    Serial.println("빛 투과 안됨");
    Serial.println(lightValue);
    delay(2000);
    servoLight.write(0);  // 왼쪽으로 회전
    delay(1000);
    servoLight.write(90);   // 원위치
    delay(2000);

    // 금속 감지되면 캔, 안되면 종이 분류 (LOW=감지됨, HIGH=감지안됨)
    if (metalDetected == LOW) {
        Serial.println("금속 감지됨");
        delay(2000);
        servoMetal.write(180);  // 오른쪽으로 회전
        delay(1000);
        servoMetal.write(90);   // 원위치
        delay(2000);
    } else if (metalDetected == HIGH){
        Serial.println("금속 감지 안됨");
        servoMetal.write(0);  // 왼쪽으로 회전
        delay(1000);
        servoMetal.write(90);   // 원위치
        delay(2000);
    }
  }
/*
   // 초음파센서 거리재기
  digitalWrite(trigPin2, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);
  boxDuration = pulseIn(echoPin2, HIGH);
  boxDistance = boxDuration * 0.034 / 2;
  
  
  // 거리에 따라 LED 활성화
  if (boxDistance >= 15 && boxDistance < 20) {
    digitalWrite(ledPin3, HIGH);
    digitalWrite(ledPin8, LOW);
    digitalWrite(ledPin12, LOW);
    digitalWrite(ledPin13, LOW);
    Serial.println("유리 분류됨");
    delay(3000);
  } else if (boxDistance >= 10 && boxDistance < 15) {
    digitalWrite(ledPin3, LOW);
    digitalWrite(ledPin8, HIGH);
    digitalWrite(ledPin12, LOW);
    digitalWrite(ledPin13, LOW);
    Serial.println("플라스틱 분류됨");
    delay(3000);
  } else if (boxDistance >= 5 && boxDistance < 10) {
    digitalWrite(ledPin3, LOW);
    digitalWrite(ledPin8, LOW);
    digitalWrite(ledPin12, HIGH;
    digitalWrite(ledPin13, LOW);
    Serial.println("캔 분류됨");
    delay(3000);
  } else if (boxDistance >= 0 && boxDistance < 5) {
    digitalWrite(ledPin3, LOW);
    digitalWrite(ledPin8, LOW);
    digitalWrite(ledPin12, LOW);
    digitalWrite(ledPin13, HIGH);
    Serial.println("종이 분류됨");
    delay(3000);
  }
  */

}