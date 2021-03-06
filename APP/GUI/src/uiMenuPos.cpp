#include "uiMenuPos.h"
#include "uiMenuTrans.h"
#include "uiMenuConfig.h"
#include "uiLogon.h"
#include "uiUserManager.h"
#include "uiReport.h"
#include "uiSettle.h"
#include <QSettings>

#include "global.h"

UIMenuPos::UIMenuPos(QDialog *parent,Qt::WindowFlags f) :
    QDialog(parent,f)
{
    QObject::installEventFilter(this);

    QPixmap bg;
    bg.load(":/images/commonbg.png");
    QPalette palette;
    palette.setBrush(backgroundRole(),QBrush(bg));
    this->setPalette(palette);
    this->setAutoFillBackground(true);
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setGeometry(0,FRAME420_THVALUE,FRAME420_WIDTH,FRAME420_HEIGHT);

    this->setFixedSize(FRAME420_WIDTH,FRAME420_HEIGHT);
    QFont font("Helvetica",12,QFont::Bold);
    QFont font2("Helvetica",16,QFont::Bold);

    lbMenu=new QLabel;
    lbMenuIndexText=new QLabel;

    lbMenu->setText(tr("MENU"));
    lbMenu->setFont(font2);
    lbMenu->setAlignment(Qt::AlignLeft);

    lbMenuIndexText->setText(tr("1/2"));
    lbMenuIndexText->setFont(font2);
    lbMenuIndexText->setAlignment(Qt::AlignRight);

    btnTrans=new QPushButton;
    btnConfig=new QPushButton;
    btnBatch=new QPushButton;
    btnReceipt=new QPushButton;
    btnSettle=new QPushButton;
    btnCashierManager=new QPushButton;
    btnReport=new QPushButton;
    btnParDown=new QPushButton;

    btnPageOneCancel=new QPushButton;
    btnPageTwoCancel=new QPushButton;
    btnPageOneNext=new QPushButton;
    btnPageTwoBack=new QPushButton;

    styleWidget(btnTrans);
    styleWidget(btnConfig);
    styleWidget(btnBatch);
    styleWidget(btnReceipt);
    styleWidget(btnSettle);
    styleWidget(btnCashierManager);
    styleWidget(btnReport);
    styleWidget(btnParDown);

    styleWidget(btnPageOneCancel);
    styleWidget(btnPageOneNext);
    styleWidget(btnPageTwoBack);
    styleWidget(btnPageTwoCancel);


    btnTrans->setText(tr("Transactions"));
    btnConfig->setText(tr("Configuration"));
    btnBatch->setText(tr("Batch"));
    btnReceipt->setText(tr("Receipt"));
    btnSettle->setText(tr("Settlement"));
    btnCashierManager->setText(tr("Cashier Manager"));
    btnReport->setText(tr("Report"));
    btnParDown->setText(tr("Param Download"));

    btnTrans->setFont(font);
    btnConfig->setFont(font);
    btnBatch->setFont(font);
    btnReceipt->setFont(font);
    btnSettle->setFont(font);
    btnCashierManager->setFont(font);
    btnReport->setFont(font);
    btnParDown->setFont(font);

    btnPageOneCancel->setText(tr("Cancel"));
    btnPageTwoCancel->setText(tr("Cancel"));
    btnPageOneNext->setText(tr("Next"));
    btnPageTwoBack->setText(tr("Back"));
    btnPageOneCancel->setFont(font);
    btnPageTwoCancel->setFont(font);
    btnPageOneNext->setFont(font);
    btnPageTwoBack->setFont(font);

    btnTrans->setStyleSheet(BTN_MENU_STYLE);
    btnConfig->setStyleSheet(BTN_MENU_STYLE);
    btnBatch->setStyleSheet(BTN_MENU_STYLE);
    btnReceipt->setStyleSheet(BTN_MENU_STYLE);
    btnSettle->setStyleSheet(BTN_MENU_STYLE);
    btnCashierManager->setStyleSheet(BTN_MENU_STYLE);
    btnReport->setStyleSheet(BTN_MENU_STYLE);
    btnParDown->setStyleSheet(BTN_MENU_STYLE);


    btnPageOneCancel->setStyleSheet(BTN_GREY_STYLE);
    btnPageTwoCancel->setStyleSheet(BTN_GREY_STYLE);
    btnPageOneNext->setStyleSheet(BTN_GREY_STYLE);
    btnPageTwoBack->setStyleSheet(BTN_GREY_STYLE);

    btnPageOneCancel->setFixedHeight(30);
    btnPageTwoCancel->setFixedHeight(30);
    btnPageOneNext->setFixedHeight(30);
    btnPageTwoBack->setFixedHeight(30);

    stackWidget=new QStackedWidget();
    pageOne=new QWidget();
    pageTwo=new QWidget();
    //    pageThree=new QWidget();

    QVBoxLayout *pageOneVLayout=new QVBoxLayout(pageOne);
    QVBoxLayout *pageTwoVLayout=new QVBoxLayout(pageTwo);
    QHBoxLayout *pageOneHLayout=new QHBoxLayout;
    QHBoxLayout *pageTwoHLayout=new QHBoxLayout;

    QSpacerItem *sp1=new QSpacerItem(1,1,QSizePolicy::Expanding,QSizePolicy::Expanding);
    pageOneVLayout->addWidget(btnTrans);
    pageOneVLayout->addWidget(btnConfig);
    pageOneVLayout->addWidget(btnBatch);
    pageOneVLayout->addWidget(btnReceipt);
    pageOneVLayout->addWidget(btnSettle);
    pageOneVLayout->addItem(sp1);
    pageOneHLayout->addWidget(btnPageOneCancel);
    pageOneHLayout->addWidget(btnPageOneNext);
    pageOneVLayout->addLayout(pageOneHLayout);
    //    pageOne->setLayout(pageOneVLayout);

    QSpacerItem *sp2=new QSpacerItem(1,1,QSizePolicy::Expanding,QSizePolicy::Expanding);
    pageTwoVLayout->addWidget(btnCashierManager);
    pageTwoVLayout->addWidget(btnReport);
    pageTwoVLayout->addWidget(btnParDown);
    pageTwoVLayout->addItem(sp2);
    pageTwoHLayout->addWidget(btnPageTwoCancel);
    pageTwoHLayout->addWidget(btnPageTwoBack);
    pageTwoVLayout->addLayout(pageTwoHLayout);
    //    pageTwo->setLayout(pageTwoVLayout);

    stackWidget->addWidget(pageOne);
    stackWidget->addWidget(pageTwo);

    QHBoxLayout *hlayout=new QHBoxLayout;
    hlayout->addWidget(lbMenu);
    hlayout->addWidget(lbMenuIndexText);
    QVBoxLayout *vLayout=new QVBoxLayout(this);
    vLayout->addLayout(hlayout);
    vLayout->addWidget(stackWidget);


    connect(btnPageOneNext,SIGNAL(clicked()),this,SLOT(pageOneNextClicked()));
    connect(btnPageTwoBack,SIGNAL(clicked()),this,SLOT(pageTwoBackClicked()));
    connect(btnPageOneCancel, SIGNAL(clicked()), this, SLOT(close()));
    connect(btnPageTwoCancel, SIGNAL(clicked()), this, SLOT(close()));

    connect(btnTrans,SIGNAL(clicked()),this,SLOT(showTransMenu()));
    connect(btnConfig,SIGNAL(clicked()),this,SLOT(showConfigMenu()));
    connect(btnCashierManager,SIGNAL(clicked()),this,SLOT(showCashierManager()));
    connect(btnReport,SIGNAL(clicked()),this,SLOT(showReport()));
    connect(btnSettle,SIGNAL(clicked()),this,SLOT(showSettle()));

    connect(btnParDown,SIGNAL(clicked()),this,SLOT(Param_Down_Click()));

    this->setAutoClose(g_constantParam.TIMEOUT_UI);

}

UIMenuPos::~UIMenuPos()
{
    qDebug() << Q_FUNC_INFO;
}

void UIMenuPos::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_Escape:
        this->close();
        break;
    case Qt::Key_F1:
        this->pageTwoBackClicked();
        closeTimer->start(g_constantParam.TIMEOUT_UI);

        break;
    case Qt::Key_F2:
        this->pageOneNextClicked();
        closeTimer->start(g_constantParam.TIMEOUT_UI);

        break;
    default:
        closeTimer->start(g_constantParam.TIMEOUT_UI);
        event->ignore();
        break;
    }
}

void UIMenuPos::pageOneNextClicked()
{
    if(stackWidget->currentIndex()==0)
    {
        stackWidget->setCurrentIndex(1);
        lbMenuIndexText->setText("2/2");
    }
}

void UIMenuPos::pageTwoBackClicked()
{
    if(stackWidget->currentIndex()==1)
    {
        stackWidget->setCurrentIndex(0);
        lbMenuIndexText->setText("1/2");
    }
}

void UIMenuPos::styleWidget(QWidget *btn, int iFontSize)
{
    QFont font13;
    if(iFontSize>0)
    {
        font13.setPointSize(iFontSize);
        btn->setFont(font13);
    }
    btn->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    btn->setFixedHeight(35);
}

void UIMenuPos::showTransMenu()
{
    UIMenuTrans *uiTrans=new UIMenuTrans();
    uiTrans->exec();
}

void UIMenuPos::showConfigMenu()
{
    UIMenuConfig *uiCon=new UIMenuConfig();
    uiCon->exec();

}

void UIMenuPos::showCashierManager()
{
    UIUserManager *uiUM=new UIUserManager();
    uiUM->exec();
}

void UIMenuPos::showReport()
{
    UIReport *uiR=new UIReport();
    uiR->exec();

}

void UIMenuPos::showSettle()
{
    qDebug() << Q_FUNC_INFO;
    UISettle *uiStl = new UISettle();
    uiStl->exec();
}

void UIMenuPos::Param_Down_Click()
{
    qDebug()<<Q_FUNC_INFO;
    QSettings *settings=new QSettings("/app/text.ini",QSettings::IniFormat);
    settings->beginGroup("NormalTrans");
    settings->setValue("NormalTrans.param1", 68);
    settings->setValue("NormalTrans.param2", 612);
    settings->setValue("NormalTrans.param3", 68123);
    settings->setValue("NormalTrans.param4", "我了个打草");
    settings->endGroup();

    settings->beginGroup("ExtraTrans");
    settings->setValue("ExtraTrans.param1", 68);
    settings->setValue("ExtraTrans.param2", 612);
    settings->setValue("ExtraTrans.param3", 68123);
    settings->setValue("ExtraTrans.param4", "我了个打草");
    settings->endGroup();

    UIMsg::showNoticeMsgWithAutoClose("Param Download/nComplete.",9000);
}

void UIMenuPos::setAutoClose(int timeout)
{
    qDebug()<<timeout;
    closeTimer= new QTimer(this);
    connect(closeTimer, SIGNAL(timeout()), this, SLOT(slotQuitMenu()));
    closeTimer->start(timeout);
}

void UIMenuPos::slotQuitMenu()
{
    this->close();
}

bool UIMenuPos::eventFilter(QObject *obj, QEvent *event)
{
    if(obj==this)
    {
        if(event->type()==QEvent::WindowActivate)
        {
            qDebug() << Q_FUNC_INFO<<"Start Timer";
            closeTimer->start(g_constantParam.TIMEOUT_UI);
        }
        else if(event->type()==QEvent::WindowDeactivate)
        {
            qDebug() << Q_FUNC_INFO<<"Stop Timer";
            closeTimer->stop();
        }
    }
    return QDialog::eventFilter(obj,event);
}
