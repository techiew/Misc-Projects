import processing.core.*; 
import processing.data.*; 
import processing.event.*; 
import processing.opengl.*; 

import processing.sound.*; 

import java.util.HashMap; 
import java.util.ArrayList; 
import java.io.File; 
import java.io.BufferedReader; 
import java.io.PrintWriter; 
import java.io.InputStream; 
import java.io.OutputStream; 
import java.io.IOException; 

public class Tetris extends PApplet {



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

public void settings()
{
    size(resX, resY, P3D);
    PJOGL.setIcon("resources/icon.png"); // Window icon for the game
}

public void setup()
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
    bgm.amp(0.1f);
    click1.amp(0.8f);
    click2.amp(0.4f);
    sound = new Sound(this);
    sound.volume(0.7f);
    
    createMap();
    getNewPiece();
}

public void draw() 
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
public void update()
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
public void checkForRows() 
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
public void dissolveBlocks() 
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
public void lockCurrPieceToMap() 
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
public boolean checkIfPieceFits(int movingToX, int movingToY, int rotation) 
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
public void placePieceDownInstantly() 
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
public void updateGameSpeed()
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
public int rotate(int rx, int ry, int rState) 
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

public void resetGameState() 
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
    
boolean w = false, a = false, s = false, d = false, esc = false, space = false;
float downHoldDelay = 70; // ms between each downwards movement tick while holding the key
float downPressTime = 0;
float strafeHoldDelay = 120;
float strafePressTime = -1;
int dx = 0, dy = 0; //Delta x, delta y
int dr = 0; // Delta rotate

public void checkInputs() 
{
    
    // Check if key is still being held after a set delay
    if(millis() - downPressTime > downHoldDelay)
    {

        if(s)
        {
            dy = 1;
        }

        downPressTime = millis();
    }
    
    if(millis() - strafePressTime > strafeHoldDelay) 
    {
    
        if(a)
        {
            dx = -1;
        }
        
        if(d)
        {
            dx = 1;
        } 
        
        strafePressTime = millis();
    }
    
    // Check if we are trying to rotate the piece
    if(dr != 0) 
    {
        
        int rotateTest = rotationState;
        int maxRotate = 3; // How many states of rotation are allowed for a piece type
        
        switch(currPieceType) 
        {
            case 0:
                maxRotate = 1;
                break;
            case 1:
                maxRotate = 1;
                break;
            case 2:
                maxRotate = 1;
                break;
            case 3:
                maxRotate = 1;
                break;
            case 4:
                maxRotate = 3;
                break;
            case 5:
                maxRotate = 3;
                break;
            case 6:
                maxRotate = 3;
                break;
        }
        
        if(rotateTest + dr == -1)
        {
            rotateTest = 3;
        }
        else if(rotateTest + dr > maxRotate)
        {
            rotateTest = 0;
        }
        else
        {
            rotateTest += dr;
        }
        
        if(checkIfPieceFits(currPieceX, currPieceY, rotateTest)) 
        {
            rotationState = rotateTest;
        }
        else if(checkIfPieceFits(currPieceX + 1, currPieceY, rotateTest))
        {
            rotationState = rotateTest;
            dx = 1;
        }
        else if(checkIfPieceFits(currPieceX - 1, currPieceY, rotateTest))
        {
            rotationState = rotateTest;
            dx = -1;
        }
        else if(checkIfPieceFits(currPieceX + 2, currPieceY, rotateTest))
        {
            rotationState = rotateTest;
            dx = 2;
        }
        else if(checkIfPieceFits(currPieceX - 2, currPieceY, rotateTest))
        {
            rotationState = rotateTest;
            dx = -2;
        }
        
    }
    
    // Check if we are trying to move the piece horizontally
    if(dx != 0) 
    {
 
        if(checkIfPieceFits(currPieceX + dx, currPieceY, rotationState)) 
        {
            currPieceX += dx;
        }
        
    }
    
    // Check if we are trying to move the piece vertically
    if(dy != 0)
    {
        
        if(checkIfPieceFits(currPieceX, currPieceY + dy, rotationState))
        {
            currPieceY += dy;
            pushDownTimer = millis();
        }
        else
        {
            lockCurrPieceToMap();
        }
        
    }
    
    dx = 0;
    dy = 0;
    dr = 0;
}

public void keyPressed()
{
    
    if(key == 'w')
    {
        w = true;
    }

    if(key == 'a')
    {
        dx = -1;
        a = true;
        strafePressTime = millis();
    }
    
    if(key == 's')
    {
        dy = 1;
        s = true;
        downPressTime = millis();
    }
    
    if(key == 'd')
    {
        dx = 1;
        d = true;
        strafePressTime = millis();
    } 
    
    if(keyCode == UP) 
    {
        
    }
    
    if(keyCode == DOWN)
    {
        
    }
    
    if(keyCode == LEFT)
    {
        dr = -1;
    }
    
    if(keyCode == RIGHT)
    {
        dr = 1;
    }
    
    if(keyCode == ESC)
    {
        esc = (esc) ? false : true;
    }

    if(keyCode == 32) // Spacebar
    {
        
        if(initialPause) 
        {
            bgm.play();
            initialPause = false;
            secondCounter = millis();
        }
        else if(gameOver) 
        {
            resetGameState();
        }
        else
        {
            placePieceDownInstantly();
        }
        
    }

}

public void keyReleased()
{   
    
    if(key == 'w') 
    {
        w = false;
    }

    if(key == 'a')
    {
        a = false;
    }
    
    if(key == 's')
    {
        s = false;
    }
    
    if(key == 'd')
    {
        d = false;
    } 
    
}
int mapWidth = 12;
int mapHeight = 21;
int[] map = new int[mapWidth * mapHeight];
int tileWidth = 32;
int tileHeight = 32;

int[] targetBgColors = 
{ 
    color(38, 35, 137),
    color(165, 64, 53),
    color(110, 0, 69)
};

int backgroundColor = targetBgColors[0];
int[] tileColors = new int[mapWidth * mapHeight]; // Keeps track of the color of all tiles
int targetColorIndex = 0;
boolean changeTargetNextFrame = false;
boolean waitedSomeFrames = false;
int bgChangeFrameCounter = 0;

// "Bubbles" refer to the floating spheres in the background
PVector[] bubbles = new PVector[20];
float[] bubbleSpeed = new float[bubbles.length];
PVector[] bubblesNoise = new PVector[bubbles.length];

// DISCLAIMER: the rendering code is horribly inefficient, enter at your own risk

public void createMap() 
{
    targetColorIndex = PApplet.parseInt(random(0, targetBgColors.length));
    
    //Create map border
    for(int y = 0; y < mapHeight; y++) 
    {
        
        for(int x = 0; x < mapWidth; x++)
        {
                
            if(x == 0 || x == mapWidth - 1 || y == mapHeight - 1) 
            {
                map[y * mapWidth + x] = 1;
                continue;
            }
            
            map[y * mapWidth + x] = 0;
        }
        
    }
    
    //Set tile colors
    for(int y = 0; y < mapHeight; y++) 
    {
        
        for(int x = 0; x < mapWidth; x++)
        {
            tileColors[y * mapWidth + x] = color(0, 0, 0, 255);
        }
        
    }
    
    //Create bubbles
    for(int i = 0; i < bubbles.length; i++) 
    {
        bubbles[i] = new PVector(random(resX, resX + resX), random(resY, resY + resY), random(-100, -40));
        bubbleSpeed[i] = random(1.0f, 4.0f);
        bubblesNoise[i] = new PVector(random(-100, 100), random(-100, 100));
    }
    
}

// Render the darkening effect around the edges of the screen
public void drawVignette()
{
    pushMatrix();
    translate(resX/2, resY/2);
    fillShape.setTexture(vignette);
    shape(fillShape);
    popMatrix();
}

// Render the bubbles in the background
public void drawBubbles()
{   
    
    if(initialPause || gameOver) return;
    
    // "Attempt" to make them look like bubbles by manipulating the light on them
    pushMatrix();
    ambientLight(255, 255, 255);
    lightSpecular(255, 255, 255);
    directionalLight(0, 0, 0, -1, 0, 0.5f);
    directionalLight(0, 0, 0, 1, 0, 0.5f);
    lightSpecular(255, 0, 0);
    directionalLight(0, 0, 0, -0.25f, 0.5f, -0.5f);
    lightSpecular(255, 0, 0);
    directionalLight(0, 0, 0, 0.5f, -0.75f, -0.5f);
    lightSpecular(0, 255, 0);
    directionalLight(0, 0, 0, 1, 0.25f, 0.5f);
    directionalLight(0, 0, 0, -1, 0.25f, 0.5f);
    bubbleShape.setStroke(0);
    bubbleShape.setFill(color(red(backgroundColor), green(backgroundColor), blue(backgroundColor), 50));
    bubbleShape.setShininess(300);
    bubbleShape.setAmbient(0);
    bubbleShape.setSpecular(color(255, 255, 255));
    
    for(int i = 0; i < bubbles.length; i++) 
    {
        pushMatrix();
        
        if(bubbles[i].x < -70 || bubbles[i].y < -70)
        {
            bubbles[i].set(random(resX, resX + resX), random(resY, resY + resY), random(-100, -40));
        }
        
        translate(bubbles[i].x, bubbles[i].y, bubbles[i].z);
        shape(bubbleShape);
        bubbles[i] = bubbles[i].add(-abs(noise(bubblesNoise[i].x) * bubbleSpeed[i]), -abs(noise(bubblesNoise[i].y) * bubbleSpeed[i]));
        popMatrix();
        bubblesNoise[i].x += random(0.001f, 0.05f);
        bubblesNoise[i].y += random(0.05f, 0.1f);
    }
    
    popMatrix();
}

// Just the background color
public void drawBackground()
{   
    background(backgroundColor);
    gradualChangeTo(targetBgColors[targetColorIndex]);
}
    
// Draw the map foreground, only the non-moving blocks
public void drawForeground()
{
    pushMatrix();
    translate(resX/2 - ((tileWidth * mapWidth) / 2), 68);
    translate(tileWidth/2, tileHeight/2);
    
    for(int y = 0; y < mapHeight; y++) 
    {
        
        for(int x = 0; x < mapWidth; x++) 
        {
            
            if(map[y * mapWidth + x] == 0) 
            {
                mapFillerShape.setStroke(color(0, 0, 0, 255));
                mapFillerShape.setFill(color(100, 100, 100, 100));
                shape(mapFillerShape);
            } 
            else 
            {
                push();
                boxShape.setStroke(color(0, 0, 0, 255));
                boxShape.setStrokeWeight(4);
                
                if(map[y * mapWidth + x] == 1) 
                {
                    boxShape.setFill(color(0, 0, 0, 255));
                } 
                else
                {
                    boxShape.setFill(tileColors[y * mapWidth + x]);
                }
                
                if(blocksToRemove.size() > 0) 
                {
                    
                    for(int j = 0; j < blocksToRemove.size(); j++)
                    {
                        
                        if((y * mapWidth + x) == blocksToRemove.get(j)) 
                        {
                            int curTileColor = tileColors[y * mapWidth + x];
                            
                            // A formula with random numbers to create the effect when clearing rows
                            curTileColor += (cos(curTileColor) * 3.14f * (2.71f - curTileColor) + (curTileColor * 4.66f)) * 0.0001f; 

                            boxShape.setFill(curTileColor);
                            tileColors[y * mapWidth + x] = curTileColor;
                        }
                        
                    }
                    
                }
                
                // Textures can be applied here
                //boxShape.setTexture(textures[map[y * mapWidth + x] - 1]);
                shape(boxShape);
                pop();
            }
            
            translate(tileWidth, 0);
        }
        
        translate(0, tileHeight);
        translate(-(tileWidth * mapWidth), 0);
    }
    
    popMatrix();
}

// This controls the gradual change to another color for the background
// The background will slowly shift to a specific target color while the game plays
public void gradualChangeTo(int targetColor)
{
    int changeSpeed = 10; // Higher is slower
    int dr = 0;
    int dg = 0;
    int db = 0;
    
    if(changeTargetNextFrame) 
    {
        targetColorIndex = PApplet.parseInt(random(0, targetBgColors.length));
        changeTargetNextFrame = false;
    } 
    else if(waitedSomeFrames)
    {
        dr = (red(targetColor) > red(backgroundColor)) ? 1 : (red(targetColor) == red(backgroundColor) ? 0 : -1);
        dg = (green(targetColor) > green(backgroundColor)) ? 1 : (green(targetColor) == green(backgroundColor) ? 0 : -1);
        db = (blue(targetColor) > blue(backgroundColor)) ? 1 : (blue(targetColor) == blue(backgroundColor) ? 0 : -1);
        
        if(backgroundColor == targetColor)
        {
            changeTargetNextFrame = true;
        }
        
        backgroundColor = color(red(backgroundColor) + dr, green(backgroundColor) + dg, blue(backgroundColor) + db);
        
        waitedSomeFrames = false;
        bgChangeFrameCounter = 0;
    } 
    else
    {
        
        if(bgChangeFrameCounter > changeSpeed) 
        {
            waitedSomeFrames = true;
        } 
        else 
        {
            bgChangeFrameCounter++;
        }
        
    }
  
}

public void drawPauseScreen()
{
    pushMatrix();
    translate(resX/2 - ((tileWidth) / 2), resY / 2 - (tileHeight * 3), 50);
    translate(tileWidth/2, tileHeight/2);
    shape(pauseTextBgShape);
    text("Press space to start", 0, 0, 51);
    popMatrix();
}

public void drawGameOverScreen()
{
    pushMatrix();
    translate(resX/2 - ((tileWidth) / 2), resY / 2 - (tileHeight * 3), 50);
    translate(tileWidth/2, tileHeight/2);
    shape(pauseTextBgShape);
    push();
    fill(200, 0, 0);
    textSize(30);
    text("GAME OVER!", 0, -10, 51);
    fill(255, 255, 255);
    textSize(15);
    text("Press space to continue", -2, 30, 51);
    pop();
    popMatrix();
}

public void drawInterface() 
{
    pushMatrix();
    
    translate(resX/2 - (tileWidth * 9), 72);
    translate(tileWidth/2, tileHeight/2);
    shape(scoreTextBgShape);
    
    push();
    text("Score", 20, -5, 51);
    textAlign(RIGHT);
    textSize(20);
    text("" + score, 80, 28, 51);
    pop();
    
    popMatrix();
    
    pushMatrix();
    
    translate(resX/2 + (tileWidth * 8), 72);
    translate(tileWidth/2, tileHeight/2);
    shape(timerTextBgShape);
    
    push();
    text("Time", -20, -5, 51);
    textSize(20);
    textAlign(LEFT);
    text("" + secondsSinceStart, -80, 28, 51);
    pop();
    
    popMatrix();
}
    
int currPieceType = 0;
int currPieceX = mapWidth / 2;
int currPieceY = -1;
int rotationState = 0;
int currPieceColor;

// Render the "ghost" piece that shows where your piece will land
public void drawGhostPiece() 
{
    if(initialPause || gameOver) return;
    
    int lastFitY = 0;
    
    for(int y = 0; y < mapHeight + 2; y++) 
    {
        
        if(checkIfPieceFits(currPieceX, currPieceY + y, rotationState))
        {
            lastFitY = currPieceY + y;
        }
        else
        {
            break;
        }
        
    }
    
    pushMatrix();
    translate(resX/2 - ((tileWidth * mapWidth) / 2), 68);
    translate(tileWidth / 2, tileHeight / 2);
    translate(tileWidth * currPieceX, tileHeight * lastFitY);
    
    for(int y = 0; y < 4; y++) 
    {
        
        for(int x = 0; x < 4; x++) 
        {

            if(tetrominoes[currPieceType].charAt(rotate(x, y, rotationState)) == 'X')
            {
                //boxShape.setTexture(textures[currPieceType + 1]);
                boxShape.setFill(color(255, 255, 255, 150));
                boxShape.setStroke(color(0, 0, 0, 0));
                shape(boxShape);
            }
            
            translate(tileWidth, 0);
        }

        translate(0, tileHeight);
        translate(-(tileWidth * 4), 0);
    }
    
    popMatrix();
}

// Draw current falling tetronimo
public void drawFallingPiece()
{
    if(initialPause) return;
    
    pushMatrix();
    translate(resX/2 - ((tileWidth * mapWidth) / 2), 68);
    translate(tileWidth / 2, tileHeight / 2);
    translate(tileWidth * currPieceX, tileHeight * currPieceY);
    
    for(int y = 0; y < 4; y++) 
    {
        
        for(int x = 0; x < 4; x++) 
        {

            if(tetrominoes[currPieceType].charAt(rotate(x, y, rotationState)) == 'X')
            {
                //boxShape.setTexture(textures[currPieceType + 1]);
                boxShape.setFill(currPieceColor);
                boxShape.setStroke(color(0, 0, 0, 255));
                shape(boxShape);
            }
            
            translate(tileWidth, 0);
        }

        translate(0, tileHeight);
        translate(-(tileWidth * 4), 0);
    }
    
    popMatrix();
}

public void getNewPiece()
{
    currPieceType = PApplet.parseInt(random(0, 6));
    rotationState = 0;
    currPieceX = mapWidth / 2 - 2;
    
    if(currPieceType == 0 || currPieceType == 5 || currPieceType == 6)
    {
        // Makes it a little bit more fair for long pieces that would usually spawn in contact with the top of the playfield
        currPieceY = -5; 
    }
    else
    {
        currPieceY = -4;
    }
    
    currPieceColor = backgroundColor;
    pushDownTimer = millis();
}
    static public void main(String[] passedArgs) {
        String[] appletArgs = new String[] { "Tetris" };
        if (passedArgs != null) {
          PApplet.main(concat(appletArgs, passedArgs));
        } else {
          PApplet.main(appletArgs);
        }
    }
}
