#include <iostream>
#include <set>
#include <vector>
#include <string>
#include "game.hpp"
Game::~Game()
{
  _game_screen.RemoveMouseEventListener(*this);
  _game_screen.RemoveAnimationEventListener(*this);
  // delete the memory that was allocated for the game pieces
  delete _ball;
  delete _paddle;
  delete _score;
  delete _state;
  for (int i = 0; i < _bricks.size(); i++)
  {
    delete _bricks[i];
  }
  _bricks.clear();
}
void Game::initialize()
{
  _game_screen.Initialize(IMAGE_SIZE, IMAGE_SIZE);
  // allocate memory for the `_paddle` and set its initial (X,Y) location
  // set X to place the paddle in the middle of the screen, and set Y to
  // just above the bottom of the screen
  _paddle = new Paddle(PADDLE_HEIGHT, &_game_screen);
  _paddle->setX((IMAGE_SIZE - _paddle->getWidth()) / 2);
  _paddle->setY(IMAGE_SIZE - PADDLE_HEIGHT * 3);
  // allocate memory for the `_ball` and set its initial (X,Y) location
  // set X and Y to place the ball in the middle of the screen
  _ball = new Ball(BALL_DIAMETER, &_game_screen, COLOR_PURPLE);
  _ball->setX((IMAGE_SIZE - BALL_DIAMETER) / 2);
  _ball->setY(IMAGE_SIZE / 2);

  //allocate memory for _state

  _state = new Text(STATE_FONT_SIZE, &_game_screen, COLOR_BLUE);
  _state->setText(INITIAL_TEXT);
  _state->setX(INITIAL_TEXT_X);
  _state->setY(IMAGE_SIZE/2 - BRICK_WIDTH);

  //allocate memory for _score

  _score = new Score(SCORE_FONT_SIZE, &_game_screen, HIGH_SCORE_FILE);



  // create a vector of colors
  // these will be used to color the rows of bricks
  std::vector<graphics::Color> colors;
  colors.push_back(COLOR_RED);
  colors.push_back(COLOR_ORANGE);
  colors.push_back(COLOR_GREEN);
  colors.push_back(COLOR_TEAL);
  colors.push_back(COLOR_BLUE);
  // fill the vector of bricks
  // set the X and Y of each brick to space them out in 5 rows at the top
  // of the game screen
  for (int i = 0; i < IMAGE_SIZE / (BRICK_WIDTH + BRICK_SPACING); i++)
  {
    for (int j = 0; j < BRICK_ROWS; j++)
    {
      graphics::Color color = colors[j % BRICK_ROWS];
      Brick * brick =
        new Brick(BRICK_WIDTH, BRICK_HEIGHT, &_game_screen, color);
      brick->setX(i * (BRICK_WIDTH + BRICK_SPACING));
      brick->setY((j + EMPTY_ROWS) * (BRICK_HEIGHT + BRICK_SPACING));
      _bricks.push_back(brick);
    }
  }
  // draw all objects onto the game screen
  draw();
  // add listener events
  _game_screen.AddMouseEventListener(*this);
  _game_screen.AddAnimationEventListener(*this);
}
void Game::OnMouseEvent(const graphics::MouseEvent & event)
{
  // change state if the game is just beginning
  if (_game_state == NOT_STARTED &&
      event.GetMouseAction() == graphics::MouseAction::kReleased)
  {
    _game_state = IN_PROGRESS;
    _state->setText("");
  }
  // set the new paddle location according to the mouse event
  if (event.GetX() + _paddle->getWidth() <= _game_screen.GetWidth())
  {
    _paddle->setX(event.GetX());
  }

  

}
void Game::moveGamePieces()
{
  // calculate the next location of the ball
  int end_x = _ball->getX() + _speed_x;
  int end_y = _ball->getY() + _speed_y;
  int step_x = (_speed_x < 0 ? -1 : 1);
  int step_y = (_speed_y < 0 ? -1 : 1);
  bool done = false;
  while (!done)
  {
    // move the X and Y locations for the ball one step at a time to be sure
    // it doesn't pass through a wall or another object on the game screen
    // note: checking these separately from each other in case their speeds
    // are set to different values (e.g., maybe the ball is set to move in
    // the X direction at a faster rate than in the Y direction); however,
    // this is not necessary with the current design set to keep them both
    // moving at the same pace
    if (_ball->getX() != end_x)
    {
      _ball->setX(_ball->getX() + step_x);
    }
    if (_ball->getY() != end_y)
    {
      _ball->setY(_ball->getY() + step_y);
    }
    // stop this move and change X direction if the ball hits a side wall
    if (_ball->getX() + _ball->getWidth() >= _game_screen.GetWidth() ||
        _ball->getX() <= 0)
    {
      _speed_x *= -1;
      done = true;
    }
    // stop this move and change Y direction if the ball hits the paddle
    // or the top of the game screen
    else if (_ball->getY() <= 0 || _ball->collidesWith(_paddle))
    {
      _speed_y *= -1;
      done = true;
    }
    // stop this move if ball has hit the bottom of the game screen
    else if (_ball->getY() + _ball->getHeight() >= _game_screen.GetHeight())
    {
      done = true;
    }
    // stop this move if "one full move" worth of steps has completed
    else if (_ball->getX() == end_x && _ball->getY() == end_y)
    {
      done = true;
    }
  }
}
void Game::checkGameState()
{
  // check whether any bricks have been hit by the ball
  for (auto it = _bricks.begin(); it != _bricks.end(); )
  {
    if (_ball->collidesWith(*it))
    {
      it = _bricks.erase(it);
      _speed_y *= -1;
      _score++;
    }
    else
    {
      it++;
    }
  }
  // if all bricks have been removed, then player won
  if (_bricks.size() == 0)
  {
    _game_state = WON;
  }
  // if ball has hit the bottom of the game screen, then player lost
  else if (_ball->getY() + _ball->getHeight() >= _game_screen.GetHeight())
  {
    _game_state = LOST;
  }

  if (_game_state == WON)
  {
    _state->setX(PLAYER_WON_TEXT_X);
    _state->setText(PLAYER_WON_TEXT);
  }
  else if (_game_state == LOST)
  {
    _state->setX(PLAYER_LOST_TEXT_X);
    _state->setText(PLAYER_LOST_TEXT);
  }



}
void Game::OnAnimationStep()
{
  if (_game_state != IN_PROGRESS)
  {
    return;
  }
  moveGamePieces();
  checkGameState();
  // redraw the game screen
  draw();
}
void Game::draw()
{
  // erase everything
  _game_screen.DrawRectangle(
    0, 0, _game_screen.GetHeight(), _game_screen.GetWidth(), COLOR_WHITE);
  // then redraw with latest values for all objects on the screen
  // note: the x/y location of the ball would have changed, and in addition,
  // bricks could have been removed
  _ball->draw();
  _paddle->draw();
  for (int i = 0; i < _bricks.size(); i++)
  {
    _bricks[i]->draw();
  }
  // flush the new image onto the screen
  _game_screen.Flush();

  _state->draw();
  _score->draw();
}