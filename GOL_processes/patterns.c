/*SO 2012 TP2
Conway's Game of Life implementation 
Filipe Batista, 2006124931
M. Inês Coelho, 2004104311
Time Spent: 55h
*/

#include "header.h"

//define pattern types name and area
char * pattern_types[PATTERN_NUMBER] = {"Single", "Block", "Glider", "Space Ship", "Pulsar"};
int pattern_area[PATTERN_NUMBER][2] = {{1,1},{2,2},{3,3},{4,5},{13,13}};


/*ask the user the quantities of each pattern type and create world*/
int create_world () 
{
	int type, free_space, qtt_allowed, qtt;
	int * pattern_qtt;

	//Create array to store pattern quantities	
	pattern_qtt = malloc (sizeof(int)*PATTERN_NUMBER);
	if (pattern_qtt == NULL) {
		fprintf(stderr, "Insuficient memory to create pattern_qtt array!");
		return 0;
	}

	free_space = dimension*dimension; //calculate grid free space

	//user input
	fprintf(stdout, "Number of each pattern instance in the game:\n");

	for (type = PATTERN_NUMBER-1; type >= 0; type--) {

		//to assure randomness only one half of the free space can be used to insert a new pattern
		qtt_allowed = (free_space/2)/(pattern_area[type][0]*pattern_area[type][1]); 

		while(true) {
			fprintf(stdout, "   - %s (max %d): ", pattern_types[type], qtt_allowed);
			scanf("%d", &qtt);
			if (qtt > qtt_allowed || qtt <0) 
				fprintf(stdout, "The maximum quantity allowed is %d!\n", qtt_allowed);
			else {
				if (qtt!=0) //recalculate free space; to assure randomness only one half of this space can be used
					free_space = (free_space - pattern_area[type][0]*pattern_area[type][1]*qtt)/2; 
				pattern_qtt[type] = qtt;
				break;
			}
		}
	
	}

	create_patterns(pattern_qtt); //generate patterns randomly through the grid

	free(pattern_qtt); //release resources

	return 0;
}


int create_patterns(int * pattern_qtt) 
{
	int number, type, x_value = 0, y_value = 0, x, y;

	// initialize random seed
	srand (time(NULL) );

	for (type = PATTERN_NUMBER-1; type >= 0; type--) {
		number = 0;

		while (number < pattern_qtt[type] )
		{
			//calculate a random x and y
			x_value = x_value + rand() % dimension;
			if (x_value > dimension-pattern_area[type][0]) //if x_value is unable to contain the all pattern in the grid
				x_value/=10; //x_value is divided by 10
			y_value = y_value + rand() % dimension; 
			if (y_value > dimension-pattern_area[type][1]) //if y_value is unable to contain the all pattern in the grid
				y_value/=10; //y_value is divided by 10

			if ( (CELL(x_value,y_value) == -1) && (CELL(x_value,y_value+pattern_area[type][1]-1) == -1) && 
					(CELL(x_value+pattern_area[type][0]-1,y_value) == -1) && 
					(CELL(x_value+pattern_area[type][0]-1,y_value+pattern_area[type][1]-1) == -1)) //detect superimpositions
			{
					switch (type) 
					{
						case 0: //single			
							CELL(x_value,y_value) = 1;
							break;
						case 1: //block
							draw_block(x_value, y_value);
							break;
						case 2: //glider
							draw_glider(x_value, y_value, type);
							break;
						case 3: //space ship
							draw_spaceship(x_value, y_value, type);
							break;
						case 4: //pulsar
							draw_pulsar(x_value, y_value, type);
							break;
					}

					#ifdef DEBUG
						fprintf(stdout, "Setting %s number %d at coordinates: %d %d\n", pattern_types[type], number+1, x_value, y_value);
					#endif

					number++;
			}
		}
	}		

	//fill the area left on the grid with 0's
	for (x = 0; x < dimension; x++)
		for (y = 0; y < dimension; y++)
			if (CELL(x,y)  == -1) CELL(x,y)  = 0;
	return 0;
}

/*draw Block pattern*/
void draw_block (int x_pos, int y_pos) {

	CELL(x_pos,y_pos) = 1;
	CELL(x_pos,y_pos+1) = 1;
	CELL(x_pos+1,y_pos) = 1;
	CELL(x_pos+1,y_pos+1) = 1;
}

/*draw Glider pattern*/
void draw_glider (int x_pos, int y_pos, int type) {

	CELL(x_pos,y_pos+1) = 1;
	CELL(x_pos+1,y_pos+2) = 1;
	CELL(x_pos+2,y_pos) = 1;
	CELL(x_pos+2,y_pos+1) = 1;
	CELL(x_pos+2,y_pos+2) = 1;

	fill_blanks (x_pos, y_pos, type);

	return;
}

/*draw Lightweight Space Ship Pattern*/
void draw_spaceship (int x_pos, int y_pos, int type) {

	CELL(x_pos,y_pos) = 1;
	CELL(x_pos,y_pos+3) = 1;
	CELL(x_pos+1,y_pos+4) = 1;
	CELL(x_pos+2,y_pos+0) = 1;
	CELL(x_pos+2,y_pos+4) = 1;
	CELL(x_pos+3,y_pos+1) = 1;
	CELL(x_pos+3,y_pos+2) = 1;
	CELL(x_pos+3,y_pos+3) = 1;
	CELL(x_pos+3,y_pos+4) = 1;

	fill_blanks (x_pos, y_pos, type);

	return;
}

/*draw Pulsar pattern*/
void draw_pulsar (int x_pos, int y_pos, int type) {
	int i;

	for (i=0; i<13; i++) {
		switch (i) {
			case 0: case 5: case 7: case 12:
				CELL(x_pos+i,y_pos+2) = 1;
				CELL(x_pos+i,y_pos+3) = 1;
				CELL(x_pos+i,y_pos+4) = 1;
				CELL(x_pos+i,y_pos+8) = 1;
				CELL(x_pos+i,y_pos+9) = 1;
				CELL(x_pos+i,y_pos+10) = 1;
				break;
			case 2: case 3: case 4: case 8: case 9: case 10:
				CELL(x_pos+i,y_pos+0) = 1;
				CELL(x_pos+i,y_pos+5) = 1;
				CELL(x_pos+i,y_pos+7) = 1;
				CELL(x_pos+i,y_pos+12) = 1;
				break;
		}
	}

	fill_blanks (x_pos, y_pos, type);

	return;
}

/*fill the rest of pattern area with 0's*/
void fill_blanks(int x_pos, int y_pos, int type) 
{
	int x, y;

	//fill the area left with 0's
	for (x = x_pos; x < x_pos + pattern_area[type][0]; x++)
		for (y = y_pos; y < y_pos + pattern_area[type][1]; y++)
			if (CELL(x,y)  != 1) CELL(x,y)  = 0;

}
