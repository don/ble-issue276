// This program sends the numbers 0 through 250 to a phone
// as fast as possible. It repeats this action every time
// a device subscribes to the characteristic.

// https://github.com/don/cordova-plugin-ble-central/issues/276

#include <CurieBLE.h>
#define MAX 250

BLEPeripheral blePeripheral;
BLEService service("aa7b3c40-f6ed-4ffc-bc29-5750c59e74b3");
BLECharacteristic bpmChar("b0351694-25e6-4eb5-918c-ca9403ddac47", BLERead | BLENotify, 1);  

long startMillis;
int counter; // the data we're sending
boolean subscribed = false;

void setup() { // called when the program starts

  Serial.begin(115200); // set up the serial monitor
  while(!Serial);     // wait for the serial monitor

  setUpBLE(); // sets up the bluetooth services and characteristics 

}

void setUpBLE() {
  
    /* Set a local name for the BLE device */
    blePeripheral.setLocalName("Penelope");
    blePeripheral.setAdvertisedServiceUuid(service.uuid());  // add the service UUID
    blePeripheral.addAttribute(service);// add the BLE service
    blePeripheral.addAttribute(bpmChar);  // add the BPM characteristic

      /* Now activate the BLE device.  It will start continuously transmitting BLE
       advertising packets and will be visible to remote BLE central devices
       until it receives a new connection */
    blePeripheral.begin();

    blePeripheral.setEventHandler(BLEConnected, blePeripheralConnectHandler);
    blePeripheral.setEventHandler(BLEDisconnected, blePeripheralDisconnectHandler);
    
    bpmChar.setEventHandler(BLESubscribed, centralSubscribed);
    bpmChar.setEventHandler(BLEUnsubscribed, centralUnsubscribed);

    Serial.println("Bluetooth device active, waiting for connections...");
  
}

void loop(){
  blePeripheral.poll();

  if (subscribed && counter <= MAX){ // send the numbers 0-250 to the phone

    unsigned char bpmCharArray[1] = {(unsigned char) counter };
    bpmChar.setValue(bpmCharArray, 1); // send to phone

    Serial.print(millis());
    Serial.print(" ");
    Serial.println(counter);
   
    if (counter == MAX) {
        long elapsed = millis() - startMillis;
        Serial.print("Elapsed time ");
        Serial.print(elapsed/1000);
        Serial.println(" seconds");
    }

    counter++;

  }

}

void blePeripheralConnectHandler(BLECentral& central) {
  Serial.print("Connected event, central: ");
  Serial.println(central.address());
}

void blePeripheralDisconnectHandler(BLECentral& central) {
  Serial.print("Disconnected event, central: ");
  Serial.println(central.address());
}

void centralSubscribed(BLECentral& central, BLECharacteristic& characteristic) {
  counter = 0;
  startMillis = millis();
  subscribed = true;
}

void centralUnsubscribed(BLECentral& central, BLECharacteristic& characteristic) {
  subscribed = false;
}
