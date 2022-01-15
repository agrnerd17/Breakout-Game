#ifndef __GAME_HPP__
#define __GAME_HPP__
#include <string>
#include <vector>
#include "tools/cpputils/graphics/image.h"
#include "objects.hpp"
//------------------------
// global constants
//------------------------
const int IMAGE_SIZE = 500;
const int BALL_DIAMETER = 18;
const int PADDLE_HEIGHT = 20;
const int BRICK_HEIGHT = 20;
const int BRICK_WIDTH = 48;
const int BRICK_SPACING = 2;
const int BRICK_ROWS = 5;
const int EMPTY_ROWS = 2;
const int INITIAL_SPEED = 7; // note: must be non-zero
const int STATE_FONT_SIZE = 50;
const int SCORE_FONT_SIZE = 25;
const std::string HIGH_SCORE_FILE = "high_score.txt";
const std::string INITIAL_TEXT = "CLICK TO START";
const std::string PLAYER_LOST_TEXT = "GAME OVER";
const std::string PLAYER_WON_TEXT = "YOU WON";
const int INITIAL_TEXT_X = 80;
const int PLAYER_LOST_TEXT_X = 130;
const int PLAYER_WON_TEXT_X = 150;
//-----------------------------
// class declarations
//-----------------------------
// Runs a Breakout game.
// You do not need to modify this code.
class Game : public graphics::MouseEventListener,
             public graphics::AnimationEventListener
{
private:
  // Represents the state of the game.
  enum GameState
  {
    NOT_STARTED = 1,
    IN_PROGRESS = 2,
    LOST = 3,
    WON = 4
  };
  Ball * _ball;
  Paddle * _paddle;
  std::vector<Brick *> _bricks;
  graphics::Image _game_screen;

  //new data members
  Score * _score;
  Text * _state;

  // speed will be the same for x/y but one could be traveling in a negative
  // direction while the other is traveling in a positive direction
  int _speed_x;
  int _speed_y;
  GameState _game_state;
  // helper methods
  void draw();
  void checkGameState();
  void moveGamePieces();
public:
  Game() : _ball(nullptr), _paddle(nullptr), _game_state(NOT_STARTED),
           _speed_x(INITIAL_SPEED), _speed_y(INITIAL_SPEED) {}

  // constructor
  Game(Score * score, Text * state) : _score(nullptr), _state(nullptr) {}

  ~Game();
  // get ready to play
  void initialize();
  // begin the event loop by showing the game screen image
  void start() { _game_screen.ShowUntilClosed("Breakout"); }
  // gets mouse events from the game screen image
  void OnMouseEvent(const graphics::MouseEvent & event) override;
  // gets animation events from the game screen image
  void OnAnimationStep() override;
};
#endif