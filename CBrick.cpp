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
     x = x_;
     y = y_;
     color = color_ % (COLOR_NUM+1);
     direction = direction_;
     return;
}

void CBrick::stop(){

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

CBrick::~CBrick() {
}
