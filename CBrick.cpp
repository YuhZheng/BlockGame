#include "typedef.h"
/*
class CBrick {
      const static unsigned char brickType[SHAPE_NUM];
protected:
      static CBoard &board;
      short  x,y;
      unsigned char color, direction;
      virtual void  erase()=0;
      virtual void  show()=0;
      virtual bool isProped()=0;
public:
      short  score;
      CBrick(short x_, short y_, char color_, unsigned char direction_);
      virtual ~CBrick();
      static CBrick* newBrick();
      void stop();
      int  mvDown(int command=UNDEFINED);
      virtual int  mvLeft()=0;
      virtual int  mvRight()=0; 
      virtual int  rotateLeft();
      virtual int  rotateRight();
friend class CDisplay;
};
*/
CBrick::CBrick(short x_, short y_, char color_, unsigned char direction_){
     x0 = x = x_;
     y0 = y = y_;
     color = color_ % (COLOR_NUM+1);
     iniDirection = direction = direction_;
     return;
}

void CBrick::stop(){
     score += board<<(this);
     return;
}

int CBrick::mvDown(int command){
    int mvSteps=0;
    while ( isProped()==false ) {
          mvSteps++;
          if ( mvSteps==1 ) erase();
          y--;
          if (command==UNDEFINED) break;
    } 
    if ( mvSteps==0 ) return STOPPED;
    show();
    return MV_DOWN;
}

int CBrick::rotateLeft(){
     return mvDown();
}

int CBrick::rotateRight(){
     return mvDown();
}


CBrick* CBrick::newBrick(){
     CBrick* result=NULL;     
     int color = rand()%COLOR_NUM + 1;
     unsigned short curBrickType = rand()%SHAPE_NUM;
     
     if ( board.getRim(6, 30)&LUTRIANGLE&RUTRIANGLE ) return result;
     switch ( brickType[curBrickType] ) {
     case SQUARE:
          result = new CSquare(6, 30, color, brickType[curBrickType]);
          break;
     case HRECTANGLE:
     case VRECTANGLE:
          result = new CRectangle(6, 30, color, brickType[curBrickType]);
          break;
     case LUTRIANGLE:
     case RUTRIANGLE:
     case RDTRIANGLE:
     case LDTRIANGLE:
          if ( board.getRim(6, 30)&brickType[curBrickType] ) return newBrick();//??
          result = new CTriangle(6, 30, color, brickType[curBrickType]);
          break;          
     case HUTRAPEZIUM:
     case HDTRAPEZIUM:
     case VRTRAPEZIUM:
     case VLTRAPEZIUM:
          result = new CTrapezium(6, 30, color, brickType[curBrickType]);
          break;
     default:
           break;
     }
     if (result!=NULL) result->show();
     return result;
}

int CBrick::getRowofCenter(){
    return y;
}

void CBrick::drop(int height){
    erase();
    y = y - height;
    show();
}
/******CBrick::deleteBrick()计分仍需修改，未设初始分***********/
int CBrick::deleteBrick(){
    int result = score;
    int s;
    s = abs(x-x0) + abs(y-y0);
    if(direction != iniDirection){
        switch(direction){
            case SQUARE:
                break;
            case HRECTANGLE:
            case VRECTANGLE:
                s += 1;
                break;
            case LUTRIANGLE:
                if(iniDirection == RDTRIANGLE) s += 2;
                else s += 1;
                break;
            case RUTRIANGLE:
                if(iniDirection == LDTRIANGLE) s += 2;
                else s += 1;
                break;
            case RDTRIANGLE:
                if(iniDirection == LUTRIANGLE) s += 2;
                else s += 1;
                break;
            case LDTRIANGLE:
                if(iniDirection == RUTRIANGLE) s += 2;
                else s += 1;
                break;         
            case HUTRAPEZIUM:
                if(iniDirection == HDTRAPEZIUM) s += 2;
                else s += 1;
                break;
            case HDTRAPEZIUM:
                if(iniDirection == HUTRAPEZIUM) s += 2;
                else s += 1;
                break;
            case VRTRAPEZIUM:
                if(iniDirection == VLTRAPEZIUM) s += 2;
                else s += 1;
                break;
            case VLTRAPEZIUM:
                if(iniDirection == VRTRAPEZIUM) s += 2;
                else s += 1;
                break;
            default:
                break;
        }
    }   
    if(s> step) result += s - step;
    erase();
    return result;
}

bool CBrick::isVertical(){
    return false;
}

CBrick::~CBrick() {
}
