//
// Criado por Bruno C. do Nascimento
//

#include <GL/glut.h>
#include <iostream>
#include <string>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define PI 3.1415926535898

using namespace std;

//Variáveis

GLint sair, viewW, viewH, posX, posY, anteriorX = -1, anteriorY, qtErroPares = 0, qtAcertosPares = 0, tpares = 2, tseq = 3;
GLint posClicado, contCliques;
GLint jogoDificuldadeSelecionada = 0;
GLint contSeq = 0;
GLint posicoes3x4[3][4] = {{1,1,2,2},{3,3,4,4},{5,5,6,6}};
GLint posicoes4x5[4][5] = {{1,1,2,2,3},{3,4,4,5,5},{6,6,7,7,8},{8,9,9,10,10}};
GLint combinados3x4[3][4] = {{0,0,0,0},{0,0,0,0},{0,0,0,0}};
GLint combinados4x5[4][5] = {{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0}};
GLint animacao2x2[4] = {1,1,1,1};
GLint animacao3x3[9] = {1,1,1,1,1,1,1,1,1};
GLint seq[25];
GLdouble xM = 20, yM = 20, window = 50.0;
GLboolean primeiro, perdeSeq, ganhaSeq;

void desenhaTriangulo(double px1, double py1, double px2, double py2, double px3, double py3){
    glBegin(GL_TRIANGLES);
        glVertex2f(px1, py1);
        glVertex2f(px2, py2);
        glVertex2f(px3, py3);
    glEnd();
}

void desenhaQuadrado(double px1, double py1, double px2, double py2, double px3, double py3, double px4, double py4){
    glBegin(GL_QUADS);
        glVertex2f(px1, py1);
        glVertex2f(px2, py2);
        glVertex2f(px3, py3);
        glVertex2f(px4, py4);
    glEnd();
}

void desenhaCirculo(int r, double x, double y){
	GLint circle_points = 30;
	GLfloat angle;
    glBegin(GL_POLYGON);
      for (int i = 0; i < circle_points; i++) {
          angle = 2 * PI * i / circle_points;
          glVertex2f(r * cos(angle)+(x), r * sin(angle)+(y));
	  }
    glEnd();
}

// Desenha grade 3x4 para jogo pares no nível fácil
void desenhaGrade3x4(){
    glColor3f(0.5f, 0.5f, 0.5f);
    glLineWidth(3.0);
    glBegin(GL_LINES);
        glVertex2f(-25.0f, 50.0f);
        glVertex2f(-25.0f, -50.0f);
        glVertex2f(0.0f, 50.0f);
        glVertex2f(0.0f, -50.0f);
        glVertex2f(25.0f, 50.0f);
        glVertex2f(25.0f, -50.0f);
        glVertex2f(-50.0f, 17.0f);
        glVertex2f(50.0f, 17.0f);
        glVertex2f(-50.0f, -17.0f);
        glVertex2f(50.0f, -17.0f);
    glEnd();
}

// Desenha grade 3x4 para jogo pares no nível difícil
void desenhaGrade4x5(){
    glColor3f(0.5f, 0.5f, 0.5f);
    glLineWidth(3.0);
    glBegin(GL_LINES);
        glVertex2f(-30.0f, 50.0f);
        glVertex2f(-30.0f, -50.0f);
        glVertex2f(-10.0f, 50.0f);
        glVertex2f(-10.0f, -50.0f);
        glVertex2f(10.0f, 50.0f);
        glVertex2f(10.0f, -50.0f);
        glVertex2f(30.0f, 50.0f);
        glVertex2f(30.0f, -50.0f);
        glVertex2f(-50.0f, 25.0f);
        glVertex2f(50.0f, 25.0f);
        glVertex2f(-50.0f, 0.0f);
        glVertex2f(50.0f, 0.0f);
        glVertex2f(-50.0f, -25.0f);
        glVertex2f(50.0f, -25.0f);
    glEnd();
}

// Desenha grade para o jogo sequencia no nivel fácil
void desenhaGradeSequenciaFacil(){
    glColor3f(0.5f, 0.5f, 0.5f);
    glLineWidth(3.0);
    glBegin(GL_LINES);
        glVertex2f(0.0f, 50.0f);
        glVertex2f(0.0f, -50.0f);
        glVertex2f(50.0f, 0.0f);
        glVertex2f(-50.0f, 0.0f);
    glEnd();
}

void desenhaGradeSequenciaDificil(){
    glColor3f(0.5f, 0.5f, 0.5f);
    glLineWidth(3.0);
    glBegin(GL_LINES);
        glVertex2f(-16.67f, 50.0f);
        glVertex2f(-16.67f, -50.0f);
        glVertex2f(16.66f, 50.0f);
        glVertex2f(16.66f, -50.0f);
        glVertex2f(50.0f, 16.67f);
        glVertex2f(-50.0f, 16.67f);
        glVertex2f(50.0f, -16.66f);
        glVertex2f(-50.0f, -16.66f);
    glEnd();
}

// Sorteia pontos para embaralhar o vetor posições que contem a posicão de cada forma (nível fácil do jogo pares)
void sorteiaPontos3x4(){
    srand(time(NULL));
    int valor1, valor2, valor3, valor4, troca;
    for(int i = 0; i < 100; i++){
        valor1 = rand() % 3;
        valor2 = rand() % 4;
        valor3 = rand() % 3;
        valor4 = rand() % 4;
        troca = posicoes3x4[valor1][valor2];
        posicoes3x4[valor1][valor2] = posicoes3x4[valor3][valor4];
        posicoes3x4[valor3][valor4] = troca;
    }
}

// Sorteia pontos para embaralhar o vetor posições que contem a posicão de cada forma (nível difícil do jogo pares)
void sorteiaPontos4x5(){
    srand(time(NULL));
    int valor1, valor2, valor3, valor4, troca;
    for(int i = 0; i < 100; i++){
        valor1 = rand() % 4;
        valor2 = rand() % 5;
        valor3 = rand() % 4;
        valor4 = rand() % 5;
        troca = posicoes4x5[valor1][valor2];
        posicoes4x5[valor1][valor2] = posicoes4x5[valor3][valor4];
        posicoes4x5[valor3][valor4] = troca;
    }
}

// Sorteia a sequência a ser seguida, o vetor seq garda toda a sequência
void sorteiaSeq(){
    srand(time(NULL));
    if(jogoDificuldadeSelecionada == 1){
        for(int i=0; i<25; i++){
            seq[i] = rand() % 4;
        }
    }
    else{
        for(int i=0; i<25; i++){
            seq[i] = rand() % 9;
        }
    }
}

// Identifica o local que foi clicado com o mouse
void identificaLocalClicado(GLdouble x, GLdouble y){
    if(jogoDificuldadeSelecionada == -1){
        if(y > 17)
            posX = 0;
        else{
            if(y > -16)
                posX = 1;
            else
                posX = 2;
        }
        if(x > 0){
            if(x > 25)
                posY = 3;
            else
                posY = 2;
        }
        else{
            if(x > -25)
                posY = 1;
            else
                posY = 0;
        }
    }
    else if(jogoDificuldadeSelecionada == -2){
        if(y > 0){
            if(y > 25)
                posX = 0;
            else
                posX = 1;
        }
        else{
            if(y > -25)
                posX = 2;
            else
                posX = 3;
        }
        if(x > 10){
            if(x > 30)
                posY = 4;
            else
                posY = 3;
        }
        else{
            if(x > -10)
                posY = 2;
            else if(x < -30){
                posY = 0;
            }
            else
                posY = 1;
        }
    }
    else if(jogoDificuldadeSelecionada == 1){
        if(x > 0){
            if(y > 0)
                posClicado = 1;
            else
                posClicado = 3;
        }
        else{
            if(y > 0)
                posClicado = 0;
            else
                posClicado = 2;
        }
    }
    else{
        if(x > -17){
            if(x > 16.5){
                if(y > 16.5)
                    posClicado = 2;
                else if(y < -17)
                    posClicado = 8;
                else
                    posClicado = 5;
            }
            else{
                if(y > 16.5)
                    posClicado = 1;
                else if(y < -17)
                    posClicado = 7;
                else
                    posClicado = 4;
            }
        }
        else{
            if(y > 16.5)
                posClicado = 0;
            else if(y < -17)
                posClicado = 6;
            else
                posClicado = 3;
        }
    }
}

// funçao que retorna verdadeiro depois que um determinado tem em segundos foi alcançado
bool timer(int v){
    int t1 = time(NULL), t2, dif;
    do{
        t2 = time(NULL);
        dif = difftime(t2, t1);
    }while(dif != v);
    return true;
}

void telaWin(){
    glClearColor(0.0f,0.0f,0.0f,0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    string texto("You Win");
    glRasterPos2f(-5,0);
    for(int i = 0; i < texto.size(); i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, (int)texto[i]);
}

void telaLose(){
    glClearColor(0.0f,0.0f,0.0f,0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    string texto("You Lose");
    glRasterPos2f(-5,0);
    for(int i = 0; i < texto.size(); i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, (int)texto[i]);
}

// Desenha as formas do jogo pares no nível fácil
void desenhaParesFacil(){
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0f, 1.0f, 0.0f);
    desenhaGrade3x4();
    if(qtErroPares == 5)
        telaLose();
    else if(qtAcertosPares == 6)
        telaWin();
    else{
        for(int i = 0; i < 3; i++){
            for(int j = 0; j < 4; j++){
                if(combinados3x4[i][j] == 1 || combinados3x4[i][j] == 2){ // 2 para pares combinados e 1 para região clicado
                    if(posicoes3x4[i][j] == 1){ // Círculo Azul
                        glColor3f(0.0f, 0.0f, 1.0f);
                        desenhaCirculo(8.0, -37.5+(j*25), 33.5-(i*33));
                    }
                    else if(posicoes3x4[i][j] == 2){ // Círculo Verde
                        glColor3f(0.0f, 1.0f, 0.0f);
                        desenhaCirculo(8.0, -37.5+(j*25), 33.5-(i*33));
                    }
                    else if(posicoes3x4[i][j] == 3){ // Quadrado Vermelho
                        glColor3f(1.0f, 0.0f, 0.0f);
                        desenhaQuadrado(-45.0+(j*25), 42.0-(i*33), -30.0+(j*25), 42.0-(i*33), -30.0+(j*25), 27.0-(i*33), -45.0+(j*25), 27.0-(i*33));
                    }
                    else if(posicoes3x4[i][j] == 4){ // Quadrado Amarelo
                        glColor3f(1.0f, 1.0f, 0.0f);
                        desenhaQuadrado(-45.0+(j*25), 42.0-(i*33), -30.0+(j*25), 42.0-(i*33), -30.0+(j*25), 27.0-(i*33), -45.0+(j*25), 27.0-(i*33));
                    }
                    else if(posicoes3x4[i][j] == 5){ // Triângulo Lilás
                        glColor3f(1.0f, 0.0f, 1.0f);
                        desenhaTriangulo(-45.0+(j*25), 27.0-(i*33), -30.0+(j*25), 27.0-(i*33), -37.5+(j*25), 42.0-(i*33));
                    }
                    else if(posicoes3x4[i][j] == 6){ // Triângulo Cinza
                        glColor3f(0.5f, 0.5f, 0.5f);
                        desenhaTriangulo(-45.0+(j*25), 27.0-(i*33), -30.0+(j*25), 27.0-(i*33), -37.5+(j*25), 42.0-(i*33));
                    }
                }
            }
        }
    }
}

// Desenha as formas do jogo pares no nível difícil
void desenhaParesDificil(){
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0f, 1.0f, 0.0f);
    desenhaGrade4x5();
    glColor3f(1.0f, 0.0f, 0.0f);
    if(qtErroPares == 5)
        telaLose();
    else if(qtAcertosPares == 10)
        telaWin();
    else{
        for(int i = 0; i < 4; i++){
            for(int j = 0; j < 5; j++){
                if(combinados4x5[i][j] == 1 || combinados4x5[i][j] == 2){
                    if(posicoes4x5[i][j] == 1){ // Círculo Azul
                        glColor3f(0.0f, 0.0f, 1.0f);
                        desenhaCirculo(5.0, -40.0+(j*20), 37.5-(i*25));
                     }
                     else if(posicoes4x5[i][j] == 2){ // Círculo Verde
                        glColor3f(0.0f, 1.0f, 0.0f);
                        desenhaCirculo(5.0, -40.0+(j*20), 37.5-(i*25));
                     }
                     else if(posicoes4x5[i][j] == 3){ // Quadrado Vermelho
                        glColor3f(1.0f, 0.0f, 0.0f);
                        desenhaQuadrado(-45.0+(j*20), 42.5-(i*25), -35.0+(j*20), 42.5-(i*25), -35.0+(j*20), 32.5-(i*25), -45.0+(j*20), 32.5-(i*25));
                     }
                     else if(posicoes4x5[i][j] == 4){ // Quadrado Amarelo
                        glColor3f(1.0f, 1.0f, 0.0f);
                        desenhaQuadrado(-45.0+(j*20), 42.5-(i*25), -35.0+(j*20), 42.5-(i*25), -35.0+(j*20), 32.5-(i*25), -45.0+(j*20), 32.5-(i*25));
                     }
                     else if(posicoes4x5[i][j] == 5){ // Triângulo Lilás
                        glColor3f(1.0f, 0.0f, 1.0f);
                        desenhaTriangulo(-45.0+(j*20), 32.5-(i*25), -35.0+(j*20), 32.5-(i*25), -40.0+(j*20), 42.5-(i*25)); ;
                     }
                     else if(posicoes4x5[i][j] == 6){ // Triângulo Cinza
                        glColor3f(0.5f, 0.5f, 0.5f);
                        desenhaTriangulo(-45.0+(j*20), 32.5-(i*25), -35.0+(j*20), 32.5-(i*25), -40.0+(j*20), 42.5-(i*25)); // 3x4
                     }
                     else if(posicoes4x5[i][j] == 7){ // Círculo Preto
                        glColor3f(0.0f, 0.0f, 0.0f);
                        desenhaCirculo(5.0, -40.0+(j*20), 37.5-(i*25));
                     }
                     else if(posicoes4x5[i][j] == 8){ // Quadrado Azul
                        glColor3f(0.0f, 0.0f, 1.0f);
                        desenhaQuadrado(-45.0+(j*20), 42.5-(i*25), -35.0+(j*20), 42.5-(i*25), -35.0+(j*20), 32.5-(i*25), -45.0+(j*20), 32.5-(i*25));
                     }
                     else if(posicoes4x5[i][j] == 9){ // Quadrado Verde
                        glColor3f(0.0f, 1.0f, 0.0f);
                        desenhaQuadrado(-45.0+(j*20), 42.5-(i*25), -35.0+(j*20), 42.5-(i*25), -35.0+(j*20), 32.5-(i*25), -45.0+(j*20), 32.5-(i*25));
                     }
                     else if(posicoes4x5[i][j] == 10){ // Triângulo Vermelho
                        glColor3f(1.0f, 0.0f, 0.0f);
                        desenhaTriangulo(-45.0+(j*20), 32.5-(i*25), -35.0+(j*20), 32.5-(i*25), -40.0+(j*20), 42.5-(i*25)); // 3x4
                    }
                }
            }
        }
    }
}

// Desenha as formas do jogo sequencia no nível fácil
void desenhaSequenciaFacil(){
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    if(perdeSeq)
        telaLose();
    else if(ganhaSeq)
        telaWin();
    else{
        desenhaGradeSequenciaFacil();
        if(animacao2x2[0]){
            glColor3f(0.5f, 0.0f, 0.0f);
            desenhaCirculo(20, -25.0f, 25.0f);
        }
        else{
            glColor3f(1.0f, 1.0f, 1.0f);
            desenhaCirculo(20, -25.0, 25.0);
        }
        if(animacao2x2[1]){
            glColor3f(0.0f, 0.0f, 0.5f);
            desenhaCirculo(20, 25.0f, 25.0f);
        }
        else{
            glColor3f(1.0f, 1.0f, 1.0f);
            desenhaCirculo(20, 25.0, 25.0);
        }
        if(animacao2x2[2]){
            glColor3f(0.5f, 0.5f, 0.0f);
            desenhaCirculo(20, -25.0f, -25.0f);
        }
        else{
            glColor3f(1.0f, 1.0f, 1.0f);
            desenhaCirculo(20, -25.0, -25.0);
        }
        if(animacao2x2[3]){
            glColor3f(0.0f, 0.5f, 0.0f);
            desenhaCirculo(20, 25.0f, -25.0f);
        }
        else{
            glColor3f(1.0f, 1.0f, 1.0f);
            desenhaCirculo(20, 25.0, -25.0);
        }
    }
}

// Desenha as formas do jogo sequencia no nível difícil
void desenhaSequenciaDificil(){
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    if(perdeSeq)
        telaLose();
    else if(ganhaSeq)
        telaWin();
    else{
        desenhaGradeSequenciaDificil();
        if(animacao3x3[0]){
            glColor3f(0.5f, 0.0f, 0.0f);
            desenhaCirculo(14, -33.33f, 33.33f);
        }
        else{
            glColor3f(1.0f, 1.0f, 1.0f);
            desenhaCirculo(14, -33.33f, 33.33f);
        }
        if(animacao3x3[1]){
            glColor3f(0.0f, 0.0f, 0.5f);
            desenhaCirculo(14, 0.0f, 33.33f);
        }
        else{
            glColor3f(1.0f, 1.0f, 1.0f);
            desenhaCirculo(14, 0.0f, 33.33f);
        }
        if(animacao3x3[2]){
            glColor3f(0.0f, 0.5f, 0.0f);
            desenhaCirculo(14, 33.33f, 33.33f);
        }
        else{
            glColor3f(1.0f, 1.0f, 1.0f);
            desenhaCirculo(14, 33.33f, 33.33f);
        }
        if(animacao3x3[3]){
            glColor3f(0.5f, 0.5f, 0.0f);
            desenhaCirculo(14, -33.33f, 0.0f);
        }
        else{
            glColor3f(1.0f, 1.0f, 1.0f);
            desenhaCirculo(14, -33.33f, 0.0f);
        }
        if(animacao3x3[4]){
            glColor3f(0.5f, 0.0f, 0.5f);
            desenhaCirculo(14, 0.0f, 0.0f);
        }
        else{
            glColor3f(1.0f, 1.0f, 1.0f);
            desenhaCirculo(14, 0.0f, 0.0f);
        }
        if(animacao3x3[5]){
            glColor3f(0.5f, 0.2f, 0.2f);
            desenhaCirculo(14, 33.33f, 0.0f);
        }
        else{
            glColor3f(1.0f, 1.0f, 1.0f);
            desenhaCirculo(14, 33.33f, 0.0f);
        }
        if(animacao3x3[6]){
            glColor3f(0.0f, 0.3f, 0.0f);
            desenhaCirculo(14, -33.33f, -33.33f);
        }
        else{
            glColor3f(1.0f, 1.0f, 1.0f);
            desenhaCirculo(14, -33.33f, -33.33f);
        }
        if(animacao3x3[7]){
            glColor3f(0.0f, 0.5f, 0.5f);
            desenhaCirculo(14, 0.0f, -33.33f);
        }
        else{
            glColor3f(1.0f, 1.0f, 1.0f);
            desenhaCirculo(14, 0.0f, -33.33f);
        }
        if(animacao3x3[8]){
            glColor3f(0.5f, 0.5f, 0.5f);
            desenhaCirculo(14, 33.33f, -33.33f);
        }
        else{
            glColor3f(1.0f, 1.0f, 1.0f);
            desenhaCirculo(14, 33.33f, -33.33f);
        }
    }
}

// Usada na glutDisplayFunc()
void desenha(void){
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT);
    if(jogoDificuldadeSelecionada == -1 || jogoDificuldadeSelecionada == -2){
        if(jogoDificuldadeSelecionada == -1){
            desenhaParesFacil();
        }
        else{
            desenhaParesDificil();
        }
    }
    else if(jogoDificuldadeSelecionada == 1 || jogoDificuldadeSelecionada == 2){
        if(jogoDificuldadeSelecionada == 1)
            desenhaSequenciaFacil();
        else
            desenhaSequenciaDificil();
    }
    glutSwapBuffers();
}

// Faz a animação desenhando o circulo branco na posicão onde o vetor animacao2x2 (ou animacao3x3) tem o valor zero
void animacaoSequencia(int v){
    if(jogoDificuldadeSelecionada == 1){
        if(seq[v] == 0){
            if(animacao2x2[seq[v]])
                animacao2x2[seq[v]] = 0;
            else
                animacao2x2[seq[v]] = 1;
        }
        else if(seq[v] == 1){
            if(animacao2x2[seq[v]])
                animacao2x2[seq[v]] = 0;
            else
                animacao2x2[seq[v]] = 1;
        }
        else if(seq[v] == 2){
             if(animacao2x2[seq[v]])
                animacao2x2[seq[v]] = 0;
            else
                animacao2x2[seq[v]] = 1;
        }
        else{
             if(animacao2x2[seq[v]])
                animacao2x2[seq[v]] = 0;
            else
                animacao2x2[seq[v]] = 1;
        }
    }
    else{
        if(seq[v] == 0){
            if(animacao3x3[seq[v]])
                animacao3x3[seq[v]] = 0;
            else
                animacao3x3[seq[v]] = 1;
        }
        else if(seq[v] == 1){
            if(animacao3x3[seq[v]])
                animacao3x3[seq[v]] = 0;
            else
                animacao3x3[seq[v]] = 1;
        }
        else if(seq[v] == 2){
             if(animacao3x3[seq[v]])
                animacao3x3[seq[v]] = 0;
            else
                animacao3x3[seq[v]] = 1;
        }
        else if(seq[v] == 3){
             if(animacao3x3[seq[v]])
                animacao3x3[seq[v]] = 0;
            else
                animacao3x3[seq[v]] = 1;
        }
        else if(seq[v] == 4){
             if(animacao3x3[seq[v]])
                animacao3x3[seq[v]] = 0;
            else
                animacao3x3[seq[v]] = 1;
        }
        else if(seq[v] == 5){
             if(animacao3x3[seq[v]])
                animacao3x3[seq[v]] = 0;
            else
                animacao3x3[seq[v]] = 1;
        }
        else if(seq[v] == 6){
             if(animacao3x3[seq[v]])
                animacao3x3[seq[v]] = 0;
            else
                animacao3x3[seq[v]] = 1;
        }
        else if(seq[v] == 7){
             if(animacao3x3[seq[v]])
                animacao3x3[seq[v]] = 0;
            else
                animacao3x3[seq[v]] = 1;
        }
        else if(seq[v] == 8){
             if(animacao3x3[seq[v]])
                animacao3x3[seq[v]] = 0;
            else
                animacao3x3[seq[v]] = 1;
        }
    }
    desenha();
}

// Função que auxilia nas regras do jogo pares
void pares(){
    if(primeiro){
        anteriorX = posX;
        anteriorY = posY;
        primeiro = false;
    }
    else if(jogoDificuldadeSelecionada == -1){ // Para dificuldade fácil
        if(posicoes3x4[anteriorX][anteriorY] == posicoes3x4[posX][posY]){
            combinados3x4[anteriorX][anteriorY] = 2;
            combinados3x4[posX][posY] = 2;
            qtAcertosPares++;
            primeiro = true;
        }
        else{
            qtErroPares++;
            desenha();
            if(timer(tpares));
            combinados3x4[anteriorX][anteriorY] = 0;
            combinados3x4[posX][posY] = 0;
            primeiro = true;
        }
    }
    else{
        if(posicoes4x5[anteriorX][anteriorY] == posicoes4x5[posX][posY]){
            combinados4x5[anteriorX][anteriorY] = 2;
            combinados4x5[posX][posY] = 2;
            qtAcertosPares++;
            primeiro = true;
        }
        else{
            qtErroPares++;
            desenha();
            if(timer(tpares));
            combinados4x5[anteriorX][anteriorY] = 0;
            combinados4x5[posX][posY] = 0;
            primeiro = true;
        }
    }
}

// Função que auxilia nas regras do jogo sequencia
void sequencia(){
    if(primeiro){
        if(contSeq >= 10)
            tseq = 2;
        else if(contSeq >= 20)
            tseq = 1;
        for(int i=0; i <= contSeq; i++){
            timer(1);
            animacaoSequencia(i);
            timer(tseq);
            animacaoSequencia(i);
            timer(1);
        }
        contSeq++;
        contCliques = 0;
        primeiro = false;
    }
    else{
        if(posClicado == seq[contCliques]){
            timer(1);
            animacaoSequencia(contCliques);
            timer(1);
            animacaoSequencia(contCliques);
            timer(1);
            contCliques++;
        }
        else{
            perdeSeq = true;
            desenha();
        }
        if(contCliques == 25){
            ganhaSeq = true;
            desenha();
        }
        else if(contCliques == contSeq){
            primeiro = true;
            timer(1);
            sequencia();
        }
    }
}

// Chamada quando a tela é redimensionada
void reshape(GLsizei w, GLsizei h){
    glViewport(0, 0, w, h);
    viewW = w;
    viewH = h;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-window, window, -window, window);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// Trata evento do mouse
void mouse(int button, int state, int x, int y){
    if(button == GLUT_LEFT_BUTTON){
        if(state == GLUT_DOWN){
            xM = ( (2 * window * x) / viewW) - window;
            yM = ( ( (2 * window) * (viewH - y) ) / viewH) - window;
            identificaLocalClicado(xM, yM);
            if(jogoDificuldadeSelecionada == -1 && combinados3x4[posX][posY] == 0){
                combinados3x4[posX][posY] = 1; // Indica que essa posicao foi clicada
                pares();
            }
            else if(jogoDificuldadeSelecionada == -2 && combinados4x5[posX][posY] == 0){
                combinados4x5[posX][posY] = 1;
                pares();
            }
            else if(jogoDificuldadeSelecionada == 1){
                sequencia();
            }
            else{
                sequencia();
            }
        }
    }
}

void paresMenu(int op){
    qtErroPares = 0;
    qtAcertosPares = 0;
    primeiro = true;
    switch(op){
    case -1:
        jogoDificuldadeSelecionada = -1;
        sorteiaPontos3x4();
        memset(combinados3x4, 0, sizeof(GLint[3][4]));
        break;
    case -2:
        jogoDificuldadeSelecionada = -2;
        sorteiaPontos4x5();
        memset(combinados4x5, 0, sizeof(GLint[4][5]));
        break;
    }
    glutPostRedisplay();
}

void sequenciaMenu(int op){
    primeiro = true;
    perdeSeq = false;
    ganhaSeq = false;
    contSeq = 0;
    tseq = 3;
    switch(op){
    case 1:
        jogoDificuldadeSelecionada = 1;
        sorteiaSeq();
        break;
    case 2:
        jogoDificuldadeSelecionada = 2;
        sorteiaSeq();
        break;
    }
    glutPostRedisplay();
}

void menuPrincipal(int op){
    if(op == sair)
        exit(0);
}

void criaMenu(void){
    int menu, subMenu1, subMenu2;

    subMenu1 = glutCreateMenu(paresMenu);
    glutAddMenuEntry("Fácil", -1);
    glutAddMenuEntry("Difícil", -2);

    subMenu2 = glutCreateMenu(sequenciaMenu);
    glutAddMenuEntry("Fácil", 1);
    glutAddMenuEntry("Difícil", 2);

    sair = subMenu1 + subMenu2; // Evita que o subMenu3 tenha o mesmo valor que os outros

    menu = glutCreateMenu(menuPrincipal);
    glutAddSubMenu("Formar os pares", subMenu1);
    glutAddSubMenu("Sequencia (Genius)", subMenu2);
    glutAddMenuEntry("Sair", sair);

    glutAttachMenu(GLUT_RIGHT_BUTTON);
    glFlush();
}

void inicializa(void){
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    criaMenu();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluOrtho2D(-window, window, -window, window);
}

int main(int argc, char** argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(700,600);
    glutInitWindowPosition(500,50);
    glutCreateWindow("Trabalho 1");
    inicializa();
    glutDisplayFunc(desenha);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutMainLoop();
}
