#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>

struct board{
    int n, m;
    int Map[100][100];
    int p1x, p1y, p2x, p2y;
    int p1w, p2w;
};

struct Graph{
    int visited[10000];
    int adjList[10000][4];
};

void setTextColor(int textColor, int backColor) {
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
    printf("This is the Map(O stands for player 1 and X stands for player 2): \n");
    setTextColor(11, 0);
    printf("    ");
    for(int i=0;i<m;i++){
        if(i/10 == 0)
            printf("  %d ",i);
        else
            printf(" %d ",i);
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
    scanf("%d", &n);
    m = n;
    struct board gameMap;
    struct Graph mygraph;
    gameMap.p1y = floor(n/2);
    gameMap.p2y = gameMap.p1y;
    gameMap.p1x= 0;
    gameMap.p2x = n-1;
    gameMap.n = n;
    gameMap.m = m;
    for (i = 0; i < n + 2; i++) {
        for (j = 0; j < m + 1; j++) {
            gameMap.Map[i][j] = 110;
            addedge(n,m,i,j,&mygraph);
        }
    }
    makeUnVisit(&mygraph);
    char s;
    int sw;
    do {
    	sw = 0;
    	printf("choose you opponent(c for computer and h for human): \n");
    	scanf("\n%c", &s);
    	if (!(s == 'c' || s == 'C' || s == 'H' || s == 'h')) {
    		printf("invalid input! try again: \n");
    		sw = 1;
		}
	} while (sw);
    gameMap.Map[gameMap.p1x][gameMap.p1y] = 111;
    gameMap.Map[gameMap.p2x][gameMap.p2y] = 112;
    PrintTheMap(gameMap);
    int key = 0, move;
    if (s == 'h' || s == 'H') {
    do {
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
    		printf("player 1's turn(0 to exit , W for placing wall , arrows to move): \n");
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
                        gameMap.Map[x][y] += 10;
                        gameMap.Map[x][y + 1] += 10;
                        key++;
                        break;
                    }
                    else if(validWallV(c, x, y, gameMap, &mygraph)){
                        gameMap.Map[x][y] += 100;
                        gameMap.Map[x + 1][y] += 100;
                        key++;
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
                        gameMap.Map[x][y] += 10;
                        gameMap.Map[x][y + 1] += 10;
                        key++;
                        break;
                    }
                    else if(validWallV(c, x, y, gameMap, &mygraph)){
                        gameMap.Map[x][y] += 100;
                        gameMap.Map[x + 1][y] += 100;
                        key++;
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

	srand(time(0));
	do {
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
    		printf("player 1's turn(0 to exit , W for placing wall , arrows to move): \n");
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
                        gameMap.Map[x][y] += 10;
                        gameMap.Map[x][y + 1] += 10;
                        key++;
                        break;
                    }
                    else if(validWallV(c, x, y, gameMap, &mygraph)){
                        gameMap.Map[x][y] += 100;
                        gameMap.Map[x + 1][y] += 100;
                        key++;
                        break;
                    }
                    else{
                        printf("Error! you have entered invalid character! try again: \n");
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
			srand(time(0));
			printf("computer's turn, please wait\n'");
    		move = rand();
    		if (move % 7 == 5 || move % 7 == 0) {
    			move += 1;
			}
			move = move % 7;
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
                    srand(time(0));
                    y = rand() % (gameMap.m - 1);
                    srand(time(0));
                    z = rand() % 2;
                    if (z) c = 'h';
					else c = 'v';
                    if ((c == 'h') && (x >= gameMap.n || y >= gameMap.m - 1)) {
                        //printf("Error! you have entered invalid location(you can not place a wall there)! try again: \n");
                        //continue;
                        x--, y--;
                    }
                    else if ((c == 'v') && (x >= gameMap.n - 1 || y >= gameMap.m)) {
                        //printf("Error! you have entered invalid location(you can not place a wall there)! try again: \n");
                        //continue;
                        x--, y--;
                    }
                    else if(validWallH(c, x, y, gameMap, &mygraph)){
                        gameMap.Map[x][y] += 10;
                        gameMap.Map[x][y + 1] += 10;
                        key++;
                        break;
                    }
                    else if(validWallV(c, x, y, gameMap, &mygraph)){
                        gameMap.Map[x][y] += 100;
                        gameMap.Map[x + 1][y] += 100;
                        key++;
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