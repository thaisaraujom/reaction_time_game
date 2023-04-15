#include "mainwindow.h" // Inclui o arquivo de cabeçalho da classe MainWindow
#include "ui_mainwindow.h" // Inclui o arquivo de cabeçalho gerado pelo Qt do arquivo UI
#include <QPainter> // Inclui o arquivo de cabeçalho da classe QPainter para desenho de gráficos
#include <QKeyEvent> // Inclui o arquivo de cabeçalho da classe QKeyEvent para lidar com eventos de teclado
#include <QTime> // Inclui o arquivo de cabeçalho da classe QTime para lidar com tempo
#include <QCoreApplication> // Inclui o arquivo de cabeçalho da classe QCoreApplication para gerenciamento de eventos do aplicativo
#include <QMessageBox> // Inclui o arquivo de cabeçalho da classe QMessageBox para exibir mensagens de alerta ao usuário
#include <QVector> // Inclui o arquivo de cabeçalho da classe QVector para lidar com vetores dinâmicos

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    circleSize(50) // Define o tamanho do círculo
{
    ui->setupUi(this); // Configura a interface do usuário

    // Inicia a janela em tela cheia
    showFullScreen();


    acertos = 0; // Inicializa o número de acertos
    erros = 0; // Inicializa o número de erros
    setFocusPolicy(Qt::StrongFocus); // Define a política de foco para StrongFocus

    // Mapeia as cores para as teclas correspondentes
    keyMapping[Qt::yellow] = Qt::Key_Up;
    keyMapping[Qt::green] = Qt::Key_Down;
    keyMapping[Qt::blue] = Qt::Key_Left;
    keyMapping[Qt::red] = Qt::Key_Right;

    connect(ui->resetButton, &QPushButton::clicked, this, &MainWindow::reset); // Conecta o clique do botão resetButton à função reset

    noKeyPressTimer = new QTimer(this); // Cria um novo objeto QTimer
    connect(noKeyPressTimer, &QTimer::timeout, this, &MainWindow::decrementScore); // Conecta o sinal timeout do QTimer à função decrementScore
    noKeyPressTimer->start(5000); // Inicia o QTimer e define o tempo de intervalo para 5000 milissegundos (5 segundos)

    srand(QTime::currentTime().msec()); // Define a semente para a função rand baseada no tempo atual em milissegundos

    QTimer::singleShot(50, this, &MainWindow::updateCircle); // Dispara a função updateCircle após 100 milissegundos

}

MainWindow::~MainWindow() // Destrutor da classe MainWindow
{
    delete ui; // Deleta o objeto ui
}

void MainWindow::keyPressEvent(QKeyEvent *event) // Lida com o evento de pressionar uma tecla
{
    if (event->key() == Qt::Key_Escape) { // Se a tecla pressionada for a tecla Escape
        QCoreApplication::quit(); // Encerra a aplicação
    } else {
        if (event->key() == keyMapping[circleColor.rgb()]) { // Se a tecla pressionada for a tecla mapeada para a cor atual do círculo
            // Acertou
            acertos++; // Incrementa o número de acertos
            ui->scoreLabel->setText(QString("Score: %1").arg(acertos - erros)); // Atualiza o texto do rótulo de pontuação
            ui->acertosLabel->setText(QString("Acertos: %1").arg(acertos)); // Atualiza o texto do rótulo de acertos
            ui->errosLabel->setText(QString("Erros: %1").arg(erros)); // Atualiza o texto do rótulo de erros
            int currentTime = QDateTime::currentMSecsSinceEpoch(); // Obtém o tempo atual em milissegundos desde a época
            reactionTime = currentTime - startTime; // Calcula o tempo de reação
            double reactionTimeInSeconds = reactionTime / 1000.0; // Converte o tempo de reação para segundos
            ui->reactionLabel->setText(QString("Reaction time: %1 s").arg(QString::number(reactionTimeInSeconds, 'f', 2))); // Atualiza o texto do rótulo de tempo de reação
            reactionTimes.append(reactionTimeInSeconds); // Adiciona o tempo de reação à lista de tempos de reação

            if (reactionTimes.size() > 0) {
                // Calcula o tempo médio de reação em segundos
                double averageReactionTime = std::accumulate(reactionTimes.begin(), reactionTimes.end(), 0.0) / reactionTimes.size();
                ui->averageLabel->setText(QString("Average Reaction Time: %1 s").arg(QString::number(averageReactionTime, 'f', 2))); // Atualiza o texto do rótulo de tempo médio de reação

            }
        } else {
            // Errou
            erros++; // Incrementa o número de erros
            ui->scoreLabel->setText(QString("Score: %1").arg(acertos - erros)); // Atualiza o texto do rótulo de pontuação
            ui->acertosLabel->setText(QString("Acertos: %1").arg(acertos)); // Atualiza o texto do rótulo de acertos
            ui->errosLabel->setText(QString("Erros: %1").arg(erros)); // Atualiza o texto do rótulo de erros

        }
        updateCircle(); // Atualiza o círculo
    }
}

void MainWindow::decrementScore() // Função para decrementar a pontuação
{
    erros++; // Incrementa o número de erros
    ui->scoreLabel->setText(QString("Score: %1").arg(acertos - erros)); // Atualiza o texto do rótulo de pontuação
    ui->acertosLabel->setText(QString("Acertos: %1").arg(acertos)); // Atualiza o texto do rótulo de acertos
    ui->errosLabel->setText(QString("Erros: %1").arg(erros)); // Atualiza o texto do rótulo de erros
    updateCircle(); // Atualiza o círculo
}

void MainWindow::reset() // Função para resetar o jogo
{
    acertos = 0; // Zera o número de acertos
    erros = 0; // Zera o número de erros
    reactionTimes.clear(); // Limpa a lista de tempos de reação
    ui->scoreLabel->setText("Score: 0"); // Atualiza o texto do rótulo de pontuação
    ui->reactionLabel->setText(QString("Reaction time: 0")); // Atualiza o texto do rótulo de tempo de reação
    ui->averageLabel->setText(QString("Average Reaction Time: 0"));
    ui->acertosLabel->setText(QString("Acertos: 0")); // Atualiza o texto do rótulo de acertos
    ui->errosLabel->setText(QString("Erros: 0")); // Atualiza o texto do rótulo de erros
    circleColor = Qt::yellow; // Define a cor inicial do círculo como amarelo
    updateCircle(); // Atualiza o círculo
}

void MainWindow::updateCircle() // Função para atualizar a posição e a cor do círculo
{
    // Registra o tempo de início
    startTime = QDateTime::currentMSecsSinceEpoch();
    int x = rand() % (ui->centralwidget->width() - circleSize); // Define uma posição aleatória para o círculo ao longo do eixo x
    int y = rand() % (ui->centralwidget->height() - circleSize); // Define uma posição aleatória para o círculo ao longo do eixo y
    circlePosition = QPoint(x, y); // Define a posição do círculo
    int colorIndex = rand() % 4; // Define uma cor aleatória para o círculo
    switch (colorIndex) {
    case 0:
        circleColor = Qt::yellow;
        break;
    case 1:
        circleColor = Qt::green;
        break;
    case 2:
        circleColor = Qt::blue;
        break;
    case 3:
        circleColor = Qt::red;
        break;
    }
    update(); // Atualiza a tela
    noKeyPressTimer->start(5000); // Reinicia o timer a cada 5 segundos (5000 ms)

}

void MainWindow::paintEvent(QPaintEvent *) // Função para desenhar o círculo
{
    QPainter painter(this);
    painter.setBrush(circleColor);
    painter.drawEllipse(circlePosition, circleSize, circleSize);
}





