// !!! DRIVER VERSION: 0.6.4a !!!
// !!! Api Version: 7 !!!

#include "src/BottangoCore.h"
#include "src/BasicCommands.h"
#include "Queue.h"
#include "DebugWCB.h" 

#define SERIAL1_TX_PIN      33  //  // Serial 1 Tx Pin
#define SERIAL1_RX_PIN      25 //  // Serial 1 Rx Pin
#define SERIAL1_DEFAULT_BAUD_RATE 9600
#define s1Serial Serial1
 String HOSTNAME = "Head Driver";
String serialResponse = "";
String s1;

  debugClass Debug;
  String debugInputIdentifier ="";

  // Sets up the queueinig for the incoming commands to process
Queue<String> queue = Queue<String>();
bool haveCommands;
  String currentCommand;
  String previousCommand;
//////////////////////////////////////////////////////////////////////
///*****        Command Varaiables, Containers & Flags        *****///
//////////////////////////////////////////////////////////////////////
  
  char inputBuffer[300];
  String inputString;         // a string to hold incoming data
  
  char inputM[5];
  int inputStringInt;

  volatile boolean stringComplete  = false;      // whether the serial string is complete
  String autoInputString;         // a string to hold incoming data
  volatile boolean autoComplete    = false;    // whether an Auto command is setA
  

  boolean startUp = true;
  boolean isStartUp = true;
  
  //Main Loop Timers
  unsigned long mainLoopTime; 
  unsigned long MLMillis;
  byte mainLoopDelayVar = 5;
  int commandLength;
  char CommandCharacter = ':';



  uint32_t Local_Command[6]  = {0,0,0,0,0,0};
  byte localCommandFunction     = 0;

  uint32_t Animation_Command[6]  = {0,0,0,0,0,0};
  byte AnimationCommadFunction = 0;

void SerialEvent() {
  while (Serial.available() > 0) {    
    serialResponse = Serial.readStringUntil('\r');
    // Debug.SERIAL_EVENT("Serial 1 Input: %s \n", serialResponse.c_str());
    // serialicomingport = 1;
    // serialCommandisTrue  = true;
    processSerial(serialResponse);
  }
} 
void s1SerialEvent() {
  while (s1Serial.available() > 0) {    
    serialResponse = s1Serial.readStringUntil('\r');
    // Debug.SERIAL_EVENT("Serial 1 Input: %s \n", serialResponse.c_str());
    // serialicomingport = 1;
    // serialCommandisTrue  = true;
    processSerial(serialResponse);
  }
}

void processSerial(String incomingSerialCommand){
  // turnOnLEDSerial();
    // resetSerialNumberMillis = millis();
  // incomingSerialCommand += DELIMITER;
    char buf[200];
  serialResponse.toCharArray(buf, sizeof(buf));
  char *p = buf;
  char *str;
  while ((str = strtok_r(p,"*", &p)) !=NULL){
  s1 = String(str);
  // Serial.println(s1);
  queue.push(s1);
  };
  // turnOffLED();
}

void setup()
{
  Serial.begin(115200);
  s1Serial.begin(SERIAL1_DEFAULT_BAUD_RATE,SERIAL_8N1,SERIAL1_RX_PIN,SERIAL1_TX_PIN);
  Serial.println("\n\n----------------------------------------");
  Serial.print("Booting up the ");Serial.println(HOSTNAME);

  delay(50);
  BottangoCore::bottangoSetup();
}

















void loop(){
    if(Serial.available()){SerialEvent();}
    if(s1Serial.available()){s1SerialEvent();}

    if (millis() - MLMillis >= mainLoopDelayVar){
    MLMillis = millis();
    if(startUp) {
        // colorWipeStatus("ES",blue,10);
      startUp = false;
      Serial.print("Startup complete\nStarting main loop\n\n\n");
    }
    if (queue.count()>0) {autoComplete=false;}
    if (queue.count()>0 || autoComplete) {
    if(queue.count()>0) {
      // Debug.SERIAL_EVENT("New Queue String: %s\nQueue Count %i\n", queue.peek().c_str(),queue.count());
      haveCommands = queue.count(); 
      currentCommand = queue.peek();
      inputString = queue.pop(); 
      // Debug.LOOP("Comamand Accepted into Loop: %s \n", inputString.c_str());
      inputString.toCharArray(inputBuffer, 300);inputString="";}

        else if (autoComplete) {autoInputString.toCharArray(inputBuffer, 300);autoInputString="";}
              if (inputBuffer[0] == '#'){
                if (
                    inputBuffer[1]=='D' ||          // Command for debugging
                    inputBuffer[1]=='d' ||          // Command for debugging
                    inputBuffer[1]=='L' ||          // Command designator for internal functions
                    inputBuffer[1]=='l'           // Command designator for internal functions

                  ){commandLength = strlen(inputBuffer); 
                    if (inputBuffer[1]=='D' || inputBuffer[1]=='d'){
                      debugInputIdentifier = "";                            // flush the string
                      for (int i=2; i<=commandLength-1; i++){
                        char inCharRead = inputBuffer[i];
                        debugInputIdentifier += inCharRead;                   // add it to the inputString:
                      }
                      debugInputIdentifier.toUpperCase();
                      Debug.toggle(debugInputIdentifier);
                      debugInputIdentifier = "";                             // flush the string
                      } else if (inputBuffer[1]=='L' || inputBuffer[1]=='l') {
                        localCommandFunction = (inputBuffer[2]-'0')*10+(inputBuffer[3]-'0');
                        Local_Command[0]   = '\0';                                                            // Flushes Array
                        Local_Command[0] = localCommandFunction;
                      Debug.LOOP("Entered the Local Command Structure \n");
                      }
                  //     } else if (inputBuffer[1]=='S' || inputBuffer[1]=='s'){
                  //       for (int i=1; i<commandLength;i++ ){
                  //       char inCharRead = inputBuffer[i];
                  //       serialStringCommand += inCharRead;  // add it to the inputString:
                  //     }
                  //     serialPort = serialStringCommand.substring(0,2);
                  //     serialSubStringCommand = serialStringCommand.substring(2,commandLength);
                  //     Debug.LOOP("Serial Baudrate: %s on Serial Port: %s\n", serialSubStringCommand.c_str(), serialPort); 
                  //     int tempBaud = serialSubStringCommand.toInt();
                  //     if (serialPort == "S1"|| serialPort == "s1"){
                  //       if (tempBaud == 0 || tempBaud == 1){
                  //         saveBaud("S1BDCST", tempBaud);
                  //       }else{
                  //         saveBaud("S1BAUD", tempBaud);
                  //       }
                          
                  //     } else if (serialPort == "S2" || serialPort == "s2"){
                  //         if (tempBaud == 0 || tempBaud == 1){
                  //         saveBaud("S2BDCST", tempBaud);
                  //       }else{
                  //         saveBaud("S2BAUD", tempBaud);
                  //       }
                  //     } else if (serialPort == "S3" || serialPort == "s3"){
                  //         if (tempBaud == 0 || tempBaud == 1){
                  //         saveBaud("S3BDCST", tempBaud);
                  //       }else{
                  //         saveBaud("S3BAUD", tempBaud);
                  //       }
                  //     }else if (serialPort == "S4" || serialPort == "s4"){
                  //         if (tempBaud == 0 || tempBaud == 1){
                  //         saveBaud("S4BDCST", tempBaud);
                  //       }else{
                  //         saveBaud("S4BAUD", tempBaud);
                  //       }
                  //     } else if (serialPort == "S5" || serialPort == "s5"){
                  //         if (tempBaud == 0 || tempBaud == 1){
                  //         saveBaud("S5BDCST", tempBaud);
                  //       }else{
                  //         saveBaud("S5BAUD", tempBaud);
                  //       }
                  //     }else if (serialPort == "SC" || serialPort == "sc"){
                  //         clearBaud();
                  //     }else {Debug.LOOP("No valid serial port given \n");}
                      
                  //     serialStringCommand = "";
                  //     serialPort = "";
                  //     } else if (inputBuffer[1]=='Q' || inputBuffer[1]=='q'){
                  //       int WCBQuantity = inputBuffer[2]-'0';
                  //       if (WCBQuantity == 0){
                  //         clearQuantity();
                  //       } else {saveQuantity(WCBQuantity);}
                        
                  //     }else if (inputBuffer[1]=='E' || inputBuffer[1]=='e'){
                  //       String tempESPNOWPASSWORD;                            // flush the string
                  //       for (int i=2; i<=commandLength-2; i++){
                  //         char inCharRead = inputBuffer[i];
                  //         tempESPNOWPASSWORD += inCharRead;                   // add it to the inputString:
                  //     }
                  //       if (tempESPNOWPASSWORD == "CLEAR"){
                  //         clearPassword();
                  //       } else{savePassword(tempESPNOWPASSWORD); }
                  //     }
                  //     else {Debug.LOOP("No valid command entered /n");}
                  // }
                      if(Local_Command[0]){
                        switch (Local_Command[0]){
                          case 1: Serial.println(HOSTNAME);
                                Local_Command[0]   = '\0';                                                           break;
                          case 2: Serial.println("Resetting the ESP in 3 Seconds");
                                //  DelayCall::schedule([] {ESP.restart();}, 3000);
                                ESP.restart();
                                Local_Command[0]   = '\0';                                                           break;
                          case 3: break;  //reserved for future use
                          case 4: ; break;  //reserved for future use
                          // case 5: printf("ESP-NOW Success Count: %i \nESP-NOW Failure Count %i \n", SuccessCounter, FailureCounter);
                                Local_Command[0]   = '\0';
                                break;  //prints out failure rate of ESPNOW
                          case 6: ; break;  //reserved for future use
                          case 7: ; break;  //reserved for future use
                          case 8: ; break;  //reserved for future use                                                         break;  //reserved for future use
                          case 9: ; break;  //reserved for future use

                        }
                      }
                  }
              }else if (inputBuffer[0] == CommandCharacter){
                        if( inputBuffer[1]=='W' ||        // Command for Sending ESP-NOW Messages
            inputBuffer[1]=='A' ||        // Command for Starting Animations
            inputBuffer[1]=='A' ||        // Command for Starting Animations
            inputBuffer[1]=='E' ||        // Command for Misc
            inputBuffer[1]=='e'         // Command for Misc

        ){commandLength = strlen(inputBuffer);                     //  Determines length of command character array.
          Debug.DBG("Command Length is: %i\n" , commandLength);
          if(commandLength >= 3) {
            if(inputBuffer[1]=='A' || inputBuffer[1]=='a') {
                AnimationCommadFunction = (inputBuffer[2]-'0')*10+(inputBuffer[3]-'0');
                Serial.println(AnimationCommadFunction);
                  BottangoCore::commandStreamProvider.startCommandStream(AnimationCommadFunction, false); // Start animation without looping
                Animation_Command[0] = '\0';
                Animation_Command[0]= AnimationCommadFunction;
              }  
            if(inputBuffer[1]=='E' || inputBuffer[1]=='e') {

            } 
          }
        }
      }
    ///***  Clear States and Reset for next command.  ***///
      stringComplete =false;
      autoComplete = false;
      inputBuffer[0] = '\0';
      inputBuffer[1] = '\0'; 
      inputBuffer[2] = '\0'; 
      inputBuffer[3] = '\0'; 

      }  
    if(isStartUp) {
      isStartUp = false;
      delay(500);
    }
  }
  BottangoCore::bottangoLoop();
}
