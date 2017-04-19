#!/usr/bin/env python

# import ROOT in batch mode
import sys
oldargv = sys.argv[:]
#sys.argv = [ '-b-' ]
import ROOT
#ROOT.gROOT.SetBatch(True)
sys.argv = oldargv

# load FWLite C++ libraries
ROOT.gSystem.Load("libFWCoreFWLite.so");
ROOT.gSystem.Load("libDataFormatsFWLite.so");
ROOT.FWLiteEnabler.enable()

# load FWlite python libraries
from DataFormats.FWLite import Handle, Events

# open file (you can use 'edmFileUtil -d /store/whatever.root' to get the physical file name)
events = Events("file:~ncsmith/public/pickedEventsILike/pick_DoubleEG_2016G_meNotGui.root")

for iev,event in enumerate(events):
    print "\nEvent %d: run %6d, lumi %4d, event %12d" % (iev,event.eventAuxiliary().run(), event.eventAuxiliary().luminosityBlock(),event.eventAuxiliary().event())
