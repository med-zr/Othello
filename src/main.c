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
#define MENU_STATE 0 // 1st menu
#define NOT_SELECTED 0
#define MENU_ITEMS_H_

#define INPUT_S_ID 1
#define INPUT_S_PASSWD 2
//login input from user
typedef struct {
	char id[25];
	char passwd[25];
	int score;
}login_txt_;

//sing up input
typedef struct {
	char id[25];
	char passwd[25];
	int score;
} singup_txt_;
// menu : 
typedef struct {
	int select;
	int hover;
	int input_state;
} menu;

void render_text_on_xy(SDL_Renderer* renderer,const char* text, int x,int y,SDL_Color color){
    TTF_Init();
    TTF_Font * font = TTF_OpenFont("src/Redressed-Regular.ttf", 30);
    SDL_Rect dest;
    dest.x = x; dest.y = y;
    SDL_Surface * surface = TTF_RenderText_Blended(font, text, color);
    SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);
    SDL_RenderCopy(renderer, texture, NULL, &dest);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
    TTF_CloseFont(font);
    TTF_Quit();
}
void render_on_xy(const char* file_name,SDL_Renderer *renderer,int x,int y){
    SDL_Texture * texture = initialize_texture_from_file(file_name, renderer);
    int height;int width;
    SDL_QueryTexture(texture, NULL, NULL, &width, &height);
    SDL_Rect destination;
    destination.x = x;
    destination.y = y;
    destination.w = width;
    destination.h = height;
    SDL_RenderCopy(renderer,texture, NULL, &destination);
    SDL_DestroyTexture(texture);
}

void login_txt_input(SDL_Renderer* renderer,char* text){
    SDL_StartTextInput();
 
    SDL_Event event;
    while(SDL_PollEvent(&event)){
        switch (event.type){
        	case SDL_QUIT:
               // running = false; break;
            case SDL_TEXTINPUT: // Get input from keyboard
                if (strlen(text) < 25) strcat(text, event.text.text);
                    break;
            case SDL_KEYDOWN: // Spesial keys cases
                switch (event.key.keysym.scancode){ 
                    case SDL_SCANCODE_ESCAPE: // go back
                       	menu.select=NOT_SELECTED;
                        reset_input();
                       	menu.input_state = INPUT_S_ID;
                        break;
                    case SDL_SCANCODE_BACKSPACE: // del last element :
                        text[strlen(text)-1] = 0;
                        break;
                    case SDL_SCANCODE_RETURN:
                      	 menu.input_state = INPUT_S_PASSWD;
                        SDL_StopTextInput();
                        break;

                }
        }
    }
    SDL_Color black;
    black.r=0;
    black.g=0;
    black.b=0;
        render_text_on_xy(renderer,text,340,315,black);
}

void render_login(SDL_Renderer *renderer){
    render_on_xy(LOGIN_BG,renderer,0,0);
    render_on_xy(LOG_IN,renderer,329,479);
    switch (menu.input_state){
        case INPUT_S_ID :
            login_txt_input(renderer,login_data.id);
        case INPUT_S_PASSWD :
            login_passwd_input(renderer,login_data.passwd);
	
    // hover        
    int mouse_x, mouse_y;
    SDL_GetMouseState(&mouse_x, &mouse_y);
    if (mouse_x > 329 && mouse_x < 329 + BOTTON_H && mouse_y > 479 && mouse_y < 479 + BOTTON_W)
    { render_on_xy(LOG_IN,renderer,329+4,479);}
    }

int main(int argc,char **argv){
	SDL_Window *window=NULL;
	SDL_Renderer *renderer=NULL;
	SDL_Event event;
	int done=0;
	SDL_Init(SDL_INIT_EVERYTHING);
	window=SDL_CreateWindow("Othello",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,widthwindow,heightwindow,0);
	renderer=SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
	render_login(renderer);
	SDL_Delay(60);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}