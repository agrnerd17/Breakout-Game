#include <sstream>
#include <fstream>
#include "objects.hpp"

 bool SolidObject::collidesWith( SolidObject * const obj) const
{

    if(obj->getWidth() == 0 || obj->getHeight() == 0 
    || _width == 0 || _height == 0)
    {
        return false;
    }
    else if (obj->getX() + obj->getWidth() < _x_loc)
    {
        return false;
    }
    else if (_x_loc + _width < obj->getX())
    {
        return false;
    }
    else if (obj->getY() + obj->getHeight() < _y_loc)
    {
        return false;
    }
    else if (_y_loc + _height < obj->getY())
    {
        return false;
    }
    else
    {
        return true;
    }
}

//new functions
void Score::updateText() 
{

std::stringstream ss;
std::string space;

  space.assign(SCORE_LOCATION * 5, ' ');
   ss << "Score: " << _score << space << "High Score: " << _high_score;
   setText(space);
}

void Score::initialize()
{
    setX(SCORE_LOCATION * 2);
    setY(SCORE_LOCATION);

    std::fstream highscore;
    highscore.open(_filename, std::ios_base::in);

    if(highscore.fail())
    {
        return;
    }
    else
    {
        highscore.close();
    }

}

void Score::increment()
{
    _score++;
    if(_score > _high_score)
    {
        _high_score++;
    }
    updateText();
}

void Score::saveHighScore()
{
    std::fstream _highscore;
    _highscore.open(_filename, std::ios_base::in);
    _highscore.close();
}

