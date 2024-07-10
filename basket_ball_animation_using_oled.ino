#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Ball properties
float ballX = 20;
float ballY = 40;
float ballSpeedX = 2;
float ballSpeedY = -3;
const float ballSize = 3;
const float gravity = 0.2;
const float damping = 0.8; 
const float friction = 0.99; 

// Hoop properties
const int hoopX = 100;
const int hoopY = 15; 
const int hoopWidth = 10;
const int hoopHeight = 2;
const int playerX = 20;
const int playerY = 50;
bool shooting = false;
int score = 0;

void drawHoop(int x, int y) {
  display.drawRect(x, y, 2, 20, SSD1306_WHITE); 
  display.drawRect(x - hoopWidth, y + 10, hoopWidth, hoopHeight, SSD1306_WHITE); 
  int netHeight = 10;
  int netStartX = x - hoopWidth;
  int netStartY = y + 10 + hoopHeight;
  for (int i = 0; i <= hoopWidth; i += 2) {
    display.drawLine(netStartX + i, netStartY, netStartX + i, netStartY + netHeight, SSD1306_WHITE);
  }
  for (int i = 0; i <= netHeight; i += 2) {
    display.drawLine(netStartX, netStartY + i, netStartX + hoopWidth, netStartY + i, SSD1306_WHITE);
  }
}

void drawPlayer(int x, int y) {
  display.drawLine(x, y, x, y - 10, SSD1306_WHITE); 
  display.drawCircle(x, y - 12, 2, SSD1306_WHITE); 
  display.drawLine(x, y - 8, x - 5, y - 12, SSD1306_WHITE);
  display.drawLine(x, y - 8, x + 5, y - 12, SSD1306_WHITE); 
  display.drawLine(x, y, x - 3, y + 10, SSD1306_WHITE); 
  display.drawLine(x, y, x + 3, y + 10, SSD1306_WHITE); 
}

void drawScore(int score) {
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("Score: ");
  display.print(score);
}

void setup() {
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.clearDisplay();
  display.display();
}

void loop() {
  display.clearDisplay();
  drawHoop(hoopX, hoopY);
  drawPlayer(playerX, playerY);

  if (!shooting) {
    static int shootTimer = 0;
    shootTimer++;
    if (shootTimer > 100) {
      shooting = true;
      shootTimer = 0;
      ballX = playerX; 
      ballY = playerY - 10;
      ballSpeedX = 2; 
      ballSpeedY = -3;
    }
  } else {
    ballSpeedY += gravity;
    ballX += ballSpeedX;
    ballY += ballSpeedY;

    if (ballX <= ballSize || ballX >= (SCREEN_WIDTH - ballSize)) {
      ballSpeedX = -ballSpeedX * damping;
      ballX = constrain(ballX, ballSize, SCREEN_WIDTH - ballSize);
    }
    if (ballY <= ballSize || ballY >= (SCREEN_HEIGHT - ballSize)) {
      ballSpeedY = -ballSpeedY * damping;
      ballY = constrain(ballY, ballSize, SCREEN_HEIGHT - ballSize);
    }

    ballSpeedX *= friction;
    ballSpeedY *= friction;

    display.fillCircle(ballX, ballY, ballSize, SSD1306_WHITE);
    display.drawLine(ballX - ballSize, ballY, ballX + ballSize, ballY, SSD1306_BLACK);
    display.drawLine(ballX, ballY - ballSize, ballX, ballY + ballSize, SSD1306_BLACK);
    display.drawCircleHelper(ballX, ballY, ballSize, 0b0010, SSD1306_BLACK);
    display.drawCircleHelper(ballX, ballY, ballSize, 0b1000, SSD1306_BLACK);

    if (ballX >= hoopX - hoopWidth && ballX <= hoopX && ballY >= hoopY + 10 && ballY <= hoopY + 12) {
      score++;
      shooting = false;
      ballX = playerX; 
      ballY = playerY - 10;
      ballSpeedX = 0;
      ballSpeedY = 0;
    }
  }
  drawScore(score);

  display.display();
  delay(20);
}

