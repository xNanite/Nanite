#include "statisticspage.h"
#include "ui_statisticspage.h"
#include "main.h"
#include "wallet.h"
#include "init.h"
#include "base58.h"
#include "clientmodel.h"
#include "bitcoinrpc.h"
#include "poolbrowser.h"
#include <sstream>
#include <string>

using namespace json_spirit;

StatisticsPage::StatisticsPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StatisticsPage) {
    ui->setupUi(this);

    setFixedSize(400, 420);

    connect(ui->startButton, SIGNAL(pressed()), this, SLOT(updateStatistics()));
}

int heightPrevious = -1;
int connectionPrevious = -1;
int volumePrevious = -1;
double rewardPrevious = -1;
double netPawratePrevious = -1;
double pawratePrevious = -1;
double hardnessPrevious = -1;
double hardnessPrevious2 = -1;
int stakeminPrevious = -1;
int stakemaxPrevious = -1;
int64_t marketcapPrevious = -1;
QString stakecPrevious = "";


void StatisticsPage::updateStatistics() {
    double pHardness = GetDifficulty();
    double pHardness2 = GetDifficulty(GetLastBlockIndex(pindexBest, true));
    int pPawrate = GetPoWMHashPS();
    double pPawrate2 = 0.000;
    int nHeight = pindexBest->nHeight;
    double nSubsidy = 2500;
    uint64_t nMinWeight = 0, nMaxWeight = 0, nWeight = 0;
    pwalletMain->GetStakeWeight(*pwalletMain, nMinWeight, nMaxWeight, nWeight);
    uint64_t nNetworkWeight = GetPoSKernelPS();
    int64_t volume = ((pindexBest->nMoneySupply) / 100000000);
    int64_t marketcap = _dScPriceLast * volume;
    int peers = this->model->getNumConnections();
    pPawrate2 = (double)pPawrate;
    ui->progressBar->setValue(nHeight);
    QString height = QString::number(nHeight);
    QString stakemin = QString::number(nMinWeight);
    QString stakemax = QString::number(nNetworkWeight);
    QString phase = "";

    if (pindexBest->nHeight < 14400) {
        phase = "<p align=\"center\">Time till Mining ends:</p>";
        ui->progressBar->setMinimum(0);
        ui->progressBar->setMaximum(14400);
    } else {
        ui->progressBar->hide();
        phase = "<p align=\"center\">Mining Phase Complete</p>";
    }

    QString subsidy = QString::number(nSubsidy, 'f', 6);
    QString hardness = QString::number(pHardness, 'f', 6);
    QString hardness2 = QString::number(pHardness2, 'f', 6);
    QString pawrate = QString::number(pPawrate2, 'f', 3);
    QString Qlpawrate = model->getLastBlockDate().toString();

    QString QPeers = QString::number(peers);
    QString qVolume = QLocale(QLocale::English).toString((qlonglong)volume);

    if (nHeight > heightPrevious) {
        ui->progressBar->setValue(nHeight);
        ui->heightBox->setText("<font color=\"green\">" + height + "</font>");
    } else {
        ui->heightBox->setText(height);
    }

    if (0 > stakeminPrevious) {
        ui->minBox->setText("<font color=\"green\">" + stakemin + "</font>");
    } else {
        ui->minBox->setText(stakemin);
    }

    if (0 > stakemaxPrevious) {
        ui->maxBox->setText("<font color=\"green\">" + stakemax + "</font>");
    } else {
        ui->maxBox->setText(stakemax);
    }

    if (phase != stakecPrevious) {
        ui->cBox->setText("<font color=\"white\">" + phase + "</font>");
    } else {
        ui->cBox->setText(phase);
    }

    if (marketcap > marketcapPrevious) {
        ui->marketcap->setText("<font color=\"green\">" + QString::number(marketcap) + " $</font>");
    } else if (marketcap < marketcapPrevious) {
        ui->marketcap->setText("<font color=\"white\">" + QString::number(marketcap) + " $</font>");
    } else {
        ui->marketcap->setText(QString::number(marketcap) + " $");
    }

    if (pHardness2 > hardnessPrevious2) {
        ui->diffBox2->setText("<font color=\"green\">" + hardness2 + "</font>");
    } else if (pHardness2 < hardnessPrevious2) {
        ui->diffBox2->setText("<font color=\"white\">" + hardness2 + "</font>");
    } else {
        ui->diffBox2->setText(hardness2);
    }

    if (Qlpawrate != pawratePrevious) {
        ui->localBox->setText("<font color=\"white\">" + Qlpawrate + "</font>");
    } else {
        ui->localBox->setText(Qlpawrate);
    }

    if (peers > connectionPrevious) {
        ui->connectionBox->setText("<font color=\"green\">" + QPeers + "</font>");
    } else if (peers < connectionPrevious) {
        ui->connectionBox->setText("<font color=\"white\">" + QPeers + "</font>");
    } else {
        ui->connectionBox->setText(QPeers);
    }

    if (volume > volumePrevious) {
        ui->volumeBox->setText("<font color=\"green\">" + qVolume + " XNAN" + "</font>");
    } else if (volume < volumePrevious) {
        ui->volumeBox->setText("<font color=\"white\">" + qVolume + " XNAN" + "</font>");
    } else {
        ui->volumeBox->setText(qVolume + " XNAN");
    }

    updatePrevious(nHeight, nMinWeight, nNetworkWeight, phase, nSubsidy, pHardness, pHardness2, pPawrate2, Qlpawrate, peers, volume, marketcap);
}

void StatisticsPage::updatePrevious(int nHeight, int nMinWeight, int nNetworkWeight, QString phase, double nSubsidy, double pHardness, double pHardness2, double pPawrate2, QString Qlpawrate, int peers, int volume, int64_t marketcap) {
    heightPrevious = nHeight;
    stakeminPrevious = nMinWeight;
    stakemaxPrevious = nNetworkWeight;
    stakecPrevious = phase;
    rewardPrevious = nSubsidy;
    hardnessPrevious = pHardness;
    hardnessPrevious2 = pHardness2;
    netPawratePrevious = pPawrate2;
    pawratePrevious = Qlpawrate;
    connectionPrevious = peers;
    volumePrevious = volume;
    marketcapPrevious = marketcap;
}

void StatisticsPage::setModel(ClientModel *model) {
    updateStatistics();
    this->model = model;
}


StatisticsPage::~StatisticsPage() {
    delete ui;
}
