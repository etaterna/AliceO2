// Copyright CERN and copyright holders of ALICE O2. This software is
// distributed under the terms of the GNU General Public License v3 (GPL
// Version 3), copied verbatim in the file "COPYING".
//
// See http://alice-o2.web.cern.ch/license for full licensing information.
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.
#include "Framework/AnalysisTask.h"
#include "Framework/AnalysisDataModel.h"
#include "AnalysisDataModel/TrackSelectionTables.h"

using namespace o2;
using namespace o2::framework;

namespace o2::aod
{
namespace pidtracks
{
////DECLARE_SOA_COLUMN(EtaEmcal, etaemcal, float);
////DECLARE_SOA_COLUMN(PhiEmcal, phiemcal, float);
//DECLARE_SOA_COLUMN(TPCSignal, tpcsignal, float);
//DECLARE_SOA_COLUMN(TOFSignal, tofsignal, float);
DECLARE_SOA_COLUMN(Px, px, float);
DECLARE_SOA_COLUMN(Py, py, float);
DECLARE_SOA_COLUMN(Pz, pz, float);
DECLARE_SOA_COLUMN(Sign,sign,float);
//DECLARE_SOA_COLUMN(X,x,float);
//DECLARE_SOA_COLUMN(Y,y,float);
//DECLARE_SOA_COLUMN(Z,z,float);
//DECLARE_SOA_COLUMN(Alpha,alpha,float);
//DECLARE_SOA_COLUMN(TrackType,tracktype,uint8_t);
//DECLARE_SOA_COLUMN(TPCNClsShared, tpcNClsShared, uint8_t);
//DECLARE_SOA_COLUMN(DcaXY, dcaXY, float);
//DECLARE_SOA_COLUMN(DcaZ, dcaZ, float);
//DECLARE_SOA_COLUMN(PdgCode, pdgCode, int);
} // namespace pidtracks
DECLARE_SOA_TABLE(PIDTracksMC, "AOD", "PIDTRACKSMC", aod::track::TPCSignal, aod::track::TOFSignal, pidtracks::Px, pidtracks::Py, pidtracks::Pz, pidtracks::Sign, aod::track::X, aod::track::Y, aod::track::Z, aod::track::Alpha, aod::track::TrackType, aod::track::TPCNClsShared, aod::track::DcaXY, aod::track::DcaZ, aod::mcparticle::PdgCode);
DECLARE_SOA_TABLE(PIDTracksReal, "AOD", "PIDTRACKSREAL", aod::track::TPCSignal, aod::track::TOFSignal, pidtracks::Px, pidtracks::Py, pidtracks::Pz, pidtracks::Sign, aod::track::X, aod::track::Y, aod::track::Z, aod::track::Alpha, aod::track::TrackType, aod::track::TPCNClsShared, aod::track::DcaXY, aod::track::DcaZ);
} //namespace o2::aod

void customize(std::vector<o2::framework::ConfigParamSpec>& workflowOptions)
{
  ConfigParamSpec optionDoMC{"doMC", VariantType::Bool, false, {"Fill PID train table with MC data."}};
  workflowOptions.push_back(optionDoMC);
}

#include "Framework/runDataProcessing.h"

struct CreateTableMC {
  using BigTracksMC = soa::Join<aod::FullTracks, aod::TracksExtended, aod::McTrackLabels>;

  Produces<aod::PIDTracksMC> pidTracksTable;
  void process(BigTracksMC const& tracks, aod::McParticles const& mctracks)
  {
    for (const auto& track : tracks) {
      const auto mcParticle = track.mcParticle();
      pidTracksTable(track.tpcSignal(), track.tofSignal(), track.px(), track.py(), track.pz(), track.sign(), track.x(), track.y(), track.z(), track.alpha(), track.trackType(), track.tpcNClsShared(), track.dcaXY(), track.dcaZ(), mcParticle.pdgCode());
    }
  }
};

struct CreateTableReal {
  using BigTracks = soa::Join<aod::FullTracks, aod::TracksExtended>;

  Produces<aod::PIDTracksReal> pidTracksTable;
  void process(BigTracks const& tracks)
  {
    for (const auto& track : tracks) {
      pidTracksTable(track.tpcSignal(), track.tofSignal(), track.px(), track.py(), track.pz(), track.sign(), track.x(), track.y(), track.z(), track.alpha(), track.trackType(), track.tpcNClsShared(), track.dcaXY(), track.dcaZ());
    }
  }
};

WorkflowSpec defineDataProcessing(ConfigContext const& cfgc)
{
  const bool doMC = cfgc.options().get<bool>("doMC");
  if (doMC) {
    return WorkflowSpec{
      adaptAnalysisTask<CreateTableMC>(cfgc)};
  } else {
    return WorkflowSpec{
      adaptAnalysisTask<CreateTableReal>(cfgc)};
  }
}
