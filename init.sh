#!/bin/bash

function init() {
  if test -d $1; then
    echo "$1 is already initialized, exiting..."
    return
  fi

  git clone -b ics2017 https://github.com/NJU-ProjectN/$1.git
  rm -rf $1/.git

  if test $2; then
    sed -i -e "/^export $2=.*/d" ~/.bashrc
    echo "export $2=`readlink -e $1`" >> ~/.bashrc
  fi
}

read -r -p "Are you sure to initialize everything? [y/n] " input
case $input in
  [yY])
    init nemu NEMU_HOME
    init nexus-am AM_HOME
    init nanos-lite
    init navy-apps NAVY_HOME
    source ~/.bashrc

    git add -A
    git commit -am "ics2017 initialized"

    echo "Initialization finishes!"
    ;;

  [nN])
    ;;

  *)
    echo "Invalid input..."
    ;;
esac
