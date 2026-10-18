#!/usr/bin/zsh

rm -rf ~/.local/include/vtl
rm -rf ~/.local/lib/vtl

cd $(dirname "$0")
cmake --install build --prefix ~/.local >/dev/null
