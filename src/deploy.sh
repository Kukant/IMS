#!/usr/bin/env bash

cp ../doc/dokumentace.pdf .
zip 04_xkukan00_xknetl.zip main.cpp main.h Makefile dokumentace.pdf get_statistics.py
rm dokumentace.pdf
