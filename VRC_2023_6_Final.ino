#include <Adafruit_PWMServoDriver.h>
#include <Wire.h>
#include <PS2X_lib.h>

#define ACC

#define PS2_DAT 12 // MISO
#define PS2_CMD 13 // MOSI
#define PS2_SEL 15 // SS
#define PS2_CLK 14 // SLK

#define SEN_1_PIN 39
#define SEN_2_PIN 36
#define SEN_3_PIN 2
#define SEN_4_PIN 32

int i;
int gt1, gt2, gt3, gt4;

#define DC_MOTOR_1_CHAN_1 10 //motor di chuyen 1 chan 1
#define DC_MOTOR_1_CHAN_2 11 //motor di chuyen 1 chan 2
#define DC_MOTOR_2_CHAN_1 8 //motor di chuyen 2 chan 1
#define DC_MOTOR_2_CHAN_2 9 //motor di chuyen 2 chan 2

#define DC_MOTOR_3_CHAN_1 12 //motor lay bong chan 1
#define DC_MOTOR_3_CHAN_2 13 //motor lay bong chan 2

#define DC_MOTOR_4_CHAN_1 14 //motor ban bong chan 1
#define DC_MOTOR_4_CHAN_2 15 //motor ban bong chan 2

#define Servo_180_1 4 //servo lay bong 
#define Servo_180_2 5 //servo cua xa
#define Servo_360_1 6 //servo lua bong 1
#define Servo_360_2 7 //servo lua bong 2
 
// define toc do mottor
#define MIN_SERVO_180_SPEED 204 //goc quay min servo
#define MAX_SERVO_180_SPEED 410 //goc quay max servo
#define MIN_SERVO_360_SPEED 220 //max toc theo chieu am
#define MAX_SERVO_360_SPEED 500 //max toc theo chieu duong
#define SERVO_FREQ 50 //set tan so servo
#define ENA
#define ENB

#define pressures false
#define rumble false

//khoi tao class thu vien
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
PS2X ps2x;

void setup() {
  pinMode(SEN_1_PIN,INPUT); //set dien ap thap
  pinMode(SEN_2_PIN,INPUT); //set dien ap thap
  pinMode(SEN_3_PIN,INPUT); //set dien ap thap
  pinMode(SEN_4_PIN,INPUT); //set dien ap thap
  
  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(SERVO_FREQ); // vì tần số phát xung PWM là SERVO_FREQ = 50 nên mỗi chu kỳ xung PWM là 20ms (1/50s)

  Wire.setClock(400000);
  Serial.begin(115200);
  Serial.print("Ket noi voi tay cam PS2:");
  
  delay(300) ;
 int error = -1;

    while(error != 0){
        error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, pressures, rumble);
        delay(10) ;
        switch (error)
        {
            case 0:
                Serial.println(" Ket noi tay cam PS2 thanh cong");
                break;
            case 1:
                Serial.println(" LOI: Khong tim thay tay cam, hay kiem tra day ket noi vơi tay cam ");
                break;
            case 2:
                Serial.println(" LOI: khong gui duoc lenh");
                break;
            case 3:
                Serial.println(" LOI: Khong vao duoc Pressures mode ");
                break;
        }
    }

    /*
    Giải thích code kết nối tay cầm: khởi tạo 1 biến kiểm tra lỗi error, gán giá trị -1
    error lấy giá trị từ hàm ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, pressures, rumble)
    hàm này là hàm cấu hình gamepad để có thể sử dụng với các chức năng khác của thư viện, chẳng hạn như đọc giá trị từ các nút hoặc cần điều khiển.
    Hàm này giúp arduino thiết lập và kết nối với gamepad
    Giá trị trả về:
    0: cấu hình thành công.
    1: không tìm thấy gamepad.
    2: không thể đọc được các dữ liệu từ gamepad.
    Vòng lặp while: Kiểm tra giá trị của error, nếu error khác 0, tức là chưa kết nối được, thì tiếp tục kết nối lại cho đến khi kết nối được, delay 10ms.
    Nhược điểm: Nếu tay cầm không kết nối được thì vòng lặp sẽ lặp mãi mãi
    Khi error = 0 thì thoát vòng lặp, sau đó in ra "Ket noi tay cam PS2 thanh cong", khi error khác 0 sẽ in ra lỗi tương ứng
    */
}
    
void laybong() {
        Serial.println("TRIANGLE PRESSED");
        pwm.setPWM(12, 0, 3072 );
        pwm.setPWM(13, 0, 0);
    }
    
    /*
    Giải thích hàm lấy bóng: khi hàm lấy bóng được gọi, in ra nút được bấm là nút triangle
    Sau đó, motor số 3 được khởi động.
    */

    void banbong() {
        pwm.setPWM(14, 0, 3072);
        pwm.setPWM(15, 0, 0);
        Serial.println("PAD_L2 is being held");
    }

    /*
    Giải thích hàm bắn bóng: khi hàm bắn bóng được gọi, in ra nút được bấm là nút PAD_L2
    Sau đó, motor số 4 được khởi động.
    */

    void daochieubanbong() {
        Serial.println("PAD R2 is being held");
        pwm.setPWM(14, 0, 0);//motor ban bong dao chieu
        pwm.setPWM(15, 0, 1648);//motor ban bong dao chieu
    }

    /*
    Giải thích hàm đảo chiều bắn bóng: khi hàm được gọi, in ra nút được bấm là nút PAD_R2
    Sau đó, motor số 4 được khởi động.
    */

    void tamdungbanbong() {
        pwm.setPWM(14, 0, 0);
        pwm.setPWM(15, 0, 0);
        pwm.setPWM(6, 0, 0);
        pwm.setPWM(7, 0, 0);
    }

    /*
    Giải thích hàm tạm dừng bắn bóng: Khi hàm được gọi, dừng motor 4 và 2 servo 360
    */

    void servoluabong() {
        pwm.setPWM(Servo_360_1, 0, MAX_SERVO_360_SPEED);
        pwm.setPWM(Servo_360_2, 0, MAX_SERVO_360_SPEED);
        Serial.println("pad up Pressed");
    }

    /*
    Giải thích hàm lùa bóng: Khi hàm được gọi, khởi động 2 servo 360 với tốc độ cao nhất theo chiều dương
    */

    void servodaochieu() {
        pwm.setPWM(Servo_360_1, 0, MIN_SERVO_360_SPEED);
        pwm.setPWM(Servo_360_2, 0, MIN_SERVO_360_SPEED);
        Serial.println("pad down Pressed");
    }

    /*
    Giải thích hàm servo đảo chiều: Khi hàm được gọi, khởi động 2 servo 360 với tốc độ cao nhất theo chiều âm
    */

    void tuhuy() {
        pwm.setPWM(8, 0, 0);
        pwm.setPWM(9, 0, 0);
        pwm.setPWM(10, 0, 0);
        pwm.setPWM(11, 0, 0);
        pwm.setPWM(12, 0, 0);
        pwm.setPWM(13, 0, 0);
        pwm.setPWM(14, 0, 0);
        pwm.setPWM(15, 0, 0);
        pwm.setPWM(5, 0, 0);
        pwm.setPWM(6, 0, 0);
        pwm.setPWM(7, 0, 0);
        pwm.setPWM(4, 0, 0);
    }

    /*
    Giải thích hàm tự hủy: dừng tất cả các servo và motor
    */

    void tuhuylaybong () {
        pwm.setPWM(12, 0, 0);
        pwm.setPWM(13, 0, 0);
    }

    /*
    Giả thích hàm tự hủy lấy bóng: Khi hàm được gọi, dừng motor số 3
    */

void setMotor(int motor, int value){
  int pinA, pinB;
    switch(motor){
        // khi motor 1 được gọi, đặt chỉ số chân cho motor 1
      case 1: pinA = 10;
              pinB = 11;
      break;
        // khi motor 2 được gọi, đặt chỉ số chân cho motor 2
      case 2: 
              pinA = 8;
              pinB = 9;
      break;
    }
  
  if(value >= 0 ){
    // khi tốc độ hiện tại leftCurrent/rightCurrent không âm, đặt motor quay theo chiều dương với tốc độ (value/4096)% so với tốc độ max
    pwm.setPin(pinA, value, false) ;
    pwm.setPin(pinB, 0, false) ;
  }
  else{
    // khi tốc độ hiện tại leftCurrent/rightCurrent là ám, đặt motor quay theo chiều ám với tốc độ (value/4096)% so với tốc độ max
    pwm.setPin(pinA, 0, false) ;
    pwm.setPin(pinB, -value, false) ;
  }
}

// Lợi ích khi sử dụng hàm setMotor là khiến cho các thao tác nhanh gọn hơn, code dễ nhìn và khoa học hơn, thay vì viết 4 dòng code thì chỉ cần viết 2 dòng
// Nhược điểm: Code chạy chậm hơn vì phải gọi hàm và thực hiện các thao tác so sánh, gán giá trị,... liên tục sau mỗi lần lặp loop, lãng phí bộ nhớ vì các biến mới liên tục được khai báo, các biến cũ không được giải phóng khỏi bộ nhớ
// Lưu ý: Tốc độ code chạy chậm vì phải lặp lại nhiều thao tác sau mỗi lần loop, chứ không phải vì lãng phí bộ nhớ.

#ifdef ACC
int period_pwm = 100 ; // biến có tác dụng kiểm tra thời gian là 100ms
long last_pwm = 0 ; // thời điểm của lần xét trước
int leftMotor = 2 ; // motor trái
int rightMotor = 1 ; // motor phải
int leftValue = 0 ; // tốc độ mong muốn của motor trái
int leftCurrent = 0; // tốc độ hiện tại của motor phải
int rightCurrent = 0 ; // tốc độ mong muốn của motor phải
int rightValue = 0 ; // tốc độ hiện tại của motor phải
int acc_pos = 300 ; // giá trị biến thiên dương cho phép
int acc_neg = 600 ; // giá trị biến thiên âm cho phép
#endif

void loop() {
   delay(10);
   #ifdef ACC
   long now = millis(); // hàm millis() lấy thời gian hiện tại
    if(now -last_pwm >= period_pwm){
    // khi thời gian hiện tại vượt quá 100ms so với thời gian xét gần nhất
      last_pwm = now ; // gán thời gian xét gần nhất là thời gian hiện tại
      int leftDiff = leftValue - leftCurrent ; // biến leftDiff là biến lưu sự chênh lệch giữa tốc độ hiện tại và tốc độ mong muốn
      int rightDiff = rightValue - rightCurrent ; // biến rightDiff là biến lưu sự chênh lệch giữa tốc độ hiện tại và tốc độ mong muốn

      int acc_inc, acc_dec ;
      if(leftCurrent > 0){
        // nếu tốc độ hiện tại lớn hơn 0
          acc_inc = acc_pos ; // acc_inc sẽ là giá trị cộng thêm
          acc_dec = acc_neg ; // acc_dec sẽ là giá trị trừ đi
      }else{
        // ngược lại, khi tốc độ hiện tại nhỏ hơn 0 thì phải trừ đi số 0 mới tăng lên được, phải cộng với số âm mới giảm đi được
          acc_inc = acc_neg ;
          acc_dec = acc_pos ;
      }

      if(leftDiff > acc_inc){
        // khi tốc độ hiện tại kém tốc độ mong muốn quá giá trị biến thiên dương cho phép, tốc độ hiện tại tăng lên 1 lượng bằng acc_inc
        leftCurrent += acc_inc ;
      }
      else if(leftDiff < -acc_dec){
        // Khi tốc độ hiện tại lớn hơn tốc độ mong muốn quá giá trị biến thiên âm cho phép, tốc độ hiện tại sẽ giảm đi 1 lượng bằng acc_dec
        leftCurrent -= acc_dec;
      }
      // nếu tốc độ hiện tại không vượt quá ngưỡng cho phép đối với tốc độ mong muốn thì đặt tốc độ mong muốn mới là tốc độ hiện tại đang xét
      else leftCurrent = leftValue ;
    // làm tương tự với motor phải
      if(rightDiff > acc_inc){
        rightCurrent += acc_inc ;
      }
      else if(rightDiff < -acc_dec){
        rightCurrent -= acc_dec ;
      }
      else rightCurrent = rightValue ;
      
      // sau khi tính toán xong tốc độ hiện tại của 2 motor, đặt tốc độ cho 2 motor qua hàm setMotor ()
      setMotor(leftMotor, leftCurrent) ;
      setMotor(rightMotor, rightCurrent) ;
    }
    #endif

    ps2x.read_gamepad(false, false); // gọi hàm để đọc tay điều khiển

  int speed = 4095; // giá trị max, 4095 là độ rộng max
  int nJoyX = 128 - ps2x.Analog(PSS_LX); // đọc giá trị trục X x-joystick
  int nJoyY = 128 - ps2x.Analog(PSS_LY); // đọc giá trị trục Y y-joystick
  int nMotMixL;                          // Motor (left) mixed output - giá trị joystick cho động cơ trái
  int nMotMixR;                          // Motor (right) mixed output - giá trị joystick cho động cơ phải
  
  nJoyY /= 1.5; // giảm độ nhạy trục Y
  nJoyX /= 4; // Giảm độ nhạy trục X
 bool temp = (nJoyY * nJoyX > 0);
 // temp kiểm tra hướng của joystick, nếu joystick ở góc phần tư thứ 1, 3 thì temp là true, còn lại là false
  if (nJoyY) // Forward or reverse
  // nJoyY khác 0, tức là đi thẳng hoặc đi lùi, nMotMixL và nMotMixR đồng bộ tốc độ bằng nJoyY
  {
    nMotMixL = nJoyY; 
    nMotMixR = nJoyY;
    
  }
  else // Turning
  // khi không đi thẳng hoặc đi lùi, nJoyY bằng 0
  /*
  nJoyY * temp = nJoyY * !temp = 0
  động cơ trái có giá trị là -nJoyX, động cơ phải có giá trị là nJoyX
  tức là bot sẽ quay tại chỗ vì 2 motor quay ngược chiều nhau
  */
  {
    nMotMixL = -nJoyX + (nJoyY * temp);
    nMotMixR = nJoyX + (nJoyY * !temp);
  }

  int c1 = 0, c2 = 0, c3 = 0, c4 = 0;
  // khai báo các biến lưu giá trị từ joystick
 /* code motor di chuyen
 */
 // di chuyen
  #ifdef ACC // Nếu ACC được định nghĩa
  leftValue = map(nMotMixL, -127, 127, -speed, speed) ;
  rightValue = map(nMotMixR, -127, 127, -speed, speed) ;
  /*
  Hàm map() có tác dụng ánh xạ giá trị của nMotMixL, nMotMixR từ (-127,127) sang (-speed,speed) tức là (-4095,4095)
  Giải thích ánh xạ: ánh xạ là hành động biến 1 hoặc nhiều giá trị từ tập giá trị X đủ điều kiện tồn tại giá trị (có các rằng buộc X),
  sang tập giá trị Y đủ điều kiện tồn tại giá trị (có các rằng buộc Y).
  Trong trường hợp này: {x thuộc Z | -127 <= x <= 127} ánh xạ sang {y thuộc Z | -4095 <= y <= 4095}
  Công thức ánh xạ: Nếu coi 127 và 4095 là các giá trị cho ra công suất max, ta có hệ số H = 4095/127.
  Với mỗi giá trị joystick n thuộc (-127,127) ta chỉ cần nhân n với H là ra được giá trị của n trong miền giá trị của {y thuộc Z | -4095 <= y <= 4095}
  */
 /*
 Vì sao cần dùng hàm map(): leftValue, rightValue là tốc độ mong muốn, mà điều chỉnh tốc độ phải dựa vào xung PWM có giá trị thuộc (-4095,4095)
 Nên khi muốn áp dụng giá trị joystick (nMotMixL, nMotMixR) bằng tốc độ chạy, ta dùng hàm map() để ánh xạ giá trị joystick sang PWM
 */
  #else // nếu không được định nghĩa
  if (nMotMixR >= 0)
  // nếu giá trị joystick của motor phải không âm
  {
    c3 = nMotMixR; // gán c3 với giá trị joystick
    c3 = map(c3, 0, 127, 0, speed); // ánh xạ sang xung PWM
    pwm.setPin(10, c3, false); // thiết lập tốc độ cho động cơ
    pwm.setPin(11, 0, false);
  }
// tương tự với các trường hợp còn lại
 else if (nMotMixR < 0)
  {
    c4 = abs(nMotMixR);
    c4 = map(c4, 0, 127, 0, speed);
    pwm.setPin(10, 0, false);
    pwm.setPin(11, c4, false);
  }

  if (nMotMixL >= 0)
  {
    c1 = nMotMixL;
    c1 = map(c1, 0, 127, 0, speed);
    pwm.setPin(8, c1, false);
    pwm.setPin(9, 0, false);
    }
  
  else if (nMotMixL < 0)
  {
    c2 = abs(nMotMixL);
    c2 = map(c2, 0, 127, 0, speed);
    pwm.setPin(8, 0, false);
    pwm.setPin(9, c2, false);
  }
  #endif

//dieu khien motor

 if(ps2x.ButtonPressed(PSB_TRIANGLE)) {
  // Khi nút triangle được ấn, cụ thể là click 1 lần (press)
   laybong(); // chạy hàm lấy bóng
 }
 
 if(ps2x.ButtonPressed(PSB_L2)) {
  // Khi nút L2 được ấn, cụ thể là click 1 lần (press)
   banbong(); // chạy hàm bắn bóng
   servoluabong(); // chạy hàm servo để lùa bóng
 }

 if(ps2x.ButtonPressed(PSB_R2)) {
  // Khi nút R2 là được ấn, cụ thể là click 1 lần (press)
   daochieubanbong(); // chạy hàm đảo chiều bắn bóng
   servodaochieu(); 
 }

 if(ps2x.ButtonPressed(PSB_CIRCLE)) {
  // Khi nút circle được ấn, cụ thể là click 1 lần (press)
   tamdungbanbong(); // dừng bắn bóng
 }

 if(ps2x.ButtonPressed(PSB_SQUARE)) {
  // Khi nút square được ấn, cụ thể là click 1 lần (press)
   tuhuylaybong(); // hủy lấy bóng

 }
 
//servo cua xa 180
   //dong cua xa
  if(ps2x.ButtonPressed(PSB_PAD_UP)) {
    // Khi nút up được ấn, cụ thể là click 1 lần (press)
    // quay servo 180 theo chiều âm với tốc độ max
    pwm.setPWM(Servo_180_1, 0, MIN_SERVO_180_SPEED);
    pwm.setPWM(Servo_180_2, 0, MIN_SERVO_180_SPEED);
    Serial.println("pad right pressed");
  }
   //mo cua xa
  if(ps2x.ButtonPressed(PSB_PAD_DOWN)) {
    // Khi nút down được ấn, cụ thể là click 1 lần (press)
    // quay servo 180 theo chiều dương với tốc độ max
    pwm.setPWM(Servo_180_1, 0, MAX_SERVO_180_SPEED);
    pwm.setPWM(Servo_180_2, 0, MAX_SERVO_180_SPEED);
    Serial.println("pad left Pressed");
  }

  //tu huy
 if(ps2x.ButtonPressed(PSB_CROSS)) {
    // Khi nút cross được ấn, cụ thể là click 1 lần (press)
    // dừng mọi hoạt động của robot
   tuhuy();
 }

}

/*
Giải thích về ACC:
- ACC có thể hiểu đơn giản là 1 đoạn code điều khiển động cơ, khiến cho động cơ tăng tốc từ từ (gia tốc).
- ACC chia đoạn code di chuyển thành 2 code tồn tại song song (gần giống cách hoạt động của if-else).
- Khi xe bắt đầu từ trạng thái dừng sang di chuyển, đoạn code ACC (ifdef ACC) sẽ được thực thi, các tham số như leftValue, rightValue,...sẽ được khai báo
sau đó leftValue, rightValue được lấy giá trị từ joystick và ánh xạ sang PWM, tiếp theo sẽ tính toán lại leftCurrent, rightCurrent sao cho nó ở trong khoảng giá trị phù hợp
Khi đã tính xong leftCurrent, rightCurrent, thiết lập động cơ cho motor di chuyển, sau đó kết thúc đoạn code ACC
Theo như code, khi dừng xe và không có bất kỳ hoạt động nào thêm về các motor, servo khác, sẽ kết thúc 1 vòng loop
Khi xe bắt đầu chạy lại hoặc xe không chạy nhưng thay đổi trạng thái của các motor, servo khác thì 1 vòng loop mới bắt đầu. 
*/