#!/usr/bin/env bash
set -e -u -x 
curl -X POST "localhost:5000/metrics?m=10&t=15.0&l=90"
curl -X POST "localhost:5000/metrics?m=10&t=15.0&l=90"
curl -X POST "localhost:5000/metrics?m=10&t=15.0&l=91"
curl -X POST "localhost:5000/metrics?m=10&t=15.0&l=89"
curl -X POST "localhost:5000/metrics?m=105&=18.0&l=200"
curl -X POST "localhost:5000/metrics?m=105&t=18.0&l=200"
curl -X POST "localhost:5000/metrics?m=50&t=25.0&l=250"
curl -X POST "localhost:5000/metrics?m=25&t=12.0&l=225"