/****************************************
  Скетч для "сумасшедшего миньона"
  Анастасия Варга, Артём Гуторов 2018
  Все права сохранены
  MIT License
*****************************************/

#include <Servo.h> /* подключаем библиотеку для использования сервопривода */

#define PIN_SERVO_RIGHT_HAND 8     /* определяем номер пина для привода правой руки миньона */
#define PIN_SERVO_LEFT_HAND  9     /* определяем номер пина для привода левой руки миньона */
#define PIN_TRIG             11    /* определяем номер пина для включения триггера датчика определения растояния */
#define PIN_ECHO             12    /* определяем номер пина для получения данных с эхолокатора */
#define TRIGGER_DISTANCE     10.f  /* определяем расстояние, на котором будет срабатывать датчик (см) */
#define HCSR04_BIND_DURATION 29.1f /* константное значение датчика определения растояния для времени получения сигнала */
#define PORT_SPEED_BOD       9600  /* скорость обмена данными с платой arduino бит/с (бод) */

/* создаем объекты для работы с сервоприводами */
Servo servoLeftHand;
Servo servoRightHand;

/* задаем количество повторений анимации движения рук */
const int ANIMATION_REPEAT_COUNT = 3;

/* задаем максимальный угол поворота рук миньона */
const int MINION_ROTATE_ANGLE_MAX = 150;

void setup()
{
  // инициируем подключение к плате
  Serial.begin(PORT_SPEED_BOD);

  // задаем режим работы пинов
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);

  // указываем, какой пин будет отвечать за какую руку миньона
  servoLeftHand.attach(PIN_SERVO_LEFT_HAND);
  servoRightHand.attach(PIN_SERVO_RIGHT_HAND);
}

void loop()
{
  // каждые четверть секунды проверяем, сработал ли датчик эхолокации
  delay(250);

  servoLeftHand.write(0);
  servoRightHand.write(0);
  
  if (isObstacleDetected()) {
    // если датчик сработал, то повторяем анимацию движения рук 3 раза
    for (int i = 0; i < ANIMATION_REPEAT_COUNT; ++i)
      CRAZY();
  }
}

bool isObstacleDetected()
{
  // получаем эхосигнал с датчика
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(5);
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);

  // определяет расстояние до объекта
  float distance = (pulseIn(PIN_ECHO, HIGH) / 2.f) / HCSR04_BIND_DURATION;

  return distance <= TRIGGER_DISTANCE;
}

void CRAZY()
{
  // запускаем анимацию рук
  for (int servoAngle = 0; servoAngle <= MINION_ROTATE_ANGLE_MAX; ++servoAngle)
  {
    // вращаем левой рукой
    servoLeftHand.write(servoAngle);
    // вращаем правой рукой в противоположном направлении
    servoRightHand.write(MINION_ROTATE_ANGLE_MAX - servoAngle);
    // выставляем небольшую задержку, чтобы руки двигались плавно
    delay(6);
  }

  // запускаем обратную анимацию, чтобы вернуть руки в исходное состояние
  for (int servoAngle = MINION_ROTATE_ANGLE_MAX; servoAngle >= 0; --servoAngle)
  {
    // вращаем левой рукой
    servoLeftHand.write(servoAngle);
    // вращаем правой рукой в противоположном направлении
    servoRightHand.write(MINION_ROTATE_ANGLE_MAX - servoAngle);
    // выставляем небольшую задержку, чтобы руки двигались плавно
    delay(6);
  }
}
