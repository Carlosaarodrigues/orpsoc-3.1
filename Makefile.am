orpsoc_PYTHON =        \
 orpsoc/__init__.py           \
 orpsoc/build.py              \
 orpsoc/config.py             \
 orpsoc/core.py               \
 orpsoc/coremanager.py        \
 orpsoc/orpsocconfigparser.py \
 orpsoc/plusargs.py           \
 orpsoc/system.py             \
 orpsoc/utils.py              \
 orpsoc/verilog.py            \
 orpsoc/vpi.py

orpsocdir = $(pkgpythondir)

provider_PYTHON =      \
 orpsoc/provider/__init__.py  \
 orpsoc/provider/github.py    \
 orpsoc/provider/opencores.py \
 orpsoc/provider/url.py

providerdir = $(orpsocdir)/provider

simulator_PYTHON =      \
 orpsoc/simulator/__init__.py  \
 orpsoc/simulator/icarus.py    \
 orpsoc/simulator/modelsim.py  \
 orpsoc/simulator/simulator.py \
 orpsoc/simulator/verilator.py

simulatordir = $(orpsocdir)/simulator

EXTRA_DIST = INSTALL

SUBDIRS = bin
