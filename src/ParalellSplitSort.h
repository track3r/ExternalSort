//
//  ParalellSplitSort.h
//  ExternalSort
//
//  Created by Alexandr Dem'yanenko on 22/05/14.
//  Copyright (c) 2014 Alexandr Dem'yanenko. All rights reserved.
//

#ifndef __ExternalSort__ParalellSplitSort__
#define __ExternalSort__ParalellSplitSort__

#include "MultiFileHeap.h"
#include "Io.h"

//split large input file to smaller sorted files
//returns heap object that outpust sorted values one by one
MultiFileHeap ParalellSplitSort(InputFilePtr input, std::function<TempFilePtr()> tempFactory, int numThreads, size_t workItemSize);

#endif /* defined(__ExternalSort__ParalellSplitSort__) */
