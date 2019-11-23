#include"iGraphics.h"
#include<string.h>

#define totalBricks 150
#define screenWidth 1200
#define screenHeight 650
#define yAxisMargin 50 //initial ordinate of ball center

FILE *userfile,*hs;

char str_score[4],name[20],filename[24],line[5][25],topscorer[5][20];

int i,life,score,gameover=0,highscore[5],loadgame_flag=0;
int intro=1,menumode=0,username_mode=0,writing_mode=0,play_mode=0,congrats=0;
int wantTOsave=0,wantTOload=0,pausemode=0,show_highscore=0,nosavedgame=0;

struct brickStructure
{
 int x,y,dx=100,dy=20,red,green,blue;
 int show=1;

};

brickStructure bricks[totalBricks];
 int red=0;//ball's color
 int green =0;//
 int blue=0;//

 int xBall =(int)screenWidth/2;
 int yBall =yAxisMargin;
 int radiusBall=10;
 int dx=7;//change of co-ordinate of ball per 10 mili sec
 int dy=5;//

 int xBoard=xBall-50;
 int yBoard=yBall-20;
 int dxBoard=100;//length of board
 int dyBoard=10;//height of board

 int isStarted=0;//movement of ball

 void ballChange();
 void definebricks();

 void reset()
{
    xBall =(int)screenWidth/2;
    yBall =yAxisMargin;
    xBoard=xBall-50;
    yBoard=yBall-20;
    isStarted=0;
    for(int j=0;j<totalBricks;j++){
        bricks[j].show=1;
    }
    for(int j=0;name[j];j++){
        name[j]=0;
    }
    i=0;
    score=0;
    life=3;
}

void iDraw()
{
    iClear();
    int j;

    if(intro){
        iShowBMP(0,0,"intro.bmp");
        iText(10,10,"***PRESS ENTER TO CONTINUE***",GLUT_BITMAP_HELVETICA_18);
    }

    if(menumode){

        iShowBMP(0,0,"menu bg.bmp");
        iShowBMP(300,162.5,"menu.bmp");
        reset();

    }
    if(username_mode){
        iSetColor(255,255,255);
        iText(450,400,"ENTER USERNAME:",GLUT_BITMAP_TIMES_ROMAN_24);
        iText(30,30,"***click inside the box before writing your name***",GLUT_BITMAP_HELVETICA_18);
        iRectangle(450,350,250,40);
        if(writing_mode){
                iText(460,370,name,GLUT_BITMAP_TIMES_ROMAN_24);
        }
    }

    if(nosavedgame){
        iShowBMP(0,0,"sorry.bmp");
    }

    if(wantTOsave){
        iShowBMP(0,0,"save game.bmp");
    }
    if(wantTOload){
        iShowBMP(0,0,"load game.bmp");
    }

    if(show_highscore){
        iShowBMP(0,0,"high score.bmp");

        iSetColor(255,255,255);
        iText(660,565,itoa(score,str_score,10),GLUT_BITMAP_TIMES_ROMAN_24);

        iSetColor(0,0,0);
        int pos=340;
        for(int i=0;i<5;i++){
            iText(530,pos,line[i],GLUT_BITMAP_TIMES_ROMAN_24);
            pos=pos-40;
        }
        iText(430,25,"***PRESS ENTER TO CONTINUE***",GLUT_BITMAP_HELVETICA_18);

    }

    if(gameover){
        iShowBMP(0,0,"game over.bmp");
        iSetColor(255,255,255);
        iText(900,60,"***PRESS ENTER TO CONTINUE***",GLUT_BITMAP_HELVETICA_18);
    }

    if(pausemode){
        iShowBMP(0,0,"pause.bmp");
    }

    if(congrats){
        iShowBMP(0,0,"congrats.bmp");
    }

    if(play_mode){

        iSetColor(0,255,255);
        iFilledRectangle(0,30,screenWidth,screenHeight);
        iSetColor(222,184,135);
        iFilledRectangle(0,0,screenWidth,30);

        iSetColor(255,255,255);
        iFilledRectangle(990,580,148,40);
        iFilledRectangle(8,580,143,40);

        iSetColor(0,0,0);
        iRectangle(990,580,148,40);
        iRectangle(8,580,147,40);
        iText(10,595,"LIFE:",GLUT_BITMAP_TIMES_ROMAN_24);
        iText(1000,595,"SCORE:",GLUT_BITMAP_TIMES_ROMAN_24);
        iText(1090,595,itoa(score,str_score,10),GLUT_BITMAP_TIMES_ROMAN_24);

        if(!isStarted){
            iText(30,200,"PRESS SPACE TO START, P TO PAUSE, ESC TO QUIT",GLUT_BITMAP_TIMES_ROMAN_24);
        }


        int life_x=80;//coordinate of life indicating balls
        int life_y=604;
        for(j=1;j<=life;j++)
        {
            iFilledCircle(life_x,life_y,10);
            life_x=life_x+30;
        }

        iLine(0,30,1250,30);

        for(j=0;j<1200;j=j+10){//hatch line
            iLine(j,0,j+10,30);
        }

         iSetColor(red,green,blue);
        iFilledCircle(xBall,yBall,radiusBall,1000);
        iSetColor(192,192,192);
        iFilledRectangle(xBoard,yBoard,dxBoard,dyBoard);
        iSetColor(0,0,0);
        iRectangle(xBoard,yBoard,dxBoard,dyBoard);


        for(int i=0;i<totalBricks;i++)
        {
            if(bricks[i].show)
            {
            iSetColor(bricks[i].red,bricks[i].green,bricks[i].blue);
            iFilledRectangle(bricks[i].x,bricks[i].y,bricks[i].dx,bricks[i].dy);
            iSetColor(0,0,0);
            iRectangle(bricks[i].x,bricks[i].y,bricks[i].dx,bricks[i].dy);//border
            }
        }
    }
}
void iKeyboard(unsigned char key){

    int j,c;

    if(intro){
        if(key=='\r'){
            intro=0;
            menumode=1;
        }
    }
    if(writing_mode){
         if(key == '\r')
		{
		    username_mode=0;
		    writing_mode=0;

            strcpy(filename,name);
            strcat(filename,".txt");//created a file name according to username

            if(loadgame_flag){
                loadgame_flag=0;
                userfile=fopen(filename,"r");
                if(userfile!=NULL){
                    wantTOload=1;
                }
                else {
                    nosavedgame=1;
                }
            }

            else play_mode=1;

		}
		else if(key == '\b')
        {
            i--;
            name[i]=0;
        }
		else
		{
			name[i] = key;
			i++;
		}
	}
    if(play_mode){
        if(key==' '){
            isStarted=1;
            pausemode=0;
        }
        else if(key=='p'||key=='P'){
            isStarted=0;//ball will stop moving
            play_mode=0;
            pausemode=1;//board will stop moving
        }
        else if(key==27){//if press escape
            play_mode=0;
            isStarted=0;
            wantTOsave=1;
        }
    }
    if(wantTOsave){
        if(key=='n'||key=='N'){
                menumode=1;
                wantTOsave=0;

        }
        if(key=='y'||key=='Y'){
            userfile=fopen(filename,"w");
            fprintf(userfile,"%d %d\n%d %d %d\n",score,life,xBall,yBall,xBoard);
            for(i=0;i<totalBricks;i++){
                fprintf(userfile,"%d\n",bricks[i].show);
            }
            fclose(userfile);

            menumode=1;
            wantTOsave=0;
        }
    }
    if(wantTOload){
        if(key=='n'||key=='N'){
                play_mode=1;
                wantTOload=0;
        }
        else if(key=='y'||key=='Y'){
            fscanf(userfile,"%d%d",&score,&life);
                    fscanf(userfile,"%d%d%d",&xBall,&yBall,&xBoard);
                    j=0;
                    while(!feof(userfile)){
                        fscanf(userfile,"%d",&bricks[j].show);
                        j++;
                    }
            fclose(userfile);
            wantTOload=0;
            pausemode=1;
        }

    }
    if(nosavedgame){
        if(key==27){
            nosavedgame=0;
            menumode=1;
        }
    }
    if(show_highscore){
        if(key=='\r'){
            show_highscore=0;
            menumode=1;
        }
    }
    if(congrats){
        if(key=='\r'){
            congrats=0;
            show_highscore=1;
        }
    }
    if(gameover){
        if(key=='\r'){
            gameover=0;

            hs=fopen("high score.txt","r");//take input from high score file
            for(j=0;j<5;j++){
                fscanf(hs,"%s %d",topscorer[j],&highscore[j]);
            }
            fclose(hs);

            for(j=0;j<5;j++){//make change to leaderboard
                if(score>highscore[j]){
                    break;
                }
            }
            if(j!=5){
                for(c=0;c<5;c++){
                    if(!strcmp(name,topscorer[c])){//if username matches
                        if(score>highscore[c]){
                            highscore[c]=score;
                            for(;c>0;c--){
                                if(highscore[c]>highscore[c-1]){//reorganize the upper portion of
                                    int temp=highscore[c];      //'c'th line
                                    highscore[c]=highscore[c-1];
                                    highscore[c-1]=temp;

                                    char tempc[20];
                                    strcpy(tempc,topscorer[c]);
                                    strcpy(topscorer[c],topscorer[c-1]);
                                    strcpy(topscorer[c-1],tempc);
                                }
                            }
                        }
                        break;
                    }
                }
                if(c==5){//if username doesn't match
                    for(c=4;c>j;c--){
                        highscore[c]=highscore[c-1];
                        strcpy(topscorer[c],topscorer[c-1]);
                    }
                    highscore[j]=score;
                    strcpy(topscorer[j],name);
                }
            }

            hs=fopen("high score.txt","w");//rewrite leaderboard
            for(j=0;j<5;j++){
                fprintf(hs,"%s %d\n",topscorer[j],highscore[j]);
            }
            fclose(hs);

            hs=fopen("high score.txt","r");//take input to dispay leaderboard
            for(j=0;j<5;j++){
                fgets(line[j],25,hs);
            }
            fclose(hs);
            show_highscore=1;
        }
    }
}

void iSpecialKeyboard(unsigned char key)
{
    if(!pausemode){
        if(key==GLUT_KEY_LEFT)
    {
        if(xBoard>0)
        {
            xBoard-=20;
            if(!isStarted)
            {
                xBall-=20;
            }
        }
    }
    else if(key==GLUT_KEY_RIGHT)
    {
        if(xBoard<screenWidth-dxBoard)
        {
            xBoard+=20;
            if(!isStarted)
            {
                xBall+=20;
            }
        }
    }
    }
}
void iMouseMove(int mx, int my) {

}

void iMouse(int button, int state, int mx, int my) {

    int j;

    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{

		if(menumode){
		    if(mx>=480 && mx <=714 && my >=406 && my <=456){//clicking newgame
                menumode=0;
                username_mode=1;
		    }

			else if(mx>=480 && mx<=714 && my>=337 && my<=384){//clicking resumegame
                loadgame_flag=1;
                menumode=0;
                username_mode=1;
			}

			else if(mx>=480 && mx<=714 && my>=265 && my<=314){//clicking highscore

                menumode=0;

                hs=fopen("high score.txt","r");
                for(j=0;j<5;j++){
                    fgets(line[j],25,hs);
                }
                fclose(hs);

                show_highscore=1;
			}

			else if(mx>=480 && mx<=714 && my>=193 && my<=240){// clicking quitgame
                exit(0);
			}
		}
        else if(username_mode==1 && mx>=450 && mx<=700 && my>=350 && my<=390){
                writing_mode=1;
        }
         else if(pausemode){
            if(mx>=427 && mx<=510 && my>=237 && my<=311){//click play
                pausemode=0;
                play_mode=1;
                isStarted=1;
            }
            if(mx>=560 && mx<=643 && my>=237 && my<=311){//click restart
                remove(filename);
                reset();
                play_mode=1;
                pausemode=0;
            }
            if(mx>=693 && mx<=776 && my>=237 && my<=311){//click go to menu
                pausemode=0;
                wantTOsave=1;
            }
         }
    }
}

void ballChange()
{
    if(isStarted)
    {
        xBall+=dx;
        yBall+=dy;
        if(xBall>=screenWidth||xBall<=0)
            dx*=(-1);                  //direction of motion along x axis will change
        if(yBall>=screenHeight||yBall<=0)
            dy*=(-1);                  //direction of motion along y axis will change

    //collision
    for(int i=0;i<totalBricks;i++)
    {
        if(bricks[i].show)
        {
            if(xBall>=bricks[i].x&&xBall<=bricks[i].x+bricks[i].dx &&yBall>=bricks[i].y&&yBall<=bricks[i].y+bricks[i].dy)
            {

                    dy*=(-1);
                    bricks[i].show=0;
                    score++;
                    if(score==150){
                        play_mode=0;
                        congrats=1;
                    }

            }
        }
    }
    if(xBall>=xBoard&&xBall<=xBoard+dxBoard&&yBall>=yBoard&&yBall<=yBoard+dyBoard)//collision with
    {                                                                              //board
        dy*=(-1);
    }
    else
    {
        if(yBall<yBoard)//if ball touches the ground
        {
            life--;
            xBall =(int)screenWidth/2;
            yBall =yAxisMargin;
            xBoard=xBall-50;
            yBoard=yBall-20;
            isStarted=0;
            if(life==0){
                play_mode=0;
                gameover=1;
                remove(filename);
            }
        }
    }
    }
}

void color_E(int co)
{
    bricks[co].red=52;
    bricks[co].green=152;
    bricks[co].blue=219;
}
void color_C(int co)
{
    bricks[co].red=231;
    bricks[co].green=76;
    bricks[co].blue=60;
}
void color_S(int co)
{
    bricks[co].red=175;
    bricks[co].green=122;
    bricks[co].blue=197;
}
void brick_for_E(int sumx,int sumy)
{
    int temp=i+2;
    for(;i<=temp;i++)
    {
        color_E(i);
        bricks[i].x=sumx+=40;
        bricks[i].y=sumy;
    }
}
void brick_for_C(int sumx,int sumy)
{
    int temp=i+2;
    for(;i<=temp;i++)
    {
        color_C(i);
        bricks[i].x=sumx+=40;
        bricks[i].y=sumy;
    }
}
void definebricks_for_E()
{
    int sumx=700;
    int sumy=310;


    for(i=0;i<=23;i++)
    {
        color_E(i);
        bricks[i].x=sumx;
        bricks[i].y=sumy+=10;
    }


    brick_for_E(sumx,sumy);


    sumy=sumy-10;
    brick_for_E(sumx,sumy);

    sumy=sumy-10;
    brick_for_E(sumx,sumy);

    sumy=sumy-85;
    brick_for_E(sumx,sumy);


    sumy=sumy-10;
    brick_for_E(sumx,sumy);


    sumy=sumy-10;
    brick_for_E(sumx,sumy);


    sumy=sumy-85;
    brick_for_E(sumx,sumy);

    sumy=sumy-10;
    brick_for_E(sumx,sumy);

    sumy=sumy-10;
    brick_for_E(sumx,sumy);
}
void definebricks_for_C()
{
    int sumx=200;
    int sumy=310;


    for(;i<=74;i++)
    {
        color_C(i);
        bricks[i].x=sumx;
        bricks[i].y=sumy+=10;
    }
    int temp=sumx;
    brick_for_C(sumx,sumy);


    sumx=temp;
    sumy=sumy-10;
    brick_for_C(sumx,sumy);


    sumx=temp;
    sumy=sumy-10;
    brick_for_C(sumx,sumy);

     sumx=temp;
    sumy=320;
    brick_for_C(sumx,sumy);


    sumx=temp;
    sumy=sumy+10;
    brick_for_C(sumx,sumy);

    sumx=temp;
    sumy=sumy+10;
    brick_for_C(sumx,sumy);
}
void brick_for_S(int sumx,int sumy)
{
    int temp=i+3;
    for(;i<=temp;i++)
    {
        color_S(i);
        bricks[i].x=sumx+=40;
        bricks[i].y=sumy;
    }
}
void definebricks_for_S()
{
    int sumx=420;
    int sumy=320;


    brick_for_S(sumx,sumy);

    sumy=sumy+10;
    brick_for_S(sumx,sumy);



    sumy=sumy+10;
    brick_for_S(sumx,sumy);


    sumy=sumy+85;
    brick_for_S(sumx,sumy);

    sumy=sumy+10;
    brick_for_S(sumx,sumy);

    sumy=sumy+10;
    brick_for_S(sumx,sumy);

    sumy=sumy+85;
    brick_for_S(sumx,sumy);

    sumy=sumy+10;
    brick_for_S(sumx,sumy);


    sumy=sumy+10;
    brick_for_S(sumx,sumy);

    int temp=sumx;

    sumx=sumx+160;
    sumy=320;
    for(;i<=139;i++)
    {
        color_S(i);
        bricks[i].x=sumx;
        bricks[i].y=sumy+=10;

    }

    sumx=temp+40;
    sumy=445;
    for(;i<=149;i++)
    {
        color_S(i);
        bricks[i].x=sumx;
        bricks[i].y=sumy+=10;

    }
}
void  definebricks()
{
    int flag;

    for(flag=0;flag<150;flag++)
    {

        bricks[flag].dx=40;
        bricks[flag].dy=10;

    }
    definebricks_for_E();
    definebricks_for_C();
    definebricks_for_S();
}

int main()
{
    i=0;

    definebricks();

    iSetTimer(10,ballChange);

    iInitialize(screenWidth,screenHeight,"DX Ball lite");

    return 0;
}
