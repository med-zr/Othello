#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<SDL.h>
#include<SDL_image.h>
#include<string.h>
#include<SDL_ttf.h>
#include<SDL_mixer.h>
#define widthwindow 800
#define heightwindow 700

typedef struct player{
	char nom[20];
	char logo;
	int score;
}joueur;
/*
la fonction ci-dessous permet d'encadrer les joueurs c'est a dire il limite les positions du deplacement ,exemple:
  1 2 3 4 5 6 7 8		evidement le joeur ne peut pas passer directement vers la position (2,1),mais les seuls 
1 . . . . . . . .		positions possible son ceux qui encadrent w et b.
2 . . . . . . . .		elle va donc remplacer tout ce qui est former par b et w et les positions qui l'encadrent par des 1.
3 . . . . . . . . 
4 . . . w b . . . 
5 . . . b w . . .
6 . . . . . . . .
7 . . . . . . . .
8 . . . . . . . . 
donc la fonction ci dessous va transformer ce tableau en:
0 0 0 0 0 0 0 0 0		
0 0 0 0 0 0 0 0 0 		
0 0 0 0 0 0 0 0 0 
0 0 0 1 1 1 1 0 0
0 0 0 1 1 1 1 0 0
0 0 0 1 1 1 1 0 0
0 0 0 1 1 1 1 0 0
0 0 0 0 0 0 0 0 0 
0 0 0 0 0 0 0 0 0
*/
int **encadrant(char **table){
	int **table1;
	table1=(int **)malloc(9*sizeof(int *));
	for(int i=0;i<=8;i++){
		table1[i]=(int *)malloc(9*sizeof(int));
		for(int j=0;j<=8;j++){
			*(*(table1+i)+j)=0;
		}
	}
	for(int i=0;i<=8;i++){
		for(int j=0;j<=8;j++){
			if(table[i][j]=='b'||table[i][j]=='w'){
				if(i-1>=1&&j-1>=1)	*(*(table1+i-1)+j-1)=1;
				if(i-1>=1)	*(*(table1+i-1)+j)=1;
				if(i-1>=1&&j+1<=8)	*(*(table1+i-1)+j+1)=1;
				if(j-1>=1)	*(*(table1+i)+j-1)=1;
				if(j+1<=8)	*(*(table1+i)+j+1)=1;
				if(i+1<=8&&j-1>=1)	*(*(table1+i+1)+j-1)=1;
				if(i+1<=8)	*(*(table1+i+1)+j)=1;
				if(i+1<=8&&j+1<=8)	*(*(table1+i+1)+j+1)=1;

			}
		}
	}
	return table1;		
}
/*maintenat on doit eliminer les positions impossibles a chaque joueur c'est a dire on ne doit preserver que les positions qui se situent sur les deux diagonales,la ligne
et la colonne cela est fait dans la fonction suivante en remplacant les 1 par des 2 en prenant l'exemple precedant et on choisissant de jouer avec 'b' on trouve:
0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 
0 0 0 0 0 0 0 0 0 
0 0 0 1 2 2 2 0 0
0 0 0 2 2 2 2 0 0
0 0 0 2 2 2 2 0 0
0 0 0 2 2 2 1 0 0
0 0 0 0 0 0 0 0 0 
0 0 0 0 0 0 0 0 0
*/
int **places_possible(char player,char **table,int **table1){
	for(int i=0;i<=8;i++){
		for(int j=0;j<=8;j++){
			if(*(*(table+i)+j)==player){
				for(int k=1;k<=8;k++){
					if(i+k<=8&&j-k>=1&&*(*(table1+i+k)+j-k)==1) *(*(table1+i+k)+j-k)=2;
					if(i-k>=1&&j+k<=8&&*(*(table1+i-k)+j+k)==1) *(*(table1+i-k)+j+k)=2;
					if(j+k<=8&&i+k<=8&&*(*(table1+i+k)+j+k)==1) *(*(table1+i+k)+j+k)=2;
					if(i-k>=1&&j-k>=1&&*(*(table1+i-k)+j-k)==1) *(*(table1+i-k)+j-k)=2;
					if(j+k<=8&&*(*(table1+i)+j+k)==1) *(*(table1+i)+j+k)=2;
					if(j-k>=1&&*(*(table1+i)+j-k)==1) *(*(table1+i)+j-k)=2;
					if(i+k<=8&&*(*(table1+i+k)+j)==1) *(*(table1+i+k)+j)=2;
					if(i-k>=1&&*(*(table1+i-k)+j)==1) *(*(table1+i-k)+j)=2;
				}
			}
		}
	}
	return table1;
}  
/*maintenat on doit eliminer les positions occupees par des w ou des b donc a la place des 2 on va mettre des 0,et c'est cela le role du fonction suivante,prenant 
l'exemple precedent, la fonction va transformer le tableau en:
0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 
0 0 0 0 0 0 0 0 0 
0 0 0 1 2 2 2 0 0
0 0 0 2 0 0 2 0 0
0 0 0 2 0 0 2 0 0
0 0 0 2 2 2 1 0 0
0 0 0 0 0 0 0 0 0 
0 0 0 0 0 0 0 0 0
*/
int **place_libre(int **table1,char **table){
	int **table2;
	table2=table1;
	for(int i=1;i<=8;i++){
		for(int j=1;j<=8;j++){
			if(*(*(table+i)+j)!='.') *(*(table2+i)+j)=0;
		}
	}
	return table2;	
}


//****************************************************************************************************************************************************
//la fonction suivante va donner l'adversaire du joueur c'est a dire si on donne w comme argument elle va retourner b et vis vers ca

char adversaire(char player){
	char a='w',c='b';
	if(player=='b') return a;
	if(player=='w') return c; 
}
//****************************************************************************************************************************************************************************
/* les fonctions suivantes vont chercher si une position peut etre accessible par le joueur c'est a dire elle va chercher dans les position qui precedent la position qui precede
encore la position desiree et si elle trouve le joueur donc elle retourne un 1 sinon un 0*/ 
int check_in_diag_sup(int i,int j,char player,char **table){
	// cette fonction cherche dans la diagonale mais en bas
	int s=0;
	for(int k=2;k<=8;k++){
		if(i+k<=8&&j+k<=8&&*(*(table+i+k)+j+k)==player){
		s=1;
		break;
		}
		else if(i+k<=8&&j+k<=8&&*(*(table+i+k)+j+k)=='.')	break;
	}
	return s;
}
int check_in_ligne_droite(int i,int j,char player,char **table){
	// cette fonction cherche dans la line a droite
	int s=0;
	for(int k=2;k<=8;k++){
		if(j+k<=8&&*(*(table+i)+j+k)==player){
			s=1;
			break;
		}
		else if(j+k<=8&&*(*(table+i)+j+k)=='.')	break;
	}
	return s;
}
int check_in_ligne_gauche(int i,int j,char player,char **table){
	//celle ci cherche dans la ligne a gauche
	int s=0;
	for(int k=2;k<=8;k++){
		if(j-k>=1&&*(*(table+i)+j-k)==player){
			s=1;
			break;
		}
		else if(j-k>=1&&*(*(table+i)+j-k)=='.') break;
	}
	return s;
}
int check_in_colonne_up(int i,int j,char player,char **table){
	//celle ci cherche dans la colonne en haut
	int s=0;
	for(int k=2;k<=8;k++){
		if(i-k>=1&&*(*(table+i-k)+j)==player){
			s=1;
			break;
		}
		else if(i-k>=1&&*(*(table+i-k)+j)=='.')	break;
	}
	return s;
}
int check_in_colonne_down(int i,int j,char player,char **table){
	//celle ci cherche dans la colonne en bas
	int s=0;
	for(int k=2;k<=8;k++){
		if(i+k<=8&&*(*(table+i+k)+j)==player){
			s=1;
			break;
		}
		else if(i+k<=8&&*(*(table+i+k)+j)=='.')	break;
	}
	return s;
}
int check_in_reversed_diag_gauche(int i,int j,char player,char **table){
	//celle ci cherche dans la deuxieme diagonale en bas
	int s=0;
	for(int k=2;k<=8;k++){
		if(i+k<=8&&j-k>=1&&*(*(table+i+k)+j-k)==player){
			s=1;
			break;
		}
		else if(i+k<=8&&j-k>=1&&*(*(table+i+k)+j-k)=='.')	break;
		
	}
	return s;
}
int check_in_reversed_diag_droite(int i,int j,char player,char **table){
	//celle ci cherche dans la deuxieme diagonale en haut
	int s=0;
	for(int k=2;k<=8;k++){
		if(i-k>=1&&j+k<=8&&*(*(table+i-k)+j+k)==player){
			s=1;
			break;
		}
		else if(i-k>=1&&j+k<=8&&*(*(table+i-k)+j+k)=='.')	break;
	}
	return s;
}
int check_in_diag_inf(int i,int j,char player,char **table){
	//cette fonction cherche dans la diagonale mais en haut
	int s=0;
	for(int k=2;k<=8;k++){
		if(i-k>=1&&j-k>=1&&*(*(table+i-k)+j-k)==player){
			s=1;
			break;
		}
		else if(i-k>=1&&j-k>=1&&*(*(table+i-k)+j-k)=='.')	break;
	}
	return s;
}
//**************************************************************************************************************************************************************************
/*la fonction suivante va permettre de donner les positions ou le joueur peut se deplacer c'est a dire les position ou son deplacement va permettre de consommer 
son adversaire en fait les conditon de la fonction permettent de verifier si une des positions qui encadrent la position desiree est occuper par son adversaire puis elle 
cherche si elle existe une  boule du joueur qui va consommer la boule de l'adversaire pour passer a la position desiree,si cela est verifier donc on donne a cette postion
la valeur 'o',en utilisant l'exemple precedent et en choisissant de jouer avec b la fonction suivante va nous rendre le tableau:
  1 2 3 4 5 6 7 8		 
1 . . . . . . . .		
2 . . . . . . . .		
3 . . . o . . . . 
4 . . o w b . . . 
5 . . . b w o . .
6 . . . . o . . .
7 . . . . . . . .
8 . . . . . . . .
*/
char **deplacement(char **table, char player){
	char **table4;
	table4=(char **)malloc(9*sizeof(char *));
	for(int i=0;i<=8;i++){
		table4[i]=(char *)malloc(9*sizeof(char));
		for(int j=0;j<=8;j++){
			*(*(table4+i)+j)=*(*(table+i)+j);
		}
	}		
	
	int **table_position;
	table_position=places_possible(player,table,place_libre(encadrant(table),table));
		for(int i=1;i<=8;i++){
			for(int j=1;j<=8;j++){
				if(*(*(table_position+i)+j)==2){
					if(i-1>=1&&j-1>=1&&*(*(table+i-1)+j-1)==adversaire(player)&&check_in_diag_inf(i,j,player,table)!=0) *(*(table4+i)+j)='o';
					if(i+1<=8&&j+1<=8&&*(*(table+i+1)+j+1)==adversaire(player)&&check_in_diag_sup(i,j,player,table)!=0)	*(*(table4+i)+j)='o';
					if(j+1<=8&&*(*(table+i)+j+1)==adversaire(player)&&check_in_ligne_droite(i,j,player,table)!=0)	*(*(table4+i)+j)='o';
					if(j-1>=1&&*(*(table+i)+j-1)==adversaire(player)&&check_in_ligne_gauche(i,j,player,table)!=0)	*(*(table4+i)+j)='o';
					if(i+1<=8&&*(*(table+i+1)+j)==adversaire(player)&&check_in_colonne_down(i,j,player,table)!=0)	*(*(table4+i)+j)='o';
					if(i-1>=1&&*(*(table+i-1)+j)==adversaire(player)&&check_in_colonne_up(i,j,player,table)!=0)	*(*(table4+i)+j)='o';
					if(i-1>=1&&j+1<=8&&*(*(table+i-1)+j+1)==adversaire(player)&&check_in_reversed_diag_droite(i,j,player,table)!=0)	*(*(table4+i)+j)='o';
					if(i+1<=8&&j-1>=1&&*(*(table+i+1)+j-1)==adversaire(player)&&check_in_reversed_diag_gauche(i,j,player,table)!=0)	*(*(table4+i)+j)='o';
				}
			}
		}
		return table4;
}
//****************************************************************************************************************************************************************************
/*les fonctionts suivantes vont nous permettre de calculer le nombre de boules qu'un joueur doit consommer dans son mouvement,elles vont donc calculer
 le nombre des boules adversaires qui existent avant la premiere apparence d'une boule d'un joueur */
int check1_in_diag_sup(int i,int j,char player,char **table){
	int s=0;
	for(int k=1;k<=8;k++){
		if(i+k<=8&&j+k<=8&&*(*(table+i+k)+j+k)==adversaire(player)){
		s++;
		}
		else break;
	}
	return s;
}
int check1_in_ligne_droite(int i,int j,char player,char **table){
	
	int s=0;
	for(int k=1;k<=8;k++){
		if(j+k<=8&&*(*(table+i)+j+k)==adversaire(player)){
			s++;
		}
		else break;
	}
	return s;
}
int check1_in_ligne_gauche(int i,int j,char player,char **table){
	
	int s=0;
	for(int k=1;k<=8;k++){
		if(j-k>=1&&*(*(table+i)+j-k)==adversaire(player)){
			s++;
		}
		else break;
	}
	return s;
}
int check1_in_colonne_up(int i,int j,char player,char **table){
	int s=0;
	for(int k=1;k<=8;k++){
		if(i-k>=1&&*(*(table+i-k)+j)==adversaire(player)){
			s++;
		}
		else break;
	}
	return s;
}
int check1_in_colonne_down(int i,int j,char player,char **table){
	int s=0;
	for(int k=1;k<=8;k++){
		if(i+k<=8&&*(*(table+i+k)+j)==adversaire(player)){
			s++;
		}
		else break;
	}
	return s;
}
int check1_in_reversed_diag_gauche(int i,int j,char player,char **table){
	int s=0;
	for(int k=1;k<=8;k++){
		if(i+k<=8&&j-k>=1&&*(*(table+i+k)+j-k)==adversaire(player)){
			s++;
		}
		else break;
	}
	return s;
}
int check1_in_reversed_diag_droite(int i,int j,char player,char **table){
	int s=0;
	for(int k=1;k<=8;k++){
		if(i-k>=1&&j+k<=8&&*(*(table+i-k)+j+k)==adversaire(player)){
			s++;
		}
		else break;
	}
	return s;
}
int check1_in_diag_inf(int i,int j,char player,char **table){
	int s=0;
	for(int k=1;k<=8;k++){
		if(i-k>=1&&j-k>=1&&*(*(table+i-k)+j-k)==adversaire(player)){
			s++;
		}
		else break;
	}
	return s;
}	
//**********************************************************************************************************************************************************
/*les fonctions qui suivent vont permettre le mouvement d'un joueur a une position en consommant une boule ou plus du joueur precedent*/
char **ecraser_diagonal_sup(char **table,int i,int j,char player){
	int k=check1_in_diag_sup(i,j,player,table);
	for(int n=0;n<=k;n++){
		
		if(i+n<=8&&j+n<=8)	*(*(table+i+n)+j+n)=player;
		
	}
	return table;
}
char **ecraser_diagonal_inf(char **table,int i,int j,char player){
	int k=check1_in_diag_inf(i,j,player,table);
	for(int n=0;n<=k;n++){
		
		if(i-n>=1&&j-n>=1)	*(*(table+i-n)+j-n)=player;
		
	}
	return table;
}
char **ecraser_ligne_gauche(char **table,int i,int j,char player){
	int k=check1_in_ligne_gauche(i,j,player,table);
	for(int n=0;n<=k;n++){
		
		if(j-n>=1)	*(*(table+i)+j-n)=player;
		
	}
	return table;
}
char **ecraser_ligne_droite(char **table,int i,int j,char player){
	int k=check1_in_ligne_droite(i,j,player,table);
	for(int n=0;n<=k;n++){
		
		if(j+n<=8)	*(*(table+i)+j+n)=player;
		
	}
	return table;
}
char **ecraser_colonne_up(char **table,int i,int j,char player){
	int k=check1_in_colonne_up(i,j,player,table);
	for(int n=0;n<=k;n++){
		
		if(i-n>=1)	*(*(table+i-n)+j)=player;
		
	}
	return table;
}
char **ecraser_colonne_down(char **table,int i,int j,char player){
	int k=check1_in_colonne_down(i,j,player,table);
	for(int n=0;n<=k;n++){
		
		if(i+n<=8)	*(*(table+i+n)+j)=player;
		
	}
	return table;
}
char **ecraser_reversed_diagonal_gauche(char **table,int i,int j,char player){
	int k=check1_in_reversed_diag_gauche(i,j,player,table);
	for(int n=0;n<=k;n++){
		if(i+n<=8&&j-n>=1) *(*(table+i+n)+j-n)=player;
		}
	return table;
}
char **ecraser_reversed_diagonal_droite(char **table,int i,int j,char player){
	int k=check1_in_reversed_diag_droite(i,j,player,table);
	for(int n=0;n<=k;n++){
		if(i-n>=1&&j+n<=8) *(*(table+i-n)+j+n)=player;
	}
	return table;
}
//************************************************************************************************************************************************
/*la fonction suivante permet de creer la table:
  1 2 3 4 5 6 7 8		 
1 . . . . . . . .		
2 . . . . . . . .		
3 . . . . . . . . 
4 . . . w b . . . 
5 . . . b w . . .
6 . . . . . . . .
7 . . . . . . . .
8 . . . . . . . .
*/
char **creer_table(){
	char outer_corner[10]={' ','1','2','3','4','5','6','7','8'};
	char **table;
	table=(char **)malloc(9*sizeof(char *));
	for(int i=0;i<=8;i++){
		table[i]=(char *)malloc(9*sizeof(char));
		for(int j=0;j<=8;j++){
			if(j==0||i==0)	*(*(table+i)+j)=outer_corner[i+j];
			else if(i==4&&j==4)	*(*(table+i)+j)='b';
			else if(i==4&&j==5)	*(*(table+i)+j)='w';
			else if(i==5&&j==4)	*(*(table+i)+j)='w';
			else if(i==5&&j==5)	*(*(table+i)+j)='b';
			else	*(*(table+i)+j)='.';
		}
	}
	return table;
}
//***********************************************************************************************************************************************************
/*la fontion suivante permet d'afficher l'historique des mouvement a partir d'un fichier "mouvement.txt" en ajouttant les donnees du fichier a 
un tableau de chaine de caracteres exemple:T[0] est egale a "b=(2,1)" puis elle affiche le contenue du table */
void historique_mouvements(char file[]){
	char b[20];
	FILE *fichier;
	char a[20];
	int k=0,length;
	char **T;
	T=(char **)malloc(100*sizeof(char *));
	fichier=fopen(file,"r+");
	fseek(fichier,0,SEEK_END);
	length=ftell(fichier);
	fseek(fichier,0,SEEK_SET);
	if(length!=0){
		while(!feof(fichier)){	
			fgets(a,20,fichier);
			T[k]=(char *)malloc(20*sizeof(char));
			strcpy(T[k],a);
			if(k!=0&&strcmp(a,b)==0) break;	
			strcpy(b,a);
			k++;
		}
		printf("l'historique des mouvements est:\n");
		for(int i=0;i<k;i++){
			printf("%s",T[i]);
		}
	}
	fclose(fichier);
}


//*****************************************************************************************************************************************************************************

//************************************************************************************************************************************************************
//la fonction suivante donne le nombre des  places possibles c'est a dire le nombre des 'o'
int count_o(char **table){
	int k=0;
	for(int i=1;i<=8;i++){
		for(int j=1;j<=8;j++){
			if(*(*(table+i)+j)=='o') k++;
		}
	}
	return k;			
}
void render_text_on_xy(SDL_Renderer* renderer,const char* text, int x,int y,SDL_Color color){
    TTF_Init();
    TTF_Font * font = TTF_OpenFont("src/Redressed-Regular.ttf", 30);
    SDL_Rect dest;
    dest.x = x; dest.y = y;
    SDL_Surface * surface = TTF_RenderText_Solid(font, text, color);
    SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);
    SDL_RenderCopy(renderer, texture, NULL, &dest);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
    TTF_CloseFont(font);
    TTF_Quit();
}
int settings(char player,int s,int d,Mix_Music *bgm,Mix_Chunk *soundeffect){
	SDL_Window *window=NULL;
	SDL_Renderer *renderer=NULL;
	SDL_Init(SDL_INIT_EVERYTHING);
	window=SDL_CreateWindow("logo",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,600,450,0);
	renderer=SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
	SDL_Surface *surface1=NULL,*surface2=NULL,*surface3=NULL,*surface4=NULL,*surface5=NULL,*surface6=NULL,*surface7=NULL,*surface8=NULL,*surface9=NULL;
	SDL_Texture *texture1=NULL,*texture2=NULL,*texture3=NULL,*texture4=NULL,*texture5=NULL,*texture6=NULL,*texture7=NULL,*texture8=NULL,*texture9=NULL;
	SDL_Event event;
	SDL_Rect rect;
	int done=0;
	int x,y;
	surface1=IMG_Load("src/restart-cap.jpg");
	surface2=IMG_Load("src/mouvements-cap.jpg");
	surface3=IMG_Load("src/sound-cap.jpg");
	surface5=IMG_Load("src/restart.jpg");
	surface6=IMG_Load("src/score.jpg");
	surface7=IMG_Load("src/sound-icon.jpg");
	surface8=IMG_Load("src/no-sound-icon.png");
	surface9=IMG_Load("src/background1.jpg");
	surface4=IMG_Load("src/quit-button.jpg");
	texture5= SDL_CreateTextureFromSurface(renderer,surface5);
	texture6= SDL_CreateTextureFromSurface(renderer,surface6);
	texture7= SDL_CreateTextureFromSurface(renderer,surface7);
	texture8= SDL_CreateTextureFromSurface(renderer,surface8);
	texture9= SDL_CreateTextureFromSurface(renderer,surface9);
	texture4= SDL_CreateTextureFromSurface(renderer,surface4);
	texture3= SDL_CreateTextureFromSurface(renderer,surface3);
	texture2= SDL_CreateTextureFromSurface(renderer,surface2);
	texture1= SDL_CreateTextureFromSurface(renderer,surface1);
	SDL_FreeSurface(surface5);
	SDL_FreeSurface(surface6);
	SDL_FreeSurface(surface7);
	SDL_FreeSurface(surface8);
	SDL_FreeSurface(surface9);
	SDL_FreeSurface(surface4);
	SDL_FreeSurface(surface3);
	SDL_FreeSurface(surface2);
	SDL_FreeSurface(surface1);
	SDL_SetRenderDrawColor(renderer,255,255,102,225);
	SDL_RenderClear(renderer);
	rect.x=0;
	rect.y=0;
	rect.h=500;
	rect.w=600;
	SDL_RenderCopy(renderer,texture9,NULL,&rect);
	rect.x=570;
	rect.y=10;
	rect.h=28;
	rect.w=28;
	SDL_RenderCopy(renderer,texture4,NULL,&rect);
	rect.x=100;
	rect.y=200;
	rect.h=120;
	rect.w=100;
	SDL_RenderCopy(renderer,texture5,NULL,&rect);
	rect.x=230;
	rect.y=200;
	rect.h=120;
	rect.w=100;
	SDL_RenderCopy(renderer,texture6,NULL,&rect);
	if(!Mix_PlayingMusic()){
		rect.x=350;
		rect.y=200;
		rect.h=120;
		rect.w=100;
		SDL_RenderCopy(renderer,texture8,NULL,&rect);
	}
	else if(Mix_PausedMusic()){
		rect.x=350;
		rect.y=200;
		rect.h=120;
		rect.w=100;
		SDL_RenderCopy(renderer,texture8,NULL,&rect);
	}
	else{
		rect.x=350;
		rect.y=200;
		rect.h=120;
		rect.w=100;
		SDL_RenderCopy(renderer,texture7,NULL,&rect);
	}
	rect.x=100;
	rect.y=325;
	rect.h=30;
	rect.w=100;
	SDL_RenderCopy(renderer,texture1,NULL,&rect);
	rect.x=230;
	rect.y=325;
	rect.h=30;
	rect.w=100;
	SDL_RenderCopy(renderer,texture2,NULL,&rect);
	rect.x=350;
	rect.y=325;
	rect.h=30;
	rect.w=100;
	SDL_RenderCopy(renderer,texture3,NULL,&rect);
	while(!done){
		
		SDL_RenderPresent(renderer);
		while(SDL_PollEvent(&event)){
			switch(event.type){
				case SDL_MOUSEBUTTONDOWN:
					Mix_PlayChannel(-1,soundeffect,0);
					x=event.motion.x;
					y=event.motion.y;
					if(x<200&&x>100&&y>200&&y<320){
						SDL_DestroyTexture(texture5);
						SDL_DestroyTexture(texture6);
						SDL_DestroyTexture(texture7);
						SDL_DestroyTexture(texture8);
						SDL_DestroyTexture(texture9);
						texture5=NULL;texture6=NULL;texture7=NULL;texture8=NULL;texture9=NULL;
						SDL_DestroyRenderer(renderer);
						SDL_DestroyWindow(window);
						renderer=NULL;window=NULL;
						return 1;
						
					}
					else if(x>230&&x<330&&y>200&&y<320) historique_mouvements("mouvement.txt");
					else if(x<450&&x>350&&y>200&&y<320) {
						if(!Mix_PlayingMusic()){
							rect.x=350;
							rect.y=200;
							rect.h=120;
							rect.w=100;
							SDL_RenderCopy(renderer,texture7,NULL,&rect);
							SDL_RenderPresent(renderer);
							Mix_PlayMusic(bgm,-1);
						}
						else if(Mix_PausedMusic()){
							rect.x=350;
							rect.y=200;
							rect.h=120;
							rect.w=100;
							SDL_RenderCopy(renderer,texture7,NULL,&rect);
							SDL_RenderPresent(renderer);
							Mix_ResumeMusic();
						}
						else{
							rect.x=350;
							rect.y=200;
							rect.h=120;
							rect.w=100;
							SDL_RenderCopy(renderer,texture8,NULL,&rect);
							Mix_PauseMusic();
						}
					}
					else if(x<598&&x>570&&y<38&&y>10){
						SDL_DestroyTexture(texture5);
						SDL_DestroyTexture(texture6);
						SDL_DestroyTexture(texture7);
						SDL_DestroyTexture(texture8);
						SDL_DestroyTexture(texture9);
						texture5=NULL;texture6=NULL;texture7=NULL;texture8=NULL;texture9=NULL;
						SDL_DestroyRenderer(renderer);
						SDL_DestroyWindow(window);
						renderer=NULL;window=NULL;
						done=1;
					}
					break;
				case SDL_KEYDOWN:
					if(event.key.keysym.sym==SDLK_p){
							if(!Mix_PlayingMusic()){
								rect.x=350;
								rect.y=200;
								rect.h=120;
								rect.w=100;
								SDL_RenderCopy(renderer,texture7,NULL,&rect);
								SDL_RenderPresent(renderer);
								Mix_PlayMusic(bgm,-1);
							}
							else if(Mix_PausedMusic()){
								rect.x=350;
								rect.y=200;
								rect.h=120;
								rect.w=100;
								SDL_RenderCopy(renderer,texture7,NULL,&rect);
								SDL_RenderPresent(renderer);
								Mix_ResumeMusic();
							}
							else{
								rect.x=350;
								rect.y=200;
								rect.h=120;
								rect.w=100;
								SDL_RenderCopy(renderer,texture8,NULL,&rect);
								Mix_PauseMusic();
							}
					}
					else if(event.key.keysym.sym==SDLK_s) {
						Mix_HaltMusic();
					}
					else if(event.key.keysym.sym==SDLK_ESCAPE){
						/*Mix_FreeChunk(soundeffect);
						Mix_FreeMusic(bgm);
						bgm=NULL;
						soundeffect=NULL;
						Mix_Quit();*/
						SDL_DestroyTexture(texture5);
						SDL_DestroyTexture(texture6);
						SDL_DestroyTexture(texture7);
						SDL_DestroyTexture(texture8);
						SDL_DestroyTexture(texture9);
						texture5=NULL;texture6=NULL;texture7=NULL;texture8=NULL;texture9=NULL;
						SDL_DestroyRenderer(renderer);
						SDL_DestroyWindow(window);
						renderer=NULL;window=NULL;
						done=1;
					}
					break;
				case SDL_QUIT:
					done=1;
					break;
				default:
					break;		
			}
		}
	}
	SDL_DestroyTexture(texture5);
	SDL_DestroyTexture(texture6);
	SDL_DestroyTexture(texture7);
	SDL_DestroyTexture(texture8);
	SDL_DestroyTexture(texture9);
	texture5=NULL;texture6=NULL;texture7=NULL;texture8=NULL;texture9=NULL;
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	renderer=NULL;window=NULL;
	return 0;
	//SDL_Quit();

}
void play_game(SDL_Renderer *renderer,SDL_Window *window,Mix_Music *bgm,Mix_Chunk *soundeffect);
//************************************************************************************************************************************************************
/*la fonction suivante demanded du joueur la position ou il veut aller et elle verifie si c'est une position sccessible c'est a dire s'il 
existe un 'o' dans cette place si cela est verifier donc elle ecrase les boules de l'adversaire et remplace le 'o' par les boules du joueur sinon 
elle retourne le message "deplacement illegale* et elle stock les valeurs de ces mouvements dans un fichier "mouvement.txt" puis elle demande au
suivant s'il veut recommencer ou non ,s'il demande de recommencer donc elle sauvegarde le dernier tableau dans un fichier que le joueur choisi son nom
puis elle recommence le jeu en commencant avec le joueur qui a choisit de recommencer,NB: dans le cas d'un robot avec un etre humain 
la fonction ne demande qu'a l'etre humain s'il veut recommencer */
char **move(char **table,char player,int s,int d,char player1[],SDL_Renderer *renderer,SDL_Window *window,Mix_Music *bgm,Mix_Chunk *soundeffect,char **table1){
	SDL_Event event;
	SDL_Rect rect,rect2;
	SDL_Texture *texture=NULL,*texture1=NULL,*texture2=NULL,*texture3=NULL,*texture4=NULL,*texture5=NULL,*texture6=NULL,*texture7=NULL,*texture8=NULL;
	SDL_Surface *surface=NULL,*surface1=NULL,*surface2=NULL,*surface3=NULL,*surface4=NULL,*surface5=NULL,*surface6=NULL,*surface7=NULL,*surface8=NULL;
	int done=0;
	int n,k;
	int r;
	FILE *fichier;
	char **table3,nomfichier[80];
	char **table2;
	table2=(char **)malloc(9*sizeof(char *));
	for(int i=0;i<=8;i++){
		table2[i]=(char *)malloc(9*sizeof(char));
		for(int j=0;j<=8;j++){
			*(*(table2+i)+j)=*(*(table1+i)+j);
		}
	}
	//table3=table;
	surface=IMG_Load("src/Othello-White.jpg");
	surface1=IMG_Load("src/Othello-Black.jpg");
	surface2=IMG_Load("src/Othello-logo.jpg");
	surface3=IMG_Load("src/o-case.png");
	surface4=IMG_Load("src/background.jpg");
	surface5=IMG_Load("src/settings.jpg");
	surface6=IMG_Load("src/white-round.png");
	surface7=IMG_Load("src/black-round.png");
	surface8=IMG_Load("src/home-button.png");
	texture= SDL_CreateTextureFromSurface(renderer,surface);
	SDL_FreeSurface(surface);
	texture1= SDL_CreateTextureFromSurface(renderer,surface1);
	SDL_FreeSurface(surface1);
	texture2= SDL_CreateTextureFromSurface(renderer,surface2);
	SDL_FreeSurface(surface2);
	texture3=SDL_CreateTextureFromSurface(renderer,surface3);
	SDL_FreeSurface(surface3);
	texture4=SDL_CreateTextureFromSurface(renderer,surface4);
	SDL_FreeSurface(surface4);
	texture5=SDL_CreateTextureFromSurface(renderer,surface5);
	SDL_FreeSurface(surface5);
	texture6=SDL_CreateTextureFromSurface(renderer,surface6);
	SDL_FreeSurface(surface6);
	texture7=SDL_CreateTextureFromSurface(renderer,surface7);
	SDL_FreeSurface(surface7);
	texture8=SDL_CreateTextureFromSurface(renderer,surface8);
	SDL_FreeSurface(surface8);
	if(window==NULL){
		printf("Error : %s\n",SDL_GetError() );
		exit(1);
	}
	SDL_SetRenderDrawColor(renderer,255,255,102,225);
	SDL_RenderClear(renderer);
	int x,y;
	SDL_SetRenderDrawColor(renderer,0,75,4,225);
		rect.x=0;
		rect.y=0;
		rect.h=700;
		rect.w=800;
	SDL_RenderCopy(renderer,texture4,NULL,&rect);
		rect.x=220;
		rect.y=20;
		rect.h=100;
		rect.w=374;
	SDL_RenderCopy(renderer,texture2,NULL,&rect);
		
	SDL_SetRenderDrawColor(renderer,37,75,4,225);
		rect.x=190;
		rect.y=190;
		rect.h=420;
		rect.w=420;
		SDL_RenderDrawRect(renderer,&rect);
		SDL_RenderFillRect(renderer,&rect);
	SDL_SetRenderDrawColor(renderer,3,125,88,225);
		rect.x=200;
		rect.y=200;
		rect.h=400;
		rect.w=400;
		SDL_RenderDrawRect(renderer,&rect);
	SDL_RenderFillRect(renderer,&rect);
		rect.x=760;
		rect.y=20;
		rect.h=38;
		rect.w=38;
	SDL_RenderCopy(renderer,texture5,NULL,&rect);
		rect.x=760;
		rect.y=59;
		rect.h=38;
		rect.w=38;
	SDL_RenderCopy(renderer,texture8,NULL,&rect);
	if(player=='w'){
		rect.x=10;
		rect.y=20;
		rect.h=38;
		rect.w=100;
		SDL_RenderCopy(renderer,texture6,NULL,&rect);
	}
	else{
		rect.x=10;
		rect.y=20;
		rect.h=38;
		rect.w=100;
		SDL_RenderCopy(renderer,texture7,NULL,&rect);
	}

	//SDL_RenderPresent(renderer);
		//SDL_SetRenderDrawColor(renderer,0,102,204,225);
		int a=200,b=200;
		for(int i=1;i<9;i++){
			for(int j=1;j<9;j++){
			SDL_SetRenderDrawColor(renderer,37,75,4,225);
			rect.x=a;
			rect.y=b;
			rect.h=50;
			rect.w=50;
			if(deplacement(table,player)[i][j]=='w')	SDL_RenderCopy(renderer,texture,NULL,&rect);
			else if(deplacement(table,player)[i][j]=='b') SDL_RenderCopy(renderer,texture1,NULL,&rect);
			else if(deplacement(table,player)[i][j]=='o') SDL_RenderCopy(renderer,texture3,NULL,&rect);
			SDL_RenderDrawRect(renderer,&rect);
			a+=50;
			}
			a=200;
			b+=50;

		}
		while(!done&&s==1){
		
		SDL_RenderPresent(renderer);
		while(SDL_PollEvent(&event)){
			switch(event.type){
				case SDL_MOUSEBUTTONDOWN:
				Mix_PlayChannel(-1,soundeffect,0);
				x=event.motion.x;
				y=event.motion.y;
				int a=200,b=200,c=0;
				if(event.button.button==SDL_BUTTON_LEFT){
					if(x>760&&x<798&&y>20&&y<58&&s==1){
						if(settings(player,s,d,bgm,soundeffect)){
							system("cls");
							FILE *fp;
							printf("nom de la partie\t");
							scanf("%s",&nomfichier);
							strcat(nomfichier,".txt");
							fp=fopen(nomfichier,"a+");
    						char ch[20];
    						for(int i=0;i<=8;i++){
    							for(int j=0;j<=8;j++){
   		 							fprintf(fp,"%c\n",*(*(table+i)+j) );
   					 			}
    						}
    						fprintf(fp,"%c\n",adversaire(player) );
							fclose(fp);
							fichier=fopen("mouvement.txt","w");
							fclose(fichier);
							SDL_DestroyTexture(texture);
							SDL_DestroyTexture(texture1);
							SDL_DestroyTexture(texture2);
							SDL_DestroyTexture(texture3);
							SDL_DestroyTexture(texture4);
							SDL_DestroyTexture(texture5);
							SDL_DestroyTexture(texture6);
							SDL_DestroyTexture(texture7);
							done=1;
							return move(table2,player,s,d,player1,renderer,window,bgm,soundeffect,table1);
						}
					}
					if(x>760&&x<798&&y>59&&y<97&&s==1){
						SDL_DestroyTexture(texture);
						SDL_DestroyTexture(texture1);
						SDL_DestroyTexture(texture2);
						SDL_DestroyTexture(texture3);
						SDL_DestroyTexture(texture4);
						SDL_DestroyTexture(texture5);
						SDL_DestroyTexture(texture6);
						SDL_DestroyTexture(texture7);
						done=1;
						play_game(renderer,window,bgm,soundeffect);
					}
				else {
						if(event.button.button==SDL_BUTTON_LEFT){
							while(b<600){
								while(a<600){
									if(x>=a&&x<(a+50)){
										if(y>=b&&y<(b+50)){
										
										
											c=1;
											break;
										}
									}
									a+=50;
								}
								if(c==1) break;
								a=200;
								b+=50;
							}

						}
						k=((a-200)/50)+1;
						n=((b-200)/50)+1;
						if(n<=8&&n>=1&&k<=8&&k>=1&&*(*(deplacement(table,player)+n)+k)=='o'){
							done=1;
						} 
					}
					break;
				case SDL_KEYDOWN:
					if(event.key.keysym.sym==SDLK_p){
						if(!Mix_PlayingMusic()) Mix_PlayMusic(bgm,-1);
						else if(Mix_PausedMusic()) Mix_ResumeMusic();
						else Mix_PauseMusic();
					}
					else if(event.key.keysym.sym==SDLK_s) {
						Mix_HaltMusic();
					}
					else if(event.key.keysym.sym==SDLK_ESCAPE){
						Mix_FreeChunk(soundeffect);
						Mix_FreeMusic(bgm);
						bgm=NULL;
						soundeffect=NULL;
						Mix_Quit();
						SDL_DestroyTexture(texture);
						SDL_DestroyTexture(texture1);
						SDL_DestroyTexture(texture2);
						SDL_DestroyTexture(texture3);
						SDL_DestroyTexture(texture4);
						SDL_DestroyTexture(texture5);
						SDL_DestroyTexture(texture6);
						SDL_DestroyTexture(texture7);
						SDL_DestroyRenderer(renderer);
						SDL_DestroyWindow(window);
						SDL_Quit();
						done=1;
					}
					break;
				case SDL_QUIT:
					Mix_FreeChunk(soundeffect);
					Mix_FreeMusic(bgm);
					bgm=NULL;
					soundeffect=NULL;
					Mix_Quit();
					SDL_DestroyTexture(texture);
					SDL_DestroyTexture(texture1);
					SDL_DestroyTexture(texture2);
					SDL_DestroyTexture(texture3);
					SDL_DestroyTexture(texture4);
					SDL_DestroyTexture(texture5);
					SDL_DestroyTexture(texture6);
					SDL_DestroyTexture(texture7);
					SDL_DestroyRenderer(renderer);
					SDL_DestroyWindow(window);
					SDL_Quit();
					
					done=1;
					break;
				default:
					break;		
			}
		}
	}

				
	SDL_RenderPresent(renderer);
	SDL_Delay(120);
	}
	if(s==0){
		int o_number=count_o(deplacement(table,player));
		int **tab3;
		int a=0,b;
		tab3=(int **)malloc((o_number)*sizeof(int *));
		for(int i=1;i<=8;i++){
			for(int j=1;j<=8;j++){
				if(*(*(deplacement(table,player)+i)+j)=='o'){
					tab3[a]=(int *)malloc(2*sizeof(int));
					tab3[a][0]=i;
					tab3[a][1]=j;
					a++;

				}

			}
		}
		n=rand()%9;
		int ind=1;
		int i=0;
		for(i=0;i<a;i++){
			if(tab3[i][0]==n){
				k=rand()%9;
				while(k!=tab3[i][1]){
					 k=rand()%9;
				}
				break;
			}
			
			if(ind==(a)||n!=tab3[a-1][0]){
				n=rand()%9;
				i=-1; 
				ind=0;
			}
			ind++;
		}	
	

	}
	if(*(*(deplacement(table,player)+n)+k)=='o'){
		fichier=fopen("mouvement.txt","a+");
		fprintf(fichier, "%c=(%d,%d)\n",player,n,k );
		fclose(fichier);
		if(check_in_diag_inf(n,k,player,table)!=0)	table3=ecraser_diagonal_inf(table,n,k,player);
		if(check_in_diag_sup(n,k,player,table)!=0)	table3=ecraser_diagonal_sup(table,n,k,player);
		if(check_in_reversed_diag_droite(n,k,player,table)!=0)	table3=ecraser_reversed_diagonal_droite(table,n,k,player);
		if(check_in_reversed_diag_gauche(n,k,player,table)!=0)	table3=ecraser_reversed_diagonal_gauche(table,n,k,player);
		if(check_in_colonne_down(n,k,player,table)!=0)	table3=ecraser_colonne_down(table,n,k,player);
		if(check_in_colonne_up(n,k,player,table)!=0)	table3=ecraser_colonne_up(table,n,k,player);
		if(check_in_ligne_droite(n,k,player,table)!=0)	table3=ecraser_ligne_droite(table,n,k,player);
		if(check_in_ligne_gauche(n,k,player,table)!=0)	table3=ecraser_ligne_gauche(table,n,k,player);
	}	
	SDL_Delay(60);
	SDL_DestroyTexture(texture);
	SDL_DestroyTexture(texture1);
	SDL_DestroyTexture(texture2);
	SDL_DestroyTexture(texture3);
	SDL_DestroyTexture(texture4);
	SDL_DestroyTexture(texture5);
	SDL_DestroyTexture(texture6);
	SDL_DestroyTexture(texture7);
	return table3;
}
//************************************************************************************************************************************************************
//la fonction suivante calcule ne nombre des 'b' dans une table prise en argumant
int count_b(char **table){
	int k=0;
	for(int i=1;i<=8;i++){
		for(int j=1;j<=8;j++){
			if(*(*(table+i)+j)=='b') k++;
		}
	}
	return k;			
}
//************************************************************************************************************************************************************
//la fonction suivante calcule ne nombre des 'w' dans une table prise en argumant
int count_w(char **table){
	int k=0;
	for(int i=1;i<=8;i++){
		for(int j=1;j<=8;j++){
			if(*(*(table+i)+j)=='w') k++;
		}
	}
	return k;			
}//************************************************************************************************************************************************************
//la fonction suivante permet de sauvegarder les caracteristiques des deux joueurs dans un fichier "players.txt"
void sauvegarder(joueur player1,joueur player2){
	FILE *fichier;
	fichier=fopen("players.txt","a+");
	fprintf(fichier, "%s %d\n",player1.nom,player1.score);
	fprintf(fichier, "%s %d\n",player2.nom,player2.score);
	fclose(fichier);
}

//************************************************************************************************************************************************************
/*la fonction suivante va jouer le jeu a tour de role de facon qu'elle affiche le nom du joueur , l'historique des mouvement et le score apres chaque
mouvement apres la fin du jeu elle enregistre le joueur gagnant dans un fichier gagnant et elle enregistre les caractairistiques des deuc joueurs
dans un fichier "players.txt" */
void play_round(char player,char joueur1[],char joueur2[],char **table,int s,int d,SDL_Renderer *renderer,SDL_Window *window,Mix_Music *bgm,Mix_Chunk *soundeffect,char **table1){
	FILE *fichier;
	fichier=fopen("mouvement.txt","w");
	fclose(fichier);
	char **tab2;
	char **tab;
	int W,B;
	char playerinter[20];
	joueur player_uno,player_dos;
	strcpy(player_dos.nom,joueur2);
	strcpy(player_uno.nom,joueur1);
	player_uno.logo=player;
	player_dos.logo=adversaire(player);
	//afficher_table(deplacement(table,player),renderer,window);
	tab=move(table,player,s,d,player_uno.nom,renderer,window,bgm,soundeffect,table1);
	
	do{
		int c;
		char **tab1;
		tab1=move(tab,adversaire(player),d,s,player_dos.nom,renderer,window,bgm,soundeffect,table1);
		if(count_o(deplacement(table,player))!=0){
		strcpy(playerinter,player_dos.nom);
		strcpy(player_dos.nom,player_uno.nom);
		strcpy(player_uno.nom,playerinter);
		tab=tab1;
		player=adversaire(player);
		c=s;
		s=d;
		d=c;
		}
		tab=tab1;
		tab2=deplacement(tab,player);
	}while(count_o(deplacement(tab,adversaire(player)))!=0||count_o(tab2)!=0);
	printf("le score est:\t\t\t\t\t\t\t\t[w=%d,b=%d]\n",count_w(tab),count_b(tab));
	fichier=fopen("gagnants.txt","a+");
	W=count_w(tab);
	B=count_b(tab);
	if(W<B&&(player_uno.logo)=='b')	{
		printf("le joueur %s a gagne\n",player_uno.nom);
		player_uno.score=B;
		player_dos.score=W;
		fprintf(fichier, "%s %d\n",player_uno.nom,player_uno.score);

	}
	else if(W<B&&(player_dos.logo)=='b')	{
		printf("le joueur %s a gagne\n",player_dos.nom);
		player_uno.score=W;
		player_dos.score=B;
		fprintf(fichier, "%s %d\n",player_dos.nom,player_dos.score);
	}
	else if(W>B&&(player_dos.logo)=='w') {
		printf("le joueur %s a gagne\n",player_dos.nom);
		player_uno.score=B;
		player_dos.score=W;
		fprintf(fichier, "%s %d\n",player_dos.nom,player_dos.score);
	}
	else if(W>B&&(player_uno.logo)=='w') {
		printf("le joueur %s a gagne\n",player_uno.nom);
		player_uno.score=W;
		player_dos.score=B;
		fprintf(fichier, "%s %d\n",player_uno.nom,player_uno.score);
	}
	else printf("egalitee\n");
	sauvegarder(player_uno,player_dos);	
	fclose(fichier);

}
//************************************************************************************************************************************************************
//la fonction suivante permet de jouer le jeu et donne le logo du premier jouer'w' ou 'b' selon son choix s'il est un humain sinon il lui donne 'b'
/*void jouer(char **table,char joueur1[],char joueur2[],int d,int s){
	FILE *fichier;
	char player;

	

	fichier=fopen("mouvement.txt","w");
	fclose(fichier);
	system("cls");
	if(d==1){
		printf("choisissez votre avatar b ou w? \t");
		player=getchar();
		rewind(stdin);
	}
	else player='b';
	play_round(player,joueur1,joueur2,table,d,s);
	fichier=fopen("mouvement.txt","w");
	fclose(fichier);
}*/
//************************************************************************************************************************************************************
/*la fonction suivante permet de charger une partie deja sauvegarder de facon a prendre les donnees du fichier deja nomme et les ajouter a un tableau
 de caracteres on va donc creer un vouveau tableu qu'on va utiliser pour jouer la partie	*/
void charger_partie(char joueur1[],char joueur2[],int s,int d,SDL_Renderer *renderer,SDL_Window *window,Mix_Music *bgm,Mix_Chunk *soundeffect){
	FILE *fichier;
	char partie[20];
	FILE *fp;
	char **table,**table1,a[2],player;

	fichier=fopen("mouvement.txt","w");
	fclose(fichier);
	
	printf("quelle partie voulez vous charger?\t");
	scanf("%s",&partie);
	strcat(partie,".txt");
	
	fp=fopen(partie,"r+");
	if(fp==NULL){
		printf("Erreur!!\n");
		exit(1);
	}
	table=(char **)malloc(9*sizeof(char *));
	table1=(char **)malloc(9*sizeof(char *));
	for(int i=0;i<=8;i++){
		table[i]=(char *)malloc(9*sizeof(char));
		table1[i]=(char *)malloc(9*sizeof(char));
		for(int j=0;j<=8;j++){
			if(i==0&&j==0) {
				*(*(table1+i)+j)=' ';
				*(*(table+i)+j)=' ';
			}
			else{
				fscanf(fp,"%s\n",a);
				*(*(table+i)+j)=a[0];
				*(*(table1+i)+j)=a[0];
			}
		}
	}
	fscanf(fp,"%s\n",a);
	player=a[0];
	fclose(fp);
	//system("cls");
	play_round(player,joueur1,joueur2,table,s,d,renderer,window,bgm,soundeffect,table1);

}
//************************************************************************************************************************************************************
/*la fonction suivante permet d'afficher les dix meilleurs scores en utilisant le fichiere "gagnants.txt" et en prenant juste le score des gagnants puis
on l'ajoute a un tableau d'entiers et on le tri par ordre croissant puis on charche dans le fichier le joueur qui a son image dans le tableau trier
et on l'affiche , on se limite des 10 meilleurs scores*/
void dix_meilleurs_scores(char file[]){
	char b[20];
	FILE *fichier;
	char a[20];
	int k=0,length;
	char **T;
	T=(char **)malloc(100*sizeof(char *));
	fichier=fopen(file,"r+");
	fseek(fichier,0,SEEK_END);
	length=ftell(fichier);
	fseek(fichier,0,SEEK_SET);
	fgets(a,20,fichier);
	if(length!=0){
		while(!feof(fichier)){	
			if(k!=0&&a==b){
				fgets(a,20,fichier);
				continue;
			}
			T[k]=(char *)malloc(20*sizeof(char));
			strcpy(T[k],a);
			strcpy(b,a);
			k++;
			fgets(a,20,fichier);

		}
	}
	char **table;
	table=(char **)malloc(k*sizeof(char *));
	for(int i=0;i<=k-1;i++){
		table[i]=(char *)malloc(2*sizeof(char));
		int j=0;
		int d=0;
		while(d<1){
			if(*(*(T+i)+j)==' '){
				table[i][0]=T[i][j+1];
				table[i][1]=T[i][j+2];
				table[i][2]=T[i][j+3];
				d++;

			}

			j++;
		}
		
	}

	int table_score[k];
	for(int i=0;i<=k-1;i++){
		table_score[i]=atoi(table[i]);
	}
	for(int i=0;i<k;i++){
		int l;
		for(int j=0;j<=k-1;j++){
			if(table_score[j]>table_score[j+1]){
				l=table_score[j+1];
				table_score[j+1]=table_score[j];
				table_score[j]=l;
			}
		}
	}
	int c=0,ind=0;
	for(int i=0;i<10;i++){
		for(int j=0;j<=k-1;j++){
			if(table_score[c]==atoi(table[j])){
				while(table_score[c+1]==table_score[c]){
					c++;
				}
				printf("%s",T[j] );
				ind++;
			}
		}
		c++;
		if(ind==10) break;
	}
	fclose(fichier);
}
int jouer_contre(SDL_Renderer *renderer,SDL_Window *window,Mix_Music *bgm,Mix_Chunk *soundeffect){
	SDL_Surface *surface1=NULL,*surface2=NULL,*surface3=NULL,*surface4=NULL,*surface5=NULL,*surface6=NULL,*surface7=NULL,*surface8=NULL,*surface9=NULL;
	SDL_Texture *texture1=NULL,*texture2=NULL,*texture3=NULL,*texture4=NULL,*texture5=NULL,*texture6=NULL,*texture7=NULL,*texture8=NULL,*texture9=NULL;
	SDL_Event event;
	SDL_Rect rect;
	int done=0;
	int x,y;
	surface1=IMG_Load("src/quit-button.jpg");
	surface2=IMG_Load("src/home-button.png");
	surface3=IMG_Load("src/sound-icon.jpg");
	surface4=IMG_Load("src/no-sound-icon.png");
	surface5=IMG_Load("src/contre-humain.png");
	surface6=IMG_Load("src/contre-robot.png");
	surface7=IMG_Load("src/Othello-logo.jpg");
	surface8=IMG_Load("src/background.jpg");
	surface9=IMG_Load("src/choose-opponent.png");
	texture1= SDL_CreateTextureFromSurface(renderer,surface1);
	texture2= SDL_CreateTextureFromSurface(renderer,surface2);
	texture3= SDL_CreateTextureFromSurface(renderer,surface3);
	texture4= SDL_CreateTextureFromSurface(renderer,surface4);
	texture5= SDL_CreateTextureFromSurface(renderer,surface5);
	texture6= SDL_CreateTextureFromSurface(renderer,surface6);
	texture7= SDL_CreateTextureFromSurface(renderer,surface7);
	texture8= SDL_CreateTextureFromSurface(renderer,surface8);
	texture9= SDL_CreateTextureFromSurface(renderer,surface9);
	SDL_FreeSurface(surface1);
	SDL_FreeSurface(surface2);
	SDL_FreeSurface(surface3);
	SDL_FreeSurface(surface4);
	SDL_FreeSurface(surface5);
	SDL_FreeSurface(surface6);
	SDL_FreeSurface(surface7);
	SDL_FreeSurface(surface8);
	SDL_FreeSurface(surface9);
	SDL_SetRenderDrawColor(renderer,204,255,229,225);
	SDL_RenderClear(renderer);
	rect.x=0;
	rect.y=0;
	rect.h=700;
	rect.w=800;
	SDL_RenderCopy(renderer,texture8,NULL,&rect);
	rect.x=220;
	rect.y=20;
	rect.h=100;
	rect.w=374;
	SDL_RenderCopy(renderer,texture7,NULL,&rect);
	rect.x=220;
	rect.y=125;
	rect.h=100;
	rect.w=374;
	SDL_RenderCopy(renderer,texture9,NULL,&rect);
	
	SDL_SetRenderDrawColor(renderer,0,102,102,225);
	rect.x=210;
	rect.y=295;
	rect.h=60;
	rect.w=410;
	SDL_RenderDrawRect(renderer,&rect);
	SDL_RenderFillRect(renderer,&rect);
	rect.x=210;
	rect.y=395;
	rect.h=60;
	rect.w=410;
	SDL_RenderDrawRect(renderer,&rect);
	SDL_RenderFillRect(renderer,&rect);
	SDL_SetRenderDrawColor(renderer,204,255,229,225);
	rect.x=215;
	rect.y=300;
	rect.h=50;
	rect.w=400;
	SDL_RenderCopy(renderer,texture5,NULL,&rect);
	rect.x=215;
	rect.y=400;
	rect.h=50;
	rect.w=400;
	SDL_RenderCopy(renderer,texture6,NULL,&rect);
	

	if(!Mix_PlayingMusic()){
		rect.x=760;
		rect.y=20;
		rect.h=38;
		rect.w=38;
		SDL_RenderCopy(renderer,texture4,NULL,&rect);
	}
	else if(Mix_PausedMusic()){
		rect.x=760;
		rect.y=20;
		rect.h=38;
		rect.w=38;
		SDL_RenderCopy(renderer,texture4,NULL,&rect);
	}
	else{
		rect.x=760;
		rect.y=20;
		rect.h=38;
		rect.w=38;
		SDL_RenderCopy(renderer,texture3,NULL,&rect);
	}

	rect.x=760;
	rect.y=59;
	rect.h=38;
	rect.w=38;
	SDL_RenderCopy(renderer,texture2,NULL,&rect);

	while(!done){
		
		SDL_RenderPresent(renderer);
		while(SDL_PollEvent(&event)){
			switch(event.type){
				case SDL_MOUSEBUTTONDOWN:
					Mix_PlayChannel(-1,soundeffect,0);
					x=event.motion.x;
					y=event.motion.y;
					if(x<620&&x>220&&y>300&&y<350){
						return 1;
					}
					else if(x<620&&x>220&&y>400&&y<450) return 0;
					else if(x>760&&x<798&&y>59&&y<97){
						SDL_DestroyTexture(texture9);
						SDL_DestroyTexture(texture8);
						SDL_DestroyTexture(texture1);
						SDL_DestroyTexture(texture2);
						SDL_DestroyTexture(texture3);
						SDL_DestroyTexture(texture4);
						SDL_DestroyTexture(texture5);
						SDL_DestroyTexture(texture6);
						SDL_DestroyTexture(texture7);
						done=1;
						play_game(renderer,window,bgm,soundeffect);
						return 2;
					}
					else if(x<798&&x>760&&y>20&&y<58) {
						
							//SDL_RenderPresent(renderer);
						if(!Mix_PlayingMusic()){
							rect.x=760;
							rect.y=20;
							rect.h=38;
							rect.w=38;
							SDL_RenderCopy(renderer,texture3,NULL,&rect);
							SDL_RenderPresent(renderer);
							Mix_PlayMusic(bgm,-1);
						}
						else if(Mix_PausedMusic()){
							rect.x=760;
							rect.y=20;
							rect.h=38;
							rect.w=38;
							SDL_RenderCopy(renderer,texture3,NULL,&rect);
							SDL_RenderPresent(renderer);
							Mix_ResumeMusic();
						}
						else{
							rect.x=760;
							rect.y=20;
							rect.h=38;
							rect.w=38;
							SDL_RenderCopy(renderer,texture4,NULL,&rect);
							Mix_PauseMusic();
						}

					}
					break;
				case SDL_KEYDOWN:
					if(event.key.keysym.sym==SDLK_p){
						if(!Mix_PlayingMusic()) Mix_PlayMusic(bgm,-1);
						else if(Mix_PausedMusic()) Mix_ResumeMusic();
						else Mix_PauseMusic();
					}
					else if(event.key.keysym.sym==SDLK_s) {
						Mix_HaltMusic();
					}
					else if(event.key.keysym.sym==SDLK_ESCAPE){
						Mix_FreeChunk(soundeffect);
						Mix_FreeMusic(bgm);
						bgm=NULL;
						soundeffect=NULL;
						Mix_Quit();
						SDL_DestroyTexture(texture9);
						SDL_DestroyTexture(texture8);
						SDL_DestroyTexture(texture1);
						SDL_DestroyTexture(texture2);
						SDL_DestroyTexture(texture3);
						SDL_DestroyTexture(texture4);
						SDL_DestroyTexture(texture5);
						SDL_DestroyTexture(texture6);
						SDL_DestroyTexture(texture7);
						SDL_DestroyRenderer(renderer);
						SDL_DestroyWindow(window);
						SDL_Quit();
						done=1;
					}
					break;
				case SDL_QUIT:
					Mix_FreeChunk(soundeffect);
					Mix_FreeMusic(bgm);
					bgm=NULL;
					soundeffect=NULL;
					Mix_Quit();
					SDL_DestroyTexture(texture9);
					SDL_DestroyTexture(texture8);
					SDL_DestroyTexture(texture1);
					SDL_DestroyTexture(texture2);
					SDL_DestroyTexture(texture3);
					SDL_DestroyTexture(texture4);
					SDL_DestroyTexture(texture5);
					SDL_DestroyTexture(texture6);
					SDL_DestroyTexture(texture7);
					SDL_DestroyRenderer(renderer);
					SDL_DestroyWindow(window);
					SDL_Quit();
					
					done=1;
					break;
				default:
					break;		
			}
		}
	}
	SDL_DestroyTexture(texture1);
	SDL_DestroyTexture(texture2);
	SDL_DestroyTexture(texture3);
	SDL_DestroyTexture(texture4);
	SDL_DestroyTexture(texture5);
	SDL_DestroyTexture(texture6);
	SDL_DestroyTexture(texture7);
	SDL_DestroyTexture(texture8);
	SDL_DestroyTexture(texture9);
	Mix_FreeChunk(soundeffect);
	Mix_FreeMusic(bgm);
	bgm=NULL;
	soundeffect=NULL;
	Mix_Quit();
}

char select_logo(Mix_Music *bgm,Mix_Chunk *soundeffect){
	SDL_Window *window=NULL;
	SDL_Renderer *renderer=NULL;
	SDL_Init(SDL_INIT_EVERYTHING);
	window=SDL_CreateWindow("logo",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,600,450,0);
	renderer=SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
	SDL_Surface *surface4=NULL,*surface5=NULL,*surface6=NULL,*surface7=NULL,*surface8=NULL;
	SDL_Texture *texture4=NULL,*texture5=NULL,*texture6=NULL,*texture7=NULL,*texture8=NULL;
	SDL_Event event;
	SDL_Rect rect;
	int done=0;
	int x,y;
	surface4=IMG_Load("src/quit-button.jpg");
	surface5=IMG_Load("src/Othello-White-logo.jpg");
	surface6=IMG_Load("src/Othello-Black-logo.jpg");
	surface7=IMG_Load("src/select-character.png");
	surface8=IMG_Load("src/background1.jpg");
	texture4= SDL_CreateTextureFromSurface(renderer,surface4);
	texture5= SDL_CreateTextureFromSurface(renderer,surface5);
	texture6= SDL_CreateTextureFromSurface(renderer,surface6);
	texture7= SDL_CreateTextureFromSurface(renderer,surface7);
	texture8= SDL_CreateTextureFromSurface(renderer,surface8);
	SDL_FreeSurface(surface4);
	SDL_FreeSurface(surface5);
	SDL_FreeSurface(surface6);
	SDL_FreeSurface(surface7);
	SDL_FreeSurface(surface8);
	SDL_SetRenderDrawColor(renderer,255,255,102,225);
	SDL_RenderClear(renderer);
	rect.x=0;
	rect.y=0;
	rect.h=500;
	rect.w=600;
	SDL_RenderCopy(renderer,texture8,NULL,&rect);
	rect.x=100;
	rect.y=20;
	rect.h=100;
	rect.w=400;
	SDL_RenderCopy(renderer,texture7,NULL,&rect);
	rect.x=100;
	rect.y=200;
	rect.h=120;
	rect.w=100;
	SDL_RenderCopy(renderer,texture5,NULL,&rect);
	rect.x=350;
	rect.y=200;
	rect.h=120;
	rect.w=100;
	SDL_RenderCopy(renderer,texture6,NULL,&rect);
	rect.x=570;
	rect.y=10;
	rect.h=28;
	rect.w=28;
	SDL_RenderCopy(renderer,texture4,NULL,&rect);
	SDL_Color color;
	color.r=255;
	color.g=255;
	color.b=255;
	render_text_on_xy(renderer,"WHITE",100,330,color);
	render_text_on_xy(renderer,"BLACK",350,330,color);
	while(!done){
		
		SDL_RenderPresent(renderer);
		while(SDL_PollEvent(&event)){
			switch(event.type){
				case SDL_MOUSEBUTTONDOWN:
					Mix_PlayChannel(-1,soundeffect,0);
					x=event.motion.x;
					y=event.motion.y;
					if(x<200&&x>100&&y>200&&y<320){
						SDL_DestroyTexture(texture4);
						SDL_DestroyTexture(texture5);
						SDL_DestroyTexture(texture6);
						SDL_DestroyTexture(texture7);
						SDL_DestroyTexture(texture8);
						texture5=NULL;texture6=NULL;texture7=NULL;texture8=NULL;
						SDL_DestroyRenderer(renderer);
						SDL_DestroyWindow(window);
						renderer=NULL;window=NULL;
						return 'w';
					}
					else if(x<450&&x>350&&y>200&&y<320) {
						SDL_DestroyTexture(texture4);
						SDL_DestroyTexture(texture5);
						SDL_DestroyTexture(texture6);
						SDL_DestroyTexture(texture7);
						SDL_DestroyTexture(texture8);
						texture5=NULL;texture6=NULL;texture7=NULL;texture8=NULL;
						SDL_DestroyRenderer(renderer);
						SDL_DestroyWindow(window);
						renderer=NULL;window=NULL;
						return 'b';
					}
					else if(x>570&&x<598&&y>10&&y<38){
						SDL_DestroyTexture(texture4);
						SDL_DestroyTexture(texture5);
						SDL_DestroyTexture(texture6);
						SDL_DestroyTexture(texture7);
						SDL_DestroyTexture(texture8);
						texture5=NULL;texture6=NULL;texture7=NULL;texture8=NULL;
						SDL_DestroyRenderer(renderer);
						SDL_DestroyWindow(window);
						renderer=NULL;window=NULL;
						return 'o';
						done=1;
					}
					break;
				case SDL_KEYDOWN:
					if(event.key.keysym.sym==SDLK_p){
						if(!Mix_PlayingMusic()) Mix_PlayMusic(bgm,-1);
						else if(Mix_PausedMusic()) Mix_ResumeMusic();
						else Mix_PauseMusic();
					}
					else if(event.key.keysym.sym==SDLK_s) {
						Mix_HaltMusic();
					}
					else if(event.key.keysym.sym==SDLK_ESCAPE){
						SDL_DestroyTexture(texture4);
						SDL_DestroyTexture(texture5);
						SDL_DestroyTexture(texture6);
						SDL_DestroyTexture(texture7);
						SDL_DestroyTexture(texture8);
						texture5=NULL;texture6=NULL;texture7=NULL;texture8=NULL;
						SDL_DestroyRenderer(renderer);
						SDL_DestroyWindow(window);
						renderer=NULL;window=NULL;
						return 'o';
						done=1;
					}
					break;
				case SDL_QUIT:
					printf("hellloooooooo\n");
					break;
				default:
					break;		
			}
		}
	}
	SDL_DestroyTexture(texture4);
	SDL_DestroyTexture(texture5);
	SDL_DestroyTexture(texture6);
	SDL_DestroyTexture(texture7);
	SDL_DestroyTexture(texture8);
	texture5=NULL;texture6=NULL;texture7=NULL;texture8=NULL;
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	renderer=NULL;window=NULL;
	return 'o';
	//SDL_Quit();

}


//************************************************************************************************************************************************************
/*la fonction suivante permettra de demander au joueurs de choisir parmis 3 options de charger un jeu, de jouer un jeu ou d'afficher les dix meilleurs scores
s'il choisit de charger une partie alors elle lui demande si 2 humains vont jouer ou bien 2 robots ou bien 1 etre humain et un robot si un etre humain va 
alors elle lui demande de saisir son nom , c'est aussi le cas pour le choix de jouer une nouvelle partie, mais s'il choisi de charger les 10 meilleurs scores
donc elle n'affiche que les 10  meilleurs scores */
void play_game(SDL_Renderer *renderer,SDL_Window *window,Mix_Music *bgm,Mix_Chunk *soundeffect){

	char **table;
	table=creer_table();
	char logo;
	SDL_Event event;
	SDL_Rect rect,rect2;
	SDL_Texture *texture=NULL,*texture1=NULL,*texture2=NULL,*texture3=NULL,*texture4=NULL,*texture5=NULL,*texture6=NULL,*texture7=NULL,*texture8=NULL,*texture9=NULL,*texture10=NULL,*texture11=NULL;
	SDL_Surface *surface=NULL,*surface1=NULL,*surface2=NULL,*surface3=NULL,*surface4=NULL,*surface5=NULL,*surface6=NULL,*surface7=NULL,*surface8=NULL,*surface9=NULL,*surface10=NULL,*surface11=NULL;
	int done=0;
	int x,y,z;


	//if(Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,2048)<0) printf("ERREUR!!!\n");
	//Mix_Music *bgm=Mix_LoadMUS("src/Music-Othello.mp3");
	//Mix_Chunk *soundeffect=Mix_LoadWAV("src/soundeffect.wav");
	surface1=IMG_Load("src/Othello-logo.jpg");
	surface2=IMG_Load("src/jouer.jpg");
	surface3=IMG_Load("src/charger.jpg");
	surface4=IMG_Load("src/score.jpg");
	surface5=IMG_Load("src/background.jpg");
	surface6=IMG_Load("src/sound-icon.jpg");
	surface7=IMG_Load("src/no-sound-icon.png");
	surface8=IMG_Load("src/new-round.jpg");
	surface9=IMG_Load("src/load.jpg");
	surface10=IMG_Load("src/ten-best-scores.jpg");
	surface11=IMG_Load("src/jouer-contre.png");
    texture1= SDL_CreateTextureFromSurface(renderer,surface1);
	texture2= SDL_CreateTextureFromSurface(renderer,surface2);
	texture3= SDL_CreateTextureFromSurface(renderer,surface3);
	texture4= SDL_CreateTextureFromSurface(renderer,surface4);
	texture5= SDL_CreateTextureFromSurface(renderer,surface5);
	texture6= SDL_CreateTextureFromSurface(renderer,surface6);
	texture7= SDL_CreateTextureFromSurface(renderer,surface7);
	texture8= SDL_CreateTextureFromSurface(renderer,surface8);
	texture9= SDL_CreateTextureFromSurface(renderer,surface9);
	texture10= SDL_CreateTextureFromSurface(renderer,surface10);
	texture11= SDL_CreateTextureFromSurface(renderer,surface11);
	SDL_FreeSurface(surface2);
	SDL_FreeSurface(surface1);
	SDL_FreeSurface(surface3);
	SDL_FreeSurface(surface4);
	SDL_FreeSurface(surface5);
	SDL_FreeSurface(surface6);
	SDL_FreeSurface(surface7);
	SDL_FreeSurface(surface8);
	SDL_FreeSurface(surface9);
	SDL_FreeSurface(surface10);
	SDL_FreeSurface(surface11);
	SDL_SetRenderDrawColor(renderer,255,255,102,225);
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer,0,75,4,225);
	rect.x=0;
	rect.y=0;
	rect.h=700;
	rect.w=800;
	SDL_RenderCopy(renderer,texture5,NULL,&rect);

	rect.x=220;
	rect.y=20;
	rect.h=100;
	rect.w=374;
	SDL_RenderCopy(renderer,texture1,NULL,&rect);
	rect.x=150;
	rect.y=400;
	rect.h=100;
	rect.w=100;
	SDL_RenderCopy(renderer,texture2,NULL,&rect);
	rect.x=350;
	rect.y=400;
	rect.h=100;
	rect.w=100;
	SDL_RenderCopy(renderer,texture3,NULL,&rect);
	rect.x=550;
	rect.y=400;
	rect.h=100;
	rect.w=100;
	SDL_RenderCopy(renderer,texture4,NULL,&rect);
	rect.x=150;
	rect.y=506;
	rect.h=30;
	rect.w=100;
	SDL_RenderCopy(renderer,texture8,NULL,&rect);
	rect.x=350;
	rect.y=506;
	rect.h=30;
	rect.w=100;
	SDL_RenderCopy(renderer,texture9,NULL,&rect);
	rect.x=550;
	rect.y=506;
	rect.h=30;
	rect.w=100;
	SDL_RenderCopy(renderer,texture10,NULL,&rect);
	if(!Mix_PlayingMusic()){
		rect.x=760;
		rect.y=20;
		rect.h=38;
		rect.w=38;
		SDL_RenderCopy(renderer,texture7,NULL,&rect);
	}
	else if(Mix_PausedMusic()){
		rect.x=760;
		rect.y=20;
		rect.h=38;
		rect.w=38;
		SDL_RenderCopy(renderer,texture7,NULL,&rect);
	}
	else{
		rect.x=760;
		rect.y=20;
		rect.h=38;
		rect.w=38;
		SDL_RenderCopy(renderer,texture6,NULL,&rect);
	}
	while(!done){
		
		SDL_RenderPresent(renderer);
		while(SDL_PollEvent(&event)){
			switch(event.type){
				case SDL_MOUSEBUTTONDOWN:
					Mix_PlayChannel(-1,soundeffect,0);
					x=event.motion.x;
					y=event.motion.y;
					system("cls");
					if(x<250&&x>150&&y>400&&y<500){
						int a=jouer_contre(renderer,window,bgm,soundeffect);
						if(a==1||a==0){
						logo=select_logo(bgm,soundeffect);
						while(logo=='o'){
							a=jouer_contre(renderer,window,bgm,soundeffect);
							if(a==2) return;
							else logo=select_logo(bgm,soundeffect);
						}
						rect.x=0;
						rect.y=0;
						rect.h=700;
						rect.w=800;
						SDL_RenderCopy(renderer,texture11,NULL,&rect);
						SDL_RenderPresent(renderer);
						char player1[20],player2[20];
						if(a==1){
							puts("donner votre nom:");
							scanf("%s",&player1);
							rewind(stdin);
							puts("le nom du deuxieme joueur");
							scanf("%s",&player2);
							rewind(stdin);
						}
						else if(a==0){
							puts("donner votre nom:");
							scanf("%s",&player1);
							rewind(stdin);
							strcpy(player2,"robot1");

						}
						play_round(logo,player1,player2,table,1,a,renderer,window,bgm,soundeffect,creer_table());
						}
						return;
						done=1;

					}
					else if(x<450&&x>350&&y>400&&y<500){
						int a=jouer_contre(renderer,window,bgm,soundeffect);
						rect.x=0;
						rect.y=0;
						rect.h=700;
						rect.w=800;
						SDL_RenderCopy(renderer,texture11,NULL,&rect);
						SDL_RenderPresent(renderer);
						char player1[20],player2[20];
						if(a==1){
							puts("donner votre nom:");
							scanf("%s",&player1);
							rewind(stdin);
							puts("le nom du deuxieme joueur");
							scanf("%s",&player2);
							rewind(stdin);
						}
						else{
							puts("donner votre nom:");
							scanf("%s",&player1);
							rewind(stdin);
							strcpy(player2,"robot1");

						}
						charger_partie(player1,player2,1,a,renderer,window,bgm,soundeffect);
						return;
						done=1;
					}
					else if(x<650&&x>550&&y>400&&y<500) dix_meilleurs_scores("gagnants.txt");
					else if(x<798&&x>760&&y>20&&y<58) {
							rect.x=760;
							rect.y=20;
							rect.h=38;
							rect.w=38;
							SDL_RenderCopy(renderer,texture7,NULL,&rect);
							//SDL_RenderPresent(renderer);
						if(!Mix_PlayingMusic()){
							Mix_PlayMusic(bgm,-1);
						}
						else if(Mix_PausedMusic()){
							rect.x=760;
							rect.y=20;
							rect.h=38;
							rect.w=38;
							SDL_RenderCopy(renderer,texture6,NULL,&rect);
							SDL_RenderPresent(renderer);
							Mix_ResumeMusic();
						}
						else Mix_PauseMusic();

					}
					break;
				case SDL_KEYDOWN:
					if(event.key.keysym.sym==SDLK_p){
							rect.x=760;
							rect.y=20;
							rect.h=38;
							rect.w=38;
							SDL_RenderCopy(renderer,texture7,NULL,&rect);
							//SDL_RenderPresent(renderer);
							if(!Mix_PlayingMusic()){
								Mix_PlayMusic(bgm,-1);
							}
							else if(Mix_PausedMusic()){
								rect.x=760;
								rect.y=20;
								rect.h=38;
								rect.w=38;
								SDL_RenderCopy(renderer,texture6,NULL,&rect);
								SDL_RenderPresent(renderer);
								Mix_ResumeMusic();
							}
							else Mix_PauseMusic();
					}
					else if(event.key.keysym.sym==SDLK_s) {
						Mix_HaltMusic();
					}
					else if(event.key.keysym.sym==SDLK_ESCAPE){
						done=1;
					}

					break;
				case SDL_QUIT:
					done=1;
					break;
				default:
					break;		
			}
		}
				
	SDL_RenderPresent(renderer);
	SDL_Delay(120);
	}
	//Mix_FreeChunk(soundeffect);
	//Mix_FreeMusic(bgm);
	//bgm=NULL;
	//soundeffect=NULL;
	//Mix_Quit();
	SDL_DestroyTexture(texture1);
	SDL_DestroyTexture(texture2);
	SDL_DestroyTexture(texture3);
	SDL_DestroyTexture(texture4);
	SDL_DestroyTexture(texture5);
	SDL_DestroyTexture(texture6);
	SDL_DestroyTexture(texture7);
	SDL_DestroyTexture(texture8);
	SDL_DestroyTexture(texture9);
	SDL_DestroyTexture(texture10);
	SDL_DestroyTexture(texture11);
	
}
//***********************************************************************************************************************************************************
//cette fonction permettera dau joueur de creer un nouveau compte et prend en compte le cas ou le username exist deja
void creer_compte(SDL_Renderer *renderer,SDL_Window *window,Mix_Music *bgm,Mix_Chunk *soundeffect){
	FILE *fichier;
	FILE *ID;
	char nouuveau_username[40],nouuveau_motdepass[40],buffer[40];
	ID=fopen("identification.txt","a+");
	fichier=fopen("users.txt","a+");
	
	printf("donner le username\t");
	scanf("%s",nouuveau_username);
	rewind(stdin);
	fscanf(fichier,"%s",buffer);
	while(!feof(fichier)){
		if(strcmp(buffer,nouuveau_username)==0){
			printf("username existe deja !\n");
			printf("donner un nouveau username\t");
			scanf("%s",nouuveau_username);
			rewind(stdin);
			fclose(fichier);
			fichier=fopen("users.txt","a+");
			fscanf(fichier,"%s",buffer);
			rewind(stdin);
		}
		else 	fscanf(fichier,"%s",buffer);
		
		
	}
	fprintf(fichier, "%s\n",nouuveau_username );
	int i = 0;
	char hide;
  
   	printf("donner le mot de passe : \n");//on va essayer de masquer les caractere du mot de passe
  	 while((hide = getch()) != 13){
  	     if(i < 0)
  	         i = 0;
  	     if(hide == 8){
   	        putch('\b');
   	        putch(' ');
   	        putch('\b');
   	        i--;
   	    continue;
   	    }
   	    nouuveau_motdepass[i++] = hide;
   	    putch('*');
  	 }
  	nouuveau_motdepass[i] = '\0';
	rewind(stdin);
	printf("\n");
	strcat(nouuveau_username,nouuveau_motdepass);
	fprintf(ID, "%s\n",nouuveau_username );
	Mix_PlayMusic(bgm,-1);
	play_game(renderer,window,bgm,soundeffect);
	return;
	fclose(fichier);
	fclose(ID);
}
void authentification(SDL_Renderer *renderer,SDL_Window *window,Mix_Music *bgm,Mix_Chunk *soundeffect){
	int k=0,length,r,s=0;
	char **tableuID;
	char username[40],buff[40],buff1[100],nouuveau_username[40],nouuveau_motdepass[40];
	char motdepasse[20];
	FILE *fichier,*ID;
	ID=fopen("identification.txt","a+");
	printf("\t\t\t\t\t\tAthentification \n\n\n");
	printf("Username:\n");
	scanf("%s",username);
	rewind(stdin);
	
	fichier=fopen("users.txt","a+");
	
	while(!feof(fichier)){
		fscanf(fichier,"%s\n",buff);
		if(strcmp(buff,username)==0){
		printf("username valid\n");
		int i = 0;
		char hide;
  
 	  	printf("donner le mot de passe : \n");
 	 	 while((hide = getch()) != 13){
  		     if(i < 0)
  		         i = 0;
  		     if(hide == 8){
 	  	        putch('\b');
 	  	        putch(' ');
 	  	        putch('\b');
 	  	        i--;
 	  	    continue;
 	  	    }
 	  	    motdepasse[i++] = hide;
 	  	    putch('*');
 	 	 }
 	 	motdepasse[i] = '\0';
		rewind(stdin);
		printf("\n");
		strcat(username,motdepasse);
		while(!feof(ID)){
			fscanf(ID,"%s\n",buff1);
			if(strcmp(buff1,username)==0){
			Mix_PlayMusic(bgm,-1);
			play_game(renderer,window,bgm,soundeffect);
			return;
			}
			else	k++;
		}
		}

	}
	

if(k!=0){
	printf("mot de passe invalide\n");
	printf("voulez vous creer un nouveau compte? 1:oui 0:non\n");
	scanf("%d",&s);
	rewind(stdin);
	if(s==1){
		creer_compte(renderer,window,bgm,soundeffect);
	}	
	else return;
}	
printf("username n'existe pas\n");
printf("voulez vous creer un compte? 1:oui 2:non\n");
scanf("%d",&r);
rewind(stdin);
if(r==1){
	creer_compte(renderer,window,bgm,soundeffect);
	}

else return;
fclose(fichier);
fclose(ID);
	

}

void donnees_user(SDL_Renderer *renderer,SDL_Window *window,Mix_Music *bgm,Mix_Chunk *soundeffect){
	system("cls");
	int r;
	printf("voulez-vous? 1:se connecter 2:creer un nouveau compte \n");
	scanf("%d",&r);
	rewind(stdin);
	if(r==1)	authentification(renderer,window,bgm,soundeffect);
	else creer_compte(renderer,window,bgm,soundeffect);
}



int main(int argc,char **argv){
	SDL_Window *window=NULL;
	SDL_Renderer *renderer=NULL;
	SDL_Event event;
	SDL_Rect rect;
	SDL_Texture *texture1=NULL;
	SDL_Surface *surface1=NULL;
	int done=0;
	SDL_Init(SDL_INIT_EVERYTHING);
	window=SDL_CreateWindow("Othello",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,widthwindow,heightwindow,0);
	renderer=SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);

	if(Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,2048)<0) printf("ERREUR!!!\n");
	Mix_Music *bgm=Mix_LoadMUS("src/Music-Othello.mp3");
	Mix_Chunk *soundeffect=Mix_LoadWAV("src/soundeffect.wav");

	surface1=IMG_Load("src/sign-in.png");
	texture1= SDL_CreateTextureFromSurface(renderer,surface1);
	SDL_FreeSurface(surface1);
	rect.x=0;
	rect.y=0;
	rect.h=700;
	rect.w=800;
	SDL_RenderCopy(renderer,texture1,NULL,&rect);
	SDL_RenderPresent(renderer);
	donnees_user(renderer,window,bgm,soundeffect);
	SDL_DestroyTexture(texture1);
	Mix_FreeChunk(soundeffect);
	Mix_FreeMusic(bgm);
	bgm=NULL;
	soundeffect=NULL;
	SDL_Delay(30);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
//gcc src/blank.c -o bin/prog -I include -L lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
