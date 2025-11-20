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
#include "RandomProvider.hpp"
#include "ReplayEngine.hpp"
#include "RawImage.hpp"
#include "VideoConfig.hpp"
#include "ExportEvents.hpp"
#include "FrameStore.hpp"
#include "PNGWriter.hpp"
#include "VideoExporter.hpp"

namespace ds { inline const char* version() { return DS_VERSION_STRING; } }