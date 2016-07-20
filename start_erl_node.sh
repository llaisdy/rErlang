#!/bin/sh

export R_HOME=/usr/lib/R
erl -pa ebin -name node01@localhost -setcookie rerlang
