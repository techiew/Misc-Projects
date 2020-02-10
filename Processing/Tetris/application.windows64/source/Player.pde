int currPieceType = 0;
int currPieceX = mapWidth / 2;
int currPieceY = -1;
int rotationState = 0;
color currPieceColor;

// Render the "ghost" piece that shows where your piece will land
void drawGhostPiece() 
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
void drawFallingPiece()
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

void getNewPiece()
{
    currPieceType = int(random(0, 6));
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
