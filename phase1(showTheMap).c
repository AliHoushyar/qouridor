#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>

struct board{ //this is the structue that we need to save the
    int n, m; // size of thg map
    int Map[100][100]; // an array so show the status of each cell
    int p1x, p1y, p2x, p2y; // the location of players
    int p1w, p2w; // the number of player's walls
};

struct Graph{
    int visited[10000];
    int adjList[10000][4];
};

void setTextColor(int textColor, int backColor) { // a function to change the color
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	int colorAttribute = backColor << 4 | textColor;
	SetConsoleTextAttribute(consoleHandle, colorAttribute);
}


void addedge(int n,int m,int i,int j,struct Graph *mygraph){
    if(i>=n || j>=m){
        mygraph->adjList[i*(m+1)+j][0]= -1;
        mygraph->adjList[i*(m+1)+j][1]= -1;
        mygraph->adjList[i*(m+1)+j][2]= -1;
        mygraph->adjList[i*(m+1)+j][3]= -1;
    }
    else if(i==0){
        if(j==0){
            mygraph->adjList[i*(m+1)+j][0]= i*(m+1)+j+m+1;
            mygraph->adjList[i*(m+1)+j][1]= i*(m+1)+j+1;
            mygraph->adjList[i*(m+1)+j][2]= -1;
            mygraph->adjList[i*(m+1)+j][3]= -1;
        }
        else if(j==m-1){
            mygraph->adjList[i*(m+1)+j][0]= i*(m+1)+j+m+1;
            mygraph->adjList[i*(m+1)+j][1]= i*(m+1)+j-1;
            mygraph->adjList[i*(m+1)+j][2]= -1;
            mygraph->adjList[i*(m+1)+j][3]= -1;
        }
        else{
            mygraph->adjList[i*(m+1)+j][0]= i*(m+1)+j+m+1;
            mygraph->adjList[i*(m+1)+j][1]= i*(m+1)+j-1;
            mygraph->adjList[i*(m+1)+j][2]= i*(m+1)+j+1;
            mygraph->adjList[i*(m+1)+j][3]= -1;
        }
    }
    else if(i==n-1){
        if(j==0){
            mygraph->adjList[i*(m+1)+j][0]= i*(m+1)+j-m-1;
            mygraph->adjList[i*(m+1)+j][1]= i*(m+1)+j+1;
            mygraph->adjList[i*(m+1)+j][2]= -1;
            mygraph->adjList[i*(m+1)+j][3]= -1;
        }
        else if(j==m-1){
            mygraph->adjList[i*(m+1)+j][0]= i*(m+1)+j-m-1;
            mygraph->adjList[i*(m+1)+j][1]= i*(m+1)+j-1;
            mygraph->adjList[i*(m+1)+j][2]= -1;
            mygraph->adjList[i*(m+1)+j][3]= -1;
        }
        else{
            mygraph->adjList[i*(m+1)+j][0]= i*(m+1)+j-m-1;
            mygraph->adjList[i*(m+1)+j][1]= i*(m+1)+j-1;
            mygraph->adjList[i*(m+1)+j][2]= i*(m+1)+j+1;
            mygraph->adjList[i*(m+1)+j][3]= -1;
        }
    }
    else{
        if(j==0){
            mygraph->adjList[i*(m+1)+j][0]= i*(m+1)+j+m+1;
            mygraph->adjList[i*(m+1)+j][1]= i*(m+1)+j-m-1;
            mygraph->adjList[i*(m+1)+j][2]= i*(m+1)+j+1;
            mygraph->adjList[i*(m+1)+j][3]= -1;
        }
        else if(j==m-1){
            mygraph->adjList[i*(m+1)+j][0]= i*(m+1)+j+m+1;
            mygraph->adjList[i*(m+1)+j][1]= i*(m+1)+j-m-1;
            mygraph->adjList[i*(m+1)+j][2]= i*(m+1)+j-1;
            mygraph->adjList[i*(m+1)+j][3]= -1;
        }
        else{
            mygraph->adjList[i*(m+1)+j][0]= i*(m+1)+j+m+1;
            mygraph->adjList[i*(m+1)+j][1]= i*(m+1)+j-m-1;
            mygraph->adjList[i*(m+1)+j][2]= i*(m+1)+j-1;
            mygraph->adjList[i*(m+1)+j][3]= i*(m+1)+j+1;
        }
    }
}

void delEdge(int s,int e,struct Graph *gg){
	int i;
    for(i=0;i<4;i++){
        if(gg->adjList[s][i]==e)
            gg->adjList[s][i]=-1;
    }
    for(i=0;i<4;i++){
        if(gg->adjList[e][i]==s)
            gg->adjList[e][i]=-1;
    }
}

void undoDeleteEdge(int s,int e,struct Graph *mg){
	int i;
    for(i=0;i<4;i++){
        if(mg->adjList[s][i]==-1){
            mg->adjList[s][i]=e;
            break;
        }
    }
    for(i=0;i<4;i++){
        if(mg->adjList[e][i]==-1){
            mg->adjList[e][i]=s;
            break;
        }
    }
}

int flag1=0,flag2=0;

int dfs(int str, int who,int n,int m,struct Graph *g){
    g->visited[str] = 1;
    //printf("m,n: %d , %d in %d \n",m,n,str);
    if(who == 2 && str<=m-1)
        flag2 = 1;
    if(who == 1 && str>=(n-1)*(m+1)){
        flag1 = 1;
    }
    //printf("----------%d--------%d\n",str,flag1);
    int i;
    for(i=0;i<4;i++){
        if(g->adjList[str][i]!=-1){
            if(g->visited[g->adjList[str][i]] == 0)
                dfs(g->adjList[str][i],who,n,m,g);
        }
    }

}

void makeUnVisit(struct Graph *g){
	int i;
    for(i=0;i<10000;i++){
        g->visited[i]=0;
    }
    return;
}

void clearScreen() { // a function to clear the screen
	system("cls");
}


void PrintTheMap(struct board GameMap) { // a function to print the map
// each cell has a 3-digit number, the first number shows if there is a wall at the left of that cell or not, the second
// one shows if there is a wall at the top of that cell or not and the last one shows wheter there is a player or not(0 stands as nothing, 1 stands as player 1 and so does 2)
	setTextColor(15, 0);
    int i, j, sw, n = GameMap.n, m = GameMap.m;
    int left, up, play;
    clearScreen();
    printf("This is the Map(O stands for player 1 and X stands for player 2): \n");
    setTextColor(1, 0);
    for (i = 0; i < n + 1; i++) {
        for (j = 0; j < m + 1; j++) {
            if (j == m) continue;
            left = GameMap.Map[i][j] / 100;
            up = (GameMap.Map[i][j] / 10) % 10;
            play = GameMap.Map[i][j] % 10;
            if (up == 1) printf("...."); // a normal way
            else {
    			setTextColor(4, 0);
				printf("===="); // a wall
				setTextColor(1, 0);
			}
        }
        printf(".");
        printf("\n");
        for (j = 0; j < m + 1; j++) {
        	if (i == n) break;
            left = GameMap.Map[i][j] / 100;
            up = (GameMap.Map[i][j] / 10) % 10;
            play = GameMap.Map[i][j] % 10;
            if (left == 1) printf(":"); // a normal way
            else {
            	setTextColor(4, 0);
				printf("|"); // a wall
				setTextColor(1, 0);
			}
            if (!play) printf("   ");
            else if (play == 1) {
            	setTextColor(2, 0);
				printf(" O ");
				setTextColor(1, 0);
			}
            else {
            	setTextColor(14, 0);
				printf(" X ");
				setTextColor(1, 0);
			}
        }
        printf("\n");
    }
}


bool validWallH(char c, int x, int y, struct board gameMap , struct Graph *gr) {
    if ((c == 'H' || c == 'h') && (gameMap.Map[x][y]/10)%10 != 2 && (gameMap.Map[x][y + 1]/10)%10 != 2) {
        if ((gameMap.Map[x][y + 1]/100 == 2) && (gameMap.Map[x - 1][y + 1]/100 == 2)) {
            if (!(gameMap.Map[x - 2][y + 1]/100 == 2 && gameMap.Map[x + 1][y + 1]/100 == 2)) return 0;
        }
        delEdge((x*(gameMap.m+1)+y),(x*(gameMap.m+1)+y-gameMap.m-1),gr);
        delEdge((x*(gameMap.m+1)+y+1),(x*(gameMap.m+1)+y-gameMap.m),gr);
        makeUnVisit(gr);
        flag1 = 0;
        flag2 =0;
        dfs( (gameMap.p1x*(gameMap.m+1)+gameMap.p1y),1,gameMap.n,gameMap.m,gr );
        if(flag1==1){
            makeUnVisit(gr);
            dfs( (gameMap.p2x*(gameMap.m+1)+gameMap.p2y),2,gameMap.n,gameMap.m,gr );
            if(flag2==1){
                return 1;
            }
            else{
                undoDeleteEdge((x*(gameMap.m+1)+y),(x*(gameMap.m+1)+y-gameMap.m-1),gr);
                undoDeleteEdge((x*(gameMap.m+1)+y+1),(x*(gameMap.m+1)+y-gameMap.m),gr);
                return 0;
            }
        }
        else{
            undoDeleteEdge((x*(gameMap.m+1)+y),(x*(gameMap.m+1)+y-gameMap.m-1),gr);
            undoDeleteEdge((x*(gameMap.m+1)+y+1),(x*(gameMap.m+1)+y-gameMap.m),gr);
            return 0;
        }
    }
    return 0;
}

bool validWallV(char c, int x, int y, struct board gameMap , struct Graph *gr) {
    if ((c == 'V' || c == 'v') && gameMap.Map[x][y]/100 != 2 && gameMap.Map[x + 1][y]/100 != 2) {
        if ((gameMap.Map[x + 1][y]/10) % 10 == 2 && (gameMap.Map[x + 1][y - 1]/10) % 10 == 2) {
            if (!((gameMap.Map[x + 1][y + 1]/10) % 10 == 2 && (gameMap.Map[x + 1][y - 2]/10) % 10 == 2)) return 0;
        }
        delEdge((x*(gameMap.m+1)+y),(x*(gameMap.m+1)+y-1),gr);
        delEdge((x*(gameMap.m+1)+y+gameMap.m+1),(x*(gameMap.m+1)+y+gameMap.m),gr);
        makeUnVisit(gr);
        flag1=0;
        flag2=0;
        dfs( (gameMap.p2x*(gameMap.m+1)+gameMap.p2y),2,gameMap.n,gameMap.m,gr );
        if(flag2==1){
            makeUnVisit(gr);
            dfs( (gameMap.p1x*(gameMap.m+1)+gameMap.p1y),1,gameMap.n,gameMap.m,gr );
            if(flag1==1){
                return 1;
            }
            else{
                undoDeleteEdge((x*(gameMap.m+1)+y),(x*(gameMap.m+1)+y-1),gr);
                undoDeleteEdge((x*(gameMap.m+1)+y+gameMap.m+1),(x*(gameMap.m+1)+y+gameMap.m),gr);
                return 0;
            }
        }
        else{
            undoDeleteEdge((x*(gameMap.m+1)+y),(x*(gameMap.m+1)+y-1),gr);
            undoDeleteEdge((x*(gameMap.m+1)+y+gameMap.m+1),(x*(gameMap.m+1)+y+gameMap.m),gr);
            return 0;
        }
    }
    return 0;
}

int main() {
    int n, m, i, j;
    printf("Please enter the size of the map: \n");
    scanf("%d", &n); // it gets the size of the map
    struct board gameMap;
    struct Graph mygraph;
    int x1, y1, x2, y2;
    m = n, gameMap.n = n, gameMap.m = m;
    do { 
    	printf("please enter the location of the first player: \n");
    	scanf("%d %d", &gameMap.p1x, &gameMap.p1y);
    	if(!(0 <= gameMap.p1x && gameMap.p1x < gameMap.n && 0 <= gameMap.p1y && gameMap.p1y < gameMap.m)) printf("invalid input! try again: \n"); //it check if the location is valid or not
		// a location is not valid if it is outside the map, it is also considered that the location of the first cell is (0, 0) and the last cell is (n - 1, n - 1)
    	
	} while(!(0 <= gameMap.p1x && gameMap.p1x < gameMap.n && 0 <= gameMap.p1y && gameMap.p1y < gameMap.m));
	do {
    	printf("please enter the location of the second player: \n");
    	scanf("%d %d", &gameMap.p2x, &gameMap.p2y);
    	if (!(0 <= gameMap.p2x && gameMap.p2x < gameMap.n && 0 <= gameMap.p2y && gameMap.p2y < gameMap.m && gameMap.p2x != gameMap.p1x && gameMap.p2y != gameMap.p1y)) printf("invalid input! try again: \n");
		// same as the previous one, but it also checks if it is placing in the place of the 1st player or not
	} while(!(0 <= gameMap.p2x && gameMap.p2x < gameMap.n && 0 <= gameMap.p2y && gameMap.p2y < gameMap.m && gameMap.p2x != gameMap.p1x && gameMap.p2y != gameMap.p1y));

    for (i = 0; i < n + 2; i++){
        for (j = 0; j < m + 1; j++) {
            gameMap.Map[i][j] = 110;
            addedge(n, m, i, j, &mygraph);
        } // fulfiling tha map as if there is no player and wall
    }
    makeUnVisit(&mygraph);
    gameMap.Map[gameMap.p1x][gameMap.p1y] = 111; // puting the player 1 at the right place
    gameMap.Map[gameMap.p2x][gameMap.p2y] = 112; // doing the same thing for player number 2
    int p1Wall, p2Wall;
    do {
    	printf("please enter the number of first player's walls: \n");
    	scanf("%d", &p1Wall);
    	if (p1Wall > 10) printf("you can not have more than 10 walls! try again: \n"); // we know that there can be only 10 walls for each player
		//it is also considered that the user does not enter a charachter that is not integer
	} while(p1Wall > 10 || p1Wall % 1 != 0);
	int x, y;
	char c;
	for (i = 0; i < p1Wall; i++) {
		printf("player number 1, please enter the wall number %d starting location and type(H or V): \n", i + 1);
		scanf("%d %d %c", &x, &y, &c); // getting the location of each player's walls
		if ((c == 'h' || c == 'H') && (x >= gameMap.n || y >= gameMap.m - 1)) {
			printf("Error! you have enterd invalid location(you can not place a wall there!\n");
			i--;
			continue;
		}
		else if ((c == 'v' || c == 'V') && (x >= gameMap.n - 1 || y >= gameMap.m)) {
            printf("Error! you have entered invalid location(you can not place a wall there)! try again: \n");
            i--;
			continue;
        }
        else if(validWallH(c, x, y, gameMap, &mygraph)) {
        	gameMap.Map[x][y] += 10;
            gameMap.Map[x][y + 1] += 10;
            continue;
        }
        else if(validWallV(c, x, y, gameMap, &mygraph)){
            gameMap.Map[x][y] += 100;
            gameMap.Map[x + 1][y] += 100;
    		continue;
            }
            else{
            	printf("Error! you have entered invalid character(you can not place a wall there)! try again: \n");
                i--;
				continue;
            }
	}
	do { // the same for the second player(if the game becomes a 4-person game this to do-whiles should be made into functions which will be done in upcomming projects)
		printf("please enter the number of second player's walls: \n");
    	scanf("%d", &p2Wall);
    	if (p2Wall > 10) printf("you can not have more than 10 walls! try again: \n");
    	else if (p2Wall % 1 != 0) printf("the number of your walls can not be a float number! try again: \n");
	} while (p2Wall > 10 || p2Wall % 1 != 0);
	for (i = 0; i < p1Wall; i++) {
		printf("player number 1, please enter the wall number %d starting location and type(H or V): \n", i + 1);
		scanf("%d %d %c", &x, &y, &c); // getting the location of each player's walls
		if ((c == 'h' || c == 'H') && (x >= gameMap.n || y >= gameMap.m - 1)) {
			printf("Error! you have enterd invalid location(you can not place a wall there!\n");
			i--;
			continue;
		}
		else if ((c == 'v' || c == 'V') && (x >= gameMap.n - 1 || y >= gameMap.m)) {
            printf("Error! you have entered invalid location(you can not place a wall there)! try again: \n");
            i--;
			continue;
        }
        else if(validWallH(c, x, y, gameMap, &mygraph)) {
        	gameMap.Map[x][y] += 10;
            gameMap.Map[x][y + 1] += 10;
            continue;
        }
        else if(validWallV(c, x, y, gameMap, &mygraph)){
            gameMap.Map[x][y] += 100;
            gameMap.Map[x + 1][y] += 100;
    		continue;
            }
        else{
           	printf("Error! you have entered invalid character(you can not place a wall there)! try again: \n");
            i--;
			continue;
        }
	}
    PrintTheMap(gameMap); // it prints the map
}

