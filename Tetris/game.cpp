// game.cpp

#include "game.h"       // Includes the Game class which handles the game logic
#include "board.h"      // Includes the Board class which represents the game grid
#include "piece.h"      // Includes the Piece class which represents the Tetris pieces
#include "audio_manager.h" // Includes the AudioManager class for managing sounds and music

Game::Game() {
    std::srand(std::time(nullptr));  // Initialize random number generator based on the current time
    audio = new AudioManager();      // Initialize the audio manager
    
    // Define the different types of Tetris pieces
    PieceType types[] = {PieceType::I, PieceType::O, PieceType::T, PieceType::L, 
                         PieceType::J, PieceType::S, PieceType::Z};
    PieceType randomType = types[std::rand() % 7];  // Select a random piece type
    
    piece = new Piece(this, nullptr, 4, 0, randomType);  // Create a new piece with the random type
    board = new Board(this, piece, audio, grid_Width, grid_Height);  // Create the game board and pass the piece and audio manager
    piece->setBoard(board);  // Set the board for the piece

    // Initialize other game parameters
    score = 0;
    speed = 500;
    gameOver = false;
    run = true;
    fillProgress = 0;
    isFilling = false;
    once = false;

    // Initialize audio, load sounds, and play background music
    audio->init();
    audio->loadAllSounds();
    audio->playMusic("background", -1);
}

Game::~Game() {
    delete board;  // Clean up dynamically allocated board object
    delete piece;  // Clean up dynamically allocated piece object
    delete audio;  // Clean up dynamically allocated audio manager
}

void Game::start() {    
    SDL_Init(SDL_INIT_VIDEO);  // Initialize SDL video subsystem
    TTF_Init();  // Initialize SDL_ttf library for font rendering
    
    // Create a new SDL window and renderer
    SDL_Window *window = SDL_CreateWindow("Tetris", win_Width, win_Height, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);

    // Main game loop
    while(run) {
        handleEvents();  // Handle user input (events)
        
        // Clear the screen and set drawing color
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);    

        render(renderer);  // Render the game state
        
        // If the game is over, display "Game Over" and the restart message
        if(gameOver) {
            displayText(renderer, "=== Game Over===", scoreX, scoreY + 40, {255, 0, 0, 255}, 48);
            displayText(renderer, "Press 'esc' to quit or 'r' to restart", scoreX, scoreY + 120, {255, 255, 255, 255}, 24);
            audio->stopMusic();  // Stop the background music
            if(!once) {
                audio->playSound("gameover");  // Play the game over sound once
                once = true;
            }
        }
        
        // Game loop continues until the game is over
        while (!gameOver) {   
            handleEvents();  // Handle events during game play
            update();  // Update the game state (piece movement, collisions, etc.)

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);  // Set the background color
            SDL_RenderClear(renderer);  // Clear the screen

            render(renderer);  // Render the current game state

            SDL_RenderPresent(renderer);  // Present the rendered frame to the screen
            SDL_Delay(speed);  // Delay to control game speed
        }    
        
        SDL_RenderPresent(renderer);  // Present the last frame when game is over

        SDL_Event event;  // Event variable to capture events
    }

    // Clean up SDL resources
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}

void Game::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {  // Poll all events
        switch (event.type) {
            case SDL_EVENT_QUIT:  // Handle window close event
                gameOver = true;
                run = false;
                break;

            case SDL_EVENT_KEY_DOWN:  // Handle key press events
                switch (event.key.key) {
                    case SDLK_ESCAPE:  // Escape key to quit
                        gameOver = true;
                        run = false;
                        break;
                    case SDLK_R:  // 'R' key to reset the game
                        resetGame();
                        return;  // Exit handle events function after reset
                    case SDLK_A:  // 'A' key to move piece left
                        if (!gameOver) piece->movePiece(-1, 0);
                        audio->playSound("move");
                        break;
                    case SDLK_D:  // 'D' key to move piece right
                        if (!gameOver) piece->movePiece(1, 0);
                        audio->playSound("move");
                        break;
                    case SDLK_S:  // 'S' key to move piece down
                        if (!gameOver) piece->movePiece(0, 1);
                        audio->playSound("move");
                        break;
                    case SDLK_W:  // 'W' key to rotate piece
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
    if (!checkCollision(0,1)) {  // Check if moving the piece down is possible
        piece->movePiece(0, 1);  // Move piece down
    } else {
        // If piece lands, set blocks on the board and handle line clearing
        for (const auto& block : piece->getBlock()) {
            int x = piece->getPieceX() + block.first;
            int y = piece->getPieceY() + block.second;
            board->setCell(x, y, piece->getColor());  // Set the color of the block on the board
        }
        audio->playSound("pieceLanded");  // Play sound when piece lands
        updateSpeed();  // Update the speed based on the score
        
        board->clearFullLines();  // Clear any full lines on the board

        spawnPiece();  // Spawn a new piece
    }
}

void Game::render(SDL_Renderer* renderer) {
    board->draw(renderer);  // Draw the game board

    if (gameOver) {
        fillGridAnimation(renderer);  // Fill the grid with animation if the game is over
    } else {
        piece->draw(renderer);  // Draw the current piece
    }

    // Display score on the screen
    displayText(renderer, "Score:", scoreX, scoreY, {255, 255, 255, 255}, 24);
    displayText(renderer, std::to_string(score), scoreX + 80, scoreY, {255, 255, 255, 255}, 24);
}

void Game::spawnPiece() {
    // Delete old piece and create a new one
    if (piece) {
        delete piece;
        piece = nullptr;
    }
    
    // Create a new random piece
    PieceType types[] = {PieceType::I, PieceType::O, PieceType::T, PieceType::L, 
                         PieceType::J, PieceType::S, PieceType::Z};
    PieceType randomType = types[std::rand() % 7];
    piece = new Piece(this, board, 4, 0, randomType);

    // If the piece cannot spawn due to collision, game is over
    if (checkCollision(0, 0)) {  
        gameOver = true;
    }    
}

bool Game::checkCollision(int dx, int dy) {
    // Check if moving the piece by (dx, dy) will cause a collision
    for (const auto& block : piece->getBlock()) {
        int newX = piece->getPieceX() + block.first + dx;
        int newY = piece->getPieceY() + block.second + dy;

        if (newX < 0 || newX >= grid_Width ||
            newY < 0 || newY >= grid_Height ||
            !board->isCellEmpty(newX, newY)) {
            return true;  // Collision detected
        }       
    }
    return false;  // No collision
}

void Game::resetGame() {
    // Delete old piece and board, and create new ones
    if (piece) {
        delete piece;
        piece = nullptr;
    }
    
    if (board) {
        delete board;
    }

    board = new Board(this, piece, audio, grid_Width, grid_Height);  // Create new board
    spawnPiece();  // Spawn a new piece
    
    // Reset game state
    score = 0;
    speed = 500;
    gameOver = false;
    run = true;
    once = false;
    fillProgress = 0;
    isFilling = false;

    audio->playMusic("background", -1);  // Play background music
}

void Game::displayErrorMessage(const std::string& message) {
    // Display an error message box
    SDL_ShowSimpleMessageBox(
        SDL_MESSAGEBOX_ERROR, 
        "Error",             
        message.c_str(),      
        nullptr              
    );
}

void Game::displayText(SDL_Renderer* renderer, std::string t, float x, float y, SDL_Color c, int fontSize) {
    SDL_Color color = c;
    TTF_Font* font = TTF_OpenFont("arial.ttf", fontSize);  // Load font
    std::string text = t;
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), text.size(), color);  // Render text to surface
    
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);  // Create texture from surface
    
    // Set the destination rectangle for the text
    SDL_FRect dest = {x, y, static_cast<float>(surface->w), static_cast<float>(surface->h)};
    SDL_RenderTexture(renderer, texture, NULL, &dest);  // Render the text
    
    SDL_DestroySurface(surface);  // Clean up surface
    SDL_DestroyTexture(texture);  // Clean up texture
    TTF_CloseFont(font);  // Close the font
}

void Game::fillGridAnimation(SDL_Renderer* renderer) {
    // Animate filling the grid with color when the game is over
    if (!isFilling) {
        isFilling = true;
        fillProgress = 0;
    }

    if (fillProgress < grid_Width * grid_Height) {
        int x = fillProgress % grid_Width;
        int y = grid_Height - 1 - (fillProgress / grid_Width);

        SDL_Color gray = {128, 128, 128, 255};
        board->setCell(x, y, gray);  // Set gray color for the cell

        board->draw(renderer);  // Draw the updated grid

        fillProgress++;  // Increment the fill progress
    } else {
        isFilling = false;  // End the filling animation
    }
}

void Game::updateSpeed() {
    // Adjust the game speed based on the score
    if (score >= 500) speed = 400;
    if (score >= 1000) speed = 300;
    if (score >= 2000) speed = 200;
    if (score >= 5000) speed = 100;
}

