#!/bin/sh

mkdir /usr/local/mysql
mkdir /usr/local/mysql/lib
mkdir /usr/local/mysql/include
mkdir /usr/local/mysql/mysql-include
cp -R /Users/patrick/mysql-5.0.51a-osx10.4-universal/lib/* /usr/local/mysql/lib
cp -R /Users/patrick/mysql-5.0.51a-osx10.4-universal/include/* /usr/local/mysql/include
ln -s /usr/local/mysql/include /usr/local/mysql/mysql-include/mysql
