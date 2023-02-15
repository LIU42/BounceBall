#include "bounceball.h"

SDL_RWops* MainGame::getResource(LPCSTR name, LPCSTR type)
{
    HINSTANCE hInst = sysInfo.info.win.hinstance;
    HRSRC hRsrc = FindResource(hInst, name, type);
    DWORD size = SizeofResource(hInst, hRsrc);
    HGLOBAL hGlobal = LoadResource(hInst, hRsrc);
    LPVOID data = LockResource(hGlobal);
    return SDL_RWFromConstMem(data, size);
}

SDL_Surface* MainGame::loadSurface(Uint32 id)
{
    SDL_RWops* pResource = getResource(MAKEINTRESOURCE(id), TEXT("PNG"));
    SDL_Surface* originalSurface = IMG_LoadPNG_RW(pResource);
    SDL_Surface* convertedSurface = SDL_ConvertSurface(originalSurface, format, NULL);
    SDL_FreeSurface(originalSurface);
    SDL_FreeRW(pResource);
    return convertedSurface;
}

void MainGame::initEnvironment()
{
    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();
    SDL_VERSION(&sysInfo.version);
}

void MainGame::initWindow()
{
    window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    surface = SDL_GetWindowSurface(window);
    format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32);
    screenRect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
    SDL_GetWindowWMInfo(window, &sysInfo);
}

void MainGame::setDarkMode()
{
    DwmSetWindowAttribute(sysInfo.info.win.window, DARK_MODE_CODE, &IS_DARK_MODE, sizeof(IS_DARK_MODE));
}

void MainGame::restoreWindow()
{
    SDL_MinimizeWindow(window);
    SDL_RestoreWindow(window);
}

void MainGame::initGame()
{
    bestScore = 0;
    restart();
}

void MainGame::initBlock()
{
    for (int i = 0; i < Block::ROWS; i++)
    {
        for (int j = 0; j < Block::COLS; j++)
        {
            int x = i * Block::SIZE + WINDOW_BORDER_X;
            int y = j * Block::SIZE + WINDOW_BORDER_Y;

            block[i][j].init(x, y);
        }
    }
}

void MainGame::loadImage()
{
    images.plank = loadSurface(IDB_PNG1);
    images.ball = loadSurface(IDB_PNG2);
    images.block = loadSurface(IDB_PNG3);
}

void MainGame::loadFont()
{
    fonts.title = TTF_OpenFontRW(getResource(MAKEINTRESOURCE(IDR_FONT1), RT_FONT), true, TITLE_FONT_SIZE);
    fonts.info = TTF_OpenFontRW(getResource(MAKEINTRESOURCE(IDR_FONT1), RT_FONT), true, INFO_FONT_SIZE);
}

void MainGame::freeImage()
{
    SDL_FreeSurface(images.plank);
    SDL_FreeSurface(images.ball);
    SDL_FreeSurface(images.block);
}

void MainGame::freeFont()
{
    TTF_CloseFont(fonts.title);
    TTF_CloseFont(fonts.info);
}

void MainGame::closeWindow()
{
    SDL_DestroyWindow(window);
    SDL_FreeFormat(format);
}

void MainGame::closeEnvironment()
{
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

void MainGame::close()
{
    freeImage();
    freeFont();
    closeWindow();
    closeEnvironment();
}

void MainGame::levelUp()
{
    ball.speedUp(score);
}

void MainGame::gameover()
{
    if (ball.getCenterY() >= SCREEN_HEIGHT + GAMEOVER_DELAY_DISTANCE)
    {
        if (score > bestScore) { bestScore = score; }
        status = OVER;
    }
    else if (hitCount == Block::ROWS * Block::COLS)
    {
        if (score > bestScore) { bestScore = score; }
        status = WIN;
    }
}

void MainGame::restart()
{
    status = START;
    score = 0;
    hitCount = 0;
    plank.init(SCREEN_WIDTH);
    ball.init(SCREEN_WIDTH);
    initBlock();
}

void MainGame::reflect()
{
    reflectOnPlank();
    reflectOnEdge();
    reflectOnBlock();
}

void MainGame::reflectOnPlank()
{
    if (ball.getCenterY() >= ball.INIT_TOP + ball.RADIUS && ball.getCenterY() < ball.INIT_TOP + ball.RADIUS + ball.getSpeed())
    {
        if (ball.getCenterX() >= plank.getLeftBorder() - ball.getSpeed() && ball.getCenterX() <= plank.getRightBorder() + ball.getSpeed())
        {
            ball.reflectY();
            score += CATCH_SCORE;
        }
    }
}

void MainGame::reflectOnEdge()
{
    if (ball.getCenterX() <= ball.RADIUS || ball.getCenterX() >= SCREEN_WIDTH - ball.RADIUS) { ball.reflectX(); }
    if (ball.getCenterY() <= ball.RADIUS) { ball.reflectY(); }
}

void MainGame::reflectOnBlock()
{
    bool isReflectX = false;
    bool isReflectY = false;

    for (int x = 0; x < Block::ROWS; x++)
    {
        for (int y = 0; y < Block::COLS; y++)
        {
            if (block[x][y].getIsAlive())
            {
                int distanceX = ball.getCenterX() - block[x][y].getCenterX();
                int distanceY = ball.getCenterY() - block[x][y].getCenterY();

                int absDistanceX = SDL_abs(distanceX);
                int absDistanceY = SDL_abs(distanceY);

                if (absDistanceX > ball.RADIUS + Block::SIZE / 2) { break; }

                if (SDL_pow(absDistanceX, 2) + SDL_pow(absDistanceY, 2) <= SDL_pow(ball.RADIUS + Block::SIZE / 2, 2))
                {
                    if (absDistanceX == absDistanceY)
                    {
                        if (!isReflectX) { ball.reflectX(distanceX / absDistanceX); isReflectX = true; }
                        if (!isReflectY) { ball.reflectY(distanceY / absDistanceY); isReflectY = true; }
                    }
                    else if (absDistanceX < absDistanceY && !isReflectY) { ball.reflectY(); isReflectY = true; }
                    else if (absDistanceX > absDistanceY && !isReflectX) { ball.reflectX(); isReflectX = true; }

                    block[x][y].destoryed();
                    score += HIT_SCORE;
                    hitCount += 1;
                }
            }
        }
    }
}

bool MainGame::isRunning()
{
    return status != EXIT;
}

void MainGame::update()
{
    if (status == PLAYING)
    {
        ball.move();
        levelUp();
        gameover();
        reflect();
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
                case WIN: restart(); status = PLAYING; break;
                case OVER: restart(); status = PLAYING; break;
            }
        }
        if (event.type == SDL_MOUSEMOTION && status == PLAYING)
        {
            if (event.motion.x >= plank.WIDTH / 2 && event.motion.x <= SCREEN_WIDTH - plank.WIDTH / 2)
            {
                plank.moveTo(event.motion.x);
            }
            else if (event.motion.x < plank.WIDTH / 2)
            {
                plank.moveTo(plank.WIDTH / 2);
            }
            else if (event.motion.x > SCREEN_WIDTH - plank.WIDTH / 2)
            {
                plank.moveTo(SCREEN_WIDTH - plank.WIDTH / 2);
            }
        }
        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_p && status == PLAYING) { status = PAUSE; }
        if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_MINIMIZED && status == PLAYING) { status = PAUSE; }
    }
}

void MainGame::displayText(const char* text, int x, int y, TTF_Font* font)
{
    static SDL_Surface* textSurface;
    static SDL_Rect textRect;

    textSurface = TTF_RenderText_Blended(font, text, WHITE);
    textRect.x = x;
    textRect.y = y;

    SDL_BlitSurface(textSurface, NULL, surface, &textRect);
    SDL_FreeSurface(textSurface);
}

void MainGame::displayImage()
{
    if (status == PLAYING || status == PAUSE)
    {
        displayPlank();
        displayBall();
        displayBlock();
    }
}

void MainGame::displayInfo()
{
    static char text[TEXT_LENGTH];

    if (status == START)
    {
        displayText("BOUNCE BALL", (SCREEN_WIDTH - MAIN_TITLE_WIDTH) / 2, TITLE_TOP, fonts.title);
        displayText("Click anywhere to START...", (SCREEN_WIDTH - INFO_WIDTH) / 2, INFO_TOP, fonts.info);
    }
    else if (status == PLAYING || status == PAUSE)
    {
        if (status == PAUSE)
        {
            displayText("PAUSE", TEXT_BORDER, SCREEN_HEIGHT - INFO_FONT_SIZE - TEXT_BORDER, fonts.info);
        }
        SDL_snprintf(text, TEXT_LENGTH, "Score: %d", score);
        displayText(text, SCREEN_WIDTH - TEXT_BORDER - DURING_SCORE_WIDTH, SCREEN_HEIGHT - INFO_FONT_SIZE - TEXT_BORDER, fonts.info);
    }
    else if (status == OVER || status == WIN)
    {
        switch (status)
        {
            case OVER: SDL_snprintf(text, TEXT_LENGTH, "GAMEOVER!"); break;
            case WIN: SDL_snprintf(text, TEXT_LENGTH, "YOU WIN!"); break;
        }
        displayText(text, (SCREEN_WIDTH - OVER_TITLE_WIDTH) / 2, TITLE_TOP, fonts.title);
        SDL_snprintf(text, TEXT_LENGTH, "Your score: %d", score);
        displayText(text, (SCREEN_WIDTH - SCORE_WIDTH) / 2, SCORE_TOP, fonts.info);
        SDL_snprintf(text, TEXT_LENGTH, "Best score: %d", bestScore);
        displayText(text, (SCREEN_WIDTH - SCORE_WIDTH) / 2, BEST_SCORE_TOP, fonts.info);
        displayText("Click anywhere to RESTART...", (SCREEN_WIDTH - INFO_WIDTH) / 2, INFO_TOP, fonts.info);
    }
}

void MainGame::displayPlank()
{
    SDL_BlitSurface(images.plank, NULL, surface, &plank);
}

void MainGame::displayBall()
{
    SDL_BlitSurface(images.ball, NULL, surface, &ball);
}

void MainGame::displayBlock()
{
    for (int x = 0; x < Block::ROWS; x++)
    {
        for (int y = 0; y < Block::COLS; y++)
        {
            if (block[x][y].getIsAlive())
            {
                SDL_BlitSurface(images.block, NULL, surface, &block[x][y]);
            }
        }
    }
}

void MainGame::display()
{
    SDL_FillRect(surface, &screenRect, 0x161616);
    displayImage();
    displayInfo();
    SDL_UpdateWindowSurface(window);
}

void MainGame::delay(Uint32 startTick, Uint32 endTick)
{
    int deltaTick = endTick - startTick;
    int delayTick = 1000 / GAME_FPS - deltaTick;

    if (delayTick > 0)
    {
        SDL_Delay(delayTick);
    }
}