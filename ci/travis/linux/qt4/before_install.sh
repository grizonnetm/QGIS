###########################################################################
#    before_install.sh
#    ---------------------
#    Date                 : March 2016
#    Copyright            : (C) 2016 by Matthias Kuhn
#    Email                : matthias at opengis dot ch
###########################################################################
#                                                                         #
#   This program is free software; you can redistribute it and/or modify  #
#   it under the terms of the GNU General Public License as published by  #
#   the Free Software Foundation; either version 2 of the License, or     #
#   (at your option) any later version.                                   #
#                                                                         #
###########################################################################

export DEBIAN_FRONTEND=noninteractive
export OTB_VERSION='5.6.1'

pushd ${HOME}

curl -L https://github.com/opengisch/osgeo4travis/archive/qt4bin.tar.gz | tar -xzC /home/travis --strip-components=1
curl -L https://cmake.org/files/v3.5/cmake-3.5.0-Linux-x86_64.tar.gz | tar --strip-components=1 -zxC /home/travis/osgeo4travis

#download OTB package 5.6 for processing test
wget https://www.orfeo-toolbox.org/packages/OTB-${OTB_VERSION}-Linux64.run -O /home/travis/OTB-${OTB_VERSION}-Linux64.run && sh /home/travis/OTB-${OTB_VERSION}-Linux64.run

popd
pip install --user autopep8 nose2 pyyaml mock future termcolor
