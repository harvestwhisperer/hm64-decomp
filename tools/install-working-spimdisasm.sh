#!/bin/bash

# 1.16.4 and 1.16.4 currently break linking
pip uninstall spimdisasm
pip install spimdisasm==1.16.3