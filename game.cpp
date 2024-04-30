#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>

float towerX = 0.0f;
float towerY = 0.0f;
int towerHealth = 100;

int numEnemies = 1; // Initial number of enemies
std::vector<std::pair<float, float>> enemies;
std::vector<float> enemySpeeds;

int score = 0;
int elapsedTime = 0; // Elapsed time since start

void drawTower() {
    glColor3f(0.5f, 0.5f, 0.5f); // Gray tower
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i += 5) {
        float angle = i * 3.14159265358979323846 / 180;
        glVertex2f(towerX + 0.5f * cos(angle), towerY + 0.5f * sin(angle));
    }
    glEnd();
}

void drawEnemy(float x, float y) {
    glColor3f(1.0f, 0.0f, 0.0f); // Red enemy
    glRectf(x - 0.25f, y - 0.25f, x + 0.25f, y + 0.25f);
}

void shootEnemy(float clickX, float clickY) {
    for (int i = 0; i < numEnemies; ++i) {
        if (sqrt(pow(clickX - enemies[i].first, 2) + pow(clickY - enemies[i].second, 2)) <= 0.25f) {
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
        std::cout << "Game Over!\n";
        exit(0);
    }
}

void draw() {
    glClear(GL_COLOR_BUFFER_BIT);
    drawTower();
    for (int i = 0; i < numEnemies; ++i) {
        if (i < enemies.size() && i < enemySpeeds.size()) {
            drawEnemy(enemies[i].first, enemies[i].second);
        }
    }
    glutSwapBuffers();
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        float clickX = ((float)x / glutGet(GLUT_WINDOW_WIDTH)) * 10.0f - 5.0f;
        float clickY = 5.0f - ((float)y / glutGet(GLUT_WINDOW_HEIGHT)) * 10.0f;
        shootEnemy(clickX, clickY);
    }
}

void update(int value) {
    // Increase elapsed time
    elapsedTime += 25;

    // Calculate the number of enemies to add based on elapsed time
    int enemiesToAdd = elapsedTime / 5000; // Add one enemy every 5 seconds
    if (enemiesToAdd > 0) {
        numEnemies += enemiesToAdd; // Add the calculated number of enemies
        elapsedTime %= 5000; // Reset elapsed time
    }

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
            }
        }

        // Create new enemies every 0.5 second if the vector has enough elements
        if (elapsedTime % 500 == 0 && i >= enemies.size() && i >= enemySpeeds.size()) {
            float angle = (float)rand() / RAND_MAX * 2 * 3.14159265358979323846;
            enemies.push_back(std::make_pair(5.0f * cos(angle), 5.0f * sin(angle)));
            enemySpeeds.push_back(static_cast<float>(rand() % 3) / 100.0f + 0.01f); // Random speed between 0.01 and 0.03
        }
    }

    glutPostRedisplay();
    glutTimerFunc(25, update, 0);
}

int main(int argc, char** argv) {
    srand(time(NULL));
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Tower Defense");
    glutDisplayFunc(draw);
    glutMouseFunc(mouse);
    glutTimerFunc(25, update, 0);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // White background
    gluOrtho2D(-5.0f, 5.0f, -5.0f, 5.0f); // Set up 2D orthographic viewing region

    glutMainLoop();
    return 0;
}