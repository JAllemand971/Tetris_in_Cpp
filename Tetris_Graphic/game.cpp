// game.cpp

#include "game.h"
#include "board.h"
#include "piece.h"
#include "audio_manager.h"

Game::Game() {
	std::srand(std::time(nullptr));
	audio = new AudioManager();
    PieceType types[] = {PieceType::I, PieceType::O, PieceType::T, PieceType::L, 
                         PieceType::J, PieceType::S, PieceType::Z};
    PieceType randomType = types[std::rand() % 7];
    
    piece = new Piece(this, nullptr, 4, 0, randomType);
    board = new Board(this, piece, audio, grid_Width, grid_Height);    
    piece->setBoard(board);

	
	score = 0;
    gameOver = false;
    run = true;
    
    fillProgress = 0;
    isFilling = false;
    
    bool once = false;
     
    audio->init();
}

Game::~Game() {
    delete board;
    delete piece;
    delete audio;
}

void Game::start() {	
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    
	audio->init();
	audio->loadAllSounds();
	audio->playMusic("background", -1);
    
    SDL_Window *window = SDL_CreateWindow("Tetris", win_Width, win_Height, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);
	
	   
    while(run){
    	handleEvents();
    	
    	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);	
    	
    	render(renderer);
    	if(gameOver){
    		displayText(renderer, "=== Game Over===", scoreX, scoreY + 40, {255, 0, 0, 255}, 48);
    		displayText(renderer, "Press 'esc' to quit or 'r' to restart", scoreX, scoreY + 120, {255, 255, 255, 255}, 24);
			audio->stopMusic();
			if(!once){
				audio->playSound("gameover");
				once = true;
			}			
		}
        
    	
    	while (!gameOver) {  	
	        handleEvents();
	        update();

	        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	        SDL_RenderClear(renderer);
	        
			render(renderer);
			
	        SDL_RenderPresent(renderer); 
			SDL_Delay(500); 		
    	}	
    	
    	SDL_RenderPresent(renderer);
		
		SDL_Event event;
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}

void Game::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_EVENT_QUIT:
                gameOver = true;
                run = false;
                break;

            case SDL_EVENT_KEY_DOWN:
                switch (event.key.key) {
                    case SDLK_ESCAPE:
                        gameOver = true;
                        run = false;
                        break;
                    case SDLK_R:
						resetGame();
						return;	                         
					case SDLK_A:
						if (!gameOver) piece->movePiece(-1, 0);
						audio->playSound("move");
						break;
					case SDLK_D:
						if (!gameOver) piece->movePiece(1, 0);
						audio->playSound("move");
						break;
					case SDLK_S:
						if (!gameOver) piece->movePiece(0, 1);
						audio->playSound("move");
						break;
					case SDLK_W:
						if (!gameOver) piece->rotatePiece();
						audio->playSound("rotate");
						break;
					default:
						break;						 
            }
			break;                
        }
    }
}

void Game::update() {
	if (!checkCollision(0,1)) 
	{
    	piece->movePiece(0, 1);
	} 
	else
	{
		for (const auto& block : piece->getBlock()) {
            int x = piece->getPieceX() + block.first;
       		int y = piece->getPieceY() + block.second;
      		board->setCell(x, y, piece->getColor());
    	}
		audio->playSound("pieceLanded");
		
		board->clearFullLines();      

		spawnPiece();	
	}
}

void Game::render(SDL_Renderer* renderer) {

    board->draw(renderer);

	if (gameOver) {
        fillGridAnimation(renderer);
    }
    else{
    	piece->draw(renderer);
	}  
    
    displayText(renderer, "Score:", scoreX, scoreY, {255, 255, 255, 255}, 24);
    displayText(renderer, std::to_string(score), scoreX + 80, scoreY, {255, 255, 255, 255}, 24);
}

void Game::spawnPiece() {
    
    delete piece;
    
    PieceType types[] = {PieceType::I, PieceType::O, PieceType::T, PieceType::L, 
                         PieceType::J, PieceType::S, PieceType::Z};
    PieceType randomType = types[std::rand() % 7];
    
    piece = new Piece(this, board, 4, 0, randomType);
    
    if (checkCollision(0, 0)) {  
        gameOver = true;
    }    
}

bool Game::checkCollision(int dx, int dy) {
    for (const auto& block : piece->getBlock()) {
        int newX = piece->getPieceX() + block.first + dx;
        int newY = piece->getPieceY() + block.second + dy;

        if (newX < 0 || newX >= grid_Width ||
			newY < 0 || newY >= grid_Height ||
			!board->isCellEmpty(newX, newY)) {
    		return true;
		}		
    }
    return false;
}

void Game::resetGame(){
	if (board) {
        delete board; 
    }
	board = new Board(this, piece, audio, grid_Width, grid_Height);
	score = 0;
	gameOver = false;
	run = true;
	fillProgress = 0;
	isFilling = false;
	spawnPiece();
}

void Game::displayErrorMessage(const std::string& message) {
    SDL_ShowSimpleMessageBox(
        SDL_MESSAGEBOX_ERROR, 
        "Erreur",             
        message.c_str(),      
        nullptr              
    );
}

void Game::displayText(SDL_Renderer* renderer, std::string t, float x, float y, SDL_Color c, int fontSize) {
	SDL_Color color = c;
	TTF_Font* font = TTF_OpenFont("arial.ttf", fontSize);	
	std::string text = t;
	SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), text.size(), color);
	
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);	
	
	SDL_FRect dest = {x, y, static_cast<float>(surface->w), static_cast<float>(surface->h)};
    SDL_RenderTexture(renderer, texture, NULL, &dest);
	
	SDL_DestroySurface(surface);
    SDL_DestroyTexture(texture);
    TTF_CloseFont(font);
}

void Game::fillGridAnimation(SDL_Renderer* renderer) {
    if (!isFilling) {
        isFilling = true;
        fillProgress = 0;
    }

    if (fillProgress < grid_Width * grid_Height) {
        int x = fillProgress % grid_Width;
        int y = grid_Height - 1 - (fillProgress / grid_Width);

        SDL_Color gray = {128, 128, 128, 255};
        board->setCell(x, y, gray);

        board->draw(renderer);

        fillProgress++;
    } else {

        isFilling = false;
    }
}

