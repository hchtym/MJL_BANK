#include "uiReportSummary.h"
#include "transData.h"
#include "xdata.h"
#include "sav.h"
#include "commontools.h"
#include "global.h"

extern "C"{
    void Uart_Printf(char *fmt,...);
}

UIReportSummary::UIReportSummary(QDialog *parent,Qt::WindowFlags f) :
    QDialog(parent,f)
{
    QObject::installEventFilter(this);
    RemoveKeyEventBug();

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
    QFont font2("Helvetica",14,QFont::Bold);
    QFont font3("Helvetica",14);

    this->setStyleSheet("QDialog{border: 3px solid silver;}");

    lbHead=new QLabel();
    QFont fontH("Helvetica",18,QFont::Bold);
    lbHead->setText(tr("Summary"));
    lbHead->setFont(fontH);
    lbHead->setAlignment(Qt::AlignCenter);
    lbHead->setMinimumHeight(40);
    lbHead->setMaximumHeight(40);
    lbHead->setStyleSheet(HEAD_STYLE);
    lbHead->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    //--------------define--------------------//
    tbSummary=new QTableWidget();

    //    ----------------------------------  //
    btnCancel=new QPushButton;
    btnCancel->setText(tr("Cancel"));
    btnCancel->setFont(font2);
    btnCancel->setMinimumHeight(30);
    btnCancel->setStyleSheet(BTN_GREY_STYLE);

    QVBoxLayout *v1Lay=new QVBoxLayout();
    v1Lay->addWidget(tbSummary);

    QHBoxLayout *h2Lay=new QHBoxLayout();
    //    h2Lay->addSpacing(10);
    h2Lay->addWidget(btnCancel);
    //    h2Lay->addSpacing(10);

    QVBoxLayout *layout=new QVBoxLayout(this);
    layout->addWidget(lbHead);
    layout->addLayout(v1Lay);
    layout->addLayout(h2Lay);

    connect(btnCancel, SIGNAL(clicked()), this, SLOT(close()));
    connect(tbSummary,SIGNAL(viewportEntered()),this,SLOT(restartTimeOut()));
    connect(tbSummary,SIGNAL(clicked(QModelIndex)),this,SLOT(restartTimeOut()));
    connect(tbSummary->verticalScrollBar(),SIGNAL(sliderMoved(int)),this,SLOT(restartTimeOut()));
    //Animation
    QPropertyAnimation *animation1 = new QPropertyAnimation(this, "pos");
    animation1->setDuration(100);
    animation1->setStartValue(mapToParent(QPoint(FRAME420_WIDTH, 0)));
    animation1->setEndValue(mapToParent(QPoint(0, 0)));
    animation1->setEasingCurve(QEasingCurve::OutQuint);
    animation1->start();

//    setTestInfo(); // 设置测试数据
    this->setTransTotal();
    this->setAutoClose(g_constantParam.TIMEOUT_UI);
}

UIReportSummary::~UIReportSummary()
{
    qDebug() <<"delete:: "<< Q_FUNC_INFO;
}



void UIReportSummary::setTransTotal()
{
    qDebug()<<Q_FUNC_INFO;
    TRANSTOTAL			*pTransTotal = NULL;
    xDATA::ReadValidFile(xDATA::DataSaveTransInfo);
    //:- 监控统计信息
    pTransTotal = &g_transInfo.TransTotal;	//:- 赋值pTransTotal
//    Uart_Printf("\n--[交易统计]--\n");
//    Uart_Printf("交易总笔数:%d\n\n",pTransTotal->uiTotalNb);
//    Uart_Printf("存款总笔数:%d\n",pTransTotal->uiDepositNb);
//    Uart_Printf("存款总金额:%d\n\n",pTransTotal->ulDepositAmount);
//    Uart_Printf("取款总笔数:%d\n",pTransTotal->uiAdvanceNb);
//    Uart_Printf("取款总金额:%d\n\n",pTransTotal->ulAdvanceAmount);
//    Uart_Printf("转账总笔数:%d\n",pTransTotal->uiTransferNb);
//    Uart_Printf("转账总金额:%d\n\n",pTransTotal->ulTransferAmount);
//    Uart_Printf("撤销总笔数:%d\n",pTransTotal->uiVoidNb);
//    Uart_Printf("撤销总金额:%d\n",pTransTotal->ulVoidAmount);
//    Uart_Printf("-------------\n");

    tbSummary->setRowCount(14);
    tbSummary->setColumnCount(2);
    tbSummary->setFrameShape(QFrame::NoFrame);
    tbSummary->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tbSummary->setSelectionMode(QAbstractItemView::SingleSelection);
    tbSummary->setSelectionBehavior(QAbstractItemView::SelectRows);
    tbSummary->horizontalHeader()->setVisible(false);
    tbSummary->horizontalHeader()->setDefaultSectionSize(110);
    tbSummary->horizontalHeader()->setStretchLastSection(true);
    tbSummary->verticalHeader()->setVisible(false);

    tbSummary->verticalScrollBar()->setStyleSheet(SCROLL_VERTICAL_STYLE);
//    tbSummary->verticalHeader()->setStretchLastSection(true);

//    QTableWidgetItem *itmCol1=new QTableWidgetItem();
//    itmCol1->setText("Catalog");
//    QTableWidgetItem *itmCol2=new QTableWidgetItem();
//    itmCol2->setText("Data");

//    tbSummary->setHorizontalHeaderItem(0,itmCol1);
//    tbSummary->setHorizontalHeaderItem(1,itmCol2);

    // --------------------- item------------- //
    QFont fontTitle("Helvetica",14,QFont::Bold);

    QTableWidgetItem *itmTrans=new QTableWidgetItem();
    itmTrans->setText("All Transaction:");
    itmTrans->setFont(fontTitle);
    QTableWidgetItem *itmAdvance=new QTableWidgetItem();
    itmAdvance->setText("Cash Advance:");
    itmAdvance->setFont(fontTitle);
    QTableWidgetItem *itmDeposit=new QTableWidgetItem();
    itmDeposit->setText("Cash Deposit:");
    itmDeposit->setFont(fontTitle);
    QTableWidgetItem *itmTransfer=new QTableWidgetItem();
    itmTransfer->setText("P2P Transfer:");
    itmTransfer->setFont(fontTitle);
    QTableWidgetItem *itmVOID=new QTableWidgetItem();
    itmVOID->setText("VOID:");
    itmVOID->setFont(fontTitle);

    QTableWidgetItem *itmTransNumTitle=new QTableWidgetItem();
    itmTransNumTitle->setText("Total:");
    QTableWidgetItem *itmAdvanceNumTitle=new QTableWidgetItem();
    itmAdvanceNumTitle->setText("Total:");
    QTableWidgetItem *itmDepositNumTitle=new QTableWidgetItem();
    itmDepositNumTitle->setText("Total:");
    QTableWidgetItem *itmTransferNumTitle=new QTableWidgetItem();
    itmTransferNumTitle->setText("Total:");
    QTableWidgetItem *itmVOIDNumTitle=new QTableWidgetItem();
    itmVOIDNumTitle->setText("Total:");

    QTableWidgetItem *itmAdvanceAmountTitle=new QTableWidgetItem();
    itmAdvanceAmountTitle->setText("Amount:");
    QTableWidgetItem *itmDepositAmountTitle=new QTableWidgetItem();
    itmDepositAmountTitle->setText("Amount:");
    QTableWidgetItem *itmTransferAmountTitle=new QTableWidgetItem();
    itmTransferAmountTitle->setText("Amount:");
    QTableWidgetItem *itmVOIDAmountTitle=new QTableWidgetItem();
    itmVOIDAmountTitle->setText("Amount:");

    // ----------------------  DATA--------------------- ///
    QTableWidgetItem *itmTransNum=new QTableWidgetItem();
    itmTransNum->setText(QString::number(pTransTotal->uiTotalNb));
    itmTransNum->setTextAlignment(Qt::AlignHCenter);
    QTableWidgetItem *itmAdvanceNum=new QTableWidgetItem();
    itmAdvanceNum->setText(QString::number(pTransTotal->total[TotalAdvance].uiNb));
    itmAdvanceNum->setTextAlignment(Qt::AlignHCenter);
    QTableWidgetItem *itmDepositNum=new QTableWidgetItem();
    itmDepositNum->setText(QString::number(pTransTotal->total[TotalDeposit].uiNb));
    itmDepositNum->setTextAlignment(Qt::AlignHCenter);
    QTableWidgetItem *itmTransferNum=new QTableWidgetItem();
    itmTransferNum->setText(QString::number(pTransTotal->total[TotalTransfer].uiNb));
    itmTransferNum->setTextAlignment(Qt::AlignHCenter);
    QTableWidgetItem *itmVOIDNum=new QTableWidgetItem();
    itmVOIDNum->setText(QString::number(pTransTotal->total[TotalVoid].uiNb));
    itmVOIDNum->setTextAlignment(Qt::AlignHCenter);

    QTableWidgetItem *itmAdvanceAmount=new QTableWidgetItem();
    unsigned char aucBuf[12];
    memset(aucBuf,0,sizeof(aucBuf));
    getAmount(aucBuf, pTransTotal->total[TotalAdvance].ulAmount, 2);
    itmAdvanceAmount->setText(QString::fromAscii((const char *)aucBuf));

    QTableWidgetItem *itmDepositAmount=new QTableWidgetItem();
    memset(aucBuf,0,sizeof(aucBuf));
    getAmount(aucBuf, pTransTotal->total[TotalDeposit].ulAmount, 2);
    itmDepositAmount->setText(QString::fromAscii((const char *)aucBuf));

    QTableWidgetItem *itmTransferAmount=new QTableWidgetItem();
    memset(aucBuf,0,sizeof(aucBuf));
    getAmount(aucBuf, pTransTotal->total[TotalTransfer].ulAmount, 2);
    itmTransferAmount->setText(QString::fromAscii((const char *)aucBuf));

    QTableWidgetItem *itmVOIDAmount=new QTableWidgetItem();
    memset(aucBuf,0,sizeof(aucBuf));
    getAmount(aucBuf, pTransTotal->total[TotalVoid].ulAmount, 2);
    itmVOIDAmount->setText(QString::fromAscii((const char *)aucBuf));


    // ----------------------setItem----------------------- //
     //合并单元格的效果
    //     ui.qtablewidget->setSpan(2, 2, 3, 2);
     //第一个参数：要改变的单元格行数
     //第二个参数：要改变的单元格列数
     //第三个参数：需要合并的行数
     //第四个参数：需要合并的列数
    tbSummary->setItem(0,0,itmTrans);
    tbSummary->setItem(1,0,itmTransNumTitle);
    tbSummary->setItem(1,1,itmTransNum);
    tbSummary->setSpan(0,0,1,2);

    tbSummary->setItem(2,0,itmAdvance);
    tbSummary->setSpan(2,0,1,2);
    tbSummary->setItem(3,0,itmAdvanceNumTitle);
    tbSummary->setItem(3,1,itmAdvanceNum);
    tbSummary->setItem(4,0,itmAdvanceAmountTitle);
    tbSummary->setItem(4,1,itmAdvanceAmount);

    tbSummary->setItem(5,0,itmDeposit);
    tbSummary->setSpan(5,0,1,2);
    tbSummary->setItem(6,0,itmDepositNumTitle);
    tbSummary->setItem(6,1,itmDepositNum);
    tbSummary->setItem(7,0,itmDepositAmountTitle);
    tbSummary->setItem(7,1,itmDepositAmount);

    tbSummary->setItem(8,0,itmTransfer);
    tbSummary->setSpan(8,0,1,2);
    tbSummary->setItem(9,0,itmTransferNumTitle);
    tbSummary->setItem(9,1,itmTransferNum);
    tbSummary->setItem(10,0,itmTransferAmountTitle);
    tbSummary->setItem(10,1,itmTransferAmount);

    tbSummary->setItem(11,0,itmVOID);
    tbSummary->setSpan(11,0,1,2);
    tbSummary->setItem(12,0,itmVOIDNumTitle);
    tbSummary->setItem(12,1,itmVOIDNum);
    tbSummary->setItem(13,0,itmVOIDAmountTitle);
    tbSummary->setItem(13,1,itmVOIDAmount);
}

void UIReportSummary::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_Escape:
        this->close();
        break;
    case Qt::Key_Enter:
        break;
    default:
        closeTimer->start(g_constantParam.TIMEOUT_UI);
        event->ignore();
        break;
    }
}

void UIReportSummary::setAutoClose(int timeout)
{
    qDebug()<<timeout;
    closeTimer= new QTimer(this);
    connect(closeTimer, SIGNAL(timeout()), this, SLOT(slotQuitMenu()));
    closeTimer->start(timeout);
}

void UIReportSummary::slotQuitMenu()
{
    UIMsg::showNoticeMsgWithAutoCloseNoBeep("TIME OUT",g_constantParam.TIMEOUT_ERRMSG);
    this->close();
}

bool UIReportSummary::eventFilter(QObject *obj, QEvent *event)
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

void UIReportSummary::restartTimeOut()
{
    qDebug()<<Q_FUNC_INFO;
    closeTimer->start(g_constantParam.TIMEOUT_UI);
}

