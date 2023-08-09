float kp = 13;
int pos;

float value;
int leftmax = 250;
int rightmax = 250;
int leftbase = 80;
int rightbase = 80;
float leftmotor, rightmotor, motorspeed;
float prevmotorspeed1 = 0.0;
float prevmotorspeed2 = 0.0;
float prevError = 0.0;

// motor driver pins
int IN1 = 5;
int IN2 = 6;
int IN3 = 10;
int IN4 = 11;

int bg;
int line;

int S1, S2, S3, S4, S5, S6, S7, S8, S9; // define sensor

void setup() {
  Serial.begin(9600);

  // motor driver output
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN4, OUTPUT);

  //sensor input
  pinMode(S1, INPUT);
  pinMode(S2, INPUT);
  pinMode(S3, INPUT);
  pinMode(S3, INPUT);
  pinMode(S5, INPUT);
  pinMode(S6, INPUT);
  pinMode(S7, INPUT);
  pinMode(S8, INPUT);
  //  pinMode(S9, INPUT); // 9th IR for arena inversion checking
}

void loop() {

  // sensor value read and stored
  S1 = digitalRead(2);
  S2 = digitalRead(3);
  S3 = digitalRead(4);
  S4 = digitalRead(12);
  S5 = digitalRead(13);
  S6 = digitalRead(7);
  S7 = digitalRead(8);
  S8 = digitalRead(9);
  //S9 = digitalRead(A4);

  if (S1 == 0 && S8 == 0 && S9 == 0) {       //For Arena Inversion
    bg = 0;
    line = 1;
  } else {
    bg = 1;    //white background
    line = 0;  // black line
  }

  // +ve error is to turn left
  // -ve error is to turn right
  if (S8 == line) {
    pos = -18;
    delay(13);
  } else if (S1 == line) {
    pos = 18;
    delay(13);
  } else if (S7 == line) {
    pos = -9;
    delay(100);
  } else if (S2 == line) {
    pos = 9;
    delay(100);
  } else if (S6 == line) {
    pos = -4;
  } else if (S3 == line) {
    pos = 4;
  } else if (S5 == line) {
    pos = -1;
  } else if (S4 == line) {
    pos = 1;
  } else {
    pos = 0;
  }

  speed_control();
}

void speed_control() {
  float error = pos * kp;

  motorspeed = error;

  Serial.print(error);
  Serial.print(" ");
  float leftmotor = leftbase + motorspeed;
  float rightmotor = rightbase - motorspeed;

  // to avoid motor speed PWM overwriting
  if (leftmotor > 250) {
    leftmotor = 250;
  } else if (leftmotor < -250) {
    leftmotor = -250;
  }
  if (rightmotor > 250) {
    rightmotor = 250;
  } else if (rightmotor < -250) {
    rightmotor = -250;
  }


  Move(leftmotor, rightmotor);
}

void Move(int leftmotor, int rightmotor) {
  // giving motor PWM values
  if (rightmotor > 0) {
    //rightmotor = map(rightmotor, 0, 255, 80, 255);
    analogWrite(IN2, rightmotor);
    analogWrite(IN1, 0);
  } else {
    //rightmotor = map(rightmotor, 0, -255, -80, -255);
    analogWrite(IN1, abs(rightmotor));
    analogWrite(IN2, 0);
  }
  if (leftmotor > 0) {
    //leftmotor = map(leftmotor, 0, 255, 80, 255);
    analogWrite(IN3, leftmotor);
    analogWrite(IN4, 0);
  } else {
    //leftmotor = map(leftmotor, 0, -255, -80, -255);
    analogWrite(IN4, abs(leftmotor));
    analogWrite(IN3, 0);
  }

  Serial.print(" right: ");
  Serial.print(leftmotor);
  Serial.print("     ");
  Serial.print("left: ");
  Serial.print(rightmotor);
  Serial.println("     ");
}
