#include "../includes/MimeUtils.hpp"

std::string MimeUtils::getTypeOfContent(const std::string &extension) {
	if (isImage(extension))
		return "image";
	else if (isVideo(extension))
		return "video";
	return "text";
}

std::ios_base::openmode MimeUtils::getOpenMode(const std::string &extension) {
	std::string type = getTypeOfContent(extension);
	if (type == "image" || type == "video")
		return std::ifstream::binary;
	return std::ifstream::in;
}

bool MimeUtils::isImage(const std::string &extension) {
	return extension == "jpg" || extension == "png";
}

bool MimeUtils::isVideo(const std::string &extension) {
	return extension == "mp4";
}
