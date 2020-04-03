//read value from limit switch 
#define switch1 4
#define switch2 7
//link to L298N Motor Driver
#define in1 8
#define in2 9
#define ena 10
//Read value from potentio meter
#define poten_read A2 //

int state = 1; //working state : 1=push 2=release 
int spd = 0 ; //for vary motor speed
int push_rate = 0;  
float T = 0;
float ei_rate = 2; //for I:E rate = 1:2 **you can specify your own rate 
float push_time = 1;
float release_time = 2;

unsigned long T_1;
unsigned long T_2;
unsigned long T_state1;
unsigned long T_state2;

void setup() {
  pinMode(switch1, INPUT_PULLUP);
  pinMode(switch2, INPUT_PULLUP);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(ena, OUTPUT);
  pinMode(poten_read, INPUT);
  Serial.begin(115200);
}

void loop() {
//define state of work, start with state1 (push) 
  if ((digitalRead(switch2) == 0 & state == 1) || (T_state1 >= push_time & state==1)) {
    state = 2;            //either limit switch2 being pressed or Pushing period exceed push_time state will change to state2 (release)
    spd = 140;            //define speed of releasing [constant] **depend on your equipment >> please adjust your own satisfied speed
    T_2 = millis();       //start time of state 2
    delay(push_time - T_state1);  //hold on to keep true push time equal to push_time
    analogWrite(ena, 0);  //stop motor drive 
  }
  else if ((digitalRead(switch1) == 0 & state == 2)|| (T_state2 >= release_time & state==2)) {
    state = 1;            //either limit switch1 being pressed or Releasing period exceed release_time state will change to state1 (push)
    spd = 255 ;           //define speed of pushing [constant] **depend on your equipment >> please adjust your own satisfied speed 
    T_1 = millis();       //start time of state 1
    delay(release_time - T_state2); //hold on to keep true release time equal to release_time
    analogWrite(ena, 0);  //stop motor drive
  }
  else {
    state = state; 
  }
    
  push_rate = analogRead(poten_read);   //adjust potentiometer to specify your breath period
  T = map(push_rate,0,1023,2000,4000);  //minimum breath period = 2 seconds & maximum = 4 seconds 
  push_time = T*(1+ei_rate);            //how long will your equipment push a bag 
                                        // **push time will depend on your limit switch2 location 
  release_time = (ei_rate)*T/(1+ei_rate); //how long will your equipment release a bag 
                                            //**same as push time it depend on your limit switch1 location
  
  T_state1 = millis()- T_1 ;  //how long have your system been in state1 (push)
  T_state2 = millis()- T_2 ;  //how long have your system been in state2 (release)
  
  if (state == 2) {
    //define motor direction (cw/ccw) for releasing
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    analogWrite(ena,spd);  
  } 
  else if (state == 1) {
    //define motor direction (cw/ccw) for pushing
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW); 
    analogWrite(ena,spd);
  }
  else {
    analogWrite(ena, 0);
  }
}
  

  
  
