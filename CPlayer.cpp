#include "typedef.h"
/*
class CPlayer {
      int  score;
      char gameFilePath[MAX_PATH];
      CDisplay* pDisplay;
      CBoard&   board;
      
      HWND hWnd; //main window 
      UINT_PTR hTimer;
      int  command;
      CBrick* pBrick;//CTrapezium* pTrapezium;
      void newGame();
      void saveGame();
      void openGame();
public:
       CPlayer(CBoard& board_);
       void init(HWND hWnd_);
       void repaint();
       bool clickButton(LPARAM hButton);
       void executeCommand();
       ~CPlayer();
};
*/
CPlayer::CPlayer(CBoard& board_):board(board_) {
      hTimer = 0;
      pDisplay = NULL;
      pBrick = NULL;
      command = UNDEFINED;
      return; 
}

void CPlayer::init(HWND hWnd_) {
     
     score = 0;
     hTimer = 0;
     hWnd = hWnd_;
     gameFilePath[0] = 0;
     pDisplay = new CDisplay(hWnd);
     board.init(pDisplay);
     return; 
}

void CPlayer::repaint() {
     
     pDisplay->refresh(score);
     board.repaint();     
     return; 
}

void CPlayer::newGame() {
     /*************************************************************************/
     /*                                                                       */
     /*      添加代码，清楚当前游戏进度，重置为开始玩游戏的状态               */
     /*                                                                       */
     /*************************************************************************/
     
     pBrick = CBrick::newBrick();//newBrick();
     if ( pBrick==NULL ) return;
     command = UNDEFINED;
     if ( hTimer!=0 ) KillTimer(hWnd, TIMER_ID);
     hTimer = SetTimer(hWnd, TIMER_ID, MV_SPEED, NULL);
     pDisplay->show(score);

     return;      
}

void CPlayer::executeCommand() {
     short result;
     char  str_gameover[] = "Game Over";
     if (pBrick==NULL) return;
     switch ( command ) {
     case MV_LEFT:
          result = pBrick->mvLeft();
          break;
     case MV_RIGHT:
          result = pBrick->mvRight();
          break;
     case RT_LEFT:
          result = pBrick->rotateLeft();
          break;
     case RT_RIGHT:
          result = pBrick->rotateRight();
          break;
     default:
          result = pBrick->mvDown(command);
          break;     
     }
     command = UNDEFINED;
     if ( result != STOPPED ) return;
     
     /*****如何解决指针删除问题？*****/
     pBrick->stop();
     score += pBrick->score;
     board.repaint();
     //delete pBrick;
     /**************************/
     
     pBrick = CBrick::newBrick();
     if ( pBrick==NULL ) pDisplay->show(str_gameover);
     return;
}

CPlayer::~CPlayer() {
      if ( hTimer!=0 ) KillTimer(hWnd, TIMER_ID);
      if (pDisplay != NULL) delete pDisplay;
      if (pBrick!=NULL) delete pBrick;
      return; 
}

bool CPlayer::clickButton(LPARAM hButton) {
     
     command = pDisplay->parseCommand(hButton);
     switch ( command ) {
            case NEW_GAME:
                 newGame();
                 break;
            case SAVE_GAME:
                 saveGame();
                 break;
            case OPEN_GAME:
                 openGame();
                 break;
            case EXIT_GAME:
                 return false;
            default:
                    break;
     }
     return true;
}

void CPlayer::saveGame() {
     OPENFILENAME ofnGame;
          
     if ( hTimer!=0 ) KillTimer(hWnd, TIMER_ID);
         
     ZeroMemory(&ofnGame, sizeof(ofnGame));
     ofnGame.lStructSize = sizeof(ofnGame);
     ofnGame.hwndOwner = hWnd;
     ofnGame.lpstrFilter = "brick-game files (*.brk)\0*.brk\0\0";
     ofnGame.lpstrFile = gameFilePath;
     ofnGame.nMaxFile = MAX_PATH;
     ofnGame.lpstrDefExt = "brk";
     ofnGame.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY |
         OFN_OVERWRITEPROMPT;

     while (gameFilePath[0] == 0) 
         if (GetOpenFileName(&ofnGame)==false) {
         hTimer = SetTimer(hWnd, TIMER_ID, 500, NULL);
         return;
         }
     ofstream gameFile(gameFilePath, ios::out);
     /*************************************************************************/
     /*                                                                       */
     /*           添加代码，将游戏进度保存到文件"gameFile"中                  */
     /*                                                                       */
     /*************************************************************************/
     gameFile.close();
     hTimer = SetTimer(hWnd, TIMER_ID, 500, NULL);
     return;
}

void CPlayer::openGame() {
     OPENFILENAME ofnGame;
     
     if ( hTimer!=0 ) KillTimer(hWnd, TIMER_ID);
      
     ZeroMemory(&ofnGame, sizeof(ofnGame));
     ofnGame.lStructSize = sizeof(ofnGame);
     ofnGame.hwndOwner = hWnd;
     ofnGame.lpstrFilter = "brick-game files (*.brk)\0*.brk\0\0";
     ofnGame.lpstrFile = gameFilePath;
     ofnGame.nMaxFile = MAX_PATH;
     ofnGame.lpstrDefExt = "brk";
     ofnGame.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;

     gameFilePath[0] = 0;
     while (gameFilePath[0] == 0) 
         if (GetOpenFileName(&ofnGame)==false) {
         hTimer = SetTimer(hWnd, TIMER_ID, 500, NULL);
         return;
         }
         
     ifstream gameFile(gameFilePath, ios::in);
     /*************************************************************************/
     /*                                                                       */
     /*           添加代码，从文件"gameFile"中读入游戏进度                    */
     /*                                                                       */
     /*************************************************************************/
     gameFile.close();
     hTimer = SetTimer(hWnd, TIMER_ID, 500, NULL);
     return;
}
