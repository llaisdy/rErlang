#!/bin/sh

export R_HOME=/usr/lib/R
erl -pa bin -name node01@localhost -setcookie rerlang
