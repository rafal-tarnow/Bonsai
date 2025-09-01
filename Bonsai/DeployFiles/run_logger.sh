#!/bin/bash

export LD_LIBRARY_PATH=/opt/Bonsai/Bonsai_1.0.0/lib:$LD_LIBRARY_PATH

export BONSAI_LOG_CHANNEL=9

/opt/Bonsai/Bonsai_1.0.0/bin/appLogger
