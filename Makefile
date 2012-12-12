# Base Makefile for Ogre3D Applications
# Author: Lazaros Karydas
# 
# Description:
# Cleans files, run cmake then make and executes

#all:	remove_files do_cmake do_make execute
all:	do_make execute


remove_files:
	@echo ""
	@echo "Removing files..."
	@cd build;rm -fr CMakeFiles;echo "removed directory: 'CMakeFiles'"
	@cd build;rm -fv CMakeCache.txt cmake_install.cmake Makefile

do_cmake:
	@echo ""
	@echo "Running cmake..."
	@cd build;cmake .. > cmake.output

do_make:
	@echo "Building..."
	@cd build;make

execute:
	@echo ""
	@echo "Executing..."
	@cd build/dist/bin;./OgreApp

clean_for_svn: remove_files
	@rm -fv build/cmake.output
	@rm -fv build/dist/bin/ogre.cfg
	@rm -fv build/dist/bin/*.log
	@rm -fv build/dist/bin/OgreApp
	@rm -fv build/dist/media/*.dat