#include <windows.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#include "gl/gl.h"
#include "gl/glu.h"
#include "gl/glaux.h"

#include "main.h"
#include "game.h"


static t_box box[BOX_MAX];
static t_man man;

static int n_box = 0;
static int pole[N_COL][N_ROW];
static int polebox[N_COL][N_ROW];
static int lines = 0;
static int pobashke = 0;
//static int gamestatus = GAMEMENU;
static int gamestatus = GAME;


/***********************************/
void resize(int width,int height, HGLRC hGLRC, HDC hDC)
{
 
	wglMakeCurrent(hDC, hGLRC);
	glViewport(0,0,width,height);
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	
	/*glOrtho(0, N_COL, 0, N_ROW, -1, 1);*/
	glOrtho(SCREEN_LEFT, SCREEN_RIGHT, SCREEN_BOTTOM, SCREEN_TOP, -1, 1);
	
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	wglMakeCurrent(0,0); 

}  

void rectangle(float x1, float y1, float x2, float y2)
{
	glBegin(GL_POLYGON);
	glVertex3f(x1, y2, 0);
	glVertex3f(x2, y2, 0);
	glVertex3f(x2, y1, 0);
	glVertex3f(x1, y1, 0);
	glEnd();	
}

void rectangletext(float x1, float y1, float x2, float y2)
{
	glBegin(GL_POLYGON);
	glNormal3f(0, 0, 1);
	glVertex3f(x1, y2, 0.8);
	glVertex3f(x2, y2, 0.8);
	glVertex3f(x2, y1, 0.8);
	glVertex3f(x1, y1, 0.8);
	glEnd();	
}

void rectangletex(float x1, float y1, float x2, float y2)
{
	glPushMatrix();
	//glScalef(2, 2, 1);
	glDisable(GL_COLOR_MATERIAL);
	glEnable(GL_TEXTURE_2D);
	glBegin (GL_POLYGON);
	glNormal3f(0,0,1);
	glColor3f(0.4, 0.7, 0.8);
	glTexCoord2d (0, 1); 
	glVertex3f (x1,  y2, 0);
	glTexCoord2d (1, 1); 
	glVertex3f ( x2, y2, 0);
	glTexCoord2d (1, 0); 
	glVertex3f ( x2, y1, 0);
	glTexCoord2d (0, 0); 
	glVertex3f (x1, y1, 0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_COLOR_MATERIAL);
	glPopMatrix();
}

int textout(float x1, float y1, int len, char* string1)
{
	glPushMatrix();
	
	glTranslatef((float)x1, (float)(y1) , 0.2);
//	glScalef(0.5, 0.5, 0.5);
	glListBase(1000);

	glCallLists(len, GL_UNSIGNED_BYTE, (GLvoid *)string1);

	glPopMatrix();
        
	return 0;

}

/***************************************************************************/
// подготовка текстуры
void LoadPic1()
{
	int i, j;
	AUX_RGBImageRec *image; 
	short *pImage = NULL;

#ifdef UNICODE     
	image = auxDIBImageLoad( (LPCWSTR) "nebo.bmp"); 
#else     
	image = auxDIBImageLoad( (LPCSTR) "nebo.bmp"); 
#endif 	

	if (image == NULL)
	{
		return;
	}
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1) ;
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, image->sizeX, image->sizeY,
		GL_RGB, GL_UNSIGNED_BYTE, image->data); 	



    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                 image->sizeX, image->sizeY,     
                 0, GL_RGB, GL_UNSIGNED_BYTE, image->data);
                 //0, GL_RGB, GL_UNSIGNED_BYTE, pic1);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glEnable(GL_TEXTURE_2D);


}


/***********************************/
void box_to_pole()
{
	int i;
	memset(&pole, 0, sizeof(pole));
	for (i=0; i<BOX_MAX; i++)
	{
		if (box[i].status != BOX_OFF)
		{
			pole[box[i].x][box[i].y] = box[i].status;
			polebox[box[i].x][box[i].y] = i;
		}
	}
}

void move_all_boxes()
{
	int i, j;
	for (i=0; i<BOX_MAX; i++)
	{
//		if (box[i].status == BOX_FALLS) 
//		{
			if ((box[i].status == BOX_FALLS) &&
				(man.x == box[i].x) && (man.y == box[i].y-1))
			{
				pobashke++;
			}

			if ((box[i].y > 0) &&
			(pole[box[i].x][box[i].y - 1] != BOX_LIES))
			{
				if (box[i].status != BOX_OFF)
				{
					box[i].y -= 1;
					if ((box[i].y == 0) || (pole[box[i].x][box[i].y - 1] == BOX_LIES))
					{
						box[i].status = BOX_LIES;
					}
				}
			}
			else
			{
				box[i].status = BOX_LIES;
				pole[box[i].x][box[i].y] = BOX_LIES;
			}
//		}

	}
	box_to_pole();
}

void new_box()
{
	/*do
	{
		box[n_box].x = fmod(rand(), N_COL);
	}
	while (pole[box[n_box].x][N_ROW-1] != BOX_OFF);*/
	box[n_box].x = fmod(rand(), N_COL);
	if (pole[box[n_box].x][N_ROW-1] == BOX_OFF)
	{
		box[n_box].y = N_ROW-1;
		box[n_box].status = BOX_FALLS;
		n_box++;
		box_to_pole();
	}
	else
	{
		gamestatus = GAMEOVER;
	}
}

void check_line()
{
	int i, j, k;
	int sum;
	for (j=0; j<N_ROW; j++)
	{
		// count line
		sum=0;
		for (i=0; i<N_COL; i++)
		{
			sum += pole[i][j];
		}
		// if line full
		if (sum == N_COL*2)
		{
			/*for (i=0; i<N_COL; i++)
			{
				for (k=j; k<N_ROW-1; k++)
				{
					pole[i][k] = pole[i][k+1];
				}
			}
			pole[i][N_ROW-1] = BOX_OFF;*/
			for (i=0; i<BOX_MAX; i++)
			{
				if (box[i].y == j)
				{
					box[i].x = BOX_X_INIT;
					box[i].y = BOX_Y_INIT;
					box[i].status = BOX_OFF;

				}
				else
				{
					// it should be always > 0, maybe unless two rows is destroying...
					if (box[i].y > 0)
					{
						box[i].y--;
					}
				}
			}
			n_box -= N_COL;
			// tidy up box-array
			for (i=1; i<BOX_MAX; i++)
			{
				if (box[i].status != BOX_OFF)
				{
					k=i;
					while ((k>0) &&(box[k-1].status == BOX_OFF))
					{
						box[k-1].status = box[k].status;
						box[k-1].x = box[k].x;
						box[k-1].y = box[k].y;
						box[k].status = BOX_OFF;
						box[k].x = BOX_X_INIT;
						box[k].y = BOX_Y_INIT;

						k--;
					}
				}
			}
			lines++;
		}
	}
	box_to_pole();

	if ((man.y > 0) && (pole[(int)man.x][(int)man.y-1] == BOX_OFF))
	{
		man.y -= 1;
	}

}


void timer(void)
{
	int i, j;
	static int count=0;
	count++;
	if (count >= DTICS_MAX * TICS_MIN)
	{
		count = 0;
	}
	if (gamestatus == GAME)
	{
		if (fmod(count, DTICS_BOX_FALLS) == DTICS_FMOD_MOVE)
		{
			move_all_boxes();
			//check_line();
		}
		if (fmod(count, DTICS_BOX_FALLS) == DTICS_FMOD_FALL)
		{
			//move_all_boxes();
			check_line();
		}
	
		if (fmod(count, DTICS_BOX_NEW) == DTICS_FMOD_NEW)
		{
			new_box();
			man.ay = 0;
		}
	}



	return;
}

void draw_man(t_man man)
{
	glPushMatrix();
	glColor3f(1.0, 1.0, 0.5);
	glBegin(GL_POLYGON);
/*	glVertex3f(man.x - HSX, man.y + HSY, 0);
	glVertex3f(man.x + HSX, man.y + HSY, 0);
	glVertex3f(man.x + HSX, man.y - HSY, 0);
	glVertex3f(man.x - HSX, man.y - HSY, 0);*/
	glVertex3f(man.x, man.y + SY, 0);
	glVertex3f(man.x + SX, man.y + SY, 0);
	glVertex3f(man.x + SX, man.y, 0);
	glVertex3f(man.x, man.y, 0);
	glEnd();

	glTranslatef(man.x + HSX, man.y + HSY, 0);
	if (pole[(int)man.x][(int)man.y+3] != BOX_OFF)
	{
		glScalef(1.0, 0.8, 1.0);
	}
	if (pole[(int)man.x][(int)man.y+2] != BOX_OFF)
	{
		glScalef(1.0, 0.7, 1.0);
	}
	if (pole[(int)man.x][(int)man.y+1] != BOX_OFF)
	{
		glScalef(1.0, 0.6, 1.0);
	}
	auxSolidSphere(0.4);

	glRotatef(man.ay, 0, 1, 0);
	glColor3f(0, 0, 0);
	glTranslatef(-0.1, 0.1, 0.4);
	auxSolidSphere(0.1);
	glTranslatef(0.2, 0.0, 0.0);
	auxSolidSphere(0.1);
	
	glPopMatrix();
	return;
}

void drawbox(t_box box)
{
	//char num[3] = {'0', '1', '2'};
	switch (box.status)
	{
	case BOX_FALLS:
		glPushMatrix();
		glColor3f(1.0, 0.0, 0.0);
		glTranslatef(START_X + box.x*SX + HSX, START_Y + box.y*SY + HSY, 0);
		glRotatef(20, -1,-1,0);
		auxSolidCube(0.8);
	/*	glDisable(GL_COLOR_MATERIAL);
		glBegin(GL_POLYGON);
		glNormal3f(0, 0, 1);
		glVertex3f(box.x, box.y + SY, 0.2);
		glVertex3f(box.x + SX, box.y + SY, 0.2);
		glVertex3f(box.x + SX, box.y, 0.2);
		glColor3f(1.0, 1.0, 1.0);
		glVertex3f(box.x, box.y, 0.2);
		glEnd();
		glEnable(GL_COLOR_MATERIAL);*/
	//	glColor3f(1.0, 1.0, 1.0);
	//	textout(	START_X + box.x*SX, START_Y + box.y*SY, 1, &num[pole[box.x][box.y]] );
		glPopMatrix();
		break;
	case BOX_LIES:
		glPushMatrix();
		glColor3f(0.5, 0.0, 0.0);
		glTranslatef(START_X + box.x*SX + HSX, START_Y + box.y*SY + HSY, 0);
		glRotatef(20, -1,-1,0);
		auxSolidCube(0.8);
	/*	glDisable(GL_COLOR_MATERIAL);
		glBegin(GL_POLYGON);
		glNormal3f(0, 0, 1);
		glVertex3f(box.x, box.y + SY, 0.2);
		glVertex3f(box.x + SX, box.y + SY, 0.2);
		glVertex3f(box.x + SX, box.y, 0.2);
		glColor3f(1.0, 1.0, 1.0);
		glVertex3f(box.x, box.y, 0.2);
		glEnd();
		glEnable(GL_COLOR_MATERIAL);*/
	//	glColor3f(1.0, 1.0, 1.0);
	//	textout(	START_X + box.x*SX, START_Y + box.y*SY, 1, &num[pole[box.x][box.y]] );
		glPopMatrix();
		break;
	}

}


void draw_menu_new()
{
	glColor3f(MENU_COLOR_RECT_R, MENU_COLOR_RECT_G, MENU_COLOR_RECT_B);
	rectangletext(MENU_NEW_LEFT, MENU_NEW_TOP, MENU_NEW_RIGHT, MENU_NEW_BOTTOM);
	glColor3f(MENU_COLOR_TEXT_R, MENU_COLOR_TEXT_G, MENU_COLOR_TEXT_B);
	textout(MENU_NEW_LEFT, MENU_NEW_BOTTOM, 10, "НОВАЯ ИГРА");
}

void process_key(char c)
{
	int intx, inty;
	int i, j;

	intx = (int)man.x;
	inty = (int)man.y;
	


	switch(c)
	{
	case VK_LEFT: case '4':
		if ((intx >= 1) && (gamestatus == GAME))
		{
			// horizontal move and box pushing
			switch(man.flag_up)
			{
			case MAN_FLAG_UP_0:
			
			
				if (pole[intx-1][inty] == BOX_OFF)
				{
					man.x -= 1;
				}
				else
				{
					if (intx >=2)
					{
						if (pole[intx-2][inty] == BOX_OFF)
						{
							// move box
							box[polebox[intx-1][inty]].x -= 1;
							if (pole[intx-2][inty-1] != BOX_LIES)
							{
								box[polebox[intx-1][inty]].status = BOX_FALLS;
							}

	
						}
					}
	
				}
			break;
			// jump and _no_ box pushing
			case MAN_FLAG_UP_1:
				if ((man.x > 0) &&
					(pole[intx-1][inty+1] == BOX_OFF))
				{	
					man.x -= 1;	
					man.y += 1;	
				}
			
			break;
			case MAN_FLAG_UP_2: 
				if ((man.x > 0) &&
					(pole[intx-2][inty+1] == BOX_OFF))
				{	
					man.x -= 2;	
					man.y += 1;	
				}
			break;
			}
			
		
			box_to_pole();
			man.flag_up = MAN_FLAG_UP_0;
		}
		man.ay = -MAN_DANGLEY;
		break;
	case VK_RIGHT: case '6':
		if ((intx < N_COL-1) && (gamestatus == GAME))
		{
			// horizontal move and box pushing
			switch(man.flag_up)
			{
			case MAN_FLAG_UP_0:
			
				if (pole[intx+1][inty] == BOX_OFF)
				{	
					man.x += 1;	
				}
				else
				{
					if (intx < N_COL - 2)
					{
						if (pole[intx+2][inty] == BOX_OFF)
						{
							// move box
							box[polebox[intx+1][inty]].x += 1;
							if (pole[intx+2][inty-1] != BOX_LIES)
							{
								box[polebox[intx+1][inty]].status = BOX_FALLS;
							}	
						}
					}
	
				}
			break;
			// jump and _no_ box pushing
			case MAN_FLAG_UP_1: 
			
				if ((man.x < N_COL-1) &&
					(pole[intx+1][inty+1] == BOX_OFF))
				{	
					man.x += 1;	
					man.y += 1;	
				}

			
			break;
			case MAN_FLAG_UP_2: 
				if ((man.x < N_COL - 2) &&
					(pole[intx+2][inty+1] == BOX_OFF))
				{	
					man.x += 2;	
					man.y += 1;	
				}

			
			break;
			}
			box_to_pole();
			man.flag_up = MAN_FLAG_UP_0;
		}
		man.ay = MAN_DANGLEY;
		break;

	case VK_UP: 
		if (gamestatus == GAME)
		{
			man.flag_up = MAN_FLAG_UP_1;
		}
		break;
	case VK_DOWN: 
		if (gamestatus == GAME)
		{
			man.flag_up = MAN_FLAG_UP_2;
		}
		break;
		
	case '7': case 36:
		if ((man.x > 1) &&
			(pole[intx-2][inty+1] == BOX_OFF) &&
			(gamestatus == GAME))
		{	
			man.x -= 2;	
			man.y += 1;	
		}
		man.ay = -MAN_DANGLEY;
		break;
	case '9': case 33:
		if ((man.x < N_COL-2) &&
			(pole[intx+2][inty+1] == BOX_OFF) &&
			(gamestatus == GAME))
		{	
			man.x += 2;	
			man.y += 1;	
		}
		man.ay = MAN_DANGLEY;
		break;
	case VK_SPACE:
		if (gamestatus == GAME)
		{
			gamestatus = GAMEPAUSE;
		}
		else
		{
			if (gamestatus == GAMEPAUSE)
			{
				gamestatus = GAME;
			}
		}
		break;

	}

	intx = (int)man.x;
	inty = (int)man.y;
	while ((man.y > 0) && (pole[intx][inty-1] != BOX_LIES))
	{
		man.y -= 1.0;
		inty--;
	}


}

void game()
{
	static int count = 0;
	static unsigned char first_time = 1;

	/***********/
	char buff[64];
	int i, j;
	/***********/

	if (first_time==1)
	{

		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);		
//		glEnable(GL_LIGHT1);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_COLOR_MATERIAL);
		LoadPic1();




		for (i=0; i<BOX_MAX; i++)
		{
			box[i].x = BOX_X_INIT;
			box[i].y = BOX_Y_INIT;
			box[i].status = BOX_OFF;
		}
		/*
		for (j=0; j<N_ROW; j++)
		{
			for (i=0; i<N_COL; i++)
			{
				pole[i][j] = 0;
			}
		}
		*/
		srand(time);
		new_box();
		
		man.x = fmod(rand(), N_COL);
		man.y = 0;
		man.flag_up = MAN_FLAG_UP_0;
		man.ay = 0;

		first_time = 0;
	}


	glPushMatrix(); // all
//	glScalef(0.1, 0.1, 1);

	// pole
	glColor3f(0.0, 0.0, 0.5);


	rectangletex(	START_X + 0*SX, START_Y + 0*SY, 
				START_X + (N_COL)*SX, START_Y + (N_ROW)*SY);
	glColor3f(0, 1.0, 0.5);



	// box to pole
	// box_to_pole();

	// draw
/*

	for (j=0; j<N_ROW; j++)
	{
		for (i=0; i<N_COL; i++)
		{
			if (pole[i][j] != BOX_OFF)
			{
		/ *		rectangle(	START_X + i*SX, START_Y + j*SY, 
							START_X + (i+0.9)*SX, START_Y + (j+0.9)*SY);* /
				drawbox()
				/ *textout(	START_X + i*SX, START_Y + j*SY, 1, &num[pole[i][j]] );* /
			}
		}
	}
*/

	if (gamestatus == GAMEMENU)
	{
		draw_menu_new();
	}
	if (gamestatus == GAME)
	{
		for (i=0; i<BOX_MAX; i++)
		{
			if (box[i].status != BOX_OFF)
			{
				drawbox(box[i]);
			}
		}
		draw_man(man);
	}
	
	// lines
	if (lines > 0)
	{
		sprintf(buff, "Линий: %d", lines);
		textout(RES_LINES_X, RES_LINES_Y, strlen(buff), buff);
	}
	if (pobashke > 0)
	{
		sprintf(buff, "По башке: %d", pobashke);
		textout(RES_POBASHKE_X, RES_POBASHKE_Y, strlen(buff), buff);
	}

	if (gamestatus == GAMEOVER)
	{
		glColor3f(1.0, 1.0, 1.0);
		textout(	START_X + N_COL*HSX, START_Y + N_ROW*HSY, 9, "GAME OVER" );
	}

	if (gamestatus == GAMEPAUSE)
	{
		glColor3f(1.0, 1.0, 1.0);
		textout(	START_X + N_COL*HSX, START_Y + N_ROW*HSY, 5, "PAUSE" );
	}


	glPopMatrix(); // all
}