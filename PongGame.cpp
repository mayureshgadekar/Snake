#include <iostream>
#include <time.h>
#include <conio.h>
#include <Windows.h>
using namespace std; 

enum eDir {STOP = 0, LEFT=1, UPLEFT=2, DOWNLEFT=3, RIGHT= 4, UPRIGHT=5, DOWNRIGHT= 6};

class cBall
{
private:
	int x, y; 
	int originalX, originalY;
	eDir dir;
public:	
	cBall(int posX, int posY)		// initial stage 
	{
		originalX = posX;
		originalY = posY; 
		x = posX; 
		y = posY; 
		dir = STOP;
	}

	void Reset()		// go back to initial stage 
	{
		x = originalX;
		y = originalY; 
		dir = STOP;
	}

	void changeDirection(eDir d)		
	{
		dir = d;
	}
	inline int getX()
	{
		return x; 
	}
	inline int getY()
	{
		return y;
	}
	void randomDirection()			//change direction after initiation
	{
		dir = (eDir)((rand() % 6) + 1);
	}
	inline eDir getDirection()
	{
		return dir;
	}

	void Move()
	{
		switch (dir)
		{
		case STOP: 
			break;
		case LEFT:
			x--;
			break;
		case RIGHT:
			x++;
		case UPLEFT:
			x--; y--;
			break;
		case DOWNLEFT:
			x--; y++;
			break;
		case UPRIGHT:
			x++;y--;
			break;
		case DOWNRIGHT:
			x++; y++;
		default:
			break;
		}
	}
/*	friend ostream & operator(ostream & o, cBall c)
	{
		o << "Ball ["
	}*/
};

class cPaddle		// controller
{
private :
	int x, y; 
	int originalX, originalY; 
public:
	cPaddle()
	{
		x = y = 0;
	}
	cPaddle(int posX, int posY): cPaddle() // call th current constructor 
	{
		originalX = posX;
		originalY = posY;
		x = posX; 
		y = posY; 
	}
	inline void Reset()
	{
		x = originalX;
		y = originalY; 
	}

	inline int getX()
	{
		return x;
	}
	inline int getY()
	{
		return y;
	}

	inline void moveUp()
	{
		y--;
	}

	inline void moveDown()
	{
		y++;
	}
};

class cGameManager		// controls both cBall and cPaddle
{
private:
	int width, height;
	int score1, score2;
	char up1, down1, up2, down2;
	bool quit;
	cBall * ball;
	cPaddle * player1; 
	cPaddle * player2;

public:
	cGameManager(int w, int h) 
	{
		srand(time(NULL));
		quit = false;
		up1 = 'w'; up2 = 'i';
		down1 = 's'; down2 = 'k';
		score1 = score2 = 0;
		width = w; height = h;  
		
		//ball position
		ball = new cBall(w / 2, h / 2);
		
		//paddle postions
		player1 = new cPaddle(1, h / 2 - 3);
		player2 = new cPaddle(w - 2, h / 2 - 3);		// shift one block form right 
	}

	~cGameManager()
	{
		delete ball, player1, player2;
	}

	void ScoreUp(cPaddle *player)
	{
		if (player == player1)
			score1++;
		else if (player == player2)
			score2++;
		ball->Reset();
		player1->Reset();
		player2->Reset(); 
	}

	void Draw()
	{
		system("cls");
		for (int i = 0;i < width + 2; i++)
			cout << "\xB2";
		cout << endl;

		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				//current values
				int ballx = ball->getX();
				int bally = ball->getY(); 
				int player1x = player1->getX();
				int player1y = player1->getY();
				int player2x = player2->getX();
				int player2y = player2->getY();

				if (j == 0)
					cout << "\xB2";				// new characters from charmap run command 

				if (ballx == j && bally == i)
					cout << "o";
				else if (player1x == j && player1y == i)
					cout << "\xDB";		//player1
				else if (player2x == j && player2y == i)
					cout << "\xDB";		//player2
				else if (player1x == j && player1y + 1== i)
					cout << "\xDB";		//player1
				else if (player1x == j && player1y + 2== i)
					cout << "\xDB";		//player1
				else if (player1x == j && player1y + 3== i)
					cout << "\xDB";		//player1

				else if (player2x == j && player2y + 1 == i)
					cout << "\xDB";		//player1
				else if (player2x == j && player2y + 2 == i)
					cout << "\xDB";		//player1
				else if (player2x == j && player2y + 3 == i)
					cout << "\xDB";		//player1
				else
					cout << " ";
				if (j == width - 1)
					cout << "\xB2";
			}
			cout << endl;
		}

		for (int i = 0;i < width + 2; i++)
			cout << "\xB2";
		cout << endl; 
		cout << "Score : " << score1 << " Score2: " << score2;
		cout << endl << "Press 'q' to end game"; 

	}

	void Input()
	{
		ball->Move(); 

		int ballx = ball->getX();
		int bally = ball->getY();
		int player1x = player1->getX();
		int player1y = player1->getY();
		int player2x = player2->getX();
		int player2y = player2->getY();

		if (_kbhit())
		{
			char current = _getch();
			if (current == up1)
				if (player1y > 0)
					player1->moveUp();
				
			if (current == up2)
				if (player2y > 0)
					player2->moveUp();

			if (current == down1)
				if (player1y + 4 < height)				// cause we have four segments
					player1->moveDown();

			if (current == down2)
				if (player2y + 4 < height)
					player2->moveDown();

			if (ball->getDirection() == STOP)
				ball->randomDirection();
			
			if (current == 'q')
				quit = true;	
		}
	}

	void Logic()
	{
		int ballx = ball->getX();				// genric state of the ball and player
		int bally = ball->getY();
		int player1x = player1->getX();
		int player1y = player1->getY();
		int player2x = player2->getX();
		int player2y = player2->getY();
		
		//ball hitting left paddle
		for (int i = 0; i < 4; i++)
			if(ballx == player1x+1)
				if(bally == player1y+i) 
					ball->changeDirection((eDir)((rand() % 3) + 4)); // enum randomizing between 3 + 4 


		//ball hitting right paddle
		for (int i = 0; i < 4; i++)
			if (bally == player2x - 1)
				if (bally == player2y + i)
					ball->changeDirection((eDir)((rand() % 3) + 1)); // enum randomizing between 3 + 1
		//bottom wall
		if (bally == height - 1)
			ball->changeDirection(ball->getDirection() == DOWNRIGHT ? UPRIGHT : UPLEFT);

		//top wall
		if (bally == 0)
			ball->changeDirection(ball->getDirection() == UPRIGHT ? DOWNRIGHT : DOWNLEFT);

		//ball hitting wall of paddle then score update
		if (ballx == width - 1)
			ScoreUp(player1);
		if (ballx == 0)
			ScoreUp(player2);

	}

	void Run()
	{
		while (!quit)
		{
			Draw();
			Input();
			Logic(); 
		//	Sleep(20);
		}
	}
};

	
int main()
{
	cGameManager c(40, 20);

	c.Run();
	
	return 0;
}
