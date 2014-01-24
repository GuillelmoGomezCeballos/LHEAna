#!/bin/sh

export folder=s12-h125-vbf-ptsqmin.5-dkr;
for file in 000001-pwgevents.lhe 000002-pwgevents.lhe 000003-pwgevents.lhe 000004-pwgevents.lhe 000005-pwgevents.lhe 000006-pwgevents.lhe 000007-pwgevents.lhe 000008-pwgevents.lhe 000009-pwgevents.lhe 000010-pwgevents.lhe 000011-pwgevents.lhe 000012-pwgevents.lhe 000013-pwgevents.lhe 000014-pwgevents.lhe 000015-pwgevents.lhe 000016-pwgevents.lhe 000017-pwgevents.lhe 000018-pwgevents.lhe 000019-pwgevents.lhe 000020-pwgevents.lhe 000021-pwgevents.lhe
do
  root -l -q -b makeNtupleLHE.C+\(\"/afs/cern.ch/work/d/dkralph/powheg/production/VBF_H/$folder/unmerged/\",\"$file\"\);
done
hadd -f ${folder}_all.root *-pwgevents.root;
rm -f *-pwgevents.root;

export folder=s12-h125-vbf-ptsqmin1-dkr;
for file in 000001-pwgevents.lhe 000002-pwgevents.lhe 000003-pwgevents.lhe 000004-pwgevents.lhe 000005-pwgevents.lhe 000006-pwgevents.lhe 000007-pwgevents.lhe 000008-pwgevents.lhe 000009-pwgevents.lhe 000010-pwgevents.lhe 000011-pwgevents.lhe 000012-pwgevents.lhe 000013-pwgevents.lhe 000014-pwgevents.lhe 000015-pwgevents.lhe 000016-pwgevents.lhe 000017-pwgevents.lhe 000018-pwgevents.lhe 000019-pwgevents.lhe 000020-pwgevents.lhe 000021-pwgevents.lhe
do
  root -l -q -b makeNtupleLHE.C+\(\"/afs/cern.ch/work/d/dkralph/powheg/production/VBF_H/$folder/unmerged/\",\"$file\"\);
done
hadd -f ${folder}_all.root *-pwgevents.root;
rm -f *-pwgevents.root;

export folder=s12-h125-vbf-ptsqmin5-dkr;
for file in 000001-pwgevents.lhe 000002-pwgevents.lhe 000003-pwgevents.lhe 000004-pwgevents.lhe 000005-pwgevents.lhe 000006-pwgevents.lhe 000007-pwgevents.lhe 000008-pwgevents.lhe 000009-pwgevents.lhe 000010-pwgevents.lhe 000011-pwgevents.lhe 000012-pwgevents.lhe 000013-pwgevents.lhe 000014-pwgevents.lhe 000015-pwgevents.lhe 000016-pwgevents.lhe 000017-pwgevents.lhe 000018-pwgevents.lhe 000019-pwgevents.lhe 000020-pwgevents.lhe 000021-pwgevents.lhe
do
  root -l -q -b makeNtupleLHE.C+\(\"/afs/cern.ch/work/d/dkralph/powheg/production/VBF_H/$folder/unmerged/\",\"$file\"\);
done
hadd -f ${folder}_all.root *-pwgevents.root;
rm -f *-pwgevents.root;

export folder=s12-h125-vbf-ptsqmin15-dkr;
for file in 000001-pwgevents.lhe 000002-pwgevents.lhe 000003-pwgevents.lhe 000004-pwgevents.lhe 000005-pwgevents.lhe 000006-pwgevents.lhe 000007-pwgevents.lhe 000008-pwgevents.lhe 000009-pwgevents.lhe 000010-pwgevents.lhe 000011-pwgevents.lhe 000012-pwgevents.lhe 000013-pwgevents.lhe 000014-pwgevents.lhe 000015-pwgevents.lhe 000016-pwgevents.lhe 000017-pwgevents.lhe 000018-pwgevents.lhe 000019-pwgevents.lhe 000020-pwgevents.lhe 000021-pwgevents.lhe
do
  root -l -q -b makeNtupleLHE.C+\(\"/afs/cern.ch/work/d/dkralph/powheg/production/VBF_H/$folder/unmerged/\",\"$file\"\);
done
hadd -f ${folder}_all.root *-pwgevents.root;
rm -f *-pwgevents.root;

export folder=s12-h125-vbf-ptsqmin30-dkr;
for file in 000001-pwgevents.lhe 000002-pwgevents.lhe 000003-pwgevents.lhe 000004-pwgevents.lhe 000005-pwgevents.lhe 000006-pwgevents.lhe 000007-pwgevents.lhe 000008-pwgevents.lhe 000009-pwgevents.lhe 000010-pwgevents.lhe 000011-pwgevents.lhe 000012-pwgevents.lhe 000013-pwgevents.lhe 000014-pwgevents.lhe 000015-pwgevents.lhe 000016-pwgevents.lhe 000017-pwgevents.lhe 000018-pwgevents.lhe 000019-pwgevents.lhe 000020-pwgevents.lhe 000021-pwgevents.lhe
do
  root -l -q -b makeNtupleLHE.C+\(\"/afs/cern.ch/work/d/dkralph/powheg/production/VBF_H/$folder/unmerged/\",\"$file\"\);
done
hadd -f ${folder}_all.root *-pwgevents.root;
rm -f *-pwgevents.root;
