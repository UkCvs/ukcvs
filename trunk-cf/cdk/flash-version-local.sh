#!/bin/sh

VERSION_FILE=$1/root/.version
echo "version=1200`date +%Y%m%d%H%M`" >  $VERSION_FILE
echo "creator=dev@ukcvs" >> $VERSION_FILE
echo "imagename=Cable-Ferret-Project" >> $VERSION_FILE
echo "homepage=http://www.ukcvs.net" >> $VERSION_FILE

