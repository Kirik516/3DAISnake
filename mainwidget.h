#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QGLWidget>
#include <QtOpenGL>
#include <QTimer>

class MainWidget : public QGLWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = 0);
    virtual ~MainWidget();

private:
    int m_quad_size;
    int m_quad_dx, m_quad_dy;

    int m_triangle_size;
    int m_triangle_dx, m_triangle_dy;
    int m_points;

    int m_cursor_ax, m_cursor_ay, m_cursor_bx, m_cursor_by;
    int m_window_size_x, m_window_size_y;
    bool m_singling;

    void self_cursor();
    void init_GL();
    void resizeGL(int nWidth, int nHeight);
    void paintGL();
    void keyPressEvent(QKeyEvent* ke);
    void mouseMoveEvent(QMouseEvent* me);
    void mousePressEvent(QMouseEvent* me);
    void mouseReleaseEvent(QMouseEvent* me);
    void singling_lb();
    void quad();
    void triangle();

protected slots:
    void quad_coord();
    void triangle_coord();

private:
    QTimer* m_timer;
};

#endif // MAINWIDGET_H
