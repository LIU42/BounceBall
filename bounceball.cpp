#include "bounceball.h"

using namespace std;

SDL_RWops* MainGame::getResource(HINSTANCE hInst, LPCSTR name, LPCSTR type)
{
    HRSRC hRsrc = FindResource(hInst, name, type);
    DWORD size = SizeofResource(hInst, hRsrc);
    LPVOID data = LockResource(LoadResource(hInst, hRsrc));
    return SDL_RWFromConstMem(data, size);
}

SDL_Surface* MainGame::loadSurface(WORD ID)
{
    SDL_RWops* src = getResource(hInstance, MAKEINTRESOURCE(ID), TEXT("PNG"));
    SDL_Surface* originImage = IMG_LoadPNG_RW(src);
    SDL_Surface* convertImage = SDL_ConvertSurface(originImage, image.format, NULL);
    SDL_FreeSurface(originImage);
    SDL_FreeRW(src);
    return convertImage;
}

void MainGame::initWindow()
{
    SDL_Init(SDL_INIT_EVERYTHING);
    hInstance = GetModuleHandle(0);
    window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_GetWindowSize(window, &screen.w, &screen.h);
}

void MainGame::initGame()
{
    status = START;
    score = 0;
    hitCount = 0;
    plank.init();
    ball.init();
    initBlock();
}

void MainGame::initBlock()
{
    for (int x = 0; x < BLOCK_ROWS; x++)
    {
        for (int y = 0; y < BLOCK_COLS; y++)
        {
            block[x][y].init(x * BLOCK_SIZE + WINDOW_BORDER_X, y * BLOCK_SIZE + WINDOW_BORDER_Y);
        }
    }
}

void MainGame::loadImage()
{
    image.format = SDL_AllocFormat(IMG_FORMAT);
    image.surface = SDL_GetWindowSurface(window);
    image.plank = loadSurface(IDB_PNG1);
    image.ball = loadSurface(IDB_PNG2);
    image.block = loadSurface(IDB_PNG3);
}

void MainGame::freeImage()
{
    SDL_FreeFormat(image.format);
    SDL_FreeSurface(image.plank);
    SDL_FreeSurface(image.ball);
    SDL_FreeSurface(image.block);
}

void MainGame::loadFont()
{
    TTF_Init();
    font.title = TTF_OpenFontRW(getResource(hInstance, MAKEINTRESOURCE(IDR_FONT1), RT_FONT), 1, TITLE_FONT_SIZE);
    font.info = TTF_OpenFontRW(getResource(hInstance, MAKEINTRESOURCE(IDR_FONT1), RT_FONT), 1, INFO_FONT_SIZE);
}

void MainGame::freeFont()
{
    TTF_CloseFont(font.title);
    TTF_CloseFont(font.info);
}

void MainGame::close()
{
    SDL_DestroyWindow(window);
    freeImage();
    freeFont();
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}

void MainGame::levelUp() { ball.speed = BALL_INIT_SPEED + score / LEVEL_UP_STEP; }

void MainGame::gameover()
{
    if (ball.rect.y >= SCREEN_HEIGHT + GAMEOVER_DELAY_DISTANCE)
    {
        if (score > bestScore) { bestScore = score; }
        status = OVER;
    }
    else if (hitCount == BLOCK_ROWS * BLOCK_COLS)
    {
        if (score > bestScore) { bestScore = score; }
        status = WIN;
    }
}

void MainGame::update()
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

void MainGame::events()
{
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT) { status = EXIT; }
        if (event.type == SDL_MOUSEBUTTONDOWN)
        {
            switch (status)
            {
                case START: status = PLAYING; break;
                case PAUSE: status = PLAYING; break;
                case WIN: case OVER: initGame(); status = PLAYING; break;
            }
        }
        if (event.type == SDL_MOUSEMOTION && status == PLAYING) { plank.move(event.motion.x); }
        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_p && status == PLAYING) { status = PAUSE; }
    }
}

void MainGame::displayText(char* text, TTF_Font* font, int x, int y)
{
    SDL_Surface* surface = TTF_RenderText_Blended(font, text, TEXT_COLOR);
    SDL_Rect rect = { x, y, TEXT_RECT_WIDTH, TEXT_RECT_HEIGHT };
    SDL_BlitSurface(surface, NULL, image.surface, &rect);
    SDL_FreeSurface(surface);
}

void MainGame::displayImage()
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

void MainGame::displayInfo()
{
    static char text[TEXT_MAX_LEN];

    if (status == START)
    {
        displayText("BOUNCE BALL", font.title, (SCREEN_WIDTH - MAIN_TITLE_LENGTH) / 2, TITLE_POSITION);
        displayText("Click anywhere to START...", font.info, (SCREEN_WIDTH - INFO_LENGTH) / 2, INFO_POSITION);
    }
    else if (status == PLAYING || status == PAUSE)
    {
        SDL_snprintf(text, TEXT_MAX_LEN, "Score: %d", score);
        displayText(text, font.info, SCREEN_WIDTH - TEXT_BORDER - TEXT_RECT_WIDTH, SCREEN_HEIGHT - INFO_FONT_SIZE - TEXT_BORDER);
        if (status == PAUSE) { displayText("PAUSE", font.info, TEXT_BORDER, SCREEN_HEIGHT - INFO_FONT_SIZE - TEXT_BORDER); }
    }
    else if (status == OVER || status == WIN)
    {
        switch (status)
        {
            case OVER: SDL_snprintf(text, TEXT_MAX_LEN, "GAMEOVER!"); break;
            case WIN: SDL_snprintf(text, TEXT_MAX_LEN, "YOU WIN!"); break;
        }
        displayText(text, font.title, (SCREEN_WIDTH - OVER_TITLE_LENGTH) / 2, TITLE_POSITION);
        SDL_snprintf(text, TEXT_MAX_LEN, "Your score: %d", score);
        displayText(text, font.info, (SCREEN_WIDTH - SCORE_LENGTH) / 2, SCORE_POSITION);
        SDL_snprintf(text, TEXT_MAX_LEN, "Best score: %d", bestScore);
        displayText(text, font.info, (SCREEN_WIDTH - SCORE_LENGTH) / 2, BEST_SCORE_POSITION);
        displayText("Click anywhere to RESTART...", font.info, (SCREEN_WIDTH - INFO_LENGTH) / 2, INFO_POSITION);
    }
}

void MainGame::display()
{
    SDL_FillRect(image.surface, &screen, BACKGROUND_COLOR);
    displayImage();
    displayInfo();
    SDL_UpdateWindowSurface(window);
}

void Plank::init() { rect = { (SCREEN_WIDTH - PLANK_WIDTH) / 2, SCREEN_HEIGHT - PLANK_POSITION, PLANK_WIDTH, PLANK_HEIGHT }; }

void Plank::move(int mouseX)
{
    if (mouseX > PLANK_WIDTH / 2 && mouseX < SCREEN_WIDTH - PLANK_WIDTH / 2) { rect.x = mouseX - PLANK_WIDTH / 2; }
    else if (mouseX <= PLANK_WIDTH / 2) { rect.x = 0; }
    else if (mouseX >= SCREEN_WIDTH - PLANK_WIDTH / 2) { rect.x = SCREEN_WIDTH - PLANK_WIDTH; }
}

void Plank::display() { SDL_BlitSurface(game.image.plank, NULL, game.image.surface, &rect); }

void Ball::init()
{
    switch (rand() % 2)
    {
        case 1: signX = 1; break;
        case 0: signX = -1; break;
    }
    signY = -1;
    speed = BALL_INIT_SPEED;
    rect = { rand() % (SCREEN_HEIGHT - 2 * BALL_INIT_MARGIN) + BALL_INIT_MARGIN, BALL_INIT_POSITION, BALL_RADIUS * 2, BALL_RADIUS * 2 };
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
        if (rect.x >= game.plank.rect.x - BALL_RADIUS - speed && rect.x <= game.plank.rect.x + PLANK_WIDTH + speed - BALL_RADIUS)
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
            if (game.block[x][y].alive)
            {
                int distanceX = (rect.x + int(BALL_RADIUS)) - (game.block[x][y].rect.x + int(BLOCK_SIZE) / 2);
                int distanceY = (rect.y + int(BALL_RADIUS)) - (game.block[x][y].rect.y + int(BLOCK_SIZE) / 2);

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

                    game.block[x][y].alive = false;
                    game.score += HIT_SCORE;
                    game.hitCount += 1;
                }
            }
        }
    }
}

void Ball::display() { SDL_BlitSurface(game.image.ball, NULL, game.image.surface, &rect); }

void Block::init(int x, int y)
{
    rect = { x, y, BLOCK_SIZE, BLOCK_SIZE };
    alive = true;
}

void Block::display() { if (alive) { SDL_BlitSurface(game.image.block, NULL, game.image.surface, &rect); } }