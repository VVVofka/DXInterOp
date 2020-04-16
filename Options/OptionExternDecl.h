#pragma once
int openDlgOptions(int* iArr); // decl Options.lib
namespace ReturnOptions{
static const int Restart = 1;
}; // *************************************************************

namespace InpOptions{
static const int LaysCntReserv = 20; 
// offset in iArr;
static const int Dirs = 0;
static const int NormDir = Dirs + 16 * 4 * 4;

static const int LaysSzUpY = NormDir + 1;
static const int LaysSzUpX = LaysSzUpY + 1;
static const int LaysSzDn = LaysSzUpX + 1;

static const int SeedRnd = LaysSzDn + 1;
static const int LaysCnt = SeedRnd + 1;
static const int AMasks = LaysCnt + 1;
              // next = AMasks + 16

// offset in dArr;
static const int kFillRnd = 0;
static const int kSigmaY = kFillRnd + 1;
static const int kSigmaX = kSigmaY + 1;
static const int kInertion = kSigmaX + 1;
static const int kBorder = kInertion + 1;
static const int kLays = kBorder + 1;
             //  next = kLays + LaysCntReserv
}; // *************************************************************
