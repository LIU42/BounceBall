#include "bounceball.h"

SDL_RWops* MainGame::getResource(LPCSTR resourceName, LPCSTR resourceType)
{
    HINSTANCE hInstance = windowInfo.info.win.hinstance;
    HRSRC hResource = FindResource(hInstance, resourceName, resourceType);
    DWORD resourceSize = SizeofResource(hInstance, hResource);
    HGLOBAL hGlobal = LoadResource(hInstance, hResource);
    LPVOID resourceData = LockResource(hGlobal);
    return SDL_RWFromConstMem(resourceData, resourceSize);
}

SDL_Surface* MainGame::loadSurface(Uint32 resourceID)
{
    SDL_RWops* pResource = getResource(MAKEINTRESOURCE(resourceID), TEXT("PNG"));
    SDL_Surface* pOriginalSurface = IMG_LoadPNG_RW(pResource);
    SDL_Surface* pConvertedSurface = SDL_ConvertSurface(pOriginalSurface, pFormat, NULL);
    SDL_FreeSurface(pOriginalSurface);
    SDL_FreeRW(pResource);
    return pConvertedSurface;
}

void MainGame::getVersion()
{
    windowInfo.version.major = SDL_MAJOR_VERSION;
    windowInfo.version.minor = SDL_MINOR_VERSION;
    windowInfo.version.patch = SDL_PATCHLEVEL;
}

void MainGame::initSystem()
{
    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();
}

void MainGame::initWindow()
{
    pWindow = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_HIDDEN);
    pSurface = SDL_GetWindowSurface(pWindow);
    pFormat = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32);
    SDL_GetWindowWMInfo(pWindow, &windowInfo);
}

void MainGame::setDarkMode()
{
    DwmSetWindowAttribute(windowInfo.info.win.window, DARK_MODE_CODE, &DARK_MODE_FLAG, sizeof(DARK_MODE_FLAG));
}

void MainGame::showWindow()
{
    SDL_ShowWindow(pWindow);
}

void MainGame::initGame()
{
    bestScore = 0;
    restartGame();
}

void MainGame::initBlockMatrix()
{
    for (int i = 0; i < Block::ROWS; i++)
    {
        for (int j = 0; j < Block::COLS; j++)
        {
            int x = i * Block::SIZE + WINDOW_BORDER_X;
            int y = j * Block::SIZE + WINDOW_BORDER_Y;

            blockMatrix[i][j].init(x, y);
        }
    }
}

void MainGame::loadImages()
{
    images.pPlank = loadSurface(IDB_PNG1);
    images.pBall = loadSurface(IDB_PNG2);
    images.pBlock = loadSurface(IDB_PNG3);
}

void MainGame::loadFonts()
{
    fonts.pTitle = TTF_OpenFontRW(getResource(MAKEINTRESOURCE(IDR_FONT1), RT_FONT), true, TITLE_FONT_SIZE);
    fonts.pInfo = TTF_OpenFontRW(getResource(MAKEINTRESOURCE(IDR_FONT1), RT_FONT), true, INFO_FONT_SIZE);
}

void MainGame::freeImages()
{
    SDL_FreeSurface(images.pPlank);
    SDL_FreeSurface(images.pBall);
    SDL_FreeSurface(images.pBlock);
}

void MainGame::freeFonts()
{
    TTF_CloseFont(fonts.pTitle);
    TTF_CloseFont(fonts.pInfo);
}

void MainGame::closeWindow()
{
    SDL_DestroyWindow(pWindow);
    SDL_FreeFormat(pFormat);
}

void MainGame::closeSystem()
{
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

void MainGame::levelUp()
{
    ball.setLevelUpSpeed(score);
}

void MainGame::gameover()
{
    if (ball.getCenterY() >= SCREEN_HEIGHT + GAMEOVER_DELAY_DISTANCE)
    {
        if (score > bestScore)
        {
            bestScore = score;
        }
        status = STATUS_FAILURE;
    }
    else if (hitCount == Block::ROWS * Block::COLS)
    {
        if (score > bestScore)
        {
            bestScore = score;
        }
        status = STATUS_SUCCESS;
    }
}

void MainGame::restartGame()
{
    status = STATUS_START;
    score = 0;
    hitCount = 0;
    plank.init();
    ball.init();
    initBlockMatrix();
}

void MainGame::reflect()
{
    reflectOnPlank();
    reflectOnEdge();
    reflectOnBlock();
}

void MainGame::reflectOnPlank()
{
    if (ball.getCenterY() < Ball::INIT_UPPER + Ball::RADIUS)
    {
        return;
    }
    if (ball.getCenterY() > Ball::INIT_UPPER + Ball::RADIUS + ball.getSpeed())
    {
        return;
    }
    if (ball.getCenterX() < plank.getLeftBorder() - ball.getSpeed())
    {
        return;
    }
    if (ball.getCenterX() > plank.getRightBorder() + ball.getSpeed())
    {
        return;
    }
    ball.reflectY();
    score += CATCH_SCORE;
}

void MainGame::reflectOnEdge()
{
    if (ball.getCenterX() <= ball.RADIUS || ball.getCenterX() >= SCREEN_WIDTH - ball.RADIUS)
    {
        ball.reflectX();
    }
    if (ball.getCenterY() <= ball.RADIUS)
    {
        ball.reflectY();
    }
}

void MainGame::reflectOnBlock()
{
    bool isReflectX = false;
    bool isReflectY = false;

    for (int x = 0; x < Block::ROWS; x++)
    {
        for (int y = 0; y < Block::COLS; y++)
        {
            if (!blockMatrix[x][y].getIsAlive())
            {
                continue;
            }
            int distanceX = ball.getCenterX() - blockMatrix[x][y].getCenterX();
            int distanceY = ball.getCenterY() - blockMatrix[x][y].getCenterY();

            int absDistanceX = SDL_abs(distanceX);
            int absDistanceY = SDL_abs(distanceY);

            if (absDistanceX > Ball::RADIUS + Block::SIZE / 2)
            {
                break;
            }
            if (SDL_pow(absDistanceX, 2) + SDL_pow(absDistanceY, 2) > SDL_pow(Ball::RADIUS + Block::SIZE / 2, 2))
            {
                continue;
            }
            if (absDistanceX == absDistanceY)
            {
                if (!isReflectX) { ball.setSignX(distanceX / absDistanceX); isReflectX = true; }
                if (!isReflectY) { ball.setSignY(distanceY / absDistanceY); isReflectY = true; }
            }
            else if (absDistanceX < absDistanceY && !isReflectY) { ball.reflectY(); isReflectY = true; }
            else if (absDistanceX > absDistanceY && !isReflectX) { ball.reflectX(); isReflectX = true; }

            blockMatrix[x][y].destroyed();
            score += HIT_SCORE;
            hitCount += 1;
        }
    }
}

void MainGame::displayText(const char* pText, int x, int y, TTF_Font* pFont)
{
    SDL_Surface* pTextSurface = TTF_RenderText_Blended(pFont, pText, WHITE);
    SDL_Rect textRect = { x, y, NULL, NULL };

    SDL_BlitSurface(pTextSurface, NULL, pSurface, &textRect);
    SDL_FreeSurface(pTextSurface);
}

void MainGame::displayImage()
{
    if (status == STATUS_PLAYING || status == STATUS_PAUSE)
    {
        displayPlank();
        displayBall();
        displayBlock();
    }
}

void MainGame::displayInfo()
{
    static char text[TEXT_LENGTH];

    if (status == STATUS_START)
    {
        displayText("BOUNCE BALL", (SCREEN_WIDTH - MAIN_TITLE_WIDTH) / 2, TITLE_UPPER, fonts.pTitle);
        displayText("Click anywhere to START...", (SCREEN_WIDTH - INFO_WIDTH) / 2, INFO_UPPER, fonts.pInfo);
    }
    else if (status == STATUS_PLAYING || status == STATUS_PAUSE)
    {
        if (status == STATUS_PAUSE)
        {
            displayText("PAUSE", TEXT_BORDER, SCREEN_HEIGHT - INFO_FONT_SIZE - TEXT_BORDER, fonts.pInfo);
        }
        SDL_snprintf(text, TEXT_LENGTH, "Score: %d", score);
        displayText(text, SCREEN_WIDTH - TEXT_BORDER - DURING_SCORE_WIDTH, SCREEN_HEIGHT - INFO_FONT_SIZE - TEXT_BORDER, fonts.pInfo);
    }
    else if (status == STATUS_FAILURE || status == STATUS_SUCCESS)
    {
        switch (status)
        {
            case STATUS_FAILURE: SDL_snprintf(text, TEXT_LENGTH, "GAMEOVER!"); break;
            case STATUS_SUCCESS: SDL_snprintf(text, TEXT_LENGTH, "YOU WIN!"); break;
        }
        displayText(text, (SCREEN_WIDTH - OVER_TITLE_WIDTH) / 2, TITLE_UPPER, fonts.pTitle);
        SDL_snprintf(text, TEXT_LENGTH, "Your score: %d", score);

        displayText(text, (SCREEN_WIDTH - SCORE_WIDTH) / 2, SCORE_UPPER, fonts.pInfo);
        SDL_snprintf(text, TEXT_LENGTH, "Best score: %d", bestScore);

        displayText(text, (SCREEN_WIDTH - SCORE_WIDTH) / 2, BEST_SCORE_UPPER, fonts.pInfo);
        displayText("Click anywhere to RESTART...", (SCREEN_WIDTH - INFO_WIDTH) / 2, INFO_UPPER, fonts.pInfo);
    }
}

void MainGame::displayPlank()
{
    SDL_BlitSurface(images.pPlank, NULL, pSurface, &plank);
}

void MainGame::displayBall()
{
    SDL_BlitSurface(images.pBall, NULL, pSurface, &ball);
}

void MainGame::displayBlock()
{
    for (int x = 0; x < Block::ROWS; x++)
    {
        for (int y = 0; y < Block::COLS; y++)
        {
            if (blockMatrix[x][y].getIsAlive())
            {
                SDL_BlitSurface(images.pBlock, NULL, pSurface, &blockMatrix[x][y]);
            }
        }
    }
}

MainGame::MainGame()
{
    srand((unsigned)time(NULL));
    getVersion();
    initSystem();
    initWindow();
    setDarkMode();
    initGame();
    loadImages();
    loadFonts();
    showWindow();
}

MainGame::~MainGame()
{
    freeImages();
    freeFonts();
    closeWindow();
    closeSystem();
}

bool MainGame::isRunning()
{
    return status != STATUS_EXIT;
}

void MainGame::update()
{
    if (status == STATUS_PLAYING)
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
        if (event.type == SDL_QUIT)
        {
            status = STATUS_EXIT;
        }
        if (event.type == SDL_MOUSEBUTTONDOWN && status != STATUS_EXIT)
        {
            if (status == STATUS_SUCCESS || status == STATUS_FAILURE)
            {
                restartGame();
            }
            status = STATUS_PLAYING;
        }
        if (event.type == SDL_MOUSEMOTION && status == STATUS_PLAYING)
        {
            int leftBound = Plank::WIDTH / 2;
            int rightBound = SCREEN_WIDTH - Plank::WIDTH / 2;

            if (event.motion.x < leftBound)
            {
                plank.moveTo(leftBound);
                continue;
            }
            if (event.motion.x > rightBound)
            {
                plank.moveTo(rightBound);
                continue;
            }
            plank.moveTo(event.motion.x);
        }
        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_p && status == STATUS_PLAYING)
        {
            status = STATUS_PAUSE;
        }
        if (event.window.event == SDL_WINDOWEVENT_MINIMIZED && status == STATUS_PLAYING)
        {
            status = STATUS_PAUSE;
        }
    }
}

void MainGame::display()
{
    SDL_FillRect(pSurface, NULL, 0x161616);
    displayImage();
    displayInfo();
    SDL_UpdateWindowSurface(pWindow);
}

void MainGame::delay(Uint32 startTick, Uint32 endTick)
{
    int deltaTick = endTick - startTick;
    int delayTick = 1000 / GAME_FPS - deltaTick;

    SDL_Delay((delayTick > 0) ? delayTick : 0);
}