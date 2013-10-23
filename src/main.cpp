/** Brick Breaker Game - Por Lauro Augusto S. Rezende **/

#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <windows.h>
#include <time.h>
#include "easy.h"
#include "string2.h"

/* limites da tela */
#define MIN_X 0
#define MAX_X 79
#define MIN_Y 0
#define MAX_Y 40

/* skins dos objetos no jogo */
#define BALL_SKIN 'o'
#define BRICK_SKIN '#'
#define STAR_SKIN '*'
#define LIFE_SKIN '\3'
#define BUFF_SKIN '@'
#define SELECTION_SKIN '>'
#define PLATFORM_SKIN '_'

/* cor dos objetos no jogo */
#define DEFAULT_COLOR WHITE_COLOR
#define BALL_COLOR randomColor(BLACK_COLOR)
#define BRICK_COLOR DEFAULT_COLOR
#define STAR_COLOR DARK_PINK_COLOR
#define LIFE_COLOR RED_COLOR
#define BUFF_COLOR CYAN_COLOR
#define SELECTION_COLOR BLUE_COLOR
#define PLATFORM_COLOR DEFAULT_COLOR
#define ALTERNATIVE_COLOR DARK_CYAN_COLOR
#define HUD_BACKGROUND_COLOR WHITE_COLOR

/* teclas de entrada */
#define UP 'w'
#define DOWN 's'
#define LEFT 'a'
#define RIGHT 'd'
#define SELECT 32
#define ENTER 13
#define PAUSE 27
#define ESCAPE 27
#define DIAGONAL_LEFT 'q'
#define DIAGONAL_RIGHT 'e'

/* level */
#define INITIAL_LEVEL 1
#define INITIAL_PLAYER_LIFES 2
#define MAX_LEVEL 5
#define PLAYER_NAME_SIZE 16
#define BRICK_LIMIT MAX_X * MAX_Y

/* delay */
#define GAME_DELAY 10
#define LOADING_DELAY 10

/* entidade */
#define ENTITY_LIMIT 20
#define ENTITY_ON_SCREEN_TIME 10000
#define INITIAL_UPDATE_ENTITIES_EVERY_GAME_TICK 5
#define CHANCE_OF_SPAWNING_RANDOM_ENTITY 30

/* plataforma */
#define INITIAL_PLATFORM_SIZE 20
#define PLATFORM_BUFF_SIZE 10
#define PLATFORM_BUFF_TIME 10000

/* score */
#define SCORE_BUFF_TIME 10000
#define SHOW_UPDATED_SCORE_AMOUNT_TIME 2000

/* frame */
#define FRAME_COLOR BLUE_COLOR
#define FRAME_X_SKIN '-'
#define FRAME_Y_SKIN '|'

/* coordenadas */
#define X 0
#define Y 1

/** variáveis globais **/

// entrada atual do teclado
char input;

// início e fim da posição da plataforma
int platform[2];

// tempo de buff da plataforma // quantidade de buffs que a plataforma contém
int platformBuffTimer, buffsOnPlatformAmount;

// quantidade de elementos na tela
int entitiesOnScreen;

// quantidade de bolas na tela
int ballsOnScreen;

// mapeamento de tijolos na tela // quebrou ou não tijolo no frame
bool brick[BRICK_LIMIT], brokeBrick;

// quantidade de tijolos na tela // limite que eles podem chegar // posição em que quebrou // quantidade de linha de tijolos
int bricksOnScreen, brickBoundaries, brokenBrickPosition[2], brickLines;

// mover os elementos na tela a cada x gameTicks
int updateEntitiesEveryGameTick;

// quantidade de vidas do jogador
int playerLifes;

// se verdadeiro a bola muda de direcao baseado no lugar em que bateu na plataforma
bool changeDirectionBasedOnPlatform;

// quantidade de pontos acumulados pelo jogador // quantidade somada ao score anteriormente // tempo de buff do score // o tempo que o hud mostra os pontos ganhos // pontos somados ao score
int score, scoreUpdate, scoreBuffTimer, scoreShowUpdateTimer, scoreIncrement;

// posições do rastro deixado pela super bola
int superBallTrail[2][5]; /// implement

// nome do jogador
char playerName[16];

// mensagens da tela
char message[][100] = { "BRICK BREAKER", "Jogar", "Ver scores", "Sair", "Pausado", "Level", "A e D para mover a platforma", // 0
                        "Q e E para trocar a direcao da bola", "Espaco para lancar a bola", "Carregando %.1f%%", "Game Over", // 7
                        "Voce possui %i vida", "Deseja continuar?", "Deseja voltar ao menu inicial?", "Voce nao possui vidas para continuar", // 11
                        "Erro. Arquivo level.txt nao encontrado", "Pressione espaco para refazer o menu", "ESC para pausar o jogo", "Score", // 15
                        "Digite seu nome", "Tamanho maximo alcancado!", "Score: %05i ponto%c", "Veja pelo lado bom",  // 19
                        "Voce se superou, com o antigo score de %05i ponto%s", "Voce superou o jogador %s com o score de %05i ponto%s", // 23
                        "N", "Jogador", "Score", "Anterior", "Proximo" // 25
                      };

/** declaração das funções **/

// centraliza determinado elemento
void gotoCenter(int size, int y);

// imprime nas dadas posições, a skin recebida pelo parâmetro
void printSkin(int x, int y, char skin, int color);

// imprime espaços nas posições x e y, se forem positivas
void printSpace(int x, int y, int n);

// move o elemento
bool updateEntity(int i);

// move a plataforma
void movePlatform();

// atualiza a plataforma referente a mudanças de tamanho
void updatePlatform();

// atualiza o tamanho da plataforma
void updatePlatformSize(int size);

// checa por input no buffer do teclado
char newInput();

// insere novo elemento na tela
void insertEntity(int x, int y, int mX, int mY, char type);

// remove um elemento da tela
void removeEntity(int i);

// gera o level informado pelos parâmetros
bool generateLevel(int level);

// move a bola conforme a plataforma e ajusta o lado que ela será lançada
void launchBall(bool setPlatform);

// atualiza os tijolos na tela, imprimindo-os novamente
void updateBricks();

// pausa o jogo
bool pauseGame();

// seta as variáveis globais aos seus valores iniciais
void setVariables();

// retorna a posição centralizada de acordo com o eixo informado
int getCenter(int axis, int adjust);

// faz uma espécie de janela nos limites especificados
void generateFrame(int initX, int finalX, int initY, int finalY);

// chance de spawnar um novo elemento na tela
void randomEntitySpawn(int newBall, int newStar, int newLife);

// insere tijolos no brick[]
void mapBrick(int x, int y, int size);

// imprime o hud do jogo
void printHud();

// retorna true se for game over e false se não
bool gameOver();

// imprime o menu na tela mapeando os caracteres no brick[]
void buildMenu();

// retorna o hash do x e y informados
int getHashPosition(int x, int y);

// retorna o x do centro da plataforma
int getCenterOfPlatform();

// deleta o frame nas posições informadas, incluindo seu conteúdo
void deleteFrame(int initX, int finalX, int initY, int finalY);

// atualiza o score com a quantidade informada
void updateScore(int amount);

// pega o nome do jogador
void getPlayerName();

// salva o score do jogador
void saveScore();

// imprime o arquivo record.txt
void printRecord();

struct Element {
    // o tipo do elemento é determinado pela sua skin
    char type;

    // posição do elemento na tela
    int x, y;

    // aceleração x e y
    int momentum[2];

    // tempo de vida
    int despawnTimer;

    // a cor da skin
    int color;
};

// vetor de entidades
Element entity[ENTITY_LIMIT];

int main() {
    bool wantsQuit = false, gotoMenu;

    do {
        gotoMenu = false;
        system("cls");
        setVariables();
        srand(time(NULL));

        /** menu **/
        buildMenu();

        insertEntity(rand() % MAX_X, 0, 1, 1, BALL_SKIN);
        int textID = 1, arrowPos[2] = { getCenter(X, strlen(message[textID])) - 2, getCenter(Y, 1) };
        printSkin(arrowPos[X], arrowPos[Y], SELECTION_SKIN, SELECTION_COLOR);
        mapBrick(arrowPos[X], arrowPos[Y], 1);

        do {
            for (int i = 0; i < entitiesOnScreen; i++) {
                updateEntity(i);
            }
            randomEntitySpawn(100, 0, 0);

            if (bricksOnScreen < 50) {
                gotoxy(MAX_X - strlen(message[16]), MAX_Y);
                printf(message[16]);
            }

            input = newInput();
            if (input) {
                printSpace(arrowPos[X], arrowPos[Y], 1);
                if (input == UP) {
                    if (arrowPos[Y] == getCenter(Y, 1)) {
                        arrowPos[Y] = getCenter(Y, 3);
                        textID = 3;
                    } else {
                        arrowPos[Y]--;
                        textID--;
                    }
                } else if (input == DOWN) {
                    if (arrowPos[Y] == getCenter(Y, 3)) {
                        arrowPos[Y] = getCenter(Y, 1);
                        textID = 1;
                    } else {
                        arrowPos[Y]++;
                        textID++;
                    }
                } else if (input == SELECT) {
                    bricksOnScreen = 0;
                    buildMenu();
                    printSpace(MAX_X - strlen(message[16]), MAX_Y, strlen(message[16]));
                }
                arrowPos[X] = getCenter(X, strlen(message[textID])) - 2;
                mapBrick(arrowPos[X], arrowPos[Y], 1);
                printSkin(arrowPos[X], arrowPos[Y], SELECTION_SKIN, SELECTION_COLOR);
            }
            Sleep(50);
        } while (input != ENTER);
        /** fim menu **/


        /** jogar **/
        if (arrowPos[Y] == getCenter(Y, 1)) {
            system("cls");

            bool wantsRetry = false;
            score = 0;

            for (int level = INITIAL_LEVEL; level <= MAX_LEVEL && !gotoMenu; level++) {

                // seta as variáveis globais para seus valores iniciais
                if (!wantsRetry) {
                    setVariables();

                    // gera o level atual, se o arquivo level.txt não existir, fecha o programa
                    if (!generateLevel(level)) {
                        return 0;
                    }

                    launchBall(true);

                } else {
                    wantsRetry = false;
                    // remove todos os elementos da tela
                    for (int i = entitiesOnScreen - 1; i >= 0; i--) {
                        removeEntity(i);
                    }
                    updateBricks();
                    launchBall(false);
                }

                updateEntitiesEveryGameTick = INITIAL_UPDATE_ENTITIES_EVERY_GAME_TICK - (level - INITIAL_LEVEL);

                /** game loop **/
                for (size_t gameTicks = 0; bricksOnScreen && !gotoMenu; gameTicks++) {

                    if (!(gameTicks % updateEntitiesEveryGameTick)) {
                        for (int i = 0; i < entitiesOnScreen; i++) {
                            // se a bola mover
                            if (updateEntity(i)) {
                                randomEntitySpawn(50, 25, 25);

                            // testa se não existe nenhuma outra bola na tela
                            } else if (!ballsOnScreen) {
                                if (gameOver()) {

                                    gotoMenu = true;
                                } else {
                                    wantsRetry = true;
                                }
                                break;
                            }
                        }
                        // checa se o jogador quis continuar
                        if (wantsRetry) {
                            level--;
                            break; // quebra o game loop
                        }
                        updatePlatform();
                        if (scoreBuffTimer) {
                            scoreBuffTimer--;
                            if (!scoreBuffTimer) {
                                scoreIncrement = 1;
                            }
                        }

                        // imprime o HUD
                        printHud();
                    }

                    // checa por input
                    input = newInput();
                    if (input == PAUSE) {
                        gotoMenu = pauseGame();
                    } else {
                        movePlatform();
                    }
                    Sleep(GAME_DELAY);
                }
            }
        }

        /** scores **/
        else if (arrowPos[Y] == getCenter(Y, 2)) {
            printRecord();

        /** sair **/
        } else {
            wantsQuit = true;
        }
    } while (!wantsQuit);
}

/** implementação das funções **/

void buildMenu() {

    // gera o frame em volta do nome do jogo
    generateFrame(getCenter(X, strlen(message[0])) - 2, getCenter(X, - strlen(message[0])) + 2, getCenter(Y, - 3) - 2, getCenter(Y, - 3) + 2);
    mapBrick(getCenter(X, strlen(message[0])) - 2, getCenter(Y, - 3) - 2, (getCenter(X, - strlen(message[0])) + 2) - (getCenter(X, strlen(message[0])) - 2));
    mapBrick(getCenter(X, strlen(message[0])) - 2, getCenter(Y, - 3) + 2, (getCenter(X, - strlen(message[0])) + 2) - (getCenter(X, strlen(message[0])) - 2));

    for (int y = getCenter(Y, - 3) - 2; y <= getCenter(Y, - 3) + 2; y++) {
        mapBrick(getCenter(X, strlen(message[0])) - 2, y, 1);
    }
    for (int y = getCenter(Y, - 3) - 2; y <= getCenter(Y, - 3) + 2; y++) {
        mapBrick(getCenter(X, - strlen(message[0])) + 2, y, 1);
    }

    mapBrick(getCenter(X, strlen(message[0])), getCenter(Y, - 3), strlen(message[0]));
    gotoCenter(strlen(message[0]), - 3);
    printf(message[0]);

    mapBrick(getCenter(X, strlen(message[1])), getCenter(Y, 1), strlen(message[1]));
    gotoCenter(strlen(message[1]), 1);
    printf(message[1]);

    mapBrick(getCenter(X, strlen(message[2])), getCenter(Y, 2), strlen(message[2]));
    gotoCenter(strlen(message[2]), 2);
    printf(message[2]);

    mapBrick(getCenter(X, strlen(message[3])), getCenter(Y, 3), strlen(message[3]));
    gotoCenter(strlen(message[3]), 3);
    printf(message[3]);

    platform[0] = MIN_X;
    platform[1] = MAX_X;
    changeDirectionBasedOnPlatform = false;
}

int getHashPosition(int x, int y) {
    return x + ((MAX_X + 1) * y);
}

void printSpace(int x, int y, int n) {
    if (x >= MIN_X && y >= MIN_Y) {
        gotoxy(x, y);
    }
    for (int i = 0; i < n; i++) {
        printf(" ");
    }
}

char newInput() {
    if (kbhit()) {
        return getch();
    }
    return 0;
}

void printSkin(int x, int y, char skin, int color) {

    // move o cursor se as coordenadas forem positivas
    if (x >= MIN_X && y >= MIN_Y) {
        gotoxy(x, y);
    }

    // muda a cor para a constante do elemento
    setTextColor(color);
    printf("%c", skin);

    setTextColor(DEFAULT_COLOR);
}

bool updateEntity(int i) { ///*********************************************///

    int x = 0, y = 0;
    bool hasMoved = true;

    /// move elementos do tipo bola
    if (entity[i].type == BALL_SKIN) {

        bool changedDirection[2] = { false, false };

        /// analisa colisões com ambiente *NEW*
        if (entity[i].momentum[X] > 0) {
            if (entity[i].x == MAX_X) {
                entity[i].momentum[X] *= - 1;
                changedDirection[X] = true;
                x = entity[i].x;
            } else
                x = entity[i].x + 1;
        }
        else if (entity[i].momentum[X] < 0) {
            if (entity[i].x == MIN_X + 1) {
                entity[i].momentum[X] *= - 1;
                changedDirection[X] = true;
                x = entity[i].x;
            } else
                x = entity[i].x - 1;
        }
        if (entity[i].momentum[Y] > 0) {
            if (entity[i].y == MAX_Y) {
                entity[i].momentum[Y] *= - 1;
                changedDirection[Y] = true;
                y = entity[i].y;
            } else
                y = entity[i].y + 1;
        }
        else if (entity[i].momentum[Y] < 0) {
            if (entity[i].y == MIN_Y + 1) {
                entity[i].momentum[Y] *= - 1;
                changedDirection[Y] = true;
                y = entity[i].y;
            } else
                y = entity[i].y - 1;
        }

        /// analisa colisões com a plataforma
        if (entity[i].y == MAX_Y - 1 && entity[i].momentum[Y] > 0) {
            for (int j = platform[0]; j <= platform[1]; j++) {
                // se estiver colidindo com a plataforma
                if (entity[i].x == j) {
                    if (changeDirectionBasedOnPlatform) {
                        // checa se está colidindo na parte da esquerda
                        if (j < getCenterOfPlatform()) {
                            entity[i].momentum[X] = - 1;
                        }
                        // checa se está colidindo na parte da direita
                        else if (j > getCenterOfPlatform()) {
                            entity[i].momentum[X] = + 1;
                        }
                        // checa se está colidindo na parte do meio
                        else {
                            entity[i].momentum[X] = 0;
                        }
                    }
                    entity[i].momentum[Y] *= - 1;
                    changedDirection[Y] = true;
                    break;
                }
            }
            if (!changedDirection[Y]) {
                hasMoved = false;
            }
        }

        else {
            /// analisa colisões com tijolos
            if (brickBoundaries >= getHashPosition(x, y)) {
                // checa o y a sua frente
                if (brick[getHashPosition(entity[i].x, y)]) {
                    // muda a direção da bola
                    entity[i].momentum[Y] *= - 1;

                    // remove o tijolo do mapa
                    brick[getHashPosition(entity[i].x, y)] = false;
                    printSpace(entity[i].x, y, 1);
                    bricksOnScreen--;

                    // grava a posição em que o tijolo estava
                    brokenBrickPosition[X] = entity[i].x;
                    brokenBrickPosition[Y] = y;

                    brokeBrick = true;
                    changedDirection[Y] = true;
                }
                // checa o x a sua frente
                else if (brick[getHashPosition(x, entity[i].y)]) {
                    // muda a direção da bola
                    entity[i].momentum[X] *= - 1;

                    // remove o tijolo do mapa
                    brick[getHashPosition(x, entity[i].y)] = false;
                    printSpace(x, entity[i].y, 1);
                    bricksOnScreen--;

                    // grava a posição em que o tijolo estava
                    brokenBrickPosition[X] = x;
                    brokenBrickPosition[Y] = entity[i].y;

                    brokeBrick = true;
                    changedDirection[X] = true;
                }
                // checa o x e o y a sua frente, ou seja, sua diagonal
                else if (brick[getHashPosition(x, y)]) {
                    // checa se a bola colidiu anteriormente
                    if (!changedDirection[X] && !changedDirection[Y]) {
                        // muda a aceleração da bola em si
                        entity[i].momentum[X] *= - 1;
                        entity[i].momentum[Y] *= - 1;

                        // grava a posição em que o tijolo estava
                        brokenBrickPosition[X] = x;
                        brokenBrickPosition[Y] = y;

                        brick[getHashPosition(x, y)] = false;
                        printSpace(x, y, 1);
                        bricksOnScreen--;

                        brokeBrick = true;
                        changedDirection[X] = true;
                        changedDirection[Y] = true;
                    }
                }
            }


            /// analisa colisões com outros elementos caso não tenha quebrado tijolos anteriormente
            if (!brokeBrick) {
                for (int j = 0; j < entitiesOnScreen; j++) {
                    if (j == i) {
                        continue;
                    }
                    // caso bateu na mesma posição y mas x diferente
                    if (entity[j].x == entity[i].x && entity[j].y == y) {
                        // muda a aceleração x da bola em si
                        entity[i].momentum[Y] *= - 1;
                        changedDirection[Y] = true;

                        // muda a aceleração x da bola que bateu
                        entity[j].momentum[Y] *= - 1;
                    }
                    // caso bateu na mesma posição x mas y diferente
                    else if (entity[j].x == x && entity[j].y == entity[i].y) {
                        // muda a aceleração y da bola em si
                        entity[i].momentum[X] *= - 1;
                        changedDirection[X] = true;

                        // muda a aceleração y da bola que bateu
                        entity[j].momentum[X] *= - 1;
                    }
                    // caso bateu na diagonal
                    else if (entity[j].x == x && entity[j].y == y) {
                        // muda a aceleração da bola em si
                        entity[i].momentum[X] *= - 1;
                        entity[i].momentum[Y] *= - 1;

                        changedDirection[X] = true;
                        changedDirection[Y] = true;

                        // muda a aceleração x e y da bola que bateu
                        entity[j].momentum[X] *= - 1;
                        entity[j].momentum[Y] *= - 1;
                    }
                }
            }
        }
    }
    /// move elementos do tipo estrela
    else if (entity[i].type == STAR_SKIN) {
        // analisa sempre um bloco abaixo
        y = entity[i].y + 1;

        if (brickBoundaries >= getHashPosition(entity[i].x, y)) {
            // checa se tem tijolo embaixo do elemento
            if (brick[getHashPosition(entity[i].x, y)]) {
                entity[i].momentum[Y] = 0;
            // senão, continua a cair
            } else {
                entity[i].momentum[Y] = 1;
                // analisa se há outros elementos em baixo dele
                for (int j = 0; j < entitiesOnScreen; j++) {
                    if (j != i && entity[j].y == y && entity[j].x == entity[i].x) {
                        entity[i].momentum[Y] = 0;
                        break;
                    }
                }
            }
        // checa se o elemento está em cima da plataforma
        } else if (y == MAX_Y) {
            hasMoved = false;
            for (int j = platform[0]; j <= platform[1]; j++) {
                // ativa o buff por PLATFORM_BUFF_TIME milisegundos
                if (entity[i].x == j) {
                    platformBuffTimer += PLATFORM_BUFF_TIME / (GAME_DELAY * updateEntitiesEveryGameTick);
                    updateScore(scoreIncrement * 2);
                    break;
                }
            }
        }
    }
    /// move elementos do tipo vida
    else if (entity[i].type == LIFE_SKIN) {
        // analisa sempre um bloco abaixo
        y = entity[i].y + 1;

        if (brickBoundaries >= getHashPosition(entity[i].x, y)) {
            // checa se tem tijolo embaixo do elemento
            if (brick[getHashPosition(entity[i].x, y)]) {
                entity[i].momentum[Y] = 0;
            // senão, continua a cair
            } else {
                entity[i].momentum[Y] = 1;
                // analisa se há outros elementos em baixo dele
                for (int j = 0; j < entitiesOnScreen; j++) {
                    if (j != i && entity[j].y == y && entity[j].x == entity[i].x) {
                        entity[i].momentum[Y] = 0;
                        break;
                    }
                }
            }
        // checa se o elemento está em cima da plataforma
        } else if (y == MAX_Y) {
            hasMoved = false;
            for (int j = platform[0]; j <= platform[1]; j++) {
                // aumenta a vida do jogador
                if (entity[i].x == j) {
                    playerLifes++;
                    updateScore(scoreIncrement * 2);
                    break;
                }
            }
        }
    }
    /// move elementos do tipo buff
    else if (entity[i].type == BUFF_SKIN) {
        // analisa sempre um bloco abaixo
        y = entity[i].y + 1;

        if (brickBoundaries >= getHashPosition(entity[i].x, y)) {
            // checa se tem tijolo embaixo do elemento
            if (brick[getHashPosition(entity[i].x, y)]) {
                entity[i].momentum[Y] = 0;
            // senão, continua a cair
            } else {
                entity[i].momentum[Y] = 1;
                // analisa se há outros elementos em baixo dele
                for (int j = 0; j < entitiesOnScreen; j++) {
                    if (j != i && entity[j].y == y && entity[j].x == entity[i].x) {
                        entity[i].momentum[Y] = 0;
                        break;
                    }
                }
            }
        // checa se o elemento está em cima da plataforma
        } else if (y == MAX_Y) {
            hasMoved = false;
            for (int j = platform[0]; j <= platform[1]; j++) {
                // aumenta a vida do jogador
                if (entity[i].x == j) {
                    scoreBuffTimer += SCORE_BUFF_TIME / (GAME_DELAY * updateEntitiesEveryGameTick);
                    scoreIncrement = 10;
                    break;
                }
            }
        }
    }

    if (hasMoved) {

        // apaga o elemento de sua posição anterior
        printSpace(entity[i].x, entity[i].y, 1);

        // atualiza a posição do elemento
        entity[i].x += entity[i].momentum[X];
        entity[i].y += entity[i].momentum[Y];

        if (entity[i].despawnTimer > 0) {
            // começa a piscar se o tempo para desaparecer for menor que a metade
            if (entity[i].despawnTimer < (ENTITY_ON_SCREEN_TIME / (GAME_DELAY * updateEntitiesEveryGameTick)) * 0.5) {

                if (entity[i].despawnTimer > (ENTITY_ON_SCREEN_TIME / (GAME_DELAY * updateEntitiesEveryGameTick)) * 0.25) {
                    if (entity[i].despawnTimer % 10) {
                        printSkin(entity[i].x, entity[i].y, entity[i].type, entity[i].color);
                    }
                } else if (entity[i].despawnTimer > (ENTITY_ON_SCREEN_TIME / (GAME_DELAY * updateEntitiesEveryGameTick)) * 0.125) {
                    if (entity[i].despawnTimer % 5) {
                        printSkin(entity[i].x, entity[i].y, entity[i].type, entity[i].color);
                    }
                } else {
                    if (entity[i].despawnTimer % 2) {
                        printSkin(entity[i].x, entity[i].y, entity[i].type, entity[i].color);
                    }
                }
            } else {
                printSkin(entity[i].x, entity[i].y, entity[i].type, entity[i].color);
            }
            entity[i].despawnTimer--;
            // remove o elemento da tela caso o despawnTimer tenha chegado a zero
            if (!entity[i].despawnTimer) {
                removeEntity(i);
                updateScore(- 1);
            }
        } else {
            // imprime o elemento em sua nova posição
            printSkin(entity[i].x, entity[i].y, entity[i].type, entity[i].color);
        }

    // remove o elemento caso não tenha movido
    } else {
        removeEntity(i);
    }

    return hasMoved;
}

void launchBall(bool setPlatform) {

    if (setPlatform) {
        // seta a posição da plataforma
        platform[0] = (MAX_X / 2) - (INITIAL_PLATFORM_SIZE / 2);
        platform[1] = (MAX_X / 2) + (INITIAL_PLATFORM_SIZE / 2);

        // imprime a plataforma
        for (int x = platform[0]; x <= platform[1]; x++) {
            printSkin(x, MAX_Y, PLATFORM_SKIN, PLATFORM_COLOR);
        }
    }

    char arrow = '\\';
    insertEntity(getCenterOfPlatform(), MAX_Y - 1, - 1, - 1, BALL_SKIN);
    printSkin(getCenterOfPlatform() - 1, MAX_Y - 2, arrow, DEFAULT_COLOR);

    do {
        gotoxy(MAX_X, MIN_Y);
        input = getch();

        // apaga a seta em sua posição anterior
        if (arrow == '\\') {
            printSpace(getCenterOfPlatform() - 1, MAX_Y - 2, 1);
        } else {
            printSpace(getCenterOfPlatform() + 1, MAX_Y - 2, 1);
        }

        // apaga a bola em sua posição anterior
        printSpace(getCenterOfPlatform(), MAX_Y - 1, 1);

        movePlatform();

        // imprime a bola acompanhando a plataforma
        entity[0].x = getCenterOfPlatform();
        printSkin(entity[0].x, MAX_Y - 1, entity[0].type, entity[0].color);

        // muda a direção da seta e a aceleração da bola quando for lançada
        if (input == DIAGONAL_LEFT) {
            arrow = '\\';
            entity[0].momentum[X] = - 1;
        } else if (input == DIAGONAL_RIGHT) {
            arrow = '/';
            entity[0].momentum[X] = + 1;
        }

        // imprime a seta que mostra a posição que a bola será lançada
        if (arrow == '\\') {
            printSkin(entity[0].x - 1, MAX_Y - 2, arrow, DEFAULT_COLOR);
        } else {
            printSkin(entity[0].x + 1, MAX_Y - 2, arrow, DEFAULT_COLOR);
        }
    } while (input != SELECT);

}

void movePlatform() {

    if (input == LEFT) {
        // checa se a plataforma está batendo no limite da esquerda
        if (platform[0] > MIN_X + 1) {
            if (platform[1] <= MAX_X) {
                printSpace(platform[1] - 1, MAX_Y, 2);
            }
            platform[0] -= 2;
            platform[1] -= 2;

            ///printSkin(platform[0], MAX_Y, PLATFORM_SKIN, PLATFORM_COLOR); modified
            printSkin(platform[0], MAX_Y, PLATFORM_SKIN, PLATFORM_COLOR);
            printSkin(- 1, - 1, PLATFORM_SKIN, PLATFORM_COLOR);
        }
    } else if (input == RIGHT) {
        // checa se a plataforma está batendo no limite da direita
        if (platform[1] < MAX_X - 1) {
            if (platform[0] >= MIN_X) {
                printSpace(platform[0], MAX_Y, 2);
            }
            platform[0] += 2;
            platform[1] += 2;


            ///printSkin(platform[1], MAX_Y, PLATFORM_SKIN, PLATFORM_COLOR); modified
            printSkin(platform[1] - 1, MAX_Y, PLATFORM_SKIN, PLATFORM_COLOR);
            printSkin(- 1, - 1, PLATFORM_SKIN, PLATFORM_COLOR);

        }
    }
}

void updatePlatform() {

    // checa se o buff está ativo
    if (platformBuffTimer) {

        platformBuffTimer--;


        int actualBuffsOnPlatformAmount = platformBuffTimer / (PLATFORM_BUFF_TIME / (GAME_DELAY * updateEntitiesEveryGameTick));
        if (decimalPart((float) platformBuffTimer / (PLATFORM_BUFF_TIME / (GAME_DELAY * updateEntitiesEveryGameTick)))) {
            actualBuffsOnPlatformAmount++;
        }

        if (buffsOnPlatformAmount < actualBuffsOnPlatformAmount) {
            updatePlatformSize(+ PLATFORM_BUFF_SIZE);
            buffsOnPlatformAmount++;
        }
        else if (buffsOnPlatformAmount > actualBuffsOnPlatformAmount) {
            updatePlatformSize(- PLATFORM_BUFF_SIZE);
            buffsOnPlatformAmount--;
        }
    }
}

void updatePlatformSize(int size) {

    platform[0] -= size / 2;
    platform[1] += size / 2;

    // verifica se a plataforma está fora dos limites da tela
    int x = platform[0] > MIN_X ? platform[0] : MIN_X;

    // apaga toda a linha
    printSpace(MIN_X, MAX_Y, MAX_X + 1);
    gotoxy(x, MAX_Y);

    for (; x <= platform[1] && x <= MAX_X; x++) {
            printSkin(- 1, - 1, PLATFORM_SKIN, PLATFORM_COLOR);
    }
}

void gotoCenter(int textSize, int adjustY) {
    gotoxy(getCenter(X, textSize), getCenter(Y, adjustY));
}

void insertEntity(int x, int y, int mX, int mY, char type) {

    entity[entitiesOnScreen].x = x;
    entity[entitiesOnScreen].y = y;
    entity[entitiesOnScreen].momentum[X] = mX;
    entity[entitiesOnScreen].momentum[Y] = mY;
    entity[entitiesOnScreen].type = type;

    // seta os atributos do elemento baseado no seu tipo
    if (type == BALL_SKIN) {
        ballsOnScreen++;
        entity[entitiesOnScreen].despawnTimer = - 1;
        entity[entitiesOnScreen].color = BALL_COLOR;
    } else {
        entity[entitiesOnScreen].despawnTimer = ENTITY_ON_SCREEN_TIME / (GAME_DELAY * updateEntitiesEveryGameTick);
        if (type == STAR_SKIN) {
            entity[entitiesOnScreen].color = STAR_COLOR;
        } else if (type == LIFE_SKIN) {
            entity[entitiesOnScreen].color = LIFE_COLOR;
        } else if (type == BUFF_SKIN) {
            entity[entitiesOnScreen].color = BUFF_COLOR;
        }
    }
    // imprime o elemento
    printSkin(entity[entitiesOnScreen].x, entity[entitiesOnScreen].y, entity[entitiesOnScreen].type, entity[entitiesOnScreen].color);

    entitiesOnScreen++;
}

void removeEntity(int i) {

    printSpace(entity[i].x, entity[i].y, 1);

    if (entity[i].type == BALL_SKIN) {
        ballsOnScreen--;
    }

    for (int j = i + 1; j < entitiesOnScreen; j++) {
        entity[j - 1].type = entity[j].type;
        entity[j - 1].x = entity[j].x;
        entity[j - 1].y = entity[j].y;
        entity[j - 1].momentum[X] = entity[j].momentum[X];
        entity[j - 1].momentum[Y] = entity[j].momentum[Y];
        entity[j - 1].despawnTimer = entity[j].despawnTimer;
        entity[j - 1].color = entity[j].color;
    }

    entitiesOnScreen--;
}

bool generateLevel(int level) {
    char loadingBar[21] = "                    ";
    int loadingAlt = 0, progress = 0, delay = LOADING_DELAY;
    float loading = 0;

    if (level == 1) {
        getPlayerName();
        delay *= 2;
    }

    system("cls");

    gotoCenter(strlen(message[5]) + 2, - 2);
    printf("%s ", message[5]);

    setTextColor(ALTERNATIVE_COLOR);
    printf("%i", level);

    setTextColor(DEFAULT_COLOR);

    srand(time(NULL));


    gotoCenter(strlen(loadingBar) + 2, MAX_Y / 2 + 1);
    printf("[%s]", loadingBar);

    do {

        if (level == 1) {
            // mostra o tutorial caso o level seja 1
            if (loading > 10 && loading < 15) {
                printSkin(getCenter(X, strlen(message[6]) + 2), getCenter(Y, 0), STAR_SKIN, STAR_COLOR);
                printSpace(- 1, - 1, 1);
                for (size_t k = 0; k < strlen(message[6]); k++) {
                    printf("%c", message[6][k]);
                    Sleep(20);
                }
            }
            else if (loading > 30 && loading < 35) {
                printSkin(getCenter(X, strlen(message[7]) + 2), getCenter(Y, 1), STAR_SKIN, STAR_COLOR);
                printSpace(- 1, - 1, 1);
                for (size_t k = 0; k < strlen(message[7]); k++) {
                    printf("%c", message[7][k]);
                    Sleep(20);
                }
            }
            else if (loading > 50 && loading < 55) {
                printSkin(getCenter(X, strlen(message[8]) + 2), getCenter(Y, 2), STAR_SKIN, STAR_COLOR);
                printSpace(- 1, - 1, 1);
                for (size_t k = 0; k < strlen(message[8]); k++) {
                    printf("%c", message[8][k]);
                    Sleep(20);
                }
            }
            else if (loading > 70 && loading < 75) {
                printSkin(getCenter(X, strlen(message[17]) + 2), getCenter(Y, 3), STAR_SKIN, STAR_COLOR);
                printSpace(- 1, - 1, 1);
                for (size_t k = 0; k < strlen(message[17]); k++) {
                    printf("%c", message[17][k]);
                    Sleep(20);
                }
            }
        }

        // aumenta a barra de carregando aleatoriamente
        loading += (float) (rand() % 51) / 10;

        if (loadingAlt <= (int) loading - 5) {
            loadingBar[progress++] = BRICK_SKIN;
            loadingAlt += 5;
            gotoCenter(strlen(loadingBar) + 2, MAX_Y / 2 + 1);
            printf("[");
            setTextColor(BRICK_COLOR);
            printf("%s", loadingBar);
            setTextColor(DEFAULT_COLOR);
            printf("]");
        }

        gotoCenter(strlen(message[9]) - 1, MAX_Y / 2);
        if (loading > 100) {
            loading += 100 - loading;
        }
        printf(message[9], loading);

        Sleep(delay);

    } while (loading < 100);

    system("cls");
    // importa o arquivo dos levels
    FILE *levelf = fopen("level.txt", "r");
    char buffer[100];

    // procura pelo level passado pelo parâmetro
    for (int levelCount = 0; levelCount < level;) {
        if (fgets(buffer, MAX_X + 2, levelf)) {
            if (buffer[0] == '_')
                levelCount++;
        }
        // senão achou o arquivo, avisa ao jogador
        else {
            gotoCenter(strlen(message[15]), 0);
            printf(message[15]);
            getch();
            return false;
        }
    }

    char bufferChar = getc(levelf);

    // gera o level na tela
    for (int i = 0; bufferChar != '_'; i++) {
        if (bufferChar == '.') {
            printSkin(- 1, - 1, BRICK_SKIN, BRICK_COLOR);
            bricksOnScreen++;
            brick[i] = true;
            Sleep(10);
        } else {
            printSpace(- 1, - 1, 1);
            brick[i] = false;
        }
        brickBoundaries++;
        bufferChar = getc(levelf);
    }

    fclose(levelf);
    return true;
}

void updateBricks() {

    gotoxy(MIN_X, MIN_Y);

    for (int i = 0; i <= brickBoundaries; i++) {
        if (brick[i]) {
            printSkin(- 1, - 1, BRICK_SKIN, BRICK_COLOR);
        } else {
            printSpace(- 1, - 1, 1);
        }
    }
}

void setVariables() {
    ballsOnScreen = 0;
    bricksOnScreen = 0;
    brickBoundaries = 0;
    buffsOnPlatformAmount = 0;
    platformBuffTimer = 0;
    brickLines = 0;
    scoreUpdate = 0;
    scoreIncrement = 1;
    scoreBuffTimer = 0;
    entitiesOnScreen = 0;
    scoreShowUpdateTimer = 0;
    changeDirectionBasedOnPlatform = true;

    playerLifes = INITIAL_PLAYER_LIFES;

    for (int i = 0; i < BRICK_LIMIT; i++) {
        brick[i] = false;
    }
}

bool pauseGame() {
    // pergunta se o jogador quer voltar ao menu inicial ou continuar
    gotoCenter(strlen(message[4]), 0);
    printf(message[4]);
    gotoCenter(strlen(message[13]), 2);
    printf(message[13]);

    do {
        input = getch();
    } while (input != PAUSE && input != 's' && input != 'n');
    // vai para o centro para apagar a mensagem
    gotoCenter(strlen(message[4]), 0);
    printSpace(- 1, - 1, strlen(message[4]));
    gotoCenter(strlen(message[13]), 2);
    printSpace(- 1, - 1, strlen(message[13]));

    updateBricks();

    if (input == 's') {
        return true;
    }
    return false;
}

void randomEntitySpawn(int newBall, int newStar, int newLife) {
    // se quebrou algum tijolo
    if (brokeBrick) {
        // checa se a quantidade de elementos na tela não está no limite
        if (entitiesOnScreen < ENTITY_LIMIT && random(CHANCE_OF_SPAWNING_RANDOM_ENTITY)) {
            if (random(newBall)) {
                // insere uma nova bola
                insertEntity(brokenBrickPosition[X], brokenBrickPosition[Y], 1, 1, BALL_SKIN);
            }
            else if (random(newStar)) {
                // insere uma nova estrela
                insertEntity(brokenBrickPosition[X], brokenBrickPosition[Y], 0, 1, STAR_SKIN);
            }
            else if (random(newLife)) {
                // insere uma nova vida
                insertEntity(brokenBrickPosition[X], brokenBrickPosition[Y], 0, 1, LIFE_SKIN);
            }
            else {
                // insere um novo buff
                insertEntity(brokenBrickPosition[X], brokenBrickPosition[Y], 0, 1, BUFF_SKIN);
            }
        }
        if (changeDirectionBasedOnPlatform) {
            updateScore(0);
        }
        brokeBrick = false;
    }
}

bool gameOver() {

    // calcula os limites verticais baseados na mensagem que será imprimida
    int initX, finalX;
    if (playerLifes) {
        initX = getCenter(X, strlen(message[11])) - 2;
        finalX = getCenter(X, strlen(message[11])) + strlen(message[11]) + 1;
    } else {
        initX = getCenter(X, strlen(message[14])) - 2;
        finalX = getCenter(X, strlen(message[14])) + strlen(message[14]) + 2;
    }
    // usa os limites verticais calculados anteriormente e faz o frame
    generateFrame(initX, finalX, getCenter(Y, - 3), getCenter(Y, 4));

    // imprime game over
    gotoCenter(strlen(message[10]), - 1);
    printf(message[10]);

    // se o jogador tiver vidas, pergunta se queres continuar
    if (playerLifes) {
        Sleep(1000);
        gotoCenter(strlen(message[11]) - 1, 1);
        printf(message[11], playerLifes);
        if (playerLifes > 1) {
            printf("s");
        }

        Sleep(1000);
        gotoCenter(strlen(message[12]), 2);
        printf(message[12]);
        gotoxy(MAX_X, MIN_Y);

        while (input != 's' && input != 'n') {
            input = getch();
        }

        if (input == 's') {
            playerLifes--;
            deleteFrame(initX, finalX, getCenter(Y, - 3), getCenter(Y, 4));
            return false;
        }
    // se o jogador não tiver mais vidas
    } else {
        Sleep(1000);
        gotoCenter(strlen(message[14]), 2);
        printf(message[14]);
        gotoxy(MAX_X, MIN_Y);
        Sleep(3000);
    }

    // fim de jogo
    saveScore();
    return true;
}

void generateFrame(int initX, int finalX, int initY, int finalY) {
    // limpa o espaço onde o frame será imprimido
    deleteFrame(initX, finalX, initY, finalY);

    // imprime o frame horizontal
    for (int x = initX; x <= finalX; x++) {
        printSkin(x, initY, FRAME_X_SKIN, FRAME_COLOR);
        printSkin(x, finalY, FRAME_X_SKIN, FRAME_COLOR);
    }
    // imprime o frame vertical
    for (int y = initY; y <= finalY; y++) {
        printSkin(initX, y, FRAME_Y_SKIN, FRAME_COLOR);
        printSkin(finalX, y, FRAME_Y_SKIN, FRAME_COLOR);
    }
}

void deleteFrame(int initX, int finalX, int initY, int finalY) {
    for (int y = initY; y <= finalY; y++) {
        printSpace(initX, y, finalX - initX + 1);
    }
}

void printHud() {
    setTextColor(BLACK_COLOR + BACKGROUND_COLOR(HUD_BACKGROUND_COLOR));
    printSpace(MIN_X, MIN_Y, 2);

    // imprime a quantidade de vidas que o jogador tem
    printSkin(- 1, - 1, LIFE_SKIN, LIFE_COLOR + BACKGROUND_COLOR(HUD_BACKGROUND_COLOR));
    setTextColor(BLACK_COLOR + BACKGROUND_COLOR(HUD_BACKGROUND_COLOR));
    printf(" %i\t", playerLifes);

    // imprime a quantidade de tijolos restantes na tela
    printSkin(- 1, - 1, BRICK_SKIN, 15 - BRICK_COLOR + BACKGROUND_COLOR(HUD_BACKGROUND_COLOR));
    setTextColor(BLACK_COLOR + BACKGROUND_COLOR(HUD_BACKGROUND_COLOR));
    printf(" %i\t", bricksOnScreen);

    // imprime a quantidade restante de estrela
    printSkin(- 1, - 1, STAR_SKIN, STAR_COLOR + BACKGROUND_COLOR(HUD_BACKGROUND_COLOR));
    setTextColor(BLACK_COLOR + BACKGROUND_COLOR(HUD_BACKGROUND_COLOR));
    printf(" %.1f\t", (float) platformBuffTimer * GAME_DELAY * updateEntitiesEveryGameTick / 1000);

    // imprime a quantidade restante de buff
    printSkin(- 1, - 1, BUFF_SKIN, BUFF_COLOR + BACKGROUND_COLOR(HUD_BACKGROUND_COLOR));
    setTextColor(BLACK_COLOR + BACKGROUND_COLOR(HUD_BACKGROUND_COLOR));
    printf(" %.1f\t\t\t\t\t", (float) scoreBuffTimer * GAME_DELAY * updateEntitiesEveryGameTick / 1000);

    // imprime o score do jogador
    gotoxy(MAX_X - strlen(message[18]) - 11, MIN_Y);
    printf("%s: %05i", message[18], score);

    // imprime a quantidade de pontos ganhos caso o score foi atualizado no frame
    if (scoreShowUpdateTimer) {
        scoreShowUpdateTimer--;
        if (!scoreShowUpdateTimer) {
            printSpace(- 1, - 1, 5);
        } else {
            // se scoreUpdate for maior que zero, cor do número verde
            if (scoreUpdate > 0) {
                setTextColor(DARK_GREEN_COLOR + BACKGROUND_COLOR(HUD_BACKGROUND_COLOR));
                printf(" +%i", scoreUpdate);
            // senão, cor do número vermelha
            } else {
                setTextColor(RED_COLOR + BACKGROUND_COLOR(HUD_BACKGROUND_COLOR));
                printf(" %i ", scoreUpdate);
            }
        }
    } else {
        printSpace(- 1, - 1, 5);
    }
    setTextColor(DEFAULT_COLOR);
}

void mapBrick(int x, int y, int size) {
    for (int oldX = x; x < size + oldX; x++) {
        brick[getHashPosition(x, y)] = true;
        bricksOnScreen++;
    }
    // seta os limites onde há tijolos mapeados
    if (getHashPosition(x, y) > brickBoundaries) {
        brickBoundaries = getHashPosition(x, y);
    }
}

int getCenter(int axis, int adjust) {
    if (axis == X) {
        return (MAX_X / 2) - (adjust / 2);
    } else if (axis == Y) {
        return (MAX_Y / 2) + adjust;
    }
    return 0;
}

int getCenterOfPlatform() {
    return platform[1] - ((platform[1] - platform[0]) / 2);
}

void updateScore(int amount) {
    if (!amount) {
        score += scoreIncrement;
        scoreUpdate = scoreIncrement;
    } else {
        score += amount;
        scoreUpdate = amount;
    }

    scoreShowUpdateTimer = SHOW_UPDATED_SCORE_AMOUNT_TIME / (GAME_DELAY * updateEntitiesEveryGameTick);
}

void getPlayerName() {
    generateFrame(getCenter(X, PLAYER_NAME_SIZE - 1) - 2, getCenter(X, - PLAYER_NAME_SIZE - 1) + 2, getCenter(Y, - 1), getCenter(Y, + 1));
    gotoCenter(strlen(message[19]), - 2);
    printf(message[19]);
    gotoxy(getCenter(X, PLAYER_NAME_SIZE - 1), getCenter(Y, 0));

    for (int i = 0;;) {
        playerName[i] = getch();
        // se enter foi pressionado
        if (playerName[i] == ENTER) {
            // coloca espaços nas casas restantes
            for (; i < PLAYER_NAME_SIZE - 1; i++) {
                playerName[i] = ' ';
            }
            break;
        // se backspace foi pressionado
        } else if (playerName[i] == '\b') {
            if (i > 0) {
                printSpace(getCenter(X, PLAYER_NAME_SIZE - 1) + i - 1, getCenter(Y, 0), 1);
                // apaga a mensagem de aviso
                printSpace(getCenter(X, strlen(message[20])), getCenter(Y, 2), strlen(message[20]));
                gotoxy(getCenter(X, PLAYER_NAME_SIZE - 1) + i - 1, getCenter(Y, 0));

                i--;
            }
        // se atingiu o tamanho limite
        } else if (i == PLAYER_NAME_SIZE - 1) {
            setTextColor(RED_COLOR);
            gotoCenter(strlen(message[20]), 2);
            printf(message[20]);
            setTextColor(DEFAULT_COLOR);
            gotoxy(getCenter(X, PLAYER_NAME_SIZE - 1) + i, getCenter(Y, 0));
        // imprime o caractere pressionado
        } else {
            printf("%c", playerName[i]);
            i++;
        }
    }
    playerName[PLAYER_NAME_SIZE - 1] = '\0';
    system("cls");
}

void saveScore() {
    FILE *txt = fopen("record.txt", "r");

    int position = - 1, score2 = - 1, i = 0;
    char buffer[32][32] = { { '\0' } }, playerName2[16], *stuffInTXT;

    do {
        stuffInTXT = fgets(buffer[i], 32, txt);
        // checa se não há nada naquela linha
        if (strlen(buffer[i]) < 24) {
            // se position não foi setada anteriormente
            if (position < 0) {
                position = i;
            }
        // ou se o score do jogador nessa linha é menor
        } else if (score > parseInt(substring(buffer[i], 17, 21))) {
            // se position não foi setada anteriormente
            if (position < 0) {
                position = i;
                // copia os atributos do jogador que estava nessa posição
                score2 = parseInt(substring(buffer[i], 17, 21));
                strcpy(playerName2, substring(buffer[i], 0, 14));
            }
        }
        i++;
    } while (stuffInTXT);

    fclose(txt);

    // se position = 0, ou é a primeira vez que imprime algo, ou o jogador está com o score maior que o(s) jogador(es) já gravado(s)

    txt = fopen("record.txt", "w");
    bool hasPrinted = false;
    for (int j = 0; j < i;) {
        // se não imprimiu os atributos do jogador atual ainda
        if (!hasPrinted && j == position) {
            fprintf(txt, "%s [%05i]\n", playerName, score);
            hasPrinted = true;
        } else {
            fprintf(txt, buffer[j]);
            j++;
        }
    }
    fclose(txt);


    // calcula os limites do frame
    int initX, finalX, initY = getCenter(Y, 1), finalY, messageCode, messageSize;
    if (score2 > 0) {
        if (equals(playerName, playerName2)) {
            messageCode = 23;
            messageSize = strlen(message[messageCode]) + 1;
        } else {
            trim(playerName2);
            messageCode = 24;
            messageSize = strlen(message[messageCode]) + strlen(playerName2) - 1;
        }

        initX = getCenter(X, messageSize) - 2;
        finalX = getCenter(X, - messageSize) + 2;
        finalY = getCenter(Y, 8);
    }
    else {
        initX = getCenter(X, strlen(message[21])) - 2;
        finalX = getCenter(X, - strlen(message[21])) + 2;
        finalY = getCenter(Y, 5);
    }

    // gera o frame
    generateFrame(initX, finalX, initY, finalY);


    // imprime o score
    gotoCenter(strlen(message[21]), 3);
    printf(message[21], score, score2 > 0 ? 's' : '\0');
    ///printf("%i", score2); dbg
    if (score2 > 0) {
        Sleep(1000);
        gotoCenter(strlen(message[22]), 5);
        printf(message[22]);

        gotoCenter(messageSize, 6);

        if (messageCode == 23) {
            printf(message[messageCode], score2, score2 > 1 ? "s!" : "!");
        } else {
            printf(message[messageCode], playerName2, score2, score2 > 1 ? "s!" : "!");
        }
    }
    gotoxy(MAX_X, MIN_Y);
    Sleep(5000);
    getch();
}

void printRecord() {
    int messageSize = strlen(message[25]) + PLAYER_NAME_SIZE + strlen(message[27]) + 8;

    generateFrame(getCenter(X, messageSize) - 1, getCenter(X, - messageSize) + 1, getCenter(Y, - 7), getCenter(Y, + 6));
    generateFrame(getCenter(X, messageSize) + 4, getCenter(X, - messageSize) - 7, getCenter(Y, - 7), getCenter(Y, + 6));

    int middleX = getCenter(X, - messageSize) - 7 - (((getCenter(X, - messageSize) - 7) - (getCenter(X, messageSize) + 4)) / 2);

    gotoxy(getCenter(X, messageSize) + 1, getCenter(Y, - 6));
    printf(message[25]);

    gotoxy(middleX - strlen(message[26]) / 2, getCenter(Y, - 6));
    printf(message[26]);

    gotoxy(getCenter(X, - messageSize) - strlen(message[27]), getCenter(Y, - 6));
    printf(message[27]);

    gotoxy(getCenter(X, messageSize), getCenter(Y, - 5));
    for (int i = 0; i < (getCenter(X, - messageSize) + 1) - (getCenter(X, messageSize) - 1) - 1; i++) {
        printSkin(- 1, - 1, FRAME_X_SKIN, FRAME_COLOR);
    }

    FILE *txt = fopen("record.txt", "r");

    char buffer[10][10][25];
    int pageLimit = 0, line;
    // recupera todos os atributos gravados no record.txt
    for (line = 0; fgets(buffer[pageLimit][line], 25, txt); line++) {
        if (line == 9) {
            line = 0;
            pageLimit++;
            fgets(buffer[pageLimit][line], 25, txt);
        }
    }

    fclose(txt);

    char playerName2[PLAYER_NAME_SIZE];
    int currentPage = 0;

    do {
        for (int i = 0; i < 10; i++) {
            // apaga a posição anterior
            printSpace(getCenter(X, messageSize) + 1, getCenter(Y, i - 4), 2);
            // apaga o nome anterior
            printSpace(middleX - PLAYER_NAME_SIZE / 2, getCenter(Y, i - 4), PLAYER_NAME_SIZE);
            // apaga o score anterior
            printSpace(getCenter(X, - messageSize) - 5, getCenter(Y, i - 4), 5);
        }

        for (int i = 0; i < 10; i++) {
            if (i == line && currentPage == pageLimit) {
                break;
            }

            // imprime a posição atual
            gotoxy(getCenter(X, messageSize) + 1, getCenter(Y, i - 4));
            printf("%02i", i + 1 + currentPage * 10);

            // pega o nome do jogador e remove os espaços
            strcpy(playerName2, substring(buffer[currentPage][i], 0, 14));
            trim(playerName2);

            // imprime o nome do jogador atual centralizado
            gotoxy(middleX - strlen(playerName2) / 2, getCenter(Y, i - 4));
            printf(playerName2);

            // imprime o score atual
            gotoxy(getCenter(X, - messageSize) - 5, getCenter(Y, i - 4));
            printf(substring(buffer[currentPage][i], 17, 21));
        }

        // imprime < anterior
        if (currentPage > 0) {
            gotoxy(getCenter(X, messageSize), getCenter(Y, + 7));
            printSkin(- 1, - 1, '<', SELECTION_COLOR);
            printf(" %s", message[28]);
        // senão, o apaga
        } else {
            printSpace(getCenter(X, messageSize), getCenter(Y, + 7), strlen(message[28]) + 2);

        }

        // imprime próximo >
        if (currentPage < pageLimit) {
            gotoxy(getCenter(X, - messageSize) - strlen(message[29]), getCenter(Y, + 7));
            printf("%s ", message[29]);
            printSkin(- 1, - 1, '>', SELECTION_COLOR);
        // senão, o apaga
        } else {
            printSpace(getCenter(X, - messageSize) - strlen(message[29]), getCenter(Y, + 7), strlen(message[29]) + 2);
        }
        gotoxy(MAX_X, MIN_Y);

        do {
            input = getch();

            if (input == LEFT && currentPage > 0) {
                currentPage--;
                break;
            } else if (input == RIGHT && currentPage < pageLimit) {
                currentPage++;
                break;
            }
        } while (input != ESCAPE);

    } while (input != ESCAPE);
}

void loadingAnimation(int x, int y, int seconds) {
    for (seconds *= 2; seconds; seconds--) {
        printSpace(x, y + 1, 1);
        printSkin(x, y, BALL_SKIN, rand() % 15 + 1);
        Sleep(125);

        printSpace(x, y, 1);
        printSkin(x + 1, y, BALL_SKIN, rand() % 15 + 1);
        Sleep(125);

        printSpace(x + 1, y, 1);
        printSkin(x + 1, y + 1, BALL_SKIN, rand() % 15 + 1);
        Sleep(125);

        printSpace(x + 1, y + 1, 1);
        printSkin(x, y + 1, BALL_SKIN, rand() % 15 + 1);
        Sleep(125);
    }
}
