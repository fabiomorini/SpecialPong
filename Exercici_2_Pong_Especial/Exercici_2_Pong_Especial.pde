import processing.serial.*;

Pelota pelota;

void setup(){
  size(768, 432);
  frameRate(60);
  pelota = new Pelota();
}

void draw(){
  background(0);
  pelota.show();
  pelota.update();
  pelota.edges();
}
