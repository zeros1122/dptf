################################################################################
## Copyright (c) 2013 Intel Corporation All Rights Reserved
##
## Licensed under the Apache License, Version 2.0 (the "License"); you may not 
## use this file except in compliance with the License.
##
## You may obtain a copy of the License at
##     http://www.apache.org/licenses/LICENSE-2.0
##
## Unless required by applicable law or agreed to in writing, software 
## distributed under the License is distributed on an "AS IS" BASIS, WITHOUT 
## WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
## 
## See the License for the specific language governing permissions and 
## limitations under the License.
##
################################################################################

ifneq ($(filter default undefined,$(origin CC)),)
CC := gcc
endif

ESIF_SOURCES := ../..
ESIF_UF_SOURCES := $(ESIF_SOURCES)/ESIF_UF/Sources
ESIF_CM_SOURCES := $(ESIF_SOURCES)/ESIF_CM/Sources
ESIF_LF_SOURCES := $(ESIF_SOURCES)/ESIF_LF/Sources
ESIF_WS_SOURCES := $(ESIF_SOURCES)/ESIF_WS/Sources
ESIF_LIB_SOURCES := $(ESIF_SOURCES)/ESIF_LIB/Sources

LDLIBS := -lpthread -ldl -lrt 

CFLAGS += -Wno-multichar
CPPFLAGS += -I$(ESIF_UF_SOURCES) -I$(ESIF_CM_SOURCES) -I$(ESIF_LF_SOURCES) 
CPPFLAGS += -I$(ESIF_WS_SOURCES) -I$(ESIF_LIB_SOURCES)
CPPFLAGS += -DESIF_ATTR_USER 
CPPFLAGS += -DESIF_ATTR_OS_CHROME 
CPPFLAGS += -DESIF_ATTR_DAEMON 

###############################################################################
# EISF_UF 
###############################################################################

# OS Specific
OBJ := $(ESIF_UF_SOURCES)/lin/main.o
OBJ += $(ESIF_LF_SOURCES)/lin/esif_ipc_os_lin.o

# Common Source 
OBJ += $(ESIF_CM_SOURCES)/esif_debug.o
OBJ += $(ESIF_CM_SOURCES)/esif_ipc.o
OBJ += $(ESIF_CM_SOURCES)/esif_hash_table.o
OBJ += $(ESIF_CM_SOURCES)/esif_link_list.o

OBJ += $(ESIF_UF_SOURCES)/esif_uf.o
OBJ += $(ESIF_UF_SOURCES)/esif_uf_action.o
OBJ += $(ESIF_UF_SOURCES)/esif_uf_action_config.o
OBJ += $(ESIF_UF_SOURCES)/esif_uf_action_constant.o
OBJ += $(ESIF_UF_SOURCES)/esif_uf_action_system.o
OBJ += $(ESIF_UF_SOURCES)/esif_uf_actmgr.o
OBJ += $(ESIF_UF_SOURCES)/esif_uf_app.o
OBJ += $(ESIF_UF_SOURCES)/esif_uf_appmgr.o
OBJ += $(ESIF_UF_SOURCES)/esif_uf_cnjmgr.o
OBJ += $(ESIF_UF_SOURCES)/esif_uf_conjure.o
OBJ += $(ESIF_UF_SOURCES)/esif_uf_dsp.o
OBJ += $(ESIF_UF_SOURCES)/esif_uf_dspmgr.o
OBJ += $(ESIF_UF_SOURCES)/esif_uf_event.o
OBJ += $(ESIF_UF_SOURCES)/esif_uf_ipc.o
OBJ += $(ESIF_UF_SOURCES)/esif_uf_pm.o
OBJ += $(ESIF_UF_SOURCES)/esif_uf_primitive.o
OBJ += $(ESIF_UF_SOURCES)/esif_uf_service.o
OBJ += $(ESIF_UF_SOURCES)/esif_uf_shell.o
OBJ += $(ESIF_UF_SOURCES)/esif_uf_test.o

OBJ += $(ESIF_LIB_SOURCES)/esif_lib_databank.o
OBJ += $(ESIF_LIB_SOURCES)/esif_lib_datacache.o
OBJ += $(ESIF_LIB_SOURCES)/esif_lib_datavault.o
OBJ += $(ESIF_LIB_SOURCES)/esif_lib_esifdata.o
OBJ += $(ESIF_LIB_SOURCES)/esif_lib_iostream.o
OBJ += $(ESIF_LIB_SOURCES)/esif_lib_istring.o

OBJ += $(ESIF_WS_SOURCES)/esif_ws_algo.o
OBJ += $(ESIF_WS_SOURCES)/esif_ws_http.o
OBJ += $(ESIF_WS_SOURCES)/esif_ws_server.o
OBJ += $(ESIF_WS_SOURCES)/esif_ws_socket.o


###############################################################################
# BUILD 
###############################################################################

%.o: %.c $(DEPS)
	$(CC) -c $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) -o $@ $<

all: esif_ufd

esif_ufd: $(OBJ)
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(LDFLAGS) -o $@ $^ $(LDLIBS) 

clean:
	rm -f $(OBJ) esif_ufd
