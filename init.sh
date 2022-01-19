#!/bin/bash

# usage: init repo branch [env]
function init() {
  if [ -d $1 ]; then
    echo "$1 is already initialized, skipping..."
    return
  fi

  while [ ! -d $1 ]; do
    git clone -b $2 https://github.com/NJU-ProjectN/$1.git
  done
  log="$1 `cd $1 && git log --oneline --no-abbrev-commit -n1`"$'\n'
  rm -rf $1/.git

  git add -A $1
  git commit -am "$1 $2 initialized"$'\n\n'"$log"

  if [ $3 ] ; then
    sed -i -e "/^export $3=.*/d" ~/.bashrc
    echo "export $3=`readlink -e $1`" >> ~/.bashrc

    echo "By default this script will add environment variables into ~/.bashrc."
    echo "After that, please run 'source ~/.bashrc' to let these variables take effect."
    echo "If you use shell other than bash, please add these environment variables manually."
  fi
}

# usage: init_no_git repo branch
function init_no_git() {
  if [ -d $1 ]; then
    echo "$1 is already initialized, skipping..."
    return
  fi

  while [ ! -d $1 ]; do
    git clone -b $2 https://github.com/NJU-ProjectN/$1.git
  done
  log="$1 `cd $1 && git log --oneline --no-abbrev-commit -n1`"$'\n'

  sed -i -e "/^\/$1/d" .gitignore
  echo "/$1" >> .gitignore

  git add -A .gitignore
  git commit --no-verify --allow-empty -am "$1 $2 initialized without tracing"$'\n\n'"$log"
}

case $1 in
  nemu)
    init nemu ics2022 NEMU_HOME
    ;;
  abstract-machine)
    init abstract-machine ics2022 AM_HOME
    init_no_git fceux-am ics2021
    ;;
  am-kernels)
    init_no_git am-kernels ics2021
    ;;
  nanos-lite)
    init nanos-lite ics2021
    ;;
  navy-apps)
    init navy-apps ics2021 NAVY_HOME
    ;;
  *)
    echo "Invalid input..."
    exit
    ;;
esac
