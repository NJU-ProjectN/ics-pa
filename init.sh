#!/bin/bash

# pick the rc file of the user's login shell
case `basename "$SHELL"` in
  zsh)  RCFILE=${ZDOTDIR:-$HOME}/.zshrc ;;
  bash) RCFILE=~/.bashrc ;;
  *)    RCFILE= ;;
esac

# usage: addenv env_name path
function addenv() {
  if [ -z "$RCFILE" ]; then
    echo "Unsupported shell $SHELL, please add the following environment variable manually:"
    echo "  $1=`readlink -f "$2"`"
    return
  fi
  local path=`readlink -f "$2"`
  touch "$RCFILE"
  sed -i --follow-symlinks -e "/^export $1=.*/d" "$RCFILE"
  echo "export $1=\"$path\"" >> "$RCFILE"
  echo "Environment variable $1 has been added into $RCFILE."
  echo "Please run 'source $RCFILE' to let it take effect."
}

# usage: init repo branch directory trace [env]
# trace = true|false
function init() {
  if [ -d $3 ]; then
    echo "$3 is already initialized, skipping..."
    if [ $5 ] ; then
      addenv $5 $3
    fi
    return
  fi

  while [ ! -d $3 ]; do
    git clone -b $2 git@github.com:$1.git $3
  done
  log="$1 `cd $3 && git log --oneline --no-abbrev-commit -n1`"$'\n'

  if [ $4 == "true" ] ; then
    rm -rf $3/.git
    git add -A $3
    git commit -am "$1 $2 initialized"$'\n\n'"$log"
  else
    sed -i -e "/^\/$3/d" .gitignore
    echo "/$3" >> .gitignore
    git add -A .gitignore
    git commit --no-verify --allow-empty -am "$1 $2 initialized without tracing"$'\n\n'"$log"
  fi

  if [ $5 ] ; then
    addenv $5 $3
  fi
}

case $1 in
  nemu)
    init NJU-ProjectN/nemu ics2024 nemu true NEMU_HOME
    ;;
  abstract-machine)
    init NJU-ProjectN/abstract-machine ics2024 abstract-machine true AM_HOME
    init NJU-ProjectN/fceux-am ics2021 fceux-am false
    ;;
  am-kernels)
    init NJU-ProjectN/am-kernels ics2021 am-kernels false
    ;;
  nanos-lite)
    init NJU-ProjectN/nanos-lite ics2021 nanos-lite true
    ;;
  navy-apps)
    init NJU-ProjectN/navy-apps ics2024 navy-apps true NAVY_HOME
    ;;
  *)
    echo "Invalid input..."
    exit
    ;;
esac
