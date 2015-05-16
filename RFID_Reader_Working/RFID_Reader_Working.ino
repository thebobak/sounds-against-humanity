/*
  
	Sounds Againts Humanity

	Author:	Bobak Shafiei [http://thebobak.com]   
	Github:	thebobak.github.com

	Based on portions of the RFID program from Parallax/Joe Grand
	https://www.parallax.com/downloads/rfid-arduino-example-code

	Version:	1.0 [May 10, 2015]

  
*/


#include <SoftwareSerial.h>

////////////////////////
// Pin Configurations //
////////////////////////

#define enablePin  9 		// Connects to the RFID's ENABLE pin
#define rxPin      2 		// Serial input (connects to the RFID's SOUT pin)
#define txPin      3 		// Serial output (unused)
#define rxMP3      10 		// Serial Input from MP3 Player
#define txMP3      11 		// Serial Output to MP3 Player

#define GREEN_LED  6
#define RED_LED    5



/////////////////////////
// RFID Configurations //
/////////////////////////

#define BUFSIZE    11 		// Size of receive buffer (in bytes) (10-byte unique ID + null character)

#define RFID_START  0x0A 	// RFID Reader Start and Stop bytes
#define RFID_STOP   0x0D


///////////////////////////
// Software Serial Ports //
///////////////////////////

SoftwareSerial rfidSerial   = SoftwareSerial(rxPin, txPin);
SoftwareSerial COM1         = SoftwareSerial(rxMP3, txMP3);



String NOW_PLAYING;



//////////////////
// Track Struct //
//////////////////

struct Track
{
    const int  index;		// index of this track  
    const char code[BUFSIZE];		// The RFID tag's code for this track
    
};


/////////////////////
// Array of tracks //
/////////////////////

/*
In this version of the software, the Unique IDs of each RFID
card are in a key/value pair with an index number.
Future versions might have a more versatile method.
*/


Track tracks[]  = {
  // Index (int),   RFID Unique ID (String) 
  
  {1, "20004041DD"},
  {2, "2000404223"},
  {3, "2000409206"},
  {4, "20004043DB"},
  {5, "2000404406"},
  {6, "2000409B43"},
  {7, "20004073CE"},
  {8, "200040748D"},
  {9, "20004074A6"},
  {10, "200040A183"},
  {11, "2000405330"},
  {12, "2000404182"},
  {13, "20004082F7"},
  {14, "2000407412"},
  {15, "2000407385"},
  {16, "2000404E7A"},
  {17, "2000404E0D"},
  {18, "20004062D1"},
  {19, "2000406308"},
  {20, "2000408205"},
  {21, "2000404210"},
  {22, "2000408264"},
  {23, "200040811E"},
  {24, "2000469AAF"},
  {25, "2000469ADA"},
  {26, "2000406661"},
  {27, "200040671B"},
  {28, "200040663E"},
  {29, "2000404A75"},
  {30, "2000408C34"},
  {31, "2000408B3A"},
  {32, "2000407BE7"},
  {33, "2000407AEE"},
  {34, "2000407AD9"},
  {35, "200040834A"},
  {36, "2000408337"},
  {37, "20004082DC"},
  {38, "20004069BC"},
  {39, "2000468259"},
  {40, "2000465BDD"},
  {41, "2000465BF6"},
  {42, "2000407D1A"},
  {43, "2000407CE9"},
  {44, "2000407D5B"},
  {45, "2000407D2E"},
  {46, "2000407CEB"},
  {47, "2000404A1C"},
  {48, "2000404A5E"},
  {49, "20004041DB"},
  {50, "2000404180"},
  {51, "4F0040CB91"},
  {52, "4F004223C5"}

};


const size_t NUM_TRACKS = sizeof(tracks) / sizeof(tracks[0]);


////////////////////////////////////
// Setup.  Runs once at the start //
////////////////////////////////////

void setup()
{
	// define pin modes
	pinMode(enablePin, OUTPUT);
	pinMode(rxPin, INPUT);
        pinMode(GREEN_LED, OUTPUT);
        pinMode(RED_LED, OUTPUT);

	// Disable RFID Reader //
	digitalWrite(enablePin, HIGH);
  
	// Setup Arduino Serial Monitor
	Serial.begin(9600);
	while (!Serial);   // wait until ready
	Serial.println("\n\nSounds Against Humanity");
  
	// Setup Software Serial Ports (RFID and MP3 modules)
	rfidSerial.begin(2400);
	COM1.begin(115200);
	rfidSerial.listen();
	Serial.flush();   // wait for all bytes to be transmitted to the Serial Monitor


  	while (!COM1);   // wait until ready
  	// Raise volume on device
	for(int i=0; i<15; i++) {
		COM1.write("+");
	}
	COM1.flush();

        // Blink the Lights //
        digitalWrite(GREEN_LED, HIGH);
        delay(300);
        digitalWrite(GREEN_LED, LOW);
        delay(300);
        digitalWrite(GREEN_LED, HIGH);
        delay(300);
        digitalWrite(GREEN_LED, LOW);
        delay(300);
        digitalWrite(GREEN_LED, HIGH);
        

}


////////////////////
// Game Functions //
////////////////////


String getRFID() {    // Returns a String containing the RFID of the scanned card

  	char rfidData[BUFSIZE];  // Buffer for incoming data
  	char offset = 0;         // Offset into buffer
  	rfidData[0] = 0;         // Clear the buffer

  	String theCard = ""; 

  	digitalWrite(enablePin, LOW);   // enable the RFID Reader
  	while(1) {
    	if (rfidSerial.available() > 0) { // If there are any bytes available to read, then the RFID Reader has probably seen a valid tag
      		rfidData[offset] = rfidSerial.read();  // Get the byte and store it in our buffer
      		if (rfidData[offset] == RFID_START) {    // If we receive the start byte from the RFID Reader, then get ready to receive the tag's unique ID
        		offset = -1;     // Clear offset (will be incremented back to 0 at the end of the loop)
      		}
      		else if (rfidData[offset] == RFID_STOP) { // If we receive the stop byte from the RFID Reader, then the tag's entire unique ID has been sent
        		rfidData[offset] = 0; // Null terminate the string of bytes we just received
        		break;                // Break out of the loop
      		}
          
      		offset++;  // Increment offset into array
      		if (offset >= BUFSIZE) {
      			offset = 0;
      		} // If the incoming data string is longer than our buffer, wrap around to avoid going out-of-bounds
    	}
  	}

	digitalWrite(enablePin, HIGH);  // Disable RFID reader
        digitalWrite(GREEN_LED, LOW);
        digitalWrite(RED_LED, HIGH);
        delay(100);
        digitalWrite(GREEN_LED, HIGH);
        delay(100);
        digitalWrite(GREEN_LED, LOW);
        delay(100);
        digitalWrite(GREEN_LED, HIGH);
        digitalWrite(RED_LED, LOW);

	// Copy the RFID to a String object //
	for(int i=0; i< BUFSIZE; i++) {
    	theCard += rfidData[i];
  	}
	return theCard;

} // end of getRFID Function



String formatIndex(int index) {   // Formats the index number into a 5-digit, zero padded string, which is what the sound player expects
  String input = String(index);
  String output = "00000";
  int insert  = 4;
  
  for(int i = input.length() - 1; i >=0; i--) {
    output[insert] = input[i];
    insert--;
  }
  
  return output;
}




bool playTrack(String scannedCard) {  // Transmits track to be played to the sound player //

	for (int i = 0; i < NUM_TRACKS; i++) {
    	if (NOW_PLAYING != scannedCard) {
    		if (scannedCard == tracks[i].code) {
        		NOW_PLAYING = tracks[i].code;
        		Serial.print("Found Card ID:  ");
        		//Serial.println(tracks[i].code);
        		Serial.print("Playing Track:  ");
        		//Serial.println(tracks[i].title);
                        Serial.println(formatIndex(tracks[i].index));

        		// Possible Addition:  This listens for a return from the MP3 that the track has finished
        		COM1.write("s"); //stop current track
        		delay(100);
        		COM1.println(formatIndex(tracks[i].index));
                        //COM1.println(scannedCard);
                        delay(100);
      		}
      		else {
        		Serial.println("Card Not Found");
      		}
    	}
    	else {
    		Serial.println("Try again with different card");
    	}
  	}
  
  return true;
} // end of playTrack Function



///////////////////////
// Main program loop //
///////////////////////

void loop() {

	Serial.println("Ready to Scan\n");
	if(playTrack(getRFID())) {
          delay(100);
          }
          else {
            delay(1000);
          }

}






