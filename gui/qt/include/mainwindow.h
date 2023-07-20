#pragma once

#include <water_effect.h>
#include <wre/water_ripple.h>

#include <memory>

#include <QMainWindow>
#include <QPushButton>
#include <QTimer>
#include <QEvent>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

    bool eventFilter(QObject* obj, QEvent* event) override;

private:
    void setup_layout();
    void setup_signals();
private slots:
    void OnLoadImage();
    void OnImageUpdate();
private:
    std::unique_ptr<Ui::MainWindow> m_ui;
    std::unique_ptr<QTimer> m_timer;
    water_effect m_water_effect;
    bool m_clear;
};