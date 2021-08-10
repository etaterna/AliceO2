// Copyright CERN and copyright holders of ALICE O2. This software is
// distributed under the terms of the GNU General Public License v3 (GPL
// Version 3), copied verbatim in the file "COPYING".
//
// See http://alice-o2.web.cern.ch/license for full licensing information.
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.
#include "Framework/runDataProcessing.h"
#include "Framework/AnalysisTask.h"
#include "Framework/AnalysisDataModel.h"

using namespace o2;
using namespace o2::framework;

namespace o2::aod
{
namespace pidtracks
{
//DECLARE_SOA_COLUMN(EtaEmcal, etaemcal, float);
//DECLARE_SOA_COLUMN(PhiEmcal, phiemcal, float);
DECLARE_SOA_COLUMN(TPCSignal, tpcsignal, float);
DECLARE_SOA_COLUMN(TOFSignal, tofsignal, float);
DECLARE_SOA_COLUMN(Px, px, float);
DECLARE_SOA_COLUMN(Py, py, float);
DECLARE_SOA_COLUMN(Pz, pz, float);
DECLARE_SOA_COLUMN(PDGCode, pdgcode, float);
} // namespace pidtracks
DECLARE_SOA_TABLE(PIDTracks, "AOD", "PIDTRACKS", pidtracks::EtaEmcal, pidtracks::PhiEmcal, pidtracks::TPCSignal, pidtracks::TOFSignal, pidtracks::P, pidtracks::PDGCode);
} //namespace o2::aod

struct CreateTrainingTable {
  using BigTracksMC = soa::Join<aod::FullTracks, aod::McTrackLabels>;

  Produces<aod::PIDTracks> pidTracksTable;
  void process(BigTracksMC const& tracks, aod::McParticles const& mctracks)
  {

    for (const auto& track : tracks) {
      const auto mcParticle = track.mcParticle();
      pidTracksTable(track.trackEtaEmcal(), track.trackPhiEmcal(), track.tpcSignal(), track.tofSignal(), track.px(), track.py(), track.pz(), mcParticle.pdgCode());
    }
  }
};

WorkflowSpec defineDataProcessing(ConfigContext const& cfgc)
{
  return WorkflowSpec{
    adaptAnalysisTask<CreateTrainingTable>(cfgc)};
}
