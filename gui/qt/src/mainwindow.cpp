#include <mainwindow.h>
#include <ui_mainwindow.h>
#include <water_effect.h>

#include <cstdint>
#include <memory>

#include <QPixmap>
#include <QMouseEvent>
#include <QPixmap>
#include <QImage>
#include <QGridLayout>
#include <QScrollArea>
#include <QLayout>
#include <QMainWindow>
#include <QGridLayout>

#define DEFAULT_IMAGE_PATH "./resources/screen.png"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_ui(new Ui::MainWindow)
    , m_timer(new QTimer(this))
    , m_water_effect{ DEFAULT_IMAGE_PATH }
    , m_clear{ false }
{
    m_ui->setupUi(this);
    setup_layout();
    setup_signals();
}

void MainWindow::setup_signals()
{
    connect(this, &MainWindow::resizeEvent, [this]() {
        m_ui->label->setMaximumSize(QSize(this->width(), this->height() - m_ui->pushButton->height()));
    });

    connect(m_ui->pushButton, SIGNAL(clicked()), this, SLOT(OnLoadImage()));
    connect(m_timer.get(), &QTimer::timeout, this, &MainWindow::OnImageUpdate);
}

void MainWindow::setup_layout()
{
    m_ui->label->setScaledContents(true);
    m_ui->label->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

    QVBoxLayout* image_layout = new QVBoxLayout;
    image_layout->addWidget(m_ui->label);
    image_layout->setContentsMargins(0, 0, 0, 0);
    image_layout->setSpacing(0);
    image_layout->setStretchFactor(m_ui->label, 1);

    m_ui->pushButton->setFixedSize(150, 40);

    QHBoxLayout* button_layout = new QHBoxLayout;
    button_layout->addStretch();
    button_layout->addWidget(m_ui->pushButton, 0, Qt::AlignHCenter);
    button_layout->addStretch();

    QVBoxLayout* main_layout = new QVBoxLayout;
    main_layout->addLayout(image_layout, 1);
    main_layout->addLayout(button_layout);

    // Allow the label to receive mouse move events even if no button is pressed
    m_ui->label->setMouseTracking(true);
    m_ui->label->installEventFilter(this);

    this->centralWidget()->setLayout(main_layout);
}

MainWindow::~MainWindow()
{
}

void MainWindow::OnLoadImage()
{
    if (!m_timer->isActive())
        m_timer->start(1);
}

void MainWindow::OnImageUpdate()
{
    if (m_clear)
        m_water_effect.clear();
    
    m_water_effect.render();
    m_ui->label->setPixmap(QPixmap::fromImage(m_water_effect.get_render_output()));
}

bool MainWindow::eventFilter(QObject* obj, QEvent* event)
{
    //const uint32_t move_radius{ 50 };
    const uint32_t click_radius{ 100 };

    if (qobject_cast<QLabel *>(obj) == m_ui->label)
    {
        auto ev_type = event->type();
        if (ev_type == QEvent::MouseMove)
        {
            QMouseEvent* mouse_event = static_cast<QMouseEvent *>(event);
            
            int x(mouse_event->position().x());
            int y(mouse_event->position().y());

            int imgX = x * m_water_effect.width() / m_ui->label->width();
            int imgY = y * m_water_effect.height() / m_ui->label->height();

            m_water_effect.make_ripple(imgX, imgY, 30, 500);
            return true;
        }
        else if (ev_type == QEvent::MouseButtonPress)
        {
            QMouseEvent* mouse_event = static_cast<QMouseEvent *>(event);

            if (mouse_event->button() == Qt::LeftButton)
            {
                int x(mouse_event->position().x());
                int y(mouse_event->position().y());

                int imgX = x * m_water_effect.width() / m_ui->label->width();
                int imgY = y * m_water_effect.height() / m_ui->label->height();

                m_water_effect.make_ripple(imgX, imgY, click_radius, 30*click_radius);
                return true;
            }
            else if (mouse_event->button() == Qt::RightButton)
            {
                m_clear = !m_clear;
                return true;
            }
        }
    }

    return QObject::eventFilter(obj, event);
}