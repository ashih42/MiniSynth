#!/bin/sh

echo 'Installing dependencies...'
HOMEBREW_NO_AUTO_UPDATE=1 brew install -v sdl2 sdl2_image sdl2_ttf

echo 'Building MiniSynth...'
make
