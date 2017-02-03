#!/usr/bin/env bash
set -e
export DEBIAN_FRONTEND=noninteractive

echo "-- System update..."
apt-get --assume-yes --quiet update

echo "-- Installing basic packages..."
apt-get --assume-yes --quiet install gcc g++ doxygen --install-suggests

echo "-- Installing juce dependency packages...."
apt-get --assume-yes --quiet install libfreetype6 libx11-6 libxinerama1 libxcursor1  libasound2 libglu1-mesa freeglut3 libxcomposite1 libxrandr2
apt-get --assume-yes --quiet install libfreetype6-dev libx11-dev libxinerama-dev libxcursor-dev libasound2-dev mesa-common-dev libglu1-mesa-dev freeglut3-dev libxcomposite-dev libxrandr-dev