import processing.serial.*;

Pelota pelota;
Player player1;
Player player2;
int pene;

void setup(){
  commSetup();
  size(768, 432);
  frameRate(60);
  pelota = new Pelota();
  player1 = new Player();
  player2 = new Player();
}

void draw(){

  if ( isConnected ) {
      if ( dataAvailable(CHANNEL_2) ) {
        pene = getData(CHANNEL_2);
      }
    }
  
  background(0);
  stroke(255);
  rect(5, mouseY, 20,40);
  rect(width-30, mouseX, 20,40);
  pelota.show();
  pelota.update();
  pelota.edges();
  addScore();
  drawScore();
  winGame();
  commManager();  
}

  void winGame(){
    if(player1.score == 10 || player2.score == 10){
      exit();
    }
  }

  void drawScore(){
    textSize(32);
    text(player1.score, (width/2)-50, 30);
    text(player2.score, (width/2)+50, 30);
  }
  
  void addScore(){
    if(pelota.x < 10)
        player2.score += 1;
    else if(pelota .x > width-10)
        player1.score += 1;
  }
