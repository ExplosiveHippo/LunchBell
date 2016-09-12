/*
  Checking Gmail Inbox with the Process class and the Curl command
 
 This code checks for unread messages under a specific label in the Gmail Inbox
 The label can be specified via REST webservies, thanks to the Bridge Class (included in the process Class)
 and then saved on a configuration file in the local file system so it will be available after the reboot
 A LED on Pin 13 will inform about the presence of unread messages
 
 This project uses code by Stefano Guglielmetti and Massimo Ronca 2013
 
 Based on the ShellCommands Example by Cristian Maglie and Tom Igoe
 and on the LedControl library examples from http://playground.arduino.cc/Main/LedControl
 
 This example code is in the public domain.
 */

#include <Process.h>
#include <FileIO.h>

const int notificationPin =  12; /* Hammer on Pin 13 */
const char* settings_file = "/root/gmail_settings\0"; /* This is the settings file */

char labelbuffer[256]; /* We will need a buffer for getting the label parameter via Bridge's REST APIs */
String label;

int lastReading;

/* GMAIL SETTINGS */

//Enter gmail account information here
const String username = "xxxxxxxx";
const String password = "xxxxxxxx";

void setup() {
  pinMode(notificationPin, OUTPUT); /* Initialize the LED Pin */


  Bridge.begin(); 	/* Initialize the Bridge */
  Serial.begin(9600); 	/* Initialize the Serial for debugging */
  FileSystem.begin();   /* Setup File IO */
}

void loop() {
/* Checks if a label has been passed via webservices 
  the call is like http://arduino.local/data/put/label/LABEL
*/
  Bridge.get("label", labelbuffer, 256); 

  /*
    Checks if a label has been specified via webservices and stores it on a configuration file
   If a label has not been specified, it retrieves the last on from the configuration file
   */

  if (String(labelbuffer).length() > 0 && label != String(labelbuffer)){
    label = String(labelbuffer);
    File settings = FileSystem.open(settings_file, FILE_WRITE);
    settings.print(label);
    settings.close();
  } 
  else {
    label = "";
    File settings = FileSystem.open(settings_file, FILE_READ);
    while (settings.available() > 0){
      char c = settings.read();
      label += c;
    }
    settings.close();
  }
  
  Serial.println("label:" + label);

  Process p;

  /* 
   This command checks for a specified sender and returns the number of messages, 
   i changed it to check for a label because I thought the label was more flexible as I could configure it from Gmail, 
   but I leave it here for your pleasure
   p.runShellCommand("curl -u USERNAME:PASSWORD \"https://mail.google.com/mail/feed/atom\" -k --silent |grep -o \"<email>" + String(email_addr) + "</email>\"|wc -l"); 
   */


  /* This command checks for a specified label and returns the number of messages */
  p.runShellCommand("curl -u " + username + ":" + password + " \"https://mail.google.com/mail/feed/atom/" + label + "\" -k --silent |grep -o \"<fullcount>[0-9]*</fullcount>\" |grep -o \"[0-9]*\"");

  while(p.running());  /* do nothing until the process finishes, so you get the whole output */

  /* Read command output. runShellCommand() shoulsd have passed an integer number" */
  int result = p.parseInt();  /* look for an integer */
  Serial.print("Result:"); /* Some serial debugging */
  Serial.println(result);
  p.flush();
  

  // This logic checks if the newest reading is more than the previous, meaning there is an ADDITIONAL email in the inbox, not just a change in value  
  if (result > lastReading){
    for(int i = 0; i < 3; i++){
      digitalWrite(hammerPin, HIGH); /* If I got messages, then I activate the notifiation */
      delay(100);
      digitalWrite(hammerPin, LOW);
      delay(1000);
    }
  }else{
    digitalWrite(hammerPin, LOW); /* No messages, so I turn the LED off */
  }

  //Last reading is now the current result
  lastReading = result;
  
  delay(2000);  // wait 2 seconds before you do it again
}









