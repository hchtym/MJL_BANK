#include "uiReportSearch.h"
#include "transData.h"
#include "xdata.h"
#include "commontools.h"
#include "global.h"

UIReportSearch::UIReportSearch(QDialog *parent,Qt::WindowFlags f) :
    QDialog(parent,f)
{
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
    lbHead->setText(tr("Search Transaction"));
    lbHead->setFont(fontH);
    lbHead->setAlignment(Qt::AlignCenter);
    lbHead->setMinimumHeight(40);
    lbHead->setMaximumHeight(40);
    lbHead->setStyleSheet(HEAD_STYLE);
    lbHead->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    //--------------define--------------------//
    lbSearchNotice=new QLabel();
    leSearch=new QLineEdit();

    lbSearchNotice->setText("Please Input the TRACE No. :");
    lbSearchNotice->setAlignment(Qt::AlignCenter);
    lbSearchNotice->setFont(font);

    leSearch->setAlignment(Qt::AlignCenter);
    leSearch->setMaxLength(TRANS_AUTHCODE_LEN);
    leSearch->setFont(font);

    const QString  REGEX_AMOUNT = "^[0-9]\\d*$";
    QRegExp regx(REGEX_AMOUNT);
    QValidator *validator1 = new QRegExpValidator(regx, leSearch);
    leSearch->setValidator(validator1);

    leSearch->setStyleSheet("border: 3px solid silver;border-radius: 6px;");
    leSearch->setMinimumHeight(35);
    leSearch->setMaxLength(10);
    //    ----------------------------------  //
    btnCancel=new QPushButton;
    btnSubmit=new QPushButton;
    btnCancel->setText(tr("Cancel"));
    btnSubmit->setText(tr("Search"));
    btnCancel->setFont(font2);
    btnSubmit->setFont(font2);
    btnCancel->setMinimumHeight(30);
    btnSubmit->setMinimumHeight(30);
    btnCancel->setStyleSheet(BTN_CANCEL_STYLE);
    btnSubmit->setStyleSheet(BTN_SUBMIT_STYLE);

    QSpacerItem *sp1=new QSpacerItem(1,1,QSizePolicy::Expanding,QSizePolicy::Expanding);
    QSpacerItem *sp2=new QSpacerItem(1,1,QSizePolicy::Expanding,QSizePolicy::Expanding);

    QVBoxLayout *v1Lay=new QVBoxLayout();
    v1Lay->addItem(sp1);
    v1Lay->addWidget(lbSearchNotice);
    v1Lay->addWidget(leSearch);
    v1Lay->addItem(sp2);

    QHBoxLayout *h2Lay=new QHBoxLayout();
    //    h2Lay->addSpacing(10);
    h2Lay->addWidget(btnCancel);
    h2Lay->addWidget(btnSubmit);
    //    h2Lay->addSpacing(10);

    QVBoxLayout *layout=new QVBoxLayout(this);
    layout->addWidget(lbHead);
    layout->addLayout(v1Lay);
    layout->addLayout(h2Lay);


    connect(btnSubmit, SIGNAL(clicked()), this, SLOT(slotSearchTransaction()) );
    connect(btnCancel, SIGNAL(clicked()), this, SLOT(close()));

    //Animation
    QPropertyAnimation *animation1 = new QPropertyAnimation(this, "pos");
    animation1->setDuration(100);
    animation1->setStartValue(mapToParent(QPoint(FRAME420_WIDTH, 0)));
    animation1->setEndValue(mapToParent(QPoint(0, 0)));
    animation1->setEasingCurve(QEasingCurve::OutQuint);
    animation1->start();
}

UIReportSearch::~UIReportSearch()
{
    qDebug() <<"delete:: "<< Q_FUNC_INFO;
}

void UIReportSearch::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_Escape:
        this->close();
        break;
    case Qt::Key_Enter:
        this->slotSearchTransaction();
        break;
    default:
        event->ignore();
        break;
    }
}

void UIReportSearch::slotSearchTransaction()
{
    qDebug()<<Q_FUNC_INFO;

    if(leSearch->text().isEmpty())
    {
        qDebug()<<"fill in the blank";

        UIMsg::showNoticeMsg(INCOMPLETE_INFORMATION);
        return;
    }

    QString transType;
    QString cardNo;
    QString amount;
    QString refNo;
    QString apprNo;
    QString operatorNo;

    ulong inputTraceNo=leSearch->text().toULong();
    for(int index = 0; index < ((int)g_constantParam.uiMaxTotalNb); index++)
    {
        qDebug()<<"index:: "<<index;
        if(g_transInfo.auiTransIndex[index])
        {
            //:- 读取数据保存到NormalTransData
            memset(&NormalTransData,0,sizeof(NORMAL_TRANS));
            int ucResult=xDATA::ReadSubsectionFile(xDATA::DataSaveSaveTrans, index);
            if(ucResult!=0)
            {
                UIMsg::showFileErrMsg((FileErrIndex)ucResult);

                return;
            }
            memcpy(&NormalTransData,&g_saveTrans,sizeof(NORMAL_TRANS));

            // 授权号 NormalTransData.aucAuthCode;
            if(inputTraceNo==NormalTransData.ulTraceNumber)
            {
                qDebug()<<inputTraceNo<<"Matched";
                switch(NormalTransData.transType)
                {
                case TransMode_CashDeposit:      //存钱
                    transType="Cash Deposit";
                    break;
                case TransMode_CashAdvance:      //取钱
                    transType="Cash Advance";
                    break;
                case TransMode_CashVoid:         //撤销
                    transType="Cash VOID";
                    break;
                case TransMode_BalanceInquiry:   //查余
                    transType="Balance Inquiry";
                    break;
                case TransMode_CardTransfer:     //转账
                    transType="P2P Transfer";
                    break;
                default:
                    qDebug()<<"This should not be entered";
                    break;
                }
                qDebug()<<"step2"<<transType;

                //Card No
                cardNo=QString::fromAscii((const char *)NormalTransData.aucSourceAcc);  // 需要部分隐藏
                qDebug()<<"card no:"<<cardNo;

                // Amount
                unsigned char aucBuf[12];
                getAmount(aucBuf, NormalTransData.ulAmount, 2);
                amount=QString::fromAscii((const char *)aucBuf);
                qDebug()<<"amount:"<<amount;

                // ref & appr
                refNo=QString::fromAscii((const char *)NormalTransData.aucRefNum);
                apprNo=QString::fromAscii((const char *)NormalTransData.aucAuthCode);
                qDebug()<<"ref & appr:"<<refNo<<apprNo;

                //operator
                operatorNo=QString::fromAscii((const char *)NormalTransData.aucCashier);

                uiRDetail=new UIReportDetail();
                uiRDetail->slotSetDetailList(transType,cardNo,amount,refNo,apprNo,operatorNo);
                uiRDetail->exec();

                break;
            }
        }
        else
        {
            UIMsg::showErrMsg("No Match Transaction");
            break;
    }
    }
}
