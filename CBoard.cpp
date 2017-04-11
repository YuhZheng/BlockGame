#include "typedef.h"
/*
class CBoard {
      CDisplay* pDisplay;
      unsigned short pixels[31][13]; 
      vector<CBrick*> vecPTRBricks;
      SRowState rowStates[31];
public:
       CBoard();
       ~CBoard();
       void repaint();
       bool init(CDisplay* pDisplay_);
       unsigned short  getRim(short x, short y);
       CBoard& operator<<(CTriangle triangle);
       CBoard& operator>>(CTriangle triangle);
};
*/

CBoard::CBoard() {
      int i;
      for( i=0; i<13; i++ ) pixels[0][i] = 0x030C;
      for( i=1; i<31; i++ ) pixels[i][0] = pixels[i][12] = 0x030C;
      for( int i=0; i<30; i++ )
          for( int j=0; j<11; j++) pixels[i+1][j+1]=0;
      for( i=1; i<31; i++ ) {
           rowStates[i].linked = false;
           rowStates[i].blankTriangles = 22;
      }
      vecPTRBricks.clear();
      pDisplay = NULL;    
}

bool CBoard::init(CDisplay* pDisplay_) {
     if ( pDisplay != NULL ) return false;
     pDisplay = pDisplay_;
     pDisplay->show(rowStates);
     return true;
}

void CBoard::repaint() {
     int i, j;
     
     for(i=1; i<=30; i++) 
         for(j=1; j<=11; j++){
			pDisplay->show(j, i, pixels[i][j]);
			pDisplay->show(j, i, pixels[i][j]>>8);
		}
     pDisplay->show(rowStates);
     
     return;
}


unsigned short CBoard::getRim(short x, short y) {
     if ( x<1 || x>11 || y<1 || y>30 ) return UNDEFINED; 
     return ((pixels[y][x]>>8)|pixels[y][x])&0xF;
}

CBoard& CBoard::operator<<(CTriangle triangle) {
     int x=triangle.x;
     int y=triangle.y;
     unsigned char color = triangle.color;
     unsigned char direction = triangle.direction;
     
     if ( x<1 || x>11 || y<1 || y>30 ) return (*this);
     switch (((pixels[y][x]>>8)|pixels[y][x])&0xF) {
            case 0xF:
                 return (*this);
            case 0:
                 pixels[y][x] = (color<<4)|direction;
                 break;
            default:
                 if (direction&pixels[y][x]&0xF) return (*this);
                 pixels[y][x] = ((color<<4)|direction)|(pixels[y][x]<<8);
                 break;
     }
     if (pDisplay==NULL) return (*this);
     pDisplay->show(x, y, (color<<4)|direction);
     rowStates[y].blankTriangles--;
     return (*this);      
}

CBoard& CBoard::operator>>(CTriangle triangle) {
     int x=triangle.x;
     int y=triangle.y;
     unsigned char direction = triangle.direction;


     if ( x<1 || x>11 || y<1 || y>30 ) return (*this);
     if ( (direction&0xF)== (pixels[y][x]&0xF) ) {
          triangle.color = (pixels[y][x]&0xF0)>>4;
          pDisplay->erase(x, y, direction);
          pixels[y][x]=pixels[y][x]>>8;
     }
     if ( (direction&0xF)== ((pixels[y][x]>>8)&0xF) ) {
          triangle.color = (pixels[y][x]&0xF000)>>12;
          pDisplay->erase(x, y, direction);
          pixels[y][x]=pixels[y][x]&0xFF;
     }
     rowStates[y].blankTriangles++;
     return (*this);
}

int CBoard::operator<<(CBrick* pBrick){
    int result = 0;
    RECT rect;    
    vector<CBrick*>::iterator ptrBrick;
     
    vecPTRBricks.push_back(pBrick);
    /********************************************/    
    int rowID = pBrick->getRowofCenter();
    rect.bottom = -1;
    rect.top = -1;
    
    if(pBrick->isVertical()){
        rowStates[rowID].linked = true;
        rowStates[rowID-1].linked = true;
    }
    for(rowID = 1;rowID < 31;rowID++){
        if(rowStates[rowID].blankTriangles > 0) continue;
        if(rowStates[rowID].linked == true){
            int temp = rowID + 1;
            while(rowStates[temp].blankTriangles == 0 && temp < 31){
                if(rowStates[temp].linked == false){
                    rect.top = temp;
                    rect.bottom = rowID;
                    //linked如何还原？？
                    break;
                }
                temp++;
            }
        }else{
            rect.top = rect.bottom = rowID;
        }
        if(rect.top != -1 || rect.bottom != -1) break;        
    } 
    /*******************************************/
    if(rect.top == -1 && rect.bottom == -1) return 0;
    CComparator comparator_obj;
    sort(vecPTRBricks.begin(),vecPTRBricks.end(),comparator_obj);
    /**********************************************/
    ptrBrick = vecPTRBricks.begin();
    int height = rect.top - rect.bottom + 1;
    while (ptrBrick!=vecPTRBricks.end()){
        rowID = (*ptrBrick)->getRowofCenter();  
        if(rowID>=rect.bottom && rowID<=rect.top){
            result += (*ptrBrick)->deleteBrick(); 
            delete *ptrBrick; 
            ptrBrick = vecPTRBricks.erase(ptrBrick);
            continue;
        } 
        if(rowID>rect.top) (*ptrBrick)->drop(height);
        ptrBrick++;
    }
    
    return result;
}

CBoard::~CBoard() {
         
}
