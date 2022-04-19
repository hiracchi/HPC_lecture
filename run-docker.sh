#!/bin/bash

docker run --rm --init -v $PWD:/home/marp/app/ -e LANG=$LANG marpteam/marp-cli presentation-marp.md --pdf

