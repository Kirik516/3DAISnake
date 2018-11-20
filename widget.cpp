#include "widget.h"

Widget::Widget(QScreen * targetScreen)
    : QWindow(targetScreen)
{
    this->c = new QOpenGLContext(this);

    this->setSurfaceType(QWindow::OpenGLSurface);

    QSurfaceFormat sf;
    sf.setRenderableType(QSurfaceFormat::OpenGL);
    sf.setSwapBehavior(QSurfaceFormat::SingleBuffer);
    sf.setMajorVersion(1);
    sf.setMinorVersion(0);
    this->c->setFormat(sf);
//    this->c->setFormat(this->requestedFormat());
    this->c->create();
    if (this->c->isValid())
    {
        qDebug() << "QOpenGLContext is valid";
    }
    else
    {
        qDebug() << "QOpenGLContext is invalid";
    }

    m_points = 0;
    m_quad_size = 80;
    m_triangle_size = 80;
    m_window_size_x = 1920;
    m_window_size_y = 1040;
    m_singling = false;
//    quad_coord();
//    triangle_coord();
    m_timer = new QTimer(this);
    m_timer->start(750);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(quad_coord()));
    connect(m_timer, SIGNAL(timeout()), this, SLOT(triangle_coord()));
}

Widget::~Widget()
{
    delete m_timer;
    this->c->doneCurrent();
}

void Widget::self_cursor()
{
    glBegin(GL_POLYGON);
        glColor3f(1, 1, 1);
        glVertex2i(m_cursor_ax, m_cursor_ay);
        glVertex2i(m_cursor_ax + 20, m_cursor_ay + 20);
        glVertex2i(m_cursor_ax + 8, m_cursor_ay + 20);
        glVertex2i(m_cursor_ax, m_cursor_ay + 30);
    glEnd();
}

void Widget::exposeEvent(QExposeEvent *)
{
    if (this->isExposed())
    {
        this->c->makeCurrent(this);

        qDebug() << this->initializeOpenGLFunctions();

        if (!this->c->isValid())
        {
            qDebug() << "QOpenGLContext is invalid";
        }

        this->glDepthFunc(GL_LEQUAL);
        this->glClearColor(0.0, 0.0, 0.0, 1.0);
//        this->glViewport(0, 0, (GLsizei) this->width(), (GLsizei) this->height());
//        this->glMatrixMode(GL_PROJECTION);
//        this->glLoadIdentity();
//        this->glFrustum(-2.0, 2.0, -2.0, 2.0, -1.5, 20.0);
//        this->glMatrixMode(GL_MODELVIEW);
        qDebug() << "Widget is exposed";
        this->render();
        return;
    }
    qDebug() << "Widget is unexposed";
}

void Widget::resizeEvent(QResizeEvent *e)
{
    if (this->isExposed())
    {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glViewport(0, 0, (GLint)e->size().width(), (GLint)e->size().height());
        m_window_size_x = e->size().width();
        m_window_size_y = e->size().height();
        this->render();
    }
}

void Widget::keyPressEvent(QKeyEvent *e)
{
    this->c->makeCurrent(this);
    switch(e->key())
    {
        case Qt::Key_Space:
            quad_coord();
            triangle_coord();
        break;
    }

    render();
}

void Widget::singling_lb()
{
    glBegin(GL_POLYGON);
        glColor4f(0,1,0, 0.25);
        glVertex2f(m_cursor_bx, m_cursor_by);
        glVertex2f(m_cursor_ax, m_cursor_bx);
        glVertex2f(m_cursor_ax, m_cursor_ay);
        glVertex2f(m_cursor_bx, m_cursor_ay);
    glEnd();
}

void Widget::quad()
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

void Widget::triangle()
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

void Widget::mouseMoveEvent(QMouseEvent *e)
{
    this->c->makeCurrent(this);
    m_cursor_ax = e->x();
    m_cursor_ay = e->y();
    qDebug() << "x: " << this->m_cursor_ax;
    qDebug() << "y: " << this->m_cursor_ay;

    this->render();
}

void Widget::mousePressEvent(QMouseEvent *e)
{
    this->c->makeCurrent(this);
    if(e->button() == Qt::LeftButton) {

        if(e->x() > m_quad_dx && e->x() < m_quad_dx + m_quad_size) {

            if(e->y() > m_quad_dy && e->y() < m_quad_dy + m_quad_size) {

                m_points++;
                quad_coord();
            }
        }

        if(e->x() > m_triangle_dx && e->x() < m_triangle_dx + m_triangle_size) {

            if(e->y() > m_triangle_dy && e->y() < m_triangle_dy + m_triangle_size) {

                m_points++;
                triangle_coord();
            }
        }

        m_singling = true;
        m_cursor_bx = e->x();
        m_cursor_by = e->y();
//        this->render(); // внизу уже есть запрос перерисвки
    }
    else
        m_singling = false;

    this->render();
}

void Widget::mouseReleaseEvent(QMouseEvent *e)
{
    this->c->makeCurrent(this);
    if(m_singling == true && e->button()==Qt::LeftButton)
        m_singling=false;

    this->render();
}

void Widget::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, m_window_size_x, m_window_size_y, 0, 1, 0);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//    qglColor(Qt::white);
    this->glColor3f(1.0, 1.0, 1.0);

//    renderText(5, 20, 0, QString::fromUtf8("You are get %1 scores:").arg(m_points), QFont());

    quad();
    triangle();

    if(m_singling == true)
        singling_lb();

    self_cursor();

    this->glEnd();
    this->glFlush();

    this->c->swapBuffers(this);
}

void Widget::quad_coord()
{
    this->c->makeCurrent(this);
    m_quad_dx = (rand() % (m_window_size_x-m_quad_size * 2)) + m_quad_size;
    m_quad_dy = (rand() % (m_window_size_y-m_quad_size * 2)) + m_quad_size;
//    qDebug() <<"(" << m_quad_dx << "," << m_quad_dy << ")";
    this->render();
}

void Widget::triangle_coord()
{
    this->c->makeCurrent(this);
    m_triangle_dx = (rand() % (m_window_size_x - m_triangle_size * 2)) + m_triangle_size;
    m_triangle_dy = (rand() % (m_window_size_y - m_triangle_size * 2)) + m_triangle_size;
//    qDebug() << m_triangle_dx << m_triangle_dy;
    this->render();
}
