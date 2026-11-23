#include <gamma.h>
#include <RGBmatrixPanel.h>
#include <Adafruit_GFX.h>

// define the wiring of the LED screen
const uint8_t CLK  = 8;
const uint8_t LAT = A3;
const uint8_t OE = 9;
const uint8_t A = A0;
const uint8_t B = A1;
const uint8_t C = A2;

// define the wiring of the inputs
const int POTENTIOMETER_PIN_NUMBER = 5;
const int BUTTON_PIN_NUMBER = 10;

// global constant for the number of Invaders in the game
const int NUM_ENEMIES = 16;

// a global variable that represents the LED screen
RGBmatrixPanel matrix(A, B, C, CLK, LAT, OE, false);

// the following functions are for printing messages
void print_level(int level);
void print_lives(int lives);
void game_over();

class Color {
  public:
    int red;
    int green;
    int blue;
    Color() {
      red = 0;
      green = 0;
      blue = 0;
    }
    Color(int r, int g, int b) {
      red = r;
      green = g;
      blue = b;
    }
    uint16_t to_333() const {
      return matrix.Color333(red, green, blue);
    }
};

const Color BLACK(0, 0, 0);
const Color RED(4, 0, 0);
const Color ORANGE(6, 1, 0);
const Color YELLOW(4, 4, 0);
const Color GREEN(0, 4, 0);
const Color BLUE(0, 0, 4);
const Color PURPLE(1, 0, 2);
const Color WHITE(4, 4, 4);
const Color LIME(2, 4, 0);
const Color AQUA(0, 4, 4);

class Invader {
  public:
    // Constructors
    Invader() {
      x = 0;
      y = 0;
      strength = 0;
    }
    // sets values for private date members x and y
    Invader(int x_arg, int y_arg) {
    }
    // sets values for private data members
    Invader(int x_arg, int y_arg, int strength_arg) {
    }
    // sets values for private data members
    void initialize(int x_arg, int y_arg, int strength_arg) {
    }
    
    // getters
    int get_x() const {
    }
    int get_y() const {
    }
    int get_strength() const {
    }

    // Moves the Invader down the screen by one row
    // Modifies: y
    void move() {
    }
    
    // draws the Invader if its strength is greater than 0
    // calls: draw_with_rgb
    void draw() {
    }
    
    // draws black where the Invader used to be
    // calls: draw_with_rgb
    void erase() {
    }    
    
    // Invader is hit by a Cannonball.
    // Modifies: strength
    // calls: draw, erase
    void hit() {
    }

  private:
    int x;
    int y;
    int strength;
    
    // draws the Invader
    void draw_with_rgb(Color body_color, Color eye_color) {
    }
};

class Cannonball {
  public:
    Cannonball() {
      x = 0;
      y = 0;
      fired = false;
    }
    
    // resets private data members to initial values
    void reset() {
    }
    
    // getters
    int get_x() const {
    }
    int get_y() const {
    }
    bool has_been_fired() const {
    }
    
    // sets private data members
    void fire(int x_arg, int y_arg) {
    }
    
    // moves the Cannonball and detects if it goes off the screen
    // Modifies: y, fired
    void move() {
    }
    
    // resets private data members to initial values
    void hit() {
    }
    
    // draws the Cannonball, if it is fired
    void draw() {
    }
    
    // draws black where the Cannonball used to be
    void erase() {
    }

  private:
    int x;
    int y;
    bool fired;
};

class Player {
  public:
    Player() {
      x = 0;
      y = 0;
      lives = 3;
    }
    
    // getters
    int get_x() const {
    }
    int get_y() const {
    }
    int get_lives() const {
    }
    
    // setter
    void set_x(int x_arg) {
    }
    
    // Modifies: lives
    void die() {
    }
    
    // draws the Player
    // calls: draw_with_rgb
    void draw() {
    }
    
    // draws black where the Player used to be
    // calls: draw_with_rgb
    void erase() {
    }

  private:
    int x;
    int y;
    int lives;

    // sets private data members x and y to initial values
    void initialize(int x_arg, int y_arg) {
    }
    
    // draws the player
    void draw_with_rgb(Color color) {
    }
};

class Game {
  public:
    Game() {
      level = 1;
      time = 0;
    }
    
    // sets up a new game of Space Invaders
    // Modifies: global variable matrix
    void setupGame() {
      
      reset_level();

      while(!player_lost()) {
        loop();
      }

      game_over();

    }
    
    // advances the game simulation one step and renders the graphics
    // see spec for details of game
    // Modifies: global variable matrix
    void update(int potentiometer_value, bool button_pressed) {

      // get current time
      time = millis();

      // updating was_button_pressed
      if (button_pressed) {
        was_button_pressed = true;
      }

      // updating the frame
      if (time >= stop_time + FRAME_DELAY) {
        stop_time += FRAME_DELAY;
        frame++;



        // checking if level cleared
        if (level_cleared()) {
          level++;
          reset_level();
        }

        // checking if player died / game over
        if (player_got_hit()) {
          player.die();
          if (player_lost()) {
            return;
          } 
          else {
            reset_level();
          }
        }
          

        // firing if button was pressed
        if (was_button_pressed) {
          ball.fire(player.get_x(), player.get_y());
          was_button_pressed = false;
        }

        // updating the ball
        if (ball.has_been_fired()) {
          ball.erase();
          ball.move();
          ball.draw();
        }

        // hit invader
        if (invader_got_hit() != DID_NOT_HIT_INVADER) {
          enemies[invader_got_hit()].hit();
        }



        // assigning a move score
        if (potentiometer_value - AVG_POTENTIOMETER_VALUE > DEADZONE_AMOUNT) {
          move_score = (potentiometer_value - AVG_POTENTIOMETER_VALUE - DEADZONE_AMOUNT) / MOVE_WEIGHT;
        }
        else if (potentiometer_value - AVG_POTENTIOMETER_VALUE < -DEADZONE_AMOUNT) {
          move_score = (potentiometer_value - AVG_POTENTIOMETER_VALUE + DEADZONE_AMOUNT) / MOVE_WEIGHT;
        }
        else {
          move_score = 0;
        }

        if (move_score != 0) {
          // move
          player.set_x(move_score + player.get_x());

          // print after movement
          player.erase();
          player.move();
          player.draw();
        }



        // invaders move
        if (frame % INVADER_MOVE == 0) {
          invaders_move();
        }

      }

      return;

    }

  private:
    int level;
    unsigned long time;
    unsigned long stop_time = 0;
    int frame = 1;
    Player player;
    Cannonball ball;
    Invader enemies[NUM_ENEMIES];
    const int FRAME_DELAY = 500;
    const int DEADZONE_AMOUUNT = 25;
    const int AVG_POTENTIOMETER_VALUE = 500;
    const int MOVE_WEIGHT = 25;
    const int DID_NOT_HIT_INVADER = -1;
    const int INVADER_MOVE = 16;
    bool was_button_pressed = false
    int move_score = 0;

    // check if Player defeated all Invaders in current level
    bool level_cleared() {
      for (int i = 0; i < NUM_ENEMIES; i++) {
        if (enemies[i].get_strength() != 0 ) {
            return false;
        }
      }
      return true;
    }

    // checking if player lost the game
    bool player_lost() {
      return lives == 0;
    }

    // checking if player was hit
    bool player_got_hit() {
      for (int i = 0; i < NUM_ENEMIES; i++) {
        if (invader_hit_player(i)) {
          return true;
        }
      }
      return false;
    }

    // FIX ME -- HOW TO DETECT COLLISION
    bool invader_hit_player(int index) {
      if (enemies[i].get_strength() == 0) {
        return false;
      }
      else {
        return enemies[i].get_y() == player.get_y();
      }
    }

    // finding which invader was hit
    int invader_got_hit() {
      for (int i = 0; i < NUM_ENEMIES; i++) {
        if (ball_hit_invader(i)) {
          return i;
        }
      }
      return DID_NOT_HIT_INVADER;
    }

    // FIX ME -- HOW TO DETECT COLLISION
    bool ball_hit_invader(int index) {
      if (enemies[i].get_strength() == 0) {
        return false;
      }
      else {
        return (enemies[index].get_y() == ball.get_y()) && (enemies[index].get_x() == ball.get_x());
      }
    }

    // checking if the first row is gone
    bool row1_is_gone() {
      for (int i = NUM_ENEMIES / 2; i < NUM_ENEMIES; i++) {
        if (enemies[i].get_strength() != 0) {
          return false;
        }
      }
      return true;
    }

    // invaders move
    void invaders_move() {
      if (!row1_is_gone()) {
        for (int i = NUM_ENEMIES / 2; i < NUM_ENEMIES; i++) {
          enemies[i].erase();
          enemies[i].move();
          enemies[i].draw();
        }
      }
      else {
        for (int i = 0; i < NUM_ENEMIES / 2; i++) {
          enemies[i].erase();
          enemies[i].move();
          enemies[i].draw();
        }
      }
    }

    // set up a level
    void reset_level() {
      print_level(level);
      print_lives(player.get_lives());

      // setting up the invaders
      if (level == 1) {
        // row 1
        for (int i = 0; i < NUM_ENEMIES / 2; i++) {
          enemies[i].initialize(4i,0,1);
        }

        // row 2
        for (int i = 0; i < NUM_ENEMIES / 2; i++) {
          enemies[i + NUM_ENEMIES / 2].initialize(4i,4,0);
        }
      }
      else if (level == 2) {
        // row 1
        for (int i = 0; i < NUM_ENEMIES / 2; i++) {
          enemies[i].initialize(4i,0,1 + i % 2);
        }

        // row 2
        for (int i = 0; i < NUM_ENEMIES / 2; i++) {
          enemies[i + NUM_ENEMIES / 2].initialize(4i,4,2 - i % 2);
        }
      }
      else if (level == 3) {
        // row 1
        for (int i = 0; i < NUM_ENEMIES / 2; i++) {
          enemies[i].initialize(4i,0,1 + i % 5);
        }

        // row 2
        for (int i = 0; i < NUM_ENEMIES / 2; i++) {
          enemies[i + NUM_ENEMIES / 2].initialize(4i,4,1 + (i + 3) % 5);
        }
      }
      else if (level == 4) {
        // row 1
        for (int i = 0; i < NUM_ENEMIES / 2; i++) {
          enemies[i].initialize(4i,0,5 - i % 2);
        }

        // row 2
        for (int i = 0; i < NUM_ENEMIES / 2; i++) {
          enemies[i + NUM_ENEMIES / 2].initialize(4i,4,2 + i % 2);
        }
      }
      else {
        // row 1
        for (int i = 0; i < NUM_ENEMIES / 2; i++) {
          enemies[i].initialize(4i,0,random(1,7));
        }

        // row 2
        for (int i = 0; i < NUM_ENEMIES / 2; i++) {
          enemies[i + NUM_ENEMIES / 2].initialize(4i,4,random(1,7));
        }
      }

      // print the invaders
      for (int i = 0; i < NUM_ENEMIES; i++) {
        enemies[i].draw();
      }

      // setting up the player
      player.initialize(0, 14);

      // print the player
      player.draw();

    }
};

// a global variable that represents the game Space Invaders
Game game;

// see https://www.arduino.cc/reference/en/language/structure/sketch/setup/
void setup() {
  Serial.begin(9600);
  pinMode(BUTTON_PIN_NUMBER, INPUT);
  matrix.begin();
  game.setupGame();
}

// see https://www.arduino.cc/reference/en/language/structure/sketch/loop/
void loop() {
  int potentiometer_value = analogRead(POTENTIOMETER_PIN_NUMBER);
  bool button_pressed = (digitalRead(BUTTON_PIN_NUMBER) == HIGH);

  game.update(potentiometer_value, button_pressed);
}

// displays Level
void print_level(int level) {
  lcd.setCursor(0,0);
  lcd.print("Level: ");
  lcd.print(level);
  delay(1000);
  fillScreen(BLACK);
}

// displays number of lives
void print_lives(int lives) {
  lcd.setCursor(0,0);
  lcd.print("Lives: ");
  lcd.print(lives);
  delay(1000);
  fillScreen(BLACK);
}

// displays "game over"
void game_over() {
  lcd.setCursor(0,0);
  lcd.print("Game Over");
}

