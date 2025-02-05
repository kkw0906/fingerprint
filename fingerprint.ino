#include <Adafruit_Fingerprint.h>

SoftwareSerial mySerial(2, 3);

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
char *s1[] = {
  "고관우","이순신"
};



void setup()
{
  Serial.begin(9600);
  Serial.println("CLEARDATA");
  Serial.println("LABEL, name");
  while (!Serial); 
  delay(100);
  Serial.println("\n\n지문 인식 프로그램");

  
  finger.begin(57600);
  delay(5);
  if (finger.verifyPassword()) {
    Serial.println("지문센서를 찾았습니다!");
  } else {
    Serial.println("센서를 찾지 못했습니다 다시 연결 해주세요");
    while (1) { delay(1); }
  }

  //지문센서 정보 출력
  // Serial.println(F("Reading sensor parameters"));
  // finger.getParameters();
  // Serial.print(F("Status: 0x")); Serial.println(finger.status_reg, HEX);
  // Serial.print(F("Sys ID: 0x")); Serial.println(finger.system_id, HEX);
  // Serial.print(F("Capacity: ")); Serial.println(finger.capacity);
  // Serial.print(F("Security level: ")); Serial.println(finger.security_level);
  // Serial.print(F("Device address: ")); Serial.println(finger.device_addr, HEX);
  // Serial.print(F("Packet len: ")); Serial.println(finger.packet_len);
  // Serial.print(F("Baud rate: ")); Serial.println(finger.baud_rate);

  //현재 저장된 지문의 개수를 불러오는 함수
  finger.getTemplateCount();
  //등록된 지문이 0개면 지문 등록을 해오라고 말해주는 함수
  if (finger.templateCount == 0) {
    Serial.print("등록된 지문이 없습니다. 지문 등록 프로그램을 이용하여 지문을 등록해주세요");
  }
  //등록된 지문 개수를 표현
  else {
    Serial.println("등록된 지문을 확인 하고있습니다...");
      Serial.print(finger.templateCount);Serial.println(" 개의 등록된 지문을 찾았습니다!");
  }
}


void loop()                     
{
  getFingerprintID();
  delay(1000);            
}

//현재 센서의 지문을 인식하는 부분
uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();//finger.getImage() -> 센서에서 손가락의 이미지를 촬영하도록 하는 부분 손가락 이미지를 촬영하고 그 정보를 switch문으로 반환
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("이미지 촬영 성공!");//이미지 촬영 성공
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("지문을 찾고 있습니다...");// 지문을 찾지 못함
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("센서와의 통신 에러"); // 센서와의 통신에서
      return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("이미지 에러"); // 촬영중 이미지에러
      return p;
    default:
      Serial.println("알 수 없는 에러");
      return p;
  }



  p = finger.image2Tz();//지문을 등록하기 전에 특징 템플릿으로 변환, 지문인식을 데이터화 실제로 지문 등록은 여기서 종료
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("이미지 변환 성공!"); //이미지 변환 성공
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("이미지가 너무 지저분합니다"); // 이미지가 너무 지저분함
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("센서와의 통신 에러"); // 센서와의 통신 에러
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("지문의 특징 확인 실패"); // 지문의 특징 확인 실패
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("지문의 특징 확인 실패"); //지문의 특징 확인 실패
      return p;
    default:
      Serial.println("알 수 없는 에러");
      return p;
  }

  
  p = finger.fingerSearch();// 변환된 지문 데이터를 저장되어 있는 데이터와 비교
  if (p == FINGERPRINT_OK) {
    Serial.println("등록된 지문과 일치 합니다!"); 
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("센서와의 통신 에러");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("일치하는 지문을 찾지 못했습니다");
    return p;
  } else {
    Serial.println("알 수 없는 에러");
    return p;
  }
  
  // Serial.print("ID #");Serial.print(finger.fingerID);//지문센서 ID 출력
  // Serial.print("과 "); Serial.print(finger.confidence);Serial.println(" 일치합니다."); //신뢰도 값을 출력하고 줄 바꿈
  

  if(s1[finger.fingerID-1]=='\0'){
    Serial.println("사용자 등록를 완료해주세요");
  }
  else{
    Serial.println(s1[finger.fingerID-1]);
  }
  Serial.print("name, ");
  Serial.print(s1[finger.fingerID-1]);
  

  return finger.fingerID;
}


// int getFingerprintIDez() {
//   uint8_t p = finger.getImage();
//   if (p != FINGERPRINT_OK)  return -1;

//   p = finger.image2Tz();
//   if (p != FINGERPRINT_OK)  return -1;

//   p = finger.fingerFastSearch();
//   if (p != FINGERPRINT_OK)  return -1;

//   // found a match!
//   Serial.print("ID #"); Serial.print(finger.fingerID);
//   Serial.print("과 "); Serial.print(finger.confidence);Serial.println(" 일치합니다.");
//   return finger.fingerID;
// }
