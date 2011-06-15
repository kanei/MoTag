#!/bin/sh
DATE=$(date +%y-%m-%d_%H-%M)
mkdir ../backup/research/$DATE
cp * ../backup/research/$DATE -r
