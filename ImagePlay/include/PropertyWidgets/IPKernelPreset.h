//#############################################################################
//  File:      IPKernelPreset.h
//  Author:    Kaspar Schmid
//  Date:      December 2014
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
