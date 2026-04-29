/*			 Name: Areeba Riaz
   			 Roll no: i22-1244 

/* PROGRAMMING FUNDAMENTAL'S PROJECT FOR FALL 2022 BS(CS)
 * You need to define the required function in the part instructed here below.
 * Avoid making any unnecessary changes, particularly the ones you don't understand.
 * The different pieces should be defined using an array - uncomment the following code once you are done creating the array.
 * TIP: Understand thoroughly before getting started with coding.
 * */

/////////////////////////////////////////////
///*** START CODING YOUR FUNTIONS HERE ***///

void movement(int delta_x)
{
	for (int i=0 ; i<4 ; i++)		//p2:grids p1:block
	{
		point_2[i][0]=point_1[i][0];
		point_2[i][1]=point_1[i][1];
		point_1[i][0] += delta_x;
	}
	if (!anamoly())	//left/right boundary		
	   for (int i=0 ; i<4 ; i++)
	   {
		point_1[i][0]=point_2[i][0];
		point_1[i][1]=point_2[i][1];
	   }
	   
}
//--------------------------------------------------------------------------------------
void rotation(bool rotate)
{
	if (rotate)
	{
		for (int i=0 ; i<4 ; i++)
		{
			int a = point_1[i][1] - point_1[1][1];
			int b = point_1[i][0] - point_1[1][0];
			point_1[i][0] = point_1[1][0] - a;
			point_1[i][1] = point_1[1][1] + b;
		}
	   if (!anamoly())
		for (int i=0 ; i<4 ; i++)
		{
			point_1[i][0]=point_2[i][0];
			point_1[i][1]=point_2[i][1];
		}
	}
}
//--------------------------------------------------------------------------------------
void fallingPiece(float& timer, float& delay , int& colorNum)
{
    if (timer>delay)
    {
        for (int i=0;i<4;i++)
        {
            point_2[i][0]=point_1[i][0];
            point_2[i][1]=point_1[i][1];
            point_1[i][1]+=1;  //piece 1 unit down
        }

        if (!anamoly())				//This will check and boundaries and then drop the blocks
	{
		int n = rand() % 7;
		for(int i=0 ; i<4 ; i++) 
		    gameGrid[point_2[i][1]][point_2[i][0]] = colorNum;
		colorNum = 1 + rand() % 7; 
		for (int i=0;i<4;i++)
		{
		    point_1[i][0] = BLOCKS[n][i] % 2;
		    point_1[i][1] = BLOCKS[n][i] / 2;
		}
        }
        timer=0;    
    }
}
//--------------------------------------------------------------------------------------
int line(int& a, int& score, bool& gameover)
{
int total=0;
    for(int i=a ; i>=1 ; --i)
    {
		int count=0;
		for (int j=0 ; j<N ; ++j)
		{
			if (gameGrid[i][j])
			{
			    if(i==1)
			      gameover=i;
			++count;
			}
		gameGrid[a][j] = gameGrid[i][j];
		}
		if (count<N)
		   --a;
		else if (count==10)
		score+=10;
   }
   return score;
}

///*** YOUR FUNCTIONS END HERE ***///
/////////////////////////////////////
