#pragma once

#define DS_VERSION_MAJOR 1
#define DS_VERSION_MINOR 0
#define DS_VERSION_PATCH 0
#define DS_VERSION_STRING "1.0.0"


#include "AbstractDataStructure.hpp"
#include "ArrayStructure.hpp"
#include "ArrayListStructure.hpp"
#include "LinkedListStructure.hpp"


#include "CommandRecorder.hpp"
#include "CommandData.hpp"


#include "RandomProvider.hpp"
#include "ISeedManager.hpp"
#include "SeedManager.hpp"


#include "FrameData.hpp"
#include "FrameRepository.hpp"
#include "FrameManager.hpp"
#include "FrameStore.hpp" 


#include "ReplayData.hpp"
#include "ReplayManager.hpp"
#include "ReplayEngine.hpp" 


#include "VideoConfig.hpp"
#include "ExportEvents.hpp"
#include "ExportModel.hpp"
#include "PNGWriter.hpp"
#include "VideoExporter.hpp"


#include "IDataStructure.hpp"
#include "IObservable.hpp"
#include "IImageExporter.hpp"
#include "IVideoExporter.hpp"
#include "ICommandSerializer.hpp"
#include "IVideoCommandBuilder.hpp"
#include "IProcessManager.hpp"


#include "JSONCommandSerializer.hpp"
#include "FFmpegCommandBuilder.hpp"
#include "SystemProcessManager.hpp"

namespace ds { 
    inline const char* version() { return DS_VERSION_STRING; }
    
    
    inline const char* description() { 
        return "Data Structures Visualization Library - SOLID + MVC Architecture"; 
    }
}
