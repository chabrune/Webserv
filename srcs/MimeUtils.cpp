#include "../includes/MimeUtils.hpp"

std::string MimeUtils::getTypeOfContent(const std::string &extension) {
	if (isImage(extension))
		return "image";
	else if (isVideo(extension))
		return "video";
    else if (isAudio(extension))
        return "audio";
    else if (isFont(extension))
        return "font";
    else
	    return "text";
}

std::ios_base::openmode MimeUtils::getOpenMode(const std::string &extension) {
	std::string type = getTypeOfContent(extension);
	if (type == "image" || type == "video")
		return std::ifstream::binary;
	return std::ifstream::in;
}

bool MimeUtils::isImage(const std::string &extension) {
	return extension == "jpg" || extension == "jpeg" || extension == "png" || extension == "gif" ||
        extension == "bmp" || extension == "tiff" || extension == "webp";
}

bool MimeUtils::isVideo(const std::string &extension) {
    return extension == "mp4" || extension == "avi" || extension == "mkv" ||
        extension == "mov" || extension == "wmv" || extension == "flv";
}

bool MimeUtils::isAudio(const std::string &extension) {
    return extension == "mp3" || extension == "wav" || extension == "ogg" ||
        extension == "flac" || extension == "aac" || extension == "wma";
}

bool MimeUtils::isFont(const std::string &extension) {
    return extension == "ttf" || extension == "otf" || extension == "woff" ||
        extension == "woff2" || extension == "eot";
}

bool MimeUtils::isMedia(const std::string &extension) {
	return isImage(extension) || isVideo(extension) || isAudio(extension) || isFont(extension);
}
