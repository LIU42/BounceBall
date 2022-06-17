#include "bounceball.h"

using namespace std;

SDL_RWops* Window::getResource(HINSTANCE hInst, LPCSTR name, LPCSTR type)
{
    HRSRC hRsrc = FindResource(hInst, name, type);
    DWORD size = SizeofResource(hInst, hRsrc);
    LPVOID data = LockResource(LoadResource(hInst, hRsrc));
    return SDL_RWFromConstMem(data, size);
}

SDL_Surface* Window::loadSurface(DWORD ID)
{
    SDL_RWops* src = getResource(hInstance, MAKEINTRESOURCE(ID), TEXT("PNG"));
    SDL_Surface* originImage = IMG_LoadPNG_RW(src);
    SDL_Surface* convertImage = SDL_ConvertSurface(originImage, format, NULL);
    SDL_FreeSurface(originImage);
    SDL_FreeRW(src);
    return convertImage;
}

void Window::text(const char* text, TTF_Font* font, int x, int y)
{
    SDL_Surface* textSurface = TTF_RenderText_Blended(font, text, TEXT_COLOR);
    SDL_Rect textRect = { x, y, TEXT_RECT_WIDTH, TEXT_RECT_HEIGHT };
    SDL_BlitSurface(textSurface, NULL, surface, &textRect);
    SDL_FreeSurface(textSurface);
}

void Window::init()
{
    SDL_Init(SDL_INIT_EVERYTHING);
    hInstance = GetModuleHandle(0);
    window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    surface = SDL_GetWindowSurface(window);
    format = SDL_AllocFormat(IMG_FORMAT);
    screenRect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
}

void Window::loadImage()
{
    plankImg = loadSurface(IDB_PNG1);
    ballImg = loadSurface(IDB_PNG2);
    blockImg = loadSurface(IDB_PNG3);
}

void Window::freeImage()
{
    SDL_FreeSurface(plankImg);
    SDL_FreeSurface(ballImg);
    SDL_FreeSurface(blockImg);
}

void Window::loadFont()
{
    TTF_Init();
    titleFont = TTF_OpenFontRW(getResource(hInstance, MAKEINTRESOURCE(IDR_FONT1), RT_FONT), 1, TITLE_FONT_SIZE);
    infoFont = TTF_OpenFontRW(getResource(hInstance, MAKEINTRESOURCE(IDR_FONT1), RT_FONT), 1, INFO_FONT_SIZE);
}

void Window::freeFont()
{
    TTF_CloseFont(titleFont);
    TTF_CloseFont(infoFont);
}

void Window::close()
{
    SDL_DestroyWindow(window);
    SDL_FreeFormat(format);
    freeImage();
    freeFont();
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}

Game::Game() : random(unsigned(time(NULL))), randX(BALL_INIT_MARGIN, SCREEN_WIDTH - BALL_INIT_MARGIN) { bestScore = 0; }

void Game::init()
{
    status = START;
    score = 0;
    hitBlockCount = 0;
    plank.init();
    ball.init();
    initBlock();
}

void Game::initBlock()
{
    for (int x = 0; x < BLOCK_ROWS; x++)
    {
        for (int y = 0; y < BLOCK_COLS; y++)
        {
            block[x][y].init(x * BLOCK_SIZE + WINDOW_BORDER_X, y * BLOCK_SIZE + WINDOW_BORDER_Y);
        }
    }
}

void Game::levelUp() { ball.speed = BALL_INIT_SPEED + score / LEVEL_UP_STEP; }

void Game::gameover()
{
    if (ball.rect.y >= SCREEN_HEIGHT + GAMEOVER_DELAY_DISTANCE)
    {
        if (score > bestScore) { bestScore = score; }
        status = OVER;
    }
    else if (hitBlockCount == BLOCK_ROWS * BLOCK_COLS)
    {
        if (score > bestScore) { bestScore = score; }
        status = WIN;
    }
}

void Game::update()
{
    if (status == PLAYING)
    {
        levelUp();
        gameover();
        ball.move();
        ball.reflectOnPlank();
        ball.reflectOnEdge();
        ball.reflectOnBlock();
    }
}

void Game::event()
{
    while (SDL_PollEvent(&window.event))
    {
        if (window.event.type == SDL_QUIT) { game.status = EXIT; }
        if (window.event.type == SDL_MOUSEBUTTONDOWN)
        {
            switch (status)
            {
                case START: status = PLAYING; break;
                case PAUSE: status = PLAYING; break;
                case WIN: case OVER: init(); status = PLAYING; break;
            }
        }
        if (window.event.type == SDL_MOUSEMOTION && status == PLAYING) { plank.move(window.event.motion.x); }
        if (window.event.type == SDL_KEYDOWN && window.event.key.keysym.sym == SDLK_p && status == PLAYING) { status = PAUSE; }
    }
}

void Game::displayImage()
{
    if (status == PLAYING || status == PAUSE)
    {
        for (int x = 0; x < BLOCK_ROWS; x++)
        {
            for (int y = 0; y < BLOCK_COLS; y++)
            {
                block[x][y].display();
            }
        }
        plank.display();
        ball.display();
    }
}

void Game::displayInfo()
{
    if (status == START)
    {
        window.text("BOUNCE BALL", window.titleFont, (SCREEN_WIDTH - MAIN_TITLE_LENGTH) / 2, TITLE_POSITION);
        window.text("Click anywhere to START...", window.infoFont, (SCREEN_WIDTH - INFO_LENGTH) / 2, INFO_POSITION);
    }
    else if (status == PLAYING || status == PAUSE)
    {
        SDL_snprintf(text, TEXT_MAX_LEN, "Score: %d", score);
        window.text(text, window.infoFont, SCREEN_WIDTH - TEXT_BORDER - TEXT_RECT_WIDTH, SCREEN_HEIGHT - INFO_FONT_SIZE - TEXT_BORDER);
        if (status == PAUSE) { window.text("PAUSE", window.infoFont, TEXT_BORDER, SCREEN_HEIGHT - INFO_FONT_SIZE - TEXT_BORDER); }
    }
    else if (status == OVER || status == WIN)
    {
        switch (status)
        {
            case OVER: SDL_snprintf(text, TEXT_MAX_LEN, "GAMEOVER!"); break;
            case WIN: SDL_snprintf(text, TEXT_MAX_LEN, "YOU WIN!"); break;
        }
        window.text(text, window.titleFont, (SCREEN_WIDTH - OVER_TITLE_LENGTH) / 2, TITLE_POSITION);
        SDL_snprintf(text, TEXT_MAX_LEN, "Your score: %d", score);
        window.text(text, window.infoFont, (SCREEN_WIDTH - SCORE_LENGTH) / 2, SCORE_POSITION);
        SDL_snprintf(text, TEXT_MAX_LEN, "Best score: %d", bestScore);
        window.text(text, window.infoFont, (SCREEN_WIDTH - SCORE_LENGTH) / 2, BEST_SCORE_POSITION);
        window.text("Click anywhere to RESTART...", window.infoFont, (SCREEN_WIDTH - INFO_LENGTH) / 2, INFO_POSITION);
    }
}

void Game::display()
{
    SDL_FillRect(window.surface, &window.screenRect, BACKGROUND_COLOR);
    displayImage();
    displayInfo();
    SDL_UpdateWindowSurface(window.window);
}

void Plank::init() { rect = { (SCREEN_WIDTH - PLANK_WIDTH) / 2, SCREEN_HEIGHT - PLANK_POSITION, PLANK_WIDTH, PLANK_HEIGHT }; }

void Plank::move(int mouseX)
{
    if (mouseX > PLANK_WIDTH / 2 && mouseX < SCREEN_WIDTH - PLANK_WIDTH / 2) { plank.rect.x = mouseX - PLANK_WIDTH / 2; }
    else if (mouseX <= PLANK_WIDTH / 2) { plank.rect.x = 0; }
    else if (mouseX >= SCREEN_WIDTH - PLANK_WIDTH / 2) { plank.rect.x = SCREEN_WIDTH - PLANK_WIDTH; }
}

void Plank::display() { SDL_BlitSurface(window.plankImg, NULL, window.surface, &rect); }

void Ball::init()
{
    switch (game.randSign(game.random))
    {
        case 1: signX = 1; break;
        case 0: signX = -1; break;
    }
    signY = -1;
    speed = BALL_INIT_SPEED;
    rect = { game.randX(game.random), BALL_INIT_POSITION, BALL_RADIUS * 2, BALL_RADIUS * 2 };
}

void Ball::move()
{
    rect.x += signX * speed;
    rect.y += signY * speed;
}

void Ball::reflectOnPlank()
{
    if (rect.y >= BALL_INIT_POSITION && rect.y < BALL_INIT_POSITION + speed)
    {
        if (rect.x >= plank.rect.x - BALL_RADIUS - speed && rect.x <= plank.rect.x + PLANK_WIDTH + speed - BALL_RADIUS)
        {
            signY = -signY;
            game.score += CATCH_SCORE;
        }
    }
}

void Ball::reflectOnEdge()
{
    if (rect.x <= 0 || rect.x >= SCREEN_WIDTH - 2 * BALL_RADIUS) { signX = -signX; }
    if (rect.y <= 0) { signY = -signY; }
}

void Ball::reflectOnBlock()
{
    bool isReflectX = false;
    bool isReflectY = false;

    for (int x = 0; x < BLOCK_ROWS; x++)
    {
        for (int y = 0; y < BLOCK_COLS; y++)
        {
            if (block[x][y].alive)
            {
                int distanceX = (rect.x + int(BALL_RADIUS)) - (block[x][y].rect.x + int(BLOCK_SIZE) / 2);
                int distanceY = (rect.y + int(BALL_RADIUS)) - (block[x][y].rect.y + int(BLOCK_SIZE) / 2);

                int absDistanceX = SDL_abs(distanceX);
                int absDistanceY = SDL_abs(distanceY);

                if (absDistanceX > BALL_RADIUS + BLOCK_SIZE / 2) { break; }
                if (SDL_pow(absDistanceX, 2) + SDL_pow(absDistanceY, 2) <= SDL_pow(BALL_RADIUS + BLOCK_SIZE / 2, 2))
                {
                    if (absDistanceX == absDistanceY)
                    {
                        if (!isReflectX) { signX = distanceX / absDistanceX; isReflectX = true; }
                        if (!isReflectY) { signY = distanceY / absDistanceY; isReflectY = true; }
                    }
                    else if (absDistanceX < absDistanceY && !isReflectY) { signY = -signY; isReflectY = true; }
                    else if (absDistanceX > absDistanceY && !isReflectX) { signX = -signX; isReflectX = true; }

                    block[x][y].alive = false;
                    game.score += HIT_SCORE;
                    game.hitBlockCount += 1;
                }
            }
        }
    }
}

void Ball::display() { SDL_BlitSurface(window.ballImg, NULL, window.surface, &rect); }

void Block::init(int x, int y)
{
    rect = { x, y, BLOCK_SIZE, BLOCK_SIZE };
    alive = true;
}

void Block::display() { if (alive) { SDL_BlitSurface(window.blockImg, NULL, window.surface, &rect); } }