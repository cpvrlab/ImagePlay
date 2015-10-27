##############################################################################
#  File:      ImagePlay.pro
#  Purpose:   QMake project definition file for ImagePlay
#  Author:    Kaspar Schmid
#  Date:      December 2014
#  Copyright: Kaspar Schmid, Switzerland
#             THIS SOFTWARE IS PROVIDED FOR EDUCATIONAL PURPOSE ONLY AND
#             WITHOUT ANY WARRANTIES WHETHER EXPRESSED OR IMPLIED.
##############################################################################

TEMPLATE = subdirs
CONFIG += ordered

SUBDIRS = IPL\
          ImagePlay

ImagePlay.depends = IPL
