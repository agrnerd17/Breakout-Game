#ifndef __OBJECTS_HPP__
#define __OBJECTS_HPP__


#include "tools/cpputils/graphics/image.h"

//------------------------
// global constants
//------------------------

const graphics::Color COLOR_BLACK   (   0,   0,   0 );
const graphics::Color COLOR_GRAY    (  50,  50,  50 );
const graphics::Color COLOR_WHITE   ( 255, 255, 255 );
const graphics::Color COLOR_RED     ( 255,   0,   0 );
const graphics::Color COLOR_DARKRED ( 210,  25,  25 );
const graphics::Color COLOR_ORANGE  ( 255, 127,   0 );
const graphics::Color COLOR_YELLOW  ( 255, 255,   0 );
const graphics::Color COLOR_GREEN   (   0, 200, 100 );
const graphics::Color COLOR_TEAL    (   0, 125, 255 );
const graphics::Color COLOR_BLUE    (   0,   0, 255 );
const graphics::Color COLOR_PURPLE  ( 127,   0, 255 );

//---------------------------
// class declarations
//---------------------------

// TODO
// add the class SolidObject

class SolidObject
{

private:

int _x_loc = 0;

int _y_loc = 0;

int _width;

int _height;

graphics::Image *_image;

graphics::Color _color;

public:

SolidObject (int width, int height, graphics::Image *image, 
const graphics::Color & COLOR_BLACK) : _width(width), _height(height), _image(image),
_color(COLOR_BLACK) {}


//accessors for x & y locations:

int getX() {return _x_loc;}

int getY() {return _y_loc;}

//new accessor

const graphics::Color & getColor() {return _color;}


//mutators for x & y:

void setX(int x) {_x_loc = x;}
void setY(int y) {_y_loc = y;}

//accessors for width & height:

int getWidth() {return _width;}

int getHeight() {return _height;}

//member function:

bool collidesWith( SolidObject * const obj) const;

//virtual function

virtual void draw() = 0;



//protected accessor

protected:

graphics::Image * getImage() {return _image;}



};

class Ball :
    public SolidObject
    {
        public:


Ball(int diameter, graphics::Image *ball_image, 
const graphics::Color & ball_color = COLOR_BLACK) : 
SolidObject(diameter,diameter, ball_image, ball_color) {}



inline void draw() 
{

    getImage()->DrawCircle(getX()+getWidth()/2, getY()+getHeight()/2, getWidth()/2, getColor());
}



    };


class Brick :
    public SolidObject
    {
        public: 

        Brick(int width, int height, graphics::Image * brick_image, const graphics::Color & brick_color = COLOR_RED):
        SolidObject(width, height, brick_image, brick_color) {}

        inline void draw() 
        {
            getImage()->DrawRectangle(getX(), getY(), getWidth(), getHeight(), getColor());

        }

    };

class Paddle :
    public Brick 
    {
        public:

        Paddle(int height, graphics::Image * paddle_image, const graphics::Color & paddle_color = COLOR_GRAY):
        Brick(4 * height, height, paddle_image, paddle_color) {}

    };

class Text :
public SolidObject
{
private:
std::string _text;

public:

Text(int font_size, graphics::Image * text_image, const graphics::Color & text_color = COLOR_BLACK, const std::string & text = "") 
: SolidObject(font_size, font_size, text_image, text_color) {}

//mutator
void setText(std::string text) {_text = text;}


//other member functions

void draw() override
{

    getImage()->DrawText(getX(), getY(), _text, getHeight(), getColor());
}

};

class Score :
public Text
{
private:

const int SCORE_LOCATION = 5;

int _score;

int _high_score;

std::string _filename;


void saveHighScore();
void updateText();
void initialize();

public:

Score(int font_size, graphics::Image * _image, const std::string & high_score,const graphics::Color & _color = COLOR_BLACK)
: 
Text(font_size, _image, _color),
_score(0), 
_high_score(0), 
_filename(high_score)
{
    initialize();
}

~Score() {saveHighScore();}

//Score accessor
int getHighScore() {return _high_score;}

//Score member functions

void increment();

};



#endif