#ifndef GAMECLASS_H
#define GAMECLASS_H

#include <fstream>
#include <string>
#include <ctime>
#include <random>
#include <vector>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <QTimer>
#include <QTimerEvent> /* CHANGE - timer for ghosts doubling */
#include <QObject>
#include <QApplication>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include "pacman.h"
#include "ghost.h"
#include "blinky.h"
#include "pinky.h"
#include "clyde.h"
#include "inky.h"
#include <vector>

using std::vector;

extern int end_game;

class   GameLoop : public QGraphicsView
{
private:
    QGraphicsScene                          *scene;
    QGraphicsView                           *view;
    QGraphicsPixmapItem                     **map_pix;
    PacMan                                  *pacman;
    Blinky                                  *blinky;
    Pinky                                   *pinky;
    Clyde                                   *clyde;
    Inky                                    *inky;
    int                                     **map_int;
    QTimer                                  *timer_pacman;
    QTimer                                  *timer_blinky;
    QTimer                                  *timer_pinky;
    QTimer                                  *timer_clyde;
    QTimer                                  *timer_inky;
    QList<Ghost* >                          ghost_instance; // CHANGE - Ghosts instance for multiple ghosts

private slots:
    void timerEvent(QTimerEvent *e); // CHANGE - Timer for boubling ghosts

public:
                                            GameLoop(char *file_name);
                                            ~GameLoop();
    void                                    ft_write_line_map(int *map, std::string str);
    void                                    ft_roll_game();
    void                                    ft_create_map();
    int                                     ft_check_file_inp(std::string str);
    void                                    ft_double_ghost(); // CHANGE - Ghost doubling function
};

#endif // GAMECLASS_H
