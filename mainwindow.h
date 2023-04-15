// Inclui as bibliotecas necessárias
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow> // Biblioteca para criar janelas de aplicativos
#include <QPoint> // Biblioteca para manipulação de pontos em coordenadas cartesianas
#include <QColor> // Biblioteca para manipulação de cores
#include <QTimer> // Biblioteca para criação de timers
#include <QtGui> // Biblioteca para a interface gráfica do usuário

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; } // Declaração do namespace
QT_END_NAMESPACE

class MainWindow : public QMainWindow // Criação da classe MainWindow que herda da classe QMainWindow
{
    Q_OBJECT // Macro para declarar que a classe é um objeto Q

public:
    MainWindow(QWidget *parent = nullptr); // Construtor da classe MainWindow
    ~MainWindow(); // Destrutor da classe MainWindow
    // Struct para ordenação das cores em um mapa
    struct CompareColors
    {
        bool operator()(const QColor& a, const QColor& b) const
        {
            return a.red() < b.red() || (a.red() == b.red() && (a.green() < b.green() || (a.green() == b.green() && a.blue() < b.blue())));
        }
    };

    std::map<QColor, Qt::Key, CompareColors> keyMapping; // Mapa de cores para teclas
    QVector<double> reactionTimes; // Vetor para armazenar os tempos de reação

protected:
    void paintEvent(QPaintEvent *event) override; // Sobrescrita da função paintEvent para desenhar o círculo
    void keyPressEvent(QKeyEvent *event) override; // Sobrescrita da função keyPressEvent para lidar com as teclas pressionadas

private slots:
    void updateCircle(); // Função para atualizar a posição e a cor do círculo
    void decrementScore(); // Função para decrementar a pontuação do usuário
    void reset(); // Função para reiniciar o jogo

private:
    Ui::MainWindow *ui; // Ponteiro para a interface gráfica do usuário
    QPoint circlePosition; // Posição do círculo
    int circleSize; // Tamanho do círculo
    QColor circleColor; // Cor do círculo
    long long startTime; // Tempo de início do jogo
    int acertos; // Número de acertos do usuário
    int erros; // Número de erros do usuário
    int reactionTime = 0; // Tempo de reação do usuário
    bool keyPressed; // Variável para verificar se alguma tecla foi pressionada
    QTimer *noKeyPressTimer; // Timer para decrementar a pontuação do usuário caso nenhuma tecla seja pressionada
};

#endif // MAINWINDOW_H





