# 
# Copyright (C) 2014, 2015 Manu T S
# 
# This is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# This software is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this software.  If not, see <http://www.gnu.org/licenses/>.
# 

# Specify uhd installation path
prefix 					= /target

# programs
CC							:= gcc
CXX							:= g++
MV							:= mv -f
RM							:= rm -f

# Specify the compiler flags according to your configuration
# and requirement. You might have to add more of boost libraries
#	depending on the requirements of the program you are compiling.
# flags
COMPILER_FLAGS	=  -I$(prefix)/include/
COMPILER_FLAGS	+= -g -O2 -Wall -fPIC
LINKER_FLAGS		=  -L$(prefix)/lib
LINKER_FLAGS		+= -luhd
LINKER_FLAGS		+= -lboost_program_options-mt
LINKER_FLAGS		+= -lboost_system-mt
LINKER_FLAGS		+= -lboost_thread-mt
LINKER_FLAGS		+= -lboost_date_time-mt
LINKER_FLAGS		+= -lboost_filesystem-mt
LINKER_FLAGS		+= -lboost_regex-mt
LINKER_FLAGS		+= -lboost_serialization-mt

# add to the list below, the programs you would like to
# compile.
srcs						:=					\
	rx_samples_to_file.cpp		\
	rx_multi_samples.cpp			\
	rx_ascii_art_dft.cpp			\

objs						= $(patsubst %.cpp,	%.o, $(srcs))
exes						= $(patsubst %.o,	%.exe, $(objs))

all							: $(exes)

$(objs)					:	%.o : %.cpp
	$(CXX) $(COMPILER_FLAGS) -c $< -o $@

$(exes)					:	%.exe	: %.o
	$(CXX) $^ -o $@ $(LINKER_FLAGS)

clean						:
	$(RM) $(objs)
