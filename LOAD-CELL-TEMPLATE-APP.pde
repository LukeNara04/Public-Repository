/*LIBRARIES*/
import processing.serial.*;
import grafica.*;

/*OBJECTS AND VARIABLES*/
Serial myPort;  
String data;
String[] datastring;

PImage bg;

PFont font1;
PFont font2;
PFont font3;

float time;//val 1
float thrust;//val2
float impulse;
float lastdata = 0;
String finish = "false";

/*PLOTS*/
GPlot plot1;
PrintWriter output;

void setup()
{
  size(1920,1080);
  printArray(Serial.list());
  myPort = new Serial(this, Serial.list()[0],115200);
  myPort.clear();
  
  plot1 = new GPlot(this);
  plot1.setPos(150,100);
  plot1.setDim(1500, 600);
  plot1.getTitle().setText("THRUST | TIME");
  plot1.getXAxis().getAxisLabel().setText("Time");
  plot1.getYAxis().getAxisLabel().setText("Thrust");
  plot1.setPointColor(color(0,0,0,255));
  plot1.setPointSize(3);
  
  
  bg =loadImage("1.jpg");
  
  font1 = createFont("font1.ttf", 60);
  font2 = createFont("font2.TTF", 20);
  font3 = createFont("font3.vlw", 20);
  
  output = createWriter("data_thrust.txt");
}


void draw()
{
    while (myPort.available() > 2)
  { 
         delay(19);
         data = myPort.readString();
         datastring = splitTokens(data, ",");
         thrust = float(datastring[0]);
         
         if (thrust > 1.5 && abs(lastdata - thrust) < 200 )  { 
           impulse = impulse+thrust/1000*9.81*0.1;
           plot1.addPoint(time, thrust);
           time = time + 100;
           
           output.println(time/1000 + "," + thrust/1000*9.81);
         }
         
         lastdata = thrust;   
         if (keyPressed) {
             output.flush(); // Writes the remaining data to the file
             output.close(); // Finishes the file
             finish = "true";
          }
  }
  
  background (bg);

  plot1.beginDraw();
  plot1.drawBackground();
  plot1.drawBox();
  plot1.drawXAxis();
  plot1.drawYAxis();
  plot1.drawTitle();
  plot1.drawGridLines(GPlot.BOTH);
  plot1.drawPoints();
  plot1.drawLines();
  plot1.endDraw();

  /*TITLE*/
  textSize(60);
  textFont(font1);
  fill(0, 92, 230);
  text("LOAD DATA", 795, 80); 
 
  /*THRUST*/
  textSize(20);
  textFont(font3);
  fill(0, 0, 0);
  text("THRUST", 800, 850);
  
  /*THRUST DATA*/
  textSize(20);
  textFont(font2);
  fill(0, 0, 0);
  text(thrust, 800, 900);
  
  /*IMPULSE*/
  textSize(20);
  textFont(font3);
  fill(0, 0, 0);
  text("IMPULSE", 1050, 850);
  
  /*IMPULSE DATA*/
  textSize(20);
  textFont(font2);
  fill(0, 0, 0);
  text(impulse, 1050, 900);
  
  /*FINISH*/
  textSize(20);
  textFont(font3);
  fill(0, 0, 0);
  text("FINISH?", 925, 850);
  
  /*FINISH DATA*/
  textSize(20);
  textFont(font2);
  fill(0, 0, 0);
  text(finish , 925, 900);
  
}
