# Copyright 2019-2020 CERN and copyright holders of ALICE O2.
# See https://alice-o2.web.cern.ch/copyright for details of the copyright holders.
# All rights not expressly granted are reserved.
#
# This software is distributed under the terms of the GNU General Public
# License v3 (GPL Version 3), copied verbatim in the file "COPYING".
#
# In applying this license CERN does not waive the privileges and immunities
# granted to it by virtue of its status as an Intergovernmental Organization
# or submit itself to any jurisdiction.

o2_add_library(FV0Reconstruction
        SOURCES src/BaseRecoTask.cxx
                src/ReadRaw.cxx
                src/CTFCoder.cxx

        PUBLIC_LINK_LIBRARIES O2::SimulationDataFormat
                              O2::Framework
                              O2::FV0Base
                              O2::FV0Simulation
                              O2::FV0Calibration
                              O2::DataFormatsFV0
                              O2::DetectorsRaw
                              O2::CommonDataFormat
                              O2::rANS
                              O2::Headers)

o2_target_root_dictionary(FV0Reconstruction
        HEADERS include/FV0Reconstruction/BaseRecoTask.h
                include/FV0Reconstruction/ReadRaw.h
                include/FV0Reconstruction/CTFCoder.h)

o2_add_executable(
  test-raw2digit
  COMPONENT_NAME fv0
  SOURCES src/test-raw2digit.cxx
  PUBLIC_LINK_LIBRARIES O2::FV0Reconstruction)
