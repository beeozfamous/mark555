import processing.serial.*;


Serial myPort; //creates a software serial port on which you will listen to Arduino
Table dataTable; //table where we will read in and store values. You can name it something more creative!
 
int numReadings = 500; //keeps track of how many readings you'd like to take before writing the file. 
int readingCounter = 0; //counts each reading to compare to numReadings. 
 
String fileName;
void setup()
{
  String portName = Serial.list()[4]; 
  print(Serial.list()[4]);
  //CAUTION: your Arduino port number is probably different! Mine happened to be 1. Use a "handshake" sketch to figure out and test which port number your Arduino is talking on. A "handshake" establishes that Arduino and Processing are listening/talking on the same port.
  //Here's a link to a basic handshake tutorial: https://processing.org/tutorials/overview/
  
  myPort = new Serial(this, portName, 115200); //set up your port to listen to the serial port
  dataTable = new Table();
  dataTable.addColumn("id"); //This column stores a unique identifier for each record. We will just count up from 0 - so your first reading will be ID 0, your second will be ID 1, etc. 
  
  //the following adds columns for time. You can also add milliseconds. See the Time/Date functions for Processing: https://www.processing.org/reference/ 
  
  //the following are dummy columns for each data value. Add as many columns as you have data values. Customize the names as needed. Make sure they are in the same order as the order that Arduino is sending them!
  dataTable.addColumn("TimeStamp");
  dataTable.addColumn("LandmarkID");
  dataTable.addColumn("AcX");
  dataTable.addColumn("AcY");
  dataTable.addColumn("AcZ");
  dataTable.addColumn("GyX");
  dataTable.addColumn("GyY");
  dataTable.addColumn("GyZ");
 
}
 
void serialEvent(Serial myPort){
  String val = myPort.readStringUntil('\n'); //The newline separator separates each Arduino loop. We will parse the data by each newline separator. 
  if (val!= null) { //We have a reading! Record it.
    val = trim(val); //gets rid of any whitespace or Unicode nonbreakable space
    //Optional, useful for debugging. If you see this, you know data is being sent. Delete if  you like. 
      int sensorVals[] = int(split(val, ',')); //parses the packet from Arduino and places the valeus into the sensorVals array. I am assuming floats. Change the data type to match the datatype coming from Arduino. 
  
      if(sensorVals.length==7){
      print("\n");
      print(val);
      TableRow newRow = dataTable.addRow(); //add a row for this new reading
      newRow.setInt("id", dataTable.lastRowIndex());//record a unique identifier (the row's index)
      
      
      ////record sensor information. Customize the names so they match your sensor column names.
      newRow.setString("TimeStamp",year()+"-"+ month()+"-"+day()+"-"+hour()+"-"+ minute()+"-"+ second()+"-"+millis());
      newRow.setInt("LandmarkID", sensorVals[0]);
      newRow.setInt("AcX", sensorVals[1]);
      newRow.setInt("AcY", sensorVals[2]);
      newRow.setInt("AcZ", sensorVals[3]);
      newRow.setInt("GyX", sensorVals[4]);
      newRow.setInt("GyY", sensorVals[5]);
      newRow.setInt("GyZ", sensorVals[6]);
      
      readingCounter++; //optional, use if you'd like to write your file every numReadings reading cycles
           
      saveTable(dataTable, "WalkAround.csv"); 
    
   }
 }
}
 
void draw()
{ 
   //visualize your sensor data in real time here! In the future we hope to add some cool and useful graphic displays that can be tuned to different ranges of values. 
}
