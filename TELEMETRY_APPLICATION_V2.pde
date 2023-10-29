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

float acx;//val1
float acy;//val2
float acz;//val3

float gyx;//val4
float gyy;//val5
float gyz;//val6

float alt;//val7
float vel;//val8
float tmp;//val9
float maxalt;//val10
int par;//val11

float rssi;//val12
float time;


/*PLOTS*/
GPlot plot1;
GPlot plot2;
GPlot plot3;
GPlot plot4;
GPlot plot5;

void setup()
{
  size(1920,1080);
  printArray(Serial.list());
  myPort = new Serial(this, Serial.list()[0],115200);
  myPort.clear();
  
  plot1 = new GPlot(this);
  plot1.setPos(0,0);
  plot1.setDim(450, 450);
  plot1.getTitle().setText("ALT | TIME");
  plot1.getXAxis().getAxisLabel().setText("Time");
  plot1.getYAxis().getAxisLabel().setText("Alt");
  plot1.setPointColor(color(0,0,0,255));
  plot1.setPointSize(3);
  
  plot2 = new GPlot(this);
  plot2.setPos(1370,0);
  plot2.setDim(450, 450);
  plot2.getTitle().setText("GYX | TIME");
  plot2.getXAxis().getAxisLabel().setText("Time");
  plot2.getYAxis().getAxisLabel().setText("Gyx");
  plot2.setPointColor(color(0,0,0,255));
  plot2.setPointSize(3);
  
  plot3 = new GPlot(this);
  plot3.setPos(0,540);
  plot3.setDim(450, 440);
  plot3.getTitle().setText("ACX | TIME");
  plot3.getXAxis().getAxisLabel().setText("Time");
  plot3.getYAxis().getAxisLabel().setText("Acx");
  plot3.setPointColor(color(0,0,0,255));
  plot3.setPointSize(3);
  
  plot4 = new GPlot(this);
  plot4.setPos(675,540);
  plot4.setDim(450, 440);
  plot4.getTitle().setText("ACY | TIME");
  plot4.getXAxis().getAxisLabel().setText("Time");
  plot4.getYAxis().getAxisLabel().setText("Acy");
  plot4.setPointColor(color(0,0,0,255));
  plot4.setPointSize(3);
  
  plot5 = new GPlot(this);
  plot5.setPos(1370,540);
  plot5.setDim(450, 440);
  plot5.getTitle().setText("ACZ | TIME");
  plot5.getXAxis().getAxisLabel().setText("Time");
  plot5.getYAxis().getAxisLabel().setText("Acz");
  plot5.setPointColor(color(0,0,0,255));
  plot5.setPointSize(3);
  
  bg =loadImage("1.png");
  
  font1 = createFont("font1.ttf", 60);
  font2 = createFont("font2.ttf", 20);
  font3 = createFont("font3.TTF", 20);
}

void draw()
{
  
  background (bg);

  
  while (myPort.available() > 0)
  { 
      data = myPort.readString();
      if (data.length()>57) { 
   
         datastring = splitTokens(data, ",");
   
         rssi = float(datastring[0]);
         acx = float(datastring[1]);
         acy = float(datastring[2]);
         acz = float(datastring[3]);
         gyx = float(datastring[4]);
         gyy = float(datastring[5]);
         gyz = float(datastring[6]);
         alt = float(datastring[7]);
         vel = float(datastring[8]);
         tmp = float(datastring[9]);
         par = int(datastring[10]);
         
         plot1.addPoint(time, alt);
         plot2.addPoint(time, gyx);
         plot3.addPoint(time, acx );
         plot4.addPoint(time, acy );
         plot5.addPoint(time, acz);
         
         if (alt > maxalt){ 
            maxalt = alt;
         } 
     }    
      time=time+0.21;
  }
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
  
  plot2.beginDraw();
  plot2.drawBackground();
  plot2.drawBox();
  plot2.drawXAxis();
  plot2.drawYAxis();
  plot2.drawTitle();
  plot2.drawGridLines(GPlot.BOTH);
  plot2.drawPoints();
  plot2.drawLines();
  plot2.endDraw();
  
  plot3.beginDraw();
  plot3.drawBackground();
  plot3.drawBox();
  plot3.drawXAxis();
  plot3.drawYAxis();
  plot3.drawTitle();
  plot3.drawGridLines(GPlot.BOTH);
  plot3.drawPoints();
  plot3.drawLines();
  plot3.endDraw();
  
  plot4.beginDraw();
  plot4.drawBackground();
  plot4.drawBox();
  plot4.drawXAxis();
  plot4.drawYAxis();
  plot4.drawTitle();
  plot4.drawGridLines(GPlot.BOTH);
  plot4.drawPoints();
  plot4.drawLines();
  plot4.endDraw();
  
  plot5.beginDraw();
  plot5.drawBackground();
  plot5.drawBox();
  plot5.drawXAxis();
  plot5.drawYAxis();
  plot5.drawTitle();
  plot5.drawGridLines(GPlot.BOTH);
  plot5.drawPoints();
  plot5.drawLines();
  plot5.endDraw();
  
  /*TITLE*/
  textSize(60);
  textFont(font1);
  fill(0, 92, 230);
  text("TELEMETRY DATA", 680, 80); 
 
  /*ALT*/
  textSize(20);
  textFont(font2);
  fill(0, 0, 0);
  text("ALT", 800, 140);
  
  /*ALT DATA*/
  textSize(20);
  textFont(font3);
  fill(0, 0, 0);
  text(alt, 795, 160);
  
  /*RSSI*/
  textSize(20);
  textFont(font2);
  fill(0, 0, 0);
  text("RSSI", 935, 140);
  
  /*RSSI DATA*/
  textSize(20);
  textFont(font3);
  fill(0, 0, 0);
  text(rssi, 930, 160);
  
  /*VEL*/
  textSize(20);
  textFont(font2);
  fill(0, 0, 0);
  text("VEL", 1080, 140);
  
  /*VEL DATA*/
  textSize(20);
  textFont(font3);
  fill(0, 0, 0);
  text(vel, 1070, 160);
  
  /*ACX*/
  textSize(20);
  textFont(font2);
  fill(0, 0, 0);
  text("ACX", 800, 240);
  
  /*ACX DATA*/
  textSize(20);
  textFont(font3);
  fill(0, 0, 0);
  text(acx, 795, 260);
  
  /*ACY*/
  textSize(20);
  textFont(font2);
  fill(0, 0, 0);
  text("ACY", 940, 240);
  
  /*ACY DATA*/
  textSize(20);
  textFont(font3);
  fill(0, 0, 0);
  text(acy, 930, 260);
  
  /*ACZ*/
  textSize(20);
  textFont(font2);
  fill(0, 0, 0);
  text("ACZ", 1080, 240);
  
  /*ACZ DATA*/
  textSize(20);
  textFont(font3);
  fill(0, 0, 0);
  text(acz, 1070, 260);
  
  /*GYX*/
  textSize(20);
  textFont(font2);
  fill(0, 0, 0);
  text("GYX", 800, 340);
  
  /*GYX DATA*/
  textSize(20);
  textFont(font3);
  fill(0, 0, 0);
  text(gyx, 795, 360);
  
  /*GYY*/
  textSize(20);
  textFont(font2);
  fill(0, 0, 0);
  text("GYY", 940, 340);
  
  /*GYY DATA*/
  textSize(20);
  font2 = createFont("", 20);
  textFont(font3);
  fill(0, 0, 0);
  text(gyy, 930, 360);
  
  /*GYZ*/
  textSize(20);
  textFont(font2);
  fill(0, 0, 0);
  text("GYZ", 1080, 340);
  
  /*GYZ DATA*/
  textSize(20);
  textFont(font3);
  fill(0, 0, 0);
  text(gyz, 1070, 360);
  
  /*TMP*/
  textSize(20);
  textFont(font2);
  fill(0, 0, 0);
  text("TMP", 800, 440);
  
  /*TMP DATA*/
  textSize(20);
  textFont(font3);
  fill(0, 0, 0);
  text(tmp, 795, 460);
  
  /*MAXALT*/
  textSize(20);
  textFont(font2);
  fill(0, 0, 0);
  text("MAX", 940, 440);
  
  /*MAXALT DATA*/
  textSize(20);
  textFont(font3);
  fill(0, 0, 0);
  text(maxalt, 930, 460);
 
  /*PAR*/
  textSize(20);
  textFont(font2);
  fill(0, 0, 0);
  text("PAR", 1080, 440);
  
  /*PAR DATA*/
  textSize(20);
  textFont(font3);
  fill(0, 0, 0);
  text(par, 1095, 460);
}
