#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <time.h>
#include <fstream>
#include <sstream>
#include "utils.h"
#include "pieces.h"
#include "functionality.h"

using namespace std;
using namespace sf;

// --- Game States ---
enum GameState { MENU, PLAYING, INSTRUCTIONS, HIGHSCORE, PAUSED, GAMEOVER };

// --- Helper: Center text horizontally (Pure PF) ---
void centerText(Text& text, float y, float windowWidth) {
    FloatRect bounds = text.getGlobalBounds();
    text.setPosition({(windowWidth - bounds.size.x) / 2.0f, y});
}

// --- Helper: Check mouse hover ---
bool isMouseOver(Text& text, RenderWindow& window) {
    FloatRect bounds = text.getGlobalBounds();
    Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));
    return bounds.contains(mousePos);
}

int main() {
    int score = 0, highscore = 0, level = 1, delta_x = 0, colorNum = 1;
    int nextColorNum = 1 + rand() % 7, nextPieceType = rand() % 7; 
    float timer = 0, delay = 0.4f;
    bool rotate = false, highscoreUpdated = false;
    GameState currentState = MENU;
    int menuSelection = 0; 
    
    float winW = 500.f, winH = 480.f;
    srand(static_cast<unsigned int>(time(0)));

    ifstream readFile("highscore.txt");
    if (readFile.is_open()) { readFile >> highscore; readFile.close(); }

    RenderWindow window(VideoMode({static_cast<unsigned int>(winW), static_cast<unsigned int>(winH)}), "PF TETRIS");
    window.setFramerateLimit(60);

    Texture obj1, obj2, obj3;
    if (!obj1.loadFromFile("assets/img/tiles.png")) return -1;
    if (!obj2.loadFromFile("assets/img/background.png")) return -1;
    if (!obj3.loadFromFile("assets/img/frame.png")) return -1;
    
    Font font;
    if (!font.openFromFile("assets/font/OpenSans-Bold.ttf")) return -1;

    Music bgMusic;
    if (bgMusic.openFromFile("assets/audio/music.wav")) {
        bgMusic.setLooping(true); bgMusic.setVolume(20); bgMusic.play();
    }

    SoundBuffer lb, mb, cb;
    lb.loadFromFile("assets/audio/land.wav");
    mb.loadFromFile("assets/audio/move.flac");
    cb.loadFromFile("assets/audio/click.wav");
    Sound landingSound(lb), moveSound(mb), clickSound(cb);

    Sprite tiles(obj1), background(obj2), frame(obj3);
    
    // UI Text
    Text menuTitle(font, "TETRIS", 70),
         btnStart(font, "START GAME", 35),
         btnHighscore(font, "HIGH SCORES", 35),
         btnInstructions(font, "CONTROLS", 35),
         btnExit(font, "EXIT GAME", 35),
         btnBack(font, "BACK TO MENU", 25),
         instrTitle(font, "HOW TO PLAY", 40),
         instrText(font, "ARROWS: MOVE & ROTATE\nSPACE: HARD DROP\nP: PAUSE GAME\nESC: MENU", 20),
         scoreLabel(font, "SCORE", 18), scoreValue(font, "0", 25),
         highscoreLabel(font, "BEST", 18), highscoreValue(font, "0", 25),
         levelLabel(font, "LEVEL", 18), levelValue(font, "1", 25),
         nextLabel(font, "NEXT", 18),
         pauseLabel(font, "PAUSED", 50),
         gameoverLabel(font, "GAME OVER", 50),
         hsPageTitle(font, "HALL OF FAME", 40);

    menuTitle.setFillColor(Color::Cyan);
    scoreValue.setFillColor(Color::Red); highscoreValue.setFillColor(Color::Blue); levelValue.setFillColor(Color::Cyan);

    Clock clock;
    while (window.isOpen()) {
        float time = clock.getElapsedTime().asSeconds(); clock.restart();

        while (const std::optional event = window.pollEvent()) {
            if (event->is<Event::Closed>()) window.close();
            
            if (const auto* kp = event->getIf<Event::KeyPressed>()) {
                if (kp->code == Keyboard::Key::Escape) {
                    if (currentState == PLAYING) currentState = PAUSED; else currentState = MENU;
                }

                if (currentState == MENU) {
                    if (kp->code == Keyboard::Key::Up) { menuSelection = (menuSelection - 1 + 4) % 4; moveSound.play(); }
                    else if (kp->code == Keyboard::Key::Down) { menuSelection = (menuSelection + 1) % 4; moveSound.play(); }
                    else if (kp->code == Keyboard::Key::Enter) {
                        clickSound.play();
                        if (menuSelection == 0) { currentState = PLAYING; score = 0; level = 1; delay = 0.4f; for(int i=0; i<M; i++) for(int j=0; j<N; j++) grid[i][j]=0; }
                        else if (menuSelection == 1) currentState = HIGHSCORE;
                        else if (menuSelection == 2) currentState = INSTRUCTIONS;
                        else if (menuSelection == 3) window.close();
                    }
                }
                else if (currentState == HIGHSCORE || currentState == INSTRUCTIONS || currentState == GAMEOVER || currentState == PAUSED) {
                    if (kp->code == Keyboard::Key::Enter || kp->code == Keyboard::Key::Backspace) {
                        clickSound.play(); currentState = MENU;
                    }
                }

                if (currentState == PLAYING) {
                    if (kp->code == Keyboard::Key::P) currentState = PAUSED;
                    else if (kp->code == Keyboard::Key::Up) { rotate = true; moveSound.play(); }
                    else if (kp->code == Keyboard::Key::Left) { delta_x = -1; moveSound.play(); }
                    else if (kp->code == Keyboard::Key::Right) { delta_x = 1; moveSound.play(); }
                    else if (kp->code == Keyboard::Key::Space) { handleHardDrop(); timer = delay; landingSound.play(); }
                }
            }

            if (const auto* mb = event->getIf<Event::MouseButtonPressed>()) {
                if (mb->button == Mouse::Button::Left) {
                    if (currentState == MENU) {
                        if (isMouseOver(btnStart, window)) { currentState = PLAYING; clickSound.play(); score = 0; level = 1; delay = 0.4f; for(int i=0; i<M; i++) for(int j=0; j<N; j++) grid[i][j]=0; }
                        if (isMouseOver(btnHighscore, window)) { currentState = HIGHSCORE; clickSound.play(); }
                        if (isMouseOver(btnInstructions, window)) { currentState = INSTRUCTIONS; clickSound.play(); }
                        if (isMouseOver(btnExit, window)) window.close();
                    } else if (currentState == HIGHSCORE || currentState == INSTRUCTIONS || currentState == GAMEOVER || currentState == PAUSED) {
                        if (isMouseOver(btnBack, window)) { currentState = MENU; clickSound.play(); }
                    }
                }
            }
        }

        if (currentState == PLAYING) {
            timer += time; if (Keyboard::isKeyPressed(Keyboard::Key::Down)) delay = 0.05f;
            handleMovement(delta_x); handleRotation(rotate);
            if (handleFalling(timer, delay, colorNum, nextColorNum, nextPieceType)) landingSound.play();
            bool got = false; handleLineClearing(score, got); 
            if (got) { currentState = GAMEOVER; highscoreUpdated = false; }
            updateGhostPiece();
            level = 1 + (score / 500); delay = 0.4f - (level * 0.03f);
            if (delay < 0.1f) delay = 0.1f;
            delta_x = 0; rotate = false;
        }

        window.clear(Color::Black);
        if (currentState == MENU) {
            centerText(menuTitle, 40.f, winW); window.draw(menuTitle);
            btnStart.setFillColor((menuSelection == 0 || isMouseOver(btnStart, window)) ? Color::Cyan : Color::White);
            btnHighscore.setFillColor((menuSelection == 1 || isMouseOver(btnHighscore, window)) ? Color::Cyan : Color::White);
            btnInstructions.setFillColor((menuSelection == 2 || isMouseOver(btnInstructions, window)) ? Color::Cyan : Color::White);
            btnExit.setFillColor((menuSelection == 3 || isMouseOver(btnExit, window)) ? Color::Cyan : Color::White);
            centerText(btnStart, 160.f, winW); centerText(btnHighscore, 220.f, winW);
            centerText(btnInstructions, 280.f, winW); centerText(btnExit, 340.f, winW);
            window.draw(btnStart); window.draw(btnHighscore); window.draw(btnInstructions); window.draw(btnExit);
        } 
        else if (currentState == PLAYING || currentState == PAUSED || currentState == GAMEOVER) {
            window.draw(background);
            scoreLabel.setPosition({320.f, 40.f}); scoreValue.setPosition({320.f, 65.f});
            highscoreLabel.setPosition({320.f, 110.f}); highscoreValue.setPosition({320.f, 135.f});
            levelLabel.setPosition({320.f, 180.f}); levelValue.setPosition({320.f, 205.f});
            nextLabel.setPosition({320.f, 260.f});
            window.draw(scoreLabel); window.draw(scoreValue); window.draw(highscoreLabel); window.draw(highscoreValue);
            window.draw(levelLabel); window.draw(levelValue); window.draw(nextLabel);
            scoreValue.setString(to_string(score)); highscoreValue.setString(to_string(highscore)); levelValue.setString(to_string(level));

            for (int i = 0; i < M; i++) {
                for (int j = 0; j < N; j++) {
                    if (grid[i][j]) {
                        tiles.setTextureRect(IntRect({grid[i][j] * 18, 0}, {18, 18}));
                        tiles.setPosition({static_cast<float>(j * 18), static_cast<float>(i * 18)});
                        tiles.move({28.f, 31.f}); window.draw(tiles);
                    }
                }
            }
            if (currentState == PLAYING || currentState == PAUSED) {
                tiles.setColor(Color(255, 255, 255, 80));
                for (int i = 0; i < 4; i++) {
                    tiles.setTextureRect(IntRect({colorNum * 18, 0}, {18, 18}));
                    tiles.setPosition({static_cast<float>(ghostPiece[i].x * 18), static_cast<float>(ghostPiece[i].y * 18)});
                    tiles.move({28.f, 31.f}); window.draw(tiles);
                }
                tiles.setColor(Color::White);
                for (int i = 0; i < 4; i++) {
                    tiles.setTextureRect(IntRect({colorNum * 18, 0}, {18, 18}));
                    tiles.setPosition({static_cast<float>(currentPiece[i].x * 18), static_cast<float>(currentPiece[i].y * 18)});
                    tiles.move({28.f, 31.f}); window.draw(tiles);
                }
                for (int i = 0; i < 4; i++) {
                    tiles.setTextureRect(IntRect({nextColorNum * 18, 0}, {18, 18}));
                    tiles.setPosition({static_cast<float>(BLOCKS[nextPieceType][i] % 2 * 18), static_cast<float>(BLOCKS[nextPieceType][i] / 2 * 18)});
                    tiles.move({335.f, 310.f}); window.draw(tiles);
                }
            }
            if (currentState == PAUSED) {
                RectangleShape overlay({winW, winH}); overlay.setFillColor(Color(0, 0, 0, 150)); window.draw(overlay);
                centerText(pauseLabel, 150.f, winW); window.draw(pauseLabel);
                centerText(btnBack, 240.f, winW); btnBack.setFillColor(Color::Cyan); window.draw(btnBack);
            }
            if (currentState == GAMEOVER) {
                RectangleShape overlay({winW, winH}); overlay.setFillColor(Color(100, 0, 0, 180)); window.draw(overlay);
                centerText(gameoverLabel, 150.f, winW); window.draw(gameoverLabel);
                centerText(btnBack, 240.f, winW); btnBack.setFillColor(Color::Yellow); window.draw(btnBack);
                if (!highscoreUpdated) {
                    if (score > highscore) { highscore = score; ofstream writeFile("highscore.txt"); writeFile << highscore; writeFile.close(); }
                    highscoreUpdated = true;
                }
            }
            window.draw(frame);
        }
        else if (currentState == HIGHSCORE) {
            centerText(hsPageTitle, 80.f, winW); window.draw(hsPageTitle);
            highscoreValue.setCharacterSize(60); centerText(highscoreValue, 180.f, winW); window.draw(highscoreValue);
            centerText(btnBack, 350.f, winW); btnBack.setFillColor(Color::Cyan); window.draw(btnBack);
            highscoreValue.setCharacterSize(25);
        }
        else if (currentState == INSTRUCTIONS) {
            centerText(instrTitle, 40.f, winW); window.draw(instrTitle);
            instrText.setPosition({60.f, 130.f}); window.draw(instrText);
            centerText(btnBack, 380.f, winW); btnBack.setFillColor(Color::Cyan); window.draw(btnBack);
        }
        window.display();
    }
    return 0;
}
