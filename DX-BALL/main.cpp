#include<Windows.h>
#include <GL/glut.h>
#include<iostream>
#include <math.h>
#include<stdio.h>

using namespace std;


float mouseX ,mouseY ;
float ballX, ballY, ballSize, ballSpeedY, ballSpeedX;
float barPosX,barSize;
int Score=0, highScore=0;
int totalBrick = 1;
float globalX= -1100.0, globalY= 600;
float blocksize= 180, blockSpacing = 20;

bool startScreen = true, gameScreen = false, gameOverScreen=false, instructionsGame = false;
bool gameQuit = false, isGameStarted = false, finishGame=false, isCatched=false;
bool isCollideToTop=false, isCollideToBottom=false, isCollideToRight=false, isCollideToLeft=false;
bool mouseButtonPressed = false, isBlockCreated = false;
struct Block{
    float posX1, posX2;
    float posY1, posY2;
    bool isDead;
}blocks[50];

void DisplayText(float x ,float y ,char *text){
    glRasterPos3f(x, y, 0);// to display text position must be ruster.
    int length = strlen(text);
    for(int i=0 ;i<length ;i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24 ,text[i]); // to display character by character.
	}
}

void RandomBallCorner(){
    ballSpeedX = 10 + rand()%40;
    if(rand()%2==0){
        ballSpeedX *= 1;
    }
    else
        ballSpeedX *= -1;
}

//Creating single Block
void BrickCreator(Block brick, int br){
    if(brick.isDead){
        glColor3f(1,1,1);
    }
    else{

        if(br%2==0){
           glColor3f(1,0,0);
            }
        else
            glColor3f(0,1,0);

         glBegin ( GL_POLYGON ) ;

                glVertex2i ( brick.posX1 , brick.posY1 ) ;

                glVertex2i ( brick.posX1 , brick.posY2 ) ;

                glVertex2i ( brick.posX2 , brick.posY2) ;

                glVertex2i ( brick.posX2 , brick.posY1 ) ;
            glEnd () ;

    }
}

// For Creating Block
void CreateBrick(){
    if(!isBlockCreated){
        float localX = globalX;
        float localY = globalY;
        int tempCounter = 0;
        for(int j=0; j<4; j++){

           for(int i = 0; i<11; i++){
                blocks[tempCounter].posX1 = localX;
                blocks[tempCounter].posX2 = localX + blocksize;
                blocks[tempCounter].posY1 = localY;
                blocks[tempCounter].posY2 = localY + 80;
                blocks[tempCounter].isDead = false;
                localX += blocksize + blockSpacing;
                tempCounter++;
           }
           localY -= 100;
            localX = globalX;
        }
        isBlockCreated = true;
    }
    //blocks[22].isDead = true;
    for(int i=0; i<44; i++){
        BrickCreator(blocks[i], i);
    }
}
void CreateBall(float cx, float cy, float r, int num_segments){
    //Ball
   glColor3f(1, 1, 1);
    glBegin(GL_POLYGON);
    for (int ii = 0; ii < num_segments; ii++)
    {
        float theta = 2.0f * 3.1415926f * float(ii) / float(num_segments);
        float x = r * cosf(theta);
        float y = r * sinf(theta);
        glVertex2f(x + cx, y + cy);
    }
    glEnd();

}
void CreateCatcher(){
    glColor3f(1,0.5,0.8);
    glPushMatrix();
    glTranslated(barPosX, -700.0, 0.0);
    glBegin(GL_POLYGON);
    glVertex2i ( 0 , 0 ) ;
    glVertex2i ( 0 , 50 ) ;
    glVertex2i ( barSize , 50) ;
    glVertex2i ( barSize , 0 ) ;
    glEnd () ;
    glPopMatrix();

}


void ResetGameState(){
    isCollideToBottom = false;
    isGameStarted = false;
    gameOverScreen=false;
    Score = 0;
    barPosX=-200;
    barSize = 400;
    ballSize= 30;
    ballX = barPosX+barSize/2; ballY = -650+ballSize;
    ballSpeedY = 40; ballSpeedX = 0;
    for(int i=0; i<44; i++){
            blocks[i].isDead = false;
        }
}

void backButton() {
	if(mouseX >=80 && mouseX <= 165 && mouseY >= 590 && mouseY <= 660)
        {
			glColor3f(0, 0, 1);
			if(mouseButtonPressed)
                {
                    mouseButtonPressed = false;
                    instructionsGame = false;
                    startScreen = true;
                }
        }
	else
        glColor3f(0, 1, 1);
	DisplayText(-1000, -550, "Back");
}


void InsructionScreenDisplay(){
    glClearColor(0,0,0,0);
	glColor3f(1, 1, 1);
	DisplayText(-900, 500, "Press Right key to move right.");
	DisplayText(-900, 400, "Press left key to move left.");
	DisplayText(-900, 300, "Press Space to shoot the Ball.");
	DisplayText(-900, 200, "Press Mouse Left button to select any Option.");
	DisplayText(-900, 100, "You get 1 points for breaking one Brick.");
	DisplayText(-900, 0  , "If the ball hit on the Ground You lose.");
    backButton();

}

void Stars(int size)
{
    glColor3f(1,1,1);
        glPointSize(size);
            glBegin(GL_POINTS);
            glVertex2f(rand()%1200+100,rand()%800);
            glVertex2f(rand()%1200-1200,rand()%800);
            glVertex2f(rand()%1200+100,rand()%800-800);
            glVertex2f(rand()%1200-1200,rand()%800-800);
        glEnd();
}




void StartScreenDisplay()
{


     glBegin ( GL_POLYGON ) ;

        glColor3f(1, 0, 0);
            glVertex2i ( -1200 , -800 ) ;

        glColor3f(0.5, 0.5, 0);
            glVertex2i ( -1200 , 800 ) ;
        glColor3f(0, 0.5, 0.05);
            glVertex2i ( 1200 , 800) ;
        glColor3f(0, 0.05, 0);
            glVertex2i ( 1200 , -800 ) ;
        glEnd () ;
        Stars(3);
    glLineWidth(5);
    glClearColor(0,0,0,0);

    // Game Title Bar
        glColor3f(0.5,0,0);
        glBegin(GL_POLYGON);
            glVertex3f(-350,650,0.5);
            glVertex3f(-400,700,0.5);
            glVertex3f(400,700,0.5);
            glVertex3f(350,650,0.5);
            glVertex3f(400,600,0.5);
            glVertex3f(-400,600,0.5);
        glEnd();
        glColor3f(1,1,1);
        DisplayText(-80, 630, "DX-Ball");

    //Start Game Button
        glColor3f(0.145, 0.580, 0.796);
        glBegin(GL_LINE_STRIP);
            glVertex2f(-200 ,200);
            glVertex2f(-200 ,300);
            glVertex2f(200 ,300);
            glVertex2f(200 ,200);
            glVertex2f(-200 ,200);
        glEnd();
    //Instruction Button
        glBegin(GL_LINE_STRIP);
            glVertex2f(-200, -50);
            glVertex2f(-200 ,50);
            glVertex2f(200 ,50);
            glVertex2f(200 ,-50);
            glVertex2f(-200, -50);
        glEnd();
    //Quit Button
        glBegin(GL_LINE_STRIP);
            glVertex2f(-200 ,-300);
            glVertex2f(-200 ,-200);
            glVertex2f(200, -200);
            glVertex2f(200, -300);
            glVertex2f(-200 ,-300);
        glEnd();

       //Start Game Button Function
        if(mouseX>=500 && mouseX<=700 && mouseY>=220 && mouseY<=280)
            {
                glColor3f(0 ,0 ,1) ; //mouse hover color
                if(mouseButtonPressed)
                    {
                        gameScreen = true ;
                        gameOverScreen = false;
                        mouseButtonPressed = false;
                        ResetGameState();
                    }
            }
        else
            glColor3f(1 , 1, 1);
        DisplayText(-100 ,235 ,"Start Game");

    //Instruction Button Function
        if(mouseX>=500 && mouseX<=700 && mouseY>=340 && mouseY<=400)
            {
                glColor3f(0 ,0 ,1);  //mouse hover color
                if(mouseButtonPressed)
                    {
                        instructionsGame = true ;
                        mouseButtonPressed = false;
                    }
            }
        else
            glColor3f(1 , 1, 1);
        DisplayText(-120 ,-20 ,"Instructions");

    //Exit Button Function
        if(mouseX>=500 && mouseX<=700 && mouseY>=460 && mouseY<=530)
            {
                glColor3f(0 ,0 ,1);
                if(mouseButtonPressed)
                    {
                    gameQuit = true ;
                    mouseButtonPressed = false;
                    }
            }
        else
            glColor3f(1 , 1, 1);
        DisplayText(-50 ,-270 ,"Quit");


}


void GamePlayScreenDisplay(){
    glClearColor(0,0,0,0);


    CreateBall(ballX, ballY, ballSize, 20);
    CreateCatcher();
    CreateBrick();
    if(Score>=44){
        finishGame = true;
        gameScreen = false;
    }

    char temp[40];
    glColor3f(1,0.2,0.1);
    sprintf(temp,"Score : %d",Score);
    DisplayText(-1180,700,temp);
}


void GameOverScreenDisplay(){
    glColor3f(1,1,1);
	glLineWidth(1);
	glColor3f(0, 1, 1);
	glBegin(GL_LINE_STRIP);				//GAME OVER
		glVertex2f(-550 ,650);
		glVertex2f(-550 ,520);
		glVertex2f(550 ,520);
		glVertex2f(550 ,650);
		glVertex2f(-550 ,650);
	glEnd();

	glColor3f(1, 1, 0);
	glBegin(GL_POLYGON);				//RESTART POLYGON
		glVertex3f(-200, 50 ,0.5);
		glVertex3f(-200 ,150 ,0.5);
		glVertex3f(200 ,150 ,0.5);
		glVertex3f(200 ,50, 0.5);
	glEnd();

	glBegin(GL_POLYGON);				//QUIT POLYGON
		glVertex3f(-200 ,-200 ,0.5);
		glVertex3f(-200 ,-100 ,0.5);
		glVertex3f(200, -100 ,0.5);
		glVertex3f(200, -200 ,0.5);
	glEnd();


	DisplayText(-200 ,560 ,"G A M E   O V E R ");

	glColor3f(0.5 , 0, 0.5);
	char temp[40];
	sprintf(temp,"Score : %d",Score);
	DisplayText(-100 ,380 ,temp);

    glColor3f(1 , 0, 0.1);
	char temp2[40];
	sprintf(temp2,"High Score : %d",highScore);
	DisplayText(-150 ,280 ,temp2);


	if(mouseX>=500 && mouseX<=700 && mouseY>=290 && mouseY<=340){
		glColor3f(0 ,1 ,1);
    if(mouseButtonPressed){
         //Reset game default values
			gameScreen = true ;
			gameOverScreen=false;
			mouseButtonPressed = false;
			ResetGameState();
            }
        }
	else
		glColor3f(0 , 0, 1);
	DisplayText(-70 ,80 ,"Restart");

	if(mouseX>=500 && mouseX<=700 && mouseY>=410 && mouseY<=460){
		glColor3f(0 ,1 ,0);
		if(mouseButtonPressed){
			exit(0);
			mouseButtonPressed = false;
		}
	}
	else
		glColor3f(0, 0, 1);
	DisplayText(-100 ,-170 ,"    Quit");
}
void FinishGameScreen(){
    glColor3f(1, 0, 0);
    DisplayText(-170,450,"Y O  U  W O N");
    glColor3f(1, 0.5, 0);
    DisplayText(-300,300,"Thanks For Playing Our Game!!");
    glColor3f(1, 1, 0);
	glBegin(GL_POLYGON);				//RESTART POLYGON
		glVertex3f(-200, 50 ,0.5);
		glVertex3f(-200 ,150 ,0.5);
		glVertex3f(200 ,150 ,0.5);
		glVertex3f(200 ,50, 0.5);
	glEnd();

	glBegin(GL_POLYGON);				//QUIT POLYGON
		glVertex3f(-200 ,-200 ,0.5);
		glVertex3f(-200 ,-100 ,0.5);
		glVertex3f(200, -100 ,0.5);
		glVertex3f(200, -200 ,0.5);
	glEnd();
	if(mouseX>=500 && mouseX<=700 && mouseY>=290 && mouseY<=340){
		glColor3f(0 ,1 ,1);
    if(mouseButtonPressed){
         //Reset game default values
			gameScreen = true ;
			finishGame=false;
			mouseButtonPressed = false;
			ResetGameState();
            }
        }
	else
		glColor3f(0 , 0, 1);
	DisplayText(-70 ,80 ,"Restart");

	if(mouseX>=500 && mouseX<=700 && mouseY>=410 && mouseY<=460){
		glColor3f(0 ,1 ,0);
		if(mouseButtonPressed){
			exit(0);
			mouseButtonPressed = false;
		}
	}
	else
		glColor3f(0, 0, 1);
	DisplayText(-100 ,-170 ,"    Quit");
}


void display()
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   if(startScreen)
    {
        StartScreenDisplay();

    }

    if(instructionsGame)
    {
        startScreen = false;
        InsructionScreenDisplay();
    }

    if(gameScreen){
        startScreen = false;
        GamePlayScreenDisplay();
    }

    if(gameOverScreen){
        GameOverScreenDisplay();
    }
     if(finishGame){
        FinishGameScreen();
    }

    if(gameQuit){
        exit(0);
    }
    glFlush();
	glLoadIdentity();
	glutSwapBuffers();

}


void Update(int v){

    if(gameScreen && isGameStarted){
         ballY += ballSpeedY;
         ballX += ballSpeedX;


         if(barPosX<= ballX  && ballX<= barPosX + barSize && ballY<= -650+ballSize){
                isCatched = true;
         }


         if(ballX<=-1200 + ballSize ){
                isCollideToLeft = true;
            }
        if(ballX>=1200 - ballSize ){
                isCollideToRight = true;
            }
        if(ballY<=-800 + ballSize){
                isCollideToBottom = true;
            }
        if(ballY>=800 - ballSize ){
                isCollideToTop = true;
            }

        if(isCollideToTop){

            ballSpeedY*=-1;
            isCollideToTop=false;
        }
        if(isCatched){

            float ballOnCatcher = (barSize+(ballX-barPosX))/barSize;
            if(ballOnCatcher>=1 && 1.1>ballOnCatcher)
                ballSpeedX= -32;
            else if(ballOnCatcher>=1.1 && 1.2>ballOnCatcher)
                ballSpeedX= -26;
            else if(ballOnCatcher>=1.2 && 1.33>ballOnCatcher)
                ballSpeedX= -18;
            else if(ballOnCatcher>=1.33 && 1.48>ballOnCatcher)
                ballSpeedX= -9;
            else if(ballOnCatcher>=1.48 && 1.52>ballOnCatcher)
                ballSpeedX= 0.0;
            else if(ballOnCatcher>=1.52 && 1.67>ballOnCatcher)
                ballSpeedX= 9;
            else if(ballOnCatcher>=1.67 && 1.8>ballOnCatcher)
                ballSpeedX= 18;
            else if(ballOnCatcher>=1.8 && 1.9>ballOnCatcher)
                ballSpeedX= 26;
            else if(ballOnCatcher>=1.9 && 2.0>=ballOnCatcher)
                ballSpeedX= 32;
            ballSpeedY*=-1;
            isCatched = false;
        }
        if(isCollideToBottom){
            if(highScore<Score)
                highScore = Score;

            gameOverScreen = true;
            gameScreen = false;
        }

        if(isCollideToLeft || isCollideToRight){
            ballSpeedX*=-1;
            isCollideToRight=false;
            isCollideToLeft=false;
        }


        for(int i=0; i<44; i++){

           if(blocks[i].posX1 - ballSize/1.5 < ballX && ballX <  blocks[i].posX2 + ballSize/1.5 && blocks[i].posY1 - ballSize/1.5 < ballY && ballY <  blocks[i].posY2 + ballSize/1.5 )
                {

                    if(!blocks[i].isDead){
                        ballSpeedY*=-1;
                        Score++;

                        blocks[i].isDead = true;

                    }

                }
        }

    }

    glutTimerFunc(100, Update, v);
}

static void idle(void)
{
    glutPostRedisplay();
}
void myinit()
{
	glClearColor(0.0,0.0,0.0,0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

    gluOrtho2D(-1200,1200,-750,750);
	glMatrixMode(GL_MODELVIEW);
}
void passiveMotionFunc(int x,int y) {


	mouseX = float(x);  //converting screen resolution to ortho 2d spec
	//cout<<"X="<<x<<endl;
	cout<<"Mouse X="<<mouseX<<endl;
	mouseY = float(y);
	//cout<<"Y="<<y<<endl;
	cout<<"Mouse Y="<<mouseY<<endl;
	//display();
}

void SpecialFunc(int key, int x, int y){
    if(gameScreen && isGameStarted)
        {
        switch(key){
            case GLUT_KEY_RIGHT:
                if(barPosX<1200-barSize)
                   // barPosX += 50;
                    barPosX += 75;
                break;
            case GLUT_KEY_LEFT:
                if(barPosX>-1200)
                   // barPosX -= 50;
                    barPosX -= 75;
                break;

             case GLUT_KEY_END:
                {
                    ResetGameState();
                }
                break;
            }
        }


    if(gameScreen && !isGameStarted)
        {
        switch(key){
            case GLUT_KEY_RIGHT:
                if(barPosX<1200-barSize)
                    {
                        barPosX += 50;
                        ballX += 50;
                    }
                break;
            case GLUT_KEY_LEFT:
                if(barPosX>-1200)
                {
                    barPosX -= 50;
                    ballX -= 50;
                }
                break;

            case GLUT_KEY_HOME:
                {
                    isGameStarted = true;
                    RandomBallCorner();
                }
                break;
            }
        }
    glutPostRedisplay();
}

static void keyBoard(unsigned char key, int x, int y)
{
     if(gameScreen && !isGameStarted)
        {
            switch (key)
                {
                case ' ':
                    {
                    isGameStarted = true;
                    RandomBallCorner();
                    }
                    break;
                }
        }


    glutPostRedisplay();
}
void mouseClick(int buttonPressed ,int state ,int x, int y) {
	if(buttonPressed == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		mouseButtonPressed = true;
	else
		mouseButtonPressed = false;
	display();
}



int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(1200,750);
    glutInitWindowPosition(0,0);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("DX-Ball Game");

    glutDisplayFunc(display);
    glutSpecialFunc(SpecialFunc);
    glutKeyboardFunc(keyBoard);
    glutTimerFunc(100, Update, 0);
    glutPassiveMotionFunc(passiveMotionFunc);
    glutMouseFunc(mouseClick);
    glutIdleFunc(idle);
    myinit();


    glutMainLoop();

    return EXIT_SUCCESS;
}
