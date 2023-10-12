//===- llvm/IR/OptBisect/Bisect.cpp - LLVM Bisect support -----------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
/// \file
/// This file implements support for a bisecting optimizations based on a
/// command line option.
//
//===----------------------------------------------------------------------===//

#include "llvm/IR/OptBisect.h"
#include "llvm/Pass.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/raw_ostream.h"
#include <cassert>

using namespace llvm;

static OptBisect &getOptBisector() {
  static OptBisect OptBisector;
  return OptBisector;
}

static cl::opt<int> OptBisectLimit("opt-bisect-limit", cl::Hidden,
                                   cl::init(OptBisect::Disabled), cl::Optional,
                                   cl::cb<void, int>([](int Limit) {
                                     getOptBisector().setLimit(Limit);
                                   }),
                                   cl::desc("Maximum optimization to perform"));

static void printPassMessage(const StringRef &Name, int PassNum,
                             StringRef TargetDesc, bool Running) {
  StringRef Status = Running ? "" : "NOT ";
  errs() << "BISECT: " << Status << "running pass "
         << "(" << PassNum << ") " << Name << " on " << TargetDesc << "\n";
}

bool OptBisect::shouldRunPass(const StringRef PassName,
                              StringRef IRDescription) {
  assert(isEnabled());

  bool IsKeyFunction = IRDescription == "_RINvMs4_NtNtNtCs8zlSoIwVZ9h_15rustc_mir_build4thir7pattern15deconstruct_patNtB6_11Constructor5splitINtNtNtNtCse4hYLEadXti_4core4iter8adapters3map3MapIB1A_INtNtNtB1I_5slice4iter4IterNtNtB8_10usefulness8PatStackENCNvMs2_B2V_NtB2V_6Matrix5heads0ENvMs7_B6_NtB6_16DeconstructedPat4ctorEEBc_";
  IsKeyFunction = IsKeyFunction || IRDescription == "_RNCINvMs4_NtNtNtCs8zlSoIwVZ9h_15rustc_mir_build4thir7pattern15deconstruct_patNtB8_11Constructor5splitINtNtNtNtCse4hYLEadXti_4core4iter8adapters3map3MapIB1C_INtNtNtB1K_5slice4iter4IterNtNtBa_10usefulness8PatStackENCNvMs2_B2X_NtB2X_6Matrix5heads0ENvMs7_B8_NtB8_16DeconstructedPat4ctorEEs0_0Be_";
  if (!IsKeyFunction)
      return true;
  int CurBisectNum = ++LastBisectNum;
  bool ShouldRun = (BisectLimit == -1 || CurBisectNum <= BisectLimit);
  printPassMessage(PassName, CurBisectNum, IRDescription, ShouldRun);
  return ShouldRun;
}

const int OptBisect::Disabled;

OptPassGate &llvm::getGlobalPassGate() { return getOptBisector(); }
