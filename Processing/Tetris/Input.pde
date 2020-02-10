boolean w = false, a = false, s = false, d = false, esc = false, space = false;
float downHoldDelay = 70; // ms between each downwards movement tick while holding the key
float downPressTime = 0;
float strafeHoldDelay = 120;
float strafePressTime = -1;
int dx = 0, dy = 0; //Delta x, delta y
int dr = 0; // Delta rotate

void checkInputs() 
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

void keyPressed()
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

void keyReleased()
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
