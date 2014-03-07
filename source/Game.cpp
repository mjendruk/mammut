#include "Game.h"

#include <QTimer>
#include <QThread>
#include <QDebug>
#include <QMouseEvent>

#include <glowutils/File.h>

#include <PerfCounter.h>

#include <rendering/menu/MenuRenderer.h>
#include <rendering/world/GameWorldRenderer.h>
#include <rendering/menu/BlankBackground.h>
#include <rendering/menu/ScreenshotBackground.h>

#include <rendering/Canvas.h>

#include <logic/world/GameMechanics.h>
#include <logic/menu/GameOverMenu.h>
#include <logic/menu/HighscoreMenu.h>
#include <logic/menu/MainMenu.h>
#include <logic/menu/PauseMenu.h>
#include <logic/menu/NewHighscoreMenu.h>

#include <sound/SoundManager.h>

Game::Game(int & argc, char ** argv)
:   AbstractApplication(argc, argv)
,   m_loop(false)
,   m_paused(false)
{
    initializeWindow();
    initializeRendering();
    
    showMainMenu();
    
    QTimer::singleShot(0, this, SLOT(run()));
}

Game::~Game()
{
}

void Game::run()
{
    m_loop = true;

    long double lastTime = m_timer.elapsed();
    long double currentTime;
    long double frameTime;
    
    while(m_loop)
    {   
        PerfCounter::begin("total");
        currentTime = m_timer.elapsed();
        frameTime = currentTime - lastTime;
        lastTime = currentTime;

        SoundManager::instance().updateSoundSystem();
        QCoreApplication::processEvents();
        
        if (!m_paused)
            m_activeMechanics->update(frameTime / std::nano::den);

        if(!m_window.isMinimized())
            m_canvas->render();
        PerfCounter::end("total");

        if (m_activeMechanics == m_gameMechanics)
            qDebug() << qPrintable(PerfCounter::generateString());
    }
}

void Game::initializeWindow()
{
    QSurfaceFormat format;
    format.setVersion(4, 1);
    format.setDepthBufferSize(24);
    format.setProfile(QSurfaceFormat::CoreProfile);
    
    m_canvas = new Canvas(format);
    m_canvas->installEventFilter(this);
    m_canvas->setSwapInterval(Canvas::NoVerticalSyncronization);
    
    QWidget * canvasWidget = QWidget::createWindowContainer(m_canvas);
    
    m_window.setCentralWidget(canvasWidget);
    m_window.setMinimumSize(800, 600);
    m_window.setFocusProxy(canvasWidget);
    m_window.setFocus();
    m_window.show();
}

void Game::initializeRendering()
{
    m_menuRenderer.reset(new MenuRenderer());
    m_gameWorldRenderer.reset(new GameWorldRenderer());
    m_blankBackground.reset(new BlankBackground());
    m_screenshotBackground.reset(new ScreenshotBackground());
}

void Game::startGame()
{
    m_gameMechanics = std::make_shared<GameMechanics>();
    m_gameWorldRenderer->setGameMechanics(m_gameMechanics.get());
    
    connect(m_gameMechanics.get(), &GameMechanics::pause, this, &Game::pauseGame);
    connect(m_gameMechanics.get(), &GameMechanics::gameOver, this, &Game::endGame);
    
    m_activeMechanics = m_gameMechanics;
    m_canvas->setRenderer(m_gameWorldRenderer.get());
}

void Game::pauseGame()
{
    auto menu = std::make_shared<PauseMenu>();
    
    connect(menu.get(), &PauseMenu::resumePressed, this, &Game::resumeGame);
    connect(menu.get(), &PauseMenu::toMainMenuPressed, this, &Game::showMainMenu);
    
    m_screenshotBackground->setScreenshot(m_canvas->screenshot());
    
    activateMenu(menu, m_screenshotBackground.get());}

void Game::resumeGame()
{
    m_activeMechanics = m_gameMechanics;
    m_canvas->setRenderer(m_gameWorldRenderer.get());
}

void Game::showMainMenu()
{
    auto menu = std::make_shared<MainMenu>();
    
    connect(menu.get(), &MainMenu::startPressed, this, &Game::startGame);
    connect(menu.get(), &MainMenu::highscorePressed, this, &Game::showHighscoreMenu);
    connect(menu.get(), &MainMenu::quitPressed, this, &Game::quit);
    
    activateMenu(menu, m_blankBackground.get());
}

void Game::endGame(int score)
{
    m_gameMechanics.reset();
    
    if (m_highscoreList.isHighscore(score))
        showNewHighscoreMenu(score);
    else
        showGameOverMenu(score);
}

void Game::showHighscoreMenu()
{
    auto menu = std::make_shared<HighscoreMenu>(m_highscoreList.scores());
    
    connect(menu.get(), &HighscoreMenu::backPressed, this, &Game::showMainMenu);
    
    activateMenu(menu, m_blankBackground.get());
}

void Game::showNewHighscoreMenu(int score)
{
    auto menu = std::make_shared<NewHighscoreMenu>(m_lastName, score);
    
    connect(menu.get(), &NewHighscoreMenu::nameEntered,
            [this, score](const QString & name)
    {
        m_highscoreList.addScore(name, score);
        m_lastName = name;
        showGameOverMenu(score);
    });
    
    activateMenu(menu, m_blankBackground.get());
}

void Game::showGameOverMenu(int score)
{
    auto menu = std::make_shared<GameOverMenu>(score);
    
    connect(menu.get(), &GameOverMenu::retryPressed, this, &Game::startGame);
    connect(menu.get(), &GameOverMenu::toMainMenuPressed, this, &Game::showMainMenu);
    
    activateMenu(menu, m_blankBackground.get());
}

void Game::quit()
{
    m_loop = false;
    QApplication::quit();
}

void Game::activateMenu(std::shared_ptr<Menu> menu, AbstractBackground * background)
{
    m_activeMechanics = menu;
    m_menuRenderer->setMenu(menu.get());
    m_menuRenderer->setBackground(background);
    m_canvas->setRenderer(m_menuRenderer.get());
}

bool Game::eventFilter(QObject * obj, QEvent * event)
{
    switch (event->type())
    {
        case QEvent::KeyPress:
            keyPressed((QKeyEvent *)event);
            break;
            
        case QEvent::KeyRelease:
            keyReleased((QKeyEvent *)event);
            break;
            
        case QEvent::Hide:
            m_loop = false;
            break;
            
        default:
            return QObject::eventFilter(obj, event);
            break;
    }
    
    return true;
}


void Game::keyPressed(QKeyEvent * keyEvent)
{
    if (keyEvent->isAutoRepeat())
        return;
    
    if (keyEvent->modifiers() == Qt::ControlModifier)
    {
        switch (keyEvent->key())
        {
            case Qt::Key_Return:
                if (m_window.isFullScreen())
                    m_window.showNormal();
                else
                    m_window.showFullScreen();
                return;
            case Qt::Key_Space:
                m_paused = !m_paused;
                return;
            case Qt::Key_R:
                glowutils::File::reloadAll();
                qDebug() << "Reload shaders.";
                return;
            case Qt::Key_I:
                m_canvas->toggleSwapInterval();
                return;
            default:
                break;
        }
    }
    
    m_activeMechanics->keyPressed(keyEvent);
}

void Game::keyReleased(QKeyEvent * keyEvent)
{
    if (keyEvent->isAutoRepeat())
        return;

    m_activeMechanics->keyReleased(keyEvent);
}
