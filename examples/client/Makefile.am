noinst_PROGRAMS = client

client_SOURCES = \
  8_ExecutionReport.cpp \
	D_NewOrderSingle.cpp \
	AF_OrderMassStatusRequest.cpp \
	F_OrderCancelRequest.cpp \
  V_MarketDataRequest.cpp \
	W_MarketDataIncrementalRefresh.cpp \
  x_SecurityListRequest.cpp \
  y_SecurityList.cpp \
	client.cpp \
	Application.h \
	Application.cpp

client_LDADD = $(top_builddir)/src/C++/libquickfix.la 

INCLUDES = -I$(top_builddir)/include -I..

all-local:
	rm -f ../../bin/client
	ln -s ../examples/client/client ../../bin/client
