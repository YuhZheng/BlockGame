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

class CTrapezium:public CBrick{
      virtual void  erase();
      virtual void  show();
      virtual bool  isProped();
public:
      CTrapezium(short x_, short y_, char color_, unsigned char direction_);
      virtual int  mvLeft();
      virtual int  mvRight(); 
      virtual int  rotateLeft();
      virtual int  rotateRight();
};
*/
CTrapezium::CTrapezium(short x_, short y_, char color_, unsigned char direction_):CBrick(x_,y_,color_,direction_){
     switch ( direction_ ) {
            case HUTRAPEZIUM:
            case HDTRAPEZIUM:
                 if ( x_<2 || x_>10 ) {
                     x = 6;
//                     CBrick::x = 6;
                 }
                 else x = x_;
                 if ( y_>30 || y_<1 ){
                     y = 30;
//                     CBrick::y = 30;
                 } 
                 else y = y_;
                 break;
            case VRTRAPEZIUM:
            case VLTRAPEZIUM:
                 if ( x_<1 || x_>11 ){
                     x = 6;
//                     CBrick::x = 6;
                 } 
                 else x = x_;
                 if ( y_>29 || y_<2 ){
                     y = 29;
//                     CBrick::y = 29;
                 } 
                 else y = y_;
                 break;
            default:
                 x=0;
                 y=0;
                 color = 0;
                 direction = 0;
                 break;
     }
     show();
     return;
}

void  CTrapezium::show(){
     board<<CTriangle(x, y, color, LUTRIANGLE)<<CTriangle(x, y, color, RDTRIANGLE);;
     switch ( direction ) {
            case HUTRAPEZIUM:
                 board<<CTriangle(x-1, y, color, RDTRIANGLE);
                 board<<CTriangle(x+1, y, color, LDTRIANGLE);
                 break;
            case HDTRAPEZIUM:
                 board<<CTriangle(x-1, y, color, RUTRIANGLE);
                 board<<CTriangle(x+1, y, color, LUTRIANGLE);
                 break;
            case VRTRAPEZIUM:
                 board<<CTriangle(x, y-1, color, LUTRIANGLE);
                 board<<CTriangle(x, y+1, color, LDTRIANGLE);
                 break;
            case VLTRAPEZIUM:
                 board<<CTriangle(x, y-1, color, RUTRIANGLE);
                 board<<CTriangle(x, y+1, color, RDTRIANGLE);
                 break;
            default:
                 break;
     }
     
     return;
}


void  CTrapezium::erase(){
     board>>CTriangle(x, y,0, LUTRIANGLE)>>CTriangle(x, y,0, RDTRIANGLE);
     switch ( direction ) {
            case HUTRAPEZIUM:
                 board>>CTriangle(x-1, y,0, RDTRIANGLE);
                 board>>CTriangle(x+1, y,0, LDTRIANGLE);
                 break;
            case VRTRAPEZIUM:
                 board>>CTriangle(x, y+1,0, LDTRIANGLE);
                 board>>CTriangle(x, y-1,0, LUTRIANGLE);
                 break;
            case HDTRAPEZIUM:
                 board>>CTriangle(x-1, y,0, RUTRIANGLE);
                 board>>CTriangle(x+1, y,0, LUTRIANGLE);
                 break;
            case VLTRAPEZIUM:
                 board>>CTriangle(x, y+1,0, RDTRIANGLE);
                 board>>CTriangle(x, y-1,0, RUTRIANGLE);
                 break;
            default:
                 break;
     } 
     return;
}

bool CTrapezium::isProped(){
     bool  stopped;

     switch ( direction ) {
            case HUTRAPEZIUM:
                 stopped = board.getRim(x,y-1)|board.getRim(x-1,y-1)|board.getRim(x+1,y-1);
                 break;
            case VRTRAPEZIUM:
                 stopped = (board.getRim(x,y-1)&RDTRIANGLE)|(board.getRim(x,y-2)&LUTRIANGLE);
                 break;
            case HDTRAPEZIUM:
                 stopped = (board.getRim(x-1,y)&LDTRIANGLE)|board.getRim(x,y-1);
                 stopped = stopped || (board.getRim(x+1,y)&RDTRIANGLE);
                 stopped = stopped || (board.getRim(x-1,y-1)&RUTRIANGLE);
                 stopped = stopped || (board.getRim(x+1,y-1)&LUTRIANGLE);
                 break;
            case VLTRAPEZIUM:
                 stopped = (board.getRim(x,y-1)&LDTRIANGLE)|(board.getRim(x,y-2)&RUTRIANGLE);
                 break;
            default:
                 return UNDEFINED;
     }      
     return stopped;
}

int CTrapezium::mvLeft(){
    bool  stopped;

     switch ( direction ) {
            case HUTRAPEZIUM:
                 if(x == 2) stopped = 1;
                 else
                     stopped = (board.getRim(x-1,y)&LUTRIANGLE)|(board.getRim(x-2,y)&RDTRIANGLE);
                 break;
            case VRTRAPEZIUM:
                 stopped = board.getRim(x-1,y)|board.getRim(x-1,y-1)|board.getRim(x-1,y+1);
                 break;
            case HDTRAPEZIUM:
                     stopped = (board.getRim(x-1,y)&LDTRIANGLE)|(board.getRim(x-2,y)&RUTRIANGLE);                 
                 break;
            case VLTRAPEZIUM:
                 stopped = (board.getRim(x,y+1)&LUTRIANGLE)|board.getRim(x-1,y);
                 stopped = stopped || (board.getRim(x-1,y+1)&RDTRIANGLE);
                 stopped = stopped || (board.getRim(x-1,y-1)&RUTRIANGLE);
                 stopped = stopped || (board.getRim(x,y-1)&LDTRIANGLE);                 
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

int CTrapezium::mvRight(){
     bool  stopped;

     switch ( direction ) {
            case HUTRAPEZIUM:
                 stopped = (board.getRim(x+1,y)&RUTRIANGLE)|(board.getRim(x+2,y)&LDTRIANGLE);
                 break;
            case VRTRAPEZIUM:
                 stopped = (board.getRim(x,y+1)&RUTRIANGLE)|board.getRim(x+1,y);
                 stopped = stopped || (board.getRim(x+1,y+1)&LDTRIANGLE);
                 stopped = stopped || (board.getRim(x+1,y-1)&LUTRIANGLE);
                 stopped = stopped || (board.getRim(x,y-1)&RDTRIANGLE);                 
                 break;
            case HDTRAPEZIUM:
                 stopped = (board.getRim(x+1,y)&RDTRIANGLE)|(board.getRim(x+2,y)&LUTRIANGLE);                 
                 break;
            case VLTRAPEZIUM:
                 stopped = board.getRim(x+1,y)|board.getRim(x+1,y-1)|board.getRim(x+1,y+1);                
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
int CTrapezium::rotateLeft(){
     bool  stopped=true;
     char lPixel, rPixel;
     
     switch ( direction ) {
            case HUTRAPEZIUM:
                 if ( board.getRim(x-1,y-1) ) break;
                 if ( board.getRim(x,y-1)) break;
                 if ( board.getRim(x+1, y+1) ) break;
                 if ( board.getRim(x+1,y)&RUTRIANGLE) break;
                 if ( board.getRim(x,y+1)&RDTRIANGLE ) break;
                 stopped = false;
                 break;
            case VRTRAPEZIUM:
                 if ( board.getRim(x-1,y+1) ) break;
                 if ( board.getRim(x-1,y)) break;
                 if ( board.getRim(x, y-1)&RDTRIANGLE ) break;
                 if ( board.getRim(x+1,y-1)) break;
                 if ( board.getRim(x+1,y)&LDTRIANGLE ) break;
                 stopped = false;
                 break;
            case HDTRAPEZIUM:
                 if ( board.getRim(x+1,y+1) ) break;
                 if ( board.getRim(x,y+1)) break;
                 if ( board.getRim(x-1, y)&LDTRIANGLE ) break;
                 if ( board.getRim(x-1,y-1)) break;
                 if ( board.getRim(x,y-1)&LUTRIANGLE ) break;
                 stopped = false;
                 break;
            case VLTRAPEZIUM:
                 if ( board.getRim(x+1,y-1) ) break;
                 if ( board.getRim(x+1,y)) break;
                 if ( board.getRim(x, y+1)&LUTRIANGLE ) break;
                 if ( board.getRim(x-1,y+1)) break;
                 if ( board.getRim(x-1,y)&RUTRIANGLE ) break;
                 stopped = false;
                 break;
            default:
                 return UNDEFINED;
     } 
     if ( stopped ) return mvDown();
     erase();
     lPixel = direction>>4;
     rPixel = direction&0xF;
     rPixel = ((rPixel&7)<<1)|(rPixel>>3);
     lPixel = ((lPixel&7)<<1)|(lPixel>>3);
     direction = (rPixel<<4)|lPixel;
     show();
     return RT_LEFT;
}

int CTrapezium::rotateRight(){
     bool  stopped=true;
     char lPixel, rPixel;
     
     switch ( direction ) {
            case HUTRAPEZIUM:
                 if ( board.getRim(x-1,y+1) ) break;
                 if ( board.getRim(x,y-1)) break;
                 if ( board.getRim(x+1, y-1) ) break;
                 if ( board.getRim(x-1,y)&LUTRIANGLE) break;
                 if ( board.getRim(x,y+1)&LDTRIANGLE ) break;
                 stopped = false;
                  break;
            case VRTRAPEZIUM:
                 if ( board.getRim(x+1,y+1) ) break;
                 if ( board.getRim(x-1,y)) break;
                 if ( board.getRim(x, y+1)&RUTRIANGLE ) break;
                 if ( board.getRim(x-1,y-1)) break;
                 if ( board.getRim(x+1,y)&LUTRIANGLE ) break;
                 stopped = false;
                 break;
            case HDTRAPEZIUM:
                 if ( board.getRim(x,y+1) ) break;
                 if ( board.getRim(x-1,y+1)) break;
                 if ( board.getRim(x+1, y)&RDTRIANGLE ) break;
                 if ( board.getRim(x+1,y-1)) break;
                 if ( board.getRim(x,y-1)&RUTRIANGLE ) break;
                 stopped = false;
                 break;
            case VLTRAPEZIUM:
                 if ( board.getRim(x-1,y-1) ) break;
                 if ( board.getRim(x+1,y)) break;
                 if ( board.getRim(x-1,y)&RDTRIANGLE ) break;
                 if ( board.getRim(x+1,y+1)) break;
                 if ( board.getRim(x,y-1)&LDTRIANGLE ) break;
                 stopped = false;
                 break;
            default:
                 return UNDEFINED;
     } 
     if ( stopped ) return mvDown();
     erase();
     lPixel = direction>>4;
     rPixel = direction&0xF;
     rPixel = (rPixel>>1)|((rPixel&1)<<3);
     lPixel = (lPixel>>1)|((lPixel&1)<<3);
     direction = (rPixel<<4)|lPixel;
     show(); 
     return RT_RIGHT;
}


