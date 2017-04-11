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
      CBrick(short x_, short y_, char color_, unsigned char direction_);
      virtual ~CBrick();
      static CBrick* newBrick();
      int  mvDown(int command=UNDEFINED);
      virtual int  mvLeft()=0;
      virtual int  mvRight()=0; 
      virtual int  rotateLeft();
      virtual int  rotateRight();
friend class CDisplay;
};

class CSquare:public CBrick{
      virtual void  erase();
      virtual void  show();
      virtual bool  isProped();
public:
      CSquare(short x_, short y_, char color_, unsigned char direction_);
      virtual int  mvLeft();
      virtual int  mvRight(); 
}; 
*/
CSquare::CSquare(short x_, short y_, char color_, unsigned char direction_):CBrick(x_,y_,color_,direction_){
     
     if ( x_<1 || x_>11 ) x = 6;
     if ( y_>30 || y_<1 ) y = 30;
     switch ( direction_ ) {
            case SQUARE:
                 break;
            default:
                 x=0;
                 y=0;
                 color = 0;
                 direction = 0;
                 break;
     }
     return;
}

void CSquare::show(){
     board<<CTriangle(x, y, color, LUTRIANGLE)<<CTriangle(x, y, color, RDTRIANGLE);
     return;
}

void CSquare::erase(){
     board>>CTriangle(x, y, 0, LUTRIANGLE)>>CTriangle(x, y, 0, RDTRIANGLE);
     return;
}

bool CSquare::isProped(){
     bool result;
     result = board.getRim(x,y-1);
     return result;

}

int CSquare::mvLeft(){
     bool  stopped;
     stopped = board.getRim(x-1,y);
     if ( stopped ) return mvDown();
     erase();
     x--;
     show();
     return MV_LEFT;

}

int CSquare::mvRight(){
     bool  stopped;
     stopped = board.getRim(x+1,y);
     if ( stopped ) return mvDown();
     erase();
     x++;
     show();
     return MV_RIGHT;
}
