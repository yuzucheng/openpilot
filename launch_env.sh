#!/usr/bin/env bash

export OMP_NUM_THREADS=1
export MKL_NUM_THREADS=1
export NUMEXPR_NUM_THREADS=1
export OPENBLAS_NUM_THREADS=1
export VECLIB_MAXIMUM_THREADS=1

export ATHENA_HOST='ws://op.caminfo.cn:7776'
export API_HOST='http://op.caminfo.cn:7777'
export MAPBOX_TOKEN='pk.eyJ1IjoibWF3ZWl5dXdlaXdlaSIsImEiOiJjbGtrdjhhMHUwbWwzM3VwYzVtbXkwY2lrIn0.-u-QP8-tfrhAIpvQiNMsOw'

if [ -z "$AGNOS_VERSION" ]; then
  export AGNOS_VERSION="10.1"
fi

export STAGING_ROOT="/data/safe_staging"
