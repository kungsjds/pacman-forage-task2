#include "gameclass.h"

int     GameLoop::ft_check_file_inp(std::string str)
{
    int i;

    i = 0;
    while (str[i])
    {
        if (str[i] < '0' || str[i] > '4')
            return (1);
        i++;
    }
    return (0);
}

GameLoop::~GameLoop()
{
    delete scene;
    delete view;
    delete pacman;
    delete pinky;
    delete clyde;
    delete blinky;
    delete inky;
    delete timer_pacman;
    delete timer_blinky;
    delete timer_pinky;
    delete timer_clyde;
    delete timer_inky;
    for (int i = 0; i < size_x; i++)
    {
        for (int j = 0; j < size_y; j++)
        {
            delete map_pix[i];
            delete map_int[i];
        }
    }
    delete []map_pix;
    delete []map_int;
    ghost_instance.clear();
    // CHANGE - Delete ghosts
}

void    GameLoop::ft_roll_game()
{
//    timer_blinky = new QTimer();
//    timer_pinky = new QTimer();
//    timer_clyde = new QTimer();
    timer_pacman = new QTimer();
    timer_inky = new QTimer();

    QObject::connect(timer_pacman, SIGNAL(timeout()), pacman, SLOT(ft_move()));

    for (int i = 0; i < ghost_instance.size(); i++) {

        // CHANGE - Setup all ghosts move
        QObject::connect(timer_inky, SIGNAL(timeout()), ghost_instance[i], SLOT(ft_move_ghost()));

        // CHANGE
        // ? Why more than a timer if every ghosts and timers have the same value ?
//        QObject::connect(timer_clyde, SIGNAL(timeout()), clyde, SLOT(ft_move_ghost()));
//        QObject::connect(timer_blinky, SIGNAL(timeout()), blinky, SLOT(ft_move_ghost()));
//        QObject::connect(timer_pinky, SIGNAL(timeout()), pinky, SLOT(ft_move_ghost()));

    };

    timer_pacman->start(300);
    timer_inky->start(400);

    // CHANGE
    // ? Why more than a timer if every ghosts and timers have the same value ?
//    timer_clyde->start(400);
//    timer_blinky->start(400);
//    timer_pinky->start(400);
}

void    GameLoop::ft_create_map()
{
    for (int i = 0; i < size_x; i++)
    {
        for (int j = 0; j < size_y; j++)
        {
            if (map_int[i][j] == 1)
            {
                map_pix[i][j].setPixmap(QPixmap(":/pics/greystone.png"));
                map_pix[i][j].setPos(j * 32, i * 32);
                scene->addItem(&(map_pix[i][j]));
            }
            if (map_int[i][j] == 3)
            {
                map_pix[i][j].setPixmap(QPixmap(":/pics/ball.png"));
                map_pix[i][j].setPos(j * 32, i * 32);
                scene->addItem(&(map_pix[i][j]));
            }
            if (map_int[i][j] == 4)
            {
                map_pix[i][j].setPixmap(QPixmap(":/pics/energizer.png"));
                map_pix[i][j].setPos(j * 32, i * 32);
                scene->addItem(&(map_pix[i][j]));
            }
        }
    }
}

void    GameLoop::ft_write_line_map(int *map, std::string str)
{
    for (int i = 0; i < size_y; i++)
        map[i] = str[i] - '0';
}
GameLoop::GameLoop(char *file_name)
{
    std::ifstream   input;
    std::string     temp;
    int             nb;

    map_int = new int *[size_x];
    map_pix = new QGraphicsPixmapItem *[size_x];
    for (int i = 0; i < size_x; i++)
    {
        map_int[i] = new int[size_y];
        map_pix[i] = new QGraphicsPixmapItem[size_y];
    }

    input.open(file_name);
    if (input.is_open())
    {
        nb = 0;
        while (std::getline(input, temp))
        {
            if (temp.length() != size_y)
            {
                std::cout << "File error.\n";
                exit(0);
            }
            if (ft_check_file_inp(temp))
            {
                std::cout << "File error.\n";
                exit(0);
            }
            ft_write_line_map(map_int[nb], temp);
            nb++;
        }
        if (nb != size_x)
        {
            std::cout << "File error.\n";
            exit(0);
        }
        input.close();
    }
    else
    {
        std::cout << "File missing: " << file_name << ".\n";
        exit(0);
    }
    scene = new QGraphicsScene();
    view = new QGraphicsView();
    this->setScene(scene);
    ft_create_map();
    this->setStyleSheet("background-color:black;");
    pacman = new PacMan(map_int, map_pix, scene);
    pacman->setFlag(QGraphicsPixmapItem::ItemIsFocusable);
    pacman->setFocus();

    blinky = new Blinky(scene, map_int, pacman);
    pinky = new Pinky(scene, map_int, pacman);
    clyde = new Clyde(scene, map_int, pacman);
    inky = new Inky(scene, map_int, pacman);
    blinky->ft_set_friends(pinky, clyde, inky);
    pinky->ft_set_friends(blinky, clyde, inky);
    clyde->ft_set_friends(blinky, pinky, inky);
    inky->ft_set_friends(blinky, pinky, clyde);

    ghost_instance.append(blinky);
    ghost_instance.append(pinky);
    ghost_instance.append(inky);
    ghost_instance.append(clyde);

    startTimer(30000);
    // CHANGE - Start ghost doubling timer

};

// CHANGE - Double the ghosts
void GameLoop::ft_double_ghost() {

    for (int i = 0; i < ghost_instance.size(); i++) {

        if (ghost_instance[i]->ft_get_killed() == 1) {

            ghost_instance.removeAt(i);

        };

    };

    int size = ghost_instance.size();

    int xblinky = 0;
    int xpinky = 0;
    int xclyde = 0;
    int xinky = 0;

    for (int i = 0; i < size; i++) {

        if (ghost_instance.size() <= 128 && ghost_instance[i]->ft_get_killed() == 0){

            if (xblinky == 0) {
                blinky = new Blinky(scene, map_int, pacman);
                blinky->ft_set_friends(pinky, clyde, inky);

                ghost_instance.append(blinky);

                xblinky = 1;
                continue;
            };

            if (xpinky == 0) {
                pinky = new Pinky(scene, map_int, pacman);
                pinky->ft_set_friends(blinky, clyde, inky);

                ghost_instance.append(pinky);

                xpinky = 1;
                continue;
            };

            if (xclyde == 0) {
                clyde = new Clyde(scene, map_int, pacman);
                clyde->ft_set_friends(blinky, pinky, inky);

                ghost_instance.append(clyde);

                xclyde = 1;
                xinky = 0;
                continue;
            };

            if (xinky == 0) {
                inky = new Inky(scene, map_int, pacman);
                inky->ft_set_friends(blinky, pinky, clyde);

                ghost_instance.append(inky);

                xinky = 1;
                xblinky = 0;
                xpinky = 0;
                xclyde = 0;
            };

        };

    };

    for (int i = size; i < ghost_instance.size(); i++) {

        QObject::connect(timer_inky, SIGNAL(timeout()), ghost_instance[i], SLOT(ft_move_ghost()));

    };

    // CHANGE - End game
    if (ghost_instance.size() >= 128) {
        end_game = 1;
    };

};

// CHANGE - timer to double ghosts
void GameLoop::timerEvent(QTimerEvent *e){

    ft_double_ghost();

};
