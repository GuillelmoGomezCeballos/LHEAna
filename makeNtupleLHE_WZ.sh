#!/bin/sh

ls -l /afs/cern.ch/work/c/ceballos/public/samples/wzlhe8tev|grep lhe|awk '{print$9}' > /afs/cern.ch/work/c/ceballos/public/samples/wzlhe8tev/Files
for file in `cat /afs/cern.ch/work/c/ceballos/public/samples/wzlhe8tev/Files`
do
  root -l -q -b makeNtupleLHE_WZ.C+\(\"/afs/cern.ch/work/c/ceballos/public/samples/wzlhe8tev\",\"$file\"\);
done
hadd -f wz_all.root WZJetsTo3LNu_8TeV-madgraph_*.root;
