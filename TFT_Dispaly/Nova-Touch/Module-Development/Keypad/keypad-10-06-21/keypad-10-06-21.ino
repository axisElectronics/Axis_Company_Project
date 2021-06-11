

#define ROWS 5
#define COLS 4

int rowpins[ROWS] = {2,3,4,5,6}; 
int colpins[COLS] = {10,11,12,13};

void setup() {
  Serial.begin(9600);

  // All COLS pins will be high.
  for(int i=0; i < COLS; i++){
    pinMode(colpins[i], OUTPUT );
    digitalWrite(colpins[i], HIGH );
  }

  // All ROWS pins will be Low.
  for(int i=0; i < ROWS; i++){
    pinMode(rowpins[i], INPUT);  
  }
 
}

void loop() {
 

}
