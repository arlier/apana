ROOTSYS = /pmtfs/disk01/ROOT_CERN/root
#CXX = $(shell $(ROOTSYS)/bin/root-config --cxx)
CXX = /opt/rh/devtoolset-3/root/usr/bin/c++

all:
	
%:%.cc
	$(CXX) -o $@ $< -g `root-config --cflags` -I$(ROOTSYS)/include -L$(ROOTSYS)/lib -Llib -D__ROOTSHAREDLIBRARY__  -fPIC -lMinuit -lTMVA -lXMLIO -lMLP -lTreePlayer $(shell $(ROOTSYS)/bin/root-config --libs) /pmtfs/disk01/pmthome/zhaor/zhaorong/cont_v1/./pmttest_cc.so
