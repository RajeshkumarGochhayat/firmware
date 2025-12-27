#include <ModbusMaster.h>

//--------------NPK SENSOR (RS485) -----------------
#define RX2 16
#define TX2 17
#define MAX485_DE_RE 15
#define NPK_SLAVE_ID 37

ModbusMaster node;

void preTransmission() {
  digitalWrite(MAX485_DE_RE, HIGH);
}

void postTransmission() {
  digitalWrite(MAX485_DE_RE, LOW);
}

bool readNPK(float &n, float &p, float &k) {
  if (node.readHoldingRegisters(0x03, 3) != node.ku8MBSuccess) {
    return false;
  }

  n = node.getResponseBuffer(0);
  p = node.getResponseBuffer(1);
  k = node.getResponseBuffer(2);

  return true;
}

void setupNPK() {
  Serial2.begin(9600, SERIAL_8N1, RX2, TX2);

  pinMode(MAX485_DE_RE, OUTPUT);
  digitalWrite(MAX485_DE_RE, LOW);

  node.begin(NPK_SLAVE_ID, Serial2);
  node.preTransmission(preTransmission);
  node.postTransmission(postTransmission);
}

void loopNPK() {
  float nitrogen, phosphorus, potassium;

  if (!readNPK(nitrogen, phosphorus, potassium)) {
    return;
  }

  String data =
    "{\"nitrogen\":"   + String(nitrogen, 1) +
    ",\"phosphorus\":" + String(phosphorus, 1) +
    ",\"potassium\":"  + String(potassium, 1) + "}";

  String payload = envelope("npk", data);

  if (!sendPost(payload)) {
    logToSD(payload);
  }
}
