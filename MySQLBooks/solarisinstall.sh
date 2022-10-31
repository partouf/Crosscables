#!/bin/sh

mkdir -p /opt/mysql/include
ln -s /opt/mysql/mysql/include /opt/mysql/include/mysql
ln -s /opt/mysql/mysql/lib/libmysqlclient.so.15 /usr/lib/libmysqlclient.so.15

