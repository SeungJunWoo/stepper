#include <TMCStepper.h>

// 핀 설정
#define EN_PIN         7   // Enable 핀
#define DIR_PIN        5   // Direction 핀
#define STEP_PIN       2   // Step 핀
#define SERIAL_PORT    Serial

// TMC2209 설정
#define R_SENSE        0.11f // Rsense value
TMC2209Stepper driver(&SERIAL_PORT, R_SENSE); 

// 목표 위치 설정
long targetPosition = 0;  
long currentPosition = 0;

void setup() {
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(EN_PIN, OUTPUT);

  // 드라이버 초기화
  SERIAL_PORT.begin(115200);
  driver.begin();
  driver.toff(5);
  driver.rms_current(600); // 모터의 RMS 전류 설정
  driver.microsteps(16);   // 마이크로스텝 설정
  driver.en_spreadCycle(true); // SpreadCycle 활성화
}

void loop() {
  if (currentPosition < targetPosition) {
    digitalWrite(DIR_PIN, HIGH); // 방향 설정
    currentPosition++;
  } else if (currentPosition > targetPosition) {
    digitalWrite(DIR_PIN, LOW); // 방향 설정
    currentPosition--;
  } else {
    return; // 이미 목표 위치에 도달한 경우
  }

  digitalWrite(STEP_PIN, HIGH);
  delayMicroseconds(100); // 스테핑 주기 설정
  digitalWrite(STEP_PIN, LOW);
  delayMicroseconds(100);
}

// 위치 설정 함수
void moveTo(long position) {
  targetPosition = position;
}
