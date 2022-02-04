/*
**  reversi.c ---- evaluates a reversi board
**
**  This file is use to create a Shared Library on HP-UX and dll on Windows.
*/

#if defined(WIN32)
#  include <windows.h>
#  define EXPORT __declspec(dllexport)
#else
#  define EXPORT
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int poisonValue = 65;
int cornerValue = 110;
int edgeValTab[5] = {65, 65, 60, 60, 55};
int loneEdgeBonus = 87;
int eVal = 55;

EXPORT int evaluate (int *board, int player) {
	int	i, j, sum, nonEdge, p, corner, t, num_open;

	/*
	** If there are only a few open squares, use simple evaluation
	*/
	sum = 0;
	num_open = 0;
	for (i = 0; i < 100; i++) {
		if (board[i] == 0) {
			if (++num_open > 12)
				break;
		} else 
			sum += (board[i] == player ? +10 : -10);
	}

	if (num_open <= 12) 
		return sum;

	/*
	** There are numerous open squares, use heuristic evaluation
	*/
	sum = 0;
	for (i = 0; i < 100; i++) {
		nonEdge = !((i>=1 && i<=8) || ((i % 10)==0 && i>0 && i<90) 
				  || ((i % 10)==9 && i>9 && i<99) || (i>=91 && i<=98));	
		if (nonEdge) {
			p = ((i==1 && board[0]==0) || 
				 (i==18 && board[9]==0) || 
				 (i==81 && board[90]==0) || 
				 (i == 88 && board[99]==0));
			corner = (i==0 || i==9 || i==90 || i==99);
			t = board[i];
			if (t != 0) {
				if (t==player) 
					sum += (p ? -poisonValue : (corner ? cornerValue : +10));
				else 
					sum -= (p ? -poisonValue : (corner ? cornerValue : +10));
			}
		}
	}

	for (i = 0; i < 5; i++) {
		j = i + 1;
		if (board[j] > 0) {
			if (board[0] == 0)
				sum += (board[j] == player ? eVal : -eVal);	
			else  {
				if (board[0] == player)
					sum += (board[j] == player ? edgeValTab[i] : 0);
				else
					sum += (board[j] != player ? -edgeValTab[i] : 0);
			}
		}
	}

 	for (i = 0; i < 5; i++) {
		j = 8-i;
		if (board[j] > 0) {
			if (board[9] == 0)
				sum += (board[j] == player ? eVal : -eVal);	
			else  {
				if (board[9] == player)
					sum += (board[j] == player ? edgeValTab[i] : 0);
				else
					sum += (board[j] != player ? -edgeValTab[i] : 0);
			}
		}
	}

	for (i = 0; i < 5; i++) {
		j = (i+1)*10;
		if (board[j] > 0) {
			if (board[0] == 0)
				sum += (board[j] == player ? eVal : -eVal);	
			else  {
				if (board[0] == player)
					sum += (board[j] == player ? edgeValTab[i] : 0);
				else
					sum += (board[j] != player ? -edgeValTab[i] : 0);
			}
		}
	}

	for (i = 0; i < 5; i++) {
		j = (i+1)*10+9;
		if (board[j] > 0) {
			if (board[9] == 0)
				sum += (board[j] == player ? eVal : -eVal);	
			else  {
				if (board[9] == player)
					sum += (board[j] == player ? edgeValTab[i] : 0);
				else
					sum += (board[j] != player ? -edgeValTab[i] : 0);
			}
		}
	}

	for (i = 0; i < 5; i++) {
		j = (8-i)*10;
		if (board[j] > 0) {
			if (board[90] == 0)
				sum += (board[j] == player ? eVal : -eVal);	
			else  {
				if (board[90] == player)
					sum += (board[j] == player ? edgeValTab[i] : 0);
				else
					sum += (board[j] != player ? -edgeValTab[i] : 0);
			}
		}
	}

	for (i = 0; i < 5; i++) {
		j = (8-i)*10+9;
		if (board[j] > 0) {
			if (board[99] == 0)
				sum += (board[j] == player ? eVal : -eVal);	
			else  {
				if (board[99] == player)
					sum += (board[j] == player ? edgeValTab[i] : 0);
				else
					sum += (board[j] != player ? -edgeValTab[i] : 0);
			}
		}
	}

	for (i = 0; i < 5; i++) {
		j = 91+i;
		if (board[j] > 0) {
			if (board[90] == 0)
				sum += (board[j] == player ? eVal : -eVal);	
			else  {
				if (board[90] == player)
					sum += (board[j] == player ? edgeValTab[i] : 0);
				else
					sum += (board[j] != player ? -edgeValTab[i] : 0);
			}
		}
	}

	for (i = 0; i < 5; i++) {
		j = 98-i;
		if (board[j] > 0) {
			if (board[99] == 0)
				sum += (board[j] == player ? eVal : -eVal);	
			else  {
				if (board[99] == player)
					sum += (board[j] == player ? edgeValTab[i] : 0);
				else
					sum += (board[j] != player ? -edgeValTab[i] : 0);
			}
		}
	}

	/*
	** EDGE BONUS 
	*/
	for (i = 0; i < 8; i++) {
		j = (i+1)*10+9;
		if (board[j] > 0) {
			if (board[j-10] == 0 && board[j+10]==0) {
				if (board[j] == player)
					sum += loneEdgeBonus;
				else 
					sum -= loneEdgeBonus;
			}
		}
	}

	for (i = 0; i < 8; i++) {
		j = i+1;
		if (board[j] > 0) {
			if (board[j-1] == 0 && board[j+1]==0) {
				if (board[j] == player)
					sum += loneEdgeBonus;
				else 
					sum -= loneEdgeBonus;
			}
		}
	}

	for (i = 0; i < 8; i++) {
		j = (i+1) * 10;
		if (board[j] > 0) {
			if (board[j-10] == 0 && board[j+10]==0) {
				if (board[j] == player)
					sum += loneEdgeBonus;
				else 
					sum -= loneEdgeBonus;
			}
		}
	}

	for (i = 0; i < 8; i++) {
		j = (i+1)*10+9;
		if (board[j] > 0) {
			if (board[j-10] == 0 && board[j+10]==0) {
				if (board[j] == player)
					sum += loneEdgeBonus;
				else 
					sum -= loneEdgeBonus;
			}
		}
	}
	
	return sum;
}


int	direction_vectors[100][9] = { 
	{5,7,8,0,0,0,0,0,0}, {5,7,8,0,0,0,0,0,0}, {4,5,6,7,8,0,0,0,0}, 
	{4,5,6,7,8,0,0,0,0}, {4,5,6,7,8,0,0,0,0}, {4,5,6,7,8,0,0,0,0}, 
	{4,5,6,7,8,0,0,0,0}, {4,5,6,7,8,0,0,0,0}, 
	{4,6,7,0,0,0,0,0,0}, {4,6,7,0,0,0,0,0,0},

	{5,7,8,0,0,0,0,0,0}, {5,7,8,0,0,0,0,0,0}, {4,5,6,7,8,0,0,0,0}, 
	{4,5,6,7,8,0,0,0,0}, {4,5,6,7,8,0,0,0,0}, {4,5,6,7,8,0,0,0,0}, 
	{4,5,6,7,8,0,0,0,0}, {4,5,6,7,8,0,0,0,0}, 
	{4,6,7,0,0,0,0,0,0}, {4,6,7,0,0,0,0,0,0},

	{2,3,5,7,8,0,0,0,0}, {2,3,5,7,8,0,0,0,0}, {1,2,3,4,5,6,7,8,0}, 
	{1,2,3,4,5,6,7,8,0}, {1,2,3,4,5,6,7,8,0}, {1,2,3,4,5,6,7,8,0}, 
	{1,2,3,4,5,6,7,8,0}, {1,2,3,4,5,6,7,8,0}, 
	{1,2,4,6,7,0,0,0,0}, {1,2,4,6,7,0,0,0,0},

	{2,3,5,7,8,0,0,0,0}, {2,3,5,7,8,0,0,0,0}, {1,2,3,4,5,6,7,8,0}, 
	{1,2,3,4,5,6,7,8,0}, {1,2,3,4,5,6,7,8,0}, {1,2,3,4,5,6,7,8,0}, 
	{1,2,3,4,5,6,7,8,0}, {1,2,3,4,5,6,7,8,0}, 
	{1,2,4,6,7,0,0,0,0}, {1,2,4,6,7,0,0,0,0},

	{2,3,5,7,8,0,0,0,0}, {2,3,5,7,8,0,0,0,0}, {1,2,3,4,5,6,7,8,0}, 
	{1,2,3,4,5,6,7,8,0}, {1,2,3,4,5,6,7,8,0}, {1,2,3,4,5,6,7,8,0}, 
	{1,2,3,4,5,6,7,8,0}, {1,2,3,4,5,6,7,8,0}, 
	{1,2,4,6,7,0,0,0,0}, {1,2,4,6,7,0,0,0,0},

	{2,3,5,7,8,0,0,0,0}, {2,3,5,7,8,0,0,0,0}, {1,2,3,4,5,6,7,8,0}, 
	{1,2,3,4,5,6,7,8,0}, {1,2,3,4,5,6,7,8,0}, {1,2,3,4,5,6,7,8,0}, 
	{1,2,3,4,5,6,7,8,0}, {1,2,3,4,5,6,7,8,0}, 
	{1,2,4,6,7,0,0,0,0}, {1,2,4,6,7,0,0,0,0},

	{2,3,5,7,8,0,0,0,0}, {2,3,5,7,8,0,0,0,0}, {1,2,3,4,5,6,7,8,0}, 
	{1,2,3,4,5,6,7,8,0}, {1,2,3,4,5,6,7,8,0}, {1,2,3,4,5,6,7,8,0}, 
	{1,2,3,4,5,6,7,8,0}, {1,2,3,4,5,6,7,8,0}, 
	{1,2,4,6,7,0,0,0,0}, {1,2,4,6,7,0,0,0,0},

	{2,3,5,7,8,0,0,0,0}, {2,3,5,7,8,0,0,0,0}, {1,2,3,4,5,6,7,8,0}, 
	{1,2,3,4,5,6,7,8,0}, {1,2,3,4,5,6,7,8,0}, {1,2,3,4,5,6,7,8,0}, 
	{1,2,3,4,5,6,7,8,0}, {1,2,3,4,5,6,7,8,0}, 
	{1,2,4,6,7,0,0,0,0}, {1,2,4,6,7,0,0,0,0},

	{2,3,5,0,0,0,0,0,0}, {2,3,5,0,0,0,0,0,0}, {1,2,3,4,5,0,0,0,0}, 
	{1,2,3,4,5,0,0,0,0}, {1,2,3,4,5,0,0,0,0}, {1,2,3,4,5,0,0,0,0}, 
	{1,2,3,4,5,0,0,0,0}, {1,2,3,4,5,0,0,0,0},
	{1,2,4,0,0,0,0,0,0}, {1,2,4,0,0,0,0,0,0},

	{2,3,5,0,0,0,0,0,0}, {2,3,5,0,0,0,0,0,0}, {1,2,3,4,5,0,0,0,0}, 
	{1,2,3,4,5,0,0,0,0}, {1,2,3,4,5,0,0,0,0}, {1,2,3,4,5,0,0,0,0}, 
	{1,2,3,4,5,0,0,0,0}, {1,2,3,4,5,0,0,0,0},
	{1,2,4,0,0,0,0,0,0}, {1,2,4,0,0,0,0,0,0}
};


int	dr[8] = {-1, -1, -1, 0, 0, +1, +1, +1};
int	dc[8] = {-1, 0, +1, -1, +1, -1, 0, +1};
int *the_board;

int valid_dir(int row, int col, int dr, int dc, int player) {
	int	looking_for;

	looking_for = (player == 1 ? 2 : 1);
	if (the_board[(row+dr)*10+(col+dc)] != looking_for)
		return 0;
	row += (2*dr);
	col += (2*dc);
	while (row >= 0 && row <= 9 && col >= 0 && col <= 9) {
		if (the_board[row*10+col] == 0)		
			return 0;
		if (the_board[row*10+col] != looking_for)		
			return 1;
		row += dr;
		col += dc;
	}
	return 0;
}


/*
**	Returns a list of legal moves
*/
EXPORT int legal_moves (int *board, int player, int *moves) {
	int	num_legal_moves, *dvs, vd, i;

	the_board = board;
	num_legal_moves = 0;
	for (i = 0; i < 100; i++) {

		if (board[i] == 0) {
			dvs = &direction_vectors[i][0];
			while (*dvs) {
				vd = valid_dir(i/10, i%10, dr[*dvs-1], dc[*dvs-1], player);
				if (vd) {
					*moves++ = i;
					num_legal_moves++;
					break;
				}
				dvs++;
			}
		}
	}
	return num_legal_moves;
}


int flip_disks(int sr, int sc, int dr, int dc, int player, int *temp_board) {
	int	oppo, i, index;

	oppo = (player == 1 ? 2 : 1);

	for (i = 0; i < 10; i++) {
		index = 10 * (sr+dr*(i+1)) + (sc + dc*(i+1));
		if (temp_board[index] != oppo)
			return 0;
		temp_board[index] = player;
	}
	return 0;
}


EXPORT int make_move_guts(int square, int *temp_board, int player) {
	int	*dvs, vd, r, c, deltar, deltac;
	
	the_board = temp_board;
	dvs = &direction_vectors[square][0];
	while (*dvs) {
		r = square / 10;
		c = square % 10;
		deltar = dr[*dvs-1];
		deltac = dc[*dvs-1];
		vd = valid_dir(r, c, deltar, deltac, player);
		if (vd) 
			flip_disks(r, c, deltar, deltac, player, temp_board);
		dvs++;
	}
	temp_board[square] = player;
	return 0;
}


