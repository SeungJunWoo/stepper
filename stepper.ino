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
const int MOTORCNT = 1;
bool driverOn = false;

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

  // Start the serial communication at a baud rate of 9600
  Serial.begin(9600);
  Serial.println("Enter integers separated by spaces (e.g., '123 456 789'):");
}

void loop() {
  // Check if data is available to read
  if (Serial.available() > 0) {
    // Read the incoming string
    String incomingData = Serial.readString();
    
    // Print the received data
    Serial.print("Received: ");
    Serial.println(incomingData);

    if (checkFirstCharacter(incomingData, 'o')) {
      Serial.println("The first character is 'o'.");
      digitalWrite(EN_PIN, LOW);  // 모터 활성화
      driverOn = true;
      return;
    }
    if (checkFirstCharacter(incomingData, 'c')) {
      Serial.println("The first character is 'c'.");
      digitalWrite(EN_PIN, HIGH);  // 모터 활성화
      driverOn = false;
      return;
    }

    // Declare an array to hold the integers
    int numbers[MOTORCNT];
    
    // Process the string to extract integers and get the number of integers
    int numCount = processIncomingData(incomingData, numbers);
  }

  if(driverOn){
    // motor control
    if (currentPosition < targetPosition) {
      digitalWrite(DIR_PIN, HIGH); 
      moveStep(STEP_PIN)
      currentPosition++;
    } 
    else if (currentPosition > targetPosition) {
      digitalWrite(DIR_PIN, LOW); 
      moveStep(STEP_PIN)
      currentPosition--;
    } 
    else {
      return; // already at target
    }
  }

}

void moveStep(int MotorPIN) {
  digitalWrite(MotorPIN, HIGH);
  delayMicroseconds(100); // 스테핑 주기 설정
  digitalWrite(MotorPIN, LOW);
  delayMicroseconds(100);
}

void moveTo(long position) {
  targetPosition = position;
}

// Function to split the string and convert to integers
int processIncomingData(String data, int *numbers) {
  // Trim any leading/trailing spaces from the string
  data.trim();

  // Declare a temporary variable to hold each integer
  int number;
  
  // Declare a variable to store each substring (i.e., each number)
  String subString = "";

  int index = 0; // Index for the numbers array
  
  // Iterate over each character in the string
  for (int i = 0; i < data.length(); i++) {
    char c = data.charAt(i);

    // If we encounter a space or the end of the string, convert the substring to an integer
    if (c == ' ' || i == data.length() - 1) {
      // If it's the last character, include it in the substring
      if (i == data.length() - 1) {
        subString += c;
      }

      // Convert the substring to an integer
      number = subString.toInt();
      
      // Store the extracted integer in the array
      if (index < MOTORCNT) { // Avoid overflow
        numbers[index] = number;
        index++;
      }

      // Clear the substring for the next number
      subString = "";
    } 
    else {
      // If it's not a space, keep adding characters to the substring
      subString += c;
    }
  }

  // Return the number of integers extracted
  return index;
}

// Function to check if the first character of a string matches a specific character
bool checkFirstCharacter(String data, char specificChar) {
  // Trim any leading/trailing spaces from the string
  data.trim();

  // Check if the string is empty
  if (data.length() == 0) {
    return false;
  }

  // Get the first character and compare it
  char firstChar = data.charAt(0);
  return firstChar == specificChar;
}
