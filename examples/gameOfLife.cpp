

#include <iostream>
#include <string>
#include <vector>
#include "EasySDL2.hpp"

using namespace std;


void copyGrid(bool grid1[100][100], bool grid2[100][100]) {

	for (int i = 0; i < 100; i++)
	{
		for (int j = 0; j < 100; j++)
		{
			grid1[i][j] = grid2[i][j];
		}
	}

}



int main(int argc, char* argv[]) {


	EasySDL2 sdl(500, 500);
	
	bool grid[100][100] = { false };


	for (int i = 1; i < 99; i++)
	{
		for (int j = 1; j < 99; j++)
		{
			float r = ((double)rand() / (RAND_MAX));
			if (r > 0.5f) grid[i][j] = true;
		}
	}





	while (!sdl.exit) {

		bool newgrid[100][100] = { false };

		for (int i = 1; i < 99; i++)
		{
			for (int j = 1; j < 99; j++)
			{
				int count = 0;
				for (int k = -1; k < 2; k++)
				{
					for (int l = -1; l < 2; l++)
					{
						if (!(k == 0 && l == 0) && grid[i + k][j + l]) count++;
					}
				}

				if (!grid[i][j] && count == 3) newgrid[i][j] = true;
				else if (!grid[i][j] && count != 3) newgrid[i][j] = false;
				else if (grid[i][j] && (count == 2 || count == 3)) newgrid[i][j] = true;
				else if (grid[i][j] && !(count == 2 || count == 3)) newgrid[i][j] = false;

			}
		}

		copyGrid(grid, newgrid);

		for (int i = 0; i < 100; i++)
		{
			for (int j = 0; j < 100; j++)
			{
				if(!grid[i][j]) sdl.drawRectFilled(i*5, j*5, 5, 5, sdl.BLACK);
				if(grid[i][j]) sdl.drawRectFilled(i*5, j*5, 5, 5, sdl.WHITE);
			}
		}


		sdl.drawFrame();
		sdl.showUntil("Q");
	}

	return 0;

}