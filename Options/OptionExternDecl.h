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
static const int LaysSzDnY = LaysSzUpX + 1;
static const int LaysSzDnX = LaysSzDnY + 1;

static const int AMasks = LaysSzDnX + 1;
static const int SeedRnd = AMasks + 16;
static const int LaysCnt = SeedRnd + 1;

// offset in dArr;
static const double kBorder = 0;
static const double kLays = kBorder + 1;
static const double kSigmaY = kLays + LaysCntReserv;
static const double kSigmaX = kSigmaY + 1;
static const double kInertion = kSigmaX + 1;
}; // *************************************************************
