#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QSurfaceFormat>
#include <QOpenGLContext>
#include <QOpenGLFunctions_1_0>

#include <QTimer>

#include <QKeyEvent>

#include <QWindow>

#include <QDebug>

class Widget : public QWindow, protected QOpenGLFunctions_1_0
{
    Q_OBJECT

    QOpenGLContext *c;

public:
    Widget(QScreen * targetScreen = 0);
    ~Widget();

private:
    int m_quad_size;
    int m_quad_dx, m_quad_dy;

    int m_triangle_size;
    int m_triangle_dx, m_triangle_dy;
    int m_points;

    int m_cursor_ax, m_cursor_ay, m_cursor_bx, m_cursor_by;
    int m_window_size_x, m_window_size_y;
    bool m_singling;

    QTimer* m_timer;

    void self_cursor();
    virtual void exposeEvent(QExposeEvent *e);
    virtual void resizeEvent(QResizeEvent *e);
    virtual void mouseMoveEvent(QMouseEvent* e);
    virtual void mousePressEvent(QMouseEvent* e);
    virtual void mouseReleaseEvent(QMouseEvent* e);
    virtual void keyPressEvent(QKeyEvent *e);
    void singling_lb();
    void quad();
    void triangle();

public slots:
    void render();

    void quad_coord();
    void triangle_coord();
};

#endif // WIDGET_H
