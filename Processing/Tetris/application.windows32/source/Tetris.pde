import processing.sound.*;

/* 
* Thanks to Javidx9 for his tutorial on programming Tetris, it was of great help - https://www.youtube.com/watch?v=8OK8_tHeCIA
*
* Sources for sound effects, music and textures: 
* Berusky 2 OST - Action Puzzle Game Soundtrack - https://opengameart.org/content/berusky-ii-ost-action-puzzle-game-soundtrack-23-tracks

* Thanks to MATTIX for these sound effects: 
* Click1.wav - https://freesound.org/people/MATTIX/sounds/348022/
* Click2.wav - https://freesound.org/people/MATTIX/sounds/349873/

* Source for the textures (which are currently disabled) - https://opengameart.org/content/8-bit-tetris
*/

String[] tetrominoes = new String[7];
int resX = 800;
int resY = 800;
int score = 0;
int secondsSinceStart = 0; // Seconds since pressing spacebar
float secondCounter = 0;
float pushDownTimer = 0;
float pushDownDelay = 1000; // Time between automatic pushdown of the falling piece
boolean gameOver = false;
boolean initialPause = true;

// PShape objects are used to render each different thing on screen
PShape boxShape;
PShape fillShape;
PShape mapFillerShape;
PShape bubbleShape;
PShape pauseTextBgShape;
PShape scoreTextBgShape;
PShape timerTextBgShape;
PImage[] textures = new PImage[7];
PImage icon;
PImage spritesheet;
PImage vignette;
PFont font;

ArrayList<Integer> blocksToRemove = new ArrayList<Integer>();
float blockDissolveTimer = 0;

Sound sound;
SoundFile bgm;
SoundFile click1;
SoundFile click2;

void settings()
{
    size(resX, resY, P3D);
    PJOGL.setIcon("resources/icon.png"); // Window icon for the game
}

void setup()
{        
    // Define the shape of the tetrominoes
    // Each X marks a tile of the shape
    tetrominoes[0] =  "--X-"; // First assignment needs to be "=" to replace the initial null value
    tetrominoes[0] += "--X-";
    tetrominoes[0] += "--X-";
    tetrominoes[0] += "--X-";
    
    tetrominoes[1] =  "--X-";
    tetrominoes[1] += "-XX-";
    tetrominoes[1] += "-X--";
    tetrominoes[1] += "----";
    
    tetrominoes[2] =  "-X--";
    tetrominoes[2] += "-XX-";
    tetrominoes[2] += "--X-";
    tetrominoes[2] += "----";
    
    tetrominoes[3] =  "----";
    tetrominoes[3] += "-XX-";
    tetrominoes[3] += "-XX-";
    tetrominoes[3] += "----";
    
    tetrominoes[4] =  "--X-";
    tetrominoes[4] += "-XX-";
    tetrominoes[4] += "--X-";
    tetrominoes[4] += "----";
    
    tetrominoes[5] =  "----";
    tetrominoes[5] += "-XX-";
    tetrominoes[5] += "--X-";
    tetrominoes[5] += "--X-";
    
    tetrominoes[6] =  "----";
    tetrominoes[6] += "-XX-";
    tetrominoes[6] += "-X--";
    tetrominoes[6] += "-X--";

    frameRate(60);
    ((PGraphicsOpenGL)g).textureSampling(3);

    // I'm using a bunch of shape objects because for some reason rect() doesn't work properly
    shapeMode(CORNER);
    boxShape = createShape(BOX, 32, 32, 1);
    fillShape = createShape(BOX, resX, resY, 1);
    mapFillerShape = createShape(BOX, 32, 32, 1);
    bubbleShape = createShape(SPHERE, 30);
    pauseTextBgShape = createShape(BOX, 200, 200, 1);
    scoreTextBgShape = createShape(BOX, 150, 40, 1);
    timerTextBgShape = createShape(BOX, 150, 40, 1);
    
    boxShape.setSpecular(color(0, 0, 0));
    boxShape.setAmbient(color(255, 255, 255));
    boxShape.setShininess(0);
    boxShape.setEmissive(0);
    
    pauseTextBgShape.setFill(color(0, 0, 0));
    pauseTextBgShape.setSpecular(color(0, 0, 0));
    pauseTextBgShape.setAmbient(color(0, 0, 0));
    pauseTextBgShape.setStroke(color(255, 255, 255));
    
    scoreTextBgShape.setFill(color(0, 0, 0));
    scoreTextBgShape.setSpecular(color(0, 0, 0));
    scoreTextBgShape.setAmbient(color(0, 0, 0));
    scoreTextBgShape.setStroke(color(255, 255, 255));
    
    timerTextBgShape.setFill(color(0, 0, 0));
    timerTextBgShape.setSpecular(color(0, 0, 0));
    timerTextBgShape.setAmbient(color(0, 0, 0));
    timerTextBgShape.setStroke(color(255, 255, 255));
    
    font = loadFont("Digital-7Mono-18.vlw");
    textFont(font);
    textAlign(CENTER, CENTER);
    
    // Not using the textures from this spritesheet anymore
    // Instead we are giving colors to the tiles based on the current background color
    spritesheet = loadImage("resources/spritesheet.png");
    vignette = loadImage("resources/vignette.png"); // For darkening the edges of the screen
    
    for(int i = 0; i < 7; i++)
    {
        textures[i] = spritesheet.get(32 * i, 0, 32, 32);
    }
    
    textureMode(REPEAT);
    sphereDetail(15);
    
    bgm = new SoundFile(this, "resources/bgm.wav");
    click1 = new SoundFile(this, "resources/click1.wav");
    click2 = new SoundFile(this, "resources/click2.wav");
    
    // Some audio mixing so we don't blow anyone's ears up
    bgm.amp(0.1);
    click1.amp(0.8);
    click2.amp(0.4);
    sound = new Sound(this);
    sound.volume(0.7);
    
    createMap();
    getNewPiece();
}

void draw() 
{        
    update();
    
    drawBackground();
    
    drawBubbles();
    
    drawVignette();
    
    drawForeground();
    
    drawGhostPiece();
    
    drawFallingPiece();
    
    drawInterface();
    
    if(initialPause) drawPauseScreen();
    
    if(gameOver) drawGameOverScreen();
}

// Main gameplay logic loop - push the current piece down, check inputs and remove full rows if they exist
void update()
{
    //println(frameRate);
    
    if(!gameOver) 
    {
        
        if(!initialPause) 
        {
        
            if(millis() - secondCounter >= 1000)
            {
                secondsSinceStart++;
                secondCounter = millis();
            }
            
            // Remove rows of blocks if there are any to be removed
            if(blocksToRemove.size() > 0) 
            {
                dissolveBlocks();
                pushDownTimer = millis();
                return; // Pause until blocks have been removed
            }
            
            checkInputs();
            
            // If the falling piece wasn't manually pushed down by the player, push it down automatically after a delay
            // pushDownTimer is manipulated to control when or if the piece should be pushed down automatically
            if(millis() - pushDownTimer > pushDownDelay) 
            {
                
                if(checkIfPieceFits(currPieceX, currPieceY + 1, rotationState))
                {
                    currPieceY++;
                }
                else
                {
                    lockCurrPieceToMap(); 
                }
                
                pushDownTimer = millis();
            }
            
        }
        
    } 
    
}

// Checks for 10 blocks on each row
void checkForRows() 
{
    
    for(int y = 0; y < mapHeight - 1; y++)
    {
        int piecesInRow = 0;
        
        for(int x = 1; x < mapWidth - 1; x++)
        {
            
            if(map[y * mapWidth + x] != 0) piecesInRow++;
            
        }
        
        if(piecesInRow == 10) 
        {
            if(!click1.isPlaying()) click1.play();
            score += 100;
            
            for(int i = 1; i < mapWidth - 1; i++) 
            {
                blocksToRemove.add(y * mapWidth + i);
                blockDissolveTimer = millis();
            }
            
        }
        
    }

}

// Removes rows of blocks and moves all the tiles above down N amount of steps
// The basic logic:
// 1. Find the height for each row we need to remove
// 2. Start by removing the lowest row we need to remove
// 3. Displace all blocks above the rows we remove
// 4. How much we displace the blocks needs to be increased by 1 for each removed row below it
void dissolveBlocks() 
{
    int dissolveTime = 200;
    
    if(millis() - blockDissolveTimer > dissolveTime) 
    {
        int startHeight = (blocksToRemove.get(blocksToRemove.size() - 1) + 1) / mapWidth;

        ArrayList<Integer> rowsToRemoveHeights = new ArrayList<Integer>();
        
        for(int i = 0; i < blocksToRemove.size() / 10; i++) 
        {
            rowsToRemoveHeights.add((blocksToRemove.get(10 * i) + 1) / mapWidth);
        }
        
        int numRowsToDisplace = 0;
        
        for(int y = startHeight; y >= 0; y--)
        {
            boolean doDisplace = true;
            
            for(int j = 0; j < rowsToRemoveHeights.size(); j++) 
            {

                if(y == rowsToRemoveHeights.get(j)) 
                {
                    numRowsToDisplace++;
                    doDisplace = false;
                }
                
            }
            
            for(int x = 1; x < mapWidth - 1; x++)
            {
                
                if(doDisplace) 
                { 
                    map[(y + numRowsToDisplace) * mapWidth + x] = map[y * mapWidth + x];
                    tileColors[(y + numRowsToDisplace) * mapWidth + x] = tileColors[y * mapWidth + x];
                }
                
                map[y * mapWidth + x] = 0;
                tileColors[y * mapWidth + x] = color(0, 0, 0, 255);
            }
            
        }
        
        blocksToRemove.clear();
    }
    
}

// Locks the current piece as part of the map and checks the game over condition
void lockCurrPieceToMap() 
{
    click2.play();
    
    int blocksThatFit = 0;
    
    for(int y = 0; y < 4; y++) 
    {
    
        for(int x = 0; x < 4; x++) 
        {
            int pieceIndex = rotate(x, y, rotationState);
            
            if(tetrominoes[currPieceType].charAt(pieceIndex) == 'X') 
            {
                int mapIndex = (currPieceY + y) * mapWidth + (currPieceX + x);
                
                if(mapIndex >= 0) 
                {
                    map[mapIndex] = currPieceType + 2;
                    tileColors[mapIndex] = currPieceColor;
                    blocksThatFit++;
                }
                
            }
    
        }
    
    }
    
    if(blocksThatFit != 4)
    {
        gameOver = true;
    }
    
    if(!gameOver) 
    {
        checkForRows();
        updateGameSpeed();
        getNewPiece();
    }
    
}

// Check if the piece fits in the position it's trying to move into
boolean checkIfPieceFits(int movingToX, int movingToY, int rotation) 
{
    for(int y = 0; y < 4; y++) 
    {

        for(int x = 0; x < 4; x++) 
        {
            int pieceIndex = rotate(x, y, rotation);
            
            int mapIndex = (movingToY + y) * mapWidth + (movingToX + x);       
                  
            if(movingToX + x <= 0 || movingToX + x >= mapWidth - 1) 
            {  
                
                if(tetrominoes[currPieceType].charAt(pieceIndex) == 'X')
                {
                    return false;
                }
                
            }
               
            if(movingToX + x >= 0 && movingToX + x < mapWidth) 
            {
                
                if(movingToY + y >= 0 && movingToY + y < mapHeight) 
                {
    
                    if(tetrominoes[currPieceType].charAt(pieceIndex) == 'X' && map[mapIndex] != 0) 
                    {
                        return false;
                    }
                    
                }
                
            }
            
        }
        
    }
    
    return true;
}

// Instantly place the current piece at the lowest point directly below it
void placePieceDownInstantly() 
{
    int lastFitY = 0;
    
    for(int y = 0; y < mapHeight + 2; y++) 
    {
        
        if(checkIfPieceFits(currPieceX, currPieceY + y, rotationState))
        {
            lastFitY = currPieceY + y;
        }
        else
        {
            currPieceY = lastFitY;
            lockCurrPieceToMap();
            break;
        }
        
    }
    
}

// Changes speed of automatic pushdown according to time elapsed, somewhat in tune with the music
void updateGameSpeed()
{
    
    if(secondsSinceStart >= 248) 
    {
        pushDownDelay = 70;
    }
    else if(secondsSinceStart >= 180)
    {
        pushDownDelay = 300;
    }
    else if(secondsSinceStart >= 120)
    {
        pushDownDelay = 500;
    }
    else if(secondsSinceStart >= 60) 
    {
        pushDownDelay = 700;
    }
    
}

// Thanks to Javidx9 for this algorithm - https://www.youtube.com/watch?v=8OK8_tHeCIA
int rotate(int rx, int ry, int rState) 
{
    
    switch(rState)
    {
        case 0:
            return ry * 4 + rx;
        case 1:
            return 12 + ry - (rx * 4);
        case 2:
            return 15 - (ry * 4) - rx;
        case 3:
            return 3 - ry + (rx * 4);
    }
    
    return 0;
}

void resetGameState() 
{
    createMap();
    getNewPiece();
    gameOver = false;
    initialPause = true;
    bgm.stop();
    secondsSinceStart = 0;
    secondCounter = millis();
    pushDownDelay = 1000;
    score = 0;
}
    
