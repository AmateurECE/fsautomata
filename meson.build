###############################################################################
# NAME:             meson.build
#
# AUTHOR:           Ethan D. Twardy <ethan.twardy@gmail.com>
#
# DESCRIPTION:      Build script
#
# CREATED:          11/03/2021
#
# LAST EDITED:      12/11/2021
#
# Copyright 2021, Ethan D. Twardy
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to
# deal in the Software without restriction, including without limitation the
# rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
# sell copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
# FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
# IN THE SOFTWARE.
###

project(
  'libgobiautomata', 'c',
  version: '0.2.0',
  default_options: [
    'buildtype=debug',
    'warning_level=3',
    'werror=true',
    'c_std=c18',
  ]
)

add_project_arguments('-frecord-gcc-switches', language: 'c')
include_directories('gobiautomata')

libgobiautomata = library(
  'gobiautomata',
  'gobiautomata/mealy.c',
  install: true,
  version: meson.project_version(),
)

install_headers(
  'gobiautomata/mealy.h',
  subdir: 'gobiautomata'
)

pkgconfig = import('pkgconfig')
pkgconfig.generate(libgobiautomata, filebase: 'libgobiautomata')

executable(
  'mealy',
  'example/mealy.c',
  link_with: libgobiautomata,
)

###############################################################################
