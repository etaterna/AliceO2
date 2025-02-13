// Copyright 2019-2020 CERN and copyright holders of ALICE O2.
// See https://alice-o2.web.cern.ch/copyright for details of the copyright holders.
// All rights not expressly granted are reserved.
//
// This software is distributed under the terms of the GNU General Public
// License v3 (GPL Version 3), copied verbatim in the file "COPYING".
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.

//root includes
#include "TH1.h"
#include "TH2.h"
#include "TFile.h"

//o2 includes
#include "TPCQC/Clusters.h"
#include "TPCBase/Painter.h"
#include "TPCBase/ROC.h"
#include "TPCBase/CRU.h"
#include "TPCBase/Mapper.h"
#include "DataFormatsTPC/ClusterNative.h"
#include "DataFormatsTPC/KrCluster.h"

ClassImp(o2::tpc::qc::Clusters);

using namespace o2::tpc::qc;

//______________________________________________________________________________
template <class T>
bool Clusters::processCluster(const T& cluster, const o2::tpc::Sector sector, const int row)
{
  const int nROC = row < 63 ? int(sector) : int(sector) + 36;
  const int rocRow = row < 63 ? row : row - 63;

  const float pad = cluster.getPad();

  const auto qMax = cluster.getQmax();
  const auto qTot = cluster.getQtot();
  const auto sigmaPad = cluster.getSigmaPad();
  const auto sigmaTime = cluster.getSigmaTime();
  const auto timeBin = cluster.getTime();

  float count = mNClusters.getCalArray(nROC).getValue(rocRow, pad);
  mNClusters.getCalArray(nROC).setValue(rocRow, pad, count + 1);

  float charge = mQMax.getCalArray(nROC).getValue(rocRow, pad);
  mQMax.getCalArray(nROC).setValue(rocRow, pad, charge + qMax);

  charge = mQTot.getCalArray(nROC).getValue(rocRow, pad);
  mQTot.getCalArray(nROC).setValue(rocRow, pad, charge + qTot);

  count = mSigmaTime.getCalArray(nROC).getValue(rocRow, pad);
  mSigmaTime.getCalArray(nROC).setValue(rocRow, pad, count + sigmaTime);

  count = mSigmaPad.getCalArray(nROC).getValue(rocRow, pad);
  mSigmaPad.getCalArray(nROC).setValue(rocRow, pad, count + sigmaPad);

  count = mTimeBin.getCalArray(nROC).getValue(rocRow, pad);
  mTimeBin.getCalArray(nROC).setValue(rocRow, pad, count + timeBin);

  return true;
}

//______________________________________________________________________________
void Clusters::fillADCValue(int cru, int rowInSector, int padInRow, int timeBin, float adcValue)
{
  const CRU cruID(cru);
  float val;
  val = mNClusters.getValue(cruID.sector(), rowInSector, padInRow);
  mNClusters.setValue(cruID.sector(), rowInSector, padInRow, val + 1);

  val = mQMax.getValue(cruID.sector(), rowInSector, padInRow);
  mQMax.setValue(cruID.sector(), rowInSector, padInRow, val + adcValue);

  val = mTimeBin.getValue(cruID.sector(), rowInSector, padInRow);
  mTimeBin.setValue(cruID.sector(), rowInSector, padInRow, val + timeBin);
}

//______________________________________________________________________________
void Clusters::analyse()
{
  mQMax /= mNClusters;
  mQTot /= mNClusters;
  mSigmaTime /= mNClusters;
  mSigmaPad /= mNClusters;
  mTimeBin /= mNClusters;
}

//______________________________________________________________________________
void Clusters::dumpToFile(std::string filename)
{
  if (filename.find(".root") != std::string::npos) {
    filename.resize(filename.size() - 5);
  }

  std::string canvasFile = filename + "_canvas.root";
  auto f = std::unique_ptr<TFile>(TFile::Open(canvasFile.c_str(), "recreate"));
  f->WriteObject(o2::tpc::painter::draw(mNClusters), mNClusters.getName().data());
  f->WriteObject(o2::tpc::painter::draw(mQMax), mQMax.getName().data());
  f->WriteObject(o2::tpc::painter::draw(mQTot), mQTot.getName().data());
  f->WriteObject(o2::tpc::painter::draw(mSigmaTime), mSigmaTime.getName().data());
  f->WriteObject(o2::tpc::painter::draw(mSigmaPad), mSigmaPad.getName().data());
  f->WriteObject(o2::tpc::painter::draw(mTimeBin), mTimeBin.getName().data());
  f->Close();

  std::string calPadFile = filename + ".root";
  auto g = std::unique_ptr<TFile>(TFile::Open(calPadFile.c_str(), "recreate"));
  g->WriteObject(&mNClusters, mNClusters.getName().data());
  g->WriteObject(&mQMax, mQMax.getName().data());
  g->WriteObject(&mQTot, mQTot.getName().data());
  g->WriteObject(&mSigmaTime, mSigmaTime.getName().data());
  g->WriteObject(&mSigmaPad, mSigmaPad.getName().data());
  g->WriteObject(&mTimeBin, mTimeBin.getName().data());
  g->Close();
}

// ===| explicit instantiations |===============================================
template bool Clusters::processCluster<o2::tpc::ClusterNative>(const o2::tpc::ClusterNative&, const o2::tpc::Sector, const int);
template bool Clusters::processCluster<o2::tpc::KrCluster>(const o2::tpc::KrCluster&, const o2::tpc::Sector, const int);
