//#############################################################################
//
//  This file is part of ImagePlay.
//
//  ImagePlay is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  ImagePlay is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with ImagePlay.  If not, see <http://www.gnu.org/licenses/>.
//
//#############################################################################

#ifndef IPKERNELPRESET_H
#define IPKERNELPRESET_H

#include <string>
#include <vector>

//-----------------------------------------------------------------------------
//! IPKernelPreset
/*!
 *
 */
struct IPKernelPreset
{
    IPKernelPreset(std::string name, int kernel[], int size)
    {
        this->name = name;
        for(int i=0; i<size; i++)
        {
            this->kernel.push_back(kernel[i]);
        }
    }

    std::string         name;
    std::vector<int>    kernel;
};

#endif // IPKERNELPRESET_H
