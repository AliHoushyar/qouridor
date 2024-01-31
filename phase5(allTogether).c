#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

const int maxDepth = 4;
const int MAX_WALLS = 10;
int lock1 = 0, lock2 = 0;
const int DELAY = 2200;

struct myWall{
    char typeH,typeV,Best;
    int sx,sy;
};

struct board{
    int n, m;
    int Map[100][100];
    int p1x, p1y, p2x, p2y;
    int p1w, p2w;
    int player1_walls[10][3];
    int player2_walls[10][3];
    int p1placedwalls, p2placedwalls;
    struct myWall vw[10000];
};

void sleep(unsigned int mseconds) {
    clock_t goal = mseconds + clock();
    while(goal > clock());
}

struct Graph{
    int visited[10000];
    int adjList[10000][4];
    int dist[10000];
    int visitSet[10000];
};

void setTextColor(int textColor, int backColor) {
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	int colorAttribute = backColor << 4 | textColor;
	SetConsoleTextAttribute(consoleHandle, colorAttribute);
}

void max_walls_checker(struct board *gameMap) {
	int p1check;
	int p2check;
	if (gameMap->p1w > 10) {
		p1check = (gameMap->p1w) / 10;
		gameMap->p1w = p1check * 10;
	}
	if (gameMap->p2w > 10) {
		p2check = (gameMap->p2w - 1) / 10;
		gameMap->p2w = p2check * 10;
	}

}
void addedge(int n,int m,int i,int j,struct Graph *mygraph){
    if(i >= n || j >= m){
        mygraph -> adjList[i * (m + 1) + j][0] = -1;
        mygraph->adjList[i * (m + 1) + j][1] = -1;
        mygraph->adjList[i * (m + 1) + j][2]= -1;
        mygraph->adjList[i * (m + 1) + j][3]= -1;
    }
    else if(i == 0){
        if(j == 0){
            mygraph -> adjList[i * (m + 1) + j][0]= i * (m + 1) + j + m + 1;
            mygraph -> adjList[i * (m + 1) + j][1]= i * (m + 1) + j + 1;
            mygraph -> adjList[i * (m + 1) + j][2]= -1;
            mygraph -> adjList[i * (m + 1) + j][3]= -1;
        }
        else if(j == m - 1){
            mygraph -> adjList[i * (m + 1) + j][0]= i * (m + 1) + j + m +1;
            mygraph -> adjList[i * (m + 1) + j][1]= i * (m + 1) + j - 1;
            mygraph -> adjList[i * (m + 1) + j][2]= -1;
            mygraph -> adjList[i * (m + 1) + j][3]= -1;
        }
        else{
            mygraph -> adjList[i * (m + 1) + j][0]= i *(m + 1) + j + m + 1;
            mygraph -> adjList[i * (m + 1) + j][1]= i * (m + 1) + j - 1;
            mygraph -> adjList[i * (m + 1) + j][2]= i * (m + 1) + j + 1;
            mygraph -> adjList[i * (m + 1) + j][3]= -1;
        }
    }
    else if(i == n - 1){
        if(j == 0){
            mygraph -> adjList[i * (m + 1) + j][0]= i * (m + 1) + j - m - 1;
            mygraph -> adjList[i * (m + 1) + j][1]= i * (m + 1) + j + 1;
            mygraph -> adjList[i * (m + 1) + j][2]= -1;
            mygraph -> adjList[i * (m + 1) + j][3]= -1;
        }
        else if(j == m - 1){
            mygraph -> adjList[i*(m+1)+j][0]= i*(m+1)+j-m-1;
            mygraph -> adjList[i * (m + 1) + j][1]= i * (m + 1) + j - 1;
            mygraph -> adjList[i * (m + 1) + j][2]= -1;
            mygraph->adjList[i * (m + 1) + j][3]= -1;
        }
        else{
            mygraph -> adjList[i * (m + 1) + j][0]= i * (m +1 ) + j - m - 1;
            mygraph -> adjList[i * (m + 1) + j][1]= i * (m + 1) + j - 1;
            mygraph -> adjList[i * (m + 1) + j][2]= i * (m + 1) + j + 1;
            mygraph -> adjList[i * (m + 1) + j][3]= -1;
        }
    }
    else{
        if(j == 0){
            mygraph -> adjList[i * (m + 1) + j][0]= i * (m + 1) + j + m + 1;
            mygraph -> adjList[i * (m + 1) + j][1]= i * (m + 1) + j - m - 1;
            mygraph -> adjList[i * (m + 1) + j][2] = i * (m + 1) + j + 1;
            mygraph -> adjList[i * (m + 1) + j][3] = -1;
        }
        else if(j == m - 1){
            mygraph -> adjList[i * (m + 1) + j][0] = i * (m + 1 ) + j + m + 1;
            mygraph -> adjList[i * (m + 1) + j][1] = i * (m + 1) + j - m - 1;
            mygraph -> adjList[i * (m + 1) + j][2] = i * (m + 1) + j - 1;
            mygraph -> adjList[i * (m + 1) + j][3]= -1;
        }
        else{
            mygraph -> adjList[i * (m + 1) + j][0] = i * (m + 1) + j + m + 1;
            mygraph->adjList[i * (m + 1) + j][1] = i * (m + 1) + j - m - 1;
            mygraph->adjList[i * (m + 1) + j][2] = i * (m + 1) + j - 1;
            mygraph->adjList[i * (m + 1) + j][3] = i * (m + 1) + j + 1;
        }
    }
}

void delEdge(int s, int e, struct Graph *gg){
	int i;
    for(i = 0; i < 4; i++){
        if(gg -> adjList[s][i] == e)
            gg -> adjList[s][i] =- 1;
    }
    for(i = 0; i < 4; i++){
        if(gg -> adjList[e][i] == s)
            gg -> adjList[e][i] =- 1;
    }
}

void undoDeleteEdge(int s,int e, struct Graph *mg){
	int i;
    for(i = 0; i < 4; i++){
        if(mg -> adjList[s][i] == -1){
            mg -> adjList[s][i] = e;
            break;
        }
    }
    for(i = 0; i < 4; i++){
        if(mg -> adjList[e][i] == -1){
            mg -> adjList[e][i] = s;
            break;
        }
    }
}

int flag1 = 0, flag2 = 0;

int dfs(int str, int who,int n,int m,struct Graph *g){
    g -> visited[str] = 1;
    if(who == 2 && str <= m - 1)
        flag2 = 1;
    if(who == 1 && str >= (n - 1) * (m + 1)){
        flag1 = 1;
    }
    int i;
    for(i = 0; i < 4;i++){
        if(g -> adjList[str][i] != -1){
            if(g -> visited[g -> adjList[str][i]] == 0)
                dfs(g -> adjList[str][i], who, n, m, g);
        }
    }
}

void makeUnVisit(struct Graph *g){
	int i;
    for(i = 0; i < 10000;i++){
        g -> visited[i] = 0;
    }
    return;
}

int getTheDirect() {
	int c = getch(), d = 0;
	if (c == 224) d = getch();
	else if(c == '0') return 5;
	else if(c=='W' || c=='w') return 6;
	if (d == 72) return 3;
	else if (d == 77) return 2;
	else if (d == 80) return 4;
	else if (d == 75) return 1;
	else return 0;
}

void clearScreen() {
	system("cls");
}

void PrintTheMap(struct board GameMap) {
	setTextColor(15, 0);
    int i, j, n = GameMap.n, m = GameMap.m;
    int left, up, play;
    clearScreen();
    printf("player one has %d wall(s) left and player 2(or computer) has %d walls left\n", GameMap.p1w, GameMap.p2w);
    printf("This is the Map(O stands for player 1 and X stands for player 2): \n");
    setTextColor(11, 0);
    printf("    ");
    for(i = 0; i < m; i++){
        if(i / 10 == 0)
            printf("  %d ", i);
        else
            printf(" %d ", i);
    }
    printf("\n");
    setTextColor(1, 0);
    for (i = 0; i < n + 1; i++) {
        for (j = 0; j < m + 1; j++) {
            if (j == m) continue;
            left = GameMap.Map[i][j] / 100;
            up = (GameMap.Map[i][j] / 10) % 10;
            play = GameMap.Map[i][j] % 10;
            if (up == 1){
                if(j==0){
                    printf("    ....");
                }
                else{
                    printf("....");
                }
            }
            else {
    			setTextColor(4, 0);
				if(j==0){
                    printf("    ====");
                }
                else{
                    printf("====");
                }
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
            if (left == 1){
                if(j==0){
                    setTextColor(11,0);
                    if(i/10 == 0)
                        printf(" %d  ",i);
                    else
                        printf(" %d ",i);
                    setTextColor(1,0);
                }
                printf(":");
            }
            else {
				if(j==0){
                    setTextColor(11, 0);
                    if(i/10 == 0)
                        printf(" %d  ",i);
                    else
                        printf(" %d ",i);
                    setTextColor(4, 0);
                    printf("|");
				}
                else
                {
                    setTextColor(4, 0);
                    printf("|");
                }
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

int random_maker(int lower, int upper) {
    int number = (rand() % (upper - lower + 1)) + lower;
    return number;
}

void deleteWalls(struct board *gameMap, struct Graph *g, int x, int y, bool check) {
    if (!check) {
        (*gameMap).Map[x][y] -= 10;
        (*gameMap).Map[x][y + 1] -= 10;
        undoDeleteEdge((x*(gameMap->m+1)+y),(x*(gameMap->m+1)+y-gameMap->m-1),g);
        undoDeleteEdge((x*(gameMap->m+1)+y+1),(x*(gameMap->m+1)+y-gameMap->m),g);
    }
    else {
        (*gameMap).Map[x][y] -= 100;
        (*gameMap).Map[x + 1][y] -= 100;
        undoDeleteEdge((x*(gameMap->m+1)+y),(x*(gameMap->m+1)+y-1),g);
        undoDeleteEdge((x*(gameMap->m+1)+y+gameMap->m+1),(x*(gameMap->m+1)+y+gameMap->m),g);
    }
}

int makeSpell(struct board *gameMap, struct Graph *g, int key) {
    srand(time(0));
    int first = random_maker(1, 3), second;
    int i, j;
    if (!(key % 2)) {
        if (first == 1 && gameMap->p1placedwalls == 0) {
            first++;
        }
        else if (first == 2 && (*gameMap).p1w <= 1) first++;
        if (first == 1) {
            for (i = 0; i < gameMap->p1placedwalls; i++) {
                deleteWalls(gameMap, g, (*gameMap).player1_walls[i][0], (*gameMap).player1_walls[i][1], (*gameMap).player1_walls[i][2]);
            }
            gameMap->p1placedwalls = 0;
            sleep(DELAY);
            max_walls_checker(gameMap);
            PrintTheMap(*gameMap);
            printf("player %d's turn(0 to exit , W for placing wall , arrows to move): \n", key % 2 + 1);
            printf("you have lost your placed walls!\n");
            return 10;
        }
        else if (first == 2) {
            do {
                second = random_maker(2, 5);
            } while(!(second != 4 && (*gameMap).p1w >= second));
            (*gameMap).p1w -= second;
            sleep(DELAY);
            max_walls_checker(gameMap);
            PrintTheMap(*gameMap);
            printf("player %d's turn(0 to exit , W for placing wall , arrows to move): \n", key % 2 + 1);
            printf("you have lost %d walls\n", second);
            return 20;
        }
        else if (first == 3) {
            second = random_maker(1, 2);
            sleep(DELAY);
            printf("you can not move for %d round(s)", second);
            return second;
        }
    }
    else {
        if (first == 1 && gameMap->p2placedwalls == 0) {
            first++;
        }
        else if (first == 2 && gameMap->p2placedwalls <= 1) first++;
        if (first == 1) {
            for (i = 0; i < gameMap->p2placedwalls; i++) {
                deleteWalls(gameMap, g, (*gameMap).player2_walls[i][0], (*gameMap).player2_walls[i][1], (*gameMap).player2_walls[i][2]);
            }
            gameMap->p2placedwalls = 0;
            sleep(DELAY);
            max_walls_checker(gameMap);
            PrintTheMap(*gameMap);
            printf("player %d's turn(0 to exit , W for placing wall , arrows to move): \n", key % 2 + 1);
            printf("you have lost your placed walls!\n");
            return 10;
        }
        else if (first == 2) {
            do {
                second = random_maker(2, 5);
            } while(!(second != 4 && (*gameMap).p2w >= second));
            (*gameMap).p2w -= second;
            sleep(DELAY);
            max_walls_checker(gameMap);
            PrintTheMap(*gameMap);
            printf("player %d's turn(0 to exit , W for placing wall , arrows to move): \n", key % 2 + 1);
            printf("you have lost %d walls\n", second);
            return 20;
        }
        else if (first == 3) {
            second = random_maker(1, 2);
            sleep(DELAY);
            printf("you can not move for %d round(s)", second);
            return second;
        }
    }
}

int makePresent(struct board *gameMap, int key) {
    srand(time(0));
    int ran, mines, plus;
	ran = random_maker(1, 2);
    if (!(key % 2)) {
        if (ran == 1 && gameMap->p2w >= 2) {
            do {
                mines = random_maker(1, 2);
            } while(mines > gameMap->p2w);
            gameMap->p2w -= mines;
            gameMap->p1w += mines;
            sleep(DELAY);
            max_walls_checker(gameMap);
            PrintTheMap(*gameMap);
            printf("player %d's turn(0 to exit , W for placing wall , arrows to move): \n", key % 2 + 1);
            printf("your enemy has lost %d walls and you have %d more walls\n", mines, mines);
        }
        else {
            do {
                plus = random_maker(2, 5);
            } while(plus == 4);
            gameMap->p1w += plus;

            sleep(DELAY);
            max_walls_checker(gameMap);
            PrintTheMap(*gameMap);
            printf("player %d's turn(0 to exit , W for placing wall , arrows to move): \n", key % 2 + 1);
            printf("you have %d more walls now\n", plus);
        }
    }
    else {
        if (ran == 1 && gameMap->p1w >= 2) {
            do {
                mines = random_maker(1, 2);
            } while(mines > gameMap->p1w);
            gameMap->p1w -= mines;
            gameMap->p2w += mines;
            sleep(DELAY);
            max_walls_checker(gameMap);
            PrintTheMap(*gameMap);
            printf("player %d's turn(0 to exit , W for placing wall , arrows to move): \n", key % 2 + 1);
            printf("your enemy has lost %d walls and you have %d more walls\n", mines, mines);
        }
        else {
            do {
                plus = random_maker(2, 5);
            } while(plus == 4);
            gameMap->p2w += plus;
            sleep(DELAY);
            max_walls_checker(gameMap);
            PrintTheMap(*gameMap);
            printf("player %d's turn(0 to exit , W for placing wall , arrows to move): \n", key % 2 + 1);
            printf("you have %d more walls now\n", plus);
        }
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

int utility(struct board *state, struct Graph *g){
    int res=0;
    dijkastra(g,(state->p1x*(state->m+1)+state->p1y),*state);
    int opdis=INT_MAX;
    for(int i=0;i<state->m+1;i++){
        opdis = min(opdis,g->dist[((state->n-1)*(state->m+1)+1)+i]);
    }
    dijkastra(g,(state->p2x*(state->m+1)+state->p2y),*state);
    int mydis=INT_MAX;
    for(int i=0;i<state->m+1;i++){
        mydis = min(mydis,g->dist[i]);
    }
    res+=(state->p1w-state->p2w);
    res+= opdis*2;
    res-= mydis*2;
    res+=(state->n-1-state->p1x);
    res-=(state->p2x);
    //printf("%d,%d .VS. %d,%d %d --> %d |||||| %d \n",state->p1x,state->p1y,state->p2x,state->p2y,res,opdis,mydis);
    //printf("\n");
    return res;
}

int mindist (const struct Graph *gr, struct board mapp){
    int minimum = INT_MAX, min_index;
    for(int i = 0;i<(mapp.n+2)*(mapp.m+1);i++){
        if(gr->visitSet[i]==0 && gr->dist[i]<=minimum){
            minimum = gr->dist[i];
            min_index = i;
        }
    }
    return min_index;
}

void dijkastra(struct Graph *gr,int src,struct board mapp){
    for(int i=0;i<(mapp.n+2)*(mapp.m+1);i++){
        gr->dist[i] = INT_MAX;
        gr->visitSet[i]=0;
    }
    gr->dist[src]=0;
    for(int i=0;i<(mapp.n+2)*(mapp.m+1);i++){
        int u = mindist(gr,mapp);
        gr->visitSet[u] = 1;
        int adj[] = {0,0,0,0};
        for(int k=0;k<4;k++){
            if(gr->adjList[u][k]!=-1)
                adj[k]=gr->adjList[u][k];
        }
        for(int j=0;j<(mapp.n+2)*(mapp.m+1);j++){
            if(gr->visitSet[j]==0){
                for(int k=0;k<4;k++){
                    if(gr->adjList[u][k] == j && gr->dist[u]!=INT_MAX){
                        if(gr->dist[u]+1<gr->dist[j]){
                            gr->dist[j]=gr->dist[u]+1;
                        }
                    }
                }
            }
        }
    }
}

//left = GameMap.Map[i][j] / 100;
//up = (GameMap.Map[i][j] / 10) % 10;

void findMoves(struct board gameState, int w, int arr[]){
    int x,y;
    if(w==1){
        x=gameState.p1x;
        y=gameState.p1y;
    }
    else{
        x=gameState.p2x;
        y=gameState.p2y;
    }
    if(gameState.Map[x][y]/100 == 1 && y > 0)
        arr[0] = 1;
    if(gameState.Map[x][y + 1]/100 == 1 && y < gameState.m - 1)
        arr[1] = 1;
    if((gameState.Map[x][y]/10)%10 == 1 && x > 0)
        arr[2] = 1;
    if((gameState.Map[x + 1][y]/10)%10 == 1 && x < gameState.n - 1)
        arr[3] = 1;
    return;
}

void findWalls(struct board *gameState, struct Graph *gr){
    struct Graph g = *gr;
    for(int i=0;i<gameState->n+2;i++){
        for(int j=0;j<gameState->m+1;j++){
            gameState->vw[i*(gameState->m + 1)+j].sx = i;
            gameState->vw[i*(gameState->m + 1)+j].sy = j;
            gameState->vw[i*(gameState->m + 1)+j].typeH = 'A';
            gameState->vw[i*(gameState->m + 1)+j].typeV = 'A';
            if (i >= gameState->n || j >= gameState->m - 1) {
                gameState->vw[i*(gameState->m + 1)+j].typeH = 'N';
            }
            if (i >= gameState->n - 1 || j >= gameState->m) {
                gameState->vw[i*(gameState->m + 1)+j].typeV = 'N';
            }
            if(gameState->vw[i*(gameState->m + 1)+j].typeH != 'N'){
                g = *gr;
                if(validWallH('h',i,j,*gameState,&g))
                    gameState->vw[i*(gameState->m + 1)+j].typeH = 'Y';
            }
            if(gameState->vw[i*(gameState->m + 1)+j].typeV != 'N'){
                g = *gr;
                if(validWallV('v',i,j,*gameState,&g))
                    gameState->vw[i*(gameState->m + 1)+j].typeV = 'Y';
            }
        }
    }
}

int terminal_state(struct board mymap){
    if (mymap.p2x == 0)
        return 2;
    else if(mymap.p1x == mymap.n - 1)
        return 1;
    else
        return 0;
}

int mini(int depth, struct board *gamemap, struct Graph *mygraph,int alpha, int beta);

int maxi(int depth, struct board *gamemap, struct Graph *mygraph,int alpha, int beta) {
    if(terminal_state(*gamemap) == 1)
        return (-1000);
    else if(terminal_state(*gamemap) == 2)
        return 2000;
    if (depth == 0) {
        //printf("DEPTH = 0 & -> p1x,p1y:%d,%d & p2x,p2y:%d,%d -> utility:%d\n",gamemap.p1x,gamemap.p1y,gamemap.p2x,gamemap.p2y,utility(gamemap,mygraph));
        return utility(gamemap,mygraph);
    }
    int best_value = INT_MIN;
    // Generate all possible moves for the maximizing player
    int moves[] = {0,0,0,0};
    findMoves(*gamemap,2,moves);
    int my = -1;
    int yy = depth;
    if(moves[2] == 1){
        //printf("(%d,%d) -> ",gamemap->p2x,gamemap->p2y);
        gamemap->p2x--;
        //printf("maxi in move 3 ::: depth = %d & -> p1x,p1y:%d,%d & p2x,p2y:%d,%d -> utility:%d\n",depth,gamemap->p1x,gamemap->p1y,gamemap->p2x,gamemap->p2y,utility(gamemap,mygraph));
        my = mini(yy-1,gamemap,mygraph,alpha,beta);
        best_value = max(best_value,my);
        //printf("maxi %d ::: in depth %d\n",best_value,depth);
        gamemap->p2x++;
        //printf("\n");
        beta = min(beta,best_value);
        if(beta <= alpha)
            return best_value;
    }
    if(moves[0] == 1){
        //printf("(%d,%d) -> ",gamemap->p2x,gamemap->p2y);
        gamemap->p2y--;
        //printf("maxi in move 1 ::: depth = %d & -> p1x,p1y:%d,%d & p2x,p2y:%d,%d -> utility:%d\n",depth,gamemap->p1x,gamemap->p1y,gamemap->p2x,gamemap->p2y,utility(gamemap,mygraph));
        my = mini(yy-1,gamemap,mygraph,alpha,beta);
        best_value = max(best_value,my);
        //printf("maxi %d ::: in depth %d\n",best_value,depth);
        gamemap->p2y++;
        //printf("\n");
        beta = min(beta,best_value);
        if(beta <= alpha)
            return best_value;
    }
    if(moves[1] == 1){
        //printf("(%d,%d) -> ",gamemap->p2x,gamemap->p2y);
        gamemap->p2y++;
        //printf("maxi in move 2 ::: depth = %d & -> p1x,p1y:%d,%d & p2x,p2y:%d,%d -> utility:%d\n",depth,gamemap->p1x,gamemap->p1y,gamemap->p2x,gamemap->p2y,utility(gamemap,mygraph));
        my = mini(yy-1,gamemap,mygraph,alpha,beta);
        best_value = max(best_value,my);
        //printf("maxi %d ::: in depth %d\n",best_value,depth);
        gamemap->p2y--;
        //printf("\n");
        beta = min(beta,best_value);
        if(beta <= alpha)
            return best_value;
    }
    if(moves[3] == 1){
        //printf("(%d,%d) -> ",gamemap->p2x,gamemap->p2y);
        gamemap->p2x++;
        //printf("maxi in move 4 ::: depth = %d & -> p1x,p1y:%d,%d & p2x,p2y:%d,%d -> utility:%d\n",depth,gamemap->p1x,gamemap->p1y,gamemap->p2x,gamemap->p2y,utility(gamemap,mygraph));
        my = mini(yy-1,gamemap,mygraph,alpha,beta);
        best_value = max(best_value,my);
        //printf("maxi %d ::: in depth %d\n",best_value,depth);
        gamemap->p2x--;
        //printf("\n");
        beta = min(beta,best_value);
        if(beta <= alpha)
            return best_value;
    }
    if(gamemap->p2w > 0 && gamemap->p1x>=(gamemap->n/2 - 1)){
        for(int i=1;i<gamemap->n;i++){
            for(int j=0;j<gamemap->m;j++){
                //printf("%d %d -> %c & %c\n",i,j,gamemap->vw[i*(gamemap->m+1)+j].typeH,gamemap->vw[i*(gamemap->m+1)+j].typeV);
                if(gamemap->vw[i*(gamemap->m+1)+j].typeH == 'Y' && i>gamemap->p1x  && j>=gamemap->p1y-1 && j<=gamemap->p1y+1){
                    delEdge((i*(gamemap->m+1)+j),(i*(gamemap->m+1)+j-gamemap->m-1),mygraph);
                    delEdge((i*(gamemap->m+1)+j+1),(i*(gamemap->m+1)+j-gamemap->m),mygraph);
                    gamemap->Map[i][j] += 10;
                    gamemap->Map[i][j + 1] += 10;
                    gamemap->p2w--;
                    gamemap->vw[i*(gamemap->m+1)+j].typeH == 'N';
                    my = mini(yy-1,gamemap,mygraph,alpha,beta);
                    best_value = max(best_value,my);
                    undoDeleteEdge((i*(gamemap->m+1)+j),(i*(gamemap->m+1)+j-gamemap->m-1),mygraph);
                    undoDeleteEdge((i*(gamemap->m+1)+j+1),(i*(gamemap->m+1)+j-gamemap->m),mygraph);
                    gamemap->Map[i][j] -= 10;
                    gamemap->Map[i][j + 1] -= 10;
                    gamemap->vw[i*(gamemap->m+1)+j].typeH == 'Y';
                    gamemap->p2w++;
                    beta = min(beta,best_value);
                    if(beta <= alpha)
                        return best_value;
                }
                if(gamemap->vw[i*(gamemap->m+1)+j].typeV == 'Y' && i>=gamemap->p1x - 1 && j>=gamemap->p1y-2 && j<=gamemap->p1y+3){
                    delEdge((i*(gamemap->m+1)+j),(i*(gamemap->m+1)+j-1),mygraph);
                    delEdge((i*(gamemap->m+1)+j+gamemap->m+1),(i*(gamemap->m+1)+j+gamemap->m),mygraph);
                    gamemap->Map[i][j] += 100;
                    gamemap->Map[i + 1][j] += 100;
                    gamemap->p2w--;
                    gamemap->vw[i*(gamemap->m+1)+j].typeV = 'N';
                    my = mini(yy-1,gamemap,mygraph,alpha,beta);
                    best_value = max(best_value,my);
                    undoDeleteEdge((i*(gamemap->m+1)+j),(i*(gamemap->m+1)+j-1),mygraph);
                    undoDeleteEdge((i*(gamemap->m+1)+j+gamemap->m+1),(i*(gamemap->m+1)+j+gamemap->m),mygraph);
                    gamemap->Map[i][j] -= 100;
                    gamemap->Map[i + 1][j] -= 100;
                    gamemap->p2w++;
                    gamemap->vw[i*(gamemap->m+1)+j].typeV = 'Y';
                    beta = min(beta,best_value);
                    if(beta <= alpha)
                        return best_value;
                }
            }
        }
    }
    //printf("maxi FINAL IN DEPTH : %d ::: %d\n",depth,best_value);
    return best_value;
}

int mini(int depth, struct board *gamemap, struct Graph *mygraph, int alpha, int beta) {
    if(terminal_state(*gamemap) == 1)
        return (-1000);
    else if(terminal_state(*gamemap) == 2)
        return 2000;
    if (depth == 0) {
        //printf("DEPTH = 0 & -> p1x,p1y:%d,%d & p2x,p2y:%d,%d -> utility:%d\n",gamemap.p1x,gamemap.p1y,gamemap.p2x,gamemap.p2y,utility(gamemap,mygraph));
        return utility(gamemap,mygraph);
    }
    int best_value = INT_MAX;
    // Generate all possible moves for the maximizing player
    int moves[] = {0,0,0,0};
    findMoves(*gamemap,1,moves);
    //printf("%d %d %d %d\n",moves[0],moves[1],moves[2],moves[3]);
    int my = -1;
    int yy = depth;
    if(moves[2] == 1){
        gamemap->p1x--;
        //printf("mini in move 3 ::: depth = %d & -> p1x,p1y:%d,%d & p2x,p2y:%d,%d -> utility:%d\n",depth,gamemap->p1x,gamemap->p1y,gamemap->p2x,gamemap->p2y,utility(gamemap,mygraph));
        my = maxi(yy-1,gamemap,mygraph,alpha,beta);
        best_value = min(best_value,my);
        //printf("mini > %d ::: in depth %d\n",best_value,depth);
        gamemap->p1x++;
        alpha = max(alpha,best_value);
        if(beta <= alpha)
            return best_value;
    }
    if(moves[0] == 1){
        gamemap->p1y--;
        //printf("mini in move 1 ::: depth = %d & -> p1x,p1y:%d,%d & p2x,p2y:%d,%d -> utility:%d\n",depth,gamemap->p1x,gamemap->p1y,gamemap->p2x,gamemap->p2y,utility(gamemap,mygraph));
        my = maxi(yy-1,gamemap,mygraph,alpha,beta);
        best_value = min(best_value,my);
        //printf("mini ! %d ::: in depth %d\n",best_value,depth);
        gamemap->p1y++;
        alpha = max(alpha,best_value);
        if(beta <= alpha)
            return best_value;
    }
    if(moves[1] == 1){
        gamemap->p1y++;
        //printf("mini in move 2 ::: depth = %d & -> p1x,p1y:%d,%d & p2x,p2y:%d,%d -> utility:%d\n",depth,gamemap->p1x,gamemap->p1y,gamemap->p2x,gamemap->p2y,utility(gamemap,mygraph));
        my = maxi(yy-1,gamemap,mygraph,alpha,beta);
        best_value = min(best_value,my);
        //printf("mini < %d ::: in depth %d\n",best_value,depth);
        gamemap->p1y--;
        alpha = max(alpha,best_value);
        if(beta <= alpha)
            return best_value;
    }
    if(moves[3] == 1){
        gamemap->p1x++;
        //printf("mini in move 4 ::: depth = %d & -> p1x,p1y:%d,%d & p2x,p2y:%d,%d -> utility:%d\n",depth,gamemap->p1x,gamemap->p1y,gamemap->p2x,gamemap->p2y,utility(gamemap,mygraph));
        my = maxi(yy-1,gamemap,mygraph,alpha,beta);
        best_value = min(best_value,my);
        //printf("mini ? %d ::: in depth %d\n",best_value,depth);
        gamemap->p1x--;
        alpha = max(alpha,best_value);
        if(beta <= alpha)
            return best_value;
    }
    if(gamemap->p1w > 0 && gamemap->p2x<=(gamemap->n/2 + 1)){
        for(int i=1;i<gamemap->n;i++){
            for(int j=0;j<gamemap->m+2;j++){
                if(gamemap->vw[i*(gamemap->m+1)+j].typeH == 'Y' && i<gamemap->p2x  && j>=gamemap->p2y-1 && j<=gamemap->p2y+1){
                    delEdge((i*(gamemap->m+1)+j),(i*(gamemap->m+1)+j-gamemap->m-1),mygraph);
                    delEdge((i*(gamemap->m+1)+j+1),(i*(gamemap->m+1)+j-gamemap->m),mygraph);
                    gamemap->Map[i][j] += 10;
                    gamemap->Map[i][j + 1] += 10;
                    gamemap->p1w--;
                    gamemap->vw[i*(gamemap->m+1)+j].typeH == 'N';
                    my = maxi(yy-1,gamemap,mygraph,alpha,beta);
                    best_value = min(best_value,my);
                    undoDeleteEdge((i*(gamemap->m+1)+j),(i*(gamemap->m+1)+j-gamemap->m-1),mygraph);
                    undoDeleteEdge((i*(gamemap->m+1)+j+1),(i*(gamemap->m+1)+j-gamemap->m),mygraph);
                    gamemap->Map[i][j] -= 10;
                    gamemap->Map[i][j + 1] -= 10;
                    gamemap->p1w++;
                    gamemap->vw[i*(gamemap->m+1)+j].typeH == 'Y';
                    alpha = max(alpha,best_value);
                    if(beta <= alpha)
                        return best_value;
                }
                if(gamemap->vw[i*(gamemap->m+1)+j].typeV == 'Y' && i>=gamemap->p2x + 1 && j>=gamemap->p2y-2 && j<=gamemap->p2y+3){
                    delEdge((i*(gamemap->m+1)+j),(i*(gamemap->m+1)+j-1),mygraph);
                    delEdge((i*(gamemap->m+1)+j+gamemap->m+1),(i*(gamemap->m+1)+j+gamemap->m),mygraph);
                    gamemap->Map[i][j] += 100;
                    gamemap->Map[i + 1][j] += 100;
                    gamemap->p1w--;
                    gamemap->vw[i*(gamemap->m+1)+j].typeV == 'N';
                    my = maxi(yy-1,gamemap,mygraph,alpha,beta);
                    best_value = min(best_value,my);
                    undoDeleteEdge((i*(gamemap->m+1)+j),(i*(gamemap->m+1)+j-1),mygraph);
                    undoDeleteEdge((i*(gamemap->m+1)+j+gamemap->m+1),(i*(gamemap->m+1)+j+gamemap->m),mygraph);
                    gamemap->Map[i][j] -= 100;
                    gamemap->Map[i + 1][j] -= 100;
                    gamemap->p1w++;
                    gamemap->vw[i*(gamemap->m+1)+j].typeV == 'Y';
                    alpha = max(alpha,best_value);
                    if(beta <= alpha)
                        return best_value;
                }
            }
        }
    }
    //printf("mini FINAL IN DEPTH : %d ::: %d\n",depth,best_value);
    return best_value;
}

int minimax(int depth, struct board *gamemap, struct Graph *mygraph,int alpha, int beta, struct myWall *theBest, int pre){
    int best_value = INT_MIN;
    int best2 = best_value;
    int best_move = 0;
    // Generate all possible moves for the maximizing player
    int moves[] = {0,0,0,0};
    findMoves(*gamemap,2,moves);
    int my = -1;
    if(moves[3] == 1){
        gamemap->p2x++;
        my = mini(depth,gamemap,mygraph,alpha,beta);
        if(best_value<=my){
            best_value = my;
            best_move = 4;
        }
        gamemap->p2x--;
        //printf("-----------------down-------|%d|----------|%d|--------------------------------------------------------------------------------\n",my,best_value);
    }
    if(moves[1] == 1){
        gamemap->p2y++;
        my = mini(depth,gamemap,mygraph,alpha,beta);
        if(best_value<=my){
            best_value = my;
            best_move = 2;
        }
        gamemap->p2y--;
        //printf("-----------------right-------|%d|----------|%d|-----------------------------------------------------------------------------\n",my,best_value);
    }
    if(moves[2] == 1){
        gamemap->p2x--;
        my = mini(depth,gamemap,mygraph,alpha,beta);
        if(best_value<=my){
            best_value = my;
            best_move = 3;
        }
        gamemap->p2x++;
        //printf("-----------------up---------|%d|--------|%d|----------------------------------------------------------------------------------\n",my,best_value);
    }
    if(moves[0] == 1){
        gamemap->p2y--;
        my = mini(depth,gamemap,mygraph,alpha,beta);
        if(best_value<=my){
            best_value = my;
            best_move = 1;
        }
        gamemap->p2y++;
        //printf("-----------------left-------|%d|----------|%d|----------------------------------------------------------------------------\n",my,best_value);
    }
    if(gamemap->p2w > 0 && gamemap->p1x>=(gamemap->n/2 - 1)){
        for(int i=gamemap->n;i>0;i--){
            for(int j=0;j<gamemap->m;j++){
                //printf("%d %d -> %c & %c\n",i,j,gamemap->vw[i*(gamemap->m+1)+j].typeH,gamemap->vw[i*(gamemap->m+1)+j].typeV);
                //printf("%d ::: %d\n",gamemap->p1x,i);
                if(gamemap->vw[i*(gamemap->m+1)+j].typeH == 'Y' && i>gamemap->p1x && j>=gamemap->p1y-1 && j<=gamemap->p1y+1){
                    //printf("H----------%d %d----------:",i,j);
                    delEdge((i*(gamemap->m+1)+j),(i*(gamemap->m+1)+j-gamemap->m-1),mygraph);
                    delEdge((i*(gamemap->m+1)+j+1),(i*(gamemap->m+1)+j-gamemap->m),mygraph);
                    gamemap->Map[i][j] += 10;
                    gamemap->Map[i][j + 1] += 10;
                    gamemap->p2w--;
                    my = mini(depth,gamemap,mygraph,alpha,beta);
                    //printf("%d\n",my);
                    if(best2<=my){
                        best2 = my;
                        theBest->sx = i;
                        theBest->sy = j;
                        theBest->Best = 'H';
                    }
                    undoDeleteEdge((i*(gamemap->m+1)+j),(i*(gamemap->m+1)+j-gamemap->m-1),mygraph);
                    undoDeleteEdge((i*(gamemap->m+1)+j+1),(i*(gamemap->m+1)+j-gamemap->m),mygraph);
                    gamemap->Map[i][j] -= 10;
                    gamemap->Map[i][j + 1] -= 10;
                    gamemap->p2w++;
                }
            }
        }
        for(int i=gamemap->n;i>0;i--){
            for(int j=0;j<gamemap->m;j++){
                //printf("%d %d -> %c & %c\n",i,j,gamemap->vw[i*(gamemap->m+1)+j].typeH,gamemap->vw[i*(gamemap->m+1)+j].typeV);
                if(gamemap->vw[i*(gamemap->m+1)+j].typeV == 'Y' && i>=gamemap->p1x - 1 && j>=gamemap->p1y-2 && j<=gamemap->p1y+3){
                    //printf("V----------%d %d----------:",i,j);
                    delEdge((i*(gamemap->m+1)+j),(i*(gamemap->m+1)+j-1),mygraph);
                    delEdge((i*(gamemap->m+1)+j+gamemap->m+1),(i*(gamemap->m+1)+j+gamemap->m),mygraph);
                    gamemap->Map[i][j] += 100;
                    gamemap->Map[i + 1][j] += 100;
                    gamemap->p2w--;
                    my = mini(depth,gamemap,mygraph,alpha,beta);
                    //printf("%d\n",my);
                    if(best2<=my){
                        best2 = my;
                        theBest->sx = i;
                        theBest->sy = j;
                        theBest->Best = 'V';
                    }
                    undoDeleteEdge((i*(gamemap->m+1)+j),(i*(gamemap->m+1)+j-1),mygraph);
                    undoDeleteEdge((i*(gamemap->m+1)+j+gamemap->m+1),(i*(gamemap->m+1)+j+gamemap->m),mygraph);
                    gamemap->Map[i][j] -= 100;
                    gamemap->Map[i + 1][j] -= 100;
                    gamemap->p2w++;
                }
            }
        }
    }
    if(best2>best_value)
        best_move = 6;
    //printf("-----------------walls-----------------|%d|-----------|%d|---------------------------------------------------------------------\n",best_value,best_move);
    //printf("%c in (%d,%d)\n",theBest->Best,theBest->sx,theBest->sy);
    return best_move;
}

int main() {
    int n, m, i, j;
    int dif;
    int sw = 1;
    while(sw){
        printf("Please enter the size of the map: \n");
        scanf("%d", &n);
        if(n>=5)
                break;
        else
            printf("Wrong Input!!! Try Again : \n");
    }
    m = n;
    struct board gameMap;
    struct Graph mygraph;
    gameMap.p1y = floor(n/2);
    gameMap.p2y = gameMap.p1y;
    gameMap.p1x= 0;
    gameMap.p2x = n-1;
    gameMap.n = n;
    gameMap.m = m;
    gameMap.p1w = MAX_WALLS;
    gameMap.p2w = MAX_WALLS;
    gameMap.p1placedwalls = 0;
    gameMap.p2placedwalls = 0;
    for (i = 0; i < n + 2; i++) {
        for (j = 0; j < m + 1; j++) {
            gameMap.Map[i][j] = 110;
            addedge(n,m,i,j,&mygraph);
        }
    }
    makeUnVisit(&mygraph);
    char s;
    while(sw){
        printf("choose you opponent(c for computer and h for human): \n");
        scanf("\n%c", &s);
        if (!(s == 'c' || s == 'C' || s == 'H' || s == 'h')) {
                printf("invalid input! try again: \n");
        }
        else
            break;
    }
    int key = 0, move;
    if(s == 'C' || s=='c'){
        while(sw){
            printf("choose dificulity of AI you want to play with : \n");
            printf("1 for Beginner\n");//random
            printf("2 for Semi-pro\n");//random with rules
            printf("3 for Pro\n");//random with better ruls
            printf("4 for Legend\n");//minimax with depth 2
            printf("5 for Ultimate\n");//minimax with depth 4 and alpha-beta puring
            scanf("%d",&dif);
            if(dif>=1 && dif<=5)
                break;
            else
                printf("invalid input! try again: \n");
        }
    }
    gameMap.Map[gameMap.p1x][gameMap.p1y] = 111;
    gameMap.Map[gameMap.p2x][gameMap.p2y] = 112;
    PrintTheMap(gameMap);
    if (s == 'h' || s == 'H') {
    do {
        max_walls_checker(&gameMap);
    	PrintTheMap(gameMap);
    	if (gameMap.p1x == gameMap.n - 1) {
            setTextColor(2,0);
			printf("player 1 is the winner!");
			setTextColor(15,0);
			break;
		}
		else if(gameMap.p2x == 0) {
            setTextColor(14,0);
			printf("player 2 is the winner!");
            setTextColor(15,0);
			break;
		}
    	if (!(key % 2)) {
            if (lock1 > 0) {
                printf("player 1 tough to your last round spell you can't move this round!\n");
                sleep(DELAY);
                lock1--;
                key++;
               	continue;
            }
    		printf("player 1's turn(0 to exit , W for placing wall , arrows to move): \n");
    		int randomNumber = random_maker(0, 1);
            if (randomNumber) {
                printf("you have gotten a present:\n");
                makePresent(&gameMap, key);
                printf("\n");
            }
            else {
            	printf("you have been spelled!\n");
                int situation = makeSpell(&gameMap, &mygraph, key);
                if (situation == 1) {
                    key++;
                    sleep(DELAY);
                    continue;
                }
                else if (situation == 2) {
                    key++;
                    lock1 = 1;
                    sleep(DELAY);
                    continue;
                }
			}
            move = getTheDirect();
    		if (!move){
    			printf("wrong input!\n");
    			continue;
			}
			else if (move == 5) break;
    		if (move == 1) {
    			if (gameMap.p1y > 0 && gameMap.Map[gameMap.p1x][gameMap.p1y] / 100 == 1) {
                    if(gameMap.Map[gameMap.p1x][gameMap.p1y - 1] % 10 == 0){
                        gameMap.Map[gameMap.p1x][gameMap.p1y]--;
                        gameMap.p1y--;
                        gameMap.Map[gameMap.p1x][gameMap.p1y]++;
                        key++;
                    }
                    else if(gameMap.Map[gameMap.p1x][gameMap.p1y - 1] % 10 == 2){
                        if(gameMap.p1y > 1){
                            if(gameMap.Map[gameMap.p1x][gameMap.p1y - 1] / 100 == 1){
                                gameMap.Map[gameMap.p1x][gameMap.p1y]--;
                                gameMap.p1y -= 2;
                                gameMap.Map[gameMap.p1x][gameMap.p1y]++;
                                key++;
                            }
                            else{
                                int validmove[] = {0,0};
                                if((gameMap.Map[gameMap.p1x][gameMap.p1y - 1] / 10) % 10 == 1)
                                    validmove[0]=1;
                                if((gameMap.Map[gameMap.p1x + 1][gameMap.p1y - 1] / 10) % 10 == 1)
                                    validmove[1]=1;
                                if(validmove[0] && validmove[1]){
                                    printf("You can choose between going 1.forward-up(By push up arrow) or 2.forward-down(by push down arrow) : ");
                                    do{
                                        int where = getTheDirect();
                                        if(where == 3){
                                            gameMap.Map[gameMap.p1x][gameMap.p1y]--;
                                            gameMap.p1y--;
                                            gameMap.p1x--;
                                            gameMap.Map[gameMap.p1x][gameMap.p1y]++;
                                            key++;
                                            break;
                                        }
                                        else if(where == 4){
                                            gameMap.Map[gameMap.p1x][gameMap.p1y]--;
                                            gameMap.p1y--;
                                            gameMap.p1x++;
                                            gameMap.Map[gameMap.p1x][gameMap.p1y]++;
                                            key++;
                                            break;
                                        }
                                        else{
                                            printf("wrong move! try again\n");
                                            continue;
                                        }
                                    }while(1);
                                }
                                else if(validmove[0]==1){
                                    gameMap.Map[gameMap.p1x][gameMap.p1y]--;
                                    gameMap.p1y--;
                                    gameMap.p1x--;
                                    gameMap.Map[gameMap.p1x][gameMap.p1y]++;
                                    key++;
                                }
                                else if(validmove[1]==1){
                                    gameMap.Map[gameMap.p1x][gameMap.p1y]--;
                                    gameMap.p1y--;
                                    gameMap.p1x++;
                                    gameMap.Map[gameMap.p1x][gameMap.p1y]++;
                                    key++;
                                }
                                else{
                                    printf("wrong move! try again\n");
                                    continue;
                                }
                            }
                        }
                        else{
                            int validmove[] = {0,0};
                            if((gameMap.Map[gameMap.p1x][gameMap.p1y - 1] / 10) % 10 == 1)
                                validmove[0]=1;
                            if((gameMap.Map[gameMap.p1x + 1][gameMap.p1y - 1] / 10) % 10 == 1)
                                validmove[1]=1;
                            if(validmove[0] && validmove[1]){
                                printf("You can choose between going 1.forward-up(By push up arrow) or 2.forward-down(by push down arrow) : ");
                                do{
                                    int where = getTheDirect();
                                    if(where == 3){
                                        gameMap.Map[gameMap.p1x][gameMap.p1y]--;
                                        gameMap.p1y--;
                                        gameMap.p1x--;
                                        gameMap.Map[gameMap.p1x][gameMap.p1y]++;
                                        key++;
                                        break;
                                    }
                                    else if(where == 4){
                                        gameMap.Map[gameMap.p1x][gameMap.p1y]--;
                                        gameMap.p1y--;
                                        gameMap.p1x++;
                                        gameMap.Map[gameMap.p1x][gameMap.p1y]++;
                                        key++;
                                        break;
                                    }
                                    else{
                                        printf("wrong move! try again\n");
                                        continue;
                                    }
                                }while(1);
                            }
                            else if(validmove[0]==1){
                                gameMap.Map[gameMap.p1x][gameMap.p1y]--;
                                gameMap.p1y--;
                                gameMap.p1x--;
                                gameMap.Map[gameMap.p1x][gameMap.p1y]++;
                                key++;
                            }
                            else if(validmove[1]==1){
                                gameMap.Map[gameMap.p1x][gameMap.p1y]--;
                                gameMap.p1y--;
                                gameMap.p1x++;
                                gameMap.Map[gameMap.p1x][gameMap.p1y]++;
                                key++;
                            }
                            else{
                                printf("wrong move! try again\n");
                                continue;
                            }
                        }
                    }
				}
				else {
					printf("wrong move! try again\n");
					continue;
				}
			}
			else if (move == 2) {
				if (gameMap.p1y < gameMap.m - 1 && gameMap.Map[gameMap.p1x][gameMap.p1y + 1] / 100 == 1) {
                    if(gameMap.Map[gameMap.p1x][gameMap.p1y + 1] % 10 == 0){
                        gameMap.Map[gameMap.p1x][gameMap.p1y]--;
                        gameMap.p1y++;
                        gameMap.Map[gameMap.p1x][gameMap.p1y]++;
                        key++;
                    }
                    else if(gameMap.Map[gameMap.p1x][gameMap.p1y + 1] % 10 == 2){
                        if(gameMap.p1y < gameMap.m - 2){
                            if(gameMap.Map[gameMap.p1x][gameMap.p1y + 2] / 100 == 1){
                                gameMap.Map[gameMap.p1x][gameMap.p1y]--;
                                gameMap.p1y += 2;
                                gameMap.Map[gameMap.p1x][gameMap.p1y]++;
                                key++;
                            }
                            else{
                                int validmove[] = {0,0};
                                if((gameMap.Map[gameMap.p1x][gameMap.p1y + 1] / 10) % 10 == 1)
                                    validmove[0]=1;
                                if((gameMap.Map[gameMap.p1x + 1][gameMap.p1y + 1] / 10) % 10 == 1)
                                    validmove[1]=1;
                                if(validmove[0] && validmove[1]){
                                    printf("You can choose between going 1.forward-up(By push up arrow) or 2.forward-down(by push down arrow) : ");
                                    do{
                                        int where = getTheDirect();
                                        if(where == 3){
                                            gameMap.Map[gameMap.p1x][gameMap.p1y]--;
                                            gameMap.p1y++;
                                            gameMap.p1x--;
                                            gameMap.Map[gameMap.p1x][gameMap.p1y]++;
                                            key++;
                                            break;
                                        }
                                        else if(where == 4){
                                            gameMap.Map[gameMap.p1x][gameMap.p1y]--;
                                            gameMap.p1y++;
                                            gameMap.p1x++;
                                            gameMap.Map[gameMap.p1x][gameMap.p1y]++;
                                            key++;
                                            break;
                                        }
                                        else{
                                            printf("wrong move! try again\n");
                                            continue;
                                        }
                                    }while(1);
                                }
                                else if(validmove[0]==1){
                                    gameMap.Map[gameMap.p1x][gameMap.p1y]--;
                                    gameMap.p1y++;
                                    gameMap.p1x--;
                                    gameMap.Map[gameMap.p1x][gameMap.p1y]++;
                                    key++;
                                }
                                else if(validmove[1]==1){
                                    gameMap.Map[gameMap.p1x][gameMap.p1y]--;
                                    gameMap.p1y++;
                                    gameMap.p1x++;
                                    gameMap.Map[gameMap.p1x][gameMap.p1y]++;
                                    key++;
                                }
                                else{
                                    printf("wrong move! try again\n");
                                    continue;
                                }
                            }
                        }
                        else{
                            int validmove[] = {0,0};
                            if((gameMap.Map[gameMap.p1x][gameMap.p1y + 1] / 10) % 10 == 1)
                                validmove[0]=1;
                            if((gameMap.Map[gameMap.p1x + 1][gameMap.p1y + 1] / 10) % 10 == 1)
                                validmove[1]=1;
                            if(validmove[0] && validmove[1]){
                                printf("You can choose between going 1.forward-up(By push up arrow) or 2.forward-down(by push down arrow) : ");
                                    do{
                                int where = getTheDirect();
                                        if(where == 3){
                                        gameMap.Map[gameMap.p1x][gameMap.p1y]--;
                                        gameMap.p1y++;
                                        gameMap.p1x--;
                                        gameMap.Map[gameMap.p1x][gameMap.p1y]++;
                                        key++;
                                        break;
                                    }
                                    else if(where == 4){
                                        gameMap.Map[gameMap.p1x][gameMap.p1y]--;
                                        gameMap.p1y++;
                                        gameMap.p1x++;
                                        gameMap.Map[gameMap.p1x][gameMap.p1y]++;
                                        key++;
                                        break;
                                    }
                                    else{
                                        printf("wrong move! try again\n");
                                        continue;
                                    }
                                }while(1);
                            }
                            else if(validmove[0]==1){
                                gameMap.Map[gameMap.p1x][gameMap.p1y]--;
                                gameMap.p1y++;
                                gameMap.p1x--;
                                gameMap.Map[gameMap.p1x][gameMap.p1y]++;
                                key++;
                            }
                            else if(validmove[1]==1){
                                gameMap.Map[gameMap.p1x][gameMap.p1y]--;
                                gameMap.p1y++;
                                gameMap.p1x++;
                                gameMap.Map[gameMap.p1x][gameMap.p1y]++;
                                key++;
                            }
                            else{
                                printf("wrong move! try again\n");
                                continue;
                            }
                        }
                    }
				}
				else {
					printf("wrong move! try again\n");
					continue;
				}
			}
			else if (move == 3) {
				if (gameMap.p1x > 0 && (gameMap.Map[gameMap.p1x][gameMap.p1y] / 10) % 10 == 1) {
					if(gameMap.Map[gameMap.p1x - 1][gameMap.p1y] % 10 == 0){
                        gameMap.Map[gameMap.p1x][gameMap.p1y]--;
                        gameMap.p1x--;
                        gameMap.Map[gameMap.p1x][gameMap.p1y]++;
                        key++;
                    }
					else if(gameMap.Map[gameMap.p1x - 1][gameMap.p1y] % 10 == 2){
                        if(gameMap.p1x > 1){
                            if((gameMap.Map[gameMap.p1x - 1][gameMap.p1y] / 10) % 10 == 1){
                                gameMap.Map[gameMap.p1x][gameMap.p1y]--;
                                gameMap.p1x-=2;
                                gameMap.Map[gameMap.p1x][gameMap.p1y]++;
                                key++;
                            }
                            else{
                                int validmove[] = {0,0};
                                if(gameMap.Map[gameMap.p1x - 1][gameMap.p1y] / 100 == 1)
                                    validmove[0]=1;
                                if(gameMap.Map[gameMap.p1x - 1][gameMap.p1y + 1] / 100 == 1)
                                    validmove[1]=1;
                                if(validmove[0] && validmove[1]){
                                    printf("You can choose between going 1.forward-right(By push right arrow) or 2.forward-left(by push left arrow) : ");
                                    do{
                                        int where = getTheDirect();
                                        if(where == 1){
                                            gameMap.Map[gameMap.p1x][gameMap.p1y]--;
                                            gameMap.p1y--;
                                            gameMap.p1x--;
                                            gameMap.Map[gameMap.p1x][gameMap.p1y]++;
                                            key++;
                                            break;
                                        }
                                        else if(where == 2){
                                            gameMap.Map[gameMap.p1x][gameMap.p1y]--;
                                            gameMap.p1y++;
                                            gameMap.p1x--;
                                            gameMap.Map[gameMap.p1x][gameMap.p1y]++;
                                            key++;
                                            break;
                                        }
                                        else{
                                            printf("wrong move! try again\n");
                                            continue;
                                        }
                                    }while(1);
                                }
                                else if(validmove[0]==1){
                                    gameMap.Map[gameMap.p1x][gameMap.p1y]--;
                                    gameMap.p1y--;
                                    gameMap.p1x--;
                                    gameMap.Map[gameMap.p1x][gameMap.p1y]++;
                                    key++;
                                }
                                else if(validmove[1]==1){
                                    gameMap.Map[gameMap.p1x][gameMap.p1y]--;
                                    gameMap.p1y++;
                                    gameMap.p1x--;
                                    gameMap.Map[gameMap.p1x][gameMap.p1y]++;
                                    key++;
                                }
                                else{
                                    printf("wrong move! try again\n");
                                    continue;
                                }
                            }
                        }
                        else{
                            int validmove[] = {0,0};
                            if(gameMap.Map[gameMap.p1x - 1][gameMap.p1y] / 100 == 1)
                                validmove[0]=1;
                            if(gameMap.Map[gameMap.p1x - 1][gameMap.p1y + 1] / 100 == 1)
                                validmove[1]=1;
                            if(validmove[0] && validmove[1]){
                                printf("You can choose between going 1.forward-right(By push up arrow) or 2.forward-left(by push down arrow) : ");
                                do{
                                    int where = getTheDirect();
                                    if(where == 1){
                                        gameMap.Map[gameMap.p1x][gameMap.p1y]--;
                                        gameMap.p1y--;
                                        gameMap.p1x--;
                                        gameMap.Map[gameMap.p1x][gameMap.p1y]++;
                                        key++;
                                        break;
                                    }
                                    else if(where == 2){
                                        gameMap.Map[gameMap.p1x][gameMap.p1y]--;
                                        gameMap.p1y++;
                                        gameMap.p1x--;
                                        gameMap.Map[gameMap.p1x][gameMap.p1y]++;
                                        key++;
                                        break;
                                    }
                                    else{
                                        printf("wrong move! try again\n");
                                        continue;
                                    }
                                }while(1);
                            }
                            else if(validmove[0]==1){
                                gameMap.Map[gameMap.p1x][gameMap.p1y]--;
                                gameMap.p1y--;
                                gameMap.p1x--;
                                gameMap.Map[gameMap.p1x][gameMap.p1y]++;
                                key++;
                            }
                            else if(validmove[1]==1){
                                gameMap.Map[gameMap.p1x][gameMap.p1y]--;
                                gameMap.p1y++;
                                gameMap.p1x--;
                                gameMap.Map[gameMap.p1x][gameMap.p1y]++;
                                key++;
                            }
                            else{
                                printf("wrong move! try again\n");
                                continue;
                            }
                        }
					}
				}
				else {
					printf("wrong move! try again\n");
					continue;
				}
			}
			else if (move == 4) {
				if (gameMap.p1x < gameMap.n - 1 && (gameMap.Map[gameMap.p1x + 1][gameMap.p1y] / 10) % 10 == 1) {
					if(gameMap.Map[gameMap.p1x + 1][gameMap.p1y] % 10 == 0){
                        gameMap.Map[gameMap.p1x][gameMap.p1y]--;
                        gameMap.p1x++;
                        gameMap.Map[gameMap.p1x][gameMap.p1y]++;
                        key++;
                    }
					else if(gameMap.Map[gameMap.p1x + 1][gameMap.p1y] % 10 == 2){
                        if(gameMap.p1x < gameMap.n - 2){
                            if((gameMap.Map[gameMap.p1x + 2][gameMap.p1y] / 10) % 10 == 1){
                                gameMap.Map[gameMap.p1x][gameMap.p1y]--;
                                gameMap.p1x+=2;
                                gameMap.Map[gameMap.p1x][gameMap.p1y]++;
                                key++;
                            }
                            else{
                                int validmove[] = {0,0};
                                if(gameMap.Map[gameMap.p1x + 1][gameMap.p1y] / 100 == 1)
                                    validmove[0]=1;
                                if(gameMap.Map[gameMap.p1x + 1][gameMap.p1y + 1] / 100 == 1)
                                    validmove[1]=1;
                                if(validmove[0] && validmove[1]){
                                    printf("You can choose between going 1.forward-right(By push right arrow) or 2.forward-left(by push left arrow) : ");
                                    do{
                                        int where = getTheDirect();
                                        if(where == 1){
                                            gameMap.Map[gameMap.p1x][gameMap.p1y]--;
                                            gameMap.p1y--;
                                            gameMap.p1x++;
                                            gameMap.Map[gameMap.p1x][gameMap.p1y]++;
                                            key++;
                                            break;
                                        }
                                        else if(where == 2){
                                            gameMap.Map[gameMap.p1x][gameMap.p1y]--;
                                            gameMap.p1y++;
                                            gameMap.p1x++;
                                            gameMap.Map[gameMap.p1x][gameMap.p1y]++;
                                            key++;
                                            break;
                                        }
                                        else{
                                            printf("wrong move! try again\n");
                                            continue;
                                        }
                                    }while(1);
                                }
                                else if(validmove[0]==1){
                                    gameMap.Map[gameMap.p1x][gameMap.p1y]--;
                                    gameMap.p1y--;
                                    gameMap.p1x++;
                                    gameMap.Map[gameMap.p1x][gameMap.p1y]++;
                                    key++;
                                }
                                else if(validmove[1]==1){
                                    gameMap.Map[gameMap.p1x][gameMap.p1y]--;
                                    gameMap.p1y++;
                                    gameMap.p1x++;
                                    gameMap.Map[gameMap.p1x][gameMap.p1y]++;
                                    key++;
                                }
                                else{
                                    printf("wrong move! try again\n");
                                    continue;
                                }
                            }
                        }
                        else{
                            int validmove[] = {0,0};
                            if(gameMap.Map[gameMap.p1x + 1][gameMap.p1y] / 100 == 1)
                                validmove[0]=1;
                            if(gameMap.Map[gameMap.p1x + 1][gameMap.p1y + 1] / 100 == 1)
                                validmove[1]=1;
                            if(validmove[0] && validmove[1]){
                                printf("You can choose between going 1.forward-right(By push up arrow) or 2.forward-left(by push down arrow) : ");
                                do{
                                    int where = getTheDirect();
                                    if(where == 1){
                                        gameMap.Map[gameMap.p1x][gameMap.p1y]--;
                                        gameMap.p1y--;
                                        gameMap.p1x++;
                                        gameMap.Map[gameMap.p1x][gameMap.p1y]++;
                                        key++;
                                        break;
                                    }
                                    else if(where == 2){
                                        gameMap.Map[gameMap.p1x][gameMap.p1y]--;
                                        gameMap.p1y++;
                                        gameMap.p1x++;
                                        gameMap.Map[gameMap.p1x][gameMap.p1y]++;
                                        key++;
                                        break;
                                    }
                                    else{
                                        printf("wrong move! try again\n");
                                        continue;
                                    }
                                }while(1);
                            }
                            else if(validmove[0]==1){
                                gameMap.Map[gameMap.p1x][gameMap.p1y]--;
                                gameMap.p1y--;
                                gameMap.p1x++;
                                gameMap.Map[gameMap.p1x][gameMap.p1y]++;
                                key++;
                            }
                            else if(validmove[1]==1){
                                gameMap.Map[gameMap.p1x][gameMap.p1y]--;
                                gameMap.p1y++;
                                gameMap.p1x++;
                                gameMap.Map[gameMap.p1x][gameMap.p1y]++;
                                key++;
                            }
                            else{
                                printf("wrong move! try again\n");
                                continue;
                            }
                        }
					}
				}
				else {
					printf("wrong move! try again\n");
					continue;
				}
			}
			else if(move == 6){
				if (gameMap.p1w == 0) {
					printf("you can't place anymore walls!");
					continue;
				}
                printf("enter the wall starting location and (H) for horizontal and (V) for vertical\n");
                int x,y;
                char c;
                while(1){
                    scanf("%d %d %c",&x,&y,&c);
                    if ((c == 'h' || c == 'H' ) && (x >= gameMap.n || y >= gameMap.m - 1)) {
                        printf("Error! you have entered invalid location(you can not place a wall there)! try again: \n");
                        continue;
                    }
                    else if ((c == 'v' || c == 'V') && (x >= gameMap.n - 1 || y >= gameMap.m)) {
                        printf("Error! you have entered invalid location(you can not place a wall there)! try again: \n");
                        continue;
                    }
                    else if(validWallH(c, x, y, gameMap, &mygraph)) {
                        gameMap.player1_walls[gameMap.p1placedwalls][0] = x;
                        gameMap.player1_walls[gameMap.p1placedwalls][1] = y;
                        gameMap.player1_walls[gameMap.p1placedwalls][2] = 0;
                        gameMap.Map[x][y] += 10;
                        gameMap.Map[x][y + 1] += 10;
                        key++;
                        gameMap.p1w--;
                        gameMap.p1placedwalls++;
                        break;
                    }
                    else if(validWallV(c, x, y, gameMap, &mygraph)){
                        gameMap.player1_walls[gameMap.p1placedwalls][0] = x;
                        gameMap.player1_walls[gameMap.p1placedwalls][1] = y;
                        gameMap.player1_walls[gameMap.p1placedwalls][2] = 1;
                        gameMap.Map[x][y] += 100;
                        gameMap.Map[x + 1][y] += 100;
                        key++;
                        gameMap.p1w--;
                        gameMap.p1placedwalls++;
                        break;
                    }
                    else{
                        printf("Error! you have entered invalid character(you can not place a wall there)! try again: \n");
                        continue;
                    }
                }
            }
			else {
				printf("you have entered invalid character! try again!\n");
				continue;
			}
		}
		else {
            if (lock2 > 0) {
                printf("player 2 tough to your last round spell you can't move this round!\n");
                sleep(DELAY);
                lock2--;
                key++;
                continue;
            }
			printf("player 2's turn(0 to exit , W for placing wall , arrows to move): \n");
    		int randomNumber = random_maker(0, 1);
            if (randomNumber) {
                printf("you have gotten a present:\n");
                makePresent(&gameMap, key);
                printf("\n");
            }
            else {
            	printf("you have been spelled!\n");
                int situation = makeSpell(&gameMap, &mygraph, key);
                if (situation == 1) {
                    key++;
                    sleep(DELAY);
                    continue;
                }
                else if (situation == 2) {
                    key++;
                    lock2 = 1;
                    sleep(DELAY);
                    continue;
                }
			}
            move = getTheDirect();
    		if (!move){
    			printf("wrong input!\n");
    			continue;
			}
			else if (move == 5) break;
    		if (move == 1) {
    			if (gameMap.p2y > 0 && gameMap.Map[gameMap.p2x][gameMap.p2y] / 100 == 1) {
                    if(gameMap.Map[gameMap.p2x][gameMap.p2y - 1] % 10 == 0){
                        gameMap.Map[gameMap.p2x][gameMap.p2y]-=2;
                        gameMap.p2y--;
                        gameMap.Map[gameMap.p2x][gameMap.p2y]+=2;
                        key++;
                    }
                    else if(gameMap.Map[gameMap.p2x][gameMap.p2y - 1] % 10 == 1){
                        if(gameMap.p2y > 1){
                            if(gameMap.Map[gameMap.p2x][gameMap.p2y - 1] / 100 == 1){
                                gameMap.Map[gameMap.p2x][gameMap.p2y]-=2;
                                gameMap.p2y -= 2;
                                gameMap.Map[gameMap.p2x][gameMap.p2y]+=2;
                                key++;
                            }
                            else{
                                int validmove[] = {0,0};
                                if((gameMap.Map[gameMap.p2x][gameMap.p2y - 1] / 10) % 10 == 1)
                                    validmove[0]=1;
                                if((gameMap.Map[gameMap.p2x + 1][gameMap.p2y - 1] / 10) % 10 == 1)
                                    validmove[1]=1;
                                if(validmove[0] && validmove[1]){
                                    printf("You can choose between going 1.forward-up(By push up arrow) or 2.forward-down(by push down arrow) : ");
                                    do{
                                        int where = getTheDirect();
                                        if(where == 3){
                                            gameMap.Map[gameMap.p2x][gameMap.p2y]-=2;
                                            gameMap.p2y--;
                                            gameMap.p2x--;
                                            gameMap.Map[gameMap.p2x][gameMap.p2y]+=2;
                                            key++;
                                            break;
                                        }
                                        else if(where == 4){
                                            gameMap.Map[gameMap.p2x][gameMap.p2y]-=2;
                                            gameMap.p2y--;
                                            gameMap.p2x++;
                                            gameMap.Map[gameMap.p2x][gameMap.p2y]+=2;
                                            key++;
                                            break;
                                        }
                                        else{
                                            printf("wrong move! try again\n");
                                            continue;
                                        }
                                    }while(1);
                                }
                                else if(validmove[0]==1){
                                    gameMap.Map[gameMap.p2x][gameMap.p2y]-=2;
                                    gameMap.p2y--;
                                    gameMap.p2x--;
                                    gameMap.Map[gameMap.p2x][gameMap.p2y]+=2;
                                    key++;
                                }
                                else if(validmove[1]==1){
                                    gameMap.Map[gameMap.p2x][gameMap.p2y]-=2;
                                    gameMap.p2y--;
                                    gameMap.p2x++;
                                    gameMap.Map[gameMap.p2x][gameMap.p2y]+=2;
                                    key++;
                                }
                                else{
                                    printf("wrong move! try again\n");
                                    continue;
                                }
                            }
                        }
                        else{
                            int validmove[] = {0,0};
                            if((gameMap.Map[gameMap.p2x][gameMap.p2y - 1] / 10) % 10 == 1)
                                validmove[0]=1;
                            if((gameMap.Map[gameMap.p2x + 1][gameMap.p2y - 1] / 10) % 10 == 1)
                                validmove[1]=1;
                            if(validmove[0] && validmove[1]){
                                printf("You can choose between going 1.forward-up(By push up arrow) or 2.forward-down(by push down arrow) : ");
                                do{
                                    int where = getTheDirect();
                                    if(where == 3){
                                        gameMap.Map[gameMap.p2x][gameMap.p2y]-=2;
                                        gameMap.p2y--;
                                        gameMap.p2x--;
                                        gameMap.Map[gameMap.p2x][gameMap.p2y]+=2;
                                        key++;
                                        break;
                                    }
                                    else if(where == 4){
                                        gameMap.Map[gameMap.p2x][gameMap.p2y]-=2;
                                        gameMap.p2y--;
                                        gameMap.p2x++;
                                        gameMap.Map[gameMap.p2x][gameMap.p2y]+=2;
                                        key++;
                                        break;
                                    }
                                    else{
                                        printf("wrong move! try again\n");
                                        continue;
                                    }
                                }while(1);
                            }
                            else if(validmove[0]==1){
                                gameMap.Map[gameMap.p2x][gameMap.p2y]-=2;
                                gameMap.p2y--;
                                gameMap.p2x--;
                                gameMap.Map[gameMap.p2x][gameMap.p2y]+=2;
                                key++;
                            }
                            else if(validmove[1]==1){
                                gameMap.Map[gameMap.p2x][gameMap.p2y]-=2;
                                gameMap.p2y--;
                                gameMap.p2x++;
                                gameMap.Map[gameMap.p2x][gameMap.p2y]+=2;
                                key++;
                            }
                            else{
                                printf("wrong move! try again\n");
                                continue;
                            }
                        }
                    }
				}
				else {
					printf("wrong move! try again\n");
					continue;
				}
			}
			else if (move == 2) {
				if (gameMap.p2y < gameMap.m - 1 && gameMap.Map[gameMap.p2x][gameMap.p2y + 1] / 100 == 1) {
                    if(gameMap.Map[gameMap.p2x][gameMap.p2y + 1] % 10 == 0){
                        gameMap.Map[gameMap.p2x][gameMap.p2y]-=2;
                        gameMap.p2y++;
                        gameMap.Map[gameMap.p2x][gameMap.p2y]+=2;
                        key++;
                    }
                    else if(gameMap.Map[gameMap.p2x][gameMap.p2y + 1] % 10 == 1){
                        if(gameMap.p2y < gameMap.m - 2){
                            if(gameMap.Map[gameMap.p2x][gameMap.p2y + 2] / 100 == 1){
                                gameMap.Map[gameMap.p2x][gameMap.p2y]-=2;
                                gameMap.p2y += 2;
                                gameMap.Map[gameMap.p2x][gameMap.p2y]+=2;
                                key++;
                            }
                            else{
                                int validmove[] = {0,0};
                                if((gameMap.Map[gameMap.p2x][gameMap.p2y + 1] / 10) % 10 == 1)
                                    validmove[0]=1;
                                if((gameMap.Map[gameMap.p2x + 1][gameMap.p2y + 1] / 10) % 10 == 1)
                                    validmove[1]=1;
                                if(validmove[0] && validmove[1]){
                                    printf("You can choose between going 1.forward-up(By push up arrow) or 2.forward-down(by push down arrow) : ");
                                    do{
                                        int where = getTheDirect();
                                        if(where == 3){
                                            gameMap.Map[gameMap.p2x][gameMap.p2y]-=2;
                                            gameMap.p2y++;
                                            gameMap.p2x--;
                                            gameMap.Map[gameMap.p2x][gameMap.p2y]+=2;
                                            key++;
                                            break;
                                        }
                                        else if(where == 4){
                                            gameMap.Map[gameMap.p2x][gameMap.p2y]-=2;
                                            gameMap.p2y++;
                                            gameMap.p2x++;
                                            gameMap.Map[gameMap.p2x][gameMap.p2y]+=2;
                                            key++;
                                            break;
                                        }
                                        else{
                                            printf("wrong move! try again\n");
                                            continue;
                                        }
                                    }while(1);
                                }
                                else if(validmove[0]==1){
                                    gameMap.Map[gameMap.p2x][gameMap.p2y]-=2;
                                    gameMap.p2y++;
                                    gameMap.p2x--;
                                    gameMap.Map[gameMap.p2x][gameMap.p2y]+=2;
                                    key++;
                                }
                                else if(validmove[1]==1){
                                    gameMap.Map[gameMap.p2x][gameMap.p2y]-=2;
                                    gameMap.p2y++;
                                    gameMap.p2x++;
                                    gameMap.Map[gameMap.p2x][gameMap.p2y]+=2;
                                    key++;
                                }
                                else{
                                    printf("wrong move! try again\n");
                                    continue;
                                }
                            }
                        }
                        else{
                            int validmove[] = {0,0};
                            if((gameMap.Map[gameMap.p2x][gameMap.p2y + 1] / 10) % 10 == 1)
                                validmove[0]=1;
                            if((gameMap.Map[gameMap.p2x + 1][gameMap.p2y + 1] / 10) % 10 == 1)
                                validmove[1]=1;
                            if(validmove[0] && validmove[1]){
                                printf("You can choose between going 1.forward-up(By push up arrow) or 2.forward-down(by push down arrow) : ");
                                    do{
                                int where = getTheDirect();
                                        if(where == 3){
                                        gameMap.Map[gameMap.p2x][gameMap.p2y]-=2;
                                        gameMap.p2y++;
                                        gameMap.p2x--;
                                        gameMap.Map[gameMap.p2x][gameMap.p2y]+=2;
                                        key++;
                                        break;
                                    }
                                    else if(where == 4){
                                        gameMap.Map[gameMap.p2x][gameMap.p2y]-=2;
                                        gameMap.p2y++;
                                        gameMap.p2x++;
                                        gameMap.Map[gameMap.p2x][gameMap.p2y]+=2;
                                        key++;
                                        break;
                                    }
                                    else{
                                        printf("wrong move! try again\n");
                                        continue;
                                    }
                                }while(1);
                            }
                            else if(validmove[0]==1){
                                gameMap.Map[gameMap.p2x][gameMap.p2y]-=2;
                                gameMap.p2y++;
                                gameMap.p2x--;
                                gameMap.Map[gameMap.p2x][gameMap.p2y]+=2;
                                key++;
                            }
                            else if(validmove[1]==1){
                                gameMap.Map[gameMap.p2x][gameMap.p2y]-=2;
                                gameMap.p2y++;
                                gameMap.p2x++;
                                gameMap.Map[gameMap.p2x][gameMap.p2y]+=2;
                                key++;
                            }
                            else{
                                printf("wrong move! try again\n");
                                continue;
                            }
                        }
                    }
				}
				else {
					printf("wrong move! try again\n");
					continue;
				}
			}
			else if (move == 3) {
				if (gameMap.p2x > 0 && (gameMap.Map[gameMap.p2x][gameMap.p2y] / 10) % 10 == 1) {
					if(gameMap.Map[gameMap.p2x - 1][gameMap.p2y] % 10 == 0){
                        gameMap.Map[gameMap.p2x][gameMap.p2y]-=2;
                        gameMap.p2x--;
                        gameMap.Map[gameMap.p2x][gameMap.p2y]+=2;
                        key++;
                    }
					else if(gameMap.Map[gameMap.p2x - 1][gameMap.p2y] % 10 == 1){
                        if(gameMap.p2x > 1){
                            if((gameMap.Map[gameMap.p2x - 1][gameMap.p2y] / 10) % 10 == 1){
                                gameMap.Map[gameMap.p2x][gameMap.p2y]-=2;
                                gameMap.p2x-=2;
                                gameMap.Map[gameMap.p2x][gameMap.p2y]+=2;
                                key++;
                            }
                            else{
                                int validmove[] = {0,0};
                                if(gameMap.Map[gameMap.p2x - 1][gameMap.p2y] / 100 == 1)
                                    validmove[0]=1;
                                if(gameMap.Map[gameMap.p2x - 1][gameMap.p2y + 1] / 100 == 1)
                                    validmove[1]=1;
                                if(validmove[0] && validmove[1]){
                                    printf("You can choose between going 1.forward-right(By push right arrow) or 2.forward-left(by push left arrow) : ");
                                    do{
                                        int where = getTheDirect();
                                        if(where == 1){
                                            gameMap.Map[gameMap.p2x][gameMap.p2y]-=2;
                                            gameMap.p2y--;
                                            gameMap.p2x--;
                                            gameMap.Map[gameMap.p2x][gameMap.p2y]+=2;
                                            key++;
                                            break;
                                        }
                                        else if(where == 2){
                                            gameMap.Map[gameMap.p2x][gameMap.p2y]-=2;
                                            gameMap.p2y++;
                                            gameMap.p2x--;
                                            gameMap.Map[gameMap.p2x][gameMap.p2y]+=2;
                                            key++;
                                            break;
                                        }
                                        else{
                                            printf("wrong move! try again\n");
                                            continue;
                                        }
                                    }while(1);
                                }
                                else if(validmove[0]==1){
                                    gameMap.Map[gameMap.p2x][gameMap.p2y]-=2;
                                    gameMap.p2y--;
                                    gameMap.p2x--;
                                    gameMap.Map[gameMap.p2x][gameMap.p2y]+=2;
                                    key++;
                                }
                                else if(validmove[1]==1){
                                    gameMap.Map[gameMap.p2x][gameMap.p2y]-=2;
                                    gameMap.p2y++;
                                    gameMap.p2x--;
                                    gameMap.Map[gameMap.p2x][gameMap.p2y]+=2;
                                    key++;
                                }
                                else{
                                    printf("wrong move! try again\n");
                                    continue;
                                }
                            }
                        }
                        else{
                            int validmove[] = {0,0};
                            if(gameMap.Map[gameMap.p2x - 1][gameMap.p2y] / 100 == 1)
                                validmove[0]=1;
                            if(gameMap.Map[gameMap.p2x - 1][gameMap.p2y + 1] / 100 == 1)
                                validmove[1]=1;
                            if(validmove[0] && validmove[1]){
                                printf("You can choose between going 1.forward-right(By push up arrow) or 2.forward-left(by push down arrow) : ");
                                do{
                                    int where = getTheDirect();
                                    if(where == 1){
                                        gameMap.Map[gameMap.p2x][gameMap.p2y]-=2;
                                        gameMap.p2y--;
                                        gameMap.p2x--;
                                        gameMap.Map[gameMap.p2x][gameMap.p2y]+=2;
                                        key++;
                                        break;
                                    }
                                    else if(where == 2){
                                        gameMap.Map[gameMap.p2x][gameMap.p2y]-=2;
                                        gameMap.p2y++;
                                        gameMap.p2x--;
                                        gameMap.Map[gameMap.p2x][gameMap.p2y]+=2;
                                        key++;
                                        break;
                                    }
                                    else{
                                        printf("wrong move! try again\n");
                                        continue;
                                    }
                                }while(1);
                            }
                            else if(validmove[0]==1){
                                gameMap.Map[gameMap.p2x][gameMap.p2y]-=2;
                                gameMap.p2y--;
                                gameMap.p2x--;
                                gameMap.Map[gameMap.p2x][gameMap.p2y]+=2;
                                key++;
                            }
                            else if(validmove[1]==1){
                                gameMap.Map[gameMap.p2x][gameMap.p2y]-=2;
                                gameMap.p2y++;
                                gameMap.p2x--;
                                gameMap.Map[gameMap.p2x][gameMap.p2y]+=2;
                                key++;
                            }
                            else{
                                printf("wrong move! try again\n");
                                continue;
                            }
                        }
					}
				}
				else {
					printf("wrong move! try again\n");
					continue;
				}
			}
			else if (move == 4) {
				if (gameMap.p2x < gameMap.n - 1 && (gameMap.Map[gameMap.p2x + 1][gameMap.p2y] / 10) % 10 == 1) {
					if(gameMap.Map[gameMap.p2x + 1][gameMap.p2y] % 10 == 0){
                        gameMap.Map[gameMap.p2x][gameMap.p2y]-=2;
                        gameMap.p2x++;
                        gameMap.Map[gameMap.p2x][gameMap.p2y]+=2;
                        key++;
                    }
					else if(gameMap.Map[gameMap.p2x + 1][gameMap.p2y] % 10 == 1){
                        if(gameMap.p2x < gameMap.n - 2){
                            if((gameMap.Map[gameMap.p2x + 2][gameMap.p2y] / 10) % 10 == 1){
                                gameMap.Map[gameMap.p2x][gameMap.p2y]-=2;
                                gameMap.p2x+=2;
                                gameMap.Map[gameMap.p2x][gameMap.p2y]+=2;
                                key++;
                            }
                            else{
                                int validmove[] = {0,0};
                                if(gameMap.Map[gameMap.p2x + 1][gameMap.p2y] / 100 == 1)
                                    validmove[0]=1;
                                if(gameMap.Map[gameMap.p2x + 1][gameMap.p2y + 1] / 100 == 1)
                                    validmove[1]=1;
                                if(validmove[0] && validmove[1]){
                                    printf("You can choose between going 1.forward-right(By push right arrow) or 2.forward-left(by push left arrow) : ");
                                    do{
                                        int where = getTheDirect();
                                        if(where == 1){
                                            gameMap.Map[gameMap.p2x][gameMap.p2y]-=2;
                                            gameMap.p2y--;
                                            gameMap.p2x++;
                                            gameMap.Map[gameMap.p2x][gameMap.p2y]+=2;
                                            key++;
                                            break;
                                        }
                                        else if(where == 2){
                                            gameMap.Map[gameMap.p2x][gameMap.p2y]-=2;
                                            gameMap.p2y++;
                                            gameMap.p2x++;
                                            gameMap.Map[gameMap.p2x][gameMap.p2y]+=2;
                                            key++;
                                            break;
                                        }
                                        else{
                                            printf("wrong move! try again\n");
                                            continue;
                                        }
                                    }while(1);
                                }
                                else if(validmove[0]==1){
                                    gameMap.Map[gameMap.p2x][gameMap.p2y]-=2;
                                    gameMap.p2y--;
                                    gameMap.p2x++;
                                    gameMap.Map[gameMap.p2x][gameMap.p2y]+=2;
                                    key++;
                                }
                                else if(validmove[1]==1){
                                    gameMap.Map[gameMap.p2x][gameMap.p2y]-=2;
                                    gameMap.p2y++;
                                    gameMap.p2x++;
                                    gameMap.Map[gameMap.p2x][gameMap.p2y]+=2;
                                    key++;
                                }
                                else{
                                    printf("wrong move! try again\n");
                                    continue;
                                }
                            }
                        }
                        else{
                            int validmove[] = {0,0};
                            if(gameMap.Map[gameMap.p2x + 1][gameMap.p2y] / 100 == 1)
                                validmove[0]=1;
                            if(gameMap.Map[gameMap.p2x + 1][gameMap.p2y + 1] / 100 == 1)
                                validmove[1]=1;
                            if(validmove[0] && validmove[1]){
                                printf("You can choose between going 1.forward-right(By push up arrow) or 2.forward-left(by push down arrow) : ");
                                do{
                                    int where = getTheDirect();
                                    if(where == 1){
                                        gameMap.Map[gameMap.p2x][gameMap.p2y]-=2;
                                        gameMap.p2y--;
                                        gameMap.p2x++;
                                        gameMap.Map[gameMap.p2x][gameMap.p2y]+=2;
                                        key++;
                                        break;
                                    }
                                    else if(where == 2){
                                        gameMap.Map[gameMap.p2x][gameMap.p2y]-=2;
                                        gameMap.p2y++;
                                        gameMap.p2x++;
                                        gameMap.Map[gameMap.p2x][gameMap.p2y]+=2;
                                        key++;
                                        break;
                                    }
                                    else{
                                        printf("wrong move! try again\n");
                                        continue;
                                    }
                                }while(1);
                            }
                            else if(validmove[0]==1){
                                gameMap.Map[gameMap.p2x][gameMap.p2y]-=2;
                                gameMap.p2y--;
                                gameMap.p2x++;
                                gameMap.Map[gameMap.p2x][gameMap.p2y]+=2;
                                key++;
                            }
                            else if(validmove[1]==1){
                                gameMap.Map[gameMap.p2x][gameMap.p2y]-=2;
                                gameMap.p2y++;
                                gameMap.p2x++;
                                gameMap.Map[gameMap.p2x][gameMap.p2y]+=2;
                                key++;
                            }
                            else{
                                printf("wrong move! try again\n");
                                continue;
                            }
                        }
					}
				}
				else {
					printf("wrong move! try again\n");
					continue;
				}
			}
			else if(move == 6){
				if (gameMap.p2w == 0) {
					printf("you can't place anymore walls!");
					continue;
				}
                printf("enter the wall starting location and (H) for horizontal and (V) for vertical\n");
                int x,y;
                char c;
                while(1){
                    scanf("%d %d %c",&x,&y,&c);
                    if ((c == 'h' || c == 'H' ) && (x >= gameMap.n || y >= gameMap.m - 1)) {
                        printf("Error! you have entered invalid location(you can not place a wall there)! try again: \n");
                        continue;
                    }
                    else if ((c == 'v' || c == 'V') && (x >= gameMap.n - 1 || y >= gameMap.m)) {
                        printf("Error! you have entered invalid location(you can not place a wall there)! try again: \n");
                        continue;
                    }
                    else if(validWallH(c, x, y, gameMap, &mygraph)){
                        gameMap.player2_walls[gameMap.p2placedwalls][0] = x;
                        gameMap.player2_walls[gameMap.p2placedwalls][1] = y;
                        gameMap.player2_walls[gameMap.p2placedwalls][2] = 0;
                        gameMap.Map[x][y] += 10;
                        gameMap.Map[x][y + 1] += 10;
                        key++;
                        gameMap.p2w--;
                        gameMap.p2placedwalls++;
                        break;
                    }
                    else if(validWallV(c, x, y, gameMap, &mygraph)){
                        gameMap.player2_walls[gameMap.p2placedwalls][0] = x;
                        gameMap.player2_walls[gameMap.p2placedwalls][1] = y;
                        gameMap.player2_walls[gameMap.p2placedwalls][2] = 1;
                        gameMap.Map[x][y] += 100;
                        gameMap.Map[x + 1][y] += 100;
                        key++;
                        gameMap.p2placedwalls++;
                        gameMap.p2w--;
                        break;
                    }
                    else{
                        printf("Error! you have entered invalid character or wrong location(you can not place a wall there)! try again: \n");
                        continue;
                    }
                }
            }
			else {
				printf("you have entered invalid character! try again!\n");
				continue;
			}
		}
	} while(1);
	}
	else if (s == 'c' || s == 'C') {
    int pre = 4;
	do {
        max_walls_checker(&gameMap);
    	PrintTheMap(gameMap);
    	if (gameMap.p1x == gameMap.n - 1) {
            setTextColor(2,0);
			printf("human is the winner!");
			setTextColor(15,0);
			break;
		}
		else if(gameMap.p2x == 0) {
            setTextColor(14,0);
			printf("computer is the winner!");
            setTextColor(15,0);
			break;
		}
    	if (!(key % 2)) {
            if (lock1 > 0) {
                lock1--;
                key++;
               	continue;
            }
    		printf("player 1's turn(0 to exit , W for placing wall , arrows to move): \n");
    		int randomNumber = random_maker(0, 1);
            if (randomNumber) {
                printf("you have gotten a present:\n");
                makePresent(&gameMap, key);
                printf("\n");
            }
            else {
            	printf("you have been spelled!\n");
                int situation = makeSpell(&gameMap, &mygraph, key);
                if (situation == 1) {
                    key++;
                    sleep(DELAY);
                    continue;
                }
                else if (situation == 2) {
                    key++;
                    lock1 = 1;
                    sleep(DELAY);
                    continue;
                }
			}
            move = getTheDirect();
    		if (!move){
    			printf("wrong input!\n");
    			continue;
			}
			else if (move == 5) break;
    		if (move == 1) {
    			if (gameMap.p1y > 0 && gameMap.Map[gameMap.p1x][gameMap.p1y] / 100 == 1) {
                    if(gameMap.Map[gameMap.p1x][gameMap.p1y - 1] % 10 == 0){
                        gameMap.Map[gameMap.p1x][gameMap.p1y]--;
                        gameMap.p1y--;
                        gameMap.Map[gameMap.p1x][gameMap.p1y]++;
                        key++;
                    }
                    else if(gameMap.Map[gameMap.p1x][gameMap.p1y - 1] % 10 == 2){
                        if(gameMap.p1y > 1){
                            if(gameMap.Map[gameMap.p1x][gameMap.p1y - 1] / 100 == 1){
                                gameMap.Map[gameMap.p1x][gameMap.p1y]--;
                                gameMap.p1y -= 2;
                                gameMap.Map[gameMap.p1x][gameMap.p1y]++;
                                key++;
                            }
                            else{
                                int validmove[] = {0,0};
                                if((gameMap.Map[gameMap.p1x][gameMap.p1y - 1] / 10) % 10 == 1)
                                    validmove[0]=1;
                                if((gameMap.Map[gameMap.p1x + 1][gameMap.p1y - 1] / 10) % 10 == 1)
                                    validmove[1]=1;
                                if(validmove[0] && validmove[1]){
                                    printf("You can choose between going 1.forward-up(By push up arrow) or 2.forward-down(by push down arrow) : ");
                                    do{
                                        int where = getTheDirect();
                                        if(where == 3){
                                            gameMap.Map[gameMap.p1x][gameMap.p1y]--;
                                            gameMap.p1y--;
                                            gameMap.p1x--;
                                            gameMap.Map[gameMap.p1x][gameMap.p1y]++;
                                            key++;
                                            break;
                                        }
                                        else if(where == 4){
                                            gameMap.Map[gameMap.p1x][gameMap.p1y]--;
                                            gameMap.p1y--;
                                            gameMap.p1x++;
                                            gameMap.Map[gameMap.p1x][gameMap.p1y]++;
                                            key++;
                                            break;
                                        }
                                        else{
                                            printf("wrong move! try again\n");
                                            continue;
                                        }
                                    }while(1);
                                }
                                else if(validmove[0]==1){
                                    gameMap.Map[gameMap.p1x][gameMap.p1y]--;
                                    gameMap.p1y--;
                                    gameMap.p1x--;
                                    gameMap.Map[gameMap.p1x][gameMap.p1y]++;
                                    key++;
                                }
                                else if(validmove[1]==1){
                                    gameMap.Map[gameMap.p1x][gameMap.p1y]--;
                                    gameMap.p1y--;
                                    gameMap.p1x++;
                                    gameMap.Map[gameMap.p1x][gameMap.p1y]++;
                                    key++;
                                }
                                else{
                                    printf("wrong move! try again\n");
                                    continue;
                                }
                            }
                        }
                        else{
                            int validmove[] = {0,0};
                            if((gameMap.Map[gameMap.p1x][gameMap.p1y - 1] / 10) % 10 == 1)
                                validmove[0]=1;
                            if((gameMap.Map[gameMap.p1x + 1][gameMap.p1y - 1] / 10) % 10 == 1)
                                validmove[1]=1;
                            if(validmove[0] && validmove[1]){
                                printf("You can choose between going 1.forward-up(By push up arrow) or 2.forward-down(by push down arrow) : ");
                                do{
                                    int where = getTheDirect();
                                    if(where == 3){
                                        gameMap.Map[gameMap.p1x][gameMap.p1y]--;
                                        gameMap.p1y--;
                                        gameMap.p1x--;
                                        gameMap.Map[gameMap.p1x][gameMap.p1y]++;
                                        key++;
                                        break;
                                    }
                                    else if(where == 4){
                                        gameMap.Map[gameMap.p1x][gameMap.p1y]--;
                                        gameMap.p1y--;
                                        gameMap.p1x++;
                                        gameMap.Map[gameMap.p1x][gameMap.p1y]++;
                                        key++;
                                        break;
                                    }
                                    else{
                                        printf("wrong move! try again\n");
                                        continue;
                                    }
                                }while(1);
                            }
                            else if(validmove[0]==1){
                                gameMap.Map[gameMap.p1x][gameMap.p1y]--;
                                gameMap.p1y--;
                                gameMap.p1x--;
                                gameMap.Map[gameMap.p1x][gameMap.p1y]++;
                                key++;
                            }
                            else if(validmove[1]==1){
                                gameMap.Map[gameMap.p1x][gameMap.p1y]--;
                                gameMap.p1y--;
                                gameMap.p1x++;
                                gameMap.Map[gameMap.p1x][gameMap.p1y]++;
                                key++;
                            }
                            else{
                                printf("wrong move! try again\n");
                                continue;
                            }
                        }
                    }
				}
				else {
					printf("wrong move! try again\n");
					continue;
				}
			}
			else if (move == 2) {
				if (gameMap.p1y < gameMap.m - 1 && gameMap.Map[gameMap.p1x][gameMap.p1y + 1] / 100 == 1) {
                    if(gameMap.Map[gameMap.p1x][gameMap.p1y + 1] % 10 == 0){
                        gameMap.Map[gameMap.p1x][gameMap.p1y]--;
                        gameMap.p1y++;
                        gameMap.Map[gameMap.p1x][gameMap.p1y]++;
                        key++;
                    }
                    else if(gameMap.Map[gameMap.p1x][gameMap.p1y + 1] % 10 == 2){
                        if(gameMap.p1y < gameMap.m - 2){
                            if(gameMap.Map[gameMap.p1x][gameMap.p1y + 2] / 100 == 1){
                                gameMap.Map[gameMap.p1x][gameMap.p1y]--;
                                gameMap.p1y += 2;
                                gameMap.Map[gameMap.p1x][gameMap.p1y]++;
                                key++;
                            }
                            else{
                                int validmove[] = {0,0};
                                if((gameMap.Map[gameMap.p1x][gameMap.p1y + 1] / 10) % 10 == 1)
                                    validmove[0]=1;
                                if((gameMap.Map[gameMap.p1x + 1][gameMap.p1y + 1] / 10) % 10 == 1)
                                    validmove[1]=1;
                                if(validmove[0] && validmove[1]){
                                    printf("You can choose between going 1.forward-up(By push up arrow) or 2.forward-down(by push down arrow) : ");
                                    do{
                                        int where = getTheDirect();
                                        if(where == 3){
                                            gameMap.Map[gameMap.p1x][gameMap.p1y]--;
                                            gameMap.p1y++;
                                            gameMap.p1x--;
                                            gameMap.Map[gameMap.p1x][gameMap.p1y]++;
                                            key++;
                                            break;
                                        }
                                        else if(where == 4){
                                            gameMap.Map[gameMap.p1x][gameMap.p1y]--;
                                            gameMap.p1y++;
                                            gameMap.p1x++;
                                            gameMap.Map[gameMap.p1x][gameMap.p1y]++;
                                            key++;
                                            break;
                                        }
                                        else{
                                            printf("wrong move! try again\n");
                                            continue;
                                        }
                                    }while(1);
                                }
                                else if(validmove[0]==1){
                                    gameMap.Map[gameMap.p1x][gameMap.p1y]--;
                                    gameMap.p1y++;
                                    gameMap.p1x--;
                                    gameMap.Map[gameMap.p1x][gameMap.p1y]++;
                                    key++;
                                }
                                else if(validmove[1]==1){
                                    gameMap.Map[gameMap.p1x][gameMap.p1y]--;
                                    gameMap.p1y++;
                                    gameMap.p1x++;
                                    gameMap.Map[gameMap.p1x][gameMap.p1y]++;
                                    key++;
                                }
                                else{
                                    printf("wrong move! try again\n");
                                    continue;
                                }
                            }
                        }
                        else{
                            int validmove[] = {0,0};
                            if((gameMap.Map[gameMap.p1x][gameMap.p1y + 1] / 10) % 10 == 1)
                                validmove[0]=1;
                            if((gameMap.Map[gameMap.p1x + 1][gameMap.p1y + 1] / 10) % 10 == 1)
                                validmove[1]=1;
                            if(validmove[0] && validmove[1]){
                                printf("You can choose between going 1.forward-up(By push up arrow) or 2.forward-down(by push down arrow) : ");
                                    do{
                                int where = getTheDirect();
                                        if(where == 3){
                                        gameMap.Map[gameMap.p1x][gameMap.p1y]--;
                                        gameMap.p1y++;
                                        gameMap.p1x--;
                                        gameMap.Map[gameMap.p1x][gameMap.p1y]++;
                                        key++;
                                        break;
                                    }
                                    else if(where == 4){
                                        gameMap.Map[gameMap.p1x][gameMap.p1y]--;
                                        gameMap.p1y++;
                                        gameMap.p1x++;
                                        gameMap.Map[gameMap.p1x][gameMap.p1y]++;
                                        key++;
                                        break;
                                    }
                                    else{
                                        printf("wrong move! try again\n");
                                        continue;
                                    }
                                }while(1);
                            }
                            else if(validmove[0]==1){
                                gameMap.Map[gameMap.p1x][gameMap.p1y]--;
                                gameMap.p1y++;
                                gameMap.p1x--;
                                gameMap.Map[gameMap.p1x][gameMap.p1y]++;
                                key++;
                            }
                            else if(validmove[1]==1){
                                gameMap.Map[gameMap.p1x][gameMap.p1y]--;
                                gameMap.p1y++;
                                gameMap.p1x++;
                                gameMap.Map[gameMap.p1x][gameMap.p1y]++;
                                key++;
                            }
                            else{
                                printf("wrong move! try again\n");
                                continue;
                            }
                        }
                    }
				}
				else {
					printf("wrong move! try again\n");
					continue;
				}
			}
			else if (move == 3) {
				if (gameMap.p1x > 0 && (gameMap.Map[gameMap.p1x][gameMap.p1y] / 10) % 10 == 1) {
					if(gameMap.Map[gameMap.p1x - 1][gameMap.p1y] % 10 == 0){
                        gameMap.Map[gameMap.p1x][gameMap.p1y]--;
                        gameMap.p1x--;
                        gameMap.Map[gameMap.p1x][gameMap.p1y]++;
                        key++;
                    }
					else if(gameMap.Map[gameMap.p1x - 1][gameMap.p1y] % 10 == 2){
                        if(gameMap.p1x > 1){
                            if((gameMap.Map[gameMap.p1x - 1][gameMap.p1y] / 10) % 10 == 1){
                                gameMap.Map[gameMap.p1x][gameMap.p1y]--;
                                gameMap.p1x-=2;
                                gameMap.Map[gameMap.p1x][gameMap.p1y]++;
                                key++;
                            }
                            else{
                                int validmove[] = {0,0};
                                if(gameMap.Map[gameMap.p1x - 1][gameMap.p1y] / 100 == 1)
                                    validmove[0]=1;
                                if(gameMap.Map[gameMap.p1x - 1][gameMap.p1y + 1] / 100 == 1)
                                    validmove[1]=1;
                                if(validmove[0] && validmove[1]){
                                    printf("You can choose between going 1.forward-right(By push right arrow) or 2.forward-left(by push left arrow) : ");
                                    do{
                                        int where = getTheDirect();
                                        if(where == 1){
                                            gameMap.Map[gameMap.p1x][gameMap.p1y]--;
                                            gameMap.p1y--;
                                            gameMap.p1x--;
                                            gameMap.Map[gameMap.p1x][gameMap.p1y]++;
                                            key++;
                                            break;
                                        }
                                        else if(where == 2){
                                            gameMap.Map[gameMap.p1x][gameMap.p1y]--;
                                            gameMap.p1y++;
                                            gameMap.p1x--;
                                            gameMap.Map[gameMap.p1x][gameMap.p1y]++;
                                            key++;
                                            break;
                                        }
                                        else{
                                            printf("wrong move! try again\n");
                                            continue;
                                        }
                                    }while(1);
                                }
                                else if(validmove[0]==1){
                                    gameMap.Map[gameMap.p1x][gameMap.p1y]--;
                                    gameMap.p1y--;
                                    gameMap.p1x--;
                                    gameMap.Map[gameMap.p1x][gameMap.p1y]++;
                                    key++;
                                }
                                else if(validmove[1]==1){
                                    gameMap.Map[gameMap.p1x][gameMap.p1y]--;
                                    gameMap.p1y++;
                                    gameMap.p1x--;
                                    gameMap.Map[gameMap.p1x][gameMap.p1y]++;
                                    key++;
                                }
                                else{
                                    printf("wrong move! try again\n");
                                    continue;
                                }
                            }
                        }
                        else{
                            int validmove[] = {0,0};
                            if(gameMap.Map[gameMap.p1x - 1][gameMap.p1y] / 100 == 1)
                                validmove[0]=1;
                            if(gameMap.Map[gameMap.p1x - 1][gameMap.p1y + 1] / 100 == 1)
                                validmove[1]=1;
                            if(validmove[0] && validmove[1]){
                                printf("You can choose between going 1.forward-right(By push up arrow) or 2.forward-left(by push down arrow) : ");
                                do{
                                    int where = getTheDirect();
                                    if(where == 1){
                                        gameMap.Map[gameMap.p1x][gameMap.p1y]--;
                                        gameMap.p1y--;
                                        gameMap.p1x--;
                                        gameMap.Map[gameMap.p1x][gameMap.p1y]++;
                                        key++;
                                        break;
                                    }
                                    else if(where == 2){
                                        gameMap.Map[gameMap.p1x][gameMap.p1y]--;
                                        gameMap.p1y++;
                                        gameMap.p1x--;
                                        gameMap.Map[gameMap.p1x][gameMap.p1y]++;
                                        key++;
                                        break;
                                    }
                                    else{
                                        printf("wrong move! try again\n");
                                        continue;
                                    }
                                }while(1);
                            }
                            else if(validmove[0]==1){
                                gameMap.Map[gameMap.p1x][gameMap.p1y]--;
                                gameMap.p1y--;
                                gameMap.p1x--;
                                gameMap.Map[gameMap.p1x][gameMap.p1y]++;
                                key++;
                            }
                            else if(validmove[1]==1){
                                gameMap.Map[gameMap.p1x][gameMap.p1y]--;
                                gameMap.p1y++;
                                gameMap.p1x--;
                                gameMap.Map[gameMap.p1x][gameMap.p1y]++;
                                key++;
                            }
                            else{
                                printf("wrong move! try again\n");
                                continue;
                            }
                        }
					}
				}
				else {
					printf("wrong move! try again\n");
					continue;
				}
			}
			else if (move == 4) {
				if (gameMap.p1x < gameMap.n - 1 && (gameMap.Map[gameMap.p1x + 1][gameMap.p1y] / 10) % 10 == 1) {
					if(gameMap.Map[gameMap.p1x + 1][gameMap.p1y] % 10 == 0){
                        gameMap.Map[gameMap.p1x][gameMap.p1y]--;
                        gameMap.p1x++;
                        gameMap.Map[gameMap.p1x][gameMap.p1y]++;
                        key++;
                    }
					else if(gameMap.Map[gameMap.p1x + 1][gameMap.p1y] % 10 == 2){
                        if(gameMap.p1x < gameMap.n - 2){
                            if((gameMap.Map[gameMap.p1x + 2][gameMap.p1y] / 10) % 10 == 1){
                                gameMap.Map[gameMap.p1x][gameMap.p1y]--;
                                gameMap.p1x+=2;
                                gameMap.Map[gameMap.p1x][gameMap.p1y]++;
                                key++;
                            }
                            else{
                                int validmove[] = {0,0};
                                if(gameMap.Map[gameMap.p1x + 1][gameMap.p1y] / 100 == 1)
                                    validmove[0]=1;
                                if(gameMap.Map[gameMap.p1x + 1][gameMap.p1y + 1] / 100 == 1)
                                    validmove[1]=1;
                                if(validmove[0] && validmove[1]){
                                    printf("You can choose between going 1.forward-right(By push right arrow) or 2.forward-left(by push left arrow) : ");
                                    do{
                                        int where = getTheDirect();
                                        if(where == 1){
                                            gameMap.Map[gameMap.p1x][gameMap.p1y]--;
                                            gameMap.p1y--;
                                            gameMap.p1x++;
                                            gameMap.Map[gameMap.p1x][gameMap.p1y]++;
                                            key++;
                                            break;
                                        }
                                        else if(where == 2){
                                            gameMap.Map[gameMap.p1x][gameMap.p1y]--;
                                            gameMap.p1y++;
                                            gameMap.p1x++;
                                            gameMap.Map[gameMap.p1x][gameMap.p1y]++;
                                            key++;
                                            break;
                                        }
                                        else{
                                            printf("wrong move! try again\n");
                                            continue;
                                        }
                                    }while(1);
                                }
                                else if(validmove[0]==1){
                                    gameMap.Map[gameMap.p1x][gameMap.p1y]--;
                                    gameMap.p1y--;
                                    gameMap.p1x++;
                                    gameMap.Map[gameMap.p1x][gameMap.p1y]++;
                                    key++;
                                }
                                else if(validmove[1]==1){
                                    gameMap.Map[gameMap.p1x][gameMap.p1y]--;
                                    gameMap.p1y++;
                                    gameMap.p1x++;
                                    gameMap.Map[gameMap.p1x][gameMap.p1y]++;
                                    key++;
                                }
                                else{
                                    printf("wrong move! try again\n");
                                    continue;
                                }
                            }
                        }
                        else{
                            int validmove[] = {0,0};
                            if(gameMap.Map[gameMap.p1x + 1][gameMap.p1y] / 100 == 1)
                                validmove[0]=1;
                            if(gameMap.Map[gameMap.p1x + 1][gameMap.p1y + 1] / 100 == 1)
                                validmove[1]=1;
                            if(validmove[0] && validmove[1]){
                                printf("You can choose between going 1.forward-right(By push up arrow) or 2.forward-left(by push down arrow) : ");
                                do{
                                    int where = getTheDirect();
                                    if(where == 1){
                                        gameMap.Map[gameMap.p1x][gameMap.p1y]--;
                                        gameMap.p1y--;
                                        gameMap.p1x++;
                                        gameMap.Map[gameMap.p1x][gameMap.p1y]++;
                                        key++;
                                        break;
                                    }
                                    else if(where == 2){
                                        gameMap.Map[gameMap.p1x][gameMap.p1y]--;
                                        gameMap.p1y++;
                                        gameMap.p1x++;
                                        gameMap.Map[gameMap.p1x][gameMap.p1y]++;
                                        key++;
                                        break;
                                    }
                                    else{
                                        printf("wrong move! try again\n");
                                        continue;
                                    }
                                }while(1);
                            }
                            else if(validmove[0]==1){
                                gameMap.Map[gameMap.p1x][gameMap.p1y]--;
                                gameMap.p1y--;
                                gameMap.p1x++;
                                gameMap.Map[gameMap.p1x][gameMap.p1y]++;
                                key++;
                            }
                            else if(validmove[1]==1){
                                gameMap.Map[gameMap.p1x][gameMap.p1y]--;
                                gameMap.p1y++;
                                gameMap.p1x++;
                                gameMap.Map[gameMap.p1x][gameMap.p1y]++;
                                key++;
                            }
                            else{
                                printf("wrong move! try again\n");
                                continue;
                            }
                        }
					}
				}
				else {
					printf("wrong move! try again\n");
					continue;
				}
			}
			else if(move == 6){
				if (gameMap.p1w == 0) {
					printf("you can't place anymore walls!");
					continue;
				}
                printf("enter the wall starting location and (H) for horizontal and (V) for vertical\n");
                int x,y;
                char c;
                while(1){
                    scanf("%d %d %c",&x,&y,&c);
                    if ((c == 'h' || c == 'H' ) && (x >= gameMap.n || y >= gameMap.m - 1)) {
                        printf("Error! you have entered invalid location(you can not place a wall there)! try again: \n");
                        continue;
                    }
                    else if ((c == 'v' || c == 'V') && (x >= gameMap.n - 1 || y >= gameMap.m)) {
                        printf("Error! you have entered invalid location(you can not place a wall there)! try again: \n");
                        continue;
                    }
                    else if(validWallH(c, x, y, gameMap, &mygraph)) {
                        gameMap.player1_walls[gameMap.p1placedwalls][0] = x;
                        gameMap.player1_walls[gameMap.p1placedwalls][1] = y;
                        gameMap.player1_walls[gameMap.p1placedwalls][2] = 0;
                        gameMap.Map[x][y] += 10;
                        gameMap.Map[x][y + 1] += 10;
                        key++;
                        gameMap.p1w--;
                        gameMap.p1placedwalls++;
                        break;
                    }
                    else if(validWallV(c, x, y, gameMap, &mygraph)){
                        gameMap.player1_walls[gameMap.p1placedwalls][0] = x;
                        gameMap.player1_walls[gameMap.p1placedwalls][1] = y;
                        gameMap.player1_walls[gameMap.p1placedwalls][2] = 1;
                        gameMap.Map[x][y] += 100;
                        gameMap.Map[x + 1][y] += 100;
                        key++;
                        gameMap.p1w--;
                        gameMap.p1placedwalls++;
                        break;
                    }
                    else{
                        printf("Error! you have entered invalid character(you can not place a wall there)! try again: \n");
                        continue;
                    }
                }
            }
			else {
				printf("you have entered invalid character! try again!\n");
				continue;
			}
		}
		else {
			printf("player 2's turn(0 to exit , W for placing wall , arrows to move): \n");
			printf("please wait ...\n");
			struct myWall bestWall;
			int dobest = 0;
			if(dif == 1){
                dobest = 0;
                move = rand();
                if (move % 7 == 5 || move % 7 == 0) {
                    move += 1;
                }
                move = move % 7;
			}
			else if(dif == 2){
                int g = rand();
                g = g%9;
                if(g<3){
                    dobest = 1;
                    move = minimax(maxDepth,&gameMap,&mygraph,INT_MIN,INT_MAX,&bestWall,pre);
                }
                else{
                    dobest = 0;
                    move = rand();
                    if (move % 7 == 5 || move % 7 == 0) {
                        move += 1;
                    }
                    move = move % 7;
                }
			}
			else if(dif == 3){
                int g = rand();
                g = g%9;
                if(g<5){
                    dobest = 1;
                    move = minimax(maxDepth,&gameMap,&mygraph,INT_MIN,INT_MAX,&bestWall,pre);
                }
                else{
                    dobest = 0;
                        move = rand();
                    if (move % 7 == 5 || move % 7 == 0) {
                        move += 1;
                    }
                    move = move % 7;
                }
			}
			else if(dif == 4){
                int g = rand();
                g = g%9;
                printf("%d\n",g);
                int r;
                scanf("%d",&r);
                if(g<7){
                    dobest = 1;
                    move = minimax(maxDepth,&gameMap,&mygraph,INT_MIN,INT_MAX,&bestWall,pre);
                }
                else{
                    dobest = 0;
                    move = rand();
                    if (move % 7 == 5 || move % 7 == 0) {
                        move += 1;
                    }
                    move = move % 7;
                }
			}
			else{
                dobest = 1;
                findWalls(&gameMap,&mygraph);
                move = minimax(maxDepth,&gameMap,&mygraph,INT_MIN,INT_MAX,&bestWall,pre);
                pre = move;
			}
    		if (move == 1) {
    			if (gameMap.p2y > 0 && gameMap.Map[gameMap.p2x][gameMap.p2y] / 100 == 1) {
                    if(gameMap.Map[gameMap.p2x][gameMap.p2y - 1] % 10 == 0){
                        gameMap.Map[gameMap.p2x][gameMap.p2y]-=2;
                        gameMap.p2y--;
                        gameMap.Map[gameMap.p2x][gameMap.p2y]+=2;
                        key++;
                    }
                    else if(gameMap.Map[gameMap.p2x][gameMap.p2y - 1] % 10 == 1){
                        if(gameMap.p2y > 1){
                            if(gameMap.Map[gameMap.p2x][gameMap.p2y - 1] / 100 == 1){
                                gameMap.Map[gameMap.p2x][gameMap.p2y]-=2;
                                gameMap.p2y -= 2;
                                gameMap.Map[gameMap.p2x][gameMap.p2y]+=2;
                                key++;
                            }
                            else{
                                int validmove[] = {0,0};
                                if((gameMap.Map[gameMap.p2x][gameMap.p2y - 1] / 10) % 10 == 1)
                                    validmove[0]=1;
                                if((gameMap.Map[gameMap.p2x + 1][gameMap.p2y - 1] / 10) % 10 == 1)
                                    validmove[1]=1;
                                if(validmove[0] && validmove[1]){
                                    do{
                                        int where = getTheDirect();
                                        if(where == 3){
                                            gameMap.Map[gameMap.p2x][gameMap.p2y]-=2;
                                            gameMap.p2y--;
                                            gameMap.p2x--;
                                            gameMap.Map[gameMap.p2x][gameMap.p2y]+=2;
                                            key++;
                                            break;
                                        }
                                        else if(where == 4){
                                            gameMap.Map[gameMap.p2x][gameMap.p2y]-=2;
                                            gameMap.p2y--;
                                            gameMap.p2x++;
                                            gameMap.Map[gameMap.p2x][gameMap.p2y]+=2;
                                            key++;
                                            break;
                                        }
                                        else{
                                            continue;
                                        }
                                    }while(1);
                                }
                                else if(validmove[0]==1){
                                    gameMap.Map[gameMap.p2x][gameMap.p2y]-=2;
                                    gameMap.p2y--;
                                    gameMap.p2x--;
                                    gameMap.Map[gameMap.p2x][gameMap.p2y]+=2;
                                    key++;
                                }
                                else if(validmove[1]==1){
                                    gameMap.Map[gameMap.p2x][gameMap.p2y]-=2;
                                    gameMap.p2y--;
                                    gameMap.p2x++;
                                    gameMap.Map[gameMap.p2x][gameMap.p2y]+=2;
                                    key++;
                                }
                                else{
                                    continue;
                                }
                            }
                        }
                        else{
                            int validmove[] = {0,0};
                            if((gameMap.Map[gameMap.p2x][gameMap.p2y - 1] / 10) % 10 == 1)
                                validmove[0]=1;
                            if((gameMap.Map[gameMap.p2x + 1][gameMap.p2y - 1] / 10) % 10 == 1)
                                validmove[1]=1;
                            if(validmove[0] && validmove[1]){
                                do{
                                    int where = getTheDirect();
                                    if(where == 3){
                                        gameMap.Map[gameMap.p2x][gameMap.p2y]-=2;
                                        gameMap.p2y--;
                                        gameMap.p2x--;
                                        gameMap.Map[gameMap.p2x][gameMap.p2y]+=2;
                                        key++;
                                        break;
                                    }
                                    else if(where == 4){
                                        gameMap.Map[gameMap.p2x][gameMap.p2y]-=2;
                                        gameMap.p2y--;
                                        gameMap.p2x++;
                                        gameMap.Map[gameMap.p2x][gameMap.p2y]+=2;
                                        key++;
                                        break;
                                    }
                                    else{
                                        continue;
                                    }
                                }while(1);
                            }
                            else if(validmove[0]==1){
                                gameMap.Map[gameMap.p2x][gameMap.p2y]-=2;
                                gameMap.p2y--;
                                gameMap.p2x--;
                                gameMap.Map[gameMap.p2x][gameMap.p2y]+=2;
                                key++;
                            }
                            else if(validmove[1]==1){
                                gameMap.Map[gameMap.p2x][gameMap.p2y]-=2;
                                gameMap.p2y--;
                                gameMap.p2x++;
                                gameMap.Map[gameMap.p2x][gameMap.p2y]+=2;
                                key++;
                            }
                            else{
                                continue;
                            }
                        }
                    }
				}
				else {
					continue;
				}
			}
			else if (move == 2) {
				if (gameMap.p2y < gameMap.m - 1 && gameMap.Map[gameMap.p2x][gameMap.p2y + 1] / 100 == 1) {
                    if(gameMap.Map[gameMap.p2x][gameMap.p2y + 1] % 10 == 0){
                        gameMap.Map[gameMap.p2x][gameMap.p2y]-=2;
                        gameMap.p2y++;
                        gameMap.Map[gameMap.p2x][gameMap.p2y]+=2;
                        key++;
                    }
                    else if(gameMap.Map[gameMap.p2x][gameMap.p2y + 1] % 10 == 1){
                        if(gameMap.p2y < gameMap.m - 2){
                            if(gameMap.Map[gameMap.p2x][gameMap.p2y + 2] / 100 == 1){
                                gameMap.Map[gameMap.p2x][gameMap.p2y]-=2;
                                gameMap.p2y += 2;
                                gameMap.Map[gameMap.p2x][gameMap.p2y]+=2;
                                key++;
                            }
                            else{
                                int validmove[] = {0,0};
                                if((gameMap.Map[gameMap.p2x][gameMap.p2y + 1] / 10) % 10 == 1)
                                    validmove[0]=1;
                                if((gameMap.Map[gameMap.p2x + 1][gameMap.p2y + 1] / 10) % 10 == 1)
                                    validmove[1]=1;
                                if(validmove[0] && validmove[1]){
                                    do{
                                        int where = getTheDirect();
                                        if(where == 3){
                                            gameMap.Map[gameMap.p2x][gameMap.p2y]-=2;
                                            gameMap.p2y++;
                                            gameMap.p2x--;
                                            gameMap.Map[gameMap.p2x][gameMap.p2y]+=2;
                                            key++;
                                            break;
                                        }
                                        else if(where == 4){
                                            gameMap.Map[gameMap.p2x][gameMap.p2y]-=2;
                                            gameMap.p2y++;
                                            gameMap.p2x++;
                                            gameMap.Map[gameMap.p2x][gameMap.p2y]+=2;
                                            key++;
                                            break;
                                        }
                                        else{
                                            continue;
                                        }
                                    }while(1);
                                }
                                else if(validmove[0]==1){
                                    gameMap.Map[gameMap.p2x][gameMap.p2y]-=2;
                                    gameMap.p2y++;
                                    gameMap.p2x--;
                                    gameMap.Map[gameMap.p2x][gameMap.p2y]+=2;
                                    key++;
                                }
                                else if(validmove[1]==1){
                                    gameMap.Map[gameMap.p2x][gameMap.p2y]-=2;
                                    gameMap.p2y++;
                                    gameMap.p2x++;
                                    gameMap.Map[gameMap.p2x][gameMap.p2y]+=2;
                                    key++;
                                }
                                else{
                                    continue;
                                }
                            }
                        }
                        else{
                            int validmove[] = {0,0};
                            if((gameMap.Map[gameMap.p2x][gameMap.p2y + 1] / 10) % 10 == 1)
                                validmove[0]=1;
                            if((gameMap.Map[gameMap.p2x + 1][gameMap.p2y + 1] / 10) % 10 == 1)
                                validmove[1]=1;
                            if(validmove[0] && validmove[1]){
                                    do{
                                int where = getTheDirect();
                                        if(where == 3){
                                        gameMap.Map[gameMap.p2x][gameMap.p2y]-=2;
                                        gameMap.p2y++;
                                        gameMap.p2x--;
                                        gameMap.Map[gameMap.p2x][gameMap.p2y]+=2;
                                        key++;
                                        break;
                                    }
                                    else if(where == 4){
                                        gameMap.Map[gameMap.p2x][gameMap.p2y]-=2;
                                        gameMap.p2y++;
                                        gameMap.p2x++;
                                        gameMap.Map[gameMap.p2x][gameMap.p2y]+=2;
                                        key++;
                                        break;
                                    }
                                    else{
                                        continue;
                                    }
                                }while(1);
                            }
                            else if(validmove[0]==1){
                                gameMap.Map[gameMap.p2x][gameMap.p2y]-=2;
                                gameMap.p2y++;
                                gameMap.p2x--;
                                gameMap.Map[gameMap.p2x][gameMap.p2y]+=2;
                                key++;
                            }
                            else if(validmove[1]==1){
                                gameMap.Map[gameMap.p2x][gameMap.p2y]-=2;
                                gameMap.p2y++;
                                gameMap.p2x++;
                                gameMap.Map[gameMap.p2x][gameMap.p2y]+=2;
                                key++;
                            }
                            else{
                                continue;
                            }
                        }
                    }
				}
				else {
					continue;
				}
			}
			else if (move == 3) {
				if (gameMap.p2x > 0 && (gameMap.Map[gameMap.p2x][gameMap.p2y] / 10) % 10 == 1) {
					if(gameMap.Map[gameMap.p2x - 1][gameMap.p2y] % 10 == 0){
                        gameMap.Map[gameMap.p2x][gameMap.p2y]-=2;
                        gameMap.p2x--;
                        gameMap.Map[gameMap.p2x][gameMap.p2y]+=2;
                        key++;
                    }
					else if(gameMap.Map[gameMap.p2x - 1][gameMap.p2y] % 10 == 1){
                        if(gameMap.p2x > 1){
                            if((gameMap.Map[gameMap.p2x - 1][gameMap.p2y] / 10) % 10 == 1){
                                gameMap.Map[gameMap.p2x][gameMap.p2y]-=2;
                                gameMap.p2x-=2;
                                gameMap.Map[gameMap.p2x][gameMap.p2y]+=2;
                                key++;
                            }
                            else{
                                int validmove[] = {0,0};
                                if(gameMap.Map[gameMap.p2x - 1][gameMap.p2y] / 100 == 1)
                                    validmove[0]=1;
                                if(gameMap.Map[gameMap.p2x - 1][gameMap.p2y + 1] / 100 == 1)
                                    validmove[1]=1;
                                if(validmove[0] && validmove[1]){
                                    do{
                                        int where = getTheDirect();
                                        if(where == 1){
                                            gameMap.Map[gameMap.p2x][gameMap.p2y]-=2;
                                            gameMap.p2y--;
                                            gameMap.p2x--;
                                            gameMap.Map[gameMap.p2x][gameMap.p2y]+=2;
                                            key++;
                                            break;
                                        }
                                        else if(where == 2){
                                            gameMap.Map[gameMap.p2x][gameMap.p2y]-=2;
                                            gameMap.p2y++;
                                            gameMap.p2x--;
                                            gameMap.Map[gameMap.p2x][gameMap.p2y]+=2;
                                            key++;
                                            break;
                                        }
                                        else{
                                            continue;
                                        }
                                    }while(1);
                                }
                                else if(validmove[0]==1){
                                    gameMap.Map[gameMap.p2x][gameMap.p2y]-=2;
                                    gameMap.p2y--;
                                    gameMap.p2x--;
                                    gameMap.Map[gameMap.p2x][gameMap.p2y]+=2;
                                    key++;
                                }
                                else if(validmove[1]==1){
                                    gameMap.Map[gameMap.p2x][gameMap.p2y]-=2;
                                    gameMap.p2y++;
                                    gameMap.p2x--;
                                    gameMap.Map[gameMap.p2x][gameMap.p2y]+=2;
                                    key++;
                                }
                                else{
                                    continue;
                                }
                            }
                        }
                        else{
                            int validmove[] = {0,0};
                            if(gameMap.Map[gameMap.p2x - 1][gameMap.p2y] / 100 == 1)
                                validmove[0]=1;
                            if(gameMap.Map[gameMap.p2x - 1][gameMap.p2y + 1] / 100 == 1)
                                validmove[1]=1;
                            if(validmove[0] && validmove[1]){
                                do{
                                    int where = getTheDirect();
                                    if(where == 1){
                                        gameMap.Map[gameMap.p2x][gameMap.p2y]-=2;
                                        gameMap.p2y--;
                                        gameMap.p2x--;
                                        gameMap.Map[gameMap.p2x][gameMap.p2y]+=2;
                                        key++;
                                        break;
                                    }
                                    else if(where == 2){
                                        gameMap.Map[gameMap.p2x][gameMap.p2y]-=2;
                                        gameMap.p2y++;
                                        gameMap.p2x--;
                                        gameMap.Map[gameMap.p2x][gameMap.p2y]+=2;
                                        key++;
                                        break;
                                    }
                                    else{
                                        continue;
                                    }
                                }while(1);
                            }
                            else if(validmove[0]==1){
                                gameMap.Map[gameMap.p2x][gameMap.p2y]-=2;
                                gameMap.p2y--;
                                gameMap.p2x--;
                                gameMap.Map[gameMap.p2x][gameMap.p2y]+=2;
                                key++;
                            }
                            else if(validmove[1]==1){
                                gameMap.Map[gameMap.p2x][gameMap.p2y]-=2;
                                gameMap.p2y++;
                                gameMap.p2x--;
                                gameMap.Map[gameMap.p2x][gameMap.p2y]+=2;
                                key++;
                            }
                            else{
                                continue;
                            }
                        }
					}
				}
				else {
					continue;
				}
			}
			else if (move == 4) {
				if (gameMap.p2x < gameMap.n - 1 && (gameMap.Map[gameMap.p2x + 1][gameMap.p2y] / 10) % 10 == 1) {
					if(gameMap.Map[gameMap.p2x + 1][gameMap.p2y] % 10 == 0){
                        gameMap.Map[gameMap.p2x][gameMap.p2y]-=2;
                        gameMap.p2x++;
                        gameMap.Map[gameMap.p2x][gameMap.p2y]+=2;
                        key++;
                    }
					else if(gameMap.Map[gameMap.p2x + 1][gameMap.p2y] % 10 == 1){
                        if(gameMap.p2x < gameMap.n - 2){
                            if((gameMap.Map[gameMap.p2x + 2][gameMap.p2y] / 10) % 10 == 1){
                                gameMap.Map[gameMap.p2x][gameMap.p2y]-=2;
                                gameMap.p2x+=2;
                                gameMap.Map[gameMap.p2x][gameMap.p2y]+=2;
                                key++;
                            }
                            else{
                                int validmove[] = {0,0};
                                if(gameMap.Map[gameMap.p2x + 1][gameMap.p2y] / 100 == 1)
                                    validmove[0]=1;
                                if(gameMap.Map[gameMap.p2x + 1][gameMap.p2y + 1] / 100 == 1)
                                    validmove[1]=1;
                                if(validmove[0] && validmove[1]){
                                    do{
                                        int where = getTheDirect();
                                        if(where == 1){
                                            gameMap.Map[gameMap.p2x][gameMap.p2y]-=2;
                                            gameMap.p2y--;
                                            gameMap.p2x++;
                                            gameMap.Map[gameMap.p2x][gameMap.p2y]+=2;
                                            key++;
                                            break;
                                        }
                                        else if(where == 2){
                                            gameMap.Map[gameMap.p2x][gameMap.p2y]-=2;
                                            gameMap.p2y++;
                                            gameMap.p2x++;
                                            gameMap.Map[gameMap.p2x][gameMap.p2y]+=2;
                                            key++;
                                            break;
                                        }
                                        else{
                                            continue;
                                        }
                                    }while(1);
                                }
                                else if(validmove[0]==1){
                                    gameMap.Map[gameMap.p2x][gameMap.p2y]-=2;
                                    gameMap.p2y--;
                                    gameMap.p2x++;
                                    gameMap.Map[gameMap.p2x][gameMap.p2y]+=2;
                                    key++;
                                }
                                else if(validmove[1]==1){
                                    gameMap.Map[gameMap.p2x][gameMap.p2y]-=2;
                                    gameMap.p2y++;
                                    gameMap.p2x++;
                                    gameMap.Map[gameMap.p2x][gameMap.p2y]+=2;
                                    key++;
                                }
                                else{
                                    continue;
                                }
                            }
                        }
                        else{
                            int validmove[] = {0,0};
                            if(gameMap.Map[gameMap.p2x + 1][gameMap.p2y] / 100 == 1)
                                validmove[0]=1;
                            if(gameMap.Map[gameMap.p2x + 1][gameMap.p2y + 1] / 100 == 1)
                                validmove[1]=1;
                            if(validmove[0] && validmove[1]){
                                do{
                                    int where = getTheDirect();
                                    if(where == 1){
                                        gameMap.Map[gameMap.p2x][gameMap.p2y]-=2;
                                        gameMap.p2y--;
                                        gameMap.p2x++;
                                        gameMap.Map[gameMap.p2x][gameMap.p2y]+=2;
                                        key++;
                                        break;
                                    }
                                    else if(where == 2){
                                        gameMap.Map[gameMap.p2x][gameMap.p2y]-=2;
                                        gameMap.p2y++;
                                        gameMap.p2x++;
                                        gameMap.Map[gameMap.p2x][gameMap.p2y]+=2;
                                        key++;
                                        break;
                                    }
                                    else{
                                        continue;
                                    }
                                }while(1);
                            }
                            else if(validmove[0]==1){
                                gameMap.Map[gameMap.p2x][gameMap.p2y]-=2;
                                gameMap.p2y--;
                                gameMap.p2x++;
                                gameMap.Map[gameMap.p2x][gameMap.p2y]+=2;
                                key++;
                            }
                            else if(validmove[1]==1){
                                gameMap.Map[gameMap.p2x][gameMap.p2y]-=2;
                                gameMap.p2y++;
                                gameMap.p2x++;
                                gameMap.Map[gameMap.p2x][gameMap.p2y]+=2;
                                key++;
                            }
                            else{
                                continue;
                            }
                        }
					}
				}
				else {
					continue;
				}
			}
			else if(move == 6){
                int x,y, z;
                char c;
                while(1){
                    x = rand() % (gameMap.n - 1);
                    y = rand() % (gameMap.m - 1);
                    z = rand() % 2;
                    if (z) c = 'h';
					else c = 'v';
                    if ((c == 'h') && (x >= gameMap.n || y >= gameMap.m - 1)) {
                        x--, y--;
                    }
                    else if ((c == 'v') && (x >= gameMap.n - 1 || y >= gameMap.m)) {
                        x--, y--;
                    }
                    if (gameMap.p2w == 0) {
					continue;
				}
                    else if(validWallH(c, x, y, gameMap, &mygraph)){
                        gameMap.player2_walls[10 - gameMap.p2w][0] = x;
                        gameMap.player2_walls[10 - gameMap.p2w][1] = y;
                        gameMap.player2_walls[10 - gameMap.p2w][2] = 0;
                        gameMap.Map[x][y] += 10;
                        gameMap.Map[x][y + 1] += 10;
                        key++;
                        gameMap.p2w--;
                        break;
                    }
                    else if(validWallV(c, x, y, gameMap, &mygraph)){
                        gameMap.player2_walls[10 - gameMap.p2w][0] = x;
                        gameMap.player2_walls[10 - gameMap.p2w][1] = y;
                        gameMap.player2_walls[10 - gameMap.p2w][2] = 0;
                        gameMap.Map[x][y] += 100;
                        gameMap.Map[x + 1][y] += 100;
                        key++;
                        gameMap.p2w--;
                        break;
                    }
                    else{
                        continue;
                    }
                }
            }
			else {
				continue;
			}
		}
	} while(1);
	}
    return 0;
}
