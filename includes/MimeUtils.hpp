#pragma once

#include "incs.hpp"

class MimeUtils {
	public:
		static std::string getTypeOfContent(const std::string &extension);
		static std::ios_base::openmode getOpenMode(const std::string &extension);
		static bool isImage(const std::string &extension);
		static bool isVideo(const std::string &extension);
        static bool isAudio(const std::string &extension);
};
