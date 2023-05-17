#include "LedControl.h" // 라이브러리 사용 선언
 
// LedControl객체의 인스턴스를 lc로 선언
// Din 핀을 11번, ClK핀을 13번 CS핀을 10번에 연결, 매트릭스는 4개를 사용 선언
LedControl lc = LedControl(3,4,5,4);
LedControl lc2 = LedControl(7,8,9,4);
//#include <SoftwareSerial.h>
#include <DFPlayer_Mini_Mp3.h>



#define tilt 2
#define joystick A0
#define joystickpush 6
#define pressure A1
#define pressure2 A2

SoftwareSerial esp(11, 10); //TX,RX
SoftwareSerial mySerial(12, 13); // RX, TX

byte data[15][8] = //14개의 데이터 선언 
{
             {
             B00000000,
             B00000000,
             B11111111,
             B11111111,
             B11111111,
             B11111111,
             B00000000,
             B00000000,
             },
             {
             B00000000,
             B00000000,
             B11111111,
             B11111111,
             B11111111,
             B11111111,
             B00000000,
             B00000000,
             },
             {
             B00000000,
             B00000000,
             B11111111,
             B11111111,
             B11111111,
             B11111111,
             B00000000,
             B00000000,
             },
             {
             B00000000,
             B00000000,
             B11111111,
             B11111111,
             B11111111,
             B11111111,
             B00000000,
             B00000000,
             }
};

byte nulll[15][8] = //14개의 데이터 선언 
{
             {
             B00000000,
             B00000000,
             B00000000,
             B11111111,
             B11111111,
             B00000000,
             B00000000,
             B00000000,
             },
};

byte fulll[15][8] = //14개의 데이터 선언 
{
             {
             B11111111,
             B11111111,
             B11111111,
             B11111111,
             B11111111,
             B11111111,
             B11111111,
             B11111111,
             },
};
 
// 초기화 세팅
void setup() 
{
  Serial.begin(9600);
  esp.begin(9600);
  mySerial.begin (9600);
  mp3_set_serial (mySerial);    //set softwareSerial for DFPlayer-mini mp3 module 
  delay(1);                     // delay 1ms to set volume
  mp3_set_volume (30);          // value 0~30

 pinMode(tilt, INPUT); // set digital 12 for tilt sensing
 pinMode(8, INPUT_PULLUP);


 for(int num=0; num<4; num++) // 매트릭스 0번부터 3번까지 세팅
  {
   lc.shutdown(num,false); // 0~3번까지 매트릭스 절전모드 해제
   lc.setIntensity(num,8); // 0~3번까지 매트릭스의 밝기 8로 선언, 0~15까지 가능
   lc.clearDisplay(num); // 0~3번까지 매트릭스 led를 초기화
   //lc2.shutdown(num,false); // 0~3번까지 매트릭스 절전모드 해제
   //lc2.setIntensity(num,8); // 0~3번까지 매트릭스의 밝기 8로 선언, 0~15까지 가능
   //lc2.clearDisplay(num); // 0~3번까지 매트릭스 led를 초기화
  }
}
 
 
void loop() 
{
    mp3_play(1);     //start play

  if(analogRead(pressure) < 900) {
    mp3_play (1);     //start play
  }
    
   Serial.println(String(analogRead(pressure)) + " " + String(digitalRead(tilt)));
   Serial.println(analogRead(joystick));
  delay(1000);
  int plzsendthis  = 0;
  
  if(analogRead(pressure) > 900) {
    plzsendthis = 100;
  }

  if(digitalRead(tilt) == 1) {
    plzsendthis += 10;
  }
  
   esp.write(plzsendthis);
   //esp.write(digitalRead(tilt));

   if(analogRead(joystick) < 300) {
    for (int sk = 0; sk < 3; sk++) { // 좌깜 횟수
      // 데이터 처리를 위해 buffer1 배열 선언
      // 모듈 4개 + 데이터처리 변수 1개 
      byte buffer1[4+1][8];
      
      // 버퍼값을 0으로 초기화 작업
      for (int i = 0; i < 4; i++)
      {
        for (int j = 0; j < 8; j++) 
        {
          buffer1[i][j] = B00000000;
        }
      }
     
      // 문자 시프트 작업
      for (int i = 0; i < 4; i++) // 전체 글자수 만큼 반복
      {
        // memcpy 함수를 사용해서 데이터를 버퍼로 옮김
        // memcpy(destination, source, size)
        // memcpy(목적지 주소값, 원본 주소값, 데이터크기)
        memcpy(&buffer1[4], &data[i], sizeof(data[i]));
        
        for (int j = 0; j < 8; j++) // 한 글자는 8열이므로, 글자당 8번 수행
        {
          for (int k = 0; k < 8; k++) // 한 글자는 8행이므로, 글자당 8번 수행
          {
            // 비트 시프트로 왼쪽으로 한칸씩 시프트 
            // 비트 시프트로 첫번째 열의 데이터가 오른쪽으로 7칸 시프트
            // 비트 OR 연산자(|)로 두 데이터를 합침
            buffer1[0][k] = (buffer1[0][k] << 1) | (buffer1[1][k] >> 7);
            buffer1[1][k] = (buffer1[1][k] << 1) | (buffer1[2][k] >> 7);
            buffer1[2][k] = (buffer1[2][k] << 1) | (buffer1[3][k] >> 7);
            buffer1[3][k] = (buffer1[3][k] << 1) | (buffer1[4][k] >> 7);
            buffer1[4][k] = (buffer1[4][k] << 1);
          }
     
          // 출력 
          for (int i = 0; i < 8; i++) 
          {
            // lc.setRow 함수는 행(Row) 기준으로 도트매트릭스를 제어
            // lc.setRow(matrix_number,Row,value)
            // lc.setRow(출력 매트릭스 번호, 행, 출력값)
            lc.setRow(3, i, buffer1[0][i]);
            lc.setRow(2, i, buffer1[1][i]);
            lc.setRow(1, i, buffer1[2][i]);
            lc.setRow(0, i, buffer1[3][i]);
            //lc2.setRow(3, i, buffer1[0][i]);
            //lc2.setRow(2, i, buffer1[1][i]);
            //lc2.setRow(1, i, buffer1[2][i]);
            //lc2.setRow(0, i, buffer1[3][i]);
          }
        }
      }
      delay(100);
    }
   }
   else if(analogRead(joystick) > 700) {
      // 데이터 처리를 위해 buffer1 배열 선언
      // 모듈 4개 + 데이터처리 변수 1개 
    for (int sk = 0; sk < 3; sk++) { // 우깜 횟수

      byte buffer1[4+1][8];
      
      // 버퍼값을 0으로 초기화 작업
      for (int i = 0; i < 4; i++)
      {
        for (int j = 0; j < 8; j++) 
        {
          buffer1[i][j] = B00000000;
        }
      }
     
      // 문자 시프트 작업
      for (int i = 0; i < 4; i++) // 전체 글자수 만큼 반복
      {
        // memcpy 함수를 사용해서 데이터를 버퍼로 옮김
        // memcpy(destination, source, size)
        // memcpy(목적지 주소값, 원본 주소값, 데이터크기)
        memcpy(&buffer1[4], &data[i], sizeof(data[i]));
        
        for (int j = 0; j < 8; j++) // 한 글자는 8열이므로, 글자당 8번 수행
        {
          for (int k = 0; k < 8; k++) // 한 글자는 8행이므로, 글자당 8번 수행
          {
            // 비트 시프트로 왼쪽으로 한칸씩 시프트 
            // 비트 시프트로 첫번째 열의 데이터가 오른쪽으로 7칸 시프트
            // 비트 OR 연산자(|)로 두 데이터를 합침
            buffer1[0][k] = (buffer1[0][k] >> 1) | (buffer1[1][k] << 7);
            buffer1[1][k] = (buffer1[1][k] >> 1) | (buffer1[2][k] << 7);
            buffer1[2][k] = (buffer1[2][k] >> 1) | (buffer1[3][k] << 7);
            buffer1[3][k] = (buffer1[3][k] >> 1) | (buffer1[4][k] << 7);
            buffer1[4][k] = (buffer1[4][k] >> 1);
          }
     
          // 출력 
          for (int i = 0; i < 8; i++) 
          {
            // lc.setRow 함수는 행(Row) 기준으로 도트매트릭스를 제어
            // lc.setRow(matrix_number,Row,value)
            // lc.setRow(출력 매트릭스 번호, 행, 출력값)
            lc.setRow(0, i, buffer1[0][i]);
            lc.setRow(1, i, buffer1[1][i]);
            lc.setRow(2, i, buffer1[2][i]);
            lc.setRow(3, i, buffer1[3][i]);
           // lc2.setRow(0, i, buffer1[0][i]);
            //lc2.setRow(1, i, buffer1[1][i]);
            //lc2.setRow(2, i, buffer1[2][i]);
            //lc2.setRow(3, i, buffer1[3][i]);
          }
        }
      }
      delay(10);
      }
   }
//   else if(digitalRead(joystickpush) == 0) {
//     for(int sk = 0; sk < 5; sk++) {
//      for (int i = 0; i < 8; i++) 
//          {
//            // lc.setRow 함수는 행(Row) 기준으로 도트매트릭스를 제어
//            // lc.setRow(matrix_number,Row,value)
//            // lc.setRow(출력 매트릭스 번호, 행, 출력값)
//            lc.setRow(0, i, fulll[0][i]);
//            lc.setRow(3, i, fulll[0][i]);
//           // lc2.setRow(0, i, fulll[0][i]);
//           // lc2.setRow(3, i, fulll[0][i]);
//          }
//          delay(300);
//          for (int i = 0; i < 8; i++) 
//          {
//          lc.setRow(0, i, nulll[0][i]);
//          lc.setRow(3, i, nulll[0][i]);
//         // lc2.setRow(0, i, nulll[0][i]);
//         // lc2.setRow(3, i, nulll[0][i]);
//          }
//          delay(300);
//     }
//   }
   else{
      for (int i = 0; i < 8; i++) 
          {
            // lc.setRow 함수는 행(Row) 기준으로 도트매트릭스를 제어
            // lc.setRow(matrix_number,Row,value)
            // lc.setRow(출력 매트릭스 번호, 행, 출력값)
            lc.setRow(0, i, nulll[0][i]);
            lc.setRow(1, i, nulll[0][i]);
            lc.setRow(2, i, nulll[0][i]);
            lc.setRow(3, i, nulll[0][i]);
            //lc2.setRow(0, i, nulll[0][i]);
           // lc2.setRow(1, i, nulll[0][i]);
           // lc2.setRow(2, i, nulll[0][i]);
          }
    }
}
