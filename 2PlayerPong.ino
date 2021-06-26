//Declarations for OLED Screen
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>


#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

//Our own definitions for the game board
#define LED 10
#define B1 12
#define B2 11
#define PEIZO 8
#define JOY_X A6
#define JOY_Y A7

float ballx = 32;
float bally = 64;
float velx = 2.2;
float vely = 3;

int player1y = 10;
int player2y = 10;

int startgame = 1;
int menu = 1;

int player1score = 0;
int player2score = 0;

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);


void setup() {
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  //Start up and initialize the display

  pinMode(LED, OUTPUT);
  pinMode(B1, INPUT);
  pinMode(B2, INPUT);
  pinMode(PEIZO, OUTPUT);
  pinMode(JOY_X, INPUT);
  pinMode(JOY_Y, INPUT);

  //Clear the display buffer
  display.clearDisplay();

  //Always need to call this to make anything show on the screen
  display.display();
}

void loop() {

  display.println(ballx);
  display.setTextColor(WHITE);
  display.clearDisplay();

  if (startgame == 1) {
    display.setCursor(36, 5);
    display.setTextSize(2);
    display.println("PONG!");
    display.setTextSize(1);
    display.setCursor(0, 30);
    display.println("Continue?");
    display.setCursor(0, 40);
    display.println("Button 1: Yes");
    display.println("Button 2: No");
    int b1Value = digitalRead(B1);
    int b2Value = digitalRead(B2);

    if (b1Value == 1 && b2Value == 0 && menu == 1) {
      startgame = 2;
      menu = 0;
    }
    if (b2Value == 1 && b1Value == 0 && menu == 1) {
      startgame = 0;
      menu = 0;
    }
  }

  else if (startgame == 0) {
    display.setCursor(30, 22);
    display.setTextSize(2);
    display.println("Okay,");
    display.setCursor(20, 42);
    display.println("Goodbye!");
    delay(5000);
  }

  else if (startgame == 2) {
    display.clearDisplay();
    analogWrite(LED, 0);

    display.drawCircle(ballx, bally, 1, WHITE);
    display.fillRect(2, player1y, 2, 20, WHITE);
    display.fillRect(126, player2y, 2, 20, WHITE);
    for (int y = 0; y < 64; y = y + 1){
       display.drawLine(64, y, 64, y+2, WHITE);
       y = y + 3;
    }
    
    display.setCursor(56, 28);
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.print(player1score);
    display.setCursor(68, 28);
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.print(player2score);

    int joyXvalue = analogRead(JOY_X);
    int joyYvalue = analogRead(JOY_Y);
    int b1Value = digitalRead(B1);
    int b2Value = digitalRead(B2);

    ballx = ballx + velx;
    bally = bally + vely;

    Serial.println(ballx);
    Serial.println(bally);

    if (bally > 59) {
      vely = -3;
    }
    else if (bally < 5) {
      vely = 3;
    }

    if (player1y <= 56) {
      if (joyYvalue > 1000) {
        player1y = player1y + 2;
      }
    }

    if (player1y >= 5) {
      if (joyYvalue < 20) {
        player1y = player1y - 2;
      }
    }

    
    if (player2y <= 56) {
      if (b1Value == 1) {
        player2y = player2y + 2;
      }
    }

    if (player2y >= 5) {
      if (b2Value == 1) {
        player2y = player2y - 2;
      }
    }
    
    if (ballx > 3 && ballx < 4 && bally >= player1y - 1 && bally <= player1y + 21) {
      velx = 2.2;
    }

    if (ballx > 122 && ballx < 125 && bally >= player2y - 1 && bally <= player2y + 21) {
      velx = -2.2;
    }

    if (ballx > 128) {
      ballx = 32;
      bally = 64;

      if (velx == -2.2) {
        velx = 2.2;
      }
      else if (velx == 2.2) {
        velx = -2.2;
      }

      if (vely == -3) {
        vely = 3;
      }
      else if (vely == 3) {
        vely = -3;
      }
      tone(PEIZO, 50, 10);
      player1score++;
    }

    else if (ballx < 1) {
      ballx = 64;
      bally = 32;

      if (velx == -2.2) {
        velx = 2.2;
      }
      else if (velx == 2.2) {
        velx = -2.2;
      }

      if (vely == -3) {
        vely = 3;
      }
      else if (vely == 3) {
        vely = -3;
      }
      tone(PEIZO, 50, 10);
      player2score++;
    }

    if (player1score > 9){
      display.clearDisplay();
      display.setCursor(28, 15);
      display.setTextSize(2);
      display.setTextColor(WHITE);
      display.println("Player 1");
      display.setCursor(35, 45); 
      display.println("Wins!");
      startgame = 0;
    }
    if (player2score > 9){
      display.clearDisplay();
      display.setCursor(28, 15);
      display.setTextSize(2);
      display.setTextColor(WHITE);
      display.println("Player 2");
      display.setCursor(35, 45); 
      display.println("Wins!");
      startgame = 0;
    }
  }
  display.display();
  delay(10);
}
