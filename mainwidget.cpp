#include "mainwidget.h"

MainWidget::MainWidget(QWidget *parent)
    : QGLWidget(parent) {
    m_points = 0;
    m_quad_size = 80;
    m_triangle_size = 80;
    m_window_size_x = 1920;
    m_window_size_y = 1040;
    m_singling = false;
    quad_coord();
    triangle_coord();
    setFormat(QGLFormat(QGL::DoubleBuffer));
    glDepthFunc(GL_LEQUAL);
    m_timer = new QTimer(this);
    m_timer->start(750);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(quad_coord()));
    connect(m_timer, SIGNAL(timeout()), this, SLOT(triangle_coord()));

    setAutoBufferSwap(true);
}

void MainWidget::init_GL() {

    qglClearColor(Qt::black);
}

void MainWidget::resizeGL(int nWidth, int nHeight) {

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, (GLint)nWidth, (GLint)nHeight);
    m_window_size_x = nWidth;
    m_window_size_y = nHeight;
}

void MainWidget::paintGL() {

//    makeCurrent();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, m_window_size_x, m_window_size_y, 0, 1, 0);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    qglColor(Qt::white);

    renderText(5, 20, 0, QString::fromUtf8("You are get %1 scores:").arg(m_points), QFont());

    quad();
    triangle();

    if(m_singling == true)
        singling_lb();

    self_cursor();
}

void MainWidget::keyPressEvent(QKeyEvent *ke)
{
    switch(ke->key())
    {
        case Qt::Key_Space:
            quad_coord();
            triangle_coord();
        break;
    }

    updateGL();

}

void MainWidget::mouseMoveEvent(QMouseEvent *me)
{
    m_cursor_ax = me->x();
    m_cursor_ay = me->y();

    updateGL();
}

void MainWidget::mousePressEvent(QMouseEvent *me)
{

    if(me->button() == Qt::LeftButton) {

        if(me->x() > m_quad_dx && me->x() < m_quad_dx + m_quad_size) {

            if(me->y() > m_quad_dy && me->y() < m_quad_dy + m_quad_size) {

                m_points++;
                quad_coord();
            }
        }

        if(me->x() > m_triangle_dx && me->x() < m_triangle_dx + m_triangle_size) {

            if(me->y() > m_triangle_dy && me->y() < m_triangle_dy + m_triangle_size) {

                m_points++;
                triangle_coord();
            }
        }

        m_singling = true;
        m_cursor_bx = me->x();
        m_cursor_by = me->y();
        updateGL();
    }
    else
        m_singling = false;

    updateGL();

}

void MainWidget::mouseReleaseEvent(QMouseEvent *me)
{

    if(m_singling == true && me->button()==Qt::LeftButton)
        m_singling=false;

    updateGL();

}

void MainWidget::self_cursor()
{
    glBegin(GL_POLYGON);
        glColor3f(1, 1, 1);
        glVertex2f(m_cursor_ax, m_cursor_ay);
        glVertex2f(m_cursor_ax + 20, m_cursor_ay + 20);
        glVertex2f(m_cursor_ax + 8, m_cursor_ay + 20);
        glVertex2f(m_cursor_ax, m_cursor_ay + 30);
    glEnd();
}


void MainWidget::singling_lb()
{
      glBegin(GL_POLYGON);
          glColor4f(0,1,0, 0.25);
          glVertex2f(m_cursor_bx, m_cursor_by);
          glVertex2f(m_cursor_ax, m_cursor_bx);
          glVertex2f(m_cursor_ax, m_cursor_ay);
          glVertex2f(m_cursor_bx, m_cursor_ay);
      glEnd();
}

void MainWidget::quad()
{
       glBegin(GL_POLYGON);
           glColor3f(0.0, 0.0, 0.5);
           glVertex2f(m_quad_dx, m_quad_dy);
           glColor3f(0.0, 0.5, 0.0);
           glVertex2f(m_quad_dx + m_quad_size, m_quad_dy);
           glColor3f(0.5, 0.0, 0.0);
           glVertex2f(m_quad_dx + m_quad_size, m_quad_dy + m_quad_size);
           glColor3f(0.5, 0.5, 0.5);
           glVertex2f(m_quad_dx, m_quad_dy + m_quad_size);
       glEnd();
}

void MainWidget::triangle()
{
    glBegin(GL_POLYGON);
        glColor3f(0.0, 0.0, 1.0);
        glVertex2f(m_triangle_dx + m_triangle_size, m_triangle_dy + m_triangle_size);
        glColor3f(0.0, 1.0, 0.0);
        glVertex2f(m_triangle_dx + m_triangle_size, m_triangle_dy);
        glColor3f(1.0, 0.0, 0.0);
        glVertex2f(m_triangle_dx, m_triangle_dy + m_triangle_size);
    glEnd();

}

void MainWidget::quad_coord()
{
       m_quad_dx = (rand() % (m_window_size_x-m_quad_size * 2)) + m_quad_size;
       m_quad_dy = (rand() % (m_window_size_y-m_quad_size * 2)) + m_quad_size;
       qDebug() <<"(" << m_quad_dx << "," << m_quad_dy << ")";
       updateGL();
}

void MainWidget::triangle_coord()
{
       m_triangle_dx = (rand() % (m_window_size_x - m_triangle_size * 2)) + m_triangle_size;
       m_triangle_dy = (rand() % (m_window_size_y - m_triangle_size * 2)) + m_triangle_size;
       qDebug() << m_triangle_dx << m_triangle_dy;
       updateGL();
}

MainWidget::~MainWidget()
{
    delete m_timer;
}



