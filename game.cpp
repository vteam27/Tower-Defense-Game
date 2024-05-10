#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include<string>

using namespace std;

float towerX = 0.0f;
float towerY = 0.0f;
int towerHealth = 100;

bool gameOver = false;
bool gameStarted = false;

int numEnemies = 1; // Initial number of enemies
std::vector<std::pair<float, float>> enemies;
std::vector<float> enemySpeeds;

int score = 0;
int elapsedTime = 0; // Elapsed time since start

void drawStartScreen() {
    // Set the background color to green, same as the game background
    glClearColor(0.0f, 0.5f, 0.0f, 1.0f); // Green color
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw beige background box for the "TOWER DEFENSE" heading
    glColor3f(0.7f, 0.5f, 0.3f);; // Beige color
    glBegin(GL_QUADS);
    glVertex2f(-1.6f, 1.8f); // Top-left corner
    glVertex2f(1.6f, 1.8f);  // Top-right corner
    glVertex2f(1.6f, 1.2f);  // Bottom-right corner
    glVertex2f(-1.6f, 1.2f); // Bottom-left corner
    glEnd();

    // Draw "TOWER DEFENSE" heading in black font
    glColor3f(0.0f, 0.0f, 0.0f); // Black color
    glRasterPos2f(-1.3f, 1.4f); // Position of the heading
    string titleText = "TOWER DEFENSE";
    for (char c : titleText) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c); // Using a larger font
    }

    // Draw the shadow for the "Start Game" button
    glColor3f(0.5f, 0.5f, 0.5f); // Darker color for shadow
    glBegin(GL_POLYGON);
    glVertex2f(-2.2f, -2.2f);
    glVertex2f(2.2f, -2.2f);
    glVertex2f(2.2f, -3.2f);
    glVertex2f(-2.2f, -3.2f);
    glEnd();

    // Draw the "Start Game" button in blue with increased size
    glColor3f(0.7f, 0.5f, 0.3f); // color
    glBegin(GL_POLYGON);
    glVertex2f(-2.0f, -2.0f);
    glVertex2f(2.0f, -2.0f);
    glVertex2f(2.0f, -3.0f);
    glVertex2f(-2.0f, -3.0f);
    glEnd();

    // Calculate the center position for the "Start Game" text within the button
    string startText = "** Start Game! **";
    int textLength = startText.length();
    float textWidth = textLength * 0.2f; // Adjust as needed
    float textX = 0.5 - textWidth / 2.0f; // Center the text horizontally
    float textY = -2.6f; // Center the text vertically

    // Draw the "Start Game" text in white with increased font size, centered within the button
    glColor3f(0.0f, 0.0f, 0.0f); // color
    glRasterPos2f(textX, textY);
    for (char c : startText) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c); // Use a larger font
    }
}




void drawTower() {
    // Draw shrunk tower base
    glColor3f(0.3f, 0.15f, 0.05f); // Dark Brown
    glBegin(GL_QUADS);
    glVertex2f(-0.3f, -0.3f);
    glVertex2f(0.3f, -0.3f);
    glVertex2f(0.3f, 0.3f);
    glVertex2f(-0.3f, 0.3f);
    glEnd();

    // Draw shrunk tower middle section
    glColor3f(0.7f, 0.7f, 0.7f); // Light Gray
    glBegin(GL_QUADS);
    glVertex2f(-0.2f, 0.3f);
    glVertex2f(0.2f, 0.3f);
    glVertex2f(0.2f, 0.8f);
    glVertex2f(-0.2f, 0.8f);
    glEnd();

    // Draw shrunk tower top section
    glColor3f(0.1f, 0.1f, 0.5f); // Dark Blue
    glBegin(GL_QUADS);
    glVertex2f(-0.15f, 0.8f);
    glVertex2f(0.15f, 0.8f);
    glVertex2f(0.15f, 1.1f);
    glVertex2f(-0.15f, 1.1f);
    glEnd();

    // Draw small rectangles on top
    glColor3f(0.2f, 0.2f, 0.2f);
    glBegin(GL_QUADS);
    for (int i = 0; i < 3; ++i) {
        float x = -0.11f + i * 0.1f;
        glVertex2f(x, 1.1f);
        glVertex2f(x + 0.02f, 1.1f);
        glVertex2f(x + 0.02f, 1.2f);
        glVertex2f(x, 1.2f);
    }
    glEnd();
}



void drawEnemy(float x, float y) {
    glPushMatrix();
    glTranslatef(x, y, 0.0f); // Translate to the position of the enemy

    // Draw the man

    // Head (circular shape)
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i += 10) {
        float angle = i * 3.14159265358979323846 / 180.0;
        glVertex2f(0.1 * cos(angle), 0.25 + 0.1 * sin(angle)); // Adjusted y-coordinate
    }
    glEnd();

    // Body (rectangle overlapping with head)
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_POLYGON);
    glVertex2f(-0.1f, -0.1); // Adjusted x-coordinate
    glVertex2f(0.1f, -0.1); // Adjusted x-coordinate
    glVertex2f(0.1f, 0.35);
    glVertex2f(-0.1f, 0.35);
    glEnd();

    // Arms (rectangles on an angle)
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_POLYGON);
    glVertex2f(-0.15, 0.0);
    glVertex2f(-0.05, 0.1);
    glVertex2f(-0.05, 0.2);
    glVertex2f(-0.15, 0.1);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2f(0.05, 0.1);
    glVertex2f(0.15, 0.0);
    glVertex2f(0.15, 0.1);
    glVertex2f(0.05, 0.2);
    glEnd();

    // Legs (rectangles attached from the body)
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_POLYGON);
    glVertex2f(-0.1f, -0.1); // Adjusted x-coordinate
    glVertex2f(0.0f, -0.1);
    glVertex2f(0.0f, -0.3);
    glVertex2f(-0.1f, -0.3); // Adjusted x-coordinate
    glEnd();

    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_POLYGON);
    glVertex2f(0.01f, -0.1); // Adjusted x-coordinate
    glVertex2f(0.1f, -0.1); // Adjusted x-coordinate
    glVertex2f(0.1f, -0.3);
    glVertex2f(0.01f, -0.3); // Adjusted x-coordinate
    glEnd();

    glPopMatrix();

}

void shootEnemy(float clickX, float clickY) {
    for (int i = 0; i < enemies.size(); ++i) {
        float distance = sqrt(pow(clickX - enemies[i].first, 2) + pow(clickY - enemies[i].second, 2));
        if (distance <= 0.40f) {
            score += 10; // Increase score
            enemies.erase(enemies.begin() + i); // Remove enemy from vector
            enemySpeeds.erase(enemySpeeds.begin() + i); // Remove enemy speed from vector
            numEnemies--; // Decrement number of enemies
            return; // Exit the function after removing the enemy
        }
    }
}

void enemyReachedTower() {
    towerHealth -= 10;
    if (towerHealth <= 0) {
        // std::cout << "Game Over!\n";
        gameOver = true;
    }
}
void drawButton(float x, float y, float width, float height, const string& text, bool active) {
    float normalColor[3] = { 0.5f, 0.5f, 0.5f };
    float pushedUpColor[3] = { 0.7f, 0.7f, 0.7f };
    float pushedDownColor[3] = { 0.3f, 0.3f, 0.3f };
    float* buttonColor = active ? pushedUpColor : pushedDownColor;

    glColor3fv(buttonColor);
    glBegin(GL_POLYGON);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();

    glColor3f(0.0f, 0.0f, 0.0f);
    glRasterPos2f(x + 0.2f, y + 0.2f);
    for (char c : text) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }

    if (!active) {
        glColor3f(0.0f, 0.0f, 0.0f);
        glBegin(GL_POLYGON);
        glVertex2f(x + width, y);
        glVertex2f(x + width + 0.02f, y - 0.02f);
        glVertex2f(x + width + 0.02f, y + height - 0.02f);
        glVertex2f(x + width, y + height);
        glEnd();

        glBegin(GL_POLYGON);
        glVertex2f(x, y);
        glVertex2f(x - 0.02f, y - 0.02f);
        glVertex2f(x + width - 0.02f, y - 0.02f);
        glVertex2f(x + width, y);
        glEnd();
    }
}




void applyPowerUp(int type) {
    if (type == 1 && score >= 150) {
        int numToRemove = min(5, static_cast<int>(enemies.size()));
        enemies.erase(enemies.begin(), enemies.begin() + numToRemove);
        enemySpeeds.erase(enemySpeeds.begin(), enemySpeeds.begin() + numToRemove);

        score -= 150;
    }
    else if (type == 2 && score >= 300) {  // Regen health
        towerHealth = 100;
        score -= 300;
    }
    else if (type == 3 && score >= 500) {  // Full attack
        enemies.clear();
        enemySpeeds.clear();
        numEnemies = 0;
        score -= 500;
    }
}



void drawGameOver() {
    // Set the background color to black for the game over screen
    glColor3f(0.0f, 0.0f, 0.0f); // Black color
    glBegin(GL_QUADS);
    glVertex2f(-5.0f, -5.0f);
    glVertex2f(-5.0f, 5.0f);
    glVertex2f(5.0f, 5.0f);
    glVertex2f(5.0f, -5.0f);
    glEnd();

    // Draw "Game Over" text in red with larger font size and shifted position
    glColor3f(1.0f, 0.0f, 0.0f); // Red color
    glRasterPos2f(-0.7f, 1.0f); // Shifted upwards and adjusted for larger text
    string gameOverText = "Game Over";
    for (char c : gameOverText) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c); // Use a larger GLUT font if available or scale accordingly
    }

    // Draw restart button with a small white shadow
    glColor3f(0.0f, 1.0f, 0.0f); // Green color
    glBegin(GL_POLYGON);
    glVertex2f(-1.5f, -1.5f);
    glVertex2f(1.5f, -1.5f);
    glVertex2f(1.5f, -2.5f);
    glVertex2f(-1.5f, -2.5f);
    glEnd();

    // Small white shadow
    glColor3f(1.0f, 1.0f, 1.0f); // White color for the shadow
    glBegin(GL_QUADS);
    glVertex2f(1.5f, -2.5f);
    glVertex2f(1.55f, -2.55f);
    glVertex2f(1.55f, -1.55f);
    glVertex2f(1.5f, -1.5f);
    glEnd();

    glBegin(GL_QUADS);
    glVertex2f(-1.5f, -2.5f);
    glVertex2f(-1.45f, -2.55f);
    glVertex2f(1.55f, -2.55f);
    glVertex2f(1.5f, -2.5f);
    glEnd();

    // Draw restart button text with a larger font size
    glColor3f(0.0f, 0.0f, 0.0f); // White color for visibility
    glRasterPos2f(-0.4f, -2.1f); // Centered position for the restart button text
    string restartText = "Restart";
    for (char c : restartText) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
    }
}


void drawHealthBar(float health) {
    // Draw background for the health bar
    glColor3f(0.3f, 0.3f, 0.3f); // Dark gray color
    glBegin(GL_QUADS);
    glVertex2f(-4.5f, 4.2f); // Bottom-left vertex
    glVertex2f(4.5f, 4.2f); // Bottom-right vertex
    glVertex2f(4.5f, 4.4f); // Top-right vertex
    glVertex2f(-4.5f, 4.4f); // Top-left vertex
    glEnd();

    // Draw health bar based on tower's health
    glColor3f(0.0f, 1.0f, 0.0f); // Green color for healthy part
    glBegin(GL_QUADS);
    glVertex2f(-4.5f, 4.2f); // Bottom-left vertex
    glVertex2f(-4.5f + 9.0f * health / 100.0f, 4.2f); // Bottom-right vertex
    glVertex2f(-4.5f + 9.0f * health / 100.0f, 4.4f); // Top-right vertex
    glVertex2f(-4.5f, 4.4f); // Top-left vertex
    glEnd();
}
void draw() {
    glClear(GL_COLOR_BUFFER_BIT);


    if (!gameStarted) { // Draw the start screen if the game has not started
        drawStartScreen();
    }
    else if (gameOver) {
        drawGameOver();
    }
    else {


        if (gameOver) {
            drawGameOver();
            glutSwapBuffers();
            return;
        }
        else {
            // New green background
            glColor3f(0.0f, 0.5f, 0.0f); // Green color
            glBegin(GL_POLYGON);
            glVertex2f(-5.0f, -5.0f);
            glVertex2f(-5.0f, 5.0f);
            glVertex2f(5.0f, 5.0f);
            glVertex2f(5.0f, -5.0f);
            glEnd();

            // Draw score on the left side of the screen
            glColor3f(0.0f, 0.0f, 0.0f); // Red color
            glRasterPos2f(-4.5f, 4.5f); // Position for the score text
            string scoreText = "Score: " + to_string(score);
            for (char c : scoreText) {
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
            }

            // Draw tower health on the right side of the screen
            glColor3f(0.0f, 0.0f, 0.0f); // Red color
            glRasterPos2f(2.5f, 4.5f); // Right-aligned position for the tower health text
            string healthText = "Tower Health: " + to_string(towerHealth);
            for (char c : healthText) {
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
            }

            // Draw health bar
            drawHealthBar(towerHealth);

            // Position power-up buttons above the health bar
            float buttonWidth = 1.0f;
            float buttonHeight = 0.5f;
            float buttonGap = 0.1f;
            float startX = -0.5f * (3 * buttonWidth + 2 * buttonGap); // Centered
            float startY = 3.6f; // Shifted above the health bar

            drawButton(startX, startY, buttonWidth, buttonHeight, "Bomb", score >= 150);
            drawButton(startX + buttonWidth + buttonGap, startY, buttonWidth, buttonHeight, "Regen", score >= 300);
            drawButton(startX + 2 * (buttonWidth + buttonGap), startY, buttonWidth, buttonHeight, "Attack", score >= 500);

            // Draw tower after power-up buttons to ensure it's on top
            drawTower();
            for (int i = 0; i < numEnemies; ++i) {
                if (i < enemies.size() && i < enemySpeeds.size()) {
                    drawEnemy(enemies[i].first, enemies[i].second);
                }
            }
        }
    }

    glutSwapBuffers();
}






void mouse(int button, int state, int x, int y) {
    if (!gameStarted) { // Check if the game has not started
        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
            // Check if the click is within the "Start Game" button
            float clickX = ((float)x / glutGet(GLUT_WINDOW_WIDTH)) * 10.0f - 5.0f;
            float clickY = 5.0f - ((float)y / glutGet(GLUT_WINDOW_HEIGHT)) * 10.0f;
            if (clickX >= -1.5f && clickX <= 1.5f && clickY >= -2.5f && clickY <= -1.5f) {
                // Reset game variables
                gameOver = false;
                towerHealth = 100;
                score = 0;
                enemies.clear();
                enemySpeeds.clear();
                numEnemies = 1;
                elapsedTime = 0;
                gameStarted = true; // Start the game
            }
        }
    }
    else {
        // Your existing mouse() code goes here
        if (gameOver) {
            if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
                // Check if the click is within the restart button
                float clickX = ((float)x / glutGet(GLUT_WINDOW_WIDTH)) * 10.0f - 5.0f;
                float clickY = 5.0f - ((float)y / glutGet(GLUT_WINDOW_HEIGHT)) * 10.0f;
                if (clickX >= -1.5f && clickX <= 1.5f && clickY >= -2.5f && clickY <= -1.5f) {
                    // Reset game variables
                    gameOver = false;
                    towerHealth = 100;
                    score = 0;
                    enemies.clear();
                    enemySpeeds.clear();
                    numEnemies = 1;
                    elapsedTime = 0;
                }
            }
        }
        else {
            if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
                float clickX = ((float)x / glutGet(GLUT_WINDOW_WIDTH)) * 10.0f - 5.0f;
                float clickY = 5.0f - ((float)y / glutGet(GLUT_WINDOW_HEIGHT)) * 10.0f;

                // Check if the click is on any of the power-up buttons
                float buttonWidth = 1.0f;
                float buttonHeight = 0.5f;
                float buttonGap = 0.1f;
                float startX = -0.5f * (3 * buttonWidth + 2 * buttonGap); // Centered
                float startY = 3.6f; // Shifted above the health bar

                if (clickY >= startY && clickY <= startY + buttonHeight) {
                    if (clickX >= startX && clickX <= startX + buttonWidth && score >= 150) applyPowerUp(1);  // Bomb
                    else if (clickX >= startX + buttonWidth + buttonGap && clickX <= startX + 2 * buttonWidth + buttonGap && score >= 300) applyPowerUp(2);  // Regen Health
                    else if (clickX >= startX + 2 * (buttonWidth + buttonGap) && clickX <= startX + 3 * buttonWidth + 2 * buttonGap && score >= 500) applyPowerUp(3);  // Full Attack
                }
                else {
                    shootEnemy(clickX, clickY);
                }
            }
        }
    }
}


void update(int value) {
    // Increase elapsed time
    elapsedTime += 25;

    // Calculate the number of enemies to add based on elapsed time
    // int enemiesToAdd = elapsedTime / 5000; // Add one enemy every 5 seconds

    // cout << enemiesToAdd << endl;

    // if (enemiesToAdd > 0) {
    //     numEnemies += enemiesToAdd; // Add the calculated number of enemies
    //     // elapsedTime %= 5000; // Reset elapsed time
    // }

    if (elapsedTime % 800 == 0) {
        numEnemies = numEnemies + min(max(1, elapsedTime / 10000), 2); // Add one enemy
    }

    // Move existing enemies and create new ones
    // Move existing enemies and create new ones
    for (int i = 0; i < numEnemies; ++i) {
        // Move existing enemies towards the center if the vector has enough elements
        if (i < enemies.size() && i < enemySpeeds.size()) {
            float deltaX = towerX - enemies[i].first;
            float deltaY = towerY - enemies[i].second;
            float magnitude = sqrt(deltaX * deltaX + deltaY * deltaY);
            enemies[i].first += deltaX / magnitude * enemySpeeds[i];
            enemies[i].second += deltaY / magnitude * enemySpeeds[i];

            // Check if enemy reached tower
            if (sqrt(pow(towerX - enemies[i].first, 2) + pow(towerY - enemies[i].second, 2)) <= 0.5f) {
                enemyReachedTower();
                enemies.erase(enemies.begin() + i); // Remove enemy from vector
                enemySpeeds.erase(enemySpeeds.begin() + i); // Remove enemy speed from vector
                numEnemies--; // Decrement number of enemies
                // Adjust loop counter and iterator to account for the removed enemy
                i--;
                continue; // Skip rest of the iteration
            }
        }

        // Create new enemies every 0.8 second if the vector has enough elements
        if (elapsedTime % 800 == 0 && i >= enemies.size() && i >= enemySpeeds.size()) {
            float angle = (float)rand() / RAND_MAX * 2 * 3.14159265358979323846;
            enemies.push_back(std::make_pair(5.0f * cos(angle), 5.0f * sin(angle)));
            enemySpeeds.push_back(static_cast<float>(rand() % 3) / 100.0f + 0.01f); // Random speed between 0.01 and 0.03
        }
    }

    // if (towerHealth <= 0) {
    //     cout << "Game Over!\n";
    //     exit(0);
    // }


    glutPostRedisplay();
    glutTimerFunc(25, update, 0);
}

int main(int argc, char** argv) {
    srand(time(NULL));
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowPosition(500, 100);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Tower Defense");

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glutDisplayFunc(draw);
    glutMouseFunc(mouse);
    glutTimerFunc(25, update, 0);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // White background
    gluOrtho2D(-5.0f, 5.0f, -5.0f, 5.0f); // Set up 2D orthographic viewing region



    glutMainLoop();
    return 0;
}
