
class Pelota{
  float x = width/2;
  float y = height/2;
  float xspeed = 3;
  float yspeed = 2;
  
 void update(){
  x = x + xspeed;
  y = y + yspeed;
 }
  
  void edges() {
  if (y < 10 || y > height-10)
    yspeed *= -1;
  if (x < 10 || x > width-10)
    xspeed *= -1;
  }
  
  void show(){
  fill(255);
  ellipse(x , y, 24, 24);  
  }
}
