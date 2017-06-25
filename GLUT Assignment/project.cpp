//Tsalesis Evangelos
//AM: 1779

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <stdio.h>
#include <stdlib.h>

#include <math.h>
#include <time.h>


#define NORTH 	0
#define WEST 	1	
#define SOUTH 	2
#define EAST	3

#define BLUE 	0
#define YELLOW 	1	
#define RED 	2
#define GREEN	3
#define MAGENTA 4
#define WHITE 	5

GLfloat black[] = {0.0, 0.0, 0.0, 1.0};
GLfloat white[] = {1.0, 1.0, 1.0, 1.0};
GLfloat yellow[] = {1.0, 1.0, 0.0, 1.0};
GLfloat red[] = {1.0, 0.0, 0.0, 1.0};
GLfloat green[] = {0.0, 1.0, 0.0, 1.0};
GLfloat blue[] = {0.0, 0.0, 1.0, 1.0};
GLfloat magenta[] = {1.0, 0.0, 1.0, 1.0};


GLfloat ambientLight[] = {0.3, 0.3, 0.3, 1.0};
GLfloat diffuseLight[] = {1.0, 1.0, 1,0, 1.0};
GLfloat specularLight[] = {1.0, 1.0, 1,0, 1.0};
GLfloat cutoffLight = 90.0;
GLfloat exponentLight = 70;


bool flagMode3 = false;

int h,w;
GLfloat look = 0;
char  s[100];
float res = 10.00;

int N;

int* gameMatrix;
int* holes;

int playerPos[3];
char playerDir;

int points;
int lives;
int reserve;

bool flagTop;

float angleTurn = 180.00;

float movePosX = 0.0;
float movePosZ = 0.0;
int moveAngle  = 0.0;

GLfloat angleX = 0.0, angleY = 0.0;
GLfloat previousAngleX = 0.0, previousAngleY = 0.0;
int xOrigin = -1, yOrigin = -1;


//-----------------------------DELAY FUNC-------------------------------
void wait(float seconds)
{
	clock_t endwait;
	endwait = clock() + seconds * CLOCKS_PER_SEC;
	while (clock() < endwait) {}
}


//---------------------GAMEPLAY MATRICES MANIPULATION-------------------
// d3:down->up, d2:north->west, d1:west->east
int T(int d3, int d2, int d1){
	return N*N*d3 + N*d2 + d1;
}

int T2(int d2, int d1){
	return N*d2 + d1;
}

void playerPositionSet(int d3, int d2, int d1){
	playerPos[0] = d3;
	playerPos[1] = d2;
	playerPos[2] = d1;
	return;
}

bool playerPositionCheck(int d3, int d2, int d1){
	if (playerPos[0] == d3 && playerPos[1] == d2 && playerPos[2] == d1){
		return true;
	}
	return false;
}


//-------------------GAMEPLAY INITIALIZATION--------------------------------
void restoreGame(){
	points = 50;
	lives = 3;
	reserve = 0;
	playerDir = NORTH;
	angleTurn = 180.00;
	bool flagTop = false;

	for (int i=0;i<N;i++){
		for (int j=0;j<N;j++){
			holes[T2(i,j)] = 0;
			for (int k=0;k<N;k++){
				if (i == 0){
					gameMatrix[T(i,j,k)] = rand() % 4;
				}
				else {
					gameMatrix[T(i,j,k)] = -1;
				}
			}
		}
	}

	gameMatrix[T(0, N/2, N/2)] = MAGENTA;
	
	playerPositionSet(1,N/2,N/2);
}


//------------------------------PROJECTION--------------------------------
void changeSize(int ww, int hh){
	h = hh;
	w = ww;

	if (h==0){
		h = 1;
	}
	float ratio = w * 1.0/ h;
	glMatrixMode(GL_PROJECTION);
	
	glLoadIdentity();
	
	glViewport(0, 0, w, h);
	gluPerspective(60.0, ratio, 0.1, 50*N);

	glMatrixMode(GL_MODELVIEW);
}

void setOrthographicProjection(){
	glMatrixMode(GL_PROJECTION);
	
	glPushMatrix();
	glLoadIdentity();

	gluOrtho2D(0,w,h,0);

	glMatrixMode(GL_MODELVIEW);
}

void restorePerspectiveProjection(){
	glMatrixMode(GL_PROJECTION);
	
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
}


//-------------------------------RENDERING------------------------------
//DRAW A FACE 1X1, (RES X RES): THE NUMBER OF POLYGONS
void drawFace(){
	glPushMatrix();
	glTranslatef(-0.5, -0.5, 0.0);
	
	glNormal3f(0, 0, 1.0);
	for (int i=0;i<res;i++){
		for (int j=0;j<res;j++){
			glBegin(GL_QUADS);
				glVertex3f(j/res, i/res, 0.0);
				glVertex3f((j+1)/res, i/res, 0.0);
				glVertex3f((j+1)/res, (i+1)/res, 0.0);
				glVertex3f(j/res, (i+1)/res, 0.0);	
			glEnd();
		}
	}
	glPopMatrix();

}

//RENDER A BOX
void drawBox(int color){
	switch (color){
		case BLUE:
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, blue);
    			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, blue);
     			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, white);
			glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 56);
			break;
		case YELLOW:
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, yellow);
    			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, yellow);
     			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, white);
			glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 56);
			break;
		case RED:
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, red);
    			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, red);
     			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, white);
			glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 56);
			break;
		case GREEN:
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, green);
    			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, green);
     			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, white);
			glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 56);
			break;
		case MAGENTA:
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, magenta);
    			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, magenta);
     			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, white);
			glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 56);
			break;
		case WHITE:
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, white);
    			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, white);
     			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, white);
			glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 56);
			break;
	}

	//FRONT
	glPushMatrix();	
	glTranslatef(0.0, 0.0, 0.5);
	drawFace();
	glPopMatrix();
	
	//BOTTOM
	glPushMatrix();
	glTranslatef(0.0, 0.0, -0.5);
	glRotatef(180, 0.0, 1.0, 0.0);
	drawFace();
	glPopMatrix();

	//LEFT
	glPushMatrix();
	glTranslatef(-0.5, 0.0, 0.0);
	glRotatef(-90, 0.0, 1.0, 0.0);
	drawFace();
	glPopMatrix();
	

	//RIGHT
	glPushMatrix();
	glTranslatef(0.5, 0.0, 0.0);
	glRotatef(90, 0.0, 1.0, 0.0);
	drawFace();
	glPopMatrix();

	//TOP
	glPushMatrix();
	glTranslatef(0.0, 0.5, 0.0);
	glRotatef(-90, 1.0, 0.0, 0.0);
	drawFace();
	glPopMatrix();

	//BOTTOM
	glPushMatrix();
	glTranslatef(0.0, -0.5, 0.0);
	glRotatef(90, 1.0, 0.0, 0.0);
	drawFace();
	glPopMatrix();

}

//DRAW AVATAR
void drawAvatar(){
	glRotatef(angleTurn, 0.0, 1.0, 0.0);

	//Torch
	GLfloat lightDirection5[] = {0.0, -0.25, 1.0};
	glLightfv(GL_LIGHT5, GL_SPOT_DIRECTION, lightDirection5);


	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, white);
    	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, white);
     	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, white);


	//head/eyes
	glPushMatrix();

	glTranslatef(0.0,0.35,0.0);
	glutSolidSphere(0.15, 20, 20);

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, black);
    	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, black);
     	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, black);
	glTranslatef(0.03, 0.06, 0.1125);
	glutSolidSphere(0.03, 10, 10);
	glTranslatef(-0.06, 0.0, 0.0);
	glutSolidSphere(0.03, 10, 10);

	glPopMatrix();	

	//body
	glPushMatrix();
	glScalef(0.4, 0.45, 0.2);
	drawBox(WHITE);
	glPopMatrix();	
	
	//right hand
	glPushMatrix();
	glTranslatef(0.25,0.01,0.0);	
	glTranslatef(0.0,0.1333*(1-cos(-moveAngle*3.14/180.00)),0.1333*sin(-moveAngle*3.14/180.00));
	glRotatef(moveAngle, 1.0, 0.0, 0.0);	

	GLfloat lightPosition5[] = {0.0, -0.05, 0.0, 1.0};
	glLightfv(GL_LIGHT5, GL_POSITION, lightPosition5);

	glScalef(0.1, 0.4, 0.1);
	drawBox(WHITE);
	glPopMatrix();	

	//left hand
	glPushMatrix();
	glTranslatef(-0.25,0.01,0.0);
	glTranslatef(0.0,0.1333*(1-cos(moveAngle*3.14/180.00)),0.1333*sin(moveAngle*3.14/180.00));
	glRotatef(-moveAngle, 1.0, 0.0, 0.0);
	glScalef(0.1, 0.4, 0.1);
	drawBox(WHITE);
	glPopMatrix();

	//right feet
	glPushMatrix();
	glTranslatef(0.1,-0.375,0.0);
	glTranslatef(0.0,0.15*(1-cos(moveAngle*3.14/180.00)),0.15*sin(moveAngle*3.14/180.00));
	glRotatef(-moveAngle, 1.0, 0.0, 0.0);	
	glScalef(0.12, 0.4, 0.12);
	drawBox(WHITE);
	glPopMatrix();

	//left feet
	glPushMatrix();
	glTranslatef(-0.1,-0.375,0.0);
	glTranslatef(0.0,0.15*(1-cos(-moveAngle*3.14/180.00)),0.15*sin(-moveAngle*3.14/180.00));
	glRotatef(moveAngle, 1.0, 0.0, 0.0);	
	glScalef(0.12, 0.4, 0.12);
	drawBox(WHITE);
	glPopMatrix();

}

//RENDER TEXT IN HUD
void renderBitmapString(float x, float y, float z, void *font, char *string){
	char *c;
	glRasterPos3f(x,y,z);
	glColor3f(1.0, 1.0, 1.0);
	for (c=string; *c != '\0'; c++){
		glutBitmapCharacter(font, *c);
	}
}

//RENDER HUD
void renderHUD(){
	glDisable(GL_LIGHTING);
	
	if (flagMode3){
		sprintf(s,"Points: %d  Lives: %d  Reserve: %d     Level: %d/%d", points, lives, reserve, playerPos[0]+1, N);
	}
	else{
		sprintf(s,"Points: %d  Lives: %d  Reserve: %d     Level: %d/%d", points, lives, reserve, playerPos[0]+1, N);
	}

	setOrthographicProjection();
	
	glPushMatrix();
	glLoadIdentity();
	renderBitmapString(30,30, 0, GLUT_BITMAP_HELVETICA_18,s);
	glPopMatrix();
	restorePerspectiveProjection();

	glEnable(GL_LIGHTING);
}

//----MAIN RENDERING ROUTINE-----
void renderScene(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	//camera
	if (flagMode3 == true){
		gluLookAt(1.0*N,1.0*N,1.0*N,  0.0, N/6.00, 0.0,   0.0, 1.0, 0.0);

		glPushMatrix();	
		glRotatef(angleX, 0.0,1.0,0.0);
		//glRotatef(angleY, sin(angleX*3.14/180.00-(3.14/4.00)) ,0.0, -sin(angleX*3.14/180.00-(3.14/4.00)));		//debugging
	
		glTranslatef(-N/2.0+0.5, 0.0, -N/2.0+0.5);
	}
	else {	
		gluLookAt(playerPos[2]+movePosX, playerPos[0], playerPos[1]+movePosZ,   playerPos[2]+movePosX+sin(angleTurn*3.14/180.00), playerPos[0]+look, playerPos[1]+movePosZ+cos(angleTurn*3.14/180.00),   0.0, 1.0, 0.0);
	}

	//lights
	GLfloat lightPosition1[] = {0, N+1, 0, 1.0};
	GLfloat lightPosition2[] = {0, N+1, N, 1.0};
	GLfloat	lightPosition3[] = {N, N+1, 0, 1.0};
	GLfloat	lightPosition4[] = {N, N+1, N, 1.0};

	glLightfv(GL_LIGHT1, GL_POSITION, lightPosition1);
	glLightfv(GL_LIGHT2, GL_POSITION, lightPosition2);
	glLightfv(GL_LIGHT3, GL_POSITION, lightPosition3);
	glLightfv(GL_LIGHT4, GL_POSITION, lightPosition4);

	GLfloat lightDirection[] = {0.0, -1.0, 0.0};
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, lightDirection);
	glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, lightDirection);
	glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, lightDirection);
	glLightfv(GL_LIGHT4, GL_SPOT_DIRECTION, lightDirection);

	
	
	//gametable and avatar
	for (int i=0; i<N; i++){
		for (int j=0; j<N; j++){
			for (int k=0; k<N; k++){
				if (flagMode3 == true){
					if (playerPositionCheck(i,j,k)){
						glPushMatrix();
						glTranslatef(k*1.0+movePosX, i*1.0, j*1.0+movePosZ);	
						drawAvatar();
						glPopMatrix();
					}
				}
				if (gameMatrix[T(i,j,k)] != -1){
					glPushMatrix();
					glTranslatef(k*1.0, i*1.0, j*1.0);
					drawBox(gameMatrix[T(i,j,k)]);
					glPopMatrix();
				}
			}
		}
	}
	if (flagMode3 == true){
		glPopMatrix();

		glEnable(GL_LIGHT5);
	}
	
	//hud
	renderHUD();

	glutSwapBuffers();
}	


//--------------------------GAMEOVER-----------------------------------
//GAMEOVER RENDER
void renderGameover(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	glDisable(GL_LIGHTING);

	sprintf(s,"GAMEOVER! Points: %d", points);
	setOrthographicProjection();
	
	glPushMatrix();
	glLoadIdentity();
	renderBitmapString(w/2-130,h/2-50, 0, GLUT_BITMAP_HELVETICA_18,s);
	sprintf(s,"Press ESC to exit.");
	renderBitmapString(w/2-75,h/2-20, 0, GLUT_BITMAP_HELVETICA_10,s);
	glPopMatrix();
	restorePerspectiveProjection();

	glutSwapBuffers();
	
}
//GAMEOVER KEYBOARD
void processNormalKeysGameover(unsigned char key, int x, int y){
	switch (key) {
		case 27:
			exit(0);
			break;
	}
}


//--------------------------GAMEPLAY-----------------------------------
//-----KEYBOARD-----
void processNormalKeys(unsigned char key, int x, int y){
	bool flagPlayerNoCollapse = false;
	bool flagCollapsEnd = false;
	bool flagCollapsEvent = false;
	bool flagDead = false;
	bool flagDestroyEvent = false;

	int countFalls = 0;
	
	switch (key) {
		case 27:
			exit(0);
			break;
		case 'w':
		case 'W':
			//MOVE
			switch (playerDir) {
				case NORTH:
					if (playerPos[1] > 0){
						//MOVE FORWARD
						if (gameMatrix[T(playerPos[0], playerPos[1]-1, playerPos[2])] == -1){
							for (int i=0; i<20;i++){
								movePosZ -= 0.05;
								if (i<5 || i>=15){ moveAngle += 12;}
								else { moveAngle -=12;}
								renderScene();
								wait(0.025);
							}
							movePosZ = 0;
	
							playerPos[1] -= 1;
						}
					     	//MOVE UP
						else {
							if (playerPos[0] < N-1){
								if (gameMatrix[T(playerPos[0]+1, playerPos[1]-1, playerPos[2])] == -1){
									playerPos[0] += 1; playerPos[1] -= 1;

									points += 5;
								}
								//MOVE TO TOP
								if (playerPos[0] == N-1 && flagTop == false) {
									points += 100;
									lives += 1;
									flagTop = true;
								}
							}
						}
					}
					break;
				case SOUTH:
					if (playerPos[1] < N-1){
						if (gameMatrix[T(playerPos[0], playerPos[1]+1, playerPos[2])] == -1){ 
							for (int i=0; i<20;i++){
								movePosZ += 0.05;
								if (i<5 || i>=15){ moveAngle += 12;}
								else { moveAngle -=12;}
								renderScene();
								wait(0.025);
							}
							movePosZ = 0;

							playerPos[1] += 1;
						}
						else {
							if (playerPos[0] < N-1){
								if (gameMatrix[T(playerPos[0]+1, playerPos[1]+1, playerPos[2])] == -1){
									playerPos[0] += 1; playerPos[1] += 1;
									
									points += 5;
								}
								if (playerPos[0] == N-1 && flagTop == false) {
									points += 100;
									lives += 1;
									flagTop = true;
								}
							}						
						}
					}
					break;
				case EAST:
					if (playerPos[2] < N-1){
						if (gameMatrix[T(playerPos[0], playerPos[1], playerPos[2]+1)] == -1){
							for (int i=0; i<20;i++){
								movePosX += 0.05;
								if (i<5 || i>=15){ moveAngle += 12;}
								else { moveAngle -=12;}
								renderScene();
								wait(0.025);
							}
							movePosX = 0;
 
							playerPos[2] += 1;
						}
						else {
							if (playerPos[0] < N-1){
								if  (gameMatrix[T(playerPos[0]+1, playerPos[1], playerPos[2]+1)] == -1){
									playerPos[0] += 1; playerPos[2] += 1;

									points += 5;
								}
								if (playerPos[0] == N-1 && flagTop == false) {
									points += 100;
									lives += 1;
									flagTop = true;
								}
							}
						}
					}
					break;
				case WEST:
					if (playerPos[2] > 0){
						if (gameMatrix[T(playerPos[0], playerPos[1], playerPos[2]-1)] == -1){ 
							for (int i=0; i<20;i++){
								movePosX -= 0.05;
								if (i<5 || i>=15){ moveAngle += 12;}
								else { moveAngle -=12;}
								renderScene();
								wait(0.025);
							}
							movePosX = 0;
 
							playerPos[2] -= 1;
						}
						else {
							if (playerPos[0] < N-1){
								if  (gameMatrix[T(playerPos[0]+1, playerPos[1], playerPos[2]-1)] == -1){
									playerPos[0] += 1; playerPos[2] -= 1;

									points += 5;
								}
								if (playerPos[0] == N-1 && flagTop == false) {
									points += 100;
									lives += 1;
									flagTop = true;
								}
							}
						}
					}
					break;
		  	}
			//FALL
			while (gameMatrix[T(playerPos[0]-1, playerPos[1], playerPos[2])] == -1){ 
				playerPos[0] -= 1;
				
				if (points - 5*countFalls > 0){ 					
					points = points - 5*countFalls;
					countFalls++;
				}
				else if (flagDead == false) {
					points = 0;
					lives--;
					flagDead = true;
				}
				renderScene();
				renderScene();
				wait(0.25);
				
				//FALL TO A HOLE
				if (playerPos[0] == 0){
					if (flagDead == false){
						lives--;
					}
					gameMatrix[T(0,N/2,N/2)] = 4;
					gameMatrix[T(1,N/2,N/2)] = -1;
					playerPositionSet(1,N/2,N/2);
				}
				//GAMEOVER
				if (lives == 0){
					glutDisplayFunc(renderGameover);
					glutReshapeFunc(changeSize);
					glutKeyboardFunc(processNormalKeysGameover);
					glutMouseFunc(NULL);
					glutMotionFunc(NULL);
					break;
				}
			}
			renderScene();
			break;
		//TURN LEFT
		case 'a': 
		case 'A':	
			for (int i=0; i<10; i++){
				angleTurn += 9;
				if (angleTurn == 360){angleTurn = 0;} 
				renderScene();
				wait(0.025);	
			}
			playerDir = (playerDir + 1) % 4;
			break;
		//TURN RIGHT
		case 'd': 
		case 'D':
			for (int i=0; i<10; i++){
				angleTurn -= 9;
				if (angleTurn == 0){angleTurn = 360;} 
				renderScene();
				wait(0.025);
			}
			if (playerDir == 0){playerDir = 3; break;}
			playerDir = (playerDir - 1) % 4;
			break;
		//TURN BACK
		case 's': 
		case 'S':
			for (int i=0; i<10; i++){
				angleTurn -= 18.0;
				if (angleTurn == 0){angleTurn = 360;} 
				renderScene();
				wait(0.025);
			}
			playerDir = (playerDir + 2) % 4;
			break;
		//DELETE FRONT BOX
		case 'q': 
		case 'Q':
			switch (playerDir) {
				case NORTH:
					if (playerPos[1] > 0){
						gameMatrix[T(playerPos[0],playerPos[1]-1,playerPos[2])] = -1;
					}
					break;
				case SOUTH:
					if (playerPos[1] < N-1){
						gameMatrix[T(playerPos[0],playerPos[1]+1,playerPos[2])] = -1;
					}
					break;
				case EAST:
					if (playerPos[2] < N-1){	
						gameMatrix[T(playerPos[0],playerPos[1],playerPos[2]+1)] = -1;
					}
					break;
				case WEST:
					if (playerPos[2] > 0){
						gameMatrix[T(playerPos[0],playerPos[1],playerPos[2]-1)] = -1;
					}
					break;
		  	} 
			break;
		//DELETE ALL FRONT BOXES
		case 'e': 
		case 'E':
			switch (playerDir) {
				case NORTH:
					if (playerPos[1] > 0 && points >= 20 && gameMatrix[T(0,playerPos[1]-1,playerPos[2])] != 4){				
						for (int i=0;i<N;i++){	
							if (gameMatrix[T(i,playerPos[1]-1,playerPos[2])] != -1) {			
								gameMatrix[T(i,playerPos[1]-1,playerPos[2])] = -1;
								flagDestroyEvent = true;
							}
						}

						holes[T2(playerPos[1]-1,playerPos[2])] = 1;
						
						if (flagDestroyEvent){	
							points -= 20;
							lives += 1;
						}
					}
					break;
				case SOUTH:
					if (playerPos[1] < N-1 && points >= 20 && gameMatrix[T(0,playerPos[1]+1,playerPos[2])] != 4){
						for (int i=0;i<N;i++){
							if (gameMatrix[T(i,playerPos[1]+1,playerPos[2])] != -1) {
								gameMatrix[T(i,playerPos[1]+1,playerPos[2])] = -1;
								flagDestroyEvent = true;
							}
						}
						
						holes[T2(playerPos[1]+1,playerPos[2])] = 1;
			
						if (flagDestroyEvent){	
							points -= 20;
							lives += 1;
						}
					}
					break;
				case EAST:
					if (playerPos[2] < N-1 && points >= 20 && gameMatrix[T(0,playerPos[1],playerPos[2]+1)] != 4){
						for (int i=0;i<N;i++){
							if (gameMatrix[T(i,playerPos[1],playerPos[2]+1)] != -1) {
								gameMatrix[T(i,playerPos[1],playerPos[2]+1)] = -1;
								flagDestroyEvent = true;
							}
						}

						holes[T2(playerPos[1],playerPos[2]+1)] = 1;
							
						if (flagDestroyEvent){	
							points -= 20;
							lives += 1;
						}
					}
					break;
				case WEST:
					if (playerPos[2] > 0 && points >= 20 && gameMatrix[T(0,playerPos[1],playerPos[2]-1)] != 4){
						for (int i=0;i<N;i++){
							if (gameMatrix[T(i,playerPos[1],playerPos[2]-1)] != -1) {
								gameMatrix[T(i,playerPos[1],playerPos[2]-1)] = -1;
								flagDestroyEvent = true;
							}
						}

						holes[T2(playerPos[1],playerPos[2]-1)] = 1;

						if (flagDestroyEvent) {
							points -= 20;
							lives += 1;
						}
					}
					break;
		  	} 
			break;
		//COLLAPSE BOXES
		case 'r': 
		case 'R':
			// when player on top of a collapsing box
			for (int i=playerPos[0]-1;i>0;i--){
				if (gameMatrix[T(i, playerPos[1], playerPos[2])] == -1){
					flagPlayerNoCollapse = true;
				}
			}
			// when player below of a collapsing box
			for (int i=playerPos[0]+1;i<N;i++){
				if (gameMatrix[T(i, playerPos[1], playerPos[2])] != -1){
					flagPlayerNoCollapse = true;
				}
			}
			// when player on top of a box above a hole
			if (holes[T2(playerPos[1], playerPos[2])] == 1){
				flagPlayerNoCollapse = true;
			}
			if (flagPlayerNoCollapse == false) {
				while (flagCollapsEnd == false){
					flagCollapsEnd = true;

					//DESTROY BOXES IN HOLES
					for (int j=0;j<N;j++){
						for (int k=0;k<N;k++){
							if (holes[T2(j,k)] == 1){
								gameMatrix[T(0,j,k)] = -1;
							}
						}
					}	

					//MOVE COLLAPSING BOXES DOWNWARD A STEP 
					for (int i=1;i<N;i++){
						for (int j=0;j<N;j++){
							for (int k=0;k<N;k++){
								if (gameMatrix[T(i,j,k)] != -1 && gameMatrix[T(i-1,j,k)] == -1){
									gameMatrix[T(i-1,j,k)] = gameMatrix[T(i,j,k)];
									gameMatrix[T(i,j,k)] = -1;
									flagCollapsEnd = false;

									flagCollapsEvent = true;
								}
							}
						}
					}
					renderScene();
					wait(0.25);	//COLLAPSE ANIMATION DELAY
				}
			}
			if (flagCollapsEvent == true){
				points += 10;
			}
			break;
		//RESTORE GAME
		case 'p':
		case 'P':
			restoreGame();
			break;
		//INCREASE BOX RESERVE
		case ' ':
			if (gameMatrix[T(playerPos[0]-1, playerPos[1], playerPos[2])] > 0 && gameMatrix[T(playerPos[0]-1, playerPos[1], playerPos[2])] < 4 && points >= 5){
				gameMatrix[T(playerPos[0]-1, playerPos[1], playerPos[2])] -= 1;

				reserve += 1;		
				points -= 5;
			}
			break;
			
	}

	glutPostRedisplay();
}

//Special Keys
void pressKey(int key, int xx, int yy) {
	
	switch (key) {
		case GLUT_KEY_LEFT: 
			
			break;
		case GLUT_KEY_RIGHT: 
			
			break;
		//LOOK UP IN 1ST PERSON
		case GLUT_KEY_UP: 
			if (flagMode3 == false){
				if (look < 1.0){
					look += 0.25;
				}
			}
			break;
		//LOOK DOWN IN 1ST PERSON
		case GLUT_KEY_DOWN: 
			if (flagMode3 == false){
				if (look > -1.0){
					look -= 0.25;
				}
			}
			break;
		//CHANGE GAME MODE (1ST/3RD PERSON)
		case GLUT_KEY_F1: 
			if (flagMode3 == false){flagMode3 = true; renderScene();}
			else {flagMode3 = false;}
			break;
		//CHANGE POLYGONS		
		case GLUT_KEY_F5: 
			if (res == 1.00){
				res = 5.00;
			}
			else if (res == 20.00){
				res = 1.00;
			}
			else {
				res = res * 2.00;
			}
			break;
	}
	glutPostRedisplay();
}


//-----MOUSE-----
void mouseButton(int button, int state, int x, int y) {
	//ADD BOX
	if (button==GLUT_LEFT_BUTTON){
		if (state==GLUT_UP){
			if (reserve > 0){ 
				switch (playerDir) {
					case NORTH:
						if (playerPos[1] > 0){
							for (int i=playerPos[0]; i<N; i++){
								if (gameMatrix[T(i,playerPos[1]-1,playerPos[2])] == -1){
									gameMatrix[T(i,playerPos[1]-1,playerPos[2])] = rand() % 4;

									reserve -= 1;
									points += 5;
									break;
								}
							}
						}
						break;
					case SOUTH:
						if (playerPos[1] < N-1){
							for (int i=playerPos[0]; i<N; i++){
								if (gameMatrix[T(i,playerPos[1]+1,playerPos[2])] == -1){
									gameMatrix[T(i,playerPos[1]+1,playerPos[2])] = rand() % 4;
	
									reserve -= 1;
									points += 5;
									break;
								}
							}
						}
						break;
					case EAST:
						if (playerPos[2] < N-1){
							for (int i=playerPos[0]; i<N; i++){
								if (gameMatrix[T(i,playerPos[1],playerPos[2]+1)] == -1){
									gameMatrix[T(i,playerPos[1],playerPos[2]+1)] = rand() % 4;

									reserve -= 1;
									points += 5;
									break;
								}
							}
						}
						break;
					case WEST:
						if (playerPos[2] > 0){
							for (int i=playerPos[0]; i<N; i++){
								if (gameMatrix[T(i,playerPos[1],playerPos[2]-1)] == -1){
									gameMatrix[T(i,playerPos[1],playerPos[2]-1)] = rand() % 4;
					
									reserve -= 1;
									points += 5;
									break;
								}
							}
						}	
						break;
				} 
			}
		}
	}
	//ROTATION IN 3RD PERSON
	if (button==GLUT_MIDDLE_BUTTON){
		if (flagMode3){
			if (state==GLUT_UP){
				previousAngleX = angleX;
				previousAngleY = angleY;		
				xOrigin = -1;
				yOrigin = -1;
			}
			else{
				xOrigin = x;
				yOrigin = y;
			}
		}
	}
	// KICK BOX
	if (button==GLUT_RIGHT_BUTTON){
		if (state==GLUT_UP){
			switch (playerDir) {
				case NORTH:
					if (playerPos[1] > 0){
						if (gameMatrix[T(playerPos[0],playerPos[1]-1,playerPos[2])] != -1){
							int temp1 = -1;
							int temp2 = -1;
							for (int i=playerPos[1]-1;i>=0;i--){
								temp1 = gameMatrix[T(playerPos[0],i,playerPos[2])];
								gameMatrix[T(playerPos[0],i,playerPos[2])] = temp2;
								if (temp1 == -1) {
									break;
								}
								temp2 = temp1;
							}
						}
					}
					break;
				case SOUTH:
					if (playerPos[1] < N-1){
						if (gameMatrix[T(playerPos[0],playerPos[1]+1,playerPos[2])] != -1){
							int temp1 = -1;
							int temp2 = -1;
							for (int i=playerPos[1]+1;i<N;i++){
								temp1 = gameMatrix[T(playerPos[0],i,playerPos[2])];
								gameMatrix[T(playerPos[0],i,playerPos[2])] = temp2;
								if (temp1 == -1) {
									break;
								}
								temp2 = temp1;
							}
						}
					}
					break;
				case EAST:
					if (playerPos[2] < N-1){
						if (gameMatrix[T(playerPos[0],playerPos[1],playerPos[2]+1)] != -1){
							int temp1 = -1;
							int temp2 = -1;
							for (int i=playerPos[2]+1;i<N;i++){
								
								temp1 = gameMatrix[T(playerPos[0],playerPos[1],i)];
								gameMatrix[T(playerPos[0],playerPos[1],i)] = temp2;
								if (temp1 == -1) {
									break;
								}
								temp2 = temp1;
							}
						}
					}
					break;
				case WEST:
					if (playerPos[2] > 0){
						if (gameMatrix[T(playerPos[0],playerPos[1],playerPos[2]-1)] != -1){
							int temp1 = -1;
							int temp2 = -1;
							for (int i=playerPos[2]-1;i>=0;i--){

								temp1 = gameMatrix[T(playerPos[0],playerPos[1],i)];
								gameMatrix[T(playerPos[0],playerPos[1],i)] = temp2;
								if (temp1 == -1) {
									break;
								}
								temp2 = temp1;
							}
						}
					}
					break;
		  	} 
		}
	}

	glutPostRedisplay();
}	

//MOVE CAMERA
void mouseMove(int x, int y){
	if (flagMode3){
		if (xOrigin >= 0){
			angleX = previousAngleX + (x - xOrigin) * 0.1 ;
		}
		if (yOrigin > 0){	
			angleY = previousAngleY + (y - yOrigin) * 0.1;
		}

		glutPostRedisplay();
	}
}


//---------------------INITIALIZATION--------------------------
//RENDER INIT
void init(){
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);

	glutIgnoreKeyRepeat(1);

	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(pressKey);
	glutMouseFunc(mouseButton);
	glutMotionFunc(mouseMove);

	glEnable(GL_NORMALIZE);
	glShadeModel(GL_SMOOTH);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
}

//LIGHT INIT
void initLighting(){
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);
	glEnable(GL_LIGHT3);
	glEnable(GL_LIGHT4);

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT,ambientLight);

	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT3, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT4, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT5, GL_DIFFUSE, diffuseLight);

	glLightfv(GL_LIGHT1, GL_SPECULAR, specularLight);
	glLightfv(GL_LIGHT2, GL_SPECULAR, specularLight);
	glLightfv(GL_LIGHT3, GL_SPECULAR, specularLight);
	glLightfv(GL_LIGHT4, GL_SPECULAR, specularLight);
	glLightfv(GL_LIGHT5, GL_SPECULAR, specularLight);

	glLightf(GL_LIGHT1,GL_SPOT_CUTOFF, cutoffLight); 
	glLightf(GL_LIGHT2,GL_SPOT_CUTOFF, cutoffLight); 
	glLightf(GL_LIGHT3,GL_SPOT_CUTOFF, cutoffLight); 
	glLightf(GL_LIGHT4,GL_SPOT_CUTOFF, cutoffLight);
	glLightf(GL_LIGHT5,GL_SPOT_CUTOFF, cutoffLight);

	glLightf(GL_LIGHT1,GL_SPOT_EXPONENT,exponentLight); 
	glLightf(GL_LIGHT2,GL_SPOT_EXPONENT,exponentLight); 
	glLightf(GL_LIGHT3,GL_SPOT_EXPONENT,exponentLight); 
	glLightf(GL_LIGHT4,GL_SPOT_EXPONENT,exponentLight);
	glLightf(GL_LIGHT5,GL_SPOT_EXPONENT,exponentLight);

}


//--------------------------MAIN------------------------------
int main (int argc, char** argv){
	//input loop
	bool flag;
	do {	
		flag = true;
		printf("Give the number of levels (odd positive number, above 1, below 20): ");	
		if (scanf("%d", &N) != 1){
			while(getchar() !='\n') {}
			flag = false;
			continue;
		}
		if (N <= 1 || N >= 20 || N % 2 == 0) { flag = false;}	
	} while (flag == false);
	//----------

	//Gameplay matrices (color of every box)
	gameMatrix = (int*) malloc((N*N*N) * sizeof(int));
	//(coordinates of every hole)
	holes = (int*) malloc((N*N) * sizeof(int));

	//init gameplay
	restoreGame();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(1024,768);
	glutCreateWindow("Game");
	
	initLighting();
	init();

	glutMainLoop();

	return 1;
}
