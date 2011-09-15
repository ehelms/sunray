#!/bin/bash

rm -rf sunray-env
virtualenv --no-site-packages --distribute sunray-env
for line in $(cat requirements.txt)
do
  pip install $line -E sunray-env
done
