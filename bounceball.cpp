#include "bounceball.h"

SDL_RWops* MainGame::getResource(HINSTANCE hInst, LPCSTR name, LPCSTR type)
{
    HRSRC hRsrc = FindResource(hInst, name, type);
    DWORD size = SizeofResource(hInst, hRsrc);
    LPVOID data = LockResource(LoadResource(hInst, hRsrc));
    return SDL_RWFromConstMem(data, size);
}

SDL_Surface* MainGame::loadSurface(int id)
{
    SDL_RWops* src = getResource(hInstance, MAKEINTRESOURCE(id), TEXT("PNG"));
    SDL_Surface* originSurface = IMG_LoadPNG_RW(src);
    SDL_Surface* convertSurface = SDL_ConvertSurface(originSurface, image.format, NULL);
    SDL_FreeSurface(originSurface);
    SDL_FreeRW(src);
    return convertSurface;
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
    static Point point;

    for (int x = 0; x < BLOCK_ROWS; x++)
    {
        for (int y = 0; y < BLOCK_COLS; y++)
        {
            point.x = x * BLOCK_SIZE + WINDOW_BORDER_X;
            point.y = y * BLOCK_SIZE + WINDOW_BORDER_Y;
            block[x][y].init(point);
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

void MainGame::levelUp()
{
    ball.speed = BALL_INIT_SPEED + score / LEVEL_UP_STEP;
}

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

void MainGame::control()
{
    while (SDL_PollEvent(&events))
    {
        if (events.type == SDL_QUIT) { status = EXIT; }
        if (events.type == SDL_MOUSEBUTTONDOWN)
        {
            switch (status)
            {
                case START: status = PLAYING; break;
                case PAUSE: status = PLAYING; break;
                case WIN: case OVER: initGame(); status = PLAYING; break;
            }
        }
        if (events.type == SDL_MOUSEMOTION && status == PLAYING) { plank.move(events.motion.x); }
        if (events.type == SDL_KEYDOWN && events.key.keysym.sym == SDLK_p && status == PLAYING) { status = PAUSE; }
    }
}

void MainGame::displayText(const char* text, Point point, TTF_Font* font)
{
    static SDL_Surface* textSurface;
    static SDL_Rect textRect;

    textSurface = TTF_RenderText_Blended(font, text, TEXT_COLOR);
    textRect.x = point.x;
    textRect.y = point.y;

    SDL_BlitSurface(textSurface, NULL, image.surface, &textRect);
    SDL_FreeSurface(textSurface);
}

void MainGame::displayImage()
{
    if (status == PLAYING || status == PAUSE)
    {
        for (int x = 0; x < BLOCK_ROWS; x++)
        {
            for (int y = 0; y < BLOCK_COLS; y++)
            {
                if (block[x][y].alive)
                {
                    block[x][y].display();
                }
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
        displayText("BOUNCE BALL", { (SCREEN_WIDTH - MAIN_TITLE_WIDTH) / 2, TITLE_POSITION }, font.title);
        displayText("Click anywhere to START...", { (SCREEN_WIDTH - INFO_WIDTH) / 2, INFO_POSITION }, font.info);
    }
    else if (status == PLAYING || status == PAUSE)
    {
        SDL_snprintf(text, TEXT_MAX_LEN, "Score: %d", score);
        displayText(text, { SCREEN_WIDTH - TEXT_BORDER - PLAYING_SCORE_WIDTH, SCREEN_HEIGHT - INFO_FONT_SIZE - TEXT_BORDER }, font.info);
        if (status == PAUSE) { displayText("PAUSE", { TEXT_BORDER, SCREEN_HEIGHT - INFO_FONT_SIZE - TEXT_BORDER }, font.info); }
    }
    else if (status == OVER || status == WIN)
    {
        switch (status)
        {
            case OVER: SDL_snprintf(text, TEXT_MAX_LEN, "GAMEOVER!"); break;
            case WIN: SDL_snprintf(text, TEXT_MAX_LEN, "YOU WIN!"); break;
        }
        displayText(text, { (SCREEN_WIDTH - OVER_TITLE_WIDTH) / 2, TITLE_POSITION }, font.title);
        SDL_snprintf(text, TEXT_MAX_LEN, "Your score: %d", score);
        displayText(text, { (SCREEN_WIDTH - SCORE_WIDTH) / 2, SCORE_POSITION }, font.info);
        SDL_snprintf(text, TEXT_MAX_LEN, "Best score: %d", bestScore);
        displayText(text, { (SCREEN_WIDTH - SCORE_WIDTH) / 2, BEST_SCORE_POSITION }, font.info);
        displayText("Click anywhere to RESTART...", { (SCREEN_WIDTH - INFO_WIDTH) / 2, INFO_POSITION }, font.info);
    }
}

void MainGame::display()
{
    SDL_FillRect(image.surface, &screen, BACKGROUND_COLOR);
    displayImage();
    displayInfo();
    SDL_UpdateWindowSurface(window);
}

void Plank::init()
{
    rect = { (SCREEN_WIDTH - PLANK_WIDTH) / 2, SCREEN_HEIGHT - PLANK_POSITION, PLANK_WIDTH, PLANK_HEIGHT };
}

void Plank::move(int mouseX)
{
    if (mouseX > PLANK_WIDTH / 2 && mouseX < SCREEN_WIDTH - PLANK_WIDTH / 2) { rect.x = mouseX - PLANK_WIDTH / 2; }
    else if (mouseX <= PLANK_WIDTH / 2) { rect.x = 0; }
    else if (mouseX >= SCREEN_WIDTH - PLANK_WIDTH / 2) { rect.x = SCREEN_WIDTH - PLANK_WIDTH; }
}

void Plank::display()
{
    SDL_BlitSurface(game.image.plank, NULL, game.image.surface, &rect);
}

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

void Ball::display()
{
    SDL_BlitSurface(game.image.ball, NULL, game.image.surface, &rect);
}

void Block::init(Point point)
{
    rect = { point.x, point.y, BLOCK_SIZE, BLOCK_SIZE };
    alive = true;
}

void Block::display()
{
    SDL_BlitSurface(game.image.block, NULL, game.image.surface, &rect);
}