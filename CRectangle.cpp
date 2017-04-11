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

class CRectangle:public CBrick{
      virtual void  erase();
      virtual void  show();
      virtual bool  isProped();
public:
      CRectangle(short x_, short y_, char color_, unsigned char direction_);
      virtual int  mvLeft();
      virtual int  mvRight(); 
      virtual int  rotateLeft();
      virtual int  rotateRight();
}; 
*/
CRectangle::CRectangle(short x_, short y_, char color_, unsigned char direction_):CBrick(x_,y_,color_,direction_){
     if ( x_<1 || x_>11 ) x = 6;
     if ( y_>30 || y_<1 ) y = 30;
     switch ( direction_ ) {
            case HRECTANGLE:
                 if ( x_<2 || x_>10 ) x = 6;
                 else x = x_;
                 if ( y_>30 || y_<1 ) y = 30;
                 else y = y_;
                 break;
            case VRECTANGLE:
                 if ( x_<1 || x_>11 ) x = 6;
                 else x = x_;
                 if ( y_>29 || y_<2 ) y = 29;
                 else y = y_;
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

void CRectangle::show(){
     board<<CTriangle(x, y, color, LUTRIANGLE)<<CTriangle(x, y, color, RDTRIANGLE);
     switch ( direction ) {
            case HRECTANGLE:
                 board<<CTriangle(x-1, y, color, LUTRIANGLE)<<CTriangle(x-1, y, color, RDTRIANGLE);
                 board<<CTriangle(x+1, y, color, LUTRIANGLE)<<CTriangle(x+1, y, color, RDTRIANGLE);
                 break;
            case VRECTANGLE:
                 board<<CTriangle(x, y-1, color, LUTRIANGLE)<<CTriangle(x, y-1, color, RDTRIANGLE);
                 board<<CTriangle(x, y+1, color, LUTRIANGLE)<<CTriangle(x, y+1, color, RDTRIANGLE);
                 break;
            default:
                 break;
     }
     
     return;
}

void CRectangle::erase(){
     board>>CTriangle(x, y, 0, LUTRIANGLE)>>CTriangle(x, y, 0, RDTRIANGLE);
     switch ( direction ) {
            case HRECTANGLE:
                 board>>CTriangle(x-1, y, color, LUTRIANGLE)>>CTriangle(x-1, y, color, RDTRIANGLE);
                 board>>CTriangle(x+1, y, color, LUTRIANGLE)>>CTriangle(x+1, y, color, RDTRIANGLE);
                 break;
            case VRECTANGLE:
                 board>>CTriangle(x, y-1, color, LUTRIANGLE)>>CTriangle(x, y-1, color, RDTRIANGLE);
                 board>>CTriangle(x, y+1, color, LUTRIANGLE)>>CTriangle(x, y+1, color, RDTRIANGLE);
                 break;
            default:
                 break;
     }
     return;
}

bool CRectangle::isProped(){
     bool result;
     switch ( direction ) {
            case HRECTANGLE:
                 result = board.getRim(x,y-1) || board.getRim(x-1,y-1) || board.getRim(x+1,y-1);
                 break;
            case VRECTANGLE:
                 result = board.getRim(x,y-2);
                 break;
            default:
                 return UNDEFINED;
     }   
     return result;
}


int CRectangle::mvLeft(){
     bool  stopped;
     switch ( direction ) {
            case HRECTANGLE:
                 stopped = board.getRim(x-2,y);
                 break;
            case VRECTANGLE:
                 stopped = board.getRim(x-1,y-1) || board.getRim(x-1,y+1) || board.getRim(x-1,y) ;
                 break;
            default:
                 return UNDEFINED;
     } 
     if ( stopped ) return mvDown();
     erase();
     x--;
     show();
     return MV_LEFT;

}

int CRectangle::mvRight(){
     bool  stopped;
     switch ( direction ) {
            case HRECTANGLE:
                 stopped = board.getRim(x+2,y);
                 break;
            case VRECTANGLE:
                 stopped = board.getRim(x+1,y-1) || board.getRim(x+1,y+1) || board.getRim(x+1,y) ;
                 break;
            default:
                 return UNDEFINED;
     }
     if ( stopped ) return mvDown();
     erase();
     x++;
     show();
     return MV_RIGHT;
}

int CRectangle::rotateLeft(){
     bool  stopped=true;
     
     switch ( direction ) {
            case VRECTANGLE:
                 if ( board.getRim(x-1,y+1) ) break;
                 if ( board.getRim(x-1,y)) break;
                 if ( board.getRim(x+1,y)) break;
                 if ( board.getRim(x+1, y-1) ) break;                 
                 stopped = false;
                 break;
            case HRECTANGLE:
                 if ( board.getRim(x-1,y-1) ) break;
                 if ( board.getRim(x,y+1)) break;
                 if ( board.getRim(x,y-1)) break;
                 if ( board.getRim(x+1, y+1) ) break;
                 stopped = false;
                 break;
            default:
                 return UNDEFINED;
     } 
     if ( stopped ) return mvDown();
     erase();
     switch (direction){
         case HRECTANGLE:
             direction = VRECTANGLE;
             break;
         case VRECTANGLE:
             direction = HRECTANGLE;
             break;
     }
     show();
     return RT_LEFT;
}

int CRectangle::rotateRight(){
     bool  stopped=true;
     
     switch ( direction ) {
            case VRECTANGLE:
                 if ( board.getRim(x+1,y+1) ) break;
                 if ( board.getRim(x-1,y)) break;
                 if ( board.getRim(x+1,y)) break;
                 if ( board.getRim(x-1, y-1) ) break;                 
                 stopped = false;
                 break;
            case HRECTANGLE:
                 if ( board.getRim(x-1,y+1) ) break;
                 if ( board.getRim(x,y+1)) break;
                 if ( board.getRim(x,y-1)) break;
                 if ( board.getRim(x+1, y-1) ) break;
                 stopped = false;
                 break;
            default:
                 return UNDEFINED;
     } 
     if ( stopped ) return mvDown();
     erase();
     switch (direction){
         case HRECTANGLE:
             direction = VRECTANGLE;
             break;
         case VRECTANGLE:
             direction = HRECTANGLE;
             break;
     }
     show();
     return RT_RIGHT;
}

bool CRectangle::isVertical(){
    if (direction == VRECTANGLE) return true;
    else return false;
}
